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

#ifndef __MUX_LL_UART_CONFIGURE_H__
#define __MUX_LL_UART_CONFIGURE_H__
#include <stdint.h>
#include "hal_core_status.h"

typedef struct {
    uint8_t uid:4;
    uint8_t type:4;
    uint8_t dcid:4;
    uint8_t scid:4;
    uint16_t magic_number;
} mux_ll_head_t;

typedef struct {
    const char *name;

    uint32_t head;

    uint16_t tx_buf_len;
    uint16_t rx_buf_len;
    uint8_t *tx_buf_addr;
    uint8_t *rx_buf_addr;

    uint16_t tx_pkt_len;

    uint8_t tx_priority;
} mux_ll_uart_user_configure_t;

typedef struct {
    uint8_t *tx_buf_addr;
    uint8_t *rx_buf_addr;
} mux_ll_uart_user_buffer_configure_t;

typedef struct {
    uint32_t uart_tx_threshold;
    uint32_t uart_rx_threshold;
} mux_ll_uart_port_configure_t;

#define MUX_LL_UART_PACKAGE_TYPE_NORMAL 0x1
#define MUX_LL_UART_PACKAGE_TYPE_DECODE 0x2
#define MUX_LL_UART_USER_RX_POLLING     0x4

#define MUX_LL_UART_SEQ_ENABLE
#define MUX_LL_UART_HEADER_CHECK_ENABLE
// #define MUX_LL_UART_DEBUG_LOG_ENABLE
#define MUX_LL_UART_CRC_CHECK_ENABLE

#ifdef MUX_LL_UART_DEBUG_LOG_ENABLE
// #define MUX_LL_UART_RB_DUMP_ENABLE
#endif




#define MUX_LL_UART_PRIORITY_LEVEL_COUNT_MAX 3

#define MUX_LL_UART_DL_PKG_SIZE 320 //240

#define MUX_LL_UART_RACE_PKG_SIZE           30
#define MUX_LL_UART_RACE_PRIORITY           2
#define MUX_LL_UART_RACE_UID                0

#define MUX_LL_UART_AUDIO_CMD_PKG_SIZE      30
#define MUX_LL_UART_AUDIO_CMD_PRIORITY      1
#define MUX_LL_UART_AUDIO_CMD_UID           (MUX_LL_UART_RACE_UID + 1)

#define MUX_LL_UART_L2R_DL_PKG_SIZE         MUX_LL_UART_DL_PKG_SIZE
#define MUX_LL_UART_L2R_DL_PRIORITY         1
#define MUX_LL_UART_L2R_DL_UID              (MUX_LL_UART_AUDIO_CMD_UID + 1)

#define MUX_LL_UART_R2L_DL_PKG_SIZE         MUX_LL_UART_DL_PKG_SIZE
#define MUX_LL_UART_R2L_DL_PRIORITY         1
#define MUX_LL_UART_R2L_DL_UID              (MUX_LL_UART_AUDIO_CMD_UID + 1)

#define MUX_LL_UART_UL_PKG_SIZE             54
#define MUX_LL_UART_UL_PRIORITY             1
#define MUX_LL_UART_UL_UID                  (MUX_LL_UART_R2L_DL_UID + 1)

#define MUX_LL_UART_LATCH_PKG_SIZE          30
#define MUX_LL_UART_LATCH_PRIORITY          0
#define MUX_LL_UART_LATCH_UID               (MUX_LL_UART_UL_UID + 1)

#define MAX_MUX_LL_USER_COUNT               MUX_LL_UART_LATCH_UID + 1


#define MUX_LL_UART_RACE_TX_BUF_SIZE        1024
#define MUX_LL_UART_RACE_RX_BUF_SIZE        1024

#define MUX_LL_UART_AUDIO_CMD_TX_BUF_SIZE   1024
#define MUX_LL_UART_AUDIO_CMD_RX_BUF_SIZE   1024

#define MUX_LL_UART_L2R_DL_TX_BUF_SIZE      1024
#define MUX_LL_UART_L2R_DL_RX_BUF_SIZE      1024

#define MUX_LL_UART_R2L_DL_TX_BUF_SIZE      32768 //32KB
#define MUX_LL_UART_R2L_DL_RX_BUF_SIZE      32768

#define MUX_LL_UART_UL_TX_BUF_SIZE          8
#define MUX_LL_UART_UL_RX_BUF_SIZE          8

#define MUX_LL_UART_LATCH_TX_BUF_SIZE       256
#define MUX_LL_UART_LATCH_RX_BUF_SIZE       256

#define MUX_LL_UART_INVALID_PRIORITY         0xF

#endif //__MUX_LL_UART_CONFIGURE_H__
