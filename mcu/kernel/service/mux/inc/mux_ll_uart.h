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

#ifndef __MUX_LL_UART_H__
#define __MUX_LL_UART_H__
#include <stdint.h>
#include <stdbool.h>
#include "mux_port_device.h"
#include "mux_ll_uart_configure.h"
#include "mux_ringbuffer.h"
#include "hal_gpt.h"
ATTR_TEXT_IN_FAST_MEM static __FORCE_INLINE__ uint32_t mux_get_tick_count(void)
{
    uint32_t count;
    hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M, &count);
    return count;
}

ATTR_TEXT_IN_FAST_MEM static __FORCE_INLINE__ uint32_t mux_get_tick_elapse(uint32_t start_count)
{
    uint32_t end_count;
    uint32_t duration_count;
    hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M, &end_count);
    hal_gpt_get_duration_count(start_count, end_count, &duration_count);
    return duration_count;
}

#ifdef MUX_LL_UART_SEQ_ENABLE
#define MUX_SEQ_LEN 1
#else
#define MUX_SEQ_LEN 0
#endif

#ifdef MUX_LL_UART_HEADER_CHECK_ENABLE
#define MUX_HEADER_CHECK_LEN 1
#else
#define MUX_HEADER_CHECK_LEN 0
#endif

#ifdef MUX_LL_UART_CRC_CHECK_ENABLE
#define MUX_DATA_CHECK_LEN 2
#else
#define MUX_DATA_CHECK_LEN 0
#endif

#define MUX_PACKAGE_HEAD_LEN (4 + MUX_SEQ_LEN + MUX_HEADER_CHECK_LEN)

#define MUX_PACKAGE_HEAD_TAIL_LEN (MUX_PACKAGE_HEAD_LEN + MUX_DATA_CHECK_LEN)

#define MUX_HEADER_HEAD_IDX         0
#define MUX_HEADER_ID_IDX           1
#define MUX_HEADER_LOW_LEN_IDX      2
#define MUX_HEADER_HIGH_LEN_IDX     3
#define MUX_HEADER_SEQ_IDX          4


#define foreach_node(node, head) \
        for (node = head; node; node = node->next)

#define last_node(node, head) ({\
    for (node = head; node && node->next; node = node->next);\
    node;\
})

#define foreach_array(type, item, array, len, ...) \
    for (uint32_t idx = 0; idx != len; idx++) {\
        type item = (type)array[idx];\
        __VA_ARGS__\
    }
#define foreach_index(idx, start, end) \
    for (uint32_t idx = start; idx != end; idx++)

#if 0
#define foreach_array(type, item, array, len) \
        for (uint32_t i = 0, v = 1; v; v=!v)\
            for (uint8_t item = array[0]; i < len; i++, item = array[i])
#endif

#ifdef AIR_LOW_LANTENCY_MUX_ENABLE
#define MUX_CHECK_AND_CVT_UART_PORT(port) \
({\
    mux_port_t _port;\
    if (is_mux_ll_port(port)) {\
        _port = port - MUX_LL_UART_0;\
    } else {\
        _port = port;\
    }\
    _port;\
})
#else
#define MUX_CHECK_AND_CVT_UART_PORT(port) (port)
#endif

#if 0
typedef struct mux_rx_buf {
    struct mux_rx_buf *next;
    uint32_t len;
    uint8_t data[0];
} mux_rx_buf_t;
#endif
typedef struct {
    uint8_t head;
    uint8_t id;
    uint16_t length;
#ifdef MUX_LL_UART_SEQ_ENABLE
    uint8_t seq;
#endif
#ifdef MUX_LL_UART_HEADER_CHECK_ENABLE
    uint8_t xor_check_sum;
#endif
} __attribute__((packed)) mux_ll_header_t;
/*
8
*/
typedef struct {
    //from  static table
    uint8_t scid;
    uint8_t dcid;
    union {
        uint8_t uid;
        uint8_t suid;
        uint8_t duid;
    };
    uint8_t tx_priority;
    //limiter
    uint32_t tx_pkt_len; //package length to UART VFIFO
#ifdef MUX_LL_UART_SEQ_ENABLE
    uint8_t seq;
#endif
    bool is_notify_user;
} mux_user_tag_t;

typedef struct mux_user_fifo_list {
    struct mux_user_fifo_list *next;
    mux_ringbuffer_t *txbuf;
    mux_ringbuffer_t *rxbuf;
    mux_user_tag_t *p_user_tag;
} mux_user_fifo_node_t;

/*
72+72+4*6 + 4*4*2 + 4 = 144 + 24 + 20=188
*/
typedef struct {
    mux_ringbuffer_t txbuf;
    mux_ringbuffer_t rxbuf;
    mux_port_setting_t *p_user_setting;             //  user hardware port assignment and port setting.
    uint32_t tx_buf_addr;                           //  user tx hardware buffer base address.
    uint32_t tx_buf_size;                           //  user tx hardware buffer total size.
    uint32_t rx_buf_addr;                           //  user rx hardware buffer base address.
    uint32_t rx_buf_size;
                             //  user rx hardware buffer total size.
    mux_user_fifo_node_t priority_usr_list[MAX_MUX_LL_USER_COUNT];
    mux_user_fifo_node_t *user_fifo_list_head[MUX_LL_UART_PRIORITY_LEVEL_COUNT_MAX];   //user fifo list head
} mux_ll_port_config_t;

extern mux_ll_port_config_t g_port_config;

#define MUX_MIN(a,b) ((a < b) ? a : b)


#ifdef MTK_CPU_NUMBER_0
mux_status_t mux_ll_uart_normal_init(uint8_t port_index, mux_ll_port_config_t *p_setting);
#endif
mux_status_t mux_init_ll(mux_port_t port, mux_port_setting_t *setting);
mux_status_t mux_deinit_ll(mux_port_t port);
mux_status_t mux_rx_ll(mux_handle_t handle, mux_buffer_t *buffer, uint32_t *receive_done_data_len);
mux_status_t mux_tx_ll(mux_handle_t handle, const mux_buffer_t buffers[], uint32_t buffers_counter, uint32_t *send_done_data_len);
mux_status_t mux_open_ll(mux_port_t port, const char *user_name, mux_handle_t *p_handle, mux_callback_t callback, void *user_data);
mux_status_t mux_close_ll(mux_handle_t handle);
mux_status_t mux_control_ll(mux_handle_t handle, mux_ctrl_cmd_t command, mux_ctrl_para_t *para);

#endif