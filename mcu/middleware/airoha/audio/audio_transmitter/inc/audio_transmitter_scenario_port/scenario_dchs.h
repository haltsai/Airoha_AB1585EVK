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
#ifndef __SCENARIO_DCHS_H__
#define __SCENARIO_DCHS_H__

#ifdef __cplusplus
extern "C" {
#endif

#if defined (AIR_DUAL_CHIP_MIXING_MODE_ROLE_MASTER_ENABLE) || defined (AIR_DUAL_CHIP_MIXING_MODE_ROLE_SLAVE_ENABLE)

/******************************************************************************
 * Include
 ******************************************************************************/

#include "audio_log.h"
#include "FreeRTOS.h"


#include "audio_nvdm_coef.h"
#include "audio_nvdm_common.h"
#include "hal_audio_message_struct.h"
#include "bt_codec.h"

/******************************************************************************
 * Pubilc Global Variables and Macro and Enum
 ******************************************************************************/
typedef enum
{
    UART_DL,
    UART_UL,
    UART_AUDIO_CMD,
    UART_MAX,
}uart_type_t;

typedef enum
{
    AUDIO_UART_COSYS_CMD_MIN,
    AUDIO_UART_COSYS_DL_OPEN,
    AUDIO_UART_COSYS_DL_START,
    AUDIO_UART_COSYS_DL_CLOSE,
    AUDIO_UART_COSYS_UL_OPEN,
    AUDIO_UART_COSYS_UL_START,
    AUDIO_UART_COSYS_UL_CLOSE,
    //UL cmd type extend here
    AUDIO_UART_COSYS_CMD_MAX = 0xFFFFFFFF,
}uart_cmd_type_t;

typedef struct
{
    uart_cmd_type_t ctrl_type;
    uint32_t param_size;
}audio_uart_cmd_header_t;

typedef struct
{
    audio_uart_cmd_header_t header;
    audio_scenario_type_t scenario_type;
    bt_a2dp_codec_type_t a2dp_codec_type;
    hal_audio_memory_t in_memory;
    hal_audio_memory_t out_memory;
    hal_audio_interface_t in_interface;
    hal_audio_interface_t out_interface;
    uint32_t sampling_rate;
    uint32_t frame_size;
    uint8_t frame_number;
    hal_audio_format_t format;
    uint8_t  irq_period;
} audio_dchs_dl_open_param_t;

typedef struct
{
    audio_uart_cmd_header_t header;
    audio_scenario_type_t scenario_type;
    uint32_t              start_asi;
    uint32_t              start_bt_clk;
    uint32_t              start_bt_intra_clk;
} audio_dchs_dl_start_param_t;

typedef struct
{
    audio_uart_cmd_header_t header;
    audio_scenario_type_t scenario_type;
} audio_dchs_dl_close_param_t;

typedef union{
    audio_dchs_dl_open_param_t  dchs_dl_open_param;
    audio_dchs_dl_start_param_t dchs_dl_start_param;
    audio_dchs_dl_close_param_t dchs_dl_close_param;
    // ul param add here
}audio_dchs_cosys_ctrl_param_t;

typedef struct
{
    uart_cmd_type_t ctrl_type;
    audio_dchs_cosys_ctrl_param_t ctrl_param;
}audio_dchs_cosys_ctrl_t;


/******************************************************************************
 * Pubilc Function Declaration
 ******************************************************************************/

extern void mcu_uart_open(uart_type_t type);
extern void mcu_uart_tx(uart_type_t type, uint8_t *param_buf, uint32_t buf_size);
extern void mcu_uart_init(void);

extern void mcu_dchs_cosys_ctrl_cmd_relay(uart_cmd_type_t ctrl_type, audio_scenario_type_t scenario_type, mcu2dsp_open_param_t *open_param, mcu2dsp_start_param_t * start_param);
extern void mcu_dchs_cosys_ctrl_cmd_execute(audio_dchs_cosys_ctrl_t  cosys_ctrl);

extern void mcu_set_local_chip_dchs_dl_status(bool is_running);
extern bool mcu_check_other_chip_dchs_dl_status(void);
extern void mcu_bt_lock_sleep_done(void);


#endif

#ifdef __cplusplus
}
#endif

#endif/*__SCENARIO_DCHS_H__*/