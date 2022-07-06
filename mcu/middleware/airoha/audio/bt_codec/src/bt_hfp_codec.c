/* Copyright Statement:
 *
 * (C) 2017  Airoha Technology Corp. All rights reserved.
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

#include "bt_hfp_codec_internal.h"
#include "bt_sink_srv_ami.h"
#include "audio_nvdm_common.h"
#include "hal_dvfs_internal.h"
#include "hal_gpt.h"
#include "hal_nvic.h"
#include "hal_audio.h"
#include "task_def.h"
#include "FreeRTOS.h"
#ifdef AIR_AIRDUMP_ENABLE
#ifdef MTK_RACE_CMD_ENABLE
#include "race_cmd_dsprealtime.h"
#endif
#endif
#ifdef MTK_AWS_MCE_ENABLE
#include "bt_aws_mce_report.h"
#endif
#include "bt_sink_srv_ami.h"
#if defined(MTK_ANC_ENABLE) && defined(MTK_VOICE_ANC_EQ)
#ifdef MTK_ANC_V2
#include "anc_control_api.h"
#else
#include "anc_control.h"
#endif
#endif
#include "hal_dvfs_internal.h"
#include "hal_audio_message_struct_common.h"


#ifdef HAL_DVFS_MODULE_ENABLED
#ifdef AIR_BTA_IC_PREMIUM_G2
#define HFP_DVFS_DEFAULT_SPEED      HAL_DVFS_HALF_SPEED_52M
#define HFP_DVFS_MAX_SPEED          HAL_DVFS_HIGH_SPEED_208M
#define HFP_DVFS_INHOUSE_2MIC_SPEED         HAL_DVFS_HALF_SPEED_52M
#define HFP_DVFS_INHOUSE_1MIC_INEAR_SPEED   HAL_DVFS_HALF_SPEED_52M
#define HFP_DVFS_INHOUSE_2MIC_INEAR_SPEED   HAL_DVFS_FULL_SPEED_104M
#define HFP_DVFS_3RD_PARTY_NR_SPEED   HAL_DVFS_FULL_SPEED_104M
#elif defined(AIR_BTA_IC_PREMIUM_G3)
#define HFP_DVFS_DEFAULT_SPEED      HAL_DVFS_OPP_LOW
#define HFP_DVFS_MAX_SPEED          HAL_DVFS_OPP_MID
#define HFP_DVFS_INHOUSE_2MIC_SPEED         HAL_DVFS_OPP_LOW
#define HFP_DVFS_INHOUSE_1MIC_INEAR_SPEED   HAL_DVFS_OPP_LOW
#define HFP_DVFS_INHOUSE_2MIC_INEAR_SPEED   HAL_DVFS_OPP_MID
#define HFP_DVFS_3RD_PARTY_NR_SPEED   HAL_DVFS_OPP_MID
#endif
#endif
#define DL_ONLY TRUE
#define UL_ONLY TRUE
#ifdef AIR_HFP_DNN_PATH_ENABLE
#define UL_DNN_EN TRUE
#endif
#define MSBC_SHARE_BUF_FRAME_SIZE 60
#define CVSD_SHARE_BUF_FRAME_SIZE 120
#define HFP_SHARE_BUF_PARTTERN_TOTAL_SIZE 480


/*******************************************************************************\
| Global Variables                                                              |
\*******************************************************************************/

static bool bt_hfp_codec_is_running = false;
static bt_hfp_codec_internal_handle_t *bt_hfp_codec_internal_handle = NULL;
#ifdef HAL_DVFS_MODULE_ENABLED
static dvfs_frequency_t g_hfp_dvfs_clk = HFP_DVFS_DEFAULT_SPEED;
#endif

log_create_module(BT_HFP_CODEC, PRINT_LEVEL_INFO);
#define TASK_LOG_MSGID_E(fmt, arg...)     LOG_MSGID_E(BT_HFP_CODEC, "[HFP CODEC]: "fmt,##arg)
#define TASK_LOG_MSGID_W(fmt, arg...)     LOG_MSGID_W(BT_HFP_CODEC, "[HFP CODEC]: "fmt,##arg)
#define TASK_LOG_MSGID_I(fmt, arg...)     LOG_MSGID_I(BT_HFP_CODEC, "[HFP CODEC]: "fmt,##arg)

/*******************************************************************************\
| Private Functions                                                             |
\*******************************************************************************/

/* Should not use this API */
static bt_codec_media_status_t bt_hfp_process(bt_media_handle_t *handle, bt_codec_media_event_t event)
{
    TASK_LOG_MSGID_E("process\r\n", 0);
    return BT_CODEC_MEDIA_STATUS_ERROR;
}

static void bt_set_aws_flag(bt_media_handle_t *handle, bool is_aws)
{
    bt_hfp_codec_internal_handle_t *internal_handle = (bt_hfp_codec_internal_handle_t *)handle;

    TASK_LOG_MSGID_I("hfp codec set aws flag:%d\r\n", 1, is_aws);
    internal_handle->aws_flag = is_aws;
}

static void bt_trigger_mic(bt_media_handle_t *handle)
{
    if (handle) {
        TASK_LOG_MSGID_I("hfp codec Trigger Mic On\r\n", 0);
        hal_audio_dsp_controller_send_message(MSG_MCU2DSP_BT_VOICE_UL_PLAY_EN, 0, 0, 0);
    }
}

static void bt_side_tone_enable(bt_media_handle_t *handle)
{
    mcu2dsp_sidetone_param_t sidetone;
    void *p_param_share;
    memset(&sidetone, 0, sizeof(mcu2dsp_sidetone_param_t));
    if (!handle) {
        TASK_LOG_MSGID_I("[HFP Codec]SideTone Enable No Handle\n", 0);
        return;
    }

    TASK_LOG_MSGID_I("[HFP Codec]SideTone Enable\n", 0);

    sidetone.in_device                       = HAL_AUDIO_DEVICE_MAIN_MIC_DUAL;
    sidetone.in_interface                    = HAL_AUDIO_INTERFACE_1;
    sidetone.in_misc_parms                   = MICBIAS_SOURCE_ALL | MICBIAS3V_OUTVOLTAGE_1p85v;
    sidetone.out_device                      = HAL_AUDIO_DEVICE_DAC_DUAL;
    sidetone.out_interface                   = HAL_AUDIO_INTERFACE_NONE;
    sidetone.out_misc_parms                  = DOWNLINK_PERFORMANCE_NORMAL;
    sidetone.in_channel                      = HAL_AUDIO_DIRECT;
    sidetone.gain                            = 600;
    sidetone.sample_rate                     = 16000;
    sidetone.on_delay_time                   = 150;

    p_param_share = hal_audio_dsp_controller_put_paramter(&sidetone, sizeof(mcu2dsp_sidetone_param_t), AUDIO_MESSAGE_TYPE_SIDETONE);
    hal_audio_dsp_controller_send_message(MSG_MCU2DSP_COMMON_SIDETONE_START, 0, (uint32_t)p_param_share, true);
}

static void bt_side_tone_disable(bt_media_handle_t *handle)
{
    mcu2dsp_sidetone_param_t sidetone;
    void *p_param_share;
    memset(&sidetone, 0, sizeof(mcu2dsp_sidetone_param_t));
    if (!handle) {
        TASK_LOG_MSGID_E("[HFP Codec]SideTone Disable No Handle\n", 0);
        return;
    }

    TASK_LOG_MSGID_I("[HFP Codec]SideTone Disable\n", 0);

    sidetone.in_device                       = HAL_AUDIO_DEVICE_MAIN_MIC_DUAL;
    sidetone.in_interface                    = HAL_AUDIO_INTERFACE_1;
    sidetone.in_misc_parms                   = MICBIAS_SOURCE_ALL | MICBIAS3V_OUTVOLTAGE_1p85v;
    sidetone.out_device                      = HAL_AUDIO_DEVICE_DAC_DUAL;
    sidetone.out_interface                   = HAL_AUDIO_INTERFACE_NONE;
    sidetone.out_misc_parms                  = DOWNLINK_PERFORMANCE_NORMAL;
    sidetone.in_channel                      = HAL_AUDIO_DIRECT;
    sidetone.gain                            = 0;
    sidetone.sample_rate                     = 16000;
    sidetone.on_delay_time                   = 0;

    p_param_share = hal_audio_dsp_controller_put_paramter(&sidetone, sizeof(mcu2dsp_sidetone_param_t), AUDIO_MESSAGE_TYPE_SIDETONE);
    hal_audio_dsp_controller_send_message(MSG_MCU2DSP_COMMON_SIDETONE_STOP, 0, (uint32_t)p_param_share, true);
}

bool bt_hfp_codec_query_is_running(void)
{
    return bt_hfp_codec_is_running;
}

hal_audio_sampling_rate_t bt_hfp_codec_query_sampling_rate(void)
{
    bt_hfp_codec_internal_handle_t *p_info = bt_hfp_codec_internal_handle;

    if (p_info->codec_type == BT_HFP_CODEC_TYPE_CVSD) {
        return HAL_AUDIO_SAMPLING_RATE_8KHZ;
    } else {
        return HAL_AUDIO_SAMPLING_RATE_16KHZ;
    }
}

hal_audio_channel_number_t bt_hfp_codec_query_channel_number(void)
{
    return HAL_AUDIO_MONO;
}

#ifdef AIR_HFP_FEATURE_MODE_ENABLE

typedef enum {
    HFP_FEATURE_MODE_NORMAL,
    HFP_FEATURE_MODE_PSAP_0,
    HFP_FEATURE_MODE_MAX,
} hfp_feature_mode_t;

static bool g_hfp_feature_mode_busy_flg = false;
static bool g_hfp_feature_mode_1st_notify = false;
static uint32_t g_hfp_feature_mode = 0;
SemaphoreHandle_t g_xSemaphore_hfp_nvkey_change_notify;

uint32_t hfp_get_feature_mode(void)
{
    return g_hfp_feature_mode;
}

void hfp_feature_mode_notify_callback(audio_nvdm_user_t user, audio_nvdm_status_t status, void *user_data)
{
    switch (status) {
        case AUDIO_NVDM_STATUS_PREV_CHANGE:
            g_hfp_feature_mode_1st_notify = true;
            break;
        case AUDIO_NVDM_STATUS_CHANGE:
            break;
        /*The audio nvkey can be update now*/
        case AUDIO_NVDM_STATUS_POST_CHANGE:
            if (g_hfp_feature_mode_1st_notify == false) {
                if (HAL_NVIC_QUERY_EXCEPTION_NUMBER == HAL_NVIC_NOT_EXCEPTION) {
                    xSemaphoreGive(g_xSemaphore_hfp_nvkey_change_notify);
                }else {
                    xSemaphoreGiveFromISR(g_xSemaphore_hfp_nvkey_change_notify,NULL);
                }
            } else {
                g_hfp_feature_mode_1st_notify = false;
            }
            break;
        default:
            break;
    }
}

/* Four type of call case
 * Case 1. call when the HFP is not start.
 * Case 2. call when the HFP is just start up, but not completed.
 * Case 3. call when the HFP has been start up.
 * Case 4. call when the HFP has been start up, and other user is changing the mode.
 * Only case 1/3 allows the mode change.
 */
bool hfp_set_feature_mode(uint32_t mode)
{
    uint32_t irq_status;
    bt_sink_srv_am_result_t am_status;

    if (mode >= HFP_FEATURE_MODE_MAX) {
        TASK_LOG_MSGID_E("hfp feature mode: invalid mode: %d", 1, mode);
        return false;
    }
    if (mode == g_hfp_feature_mode) {
        TASK_LOG_MSGID_E("hfp feature mode: no mode cahnge: %d", 1, mode);
        return false;
    }

    /* Bypass case 2: Check whether the HFP start up has been completed. */
    if (g_hfp_feature_mode_1st_notify == true) {
        TASK_LOG_MSGID_E("hfp feature mode: busy with start up, mode %d", 1, mode);
        return false;
    }

    /* Bypass case 4: Check whether previous user is busy on setting the mode */
    hal_nvic_save_and_set_interrupt_mask(&irq_status);
    if (g_hfp_feature_mode_busy_flg == true) {
        hal_nvic_restore_interrupt_mask(irq_status);
        TASK_LOG_MSGID_E("hfp feature mode: busy with previous mode change, mode %d", 1, g_hfp_feature_mode);
        return false;
    }
    g_hfp_feature_mode_busy_flg = true;
    hal_nvic_restore_interrupt_mask(irq_status);

    /* update call mode */
    g_hfp_feature_mode = mode;
    TASK_LOG_MSGID_I("hfp feature mode: set mode to %d", 1, g_hfp_feature_mode);

    /* If it's not in HFP scenario, just save the current mode for next HFP open. */
    if ((g_prCurrent_player == NULL) || (g_prCurrent_player->type != HFP)) {
        TASK_LOG_MSGID_I("hfp feature mode: not in HFP scenario", 0);
        g_hfp_feature_mode_busy_flg = false;
        return true;
    }

    am_status = am_audio_set_resume();
    if (am_status != AUD_EXECUTION_SUCCESS) {
        TASK_LOG_MSGID_E("hfp feature mode: resume fail", 0);
        g_hfp_feature_mode_busy_flg = false;
        return false;
    }

    /* wait for audio nvkey sync to DSP */
    if (pdTRUE != xSemaphoreTake(g_xSemaphore_hfp_nvkey_change_notify, 1000 / portTICK_PERIOD_MS)) {
        TASK_LOG_MSGID_E("hfp feature mode: wait DSP timeout", 0);
        g_hfp_feature_mode_busy_flg = false;
        return false;
    }
    g_hfp_feature_mode_busy_flg = false;
    TASK_LOG_MSGID_I("hfp feature mode: set mode done", 0);

    return true;
}

void hfp_replace_feature_mode_nvkey_id(bt_hfp_audio_codec_type_t codec_type, DSP_FEATURE_TYPE_LIST *p_feature_list)
{
#ifdef AIR_PSAP_ENABLE
    if (hfp_get_feature_mode() == HFP_FEATURE_MODE_PSAP_0) {
        if (codec_type == BT_HFP_CODEC_TYPE_CVSD) {
            p_feature_list[0] = FUNC_PSAP_HFP_NB_1;
        } else {
            p_feature_list[0] = FUNC_PSAP_HFP_WB_1;
        }
        TASK_LOG_MSGID_I("hfp feature mode: PSAP %d", 1, hfp_get_feature_mode());
    }
#endif
#ifdef AIR_AUDIO_DETACHABLE_MIC_ENABLE
    voice_mic_type_t mic_cur_type = hal_audio_query_voice_mic_type();
    if (mic_cur_type == VOICE_MIC_TYPE_DETACHABLE) {
        if (codec_type == BT_HFP_CODEC_TYPE_CVSD) {
            AudioFeatureList_eSCO[0] = FUNC_NB_BOOM_MIC;
        } else {
            AudioFeatureList_eSCO[0] = FUNC_WB_BOOM_MIC;
        }
        TASK_LOG_MSGID_I("hfp feature mode: boom mic %d", 1, mic_cur_type);
    }
#endif
}

uint16_t hfp_restore_feature_mode_nvkey_id(uint16_t nvkey_id)
{
    uint16_t repalce_nvkey_id;

    repalce_nvkey_id = nvkey_id;
    if (g_prCurrent_player->type == HFP) {
#ifdef AIR_PSAP_ENABLE
        if (nvkey_id == NVKEY_DSP_PARA_NB_RX_EQ_2ND) {
            repalce_nvkey_id = NVKEY_DSP_PARA_NB_RX_EQ;
        } else if (nvkey_id == NVKEY_DSP_PARA_WB_RX_EQ_2ND) {
            repalce_nvkey_id = NVKEY_DSP_PARA_WB_RX_EQ;
        } else if (nvkey_id == NVKEY_DSP_PARA_POSITIVE_GAIN_2ND) {
            repalce_nvkey_id = NVKEY_DSP_PARA_POSITIVE_GAIN;
        }
#endif
#ifdef AIR_AUDIO_DETACHABLE_MIC_ENABLE
        if (nvkey_id == NVKEY_DSP_PARA_AEC_BOOMIC) {
            repalce_nvkey_id = NVKEY_DSP_PARA_AEC;
        } if (nvkey_id == NVKEY_DSP_PARA_NR_BOOMIC) {
            repalce_nvkey_id = NVKEY_DSP_PARA_NR;
        } if (nvkey_id == NVKEY_DSP_PARA_INEAR_BOOMIC) {
            repalce_nvkey_id = NVKEY_DSP_PARA_INEAR;
        } else if (nvkey_id == NVKEY_DSP_PARA_NB_TX_EQ_BOOMIC) {
            repalce_nvkey_id = NVKEY_DSP_PARA_NB_TX_EQ;
        } else if (nvkey_id == NVKEY_DSP_PARA_WB_TX_EQ_BOOMIC) {
            repalce_nvkey_id = NVKEY_DSP_PARA_WB_TX_EQ;
        } else if (nvkey_id == NVKEY_DSP_PARA_WB_TX_VO_CPD_BOOMIC) {
            repalce_nvkey_id = NVKEY_DSP_PARA_WB_TX_VO_CPD;
        } else if (nvkey_id == NVKEY_DSP_PARA_NB_TX_FIR_EQ_BOOMIC) {
            repalce_nvkey_id = NVKEY_DSP_PARA_NB_TX_FIR_EQ;
        } else if (nvkey_id == NVKEY_DSP_PARA_WB_TX_FIR_EQ_BOOMIC) {
            repalce_nvkey_id = NVKEY_DSP_PARA_WB_TX_FIR_EQ;
        }
#endif
    }

    return repalce_nvkey_id;
}

#endif

static bt_codec_media_status_t bt_hfp_open(bt_hfp_audio_codec_type_t codec_type)
{
    mcu2dsp_open_param_t open_param;
    void *p_param_share;
    sysram_status_t status;
    DSP_FEATURE_TYPE_LIST AudioFeatureList_eSCO[2];

    if (codec_type == BT_HFP_CODEC_TYPE_CVSD) {
        AudioFeatureList_eSCO[0] = FUNC_TX_NR;
    } else {
        AudioFeatureList_eSCO[0] = FUNC_RX_NR;
    }
    AudioFeatureList_eSCO[1] = FUNC_END;

#ifdef AIR_HFP_FEATURE_MODE_ENABLE
    hfp_replace_feature_mode_nvkey_id(codec_type, AudioFeatureList_eSCO);
#endif

    /* reset share buffer before put parameters*/
    audio_nvdm_reset_sysram();
    status = audio_nvdm_set_feature(2, AudioFeatureList_eSCO);
    if (status != NVDM_STATUS_NAT_OK) {
        TASK_LOG_MSGID_E("failed to set parameters to share memory - err(%d)\r\n", 1, status);
        AUDIO_ASSERT(0);
    }

#ifdef MTK_ANC_ENABLE
    audio_set_anc_compensate(HFP, 0, NULL);
#endif

#if defined(HAL_DVFS_MODULE_ENABLED)
    hal_audio_set_dvfs_clk(AU_DSP_VOICE, &g_hfp_dvfs_clk);
    TASK_LOG_MSGID_I("[HFP][open] hal_audio_set_dvfs_clk: %d", 1, g_hfp_dvfs_clk);
    if ((gDspEcnrAlg_MicType == ECNR_DUAL_MIC) && (g_hfp_dvfs_clk < HFP_DVFS_INHOUSE_2MIC_SPEED)) {
        g_hfp_dvfs_clk = HFP_DVFS_INHOUSE_2MIC_SPEED;
        TASK_LOG_MSGID_I("[HFP][open] ECNR dual mic algorithm, dvfs lock to: %d", 1, g_hfp_dvfs_clk);
    }

#if defined(AIR_ECNR_1MIC_INEAR_ENABLE)
    if (g_hfp_dvfs_clk < HFP_DVFS_INHOUSE_1MIC_INEAR_SPEED) {
        g_hfp_dvfs_clk = HFP_DVFS_INHOUSE_1MIC_INEAR_SPEED;
    }
    TASK_LOG_MSGID_I("[HFP][open] INEAR algorithm, dvfs lock to: %d", 1, g_hfp_dvfs_clk);
#elif defined(AIR_ECNR_2MIC_INEAR_ENABLE)
    if (g_hfp_dvfs_clk < HFP_DVFS_INHOUSE_2MIC_INEAR_SPEED) {
        g_hfp_dvfs_clk = HFP_DVFS_INHOUSE_2MIC_INEAR_SPEED;
    }
    TASK_LOG_MSGID_I("[HFP][open] Dualmic INEAR algorithm, dvfs lock to: %d", 1, g_hfp_dvfs_clk);
#elif defined(AIR_3RD_PARTY_NR_ENABLE)
    if (g_hfp_dvfs_clk < HFP_DVFS_3RD_PARTY_NR_SPEED) {
        g_hfp_dvfs_clk = HFP_DVFS_3RD_PARTY_NR_SPEED;
    }
    TASK_LOG_MSGID_I("[HFP][open] 3rd Party NR algorithm, dvfs lock to: %d", 1, g_hfp_dvfs_clk);
#endif

#if defined(AIR_HFP_DNN_PATH_ENABLE)
    if (g_hfp_dvfs_clk < HFP_DVFS_MAX_SPEED) {
        g_hfp_dvfs_clk = HFP_DVFS_MAX_SPEED;
    }
    TASK_LOG_MSGID_I("[HFP][open] DNN path enable, dvfs lock to: %d", 1, g_hfp_dvfs_clk);
#endif

    if ((g_hfp_dvfs_clk > HFP_DVFS_DEFAULT_SPEED) && (g_hfp_dvfs_clk <= HFP_DVFS_MAX_SPEED)) {
        hal_dvfs_lock_control(g_hfp_dvfs_clk, HAL_DVFS_LOCK);
        TASK_LOG_MSGID_I("[HFP][open] dvfs lock to step: %d", 1, g_hfp_dvfs_clk);
    }
#endif

    memset(&open_param, 0, sizeof(mcu2dsp_open_param_t));

    open_param.param.stream_in  = STREAM_IN_HFP;
    open_param.param.stream_out = STREAM_OUT_AFE;
    open_param.audio_scenario_type = AUDIO_SCENARIO_TYPE_HFP_DL;

    open_param.stream_in_param.hfp.codec_type     = codec_type;
    open_param.stream_in_param.hfp.p_share_info   = (avm_share_buf_info_t *)(hal_audio_query_bt_voice_dl_share_info());

    open_param.stream_in_param.hfp.bt_inf_address = bt_sink_srv_ami_get_bt_inf_address();
    open_param.stream_in_param.hfp.clk_info_address   = hal_audio_query_rcdc_share_info();
#ifdef AIR_AIRDUMP_ENABLE
    open_param.stream_in_param.hfp.p_air_dump_buf   = hal_audio_query_hfp_air_dump();
#endif

    if (codec_type == BT_HFP_CODEC_TYPE_CVSD) {
        hal_audio_set_hfp_avm_info(open_param.stream_in_param.hfp.p_share_info, HFP_SHARE_BUF_PARTTERN_TOTAL_SIZE, CVSD_SHARE_BUF_FRAME_SIZE);
    } else {
        hal_audio_set_hfp_avm_info(open_param.stream_in_param.hfp.p_share_info, HFP_SHARE_BUF_PARTTERN_TOTAL_SIZE, MSBC_SHARE_BUF_FRAME_SIZE);
    }

    TASK_LOG_MSGID_I("mcuDL Codec type:%d, shareP:0x%x, rx forwarder:0x%x\r\n", 2, open_param.stream_in_param.hfp.codec_type, open_param.stream_in_param.hfp.p_share_info->StartAddr, open_param.stream_in_param.hfp.p_share_info->ForwarderAddr);

    hal_audio_get_stream_out_setting_config(AU_DSP_VOICE, &open_param.stream_out_param);
    TASK_LOG_MSGID_I("out_device(0x%x), channel(%d), interface(%d)", 3, open_param.stream_out_param.afe.audio_device, open_param.stream_out_param.afe.stream_channel, open_param.stream_out_param.afe.audio_interface);

    open_param.stream_out_param.afe.memory          = HAL_AUDIO_MEM1;
    open_param.stream_out_param.afe.format          = AFE_PCM_FORMAT_S16_LE;
    open_param.stream_out_param.afe.stream_out_sampling_rate   = 16000;
#if defined (FIXED_SAMPLING_RATE_TO_48KHZ)
    open_param.stream_out_param.afe.sampling_rate   = HAL_AUDIO_FIXED_AFE_48K_SAMPLE_RATE;
#elif defined (AIR_FIXED_DL_SAMPLING_RATE_TO_96KHZ)
    open_param.stream_out_param.afe.sampling_rate   = HAL_AUDIO_FIXED_AFE_96K_SAMPLE_RATE;
#else
    open_param.stream_out_param.afe.sampling_rate   = 16000;
#endif
    open_param.stream_out_param.afe.irq_period      = 15;
    open_param.stream_out_param.afe.frame_size      = 240;
    open_param.stream_out_param.afe.frame_number    = 4;
    open_param.stream_out_param.afe.hw_gain         = true;
#ifdef ENABLE_HWSRC_CLKSKEW
#if (PRODUCT_VERSION == 1565)
    open_param.stream_out_param.afe.clkskew_mode    = CLK_SKEW_V2;
#else
    open_param.stream_out_param.afe.clkskew_mode    = CLK_SKEW_V1;
#endif
#if defined AIR_HWSRC_TX_TRACKING_ENABLE || defined AIR_HWSRC_RX_TRACKING_ENABLE
    open_param.stream_out_param.afe.clkskew_mode    = CLK_SKEW_V1;
#endif
    TASK_LOG_MSGID_I("[HFP][MCU] open_param.stream_out_param.afe.clkskew_mode:%d", 1, open_param.stream_out_param.afe.clkskew_mode);
#endif
#ifdef AIR_HFP_DNN_PATH_ENABLE
    open_param.stream_out_param.afe.enable_ul_dnn   = true;
#endif
#if defined(MTK_EXTERNAL_DSP_NEED_SUPPORT)
    ami_set_afe_param(STREAM_OUT, HAL_AUDIO_SAMPLING_RATE_16KHZ, true);
#endif

    /* Turn on the power and set the volume */
    ami_hal_audio_status_set_running_flag(AUDIO_SCENARIO_TYPE_HFP_DL, &open_param, true);

#if (DL_ONLY == TRUE)
    p_param_share = hal_audio_dsp_controller_put_paramter(&open_param, sizeof(mcu2dsp_open_param_t), AUDIO_MESSAGE_TYPE_BT_VOICE_DL);
    if (codec_type == BT_HFP_CODEC_TYPE_CVSD) {
        hal_audio_dsp_controller_send_message(MSG_MCU2DSP_BT_VOICE_DL_OPEN, AUDIO_DSP_CODEC_TYPE_CVSD, (uint32_t)p_param_share, true);
    } else {
        hal_audio_dsp_controller_send_message(MSG_MCU2DSP_BT_VOICE_DL_OPEN, AUDIO_DSP_CODEC_TYPE_MSBC, (uint32_t)p_param_share, true);
    }
#endif


#ifdef AIR_HFP_DNN_PATH_ENABLE
#if (UL_DNN_EN == TRUE)
    open_param.param.stream_in  = STREAM_IN_AFE;
    open_param.param.stream_out = STREAM_OUT_AFE;
    open_param.audio_scenario_type = AUDIO_SCENARIO_TYPE_HFP_UL;
    hal_audio_get_stream_in_setting_config(AU_DSP_VOICE, &open_param.stream_in_param);
    open_param.stream_in_param.afe.audio_device = HAL_AUDIO_DEVICE_I2S_MASTER_R;

    open_param.stream_in_param.afe.memory          = HAL_AUDIO_MEM_SUB;
    //open_param.stream_in_param.afe.format          = AFE_PCM_FORMAT_S16_LE; //AFE_PCM_FORMAT_S32_LE
    open_param.stream_in_param.afe.format          = AFE_PCM_FORMAT_S32_LE;
    open_param.stream_in_param.afe.sampling_rate   = 16000;
    open_param.stream_in_param.afe.irq_period      = 15;
    open_param.stream_in_param.afe.frame_size      = 240;
    open_param.stream_in_param.afe.frame_number    = 6;
    open_param.stream_in_param.afe.hw_gain         = false;
    open_param.stream_in_param.afe.enable_ul_dnn   = true;

    hal_audio_get_stream_out_setting_config(AU_DSP_VOICE, &open_param.stream_out_param);
    open_param.stream_out_param.afe.audio_device = HAL_AUDIO_DEVICE_I2S_MASTER_R;
    open_param.stream_out_param.afe.memory          = HAL_AUDIO_MEM3;
    open_param.stream_out_param.afe.format          = AFE_PCM_FORMAT_S16_LE; //AFE_PCM_FORMAT_S32_LE
    open_param.stream_out_param.afe.stream_out_sampling_rate   = 16000;
    open_param.stream_out_param.afe.sampling_rate   = 16000;
    open_param.stream_out_param.afe.irq_period      = 15;
    open_param.stream_out_param.afe.frame_size      = 240;
    open_param.stream_out_param.afe.frame_number    = 4;
    open_param.stream_out_param.afe.hw_gain         = false;
    open_param.stream_out_param.afe.clkskew_mode    = CLK_SKEW_V1;
    open_param.stream_out_param.afe.enable_ul_dnn   = true;

    p_param_share = hal_audio_dsp_controller_put_paramter(&open_param, sizeof(mcu2dsp_open_param_t), AUDIO_MESSAGE_TYPE_BT_VOICE_UL);
    hal_audio_dsp_controller_send_message(MSG_MCU2DSP_BT_VOICE_UL_OPEN, AUDIO_DSP_CODEC_TYPE_DNN, (uint32_t)p_param_share, true);
#endif
#endif


    /* Set the streaming for Uplink */
    memset(&open_param, 0, sizeof(mcu2dsp_open_param_t));

    open_param.param.stream_in  = STREAM_IN_AFE;
    open_param.param.stream_out = STREAM_OUT_HFP;
    open_param.audio_scenario_type = AUDIO_SCENARIO_TYPE_HFP_UL;

    open_param.stream_out_param.hfp.codec_type  = codec_type;

    open_param.stream_out_param.hfp.p_share_info = (avm_share_buf_info_t *)hal_audio_query_bt_voice_ul_share_info();
    if (codec_type == BT_HFP_CODEC_TYPE_CVSD) {
        hal_audio_set_hfp_avm_info(open_param.stream_out_param.hfp.p_share_info, HFP_SHARE_BUF_PARTTERN_TOTAL_SIZE, CVSD_SHARE_BUF_FRAME_SIZE);
    } else {
        hal_audio_set_hfp_avm_info(open_param.stream_out_param.hfp.p_share_info, HFP_SHARE_BUF_PARTTERN_TOTAL_SIZE, MSBC_SHARE_BUF_FRAME_SIZE);
    }
    TASK_LOG_MSGID_I("mcuUL Codec type:%d, shareP:0x%x, tx forwarder:0x%x\r\n", 2, open_param.stream_out_param.hfp.codec_type, open_param.stream_out_param.hfp.p_share_info->StartAddr, open_param.stream_out_param.hfp.p_share_info->ForwarderAddr);

#ifdef MTK_AWS_MCE_ENABLE
    bt_aws_mce_report_register_callback(BT_AWS_MCE_REPORT_MODULE_HFP_AVC, am_hfp_ndvc_callback);
#endif

    hal_audio_get_stream_in_setting_config(AU_DSP_VOICE, &open_param.stream_in_param);

#ifdef AIR_HFP_DNN_PATH_ENABLE
    open_param.stream_in_param.afe.audio_device = HAL_AUDIO_DEVICE_I2S_MASTER_L;
    open_param.stream_in_param.afe.memory          = HAL_AUDIO_MEM1;
#else
    open_param.stream_in_param.afe.memory          = HAL_AUDIO_MEM1 | HAL_AUDIO_MEM3;//HAL_AUDIO_MEM3 to enable echo referencr;
#endif
    TASK_LOG_MSGID_I("in_device(0x%x), channel(%d), interface(%d)", 3, open_param.stream_in_param.afe.audio_device, open_param.stream_in_param.afe.stream_channel, open_param.stream_in_param.afe.audio_interface);

#ifdef AIR_HFP_DNN_PATH_ENABLE
    open_param.stream_in_param.afe.format          = AFE_PCM_FORMAT_S32_LE;
#else
    open_param.stream_in_param.afe.format          = AFE_PCM_FORMAT_S16_LE;
#endif
#if defined(AIR_UL_FIX_SAMPLING_RATE_48K)
    open_param.stream_in_param.afe.sampling_rate   = 48000;
    open_param.stream_in_param.afe.frame_size      = 240 * 3;
#else
    open_param.stream_in_param.afe.sampling_rate   = 16000;
    open_param.stream_in_param.afe.frame_size      = 240;
#endif
    open_param.stream_in_param.afe.irq_period      = 15;
    open_param.stream_in_param.afe.frame_number    = 3;
    open_param.stream_in_param.afe.hw_gain         = false;
#ifdef AIR_HFP_DNN_PATH_ENABLE
    open_param.stream_in_param.afe.enable_ul_dnn   = true;
#endif
    if (open_param.stream_in_param.afe.audio_device == HAL_AUDIO_DEVICE_I2S_SLAVE) {
#ifdef AIR_HWSRC_RX_TRACKING_ENABLE
        open_param.stream_in_param.afe.sampling_rate   = 16000;
        open_param.stream_in_param.afe.stream_out_sampling_rate   = 16000;
#endif
#ifdef AIR_DUAL_CHIP_I2S_ENABLE
        open_param.stream_in_param.afe.memory          = HAL_AUDIO_MEM1;//remove HAL_AUDIO_MEM3 to enable echo reference from i2s slave;
#endif
    }
#if 0
    if (open_param.stream_in_param.afe.audio_device & (HAL_AUDIO_DEVICE_I2S_MASTER | HAL_AUDIO_DEVICE_I2S_MASTER_L | HAL_AUDIO_DEVICE_I2S_MASTER_R)) {
        //for esco UL Downsample
        open_param.stream_in_param.afe.with_upwdown_sampler     = true;
        open_param.stream_in_param.afe.audio_path_input_rate    = open_param.stream_in_param.afe.i2s_master_sampling_rate;
        open_param.stream_in_param.afe.audio_path_output_rate   = open_param.stream_in_param.afe.sampling_rate;
    }
#endif
    /* Turn on the power */
    ami_hal_audio_status_set_running_flag(AUDIO_SCENARIO_TYPE_HFP_UL, &open_param, true);

#if (UL_ONLY == TRUE)
    p_param_share = hal_audio_dsp_controller_put_paramter(&open_param, sizeof(mcu2dsp_open_param_t), AUDIO_MESSAGE_TYPE_BT_VOICE_UL);
    if (codec_type == BT_HFP_CODEC_TYPE_CVSD) {
        hal_audio_dsp_controller_send_message(MSG_MCU2DSP_BT_VOICE_UL_OPEN, AUDIO_DSP_CODEC_TYPE_CVSD, (uint32_t)p_param_share, true);
    } else {
        hal_audio_dsp_controller_send_message(MSG_MCU2DSP_BT_VOICE_UL_OPEN, AUDIO_DSP_CODEC_TYPE_MSBC, (uint32_t)p_param_share, true);
    }
#endif

    return BT_CODEC_MEDIA_STATUS_OK;
}

static void bt_hfp_isr_handler(hal_audio_event_t event, void *data)
{
    bt_hfp_codec_internal_handle_t *internal_handle = (bt_hfp_codec_internal_handle_t *)data;
    bt_media_handle_t *handle = (bt_media_handle_t *)internal_handle;
    if (event == HAL_AUDIO_EVENT_HFP_PARA_SEND) {
        handle->handler(handle, BT_CODEC_MEDIA_HFP_AVC_PARA_SEND);
    }
}

static bt_codec_media_status_t bt_hfp_play(bt_media_handle_t *handle)
{
    bt_hfp_codec_internal_handle_t *internal_handle = (bt_hfp_codec_internal_handle_t *)handle;
    bool aws_flag = internal_handle->aws_flag;

    hal_audio_service_hook_callback(AUDIO_MESSAGE_TYPE_BT_VOICE_DL, bt_hfp_isr_handler, handle);
    hal_audio_service_hook_callback(AUDIO_MESSAGE_TYPE_BT_VOICE_UL, bt_hfp_isr_handler, handle);

    mcu2dsp_start_param_t start_param;
    void *p_param_share;

    // Collect parameters
    start_param.param.stream_in     = STREAM_IN_HFP;
    start_param.param.stream_out    = STREAM_OUT_AFE;

    start_param.stream_out_param.afe.aws_flag   =  aws_flag;
#if defined AIR_DUAL_CHIP_MIXING_MODE_ROLE_MASTER_ENABLE || defined AIR_DUAL_CHIP_MIXING_MODE_ROLE_SLAVE_ENABLE
    mcu_dchs_cosys_ctrl_cmd_relay(AUDIO_UART_COSYS_DL_START, AUDIO_SCENARIO_TYPE_HFP_DL , NULL, &start_param);
#endif
    p_param_share = hal_audio_dsp_controller_put_paramter(&start_param, sizeof(mcu2dsp_start_param_t), AUDIO_MESSAGE_TYPE_BT_VOICE_DL);
#if (DL_ONLY == TRUE)
    hal_audio_dsp_controller_send_message(MSG_MCU2DSP_BT_VOICE_DL_START, 0, (uint32_t)p_param_share, true);
#endif

    // Collect parameters
    start_param.param.stream_in     = STREAM_IN_AFE;
    start_param.param.stream_out    = STREAM_OUT_HFP;

    start_param.stream_in_param.afe.aws_flag   =  aws_flag;

    p_param_share = hal_audio_dsp_controller_put_paramter(&start_param, sizeof(mcu2dsp_start_param_t), AUDIO_MESSAGE_TYPE_BT_VOICE_UL);
#if (UL_ONLY == TRUE)
    hal_audio_dsp_controller_send_message(MSG_MCU2DSP_BT_VOICE_UL_START, 0, (uint32_t)p_param_share, true);
#endif

#ifdef AIR_HFP_DNN_PATH_ENABLE
#if (UL_DNN_EN == TRUE)
    start_param.param.stream_in     = STREAM_IN_AFE;
    start_param.param.stream_out    = STREAM_OUT_AFE;
    hal_audio_dsp_controller_send_message(MSG_MCU2DSP_BT_VOICE_UL_START, AUDIO_DSP_CODEC_TYPE_DNN, 0, true);
#endif
#endif

#ifdef MTK_VENDOR_SOUND_EFFECT_ENABLE
#ifdef MTK_VENDOR_SOUND_EFFECT_EXTENSION_ENABLE
    ami_execute_vendor_se(EVENT_AFTER_HFP_START);
#endif
#endif

    handle->state = BT_CODEC_STATE_PLAY;

    return BT_CODEC_MEDIA_STATUS_OK;
}

static bt_codec_media_status_t bt_hfp_stop(bt_media_handle_t *handle)
{
#ifdef MTK_VENDOR_SOUND_EFFECT_ENABLE
#ifdef MTK_VENDOR_SOUND_EFFECT_EXTENSION_ENABLE
    ami_execute_vendor_se(EVENT_BEFORE_HFP_STOP);
#endif
#endif
#if (DL_ONLY == TRUE)
    hal_audio_dsp_controller_send_message(MSG_MCU2DSP_BT_VOICE_DL_STOP, 0, 0, true);
#endif
#if (UL_ONLY == TRUE)
    hal_audio_dsp_controller_send_message(MSG_MCU2DSP_BT_VOICE_UL_STOP, 0, 0, true);
#endif
#ifdef AIR_HFP_DNN_PATH_ENABLE
#if (UL_DNN_EN == TRUE)
    hal_audio_dsp_controller_send_message(MSG_MCU2DSP_BT_VOICE_UL_STOP, AUDIO_DSP_CODEC_TYPE_DNN, 0, true);
#endif
#endif /*AIR_HFP_DNN_PATH_ENABLE*/
    hal_audio_service_unhook_callback(AUDIO_MESSAGE_TYPE_BT_VOICE_DL);
    hal_audio_service_unhook_callback(AUDIO_MESSAGE_TYPE_BT_VOICE_UL);

    handle->state = BT_CODEC_STATE_STOP;
#ifdef HAL_DVFS_MODULE_ENABLED
    if (g_hfp_dvfs_clk != HFP_DVFS_DEFAULT_SPEED) {
        hal_dvfs_lock_control(g_hfp_dvfs_clk, HAL_DVFS_UNLOCK);
        TASK_LOG_MSGID_I("[HFP][stop] dvfs unlock from:%d", 1, g_hfp_dvfs_clk);
    }
#endif

    return BT_CODEC_MEDIA_STATUS_OK;
}

static bt_media_handle_t *bt_hfp_open_codec_internal(bt_hfp_audio_codec_type_t codec_type, bt_hfp_mode_t mode)
{
    bt_hfp_codec_internal_handle_t *p_info;
    bt_media_handle_t *handle;

    TASK_LOG_MSGID_I("+open\r\n", 0);

    if ((mode != BT_HFP_MODE_SPEECH) && (mode != BT_HFP_MODE_LOOPBACK_WITH_CODEC)) {
        TASK_LOG_MSGID_E("bt_hfp_open_codec_internal: bt hfp mode error %d\r\n", 1, mode);
        return NULL;
    }

    p_info = (bt_hfp_codec_internal_handle_t *)pvPortMalloc(sizeof(bt_hfp_codec_internal_handle_t));
    if (p_info == NULL) {
        TASK_LOG_MSGID_E("bt_hfp_open_codec_internal: p_info allocate fail\r\n", 0);
        return NULL;
    }

    memset(p_info, 0, sizeof(bt_hfp_codec_internal_handle_t));
    handle = &p_info->handle;
    handle->play    = bt_hfp_play;
    handle->stop    = bt_hfp_stop;
    handle->process = bt_hfp_process;
    handle->set_aws_flag = bt_set_aws_flag;
    handle->trigger_mic = bt_trigger_mic;
    handle->side_tone_enable = bt_side_tone_enable;
    handle->side_tone_disable = bt_side_tone_disable;
    bt_hfp_codec_internal_handle = p_info;
    bt_hfp_codec_is_running = true;
    p_info->codec_type  = codec_type;
    p_info->mode        = mode;

    audio_nvdm_update_status(AUDIO_NVDM_USER_HFP, AUDIO_NVDM_STATUS_CHANGE);

    if (bt_hfp_open(codec_type) == BT_CODEC_MEDIA_STATUS_ERROR) {
        TASK_LOG_MSGID_E("bt_hfp_open_codec_internal: bt_hfp_open fail\r\n", 0);
        vPortFree(p_info);
        return NULL;
    }

    handle->state = BT_CODEC_STATE_READY;

    TASK_LOG_MSGID_I("-open\r\n", 0);

    return handle;
}

static bt_codec_media_status_t bt_hfp_close_codec_internal(void)
{
    bt_hfp_codec_internal_handle_t *p_info = bt_hfp_codec_internal_handle;

    TASK_LOG_MSGID_I("+close\r\n", 0);

    if (p_info == NULL) {
        TASK_LOG_MSGID_E("bt_hfp_close_codec_internal: p_info is NULL\r\n", 0);
        return BT_CODEC_MEDIA_STATUS_ERROR;
    }
    if ((p_info->handle.state != BT_CODEC_STATE_STOP) ||
        ((p_info->mode != BT_HFP_MODE_SPEECH) && (p_info->mode != BT_HFP_MODE_LOOPBACK_WITH_CODEC))) {
        TASK_LOG_MSGID_E("bt_hfp_close_codec_internal: status %d or mode %d is invalid\r\n", 2, p_info->handle.state, p_info->mode);
        return BT_CODEC_MEDIA_STATUS_ERROR;
    }

#if (UL_ONLY == TRUE)
    hal_audio_dsp_controller_send_message(MSG_MCU2DSP_BT_VOICE_UL_CLOSE, 0, 0, true);
#endif
#if (DL_ONLY == TRUE)
    hal_audio_dsp_controller_send_message(MSG_MCU2DSP_BT_VOICE_DL_CLOSE, 0, 0, true);
#endif
#ifdef AIR_HFP_DNN_PATH_ENABLE
#if (UL_DNN_EN == TRUE)
    hal_audio_dsp_controller_send_message(MSG_MCU2DSP_BT_VOICE_UL_CLOSE, AUDIO_DSP_CODEC_TYPE_DNN, 0, true);
#endif
#endif /*AIR_HFP_DNN_PATH_ENABLE*/

    p_info->handle.state = BT_CODEC_STATE_IDLE;
    vPortFree(p_info);

    bt_hfp_codec_is_running = false;

    TASK_LOG_MSGID_I("-close\r\n", 0);

    return BT_CODEC_MEDIA_STATUS_OK;
}

bt_media_handle_t *bt_codec_hfp_open(bt_codec_hfp_callback_t callback, const bt_codec_hfp_audio_t *param)
{
    bt_media_handle_t *handle;

    if ((param == NULL) || ((param->type != BT_HFP_CODEC_TYPE_CVSD) && (param->type != BT_HFP_CODEC_TYPE_MSBC))) {
        TASK_LOG_MSGID_E("bt_codec_hfp_open: param NULL or error\r\n", 0);
        return NULL;
    }

    TASK_LOG_MSGID_I("open_codec, codec type:%d\r\n", 1, param->type);

    handle = bt_hfp_open_codec_internal(param->type, BT_HFP_MODE_SPEECH);
    handle->handler = callback;

    return handle;
}

bt_codec_media_status_t bt_codec_hfp_close(bt_media_handle_t *handle)
{
    bt_codec_media_status_t status;

    TASK_LOG_MSGID_I("close_codec\r\n", 0);

#ifdef RACE_DSP_REALTIME_CMD_ENABLE
#ifdef AIR_AIRDUMP_ENABLE
    airdump_timer_delete();
#endif
#endif

    status = bt_hfp_close_codec_internal();

    return status;
}

