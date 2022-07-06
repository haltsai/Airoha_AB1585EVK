/* Copyright Statement:
 *
 * (C) 2020  Airoha Technology Corp. All rights reserved.
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
#include "audio_transmitter_playback_port.h"
#include "audio_log.h"
#include "FreeRTOS.h"
#include "bt_sink_srv_ami.h"

#include "audio_nvdm_coef.h"
#include "audio_nvdm_common.h"
#include "hal_audio_message_struct.h"

#ifdef AIR_AUDIO_I2S_SLAVE_TDM_ENABLE
#if defined(MTK_AVM_DIRECT)
extern HAL_DSP_PARA_AU_AFE_CTRL_t audio_nvdm_HW_config;
#endif
#endif

#if defined AIR_DUAL_CHIP_MIXING_MODE_ROLE_MASTER_ENABLE || defined AIR_DUAL_CHIP_MIXING_MODE_ROLE_SLAVE_ENABLE
extern void audio_transmitter_dchs_open_playback(audio_transmitter_config_t *config, mcu2dsp_open_param_t *open_param);
extern void audio_transmitter_dchs_start_playback(audio_transmitter_config_t *config, mcu2dsp_start_param_t *start_param);
#endif

void audio_transmitter_reset_share_info_by_block(n9_dsp_share_info_t *p_share_info, uint32_t buffer_start_address, uint16_t buffer_length, uint32_t max_payload_size)
{
    p_share_info->start_addr = buffer_start_address;
    p_share_info->sub_info.block_info.block_size = (max_payload_size + BLK_HEADER_SIZE + 3) / 4 * 4;  //4B align
    p_share_info->sub_info.block_info.block_num = buffer_length / p_share_info->sub_info.block_info.block_size;
    p_share_info->length = p_share_info->sub_info.block_info.block_size * p_share_info->sub_info.block_info.block_num;
    p_share_info->read_offset = 0;
    p_share_info->write_offset = 0;
    p_share_info->bBufferIsFull = 0;
}
void audio_transmitter_modify_share_info_by_block(n9_dsp_share_info_t *p_share_info, uint32_t max_payload_size)
{
    p_share_info->sub_info.block_info.block_size = (max_payload_size + BLK_HEADER_SIZE + 3) / 4 * 4; //4B align
    p_share_info->sub_info.block_info.block_num = p_share_info->length / p_share_info->sub_info.block_info.block_size;
    p_share_info->length = p_share_info->sub_info.block_info.block_size * p_share_info->sub_info.block_info.block_num;
}

/*------------------------------------------------PORT----AIR_AUDIO_I2S_SLAVE_TDM_ENABLE------------------------------------------------------------------*/
#if defined(AIR_AUDIO_I2S_SLAVE_TDM_ENABLE)
void audio_transmitter_tdm_open_playback(audio_transmitter_config_t *config, mcu2dsp_open_param_t *open_param)
{
    open_param->param.stream_in  = STREAM_IN_AFE;
#ifdef AIR_AUDIO_I2S_SLAVE_TDM_VIRTUAL_OUT_ENABLE
    open_param->param.stream_out = STREAM_OUT_VIRTUAL;
#else
    open_param->param.stream_out = STREAM_OUT_AFE;
#endif
    hal_audio_get_stream_in_setting_config(AU_DSP_VOICE, &open_param->stream_in_param);
    hal_audio_get_stream_out_setting_config(AU_DSP_VOICE, &open_param->stream_out_param);
    if (config->scenario_config.tdm_config.frame_size != 32 && config->scenario_config.tdm_config.frame_size != 64) {
        TRANSMITTER_LOG_I("[mcu][TDM] frame size is only support 32 and 64!", 0);
        return;
    }
    if (config->scenario_config.tdm_config.in_channel <= HAL_AUDIO_I2S_TDM_DISABLE || config->scenario_config.tdm_config.in_channel > HAL_AUDIO_I2S_TDM_8CH) {
        TRANSMITTER_LOG_I("[mcu][TDM] TDM in channel don't support!", 0);
        return;
    }
    if (config->scenario_config.tdm_config.out_channel < HAL_AUDIO_I2S_TDM_DISABLE || config->scenario_config.tdm_config.out_channel > HAL_AUDIO_I2S_TDM_8CH) {
        TRANSMITTER_LOG_I("[mcu][TDM] TDM out channel don't support!", 0);
        return;
    }
    if (config->scenario_config.tdm_config.out_channel == HAL_AUDIO_I2S_TDM_DISABLE) {
        open_param->param.stream_out = STREAM_OUT_VIRTUAL;
    }
    printf("[TDM][MCU][Cfg]sampling_rate = %d", hal_audio_sampling_rate_enum_to_value(config->scenario_config.tdm_config.sampling_rate));
    printf("[TDM][MCU][Cfg]frame size    = %d", config->scenario_config.tdm_config.frame_size);
    printf("[TDM][MCU][Cfg]in TDM channel    = %d", config->scenario_config.tdm_config.in_channel * 2);
    printf("[TDM][MCU][Cfg]out TDM channel   = %d", config->scenario_config.tdm_config.out_channel * 2);
#if 0 //i2s2 for tdm
    hal_gpio_init(41);
    hal_pinmux_set_function(41, 2); //clk

    hal_gpio_init(43);
    hal_pinmux_set_function(43, 2); //ws

    hal_gpio_init(42);
    hal_pinmux_set_function(42, 2); //tx

    hal_gpio_init(34);
    hal_pinmux_set_function(34, 2); //rx
#endif
    open_param->stream_in_param.afe.audio_device = HAL_AUDIO_DEVICE_I2S_SLAVE;
    open_param->stream_in_param.afe.memory = config->scenario_config.tdm_config.in_memory;//HAL_AUDIO_MEM1
    open_param->stream_in_param.afe.audio_interface = config->scenario_config.tdm_config.in_interface;//HAL_AUDIO_INTERFACE_1
    open_param->stream_in_param.afe.format = AFE_PCM_FORMAT_S32_LE;
    open_param->stream_in_param.afe.sampling_rate = hal_audio_sampling_rate_enum_to_value(config->scenario_config.tdm_config.sampling_rate);
    open_param->stream_in_param.afe.irq_period = 0;
    open_param->stream_in_param.afe.frame_size = config->scenario_config.tdm_config.frame_size;
    open_param->stream_in_param.afe.frame_number = 4;
    open_param->stream_in_param.afe.hw_gain = false;
#ifdef MTK_AUDIO_HW_IO_CONFIG_ENHANCE
    //I2S Slave parameters
    open_param->stream_in_param.afe.i2s_format = HAL_AUDIO_I2S_I2S;
    open_param->stream_in_param.afe.i2s_word_length = HAL_AUDIO_I2S_WORD_LENGTH_32BIT;
    open_param->stream_in_param.afe.i2S_Slave_TDM = config->scenario_config.tdm_config.in_channel;
#endif
    open_param->stream_out_param.afe.audio_device = HAL_AUDIO_DEVICE_I2S_SLAVE;
    open_param->stream_out_param.afe.memory = config->scenario_config.tdm_config.out_memory;//HAL_AUDIO_MEM1
    open_param->stream_out_param.afe.audio_interface = config->scenario_config.tdm_config.out_interface;//HAL_AUDIO_INTERFACE_1
    open_param->stream_out_param.afe.format = AFE_PCM_FORMAT_S32_LE;
    open_param->stream_out_param.afe.stream_out_sampling_rate = hal_audio_sampling_rate_enum_to_value(config->scenario_config.tdm_config.sampling_rate);
    open_param->stream_out_param.afe.sampling_rate = hal_audio_sampling_rate_enum_to_value(config->scenario_config.tdm_config.sampling_rate);
    open_param->stream_out_param.afe.irq_period = 0;
    open_param->stream_out_param.afe.frame_size = config->scenario_config.tdm_config.frame_size;
    open_param->stream_out_param.afe.frame_number = 4;
    open_param->stream_out_param.afe.hw_gain = true;
#ifdef MTK_AUDIO_HW_IO_CONFIG_ENHANCE
    //I2S Slave parameters
    open_param->stream_out_param.afe.i2s_format = HAL_AUDIO_I2S_I2S;
    open_param->stream_out_param.afe.i2s_word_length = HAL_AUDIO_I2S_WORD_LENGTH_32BIT;
    open_param->stream_out_param.afe.i2S_Slave_TDM = config->scenario_config.tdm_config.out_channel;
#endif

    open_param->stream_in_param.afe.ul_adc_mode[0] = HAL_AUDIO_ANALOG_INPUT_ACC10K;
    open_param->stream_in_param.afe.performance = AFE_PEROFRMANCE_HIGH_MODE;
#ifdef ENABLE_HWSRC_CLKSKEW
    open_param->stream_out_param.afe.clkskew_mode = CLK_SKEW_V1;
#endif
#ifdef MTK_AUDIO_HW_IO_CONFIG_ENHANCE
    open_param->stream_out_param.afe.performance = AFE_PEROFRMANCE_NORMAL_MODE;
#endif
}

void audio_transmitter_tdm_start_playback(audio_transmitter_config_t *config, mcu2dsp_start_param_t *start_param)
{
    start_param->param.stream_in = STREAM_IN_AFE;
#ifdef AIR_AUDIO_I2S_SLAVE_TDM_VIRTUAL_OUT_ENABLE
    start_param->param.stream_out = STREAM_OUT_VIRTUAL;
#else
    start_param->param.stream_out = STREAM_OUT_AFE;
#endif
    memset((void *)&start_param->stream_in_param, 0, sizeof(mcu2dsp_start_stream_in_param_t));
    memset((void *)&start_param->stream_out_param, 0, sizeof(mcu2dsp_start_stream_out_param_t));
}
#endif

/*---------------------------------------------------------------------------------------------------------------------------*/
audio_transmitter_playback_func_t audio_transmitter_playback_func[AUDIO_TRANSMITTER_SCENARIO_TYPE_MAX] = {
    //open_playback_t,                                    start_playback_t,                set_runtime_config_playback
    {NULL, NULL, NULL}, //AUDIO_TRANSMITTER_A2DP_SOURCE
#if defined(MTK_SENSOR_SOURCE_ENABLE)
    {audio_transmitter_gsensor_open_playback,     audio_transmitter_gsensor_start_playback, NULL},
#else
    {NULL, NULL, NULL}, //AUDIO_TRANSMITTER_GSENSOR
#endif
#if defined(AIR_MULTI_MIC_STREAM_ENABLE)
    {audio_transmitter_multmic_open_playback,     audio_transmitter_multmic_start_playback, NULL},
#else
    {NULL, NULL, NULL}, //AUDIO_TRANSMITTER_MULTI_MIC_STREAM
#endif
#if defined(AIR_BT_ULTRA_LOW_LATENCY_ENABLE)
    {gaming_mode_open_playback,     gaming_mode_start_playback,    gaming_mode_set_runtime_config_playback},
#else
    {NULL, NULL, NULL},//AUDIO_TRANSMITTER_GAMING_MODE
#endif
#if defined (MTK_ANC_SURROUND_MONITOR_ENABLE)
    {audio_transmitter_anc_monitor_open_playback,  audio_transmitter_anc_monitor_start_playback, NULL},
#else
    {NULL, NULL, NULL}, //AUDIO_TRANSMITTER_ANC_MONITOR_STREAM
#endif
    {audio_transmitter_test_open_playback, audio_transmitter_test_start_playback, NULL},//AUDIO_TRANSMITTER_TEST
#if defined (AIR_AUDIO_I2S_SLAVE_TDM_ENABLE)
    {audio_transmitter_tdm_open_playback,  audio_transmitter_tdm_start_playback, NULL},
#else
    {NULL, NULL, NULL}, //AUDIO_TRANSMITTER_TDM
#endif
#if defined (AIR_WIRED_AUDIO_ENABLE)
    {audio_transmitter_wired_audio_open_playback,  audio_transmitter_wired_audio_start_playback, wired_audio_set_runtime_config_playback},
#else
    {NULL, NULL, NULL}, //AUDIO_TRANSMITTER_ANC_MONITOR_STREAM
#endif
#if defined (AIR_ADVANCED_PASSTHROUGH_ENABLE)
    {advanced_passthrough_open_playback,  advanced_passthrough_start_playback, advanced_passthrough_set_runtime_config_playback},
#else
    {NULL, NULL, NULL},
#endif /* AIR_ADVANCED_PASSTHROUGH_ENABLE */
#if defined(AIR_BLE_AUDIO_DONGLE_ENABLE)
    {ble_audio_dongle_open_playback,     ble_audio_dongle_start_playback,    ble_audio_dongle_set_runtime_config_playback},
#else
    {NULL, NULL, NULL},//AUDIO_TRANSMITTER_BLE_AUDIO_SOURCE_DONGLE
#endif /* AIR_BLE_AUDIO_DONGLE_ENABLE */
#if defined (AIR_AUDIO_HW_LOOPBACK_ENABLE)
    {audio_hw_loopback_open_playback,    audio_hw_loopback_start_playback,   audio_hw_loopback_set_runtime_config_playback},
#else
    {NULL, NULL, NULL},//AUDIO_TRANSMITTER_AUDIO_HW_LOOKBACK
#endif /* AIR_AUDIO_HW_LOOPBACK_ENABLE */
#if defined (AIR_ADAPTIVE_EQ_ENABLE)
    {audio_transmitter_adaptive_eq_monitor_open_playback,    audio_transmitter_adaptive_eq_monitor_start_playback,   NULL},
#else
    {NULL, NULL, NULL},//AUDIO_TRANSMITTER_ADAPTIVE_EQ_MONITOR_STREAM
#endif /* AIR_ADAPTIVE_EQ_ENABLE */
#if defined(AIR_ULL_AUDIO_V2_DONGLE_ENABLE)
    {ull_audio_v2_dongle_open_playback,     ull_audio_v2_dongle_start_playback,    ull_audio_v2_dongle_set_runtime_config_playback},
#else
    {NULL, NULL, NULL},//AUDIO_TRANSMITTER_BLE_AUDIO_SOURCE_DONGLE
#endif /* AIR_ULL_AUDIO_V2_DONGLE_ENABLE */
#if defined (AIR_DUAL_CHIP_MIXING_MODE_ROLE_MASTER_ENABLE) || defined (AIR_DUAL_CHIP_MIXING_MODE_ROLE_SLAVE_ENABLE)
    {audio_transmitter_dchs_open_playback,    audio_transmitter_dchs_start_playback,   NULL},
#else
    {NULL, NULL, NULL},//AUDIO_TRANSMITTER_DCHS
#endif /* AIR_DUAL_CHIP_MIXING_MODE_ROLE_MASTER_ENABLE||AIR_DUAL_CHIP_MIXING_MODE_ROLE_SLAVE_ENABLE*/
};

/*---------------------------------------------------------------------------------------------------------------------------*/
audio_transmitter_runtime_config_handler_t audio_transmitter_runtime_config_handler[AUDIO_TRANSMITTER_SCENARIO_TYPE_MAX] = {
    //set_runtime_config             get_runtime_config
    { NULL},
    { NULL},
    { NULL},
#if defined(AIR_BT_ULTRA_LOW_LATENCY_ENABLE)
    { gaming_mode_get_runtime_config},
#else
    { NULL},
#endif
    { NULL},
    { NULL},
    { NULL},
#if defined (AIR_WIRED_AUDIO_ENABLE)
    { NULL},
#else
    { NULL},
#endif
#if defined(AIR_ADVANCED_PASSTHROUGH_ENABLE)
    { advanced_passthrough_get_runtime_config},
#else
    { NULL},
#endif /* AIR_ADVANCED_PASSTHROUGH_ENABLE */
#if defined(AIR_BLE_AUDIO_DONGLE_ENABLE)
    { ble_audio_dongle_get_runtime_config},
#else
    { NULL},
#endif /* defined(AIR_BLE_AUDIO_DONGLE_ENABLE) */
#if defined (AIR_AUDIO_HW_LOOPBACK_ENABLE)
    {NULL},
#else
    { NULL},
#endif /* defined(AIR_AUDIO_HW_LOOPBACK_ENABLE) */
    { NULL},// AUDIO_TRANSMITTER_ADAPTIVE_EQ_MONITOR_STREAM
#if defined(AIR_ULL_AUDIO_V2_DONGLE_ENABLE)
    { ull_audio_v2_dongle_get_runtime_config},
#else
    { NULL},
#endif /* defined(AIR_ULL_AUDIO_V2_DONGLE_ENABLE) */
};

/*---------------------------------------------------------------------------------------------------------------------------*/
state_change_handler_t audio_transmitter_state_change_handler[AUDIO_TRANSMITTER_SCENARIO_TYPE_MAX][AUDIO_TRANSMITTER_STATE_MAX] = {
    //AUDIO_TRANSMITTER_STATE_CLOSE   AUDIO_TRANSMITTER_STATE_IDLE   AUDIO_TRANSMITTER_STATE_STARTING   AUDIO_TRANSMITTER_STATE_STARTED   AUDIO_TRANSMITTER_STATE_STOPING = 4,
    {NULL, NULL, NULL, NULL, NULL},//AUDIO_TRANSMITTER_A2DP_SOURCE
    {NULL, NULL, NULL, NULL, NULL},//AUDIO_TRANSMITTER_GSENSOR
    {NULL, NULL, NULL, NULL, NULL},//AUDIO_TRANSMITTER_MULTI_MIC_STREAM
#if defined(AIR_BT_ULTRA_LOW_LATENCY_ENABLE)
    {NULL, gaming_mode_state_idle_handler, gaming_mode_state_starting_handler, gaming_mode_state_started_handler, gaming_mode_state_stoping_handler},
#else
    {NULL, NULL, NULL, NULL, NULL},
#endif
    {NULL, NULL, NULL, NULL, NULL},//AUDIO_TRANSMITTER_ANC_MONITOR_STREAM
    {NULL, NULL, NULL, NULL, NULL},//AUDIO_TRANSMITTER_TEST
    {NULL, NULL, NULL, NULL, NULL},//AUDIO_TRANSMITTER_TDM
#if defined(AIR_WIRED_AUDIO_ENABLE)
    {NULL, wired_audio_state_idle_handler, NULL, wired_audio_state_started_handler, NULL},
#else
    {NULL, NULL, NULL, NULL, NULL},
#endif
#if defined(AIR_ADVANCED_PASSTHROUGH_ENABLE)
    {NULL, advanced_passthrough_state_idle_handler, NULL, advanced_passthrough_state_started_handler, NULL},
#else
    {NULL, NULL, NULL, NULL, NULL},
#endif /* AIR_ADVANCED_PASSTHROUGH_ENABLE */
#if defined(AIR_BLE_AUDIO_DONGLE_ENABLE)
    {NULL, ble_audio_dongle_state_idle_handler, ble_audio_dongle_state_starting_handler, ble_audio_dongle_state_started_handler, ble_audio_dongle_state_stoping_handler},
#else
    {NULL, NULL, NULL, NULL, NULL},//AUDIO_TRANSMITTER_BLE_AUDIO_SOURCE_DONGLE
#endif /* defined(AIR_BLE_AUDIO_DONGLE_ENABLE) */
#if defined (AIR_AUDIO_HW_LOOPBACK_ENABLE)
    {NULL, NULL, NULL, NULL, NULL},
#else
    {NULL, NULL, NULL, NULL, NULL},
#endif /* AIR_AUDIO_HW_LOOPBACK_ENABLE */
     {NULL, NULL, NULL, NULL, NULL},//AUDIO_TRANSMITTER_ADAPTIVE_EQ_MONITOR_STREAM
#if defined(AIR_ULL_AUDIO_V2_DONGLE_ENABLE)
    {NULL, ull_audio_v2_dongle_state_idle_handler, ull_audio_v2_dongle_state_starting_handler, ull_audio_v2_dongle_state_started_handler, ull_audio_v2_dongle_state_stoping_handler},
#else
    {NULL, NULL, NULL, NULL, NULL},//AUDIO_TRANSMITTER_BLE_AUDIO_SOURCE_DONGLE
#endif /* defined(AIR_ULL_AUDIO_V2_DONGLE_ENABLE) */
};

/*---------------------------------------------------------------------------------------------------------------------------*/

audio_transmitter_clock_setting_type_t audio_transmitter_clock_setting_type[] = {
    //scenario                        sub_id                            audio_scenario_clock_setting_type
    // AUDIO_TRANSMITTER_A2DP_SOURCE                  = 0,

    // AUDIO_TRANSMITTER_GSENSOR                      = 1,
    {AUDIO_TRANSMITTER_GSENSOR, AUDIO_TRANSMITTER_GSENSOR_FUNCTION_D,                   AUDIO_SCENARIO_TYPE_GSENSOR_FUNCTION_D},
    {AUDIO_TRANSMITTER_GSENSOR, AUDIO_TRANSMITTER_GSENSOR_FUNCTION_F,                   AUDIO_SCENARIO_TYPE_GSENSOR_FUNCTION_F},
    {AUDIO_TRANSMITTER_GSENSOR, AUDIO_TRANSMITTER_GSENSOR_FUNCTION_G,                   AUDIO_SCENARIO_TYPE_GSENSOR_FUNCTION_G},
    // AUDIO_TRANSMITTER_MULTI_MIC_STREAM             = 2,
    {AUDIO_TRANSMITTER_MULTI_MIC_STREAM, AUDIO_TRANSMITTER_MULTI_MIC_STREAM_FUNCTION_A,                   AUDIO_SCENARIO_TYPE_MULTI_MIC_STREAM_FUNCTION_A},
    {AUDIO_TRANSMITTER_MULTI_MIC_STREAM, AUDIO_TRANSMITTER_MULTI_MIC_STREAM_FUNCTION_B,                   AUDIO_SCENARIO_TYPE_MULTI_MIC_STREAM_FUNCTION_B},
    {AUDIO_TRANSMITTER_MULTI_MIC_STREAM, AUDIO_TRANSMITTER_MULTI_MIC_STREAM_FUNCTION_C,                   AUDIO_SCENARIO_TYPE_MULTI_MIC_STREAM_FUNCTION_C},
    {AUDIO_TRANSMITTER_MULTI_MIC_STREAM, AUDIO_TRANSMITTER_MULTI_MIC_STREAM_FUNCTION_F,                   AUDIO_SCENARIO_TYPE_MULTI_MIC_STREAM_FUNCTION_F},
    // AUDIO_TRANSMITTER_GAMING_MODE                  = 3,
    {AUDIO_TRANSMITTER_GAMING_MODE, AUDIO_TRANSMITTER_GAMING_MODE_VOICE_HEADSET,                   AUDIO_SCENARIO_TYPE_GAMING_MODE_VOICE_HEADSET},
    {AUDIO_TRANSMITTER_GAMING_MODE, AUDIO_TRANSMITTER_GAMING_MODE_VOICE_DONGLE_USB_OUT,            AUDIO_SCENARIO_TYPE_GAMING_MODE_VOICE_DONGLE_USB_OUT},
    {AUDIO_TRANSMITTER_GAMING_MODE, AUDIO_TRANSMITTER_GAMING_MODE_MUSIC_DONGLE_USB_IN_0,           AUDIO_SCENARIO_TYPE_GAMING_MODE_MUSIC_DONGLE_USB_IN_0},
    {AUDIO_TRANSMITTER_GAMING_MODE, AUDIO_TRANSMITTER_GAMING_MODE_MUSIC_DONGLE_USB_IN_1,           AUDIO_SCENARIO_TYPE_GAMING_MODE_MUSIC_DONGLE_USB_IN_1},
    {AUDIO_TRANSMITTER_GAMING_MODE, AUDIO_TRANSMITTER_GAMING_MODE_VOICE_DONGLE_LINE_OUT,           AUDIO_SCENARIO_TYPE_GAMING_MODE_VOICE_DONGLE_LINE_OUT},
    {AUDIO_TRANSMITTER_GAMING_MODE, AUDIO_TRANSMITTER_GAMING_MODE_MUSIC_DONGLE_LINE_IN,            AUDIO_SCENARIO_TYPE_GAMING_MODE_MUSIC_DONGLE_LINE_IN},
    {AUDIO_TRANSMITTER_GAMING_MODE, AUDIO_TRANSMITTER_GAMING_MODE_VOICE_DONGLE_I2S_OUT,            AUDIO_SCENARIO_TYPE_GAMING_MODE_VOICE_DONGLE_I2S_OUT},
    {AUDIO_TRANSMITTER_GAMING_MODE, AUDIO_TRANSMITTER_GAMING_MODE_MUSIC_DONGLE_I2S_IN,             AUDIO_SCENARIO_TYPE_GAMING_MODE_MUSIC_DONGLE_I2S_IN},
    // AUDIO_TRANSMITTER_ANC_MONITOR_STREAM           = 4,
    {AUDIO_TRANSMITTER_ANC_MONITOR_STREAM, 0,            AUDIO_SCENARIO_TYPE_ANC_MONITOR_STREAM},
    // AUDIO_TRANSMITTER_TEST                         = 5,
    {AUDIO_TRANSMITTER_TEST, AUDIO_TRANSMITTER_TEST_AUDIO_LOOPBACK,            AUDIO_SCENARIO_TYPE_TEST_AUDIO_LOOPBACK},
    // AUDIO_TRANSMITTER_TDM                          = 6,
    {AUDIO_TRANSMITTER_TDM, 0,            AUDIO_SCENARIO_TYPE_TDM},
    // AUDIO_TRANSMITTER_WIRED_AUDIO                  = 7,
    {AUDIO_TRANSMITTER_WIRED_AUDIO, AUDIO_TRANSMITTER_WIRED_AUDIO_USB_OUT,                   AUDIO_SCENARIO_TYPE_WIRED_AUDIO_USB_OUT},
    {AUDIO_TRANSMITTER_WIRED_AUDIO, AUDIO_TRANSMITTER_WIRED_AUDIO_USB_IN_0,                  AUDIO_SCENARIO_TYPE_WIRED_AUDIO_USB_IN_0},
    {AUDIO_TRANSMITTER_WIRED_AUDIO, AUDIO_TRANSMITTER_WIRED_AUDIO_USB_IN_1,                  AUDIO_SCENARIO_TYPE_WIRED_AUDIO_USB_IN_1},
    {AUDIO_TRANSMITTER_WIRED_AUDIO, AUDIO_TRANSMITTER_WIRED_AUDIO_LINE_OUT,                  AUDIO_SCENARIO_TYPE_WIRED_AUDIO_LINE_OUT},
    {AUDIO_TRANSMITTER_WIRED_AUDIO, AUDIO_TRANSMITTER_WIRED_AUDIO_LINE_IN,                   AUDIO_SCENARIO_TYPE_WIRED_AUDIO_LINE_IN},
    {AUDIO_TRANSMITTER_WIRED_AUDIO, AUDIO_TRANSMITTER_WIRED_AUDIO_DUAL_CHIP_LINE_IN_MASTER,  AUDIO_SCENARIO_TYPE_WIRED_AUDIO_DUAL_CHIP_LINE_IN_MASTER},
    {AUDIO_TRANSMITTER_WIRED_AUDIO, AUDIO_TRANSMITTER_WIRED_AUDIO_DUAL_CHIP_LINE_IN_SLAVE,   AUDIO_SCENARIO_TYPE_WIRED_AUDIO_DUAL_CHIP_LINE_IN_SLAVE},
    {AUDIO_TRANSMITTER_WIRED_AUDIO, AUDIO_TRANSMITTER_WIRED_AUDIO_DUAL_CHIP_LINE_OUT_MASTER, AUDIO_SCENARIO_TYPE_WIRED_AUDIO_DUAL_CHIP_LINE_OUT_MASTER},
    // AUDIO_TRANSMITTER_ADVANCED_PASSTHROUGH         = 8,
    {AUDIO_TRANSMITTER_ADVANCED_PASSTHROUGH, AUDIO_TRANSMITTER_ADVANCED_PASSTHROUGH_HEARING_AID,                   AUDIO_SCENARIO_TYPE_ADVANCED_PASSTHROUGH_HEARING_AID},
    // AUDIO_TRANSMITTER_BLE_AUDIO_DONGLE             = 9,
    {AUDIO_TRANSMITTER_BLE_AUDIO_DONGLE, AUDIO_TRANSMITTER_BLE_AUDIO_DONGLE_VOICE_USB_OUT,                   AUDIO_SCENARIO_TYPE_BLE_AUDIO_DONGLE_VOICE_USB_OUT},
    {AUDIO_TRANSMITTER_BLE_AUDIO_DONGLE, AUDIO_TRANSMITTER_BLE_AUDIO_DONGLE_MUSIC_USB_IN_0,                  AUDIO_SCENARIO_TYPE_BLE_AUDIO_DONGLE_MUSIC_USB_IN_0},
    {AUDIO_TRANSMITTER_BLE_AUDIO_DONGLE, AUDIO_TRANSMITTER_BLE_AUDIO_DONGLE_MUSIC_USB_IN_1,                  AUDIO_SCENARIO_TYPE_BLE_AUDIO_DONGLE_MUSIC_USB_IN_1},
    {AUDIO_TRANSMITTER_BLE_AUDIO_DONGLE, AUDIO_TRANSMITTER_BLE_AUDIO_DONGLE_VOICE_LINE_OUT,                  AUDIO_SCENARIO_TYPE_BLE_AUDIO_DONGLE_VOICE_LINE_OUT},
    {AUDIO_TRANSMITTER_BLE_AUDIO_DONGLE, AUDIO_TRANSMITTER_BLE_AUDIO_DONGLE_MUSIC_LINE_IN,                   AUDIO_SCENARIO_TYPE_BLE_AUDIO_DONGLE_MUSIC_LINE_IN},
    // AUDIO_TRANSMITTER_AUDIO_HW_LOOPBACK            = 10,
    {AUDIO_TRANSMITTER_AUDIO_HW_LOOPBACK, AUDIO_TRANSMITTER_AUDIO_HW_LOOPBACK_I2S0_TO_DAC,                   AUDIO_SCENARIO_TYPE_AUDIO_HW_LOOPBACK_I2S0_TO_DAC},
    {AUDIO_TRANSMITTER_AUDIO_HW_LOOPBACK, AUDIO_TRANSMITTER_AUDIO_HW_LOOPBACK_ADC_TO_I2S0,                   AUDIO_SCENARIO_TYPE_AUDIO_HW_LOOPBACK_ADC_TO_I2S0},
    {AUDIO_TRANSMITTER_AUDIO_HW_LOOPBACK, AUDIO_TRANSMITTER_AUDIO_HW_LOOPBACK_LINEIN_TO_I2S2,                AUDIO_SCENARIO_TYPE_AUDIO_HW_LOOPBACK_LINEIN_TO_I2S2},
    // AUDIO_TRANSMITTER_AUDIO_HW_LOOPBACK            = 11,
    {AUDIO_TRANSMITTER_ADAPTIVE_EQ_MONITOR_STREAM, 0,    AUDIO_SCENARIO_TYPE_ADAPTIVE_EQ_MONITOR_STREAM},
    // AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE          = 12,
    {AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE, AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_UL_USB_OUT_0,                   AUDIO_SCENARIO_TYPE_ULL_AUDIO_V2_DONGLE_UL_USB_OUT_0},
    {AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE, AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_UL_LINE_OUT,                    AUDIO_SCENARIO_TYPE_ULL_AUDIO_V2_DONGLE_UL_LINE_OUT},
    {AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE, AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_UL_I2S_MST_OUT_0,               AUDIO_SCENARIO_TYPE_ULL_AUDIO_V2_DONGLE_UL_I2S_MST_OUT_0},
    {AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE, AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_UL_I2S_SLV_OUT_0,               AUDIO_SCENARIO_TYPE_ULL_AUDIO_V2_DONGLE_UL_I2S_SLV_OUT_0},
    {AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE, AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_DL_USB_IN_0,                    AUDIO_SCENARIO_TYPE_ULL_AUDIO_V2_DONGLE_DL_USB_IN_0},
    {AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE, AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_DL_USB_IN_1,                    AUDIO_SCENARIO_TYPE_ULL_AUDIO_V2_DONGLE_DL_USB_IN_1},
    {AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE, AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_DL_LINE_IN,                     AUDIO_SCENARIO_TYPE_ULL_AUDIO_V2_DONGLE_DL_LINE_IN},
    {AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE, AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_DL_I2S_MST_IN_0,                AUDIO_SCENARIO_TYPE_ULL_AUDIO_V2_DONGLE_DL_I2S_MST_IN_0},
    {AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE, AUDIO_TRANSMITTER_ULL_AUDIO_V2_DONGLE_DL_I2S_SLV_IN_0,                AUDIO_SCENARIO_TYPE_ULL_AUDIO_V2_DONGLE_DL_I2S_SLV_IN_0},
	// AUDIO_TRANSMITTER_DCHS           = 13
    {AUDIO_TRANSMITTER_DCHS, AUDIO_TRANSMITTER_DCHS_UART_DL,    AUDIO_SCENARIO_TYPE_DCHS_UART_DL},
    {AUDIO_TRANSMITTER_DCHS, AUDIO_TRANSMITTER_DCHS_UART_UL,    AUDIO_SCENARIO_TYPE_DCHS_UART_UL},
};
uint16_t audio_transmitter_clock_setting_type_count = sizeof(audio_transmitter_clock_setting_type) / sizeof(audio_transmitter_clock_setting_type[0]);

