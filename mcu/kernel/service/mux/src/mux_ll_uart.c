 /* Copyright Statement:
 *
 * (C) 2022  Airoha Technology Corp. All rights reserved.
 *
 * This software/firmware and related documentation ("Airoha Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to Airoha Technology Corp. ("Airoha") and/or its licensors.
 * Without the prior written permission of Airoha and/or its licensors,
 * any reproduction, modification, use or disclosure of Airoha Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 * You may only use, reproduce, modify, or distribute (as applicable) Airoha Software
 * if you have agreed to and been bound by the applicable license agreement with
 * Airoha ("License Agreement") and been granted explicit permission to do so within
 * the License Agreement ("Permitted User").  If you are not a Permitted User,
 * please cease any access or use of Airoha Software immediately.
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT AIROHA SOFTWARE RECEIVED FROM AIROHA AND/OR ITS REPRESENTATIVES
 * ARE PROVIDED TO RECEIVER ON AN "AS-IS" BASIS ONLY. AIROHA EXPRESSLY DISCLAIMS ANY AND ALL
 * WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES AIROHA PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH AIROHA SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN AIROHA SOFTWARE. AIROHA SHALL ALSO NOT BE RESPONSIBLE FOR ANY AIROHA
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND AIROHA'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO AIROHA SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT AIROHA'S OPTION, TO REVISE OR REPLACE AIROHA SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * AIROHA FOR SUCH AIROHA SOFTWARE AT ISSUE.
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>
#include <assert.h>
#include "mux.h"
#include "mux_port.h"
#include "mux_port_device.h"

#include "mux_ll_uart.h"

#include "hal_resource_assignment.h"
#include "hal_pdma_internal.h"
#include "hal_uart_internal.h"

// #define LLUART_REAL_TIME_MEASUREMENT

#define MUX_LL_UART_PKT_HEAD 0x7E
#define MUX_LL_UART_MAGIC_NUMBER 0x7EE7
#define MUX_LL_UART_HEAD(scid, dcid, type, uid) (((MUX_LL_UART_MAGIC_NUMBER << 16) | ((scid)&0xF) << 12) |  (((dcid)&0xF) << 8) |  (((type)&0xF) << 4) | ((uid)&0xF))

#define RACE_HEAD       MUX_LL_UART_HEAD(HAL_CORE_MCU , HAL_CORE_MCU , MUX_LL_UART_PACKAGE_TYPE_NORMAL, MUX_LL_UART_RACE_UID)
#define AUDIO_CMD_HEAD  MUX_LL_UART_HEAD(HAL_CORE_MCU , HAL_CORE_MCU , MUX_LL_UART_PACKAGE_TYPE_NORMAL, MUX_LL_UART_AUDIO_CMD_UID)
#define L2R_DL_HEAD     MUX_LL_UART_HEAD(HAL_CORE_DSP0, HAL_CORE_DSP0, MUX_LL_UART_PACKAGE_TYPE_NORMAL, MUX_LL_UART_L2R_DL_UID)
#define R2L_DL_HEAD     MUX_LL_UART_HEAD(HAL_CORE_DSP0, HAL_CORE_DSP0, MUX_LL_UART_PACKAGE_TYPE_NORMAL|MUX_LL_UART_USER_RX_POLLING, MUX_LL_UART_R2L_DL_UID)
#define UL_HEAD         MUX_LL_UART_HEAD(HAL_CORE_DSP0, HAL_CORE_DSP0, MUX_LL_UART_PACKAGE_TYPE_NORMAL, MUX_LL_UART_UL_UID)
#define LATCH_HEAD      MUX_LL_UART_HEAD(HAL_CORE_MCU , HAL_CORE_MCU , MUX_LL_UART_PACKAGE_TYPE_NORMAL, MUX_LL_UART_LATCH_UID)

const static  mux_ll_uart_user_configure_t g_mux_ll_uart_user_configure_table[MAX_MUX_LL_USER_COUNT] = {
/*                              name      ,  head                 , tx_buf_len                       , rx_buf_len                       , tx_buf_addr , rx_buf_addr , pkg_len                       ,tx_priority*/
[MUX_LL_UART_RACE_UID]      = {"race"     , .head = RACE_HEAD     , MUX_LL_UART_RACE_TX_BUF_SIZE     , MUX_LL_UART_RACE_RX_BUF_SIZE     , NULL        , NULL        , MUX_LL_UART_RACE_PKG_SIZE     , MUX_LL_UART_RACE_PRIORITY},
[MUX_LL_UART_AUDIO_CMD_UID] = {"audio_cmd", .head = AUDIO_CMD_HEAD, MUX_LL_UART_AUDIO_CMD_TX_BUF_SIZE, MUX_LL_UART_AUDIO_CMD_RX_BUF_SIZE, NULL        , NULL        , MUX_LL_UART_AUDIO_CMD_PKG_SIZE, MUX_LL_UART_AUDIO_CMD_PRIORITY},
// [MUX_LL_UART_L2R_DL_UID]    = {"L2R_DL"   , .head = L2R_DL_HEAD   , MUX_LL_UART_L2R_DL_TX_BUF_SIZE   , MUX_LL_UART_L2R_DL_RX_BUF_SIZE   , NULL        , NULL        , MUX_LL_UART_L2R_DL_PKG_SIZE   , MUX_LL_UART_L2R_DL_PRIORITY},
[MUX_LL_UART_R2L_DL_UID]    = {"R2L_DL"   , .head = R2L_DL_HEAD   , MUX_LL_UART_R2L_DL_TX_BUF_SIZE   , MUX_LL_UART_R2L_DL_RX_BUF_SIZE   , NULL        , NULL        , MUX_LL_UART_R2L_DL_PKG_SIZE   , MUX_LL_UART_R2L_DL_PRIORITY},
[MUX_LL_UART_UL_UID]        = {"UL"       , .head = UL_HEAD       , MUX_LL_UART_UL_TX_BUF_SIZE       , MUX_LL_UART_UL_RX_BUF_SIZE       , NULL        , NULL        , MUX_LL_UART_UL_PKG_SIZE       , MUX_LL_UART_UL_PRIORITY},
[MUX_LL_UART_LATCH_UID]     = {"latch"    , .head = LATCH_HEAD    , MUX_LL_UART_LATCH_TX_BUF_SIZE    , MUX_LL_UART_LATCH_RX_BUF_SIZE    , NULL        , NULL        , MUX_LL_UART_LATCH_PKG_SIZE    , MUX_LL_UART_LATCH_PRIORITY},
};

#ifdef CORE_CM33
/* The buffer's attribute may be changed according to the buffer's usage. */
MEM_ATTR_NONSHARED_ALIGN(4) static uint8_t mux_ll_uart_race_txbuffer[MUX_LL_UART_RACE_TX_BUF_SIZE];
MEM_ATTR_NONSHARED_ALIGN(4) static uint8_t mux_ll_uart_race_rxbuffer[MUX_LL_UART_RACE_RX_BUF_SIZE];
MEM_ATTR_NONSHARED_ALIGN(4) static uint8_t mux_ll_uart_audio_cmd_txbuffer[MUX_LL_UART_AUDIO_CMD_TX_BUF_SIZE];
MEM_ATTR_NONSHARED_ALIGN(4) static uint8_t mux_ll_uart_audio_cmd_rxbuffer[MUX_LL_UART_AUDIO_CMD_RX_BUF_SIZE];
// MEM_ATTR_SHARED_ALIGN(4)    static uint8_t mux_ll_uart_l2r_dl_txbuffer[MUX_LL_UART_L2R_DL_TX_BUF_SIZE];
// MEM_ATTR_SHARED_ALIGN(4)    static uint8_t mux_ll_uart_l2r_dl_rxbuffer[MUX_LL_UART_L2R_DL_RX_BUF_SIZE];
MEM_ATTR_SHARED_ALIGN(4)    static uint8_t mux_ll_uart_r2l_dl_txbuffer[MUX_LL_UART_R2L_DL_TX_BUF_SIZE];
MEM_ATTR_SHARED_ALIGN(4)    static uint8_t mux_ll_uart_r2l_dl_rxbuffer[MUX_LL_UART_R2L_DL_RX_BUF_SIZE];
MEM_ATTR_SHARED_ALIGN(4)    static uint8_t mux_ll_uart_ul_txbuffer[MUX_LL_UART_UL_TX_BUF_SIZE];
MEM_ATTR_SHARED_ALIGN(4)    static uint8_t mux_ll_uart_ul_rxbuffer[MUX_LL_UART_UL_RX_BUF_SIZE];
MEM_ATTR_SHARED_ALIGN(4)    static uint8_t mux_ll_uart_latch_txbuffer[MUX_LL_UART_LATCH_TX_BUF_SIZE];
MEM_ATTR_SHARED_ALIGN(4)    static uint8_t mux_ll_uart_latch_rxbuffer[MUX_LL_UART_LATCH_RX_BUF_SIZE];

static mux_ll_uart_user_buffer_configure_t mux_ll_uart_user_buffer_configure[MAX_MUX_LL_USER_COUNT] = {
    [MUX_LL_UART_RACE_UID]      = {.tx_buf_addr = mux_ll_uart_race_txbuffer       , .rx_buf_addr = mux_ll_uart_race_rxbuffer      },
    [MUX_LL_UART_AUDIO_CMD_UID] = {.tx_buf_addr = mux_ll_uart_audio_cmd_txbuffer  , .rx_buf_addr = mux_ll_uart_audio_cmd_rxbuffer },
    // [MUX_LL_UART_L2R_DL_UID]    = {.tx_buf_addr = mux_ll_uart_l2r_dl_txbuffer     , .rx_buf_addr = mux_ll_uart_l2r_dl_rxbuffer    },
    [MUX_LL_UART_R2L_DL_UID]    = {.tx_buf_addr = mux_ll_uart_r2l_dl_txbuffer     , .rx_buf_addr = mux_ll_uart_r2l_dl_rxbuffer    },
    [MUX_LL_UART_UL_UID]        = {.tx_buf_addr = mux_ll_uart_ul_txbuffer         , .rx_buf_addr = mux_ll_uart_ul_rxbuffer        },
    [MUX_LL_UART_LATCH_UID]     = {.tx_buf_addr = mux_ll_uart_latch_txbuffer      , .rx_buf_addr = mux_ll_uart_latch_rxbuffer     },
};
#endif

/*
188 + 168*4 = 188 + 672 = 860 = 35C
*/

#ifdef CORE_CM33
ATTR_ZIDATA_IN_FAST_MEM  static volatile mux_port_setting_t g_mux_port_hw_setting;
mux_ll_uart_port_configure_t mux_ll_uart_port_configure;
#endif




/*
4 + 4 + 4 + 72 + 72 + 8 + 1(4) = 24 + 144 = 168
*/

typedef struct {
    mux_callback_t callback;
    void *user_data;
} mul_ll_calback_t;

typedef struct {
    const char *user_name;
    mux_ringbuffer_t txbuf;
    mux_ringbuffer_t rxbuf;
    mux_user_tag_t *user_tag;
} mux_ll_user_config_t;

typedef struct {
    mux_user_tag_t utag;
    uint8_t pkt_seq;
} mux_ll_user_context_t;

mux_ll_user_context_t g_mux_ll_user_context[MAX_MUX_LL_USER_COUNT];

mux_ll_port_config_t g_port_config; //188
mul_ll_calback_t g_callback[MAX_MUX_LL_USER_COUNT];

typedef struct {
    uint32_t config_head;
    mux_ll_user_config_t user_config[MAX_MUX_LL_USER_COUNT]; //168 *5
    uint32_t config_tail;
} mux_ll_config_t;

volatile mux_ll_config_t * g_mux_ll_config = (volatile mux_ll_config_t *)HW_SYSRAM_PRIVATE_MEMORY_LLMUX_VAR_PORT_START;

#ifdef CORE_DSP0
static hal_core_id_t current_core_id = HAL_CORE_DSP0;
#elif defined(CORE_CM33)
static hal_core_id_t current_core_id = HAL_CORE_MCU;
#else
#error "core definition error!!"
#endif


/* define mux handle magic symble */
#define handle_to_port(handle) (handle & 0xFF)
#define handle_to_user_id(handle) ((handle >> 8) & 0xFF)
#define user_id_to_handle(user_id, port) ((MUX_LL_HANDLE_MAGIC_NUMBER << 16) | (user_id << 8) | port)

#ifdef MTK_CPU_NUMBER_0
#if 0
#define MUX_GET_SRC_CORE_ID(id)  (id&0x1)
#define MUX_GET_DEST_CORE_ID(id) ((id>>1)&0x1)
#define MUX_GET_SRC_USER_ID(id)  ((id>>2)&0x7)
#define MUX_GET_DEST_USER_ID(id) ((id>>5)&0x7)
#define MUX_ID(scid, dcid, suid, duid) \
    ((scid&0x1) | ((dcid&0x1) << 1) | ((suid&0x7) << 2) | ((duid&0x7) << 5))
#else
#define MUX_GET_DEST_CORE_ID(id) ((id)&0xF)
#define MUX_GET_DEST_USER_ID(id) ((id>>4)&0xF)
#define MUX_ID(scid, dcid, suid, duid) \
    ( ((dcid&0xF)) | ((duid&0xF) << 4))

#endif
#define CM33_CORE_ID 0x0
#define DSP_CORE_ID  0x1

ATTR_TEXT_IN_FAST_MEM static __FORCE_INLINE__ uint32_t mux_tx_vfifo(mux_ringbuffer_t *tx_vfifo, mux_ringbuffer_t *tx_ufifo, mux_user_tag_t *tag, uint32_t *min_send_len)
{
    uint8_t head_buf[MUX_PACKAGE_HEAD_LEN];
    uint8_t *pdata = head_buf;
    uint32_t user_buffer_data_len = 0;
    uint32_t vfifo_free_len;
    uint32_t tmp_pkt_len = 0;
    uint32_t real_pkt_len = 0;
    uint32_t all_send_len = 0;
#ifdef MUX_LL_UART_HEADER_CHECK_ENABLE
    uint8_t xor_check_sum = 0;
#endif
    uint16_t data_crc = 0;
    pdata = head_buf;

    user_buffer_data_len = mux_ringbuffer_data_length(tx_ufifo);
    vfifo_free_len = mux_ringbuffer_free_space(tx_vfifo);

    if (user_buffer_data_len == 0) {
        RB_LOG_D("[mux_tx_vfifo] no data in ringbuffer[0x%x],suid=%d duid=%d", 3, (uint32_t)tx_ufifo, tag->suid, tag->duid);
        return 0;
    }

    //if 4->3 ，data length in buff should larger than tx_pkt_len，or, do not send data，and tx_pkt_len must be multiple of 4
    if (tx_ufifo->flags & MUX_RB_FLAG_DATA_4TO3) {
        if (tag->tx_pkt_len > user_buffer_data_len) {
            RB_LOG_D("[mux_tx_vfifo] pkt_len=%d, but data only %d, skip", 2, tag->tx_pkt_len, user_buffer_data_len);
            return 0;
        }
    }
    tmp_pkt_len = RB_MIN(tag->tx_pkt_len, user_buffer_data_len);
    if (tmp_pkt_len == 0) {
        RB_LOG_D("[mux_tx_vfifo] User channel %d is empty!!!", 1, tag->suid);
        return 0;
    }
    tmp_pkt_len = RB_MIN(tmp_pkt_len + MUX_PACKAGE_HEAD_TAIL_LEN, vfifo_free_len);

    //TODO：set tx threshold???
#if 0
    LOG_I(common, "[%s] tmp_pkt_len=%d user_data_len=%d tx_pkt_len=%d vfifo_free_len=%d tx_ufifo->flags=%x", __func__, tmp_pkt_len, user_buffer_data_len, tag->tx_pkt_len, vfifo_free_len, tx_ufifo->flags);
    LOG_I(common,"[%s] MUX_ID=0x%x, scid=%d dcid=%d suid=%d duid=%d", \
        __func__, MUX_ID(tag->scid, tag->dcid, tag->suid, tag->duid), tag->scid, tag->dcid, tag->suid, tag->duid);
#endif
    if (tmp_pkt_len < MUX_PACKAGE_HEAD_TAIL_LEN) {
        RB_LOG_D("[mux_tx_vfifo] vfifo has no enough free space!!!", 1, tag->suid);
        return 0;
    }
    tmp_pkt_len -= MUX_PACKAGE_HEAD_TAIL_LEN;
    if (tx_ufifo->flags & MUX_RB_FLAG_DATA_4TO3) {//data 4->3, (real_pkt_len / 4 ) * 3
        real_pkt_len = tmp_pkt_len >> 2;
        real_pkt_len = real_pkt_len + (real_pkt_len << 1);
    } else {
        real_pkt_len = tmp_pkt_len;
    }
    *pdata++ = MUX_LL_UART_PKT_HEAD;//head
    *pdata++ = MUX_ID(tag->scid, tag->dcid, tag->suid, tag->duid);//TODO:what id should be included?
    *pdata++ = real_pkt_len&0xff;//tx_pkt_len low
    *pdata++ = (real_pkt_len>>8)&0xff;//tx_pkt_len high
#ifdef MUX_LL_UART_SEQ_ENABLE
    *pdata++ = tag->seq++;
     RB_LOG_D("[mux_tx_vfifo] uid=%x seq=%d", 2, tx_ufifo->uid, head_buf[4]);
#endif
#ifdef MUX_LL_UART_HEADER_CHECK_ENABLE
    foreach_array(uint8_t, item, head_buf, MUX_PACKAGE_HEAD_LEN - 1,
        xor_check_sum ^= item;
    );
    *pdata++ = xor_check_sum;
    RB_LOG_D("[mux_tx_vfifo] uid=%x check sum=0x%x", 2, tx_ufifo->uid, head_buf[5]);
    RB_LOG_D("%02X %02X %02X %02X %02X %02X ", 6, \
    head_buf[0],
    head_buf[1],
    head_buf[2],
    head_buf[3],
    head_buf[4],
    head_buf[5]
    );

#endif

#ifdef RB_WATER_MARK_ENABLE
    uint32_t total_data_len = mux_ringbuffer_data_length(tx_vfifo);
    if (total_data_len > tx_vfifo->water_mark) {
      tx_vfifo->water_mark = total_data_len;
    }

    RB_LOG_D("[mux_tx_vfifo] uid[%x] prio=%d rs_len=%d pkt_len=%d, ud_len=%d vf_len=%d uwm=%d vwm=%d", 8,\
        tx_ufifo->uid, tag->tx_priority, real_pkt_len, tag->tx_pkt_len, user_buffer_data_len, vfifo_free_len, tx_ufifo->water_mark, tx_vfifo->water_mark);
#else
    RB_LOG_D("[mux_tx_vfifo] uid[%x] prio=%d rs_len=%d pkt_len=%d, ud_len=%d vf_len=%d", 6,\
        tx_ufifo->uid, tag->tx_priority, real_pkt_len, tag->tx_pkt_len, user_buffer_data_len, vfifo_free_len);
#endif
    if (tmp_pkt_len > vfifo_free_len) {
        assert(0);//uart tx vfifo  have no enough free space
    } else {
    //TODO: do not push data if free space size less that one package size, query remaining space int vfifo
        all_send_len += mux_ringbuffer_write_st(tx_vfifo, head_buf, MUX_PACKAGE_HEAD_LEN);
        all_send_len += mux_ringbuffer_write_buffer_st(tx_ufifo, tx_vfifo, tmp_pkt_len, &data_crc);
#ifdef MUX_LL_UART_CRC_CHECK_ENABLE
        all_send_len += mux_ringbuffer_write_st(tx_vfifo, (uint8_t*)&data_crc, MUX_DATA_CHECK_LEN);
#endif
        mux_ringbuffer_write_move_hw_tail_st(tx_vfifo, real_pkt_len + MUX_PACKAGE_HEAD_TAIL_LEN);
    }
    *min_send_len = real_pkt_len;
    return all_send_len;
}
#ifdef LLUART_REAL_TIME_MEASUREMENT
extern uint32_t g_lluart_tx_cpu_time;
extern uint32_t g_lluart_tx_uart_time_end;
#endif
ATTR_TEXT_IN_FAST_MEM static __FORCE_INLINE__ void mux_tx_limiter(void)
{
    mux_ll_port_config_t *p_port_config = &g_port_config;
    mux_user_fifo_node_t * high_prio_head = p_port_config->user_fifo_list_head[1];
    mux_user_fifo_node_t * low_prio_head = p_port_config->user_fifo_list_head[2];
    mux_user_fifo_node_t * uart_latch_head = p_port_config->user_fifo_list_head[0];

    mux_user_fifo_node_t * node;
    uint32_t all_send_len = 0;
    uint32_t min_send_len = 0;
    uint32_t min_tx_pkt_len = 0xFFFFFFFF;
    mux_ringbuffer_t *tx_ufifo;
    mux_user_tag_t *tag;

    mux_ringbuffer_t *tx_vfifo = &p_port_config->txbuf;
#ifdef LLUART_REAL_TIME_MEASUREMENT
    uint32_t time_start,time_end,time_during;
    // hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M,&time_start);
    time_start = mux_get_tick_count();
#endif

    foreach_node(node, uart_latch_head) {
        tag = node->p_user_tag;
        if (mux_ringbuffer_data_length(node->txbuf) > 0) {
            RB_LOG_D("[mux_tx_limiter] Pri[%d] suid=%d tx_pkt_len=%d", 3, tag->tx_priority, tag->suid, tag->tx_pkt_len);
            all_send_len += mux_tx_vfifo(tx_vfifo, node->txbuf, tag, &min_send_len);
        }
    }

    //If someone requests latch uart, only highest fifo can send data through uart vfifo
    if (RB_CHK_FLAG(tx_vfifo, MUX_RB_FLAG_UART_LATCH)) {
        RB_LOG_W("[mux_tx_limiter] UART is in latch mode, can not send user data", 0);
        return;
    }
    // polling high priority user fifo
    foreach_node(node, high_prio_head) {
        tag = node->p_user_tag;
#ifdef MUX_LL_UART_RB_DUMP_ENABLE
        // mux_ringbuffer_hexdump(tx_ufifo, false);
#endif
        if (mux_ringbuffer_data_length(node->txbuf) > 0) {
            RB_LOG_D("[mux_tx_limiter] Pri[%d] suid=%d tx_pkt_len=%d", 3, tag->tx_priority, tag->suid, tag->tx_pkt_len);
            all_send_len += mux_tx_vfifo(tx_vfifo, node->txbuf, tag, &min_send_len);
            // min_tx_pkt_len = (min_tx_pkt_len < min_send_len) ? min_tx_pkt_len : min_send_len;
        }
    }

    // RB_LOG_I("[mux_tx_limiter] tx min_tx_pkt_len=%x min_send_len=%d all_send_len=%d", 3, (int)min_tx_pkt_len, (int)min_send_len, (int)all_send_len);

    //no data in high priority fifo
    if (all_send_len == 0) {
        foreach_node(node, low_prio_head) {
            tag = node->p_user_tag;
            if (mux_ringbuffer_data_length(node->txbuf) > 0) {
                RB_LOG_D("[mux_tx_limiter] Pri[%d] suid=%d tx_pkt_len=%d", 3, tag->tx_priority, tag->suid, tag->tx_pkt_len);
                all_send_len += mux_tx_vfifo(tx_vfifo, node->txbuf, tag, &min_send_len);
                // min_tx_pkt_len = (min_tx_pkt_len < min_send_len) ? min_tx_pkt_len : min_send_len;
            }
        }
    }

    //enable tx dma irq
    vdma_status_t status;
    status = vdma_enable_interrupt(VDMA_UART1TX);
    if (status != VDMA_OK) {
        assert(0);
        return;
    }
#ifdef LLUART_REAL_TIME_MEASUREMENT
        // hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M,&time_end);
        // hal_gpt_get_duration_count(time_start,time_end,&time_during);
        // g_lluart_tx_cpu_time+=time_during;
        // hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M,&g_lluart_tx_uart_time_end);
        g_lluart_tx_cpu_time += mux_get_tick_elapse(time_start);
        g_lluart_tx_uart_time_end = mux_get_tick_count();
#endif
    // RB_LOG_I("[mux_tx_limiter] all_send_len=%d min_tx_pkt_len=%d", 2, (int)all_send_len, (int)min_tx_pkt_len);
}

ATTR_TEXT_IN_FAST_MEM uint32_t peek_buffer_data_and_parse_first_package (
    mux_ringbuffer_t *rb, uint32_t *p_consume_len, uint32_t total_rx_left_data_size, uint8_t *duid, uint8_t *dcid)
{
    uint32_t pkt_len = 0;
    uint32_t drop_len = 0;
    uint8_t seq = 0;
    uint8_t id;
    uint8_t header[MUX_PACKAGE_HEAD_LEN];
    // uint8_t suid;
    // uint8_t scid;
    uint8_t head;
    mux_ll_header_t *p_header = (mux_ll_header_t*)&header;
    static uint32_t last_r_addr = 0;
    if (last_r_addr == 0) {
        last_r_addr = RB_START_ADDRESS(rb);
    }
#ifdef MUX_LL_UART_SEQ_ENABLE
    mux_ll_user_context_t *user_context;
#endif
#ifdef MUX_LL_UART_HEADER_CHECK_ENABLE
    uint32_t xor_check_sum = 0;
    uint8_t *pdata;
#endif
    static uint32_t fetch_pkt_count = 0;
    fetch_pkt_count++;
    *duid = 0xff;
    head = RB_FETCH_BYTE_DATA(rb, 0);

    /*max pkt len is 320 bytes, it cost at most 320 bytes time in while loop.*/
    if (head != MUX_LL_UART_PKT_HEAD) {
        RB_LOG_E("[peek first pkt] Header error start addr=0x%x val=0x%x fetch_pkt_count=%d vd_len=%d:0x%x", 5, \
            (uint32_t)RB_FETCH_BYTE_ADDRESS(rb, 0), head, fetch_pkt_count, total_rx_left_data_size, *(volatile uint32_t*)0x40090238);
        do {
            drop_len++;
            head = RB_FETCH_BYTE_DATA(rb, drop_len);
            if (drop_len >= total_rx_left_data_size) {
                break;
            }
        } while (head != MUX_LL_UART_PKT_HEAD);

        *p_consume_len = drop_len;
        RB_LOG_E("[peek first pkt] Header error end address=0x%x val=0x%x drop_len=%d", 3, \
            (uint32_t)RB_FETCH_BYTE_ADDRESS(rb, drop_len), head, drop_len);
        hexdump(RB_FETCH_BYTE_ADDRESS(rb, 0), drop_len);
        return 0;
    }

    header[MUX_HEADER_HEAD_IDX] = head;
    foreach_index(idx, 1, MUX_PACKAGE_HEAD_LEN) {
        header[idx] = RB_FETCH_BYTE_DATA(rb, idx);
    }
#ifdef MUX_LL_UART_HEADER_CHECK_ENABLE
    pdata = header;
    foreach_array(uint8_t, item, pdata, MUX_PACKAGE_HEAD_LEN - 1,
        xor_check_sum ^= item;
    );

    if (xor_check_sum != p_header->xor_check_sum) {
        *p_consume_len = MUX_PACKAGE_HEAD_LEN;
        RB_LOG_E("[peek first pkt] error!!! addr=0x%x:0x%x last_addr=0x%x Header check sum error 0x%x, drop header", 4, \
            RB_FETCH_BYTE_ADDRESS(rb, 0), *(volatile uint32_t*)0x40090234, last_r_addr, xor_check_sum);
        foreach_index(idx, 0, MUX_PACKAGE_HEAD_LEN + 16) {
           RB_LOG_E("pdata[%d] = 0x%x", 2, idx, RB_FETCH_BYTE_DATA(rb, idx));
        }
        //assert(0);
        return 0;
    }
#endif

    id = p_header->id;
    *dcid = MUX_GET_DEST_CORE_ID(id);
    *duid = MUX_GET_DEST_USER_ID(id);
    pkt_len = p_header->length;
#ifdef MUX_LL_UART_SEQ_ENABLE
    seq = p_header->seq;
#endif

    RB_LOG_D("[peek first pkt] uid[%02x] seq=%d pkt_len=%d+%d vd_len=%d addr=0x%x", 6, \
        *duid, seq, pkt_len, MUX_PACKAGE_HEAD_TAIL_LEN,total_rx_left_data_size, (uint32_t)RB_FETCH_BYTE_ADDRESS(rb, 0));
    //TODO: check hw rptr if match with sw rptr
    /*
    if (*(volatile uint32_t*)0x40090234 != ((uint32_t)RB_FETCH_BYTE_ADDRESS(rb, 0) & 0x0FFFFFFF)) {
        assert(0);
    }
    if (pkt_len > 320) {
        mux_ringbuffer_hexdump(rb, true);
        assert(0);
    }
    */
    if (total_rx_left_data_size < (pkt_len + MUX_PACKAGE_HEAD_TAIL_LEN)) {
        return 0;
    }
#ifdef MUX_LL_UART_SEQ_ENABLE
    user_context = &g_mux_ll_user_context[*duid];
    if (seq == 0) {
        user_context->pkt_seq = 0;
    }
    if (seq != user_context->pkt_seq) {
        RB_LOG_E("[peek first pkt] recv_seq=%d, expect_seq=%d", 2, seq, user_context->pkt_seq);
        assert(0);
    }
    user_context->pkt_seq++;
#endif
    last_r_addr = (uint32_t)RB_FETCH_BYTE_ADDRESS(rb, 0);
    return pkt_len; // peek real len
}

extern uint32_t g_lluart_rx_cpu_time;
ATTR_TEXT_IN_FAST_MEM static __FORCE_INLINE__ void mux_rx_deliver(void)
{
    uint32_t total_rx_left_data_size;
    uint32_t rx_pkg_len_need_to_recv;
    uint32_t write_done_len = 0;
    uint8_t duid,dcid;

#ifdef LLUART_REAL_TIME_MEASUREMENT
        uint32_t time_start,time_end,time_during;
        // hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M,&time_start);
        time_start = mux_get_tick_count();
#endif
    mux_ringbuffer_t *rx_ufifo = NULL;
    mux_ringbuffer_t *vfifo_rxrb = &g_port_config.rxbuf;
    mux_ll_user_config_t *user_config;
    mux_callback_t callback;
    void *user_data;
    uint32_t consume_len = 0;
    uint16_t data_crc = 0;

#ifdef MUX_LL_UART_CRC_CHECK_ENABLE
    uint16_t recved_data_crc = 0;
    mux_ringbuffer_t vfifo_rxrb_tmp;
#endif
    while(1) {
        total_rx_left_data_size = mux_ringbuffer_data_length(vfifo_rxrb);
#ifdef RB_WATER_MARK_ENABLE
        if (total_rx_left_data_size > vfifo_rxrb->water_mark) {
            vfifo_rxrb->water_mark = total_rx_left_data_size;
        }
#endif
        RB_LOG_D("[mux_rx_deliver] MUX_EVENT_READY_TO_READ  %d bytes data in HW FIFO=0x%x", 2, total_rx_left_data_size, (uint32_t)vfifo_rxrb);
        if (total_rx_left_data_size <= MUX_PACKAGE_HEAD_LEN )
            break;
        rx_pkg_len_need_to_recv = peek_buffer_data_and_parse_first_package(vfifo_rxrb, &consume_len, total_rx_left_data_size, &duid, &dcid);
        if (consume_len != 0) {
            mux_ringbuffer_read_move_ht_st(vfifo_rxrb, consume_len);
            mux_ringbuffer_read_move_hw_tail_ht_st(vfifo_rxrb, consume_len);//drop header
            break;
        }
        if (rx_pkg_len_need_to_recv == 0) {
            //TODO:: need remind a risk: current Rx buffer data size > Rx threshold , UART IRQ will can not clear!!!!
            break;
        }
        /* move HW FIFO SW write pointer, so, user can know how many data in HW FIFO by SW pointer. */
        /* FIXME: If user do not deal with all total_rx_left_data_size data at the following procedure,
           the data leng in vfifo_rxrb must be wrong, data length will larger than real data length.
        */
        mux_ringbuffer_write_move_ht_st(vfifo_rxrb, rx_pkg_len_need_to_recv + MUX_PACKAGE_HEAD_TAIL_LEN);
        if (duid >= MAX_MUX_LL_USER_COUNT){
            RB_LOG_E("[mux_rx_deliver]  duid=%d out of range", 1, duid);
            assert(0);
        }
#ifdef MUX_LL_UART_CRC_CHECK_ENABLE
        memcpy(&vfifo_rxrb_tmp, vfifo_rxrb, sizeof(mux_ringbuffer_t));
#endif
        user_config = &g_mux_ll_config->user_config[duid];
        //TODO: check rx_ufifo if vaild??
        rx_ufifo = &user_config->rxbuf;
#ifdef MUX_LL_UART_CRC_CHECK_ENABLE
        recved_data_crc = *(uint8_t*)RB_FETCH_BYTE_ADDRESS(vfifo_rxrb, rx_pkg_len_need_to_recv + MUX_PACKAGE_HEAD_LEN);
        recved_data_crc |= (*(uint8_t*)RB_FETCH_BYTE_ADDRESS(vfifo_rxrb, rx_pkg_len_need_to_recv + MUX_PACKAGE_HEAD_LEN + 1)) << 8;
#endif
        mux_ringbuffer_read_move_ht_st(vfifo_rxrb, MUX_PACKAGE_HEAD_LEN);//drop header
        // mux_ringbuffer_hexdump(rx_ufifo, false);
        //user fifo has not enough free sapce, just drop or assert!!!
        if(rx_pkg_len_need_to_recv > mux_ringbuffer_free_space(rx_ufifo)){
            mux_ringbuffer_hexdump(rx_ufifo, false);
            assert(0);
            // mux_ringbuffer_read_move_ht_st(vfifo_rxrb, rx_pkg_len_need_to_recv);
            // mux_ringbuffer_read_move_hw_tail_ht_st(vfifo_rxrb, rx_pkg_len_need_to_recv);
        } else {
            // receive rx_pkg_len_need_to_recv bytes data in vfifo, move SW wptr
            // mux_ringbuffer_write_move_ht_st(vfifo_rxrb, rx_pkg_len_need_to_recv);
            write_done_len = mux_ringbuffer_write_buffer_st(vfifo_rxrb, rx_ufifo, rx_pkg_len_need_to_recv, &data_crc);
            if (rx_ufifo->flags & MUX_RB_FLAG_DATA_3TO4) {
                uint32_t tmp_len = write_done_len >> 2;
                tmp_len = tmp_len + (tmp_len << 1);
                if (rx_pkg_len_need_to_recv != tmp_len) {
                // if (rx_pkg_len_need_to_recv != mux_ringbuffer_write_try(rx_ufifo, pbuf, rx_pkg_len_need_to_recv)) {
                    //user fifo have not enough free space to contain the received packet, drop the received packet
                    RB_LOG_E("[mux_rx_deliver] 3->4 copy Error!!!,write_done_len:%d,rx_pkg_len_need_to_recv:%d", 2, write_done_len, rx_pkg_len_need_to_recv);
                    mux_ringbuffer_read_move_ht_st(vfifo_rxrb, rx_pkg_len_need_to_recv);
                    mux_ringbuffer_read_move_hw_tail_ht_st(vfifo_rxrb, rx_pkg_len_need_to_recv);
                    assert(0);
                }
            } else {
                if (rx_pkg_len_need_to_recv != write_done_len) {
                // if (rx_pkg_len_need_to_recv != mux_ringbuffer_write_try(rx_ufifo, pbuf, rx_pkg_len_need_to_recv)) {
                    //user fifo have not enough free space to contain the received packet, drop the received packet
                    RB_LOG_E("[mux_rx_deliver] copy Error!!!,write_done_len:%d,rx_pkg_len_need_to_recv:%d", 2, write_done_len, rx_pkg_len_need_to_recv);
                    mux_ringbuffer_read_move_ht_st(vfifo_rxrb, rx_pkg_len_need_to_recv);
                    mux_ringbuffer_read_move_hw_tail_ht_st(vfifo_rxrb, rx_pkg_len_need_to_recv);
                    assert(0);
                }
            }
#ifdef MUX_LL_UART_CRC_CHECK_ENABLE
            if (recved_data_crc != data_crc) {
                RB_LOG_E("[mux_rx_deliver] recved_data_crc:%x,data_crc:%x", 2, recved_data_crc, data_crc);
                mux_ringbuffer_hexdump(&vfifo_rxrb_tmp, true);
                mux_ringbuffer_hexdump(vfifo_rxrb, false);
                mux_ringbuffer_hexdump(rx_ufifo, true);
                assert(0);
            }
#endif
        }
#ifdef MUX_LL_UART_CRC_CHECK_ENABLE
        mux_ringbuffer_read_move_ht_st(vfifo_rxrb, MUX_DATA_CHECK_LEN);//drop data check value
#endif
        mux_ringbuffer_read_move_hw_tail_ht_st(vfifo_rxrb, MUX_PACKAGE_HEAD_TAIL_LEN + rx_pkg_len_need_to_recv);

#ifdef RB_WATER_MARK_ENABLE
        RB_LOG_D("[mux_rx_deliver] uid[%02x] %dB rx done, uwm=%d vwm=%d", 4, duid, write_done_len, rx_ufifo->water_mark, vfifo_rxrb->water_mark);
#else
        RB_LOG_D("[mux_rx_deliver] uid[%02x] %dB rx done", 2, duid, write_done_len);
#endif
        if (g_mux_ll_user_context[duid].utag.is_notify_user) {
            //CM33 should notify DSP0 when one packet data receive done
            if (dcid == DSP_CORE_ID) {
                hal_ccni_status_t ret;
                uint32_t msg_array[3];

                /*  cm33 send ccni to dsp  */
                ret = hal_ccni_set_event(CCNI_CM4_TO_DSP0_LL_UART, (hal_ccni_message_t*)msg_array);
                if (HAL_CCNI_STATUS_OK != ret) {
                    if(ret == -3){
                        RB_LOG_E("warning: ccni busy", 0);
                    } else {
                        RB_LOG_E("cm33 ccni send error", 0);
                        RB_LOG_E("cm33 ccni send return ret = %d", 1, ret);
                    }
                } else {
                    RB_LOG_D("cm33 ccni send ok", 0);
                }
            } else {
                callback = g_callback[duid].callback;
                user_data = g_callback[duid].user_data;

                if (callback != NULL) {
                    //set need_rx_callback as flase every time before call ready to read callback
                    callback(user_id_to_handle(duid, (uint32_t)vfifo_rxrb->port_idx + MUX_LL_UART_0), MUX_EVENT_READY_TO_READ, write_done_len, user_data);
                }
            }
        }
    }
#ifdef LLUART_REAL_TIME_MEASUREMENT
        //    hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M,&time_end);
        //    hal_gpt_get_duration_count(time_start,time_end,&time_during);
        //    g_lluart_rx_cpu_time+=time_during;
           g_lluart_rx_cpu_time += mux_get_tick_elapse(time_start);
#endif
}

#ifdef LLUART_REAL_TIME_MEASUREMENT
extern uint32_t g_lluart_tx_sw_IRQ_counter;
#endif
ATTR_TEXT_IN_FAST_MEM void mux_tx_limiter_sw_irq_isr(hal_nvic_irq_t irq_number)
{
#ifdef LLUART_REAL_TIME_MEASUREMENT
    g_lluart_tx_sw_IRQ_counter++;
#endif
    mux_tx_limiter();
}

ATTR_TEXT_IN_FAST_MEM  uint32_t mux_uart_rb_get_rptr(struct mux_ringbuffer *rb)
{
    // RB_LOG_I("[mux_uart_rb_get_rptr]rb->port_idx=%d isRX=%d", 2, (uint32_t)rb->port_idx, RB_IS_RXBUF(rb));

    return uart_get_hw_rptr((uint32_t)rb->port_idx, RB_IS_RXBUF(rb));
}
ATTR_TEXT_IN_FAST_MEM  uint32_t mux_uart_rb_get_wptr(struct mux_ringbuffer *rb)
{
    // RB_LOG_I("[mux_uart_rb_get_wptr]rb->port_idx=%d isRX=%d", 2, (uint32_t)rb->port_idx, RB_IS_RXBUF(rb));
    return uart_get_hw_wptr((uint32_t)rb->port_idx, RB_IS_RXBUF(rb));
}

ATTR_TEXT_IN_FAST_MEM  void mux_uart_rb_set_tx_wptr(struct mux_ringbuffer *rb, uint32_t move_bytes)
{
#if 0
    static uint32_t send_len = 0;
    static uint32_t drop_head_len = 0;
    send_len += move_bytes;
    drop_head_len += (move_bytes - MUX_PACKAGE_HEAD_TAIL_LEN);
#endif
    uart_set_sw_move_byte((uint32_t)rb->port_idx, 0, move_bytes);
#if 0
    LOG_MSGID_I(common, "[tx_wptr]%d send_len=%d drop_head_len=%d move_bytes=%d addr=0x%x:0x%x", 6, \
        (uint32_t)rb->port_idx, send_len, drop_head_len, move_bytes, RB_WRITE_BYTE_ADDRESS(rb, 0), *(volatile uint32_t*)0x40090130);
#endif

}

ATTR_TEXT_IN_FAST_MEM  void mux_uart_rb_set_rx_rptr(struct mux_ringbuffer *rb, uint32_t move_bytes)
{
#if 0
    static uint32_t read_len = 0;
    static uint32_t drop_head_len = 0;
    read_len += move_bytes;
    drop_head_len += (move_bytes - MUX_PACKAGE_HEAD_TAIL_LEN);
#endif
    uart_set_sw_move_byte((uint32_t)rb->port_idx, 1, move_bytes);
#if 0
    LOG_MSGID_I(common, "[rx_rptr]%d read_len=%d drop_head_len=%d move_bytes=%d addr=0x%x:0x%x", 6, \
        (uint32_t)rb->port_idx, read_len, drop_head_len, move_bytes, RB_FETCH_BYTE_ADDRESS(rb, 0), *(volatile uint32_t*)0x40090234);
#endif
}

ATTR_TEXT_IN_FAST_MEM  bool mux_uart_rb_full(struct mux_ringbuffer *rb)
{
    return uart_get_buf_full_status((uint32_t)rb->port_idx, RB_IS_RXBUF(rb));
}

ATTR_TEXT_IN_FAST_MEM  bool mux_uart_rb_empty(struct mux_ringbuffer *rb)
{
    typedef bool (*uart_query_trx_empty_t)(hal_uart_port_t uart_port);
    uart_query_trx_empty_t uart_query_trx_empty[2] = {uart_query_tx_empty, uart_query_rx_empty};

    return uart_query_trx_empty[RB_TRXBUF_IDX(rb)]((uint32_t)rb->port_idx);
}

ATTR_TEXT_IN_FAST_MEM  uint32_t mux_uart_rb_data_size(struct mux_ringbuffer *rb)
{
    vdma_channel_t vdma_channel[2] = {VDMA_UART1TX, VDMA_UART1RX};
    uint32_t data_size = 0;

    vdma_get_available_receive_bytes(vdma_channel[RB_TRXBUF_IDX(rb)], &data_size);

    return data_size;
}

ATTR_TEXT_IN_FAST_MEM  uint32_t mux_uart_rb_free_space(struct mux_ringbuffer *rb)
{
    vdma_channel_t vdma_channel[2] = {VDMA_UART1TX, VDMA_UART1RX};
    uint32_t free_space = 0;

    vdma_get_available_send_space(vdma_channel[RB_TRXBUF_IDX(rb)], &free_space);

    return free_space;
}

mux_ringbuffer_ops_t mux_uart_ops = {
    .rb_get_rptr = mux_uart_rb_get_rptr,
    .rb_get_wptr = mux_uart_rb_get_wptr,
    .rb_set_tx_wptr = mux_uart_rb_set_tx_wptr,
    .rb_set_rx_rptr = mux_uart_rb_set_rx_rptr,
    .rb_full = mux_uart_rb_full,
    .rb_empty = mux_uart_rb_empty,
    .rb_data_size = mux_uart_rb_data_size,
    .rb_free_space = mux_uart_rb_free_space,
};

#ifdef CORE_CM33

static void mux_init_ll_uart_buffer(uint32_t port_index)
{
    uint32_t i;
    char rb_name[MUX_RB_MAX_NAME_LEN];
    mux_ll_user_config_t *p_user_config;
    mux_ll_port_config_t *p_port_config = &g_port_config;
    const mux_ll_uart_user_configure_t *ll_uart_config;
    mux_ll_head_t *ll_head;
    mux_user_fifo_node_t *node;
    mux_user_fifo_node_t **fifo_list_head;
    mux_user_fifo_node_t *new_fifo_node;
    mux_user_tag_t *utag;
    mux_ll_user_context_t *user_context;
    uint32_t txflags;
    uint32_t rxflags;

    #define RB_SET_UID(p_user_config, _uid)\
    do {\
        p_user_config->txbuf.uid = 0x80 | (_uid);\
        p_user_config->rxbuf.uid = _uid;\
    } while(0)
    memset(p_port_config, 0, sizeof(mux_ll_port_config_t));
    for (i = 0; i < MAX_MUX_LL_USER_COUNT; i++) {
        txflags = 0;
        rxflags = 0;
        p_user_config = &g_mux_ll_config->user_config[i];
        ll_uart_config = &g_mux_ll_uart_user_configure_table[i];
        ll_head = (mux_ll_head_t *)&ll_uart_config->head;
        user_context = &g_mux_ll_user_context[i];
        utag = &g_mux_ll_user_context[i].utag;
        utag->scid = ll_head->scid;
        utag->dcid = ll_head->dcid;
        utag->uid = ll_head->uid;
        utag->tx_priority = ll_uart_config->tx_priority;
        utag->tx_pkt_len = ll_uart_config->tx_pkt_len;
#ifdef MUX_LL_UART_SEQ_ENABLE
        utag->seq = 0;
        user_context->pkt_seq = 0;
#endif
        p_user_config->user_tag = utag;
        p_user_config->user_name = ll_uart_config->name;

        RB_SET_UID(p_user_config, ll_head->uid);
        if (ll_head->type & MUX_LL_UART_PACKAGE_TYPE_DECODE) {
            txflags = MUX_RB_FLAG_DATA_4TOH3 | MUX_RB_FLAG_TX_BUF;
            rxflags = MUX_RB_FLAG_DATA_3TOH4 | MUX_RB_FLAG_RX_BUF;
        }
        if (ll_head->type & MUX_LL_UART_USER_RX_POLLING) {
            utag->is_notify_user = false;
        } else {
            utag->is_notify_user = true;
        }
        if (ll_head->scid == HAL_CORE_DSP0) {
            txflags |= MUX_RB_FLAG_SHARED_BUFFER;
        }
        if (ll_head->dcid == HAL_CORE_DSP0) {
            rxflags |= MUX_RB_FLAG_SHARED_BUFFER;
        }

        p_user_config->txbuf.port_idx = port_index;
        p_user_config->rxbuf.port_idx = port_index;
        snprintf(rb_name, sizeof(rb_name), "%s_TX", ll_uart_config->name);
        mux_ringbuffer_init(&p_user_config->txbuf, mux_ll_uart_user_buffer_configure[i].tx_buf_addr, ll_uart_config->tx_buf_len, txflags, rb_name);
        snprintf(rb_name, sizeof(rb_name), "%s_RX", ll_uart_config->name);
        mux_ringbuffer_init(&p_user_config->rxbuf, mux_ll_uart_user_buffer_configure[i].rx_buf_addr, ll_uart_config->rx_buf_len, rxflags, rb_name);

        //add user fifo to list from tail according to priority
        fifo_list_head = &p_port_config->user_fifo_list_head[ll_uart_config->tx_priority];//priority 0 is reserved now
        new_fifo_node = &p_port_config->priority_usr_list[i];
        if (new_fifo_node == NULL) {
            assert(0);
        }

        if (*fifo_list_head == NULL) {
            // RB_LOG_I("[mux_init_ll_uart_buffer] first new_node=0x%x, usr_count=%d!!", 2, (uint32_t)new_fifo_node, i);
            *fifo_list_head = new_fifo_node;
        } else {
            last_node(node, *fifo_list_head);
            node->next = new_fifo_node;
            // RB_LOG_I("[mux_init_ll_uart_buffer] not first node=0x%x new_node=0x%x, usr_count=%d!!", 3, (uint32_t)node, (uint32_t)new_fifo_node, i);
        }

        new_fifo_node->txbuf = &p_user_config->txbuf;
        new_fifo_node->rxbuf = &p_user_config->rxbuf;
        new_fifo_node->p_user_tag = p_user_config->user_tag;
        new_fifo_node->next = NULL;

        // RB_LOG_I("[mux_init_ll_uart_buffer] node=0x%08x", 1, (uint32_t)new_fifo_node);
    }
    mux_user_tag_t *tag;
    foreach_node(node, *fifo_list_head) {
        tag = node->p_user_tag;
        LOG_I(common, "[mux_init_ll_uart_buffer] node=0x%x txbuf=%s rxbuf=%s uid=%d priority=%d tx_pkt=%d", \
            (uint32_t)node, node->txbuf->name, node->rxbuf->name, tag->uid, tag->tx_priority, tag->tx_pkt_len);
    }
    mux_ll_uart_port_configure.uart_tx_threshold = 30;//RB_MIN(RB_MIN(MUX_LL_UART_DL_PKG_SIZE, MUX_LL_UART_UL_PKG_SIZE), MUX_LL_UART_AUDIO_CMD_PKG_SIZE) - 1;
    mux_ll_uart_port_configure.uart_rx_threshold = 324;//(MUX_LL_UART_DL_PKG_SIZE + MUX_LL_UART_UL_PKG_SIZE + MUX_LL_UART_AUDIO_CMD_PKG_SIZE) - 1;
    // mux_ll_uart_port_configure.uart_rx_threshold = (MUX_LL_UART_DL_PKG_SIZE + MUX_LL_UART_UL_PKG_SIZE) - 1;
}

#endif


ATTR_TEXT_IN_FAST_MEM  void user_uart_dma_callback(hal_uart_callback_event_t status, void *user_data)
{
    switch (status) {
    case HAL_UART_EVENT_READY_TO_WRITE:
        RB_LOG_D("[udcb]: MUX_EVENT_READY_TO_WRITE", 0);
        //set need_rx_callback as flase every time before call ready to read callback
        // call user callback for every user??
        // user_config->callback(user_id_to_handle(duid, 1), HAL_UART_EVENT_READY_TO_WRITE, write_done_len, user_config->user_data);
        /*
            1. Take a packet of data from each High Priority User FIFO and put it into the UART TX VFIFO
            2. Notify user when FIFO is empty
            3. Tx threshold is set to the smallest package size
            4. If there are three high priority FIFOS, and if there is no data in one or two FIFOS, can this send a packet of data from the low Priority FIFO  ？
               or only when all data in high priority fifo is sent done, the data in low priority can be sent.
            5. Tx_pkt_len is related to the sending frequency and the size of the whole packet sent?  What kind of relationship should be specific?  The driver eats this internally
        */
        hal_nvic_set_pending_irq(SW_IRQn);
        break;
    case HAL_UART_EVENT_READY_TO_READ:
        RB_LOG_D("[udcb]: HAL_UART_EVENT_READY_TO_READ", 0);
        mux_rx_deliver();
        break;
    case HAL_UART_EVENT_TRANSMISSION_DONE:
        RB_LOG_I("[udcb]: HAL_UART_EVENT_TRANSMISSION_DONE", 0);
        break;
    case HAL_UART_EVENT_WAKEUP_SLEEP:
        RB_LOG_I("[udcb]: HAL_UART_EVENT_WAKEUP_SLEEP", 0);
        break;
#ifdef HAL_UART_FEATURE_FLOWCONTROL_CALLBACK
    case HAL_UART_EVENT_SW_FLOW_CTRL:
        RB_LOG_I("[udcb]: HAL_UART_EVENT_SW_FLOW_CTRL", 0);
        break;
#endif
    default:
        RB_LOG_E("[udcb]: unsupport event:%d", 1, status);

    }
}

ATTR_TEXT_IN_FAST_MEM  void temp_uart_enable_tx_hw_fifo_empty_irq(hal_uart_port_t UART_PORT)
{
    extern UART_REGISTER_T *const g_uart_regbase[];
    UART_REGISTER_T *uartx;
    uartx = g_uart_regbase[UART_PORT];
    // uart_unmask_send_interrupt(uartx);
    uart_mask_send_interrupt(uartx);
    // uart_mask_receive_interrupt(uartx);//timeout mask
}
ATTR_TEXT_IN_FAST_MEM void temp_uart_enable_rx_dma_irq(hal_uart_port_t UART_PORT)
{
    vdma_status_t status;
    extern vdma_channel_t  uart_port_to_dma_map[2][3];
    extern vdma_status_t vdma_enable_interrupt(vdma_channel_t channel);
    #define  uart_port_to_dma_channel(uart_port,is_rx)   (uart_port_to_dma_map[is_rx][uart_port])

    vdma_channel_t channel;
    channel = uart_port_to_dma_channel(UART_PORT, 1);
    status  = vdma_enable_interrupt(channel);
    if (status != VDMA_OK) {
        assert(0);
    }

}

mux_status_t mux_ll_uart_normal_init(uint8_t port_index, mux_ll_port_config_t *p_setting)
{
    hal_uart_config_t uart_config;
    hal_uart_dma_config_t dma_config;
    hal_uart_status_t sta;

    IRQn_Type uart_dma_irq[] = {UART_DMA0_IRQn, UART_DMA1_IRQn, UART_DMA2_IRQn};
    IRQn_Type uart_irq[] = {UART0_IRQn, UART1_IRQn, UART2_IRQn};

    uart_config.baudrate     = p_setting->p_user_setting->dev_setting.uart.uart_config.baudrate;
    uart_config.parity       = p_setting->p_user_setting->dev_setting.uart.uart_config.parity;
    uart_config.stop_bit     = p_setting->p_user_setting->dev_setting.uart.uart_config.stop_bit;
    uart_config.word_length  = p_setting->p_user_setting->dev_setting.uart.uart_config.word_length;
    RB_LOG_I("baudrate=%d, parity=%d, stop_bit=%d, word_length=%dtx_buf_addr=0x%x rx_buf_addr=0x%x", 6,\
        uart_config.baudrate   ,uart_config.parity     ,uart_config.stop_bit   ,uart_config.word_length, p_setting->tx_buf_addr, p_setting->rx_buf_addr);

    dma_config.send_vfifo_buffer      = (uint8_t *)(uint32_t *)p_setting->tx_buf_addr;
    dma_config.send_vfifo_buffer_size = p_setting->tx_buf_size;
    dma_config.receive_vfifo_buffer   = (uint8_t *)(uint32_t *)p_setting->rx_buf_addr;
    dma_config.receive_vfifo_buffer_size = p_setting->rx_buf_size;
    dma_config.send_vfifo_threshold_size = mux_ll_uart_port_configure.uart_tx_threshold;
    dma_config.receive_vfifo_threshold_size   = mux_ll_uart_port_configure.uart_rx_threshold;
    dma_config.receive_vfifo_alert_size       = 12;

    hal_uart_deinit(port_index);

    RB_LOG_I("mux_ll_uart_normal_init hal_uart_init port_index= %d", 1, port_index);
    if (HAL_UART_STATUS_OK != (sta = hal_uart_init(port_index, &uart_config))) {
        RB_LOG_E("mux_ll_uart_normal_init hal_uart_init fail, sta = %d", 1, sta);
        return MUX_STATUS_ERROR_INIT_FAIL;
    }

    if (HAL_UART_STATUS_OK != (sta = hal_uart_set_dma(port_index, &dma_config))) {
        RB_LOG_E("mux_ll_uart_normal_init hal_uart_set_dma fail, sta = %d", 1, sta);
        return MUX_STATUS_ERROR_INIT_FAIL;
    }

    if (p_setting->p_user_setting->dev_setting.uart.flowcontrol_type == MUX_UART_NONE_FLOWCONTROL) {
        hal_uart_disable_flowcontrol(port_index);
    } else if (p_setting->p_user_setting->dev_setting.uart.flowcontrol_type == MUX_UART_SW_FLOWCONTROL) {
        hal_uart_set_software_flowcontrol(port_index, 0x11, 0x13, 0x77);
    } else if (p_setting->p_user_setting->dev_setting.uart.flowcontrol_type == MUX_UART_HW_FLOWCONTROL) {
        hal_uart_set_hardware_flowcontrol(port_index);
    } else {
        assert(0);
    }

    if (HAL_UART_STATUS_OK != (sta = hal_uart_register_callback(port_index, user_uart_dma_callback, (void *)(intptr_t)port_index))) {
        RB_LOG_E("mux_ll_uart_normal_init hal_uart_register_callback fail, sta = %d", 1, sta);
        return MUX_STATUS_ERROR_INIT_FAIL;
    }

    temp_uart_enable_tx_hw_fifo_empty_irq(port_index);
    temp_uart_enable_rx_dma_irq(port_index);

    NVIC_SetPriority(SW_IRQn, DEFAULT_IRQ_PRIORITY + 1);
    NVIC_SetPriority(uart_dma_irq[port_index], DEFAULT_IRQ_PRIORITY + 2);
    NVIC_SetPriority(uart_irq[port_index], DEFAULT_IRQ_PRIORITY + 2);

    hal_nvic_register_isr_handler(SW_IRQn, mux_tx_limiter_sw_irq_isr);
    hal_nvic_enable_irq(SW_IRQn);

    return MUX_STATUS_OK;
}

mux_status_t mux_init_ll(mux_port_t port, mux_port_setting_t *setting)
{
    uint32_t port_index;
    mux_status_t sta;
    mux_port_t tmp_port = port;
    mux_ll_port_config_t *mux_ll_port_config;
    char rb_name[MUX_RB_MAX_NAME_LEN];

    port_index = MUX_CHECK_AND_CVT_UART_PORT(port);

    mux_init_ll_uart_buffer(port_index);

    RB_LOG_I("[mux_init_ll] mux_init port=%d for low latency uart, sizeof(mux_ll_config_t)=%d g_mux_ll_config=0x%x", 3, port, sizeof(mux_ll_config_t), (uint32_t)g_mux_ll_config);

    memcpy((void *)&g_mux_port_hw_setting, (void *)setting, sizeof(mux_port_setting_t));

    mux_ll_port_config = &g_port_config;
    mux_ll_port_config->p_user_setting = (mux_port_setting_t *)&g_mux_port_hw_setting;

    mux_ll_port_config->tx_buf_size = setting->tx_buffer_size;
    if (setting->tx_buffer == 0) {
        mux_ll_port_config->tx_buf_addr = (uint32_t)pvPortMallocNC(mux_ll_port_config->tx_buf_size);
    } else {
        mux_ll_port_config->tx_buf_addr = setting->tx_buffer;
    }
    mux_ll_port_config->rx_buf_size = setting->rx_buffer_size;
    if (setting->rx_buffer == 0) {
        mux_ll_port_config->rx_buf_addr = (uint32_t)pvPortMallocNC(mux_ll_port_config->rx_buf_size);
    } else {
        mux_ll_port_config->rx_buf_addr = setting->rx_buffer;
    }
    /*This is used to check if the shared memory, overwritten by someone else.*/
    g_mux_ll_config->config_head = 0x5A5A5A5A;
    g_mux_ll_config->config_tail = 0xA5A5A5A5;

    if (is_mux_ll_port(tmp_port)) {
        RB_LOG_I("[mux_init_ll] port=%d, tx_buffer=0x%x tx_buffer_size=%d rx_buffer=0x%x rx_buffer_size=%d", 5, tmp_port, setting->tx_buffer, setting->tx_buffer_size, setting->rx_buffer, setting->rx_buffer_size);
        snprintf(rb_name, sizeof(rb_name), "HW_P%d_TX", port);
        mux_ringbuffer_init(&mux_ll_port_config->txbuf, (uint8_t*)mux_ll_port_config->tx_buf_addr, mux_ll_port_config->tx_buf_size, MUX_RB_FLAG_TX_BUF, rb_name);


        mux_ll_port_config->txbuf.uid = 0xFF;
        snprintf(rb_name, sizeof(rb_name), "HW_P%d_RX", port);
        mux_ringbuffer_init(&mux_ll_port_config->rxbuf, (uint8_t*)mux_ll_port_config->rx_buf_addr, mux_ll_port_config->rx_buf_size, MUX_RB_FLAG_RX_BUF, rb_name);
        mux_ll_port_config->rxbuf.uid = 0x7F;
    } else {
        RB_LOG_I("[mux_init_ll] port=%d not ll port, setting->tx_buffer=0x%x setting->rx_buffer=0x%x", 3, tmp_port, setting->tx_buffer, setting->rx_buffer);
    }
    mux_ll_port_config->txbuf.port_idx = port_index;
    mux_ll_port_config->rxbuf.port_idx = port_index;

    mux_ll_port_config->txbuf.ops = &mux_uart_ops;
    mux_ll_port_config->rxbuf.ops = &mux_uart_ops;

    /* logging device post initialization */
    if (MUX_STATUS_OK != (sta = mux_ll_uart_normal_init(port_index, mux_ll_port_config))) {
        RB_LOG_E("[mux_init_ll] port=%d sta=0x%x", 2, tmp_port, sta);
        return MUX_STATUS_ERROR_INIT_FAIL;
    }

    return MUX_STATUS_OK;
}
mux_status_t mux_deinit_ll(mux_port_t port)
{
    return MUX_STATUS_OK;
}

#endif



#if 0
mux_status_t mux_user_control(mux_handle_t handle, mux_ctrl_cmd_t command, mux_ctrl_para_t *para)
{
    mux_user_config_t *p_user_config;
    mux_handle_t mux_handle;

    mux_handle = handle;
    if (handle_is_valid(mux_handle) == false) {
        return MUX_STATUS_ERROR_PARAMETER;
    }

    p_user_config = &g_mux_ll_config->user_config[handle_to_user_id(mux_handle)];
    switch(command) {
    case MUX_CMD_SET_USER_TAG:
        //TODO:irq is comming when writing, to read this value?
        memcpy(p_user_config->user_tag, &para->mux_user_tag, sizeof(mux_user_tag_t));
        break;
    case MUX_CMD_GET_USER_TAG:
        memcpy(&para->mux_user_tag, p_user_config->user_tag, sizeof(mux_user_tag_t));
        break;
    }
    return MUX_STATUS_OK;
}

#endif

mux_status_t mux_open_ll(mux_port_t port, const char *user_name, mux_handle_t *p_handle, mux_callback_t callback, void *user_data)
{
    uint32_t per_cpu_irq_mask;
    uint32_t uid;
    uint32_t i;
    uint32_t ll_user_count;

    const mux_ll_uart_user_configure_t *ll_uart_config = NULL;
    mux_ll_head_t *ll_head;

    ll_user_count = sizeof(g_mux_ll_uart_user_configure_table) / sizeof(g_mux_ll_uart_user_configure_table[0]);

    for (i = 0; i < ll_user_count; i++) {
        if (!strcmp(g_mux_ll_uart_user_configure_table[i].name, user_name)) {
            ll_uart_config = &g_mux_ll_uart_user_configure_table[i];
            break;
        }
    }

    if (!ll_uart_config) {
        LOG_E(common, "[mux_open_ll] port=%d, user:%s not found", port, user_name);
        return MUX_STATUS_ERROR;
    }
    ll_head = (mux_ll_head_t*)&ll_uart_config->head;
    *p_handle = user_id_to_handle(ll_head->uid, port);

    port = MUX_CHECK_AND_CVT_UART_PORT(port);
    LOG_I(common, "[mux_open_ll] name=%s port=%d, head=0x%x *p_handle=0x%x, uid=%d scid=%d dcid=%d g_mux_ll_config=0x%x type=0x%x", \
    ll_uart_config->name, port, ll_uart_config->head, *p_handle, ll_head->uid, ll_head->scid, ll_head->dcid, (uint32_t)g_mux_ll_config, ll_head->type);

    port_mux_cross_local_enter_critical(&per_cpu_irq_mask);
    uid = ll_head->uid;
    g_callback[uid].callback = callback;
    g_callback[uid].user_data = user_data;
    port_mux_cross_local_exit_critical(per_cpu_irq_mask);

    return MUX_STATUS_OK;
}


mux_status_t mux_close_ll(mux_handle_t handle)
{
    uint32_t uid;
    uint32_t per_cpu_irq_mask;

    uid = handle_to_user_id(handle);
    port_mux_cross_local_enter_critical(&per_cpu_irq_mask);
    g_callback[uid].callback = NULL;
    g_callback[uid].user_data = NULL;
    port_mux_cross_local_exit_critical(per_cpu_irq_mask);

    return MUX_STATUS_OK;
}

void mux_reset_user_fifo(mux_handle_t handle)
{
    uint32_t uid;
    mux_ll_user_config_t *user_config;

    uid = handle_to_user_id(handle);

    user_config = &g_mux_ll_config->user_config[uid];
    mux_ringbuffer_reset(&user_config->rxbuf);
}

ATTR_TEXT_IN_FAST_MEM mux_status_t mux_rx_ll(mux_handle_t handle, mux_buffer_t *buffer, uint32_t *receive_done_data_len)
{
    uint32_t uid;
    mux_ll_user_config_t *user_config;

    uid = handle_to_user_id(handle);

    user_config = &g_mux_ll_config->user_config[uid];

    *receive_done_data_len = mux_ringbuffer_read(&user_config->rxbuf, buffer->p_buf, buffer->buf_size);

    RB_LOG_D("[mux_rx_ll] [%02x] rxbuf[%s] expect size=%d real size=%d", 3, user_config->rxbuf.uid, buffer->buf_size, *receive_done_data_len);
    return MUX_STATUS_OK;
}


ATTR_TEXT_IN_FAST_MEM mux_status_t mux_tx_ll(mux_handle_t handle, const mux_buffer_t buffers[], uint32_t buffers_counter, uint32_t *send_done_data_len)
{
    mux_ringbuffer_t *txbuf;
    uint32_t send_len = 0;
    mux_ll_user_config_t *user_config;
    uint32_t uid;
    uint32_t i;
#ifdef MUX_LL_UART_RB_DUMP_ENABLE
    uint32_t expected_total_size = 0;
#endif
    const mux_buffer_t *pbuf;
    uid = handle_to_user_id(handle);

    user_config = &g_mux_ll_config->user_config[uid];

    txbuf = &user_config->txbuf;

    for (i = 0; i < buffers_counter; i++) {
        pbuf = &buffers[i];
        send_len += mux_ringbuffer_write(txbuf, pbuf->p_buf, pbuf->buf_size);
#ifdef MUX_LL_UART_RB_DUMP_ENABLE
        expected_total_size += pbuf->buf_size;
#endif
    }
#ifdef MUX_LL_UART_RB_DUMP_ENABLE
        if (send_len != expected_total_size) {
            mux_ringbuffer_hexdump(txbuf, false);
        }
#endif
    *send_done_data_len = send_len;
    RB_LOG_D("[mux_tx_ll] uid=%02x send_len=%d", 2, txbuf->uid, send_len);
#ifdef CORE_CM33
    //TODO: Finding vFIFO empty, pull SW IRQ trigger to send MUX TX
    hal_nvic_set_pending_irq(SW_IRQn);
#endif

#ifdef CORE_DSP0
    hal_ccni_status_t ret;
    uint32_t msg_array[3];

    ret = hal_ccni_set_event(CCNI_DSP0_TO_CM4_LL_UART, (hal_ccni_message_t*)msg_array);
    if (HAL_CCNI_STATUS_OK != ret) {
        RB_LOG_D("dsp ccni send error, return ret = %d", 1, ret);
    } else {
        RB_LOG_D("dsp ccni send ok", 0);
    }

#endif
    return MUX_STATUS_OK;
}


#ifdef CORE_DSP0

void mux_ll_uart_rx_event_from_mcu_handler(hal_ccni_event_t event, void * msg)
{
    hal_ccni_status_t status;
    uint32_t i;
    uint32_t data_len;
    uint8_t uids[] = {MUX_LL_UART_L2R_DL_UID, MUX_LL_UART_R2L_DL_UID, MUX_LL_UART_UL_UID};
    uint8_t uid;
    uint32_t uid_num = sizeof(uids)/sizeof(uids[0]);
    mux_callback_t callback;
    void *user_data;
    static uint32_t dsp_receive_ccni_count = 0;

    mux_ll_user_config_t *ll_user_config;
    status = hal_ccni_mask_event(event);
    if(status != HAL_CCNI_STATUS_OK){
        RB_LOG_E("MCU CCNI mask event: 0x%x something wrong, return is %d", 2, event, status);
    }

    dsp_receive_ccni_count++;
    RB_LOG_D("DSP receive ccni rx event count = %d\r\n", 1, dsp_receive_ccni_count);

    status = hal_ccni_clear_event(event);  // clear the event.
    status = hal_ccni_unmask_event(event); // unmask the event.

    //FIXME: There may be a dummy notice
    for (i = 0; i < uid_num; i++) {
        uid = uids[i];
        ll_user_config = &g_mux_ll_config->user_config[uid];
        callback = g_callback[uid].callback;
        user_data = g_callback[uid].user_data;
        data_len = mux_ringbuffer_data_length(&ll_user_config->rxbuf);
        if ((data_len > 0) && callback) {
            callback(user_id_to_handle(uid, (uint32_t)ll_user_config->rxbuf.port_idx + MUX_LL_UART_0), MUX_EVENT_READY_TO_READ, data_len, user_data);
        }
    }
}
#endif


#ifdef CORE_CM33
ATTR_TEXT_IN_FAST_MEM void mux_ll_uart_tx_event_from_dsp_handler(hal_ccni_event_t event, void * msg)
{
    hal_ccni_status_t status;

    status = hal_ccni_mask_event(event);
    if(status != HAL_CCNI_STATUS_OK){
        RB_LOG_E("MCU CCNI mask event: 0x%x something wrong, return is %d", 2, event, status);
    }
    RB_LOG_D("MCU receive ccni tx event ", 0);
    status = hal_ccni_clear_event(event);  // clear the event.
    status = hal_ccni_unmask_event(event); // unmask the event.
    hal_nvic_set_pending_irq(SW_IRQn);
}
#endif


mux_status_t mux_control_ll(mux_handle_t handle, mux_ctrl_cmd_t command, mux_ctrl_para_t *para)
{
    uint32_t uid;
    mux_ll_user_config_t *user_config;
    uid = handle_to_user_id(handle);

    user_config = &g_mux_ll_config->user_config[uid];
    switch(command) {
    case MUX_CMD_GET_LL_USER_RX_BUFFER_DATA_SIZE:
        para->mux_ll_user_rx_data_len = mux_ringbuffer_data_length(&user_config->rxbuf);
        break;
    case MUX_CMD_GET_LL_USER_TX_BUFFER_FREE_SIZE:
        para->mux_ll_user_tx_free_len = mux_ringbuffer_free_space(&user_config->txbuf);
        break;
    case MUX_CMD_SET_LL_USER_TX_PKT_LEN:
        assert(user_config->user_tag != NULL);

        user_config->user_tag->tx_pkt_len = para->mux_ll_user_tx_pkt_len;
        break;
    default:
        break;
    }
    return MUX_STATUS_OK;
}

