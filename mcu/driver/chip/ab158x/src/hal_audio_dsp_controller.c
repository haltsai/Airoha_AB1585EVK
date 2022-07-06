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

#include <string.h>
#include "hal_audio.h"
#include <assert.h>
#if defined(HAL_AUDIO_MODULE_ENABLED)

//==== Include header files ====
#include "memory_attribute.h"
#include "hal_resource_assignment.h"
#include "hal_ccni.h"
#include "hal_ccni_config.h"
#include "hal_audio_cm4_dsp_message.h"
#include "hal_audio_message_struct.h"
#include "hal_audio_message_struct_common.h"
#include "hal_audio_internal.h"
#include "hal_audio_internal_nvkey_struct.h"
#include "hal_rtc_internal.h"
#include "hal_hw_semaphore.h"
#include "hal_nvic.h"
#include "hal_clock_internal.h"
#include "assert.h"
#include "hal_log.h"

#include "hal_gpt.h"
#include "hal_core_status.h"
#include "hal_clock.h"
#ifdef HAL_PMU_MODULE_ENABLED
#include "hal_pmu.h"
#endif
#include "hal_dvfs.h"
#if defined(HAL_SLEEP_MANAGER_ENABLED)
#include "hal_core_status.h"
#include "hal_spm.h"
#include "memory_map.h"
#endif

#ifdef AIR_BT_CODEC_BLE_ENABLED
#include "avm_external.h"
#endif

#if defined AIR_DUAL_CHIP_MIXING_MODE_ROLE_MASTER_ENABLE || defined AIR_DUAL_CHIP_MIXING_MODE_ROLE_SLAVE_ENABLE
#include "scenario_dchs.h"
#endif

#include "exception_handler.h"
#define UNUSED(x)  ((void)(x))
#define HAL_AUDIO_DEBUG
//#define UNIT_TEST
#define AUDIO_HW_I2S_MASTER_BLOCK_NUMBER   (4)
//==== Static variables ====
ATTR_SHARE_ZIDATA static n9_dsp_share_info_t n9_dsp_share_info[SHARE_BUFFER_INFO_INDEX_MAX];
ATTR_SHARE_ZIDATA static audio_share_buffer_t audio_share_buffer;
ATTR_SHARE_ZIDATA static uint32_t audio_sync_request_parm[(SHARE_BUFFER_AUDIO_SYNC_REQUEST_PARAM_SIZE + 3) / 4];
#ifdef MTK_BT_SPEAKER_ENABLE
ATTR_SHARE_ZIDATA static uint8_t fec_share_buffer[SHARE_BUFFER_FEC_SIZE];
#endif

ATTR_SHARE_ZIDATA static audio_dsp_a2dp_dl_play_en_param_t audio_play_en_info;

ATTR_SHARE_ZIDATA static n9_dsp_share_info_t audio_transmitter_share_info[AUDIO_TRANSMITTER_SHARE_INFO_INDEX_MAX];

#if defined(AIR_ANC_USER_UNAWARE_ENABLE) || defined(AIR_ANC_WIND_DETECTION_ENABLE) || defined(AIR_ANC_ENVIRONMENT_DETECTION_ENABLE)
//#define SHARE_BUFFER_ANC_MONITOR_SIZE 5 //UU gain info: int32_t *5
ATTR_SHARE_ZIDATA static int32_t g_ANC_monitor_share_buffer[SHARE_BUFFER_ANC_MONITOR_INFO_COUNT];
#endif

/* ------------------------------------------------ Clock Gate Control ---------------------------------------------- */
uint32_t g_dsp_controller_cg[AUDIO_POWER_END][(AUDIO_SCEANRIO_TYPE_MAX + 31) / 32] = {0};
uint32_t g_dsp_controller_cg_control_bit[AUDIO_SCENARIO_TYPE_END] = {0}; // log the setting of each scenario type

#ifdef AIR_BT_CODEC_BLE_ENABLED
static leaudio_share_buffer_t leaudio_share_buffer;
#endif

typedef struct {
    uint32_t                           flag;
    hal_audio_callback_t               callback[AUDIO_MESSAGE_TYPE_MAX];
    void                               *user_data[AUDIO_MESSAGE_TYPE_MAX];
    hal_bt_audio_dl_open_callback_t    bt_audio_dl_open_callback;
    hal_audio_notify_task_callback_t   audio_event_task_callback;
    hal_audio_task_ms_delay_function_t task_ms_delay_func;
} audio_isr_t;

static audiosys_clkmux_control audio_clkmux_status[AUDIO_CLKMUX_BLOCK_NUM_OF_CLKMUX_BLOCK];

/** @brief Define hires clock frequency. */
typedef enum {
    DL_HIRES_NONE       = 0,
    DL_HIRES_96KHZ      = 1,
    DL_HIRES_192KHZ     = 2,
} hires_clock_frequency_dl_t;

typedef enum {
    HWSRC_HIRES_96KHZ      = 0,
    HWSRC_HIRES_192KHZ     = 1,
} hires_clock_frequency_hwsrc_t;

typedef enum {
    INTERNAL_BUS_HIRES_96KHZ      = 0,
    INTERNAL_BUS_HIRES_192KHZ     = 1,
} hires_clock_frequency_internal_bus_t;


typedef struct {
    audio_message_queue_t dsp_msg_queue;

    bool waiting;
    bool waiting_VP;
    bool waiting_RECORD;
    bool waiting_A2DP;
    bool waiting_ESCO;
    bool waiting_DAC_OFF;
    bool dsp_power_on;
    bool dac_hi_res_on;
    bool clk_mux_dac_192k_on;
    uint64_t i2s_master_low_jitter[AUDIO_HW_I2S_MASTER_BLOCK_NUMBER];   /**< Specifies the I2S0/1/2/3 Master Low jitter mode.*/
    bool hi_res_on_dl;                                                  /**< Specifies the hires ON/OFF status of downlink hires.*/
    bool hi_res_on_ul;                                                  /**< Specifies the hires ON/OFF status of uplink hires.*/
    bool hi_res_on_hwsrc;                                               /**< Specifies the hires ON/OFF status of hwsrc.*/
    bool hi_res_on_internalbus;                                         /**< Specifies the hires ON/OFF status of internalbus.*/
    hires_clock_frequency_dl_t clk_mux_hires_dl;                        /**< Specifies the hires clock frequency of downlink hires.*/
    hires_clock_frequency_hwsrc_t clk_mux_hires_hwsrc;                  /**< Specifies the hires clock frequency of hwsrc.*/
    hires_clock_frequency_internal_bus_t clk_mux_hires_internal_bus;    /**< Specifies the hires clock frequency of internal bus.*/
    bool flag_vp;                                                       /**< Specifies the ON/OFF status of VP.*/
    bool flag_dac;                                                      /**< Specifies the hires ON/OFF status of dac user.*/
    bool flag_ul_apll;                                                  /**< Specifies the hires ON/OFF status of ul apll.*/
    bool flag_dl_apll;                                                  /**< Specifies the hires ON/OFF status of dl apll.*/
    bool hi_res_top_speed;
    uint32_t running;
    uint16_t dsp_notify;
} dsp_controller_t;

static dsp_controller_t dsp_controller;
static audio_isr_t audio_isr;
static audio_dsp_a2dp_dl_time_param_t audio_sync_time_info;
static uint32_t dsp2mcu_data;
static uint32_t dsp2mcu_AUDIO_DL_ACL_data;
audio_dsp_leakage_compensation_report_param_t leakage_compensation_info;
static uint32_t hal_audio_status_get_clock_gate_setting(audio_scenario_type_t type);
extern afe_apll_source_t afe_get_apll_by_samplerate(uint32_t samplerate);
extern unsigned char *bt_pka_get_esco_forwarder_addr(uint16_t size);

#ifdef AIR_ADAPTIVE_EQ_ENABLE
audio_dsp_adaptive_eq_notify_t adaptive_eq_info;
#endif

//==== Private API ====

//== Delay function ==
static void hal_audio_delay_ms(uint32_t ms_duration)
{
    if (audio_isr.task_ms_delay_func) {
        audio_isr.task_ms_delay_func(ms_duration);
    } else {
        hal_gpt_delay_ms(ms_duration);
    }
}

//== Audio Service related ==
static void hal_audio_service_callback(audio_message_element_t *p_msg)
{
    audio_message_type_t type;
    hal_audio_event_t event = HAL_AUDIO_EVENT_NONE;
    uint16_t message16, data16;

    message16 = p_msg->message16;
    data16 = p_msg->data16;
    type = (message16 & MSG_TYPE_BASE_MASK) >> MSG_TYPE_SHIFT_BIT;
    if (type >= AUDIO_MESSAGE_TYPE_MAX) {
        log_hal_msgid_info("Hal_audio_service_callback_type_error", 0);
        assert(0);
        return;
    }

    switch (message16) {
        // Error report
        case MSG_DSP2MCU_BT_AUDIO_DL_ERROR:
        case MSG_DSP2MCU_BT_VOICE_UL_ERROR:
        case MSG_DSP2MCU_BT_VOICE_DL_ERROR:
        case MSG_DSP2MCU_PLAYBACK_ERROR:
        case MSG_DSP2MCU_RECORD_ERROR:
        case MSG_DSP2MCU_PROMPT_ERROR:
            if (data16 == DSP_ERROR_REPORT_END) {
                event = HAL_AUDIO_EVENT_END;
            } else {
                event = HAL_AUDIO_EVENT_ERROR;
            }
            break;
        // Ramp down ack
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_COMMON_SIDETONE_STOP):  //Not main stream type, need assign type
            type = AUDIO_MESSAGE_TYPE_SIDETONE;
            event = HAL_AUDIO_EVENT_END;
            break;
        // Data request
        case MSG_DSP2MCU_PLAYBACK_DATA_REQUEST:
        case MSG_DSP2MCU_PROMPT_DATA_REQUEST:
            event = HAL_AUDIO_EVENT_DATA_REQUEST;
            hal_audio_status_set_notify_flag(type, true);
            break;

        // Data notification
        case MSG_DSP2MCU_RECORD_DATA_NOTIFY:
            event = HAL_AUDIO_EVENT_DATA_NOTIFICATION;
            hal_audio_status_set_notify_flag(type, true);
            if (audio_isr.callback[type]) {
                void *user_data = audio_isr.user_data[type];
#ifdef MTK_USER_TRIGGER_FF_ENABLE
//                log_hal_msgid_info("[user_trigger_ff]hal_audio_service_callback, p_msg->message16=0x%x, p_msg->data16=%d, p_msg->data32=%d", 3, p_msg->message16, p_msg->data16, p_msg->data32);
                if (user_data) {
                    *((uint32_t *)user_data) = ((uint32_t)(p_msg->data16) << 16) | (((uint32_t)p_msg->data32) & 0xFFFF);
//                    log_hal_msgid_info("[user_trigger_ff]*((uint32_t *)user_data) = 0x%x", 1, *((uint32_t *)user_data));
                }
#endif
                audio_isr.callback[type](event, user_data);
                return;
            }
            break;

        case MSG_DSP2MCU_RECORD_WWE_VERSION:
            event = HAL_AUDIO_EVENT_WWE_VERSION;
            if (audio_isr.callback[type]) {
                //void *user_data = audio_isr.user_data[type];
                audio_isr.callback[type](event, (void *)p_msg->data32);
                return;
            }
            //hal_audio_status_set_notify_flag(type, true);
            break;

        case MSG_DSP2MCU_RECORD_WWD_NOTIFY:
            event = HAL_AUDIO_EVENT_WWD_NOTIFICATION;
            if (audio_isr.callback[type]) {
                //void *user_data = audio_isr.user_data[type];
                audio_isr.callback[type](event, (void *)p_msg->data32);
                return;
            }
            //hal_audio_status_set_notify_flag(type, true);
            break;

        case MSG_DSP2MCU_RECORD_DATA_ABORT_NOTIFY:
            event = HAL_AUDIO_EVENT_DATA_ABORT_NOTIFICATION;
            //hal_audio_status_set_notify_flag(type, true);
            break;

        case MSG_DSP2MCU_BT_AUDIO_DL_TIME_REPORT:
            event = HAL_AUDIO_EVENT_TIME_REPORT;
            memcpy(&audio_sync_time_info, (void *)p_msg->data32, sizeof(audio_dsp_a2dp_dl_time_param_t));
            break;

        case MSG_DSP2MCU_BT_AUDIO_DL_ALC_REQUEST:
            event = HAL_AUDIO_EVENT_ALC_REQUEST;
            dsp2mcu_AUDIO_DL_ACL_data = p_msg->data32;
            break;

        case MSG_DSP2MCU_BT_AUDIO_DL_LTCS_DATA_REPORT:
            event = HAL_AUDIO_EVENT_LTCS_REPORT;
            //log_hal_msgid_info("[HAL audio] LTCS debug: ASI Buf 0x%x \r\n", 1, (uint32_t)hal_audio_query_ltcs_asi_buf());
            //log_hal_msgid_info("[HAL audio] LTCS debug: MNGP Buf 0x%x \r\n", 1, (uint32_t)hal_audio_query_ltcs_min_gap_buf());
            break;

        case MSG_DSP2MCU_BT_AUDIO_DL_LTCS_TIMEOUT_REQUEST:
            event = HAL_AUDIO_EVENT_LTCS_TIMEOUT;
            break;

        case MSG_DSP2MCU_BT_AUDIO_DL_REINIT_REQUEST:
            dsp2mcu_data = p_msg->data32;
            event = HAL_AUDIO_EVENT_DL_REINIT_REQUEST;
            break;

        // Config
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_PROMPT_CONFIG):
            // To notify VP should be closed by APP
            event = HAL_AUDIO_EVENT_END;
            break;

        // Special case.
        // BT Audio DL open: async callback
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_BT_AUDIO_DL_OPEN):
            if (audio_isr.bt_audio_dl_open_callback) {
                audio_isr.bt_audio_dl_open_callback();
            }
            // Need not to notify, so we use return here.
            return;

        case MSG_DSP2MCU_COMMON_REQ_GET_AUDIO_FEATURE_PARAMETER:
            event = p_msg->data16;
            if (audio_isr.callback[type]) {
                void *user_data = audio_isr.user_data[type];
                *((uint32_t *)user_data) = p_msg->data32;
                audio_isr.callback[type](event, user_data);
            }
            return;
        case MSG_DSP2MCU_AVC_PARA_SEND:
            dsp2mcu_data = p_msg->data32;
            event = HAL_AUDIO_EVENT_HFP_PARA_SEND;
            break;
        case MSG_DSP2MCU_AUDIO_AMP:  //Not main stream type, need assign type
            type = AUDIO_MESSAGE_TYPE_AFE;
            if (data16) {
                event = HAL_AUDIO_EVENT_DATA_REQUEST;
            } else {
                event = HAL_AUDIO_EVENT_NONE;
            }
            break;
#ifdef MTK_ANC_ENABLE
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_COMMON_ANC_STOP):
        case MSG_DSP2MCU_COMMON_ANC_START_DONE:
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_COMMON_ANC_SET_VOLUME):
        case MSG_DSP2MCU_COMMON_AUDIO_ANC_SWITCH:
            type = AUDIO_MESSAGE_TYPE_ANC;
            if (audio_isr.callback[type]) {
                void *user_data = audio_isr.user_data[type];
                if (user_data != NULL) {
                    *((uint32_t *)user_data) = p_msg->data32;
                }
                audio_isr.callback[type](event, (void *)(uint32_t)message16);
            }
            return;
#endif
#if defined(MTK_ANC_SURROUND_MONITOR_ENABLE)
        case MSG_DSP2MCU_COMMON_AUDIO_ANC_ADAPTIVE:
            type = AUDIO_MESSAGE_TYPE_ADAPTIVE;
            if (audio_isr.callback[type]) {
                *(uint32_t *)(audio_isr.user_data[type]) = p_msg->data16;
                audio_isr.callback[type]((hal_audio_event_t)p_msg->data16, &p_msg->data32);
            }
            return;
#endif
#ifdef MTK_LEAKAGE_DETECTION_ENABLE
        case MSG_DSP2MCU_RECORD_LC_WZ_REPORT:
            log_hal_msgid_info("[RECORD_LC]cm4 received dsp report, data16:%d, data32:%d\r\n", 2, p_msg->data16, p_msg->data32);
            leakage_compensation_info.calibration_status = p_msg->data16;
            leakage_compensation_info.wz_set = (uint16_t)p_msg->data32;
            if (leakage_compensation_info.api_callback != NULL) {
                leakage_compensation_info.api_callback((uint16_t)p_msg->data32);
            }
            return;
#endif
        case MSG_DSP2MCU_AUDIO_TRANSMITTER_DATA_NOTIFY:
            event = HAL_AUDIO_EVENT_DATA_NOTIFICATION;
            type = AUDIO_MESSAGE_TYPE_AUDIO_TRANSMITTER;
            if (audio_isr.callback[type]) {
                uint32_t user_data[2];
                user_data[0] = (uint32_t)data16;
                user_data[1] = (uint32_t)p_msg->data32;
                audio_isr.callback[type](event, (void *)user_data);
            }
            return;
        case MSG_DSP2MCU_AUDIO_TRANSMITTER_DATA_DIRECT:
            event = HAL_AUDIO_EVENT_DATA_DIRECT;
            type = AUDIO_MESSAGE_TYPE_AUDIO_TRANSMITTER;
            if (audio_isr.callback[type]) {
                uint32_t user_data[2];
                user_data[0] = (uint32_t)data16;
                user_data[1] = (uint32_t)p_msg->data32;
                audio_isr.callback[type](event, (void *)user_data);
            }
            return;
#ifdef MTK_SLT_AUDIO_HW
        case MSG_DSP2MCU_COMMON_AUDIO_SLT_RESULT:
            type = AUDIO_MESSAGE_TYPE_COMMON;
            if (audio_isr.callback[type]) {
                void *user_data = audio_isr.user_data[type];
                if (user_data != NULL) {
                    *((uint32_t *)user_data) = p_msg->data32;
                }
                audio_isr.callback[type](event, (void *)(uint32_t)message16);
            }
            return;
#endif
        case AUDIO_CCNI_MESSAGE_ACK(MSG_DSP2MCU_AUDIO_SYNC_DONE): { /* notify APP that sync action is already done*/
            uint8_t scenario_id = data16 >> 8;
            uint8_t action_id = (uint8_t)data16;
            log_hal_msgid_info("[DSP SYNC] DSP2MCU ACK, scenario_type = %d, action_type = %d\r\n", 2, scenario_id, action_id);
            switch (scenario_id) {
                case 0: // HFP
                    /* TODO */
                    return;
                case 1: // A2DP
                    /* TODO */
                    if (action_id == 1) { /* a2dp sync stop */
                        type = AUDIO_MESSAGE_TYPE_BT_A2DP_DL;
                        event = HAL_AUDIO_EVENT_END;
                    }
                    break;
                case 2: // ANC
                    /* TODO */
                    return;
                case 3: // VP
                    if (action_id == 0) { /* VP Start */
                        type = AUDIO_MESSAGE_TYPE_PROMPT;
                        event = HAL_AUDIO_EVENT_DATA_DIRECT;
                    }
                    break;
                default:
                    return;
            }
            break;
        }
        case MSG_DSP2MCU_COMMON_UPDATE_AUDIO_NVDM_STATUS:
            event = HAL_AUDIO_EVENT_AUDIO_NVDM;
            type = AUDIO_MESSAGE_TYPE_AUDIO_NVDM;
            if (audio_isr.callback[type]) {
                uint32_t user_data;
                user_data = data16;
                audio_isr.callback[type](event, &user_data);
            }
            return;
        #ifdef AIR_ADAPTIVE_EQ_ENABLE
        case MSG_DSP2MCU_AEQ_NOTIFY_INDEX:
        {
            log_hal_msgid_info("[AEQ]AEQ INDEX:%d",1,p_msg->data32);
            if(adaptive_eq_info.callback != NULL) {
                adaptive_eq_info.callback((int16_t)(p_msg->data32));
            }
            return;
        }
        #endif
        default:
            // Need not to notify, so we use return here.
            return;
    }

    if (audio_isr.callback[type]) {
        void *user_data = audio_isr.user_data[type];
        audio_isr.callback[type](event, user_data);
    }
}

//== Message Queue related ==
static void hal_audio_message_enqueue(audio_message_queue_t *p_queue, uint32_t msg16, uint32_t data16, uint32_t data32)
{
    audio_message_element_t *p_msg_element;
    uint32_t pre_wirte_index;
    if (p_queue->write_index == 0) {
        pre_wirte_index = AUDIO_MESSAGE_QUEUE_SIZE - 1;
    }
    else{
        pre_wirte_index = p_queue->write_index -1;
    }
    if ((p_queue->write_index != p_queue->read_index) && (p_queue->message[pre_wirte_index].message16 == MSG_DSP2MCU_RECORD_DATA_NOTIFY) && (msg16 == MSG_DSP2MCU_RECORD_DATA_NOTIFY))
    {
        log_hal_msgid_info("ignore MSG_DSP2MCU_RECORD_DATA_NOTIFY, wo=%d, ro=%d", 2, p_queue->write_index, p_queue->read_index);
        return;
    }
    // Check whether queue is full
    if (((p_queue->write_index + 1) & (AUDIO_MESSAGE_QUEUE_SIZE - 1)) == p_queue->read_index) {
        log_hal_msgid_error("[HAL audio]Message queue full\r\n", 0);
        log_hal_msgid_info("DSP_to_CM4_msg_queue_full", 0);
        assert(0);
    }

    // Enqueue
    p_msg_element = &p_queue->message[p_queue->write_index];
    p_msg_element->message16 = msg16;
    p_msg_element->data16 = data16;
    p_msg_element->data32 = data32;
    p_queue->write_index = (p_queue->write_index + 1) & (AUDIO_MESSAGE_QUEUE_SIZE - 1);
}

static void hal_audio_message_dequeue(audio_message_queue_t *p_queue, audio_message_element_t *p_msg)
{
    audio_message_element_t *p_msg_element;

    // Check whether queue is empty
    if (p_queue->write_index == p_queue->read_index) {
        log_hal_msgid_error("[HAL audio]Message queue empty\r\n", 0);
        log_hal_msgid_info("DSP_to_CM4_msg_queue_empty", 0);
        assert(0);
    }

    // Dequeue
    p_msg_element = &p_queue->message[p_queue->read_index];
    p_msg->message16 = p_msg_element->message16;
    p_msg->data16 = p_msg_element->data16;
    p_msg->data32 = p_msg_element->data32;
    p_queue->read_index = (p_queue->read_index + 1) & (AUDIO_MESSAGE_QUEUE_SIZE - 1);
}

void hal_audio_dsp_message_process(void)
{
    // For data notification, request and error, we have to callback
    // For the other message, we may skip it
    while (dsp_controller.dsp_msg_queue.read_index != dsp_controller.dsp_msg_queue.write_index) {
        audio_message_element_t msg;

        hal_audio_message_dequeue(&dsp_controller.dsp_msg_queue, &msg);

        hal_audio_service_callback(&msg);
    }
}

static void hal_audio_init_share_info_section(n9_dsp_share_info_t *p_info, uint32_t *p_buf_addr, uint32_t buf_byte_size)
{
    memset(p_info, 0, sizeof(n9_dsp_share_info_t));
    p_info->start_addr = (uint32_t)p_buf_addr;
    p_info->length     = buf_byte_size;
}

//== DSP power on ==
#if 0 //Mark this for fixing build warning.
#if defined(HAL_SLEEP_MANAGER_ENABLED)
static void hal_audio_dsp_power_on_check(hal_core_id_t id)
{
    hal_core_status_t state;

    do {
        state = hal_core_status_read(id);
        if ((state == HAL_CORE_ACTIVE) || (state == HAL_CORE_SLEEP)) {
            break;
        }
        hal_audio_delay_ms(2);
    } while (1);
}
#endif
#endif
static void hal_audio_dsp_power_on(void)
{
#if defined(HAL_SLEEP_MANAGER_ENABLED)
    // DSP power on
    spm_control_mtcmos(SPM_MTCMOS_DSP, SPM_MTCMOS_PWR_ENABLE);
    //pmu_enable_power(PMU_BUCK_VAUD18, PMU_ON);
    // DSP reset
    hal_dsp_core_reset(HAL_CORE_DSP0, DSP0_BASE);
//    hal_dsp_core_reset(HAL_CORE_DSP1, DSP1_BASE);

#if 0 // will checked in CCNI
    // Wait for finish
    hal_audio_dsp_power_on_check(HAL_CORE_DSP0);
//    hal_audio_dsp_power_on_check(HAL_CORE_DSP1);
#endif

#endif
}

static void hal_audio_dsp_power_off(void)
{
#if defined(HAL_SLEEP_MANAGER_ENABLED)
    spm_control_mtcmos(SPM_MTCMOS_DSP, SPM_MTCMOS_PWR_DISABLE);
    //pmu_enable_power(PMU_BUCK_VAUD18, PMU_OFF);
#endif
}

uint32_t data_addr;
void common_type_callback(hal_audio_event_t event, void *user_data)
{

    uint32_t addr = hal_memview_infrasys_to_mcu(*(uint32_t *)user_data);
    UNUSED(addr);
    UNUSED(event);
    //log_hal_msgid_info("============================== user_data = 0x%x \n\r", 1, addr);

}
//==== Public API ====
uint32_t hal_audio_dsp2mcu_data_get(void)
{
    return dsp2mcu_data;
}

uint32_t hal_audio_dsp2mcu_AUDIO_DL_ACL_data_get(void)
{
    return dsp2mcu_AUDIO_DL_ACL_data;
}

void hal_audio_dsp_controller_init(void)
{
    // Fill share buffer information
    memset(&g_dsp_controller_cg[0][0], 0, sizeof(uint32_t) * AUDIO_POWER_END * ((AUDIO_SCEANRIO_TYPE_MAX + 31) / 32));
    memset(g_dsp_controller_cg_control_bit, 0, sizeof(uint32_t) * AUDIO_SCENARIO_TYPE_END);
    hal_audio_init_share_info_section(&n9_dsp_share_info[SHARE_BUFFER_INFO_INDEX_BT_AUDIO_DL], &audio_share_buffer.bt_audio_dl[0], SHARE_BUFFER_BT_AUDIO_DL_SIZE);
    hal_audio_init_share_info_section(&n9_dsp_share_info[SHARE_BUFFER_INFO_INDEX_BT_VOICE_UL], &audio_share_buffer.bt_voice_ul[0], SHARE_BUFFER_BT_VOICE_UL_SIZE);
    hal_audio_init_share_info_section(&n9_dsp_share_info[SHARE_BUFFER_INFO_INDEX_BT_VOICE_DL], &audio_share_buffer.bt_voice_dl[0], SHARE_BUFFER_BT_VOICE_DL_SIZE);
    hal_audio_init_share_info_section(&n9_dsp_share_info[SHARE_BUFFER_INFO_INDEX_PROMPT],      &audio_share_buffer.prompt[0],      SHARE_BUFFER_PROMPT_SIZE);
    hal_audio_init_share_info_section(&n9_dsp_share_info[SHARE_BUFFER_INFO_INDEX_RECORD],      &audio_share_buffer.record[0],      SHARE_BUFFER_RECORD_SIZE);
    hal_audio_init_share_info_section(&n9_dsp_share_info[SHARE_BUFFER_INFO_INDEX_RINGTONE],    &audio_share_buffer.ringtone[0],    SHARE_BUFFER_RINGTONE_SIZE);
    hal_audio_init_share_info_section(&n9_dsp_share_info[SHARE_BUFFER_INFO_INDEX_NVKEY_PARAMETER],    &audio_share_buffer.nvkey_param[0],   SHARE_BUFFER_NVKEY_PARAMETER_SIZE);
#ifdef AIR_BT_CODEC_BLE_ENABLED
    leaudio_share_buffer.ble_audio_ul = (uint32_t *)bt_pka_get_leaudio_AVM_addr(SHARE_BUFFER_BLE_AUDIO_UL_SIZE + SHARE_BUFFER_BLE_AUDIO_DL_SIZE + SHARE_BUFFER_BLE_AUDIO_SUB_UL_SIZE + SHARE_BUFFER_BLE_AUDIO_SUB_DL_SIZE);
    leaudio_share_buffer.ble_audio_dl = &leaudio_share_buffer.ble_audio_ul[(SHARE_BUFFER_BLE_AUDIO_UL_SIZE + 3) / 4];
    leaudio_share_buffer.ble_audio_sub_ul = &leaudio_share_buffer.ble_audio_dl[(SHARE_BUFFER_BLE_AUDIO_DL_SIZE + 3) / 4];
    leaudio_share_buffer.ble_audio_sub_dl = &leaudio_share_buffer.ble_audio_sub_ul[(SHARE_BUFFER_BLE_AUDIO_SUB_UL_SIZE + 3) / 4];
    hal_audio_init_share_info_section(&n9_dsp_share_info[SHARE_BUFFER_INFO_INDEX_BLE_AUDIO_UL], leaudio_share_buffer.ble_audio_ul, SHARE_BUFFER_BLE_AUDIO_UL_SIZE);
    hal_audio_init_share_info_section(&n9_dsp_share_info[SHARE_BUFFER_INFO_INDEX_BLE_AUDIO_DL], leaudio_share_buffer.ble_audio_dl, SHARE_BUFFER_BLE_AUDIO_DL_SIZE);
    hal_audio_init_share_info_section(&n9_dsp_share_info[SHARE_BUFFER_INFO_INDEX_BLE_AUDIO_SUB_UL], leaudio_share_buffer.ble_audio_sub_ul, SHARE_BUFFER_BLE_AUDIO_SUB_UL_SIZE);
    hal_audio_init_share_info_section(&n9_dsp_share_info[SHARE_BUFFER_INFO_INDEX_BLE_AUDIO_SUB_DL], leaudio_share_buffer.ble_audio_sub_dl, SHARE_BUFFER_BLE_AUDIO_SUB_DL_SIZE);
#endif

#if defined (AIR_BT_ULTRA_LOW_LATENCY_ENABLE) || defined (AIR_WIRED_AUDIO_ENABLE)
    hal_audio_init_share_info_section(&audio_transmitter_share_info[AUDIO_TRANSMITTER_SHARE_INFO_INDEX_GAMING_MODE_BT_RECEIVE_FROM_AIR], &audio_share_buffer.bt_audio_dl[0],        10 * 1024);
    hal_audio_init_share_info_section(&audio_transmitter_share_info[AUDIO_TRANSMITTER_SHARE_INFO_INDEX_GAMING_MODE_BT_SEND_TO_AIR], &audio_share_buffer.bt_audio_dl[0] + 10 * 1024 / 4, 10 * 1024);
    hal_audio_init_share_info_section(&audio_transmitter_share_info[AUDIO_TRANSMITTER_SHARE_INFO_INDEX_GAMING_MODE_DSP_RECEIVE_FROM_MCU_0], &audio_share_buffer.bt_audio_dl[0] + 20 * 1024 / 4, 5 * 1024);
    hal_audio_init_share_info_section(&audio_transmitter_share_info[AUDIO_TRANSMITTER_SHARE_INFO_INDEX_GAMING_MODE_DSP_RECEIVE_FROM_MCU_1], &audio_share_buffer.bt_audio_dl[0] + 25 * 1024 / 4, 5 * 1024);
    hal_audio_init_share_info_section(&audio_transmitter_share_info[AUDIO_TRANSMITTER_SHARE_INFO_INDEX_GAMING_MODE_DSP_SEND_TO_MCU], &audio_share_buffer.bt_audio_dl[0] + 30 * 1024 / 4, 10 * 1024);
#if defined (AIR_WIRED_AUDIO_ENABLE)
    hal_audio_init_share_info_section(&audio_transmitter_share_info[AUDIO_TRANSMITTER_SHARE_INFO_INDEX_WIRED_AUDIO_DSP_RECEIVE_FROM_MCU_0], &audio_share_buffer.usb_in_0[0], SHARE_BUFFER_BT_AUDIO_USB_IN_SIZE);
    hal_audio_init_share_info_section(&audio_transmitter_share_info[AUDIO_TRANSMITTER_SHARE_INFO_INDEX_WIRED_AUDIO_DSP_RECEIVE_FROM_MCU_1], &audio_share_buffer.usb_in_1[0], SHARE_BUFFER_BT_AUDIO_USB_IN_SIZE);
#endif
#endif

#if defined (AIR_BLE_AUDIO_DONGLE_ENABLE)
    hal_audio_init_share_info_section(&audio_transmitter_share_info[AUDIO_TRANSMITTER_SHARE_INFO_INDEX_BLE_AUDIO_DONGLE_BT_RECEIVE_FROM_AIR_0], &audio_share_buffer.bt_audio_dl[0] + 0 * 1024 / 4,    5 * 1024);
    hal_audio_init_share_info_section(&audio_transmitter_share_info[AUDIO_TRANSMITTER_SHARE_INFO_INDEX_BLE_AUDIO_DONGLE_BT_RECEIVE_FROM_AIR_1], &audio_share_buffer.bt_audio_dl[0] + 5 * 1024 / 4,    5 * 1024);
    hal_audio_init_share_info_section(&audio_transmitter_share_info[AUDIO_TRANSMITTER_SHARE_INFO_INDEX_BLE_AUDIO_DONGLE_BT_SEND_TO_AIR_0],      &audio_share_buffer.bt_audio_dl[0] + 10 * 1024 / 4,   5 * 1024);
    hal_audio_init_share_info_section(&audio_transmitter_share_info[AUDIO_TRANSMITTER_SHARE_INFO_INDEX_BLE_AUDIO_DONGLE_BT_SEND_TO_AIR_1],      &audio_share_buffer.bt_audio_dl[0] + 15 * 1024 / 4,   5 * 1024);
    hal_audio_init_share_info_section(&audio_transmitter_share_info[AUDIO_TRANSMITTER_SHARE_INFO_INDEX_BLE_AUDIO_DONGLE_DSP_RECEIVE_FROM_MCU_0], &audio_share_buffer.bt_audio_dl[0] + 20 * 1024 / 4,   5 * 1024);
    hal_audio_init_share_info_section(&audio_transmitter_share_info[AUDIO_TRANSMITTER_SHARE_INFO_INDEX_BLE_AUDIO_DONGLE_DSP_RECEIVE_FROM_MCU_1], &audio_share_buffer.bt_audio_dl[0] + 25 * 1024 / 4,   5 * 1024);
    hal_audio_init_share_info_section(&audio_transmitter_share_info[AUDIO_TRANSMITTER_SHARE_INFO_INDEX_BLE_AUDIO_DONGLE_DSP_SEND_TO_MCU],       &audio_share_buffer.bt_audio_dl[0] + 30 * 1024 / 4,   5 * 1024);
#endif /* AIR_BLE_AUDIO_DONGLE_ENABLE */

    #if defined (AIR_ULL_AUDIO_V2_DONGLE_ENABLE)
    hal_audio_init_share_info_section(&audio_transmitter_share_info[AUDIO_TRANSMITTER_SHARE_INFO_INDEX_ULL_AUDIO_V2_DONGLE_BT_RECEIVE_FROM_AIR_0],  (&audio_share_buffer.bt_audio_dl[0]+0*1024/4),    5*1024);
    hal_audio_init_share_info_section(&audio_transmitter_share_info[AUDIO_TRANSMITTER_SHARE_INFO_INDEX_ULL_AUDIO_V2_DONGLE_BT_RECEIVE_FROM_AIR_1],  (&audio_share_buffer.bt_audio_dl[0]+5*1024/4),    5*1024);
    hal_audio_init_share_info_section(&audio_transmitter_share_info[AUDIO_TRANSMITTER_SHARE_INFO_INDEX_ULL_AUDIO_V2_DONGLE_BT_SEND_TO_AIR_0],       (&audio_share_buffer.bt_audio_dl[0]+10*1024/4),   5*1024);
    hal_audio_init_share_info_section(&audio_transmitter_share_info[AUDIO_TRANSMITTER_SHARE_INFO_INDEX_ULL_AUDIO_V2_DONGLE_BT_SEND_TO_AIR_1],       (&audio_share_buffer.bt_audio_dl[0]+15*1024/4),   5*1024);
    hal_audio_init_share_info_section(&audio_transmitter_share_info[AUDIO_TRANSMITTER_SHARE_INFO_INDEX_ULL_AUDIO_V2_DONGLE_DSP_RECEIVE_FROM_MCU_0], (&audio_share_buffer.dongle_usb_in_0[0]),  SHARE_BUFFER_USB_SIZE);
    hal_audio_init_share_info_section(&audio_transmitter_share_info[AUDIO_TRANSMITTER_SHARE_INFO_INDEX_ULL_AUDIO_V2_DONGLE_DSP_RECEIVE_FROM_MCU_1], (&audio_share_buffer.dongle_usb_in_1[0]),  SHARE_BUFFER_USB_SIZE);
    hal_audio_init_share_info_section(&audio_transmitter_share_info[AUDIO_TRANSMITTER_SHARE_INFO_INDEX_ULL_AUDIO_V2_DONGLE_DSP_SEND_TO_MCU_0],      (&audio_share_buffer.dongle_usb_out_0[0]), SHARE_BUFFER_USB_SIZE);
    #endif /* AIR_BLE_AUDIO_DONGLE_ENABLE */

    //Add common callback register here
    hal_audio_service_hook_callback(AUDIO_MESSAGE_TYPE_COMMON, common_type_callback, &data_addr);

    // Notify DSP the share buffer address
    //Wait for UT:    hal_audio_dsp_controller_send_message(MSG_MCU2DSP_COMMON_SET_MEMORY, 0, (uint32_t)&n9_dsp_share_info, false);
    hal_audio_dsp_controller_send_message(MSG_MCU2DSP_COMMON_SET_MEMORY, 0, (uint32_t)&n9_dsp_share_info, false);

    memset((void *)&audio_clkmux_status, 0, AUDIO_CLKMUX_BLOCK_NUM_OF_CLKMUX_BLOCK * sizeof(audiosys_clkmux_control));
}

void hal_audio_dsp_controller_deinit(void)
{
    dsp_controller.dsp_power_on = false;
    hal_audio_dsp_power_off();
}

void hal_audio_ccni_isr(hal_ccni_event_t event, void *msg)
{
    uint32_t *pMsg = (uint32_t *)msg;
    uint32_t msg1, msg2;
    uint32_t msg16, data16;
    uint32_t status;

    status = hal_ccni_mask_event(event);

    msg1 = pMsg[0];
    msg2 = pMsg[1];

    status = hal_ccni_clear_event(event);
    status = hal_ccni_unmask_event(event);
    UNUSED(status);
    msg16 = msg1 >> 16;
    data16 = msg1 & 0xFFFF;

#if defined(HAL_AUDIO_DEBUG)
    log_hal_msgid_info("[HAL audio] Receive msg %x, %x, %x \r\n", 3, (unsigned int)msg16, (unsigned int)data16, (unsigned int)msg2);
#endif

    // Check the message type, clear waiting flag
    switch (msg16) {
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_BT_AUDIO_DL_OPEN):
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_BT_AUDIO_DL_CLOSE):
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_BT_AUDIO_DL_START):
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_BT_AUDIO_DL_STOP):
            dsp_controller.waiting_A2DP = false;
            break;
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_BT_VOICE_UL_OPEN):
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_BT_VOICE_UL_CLOSE):
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_BT_VOICE_UL_START):
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_BT_VOICE_UL_STOP):
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_BT_VOICE_DL_OPEN):
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_BT_VOICE_DL_CLOSE):
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_BT_VOICE_DL_START):
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_BT_VOICE_DL_STOP):
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_BT_VOICE_UL_SUSPEND):
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_BT_VOICE_UL_RESUME):
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_BT_VOICE_DL_SUSPEND):
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_BT_VOICE_DL_RESUME):
            dsp_controller.waiting_ESCO = false;
            break;
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_BLE_AUDIO_UL_OPEN):
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_BLE_AUDIO_UL_CLOSE):
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_BLE_AUDIO_UL_START):
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_BLE_AUDIO_UL_STOP):
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_BLE_AUDIO_DL_OPEN):
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_BLE_AUDIO_DL_CLOSE):
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_BLE_AUDIO_DL_START):
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_BLE_AUDIO_DL_STOP):
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_PLAYBACK_OPEN):
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_PLAYBACK_CLOSE):
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_PLAYBACK_START):
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_PLAYBACK_STOP):
            dsp_controller.waiting = false;
            break;
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_RECORD_OPEN):
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_RECORD_CLOSE):
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_RECORD_START):
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_RECORD_STOP):
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_RECORD_LC_SET_PARAM):
            dsp_controller.waiting_RECORD = false;
            break;
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_PROMPT_OPEN):
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_PROMPT_CLOSE):
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_PROMPT_START):
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_PROMPT_STOP):
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_PROMPT_DUMMY_SOURCE_OPEN):
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_PROMPT_DUMMY_SOURCE_CLOSE):
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_PROMPT_DUMMY_SOURCE_START):
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_PROMPT_DUMMY_SOURCE_STOP):
            dsp_controller.waiting_VP = false;
            break;
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_LINEIN_PLAYBACK_OPEN):
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_LINEIN_PLAYBACK_CLOSE):
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_LINEIN_PLAYBACK_START):
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_LINEIN_PLAYBACK_STOP):
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_LINEIN_PLAYBACK_SUSPEND):
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_LINEIN_PLAYBACK_RESUME):
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_TRULY_LINEIN_PLAYBACK_OPEN):
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_TRULY_LINEIN_PLAYBACK_CLOSE):
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_COMMON_AUDIO_LOOPBACK_TEST):
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_COMMON_REQ_GET_REALTIME_REF_GAIN):
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_COMMON_REQ_GET_REALTIME_LIB_VERSION):
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_COMMON_AEC_NR_SET_PARAM):
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_COMMON_SIDETONE_START):
        //case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_COMMON_SIDETONE_STOP)  //Sidetone stop ack asynchronous. no wait
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_COMMON_ANC_START):
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_COMMON_ANC_SET_VOLUME):
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_COMMON_ANC_SET_PARAM):
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_ADAPT_ANC_OPEN):
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_ADAPT_ANC_CLOSE):
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_ADAPT_ANC_START):
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_ADAPT_ANC_STOP):
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_ADAPT_ANC_SET_PARAM):
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_COMMON_DC_COMPENSATION_START):
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_COMMON_DC_COMPENSATION_STOP):
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_COMMON_DUMMY_DSP_SHUTDOWN):
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_COMMON_PEQ_SET_PARAM):
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_COMMON_ADEQ_SET_PARAM):        
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_COMMON_DEQ_SET_PARAM):
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_COMMON_SET_ALGORITHM_PARAM):
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_COMMON_AUDIO_ANC_ADAPTIVE_GET):
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_COMMON_AUDIO_ANC_ADAPTIVE_SET):
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_BT_AUDIO_DL_SUSPEND):
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_BT_AUDIO_DL_RESUME):
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_PLAYBACK_SUSPEND):
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_PLAYBACK_RESUME):
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_BLE_AUDIO_DL_SUSPEND):
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_BLE_AUDIO_DL_RESUME):
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_BLE_AUDIO_UL_SUSPEND):
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_BLE_AUDIO_UL_RESUME):
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_RECORD_SUSPEND):
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_RECORD_RESUME):
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_COMMON_SET_OUTPUT_VOLUME_PARAMETERS):
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_AUDIO_AMP_FORCE_CLOSE):
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_AUDIO_TRANSMITTER_OPEN):
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_AUDIO_TRANSMITTER_CLOSE):
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_AUDIO_TRANSMITTER_START):
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_AUDIO_TRANSMITTER_STOP):
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_AUDIO_TRANSMITTER_CONFIG):
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_AUDIO_TRANSMITTER_SUSPEND):
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_AUDIO_TRANSMITTER_RESUME):
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_AUDIO_SYNC_REQUEST):
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_COMMON_SET_DRIVER_PARAM):
            dsp_controller.waiting = false;
            break;
        case MSG_MCU2DSP_COMMON_POWER_OFF_DAC_IMMEDIATELY:
            dsp_controller.waiting_DAC_OFF = false;
            break;
    }

    // Decide whether we have to handle the message further.
    switch (msg16) {
        case MSG_DSP2MCU_AUDIO_AMP:
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_BT_AUDIO_DL_OPEN):
        case MSG_DSP2MCU_BT_AUDIO_DL_ERROR:
        case MSG_DSP2MCU_BT_VOICE_UL_ERROR:
        case MSG_DSP2MCU_BT_VOICE_DL_ERROR:
        case MSG_DSP2MCU_PLAYBACK_ERROR:
        case MSG_DSP2MCU_PLAYBACK_DATA_REQUEST:
        case MSG_DSP2MCU_RECORD_ERROR:
        case MSG_DSP2MCU_RECORD_DATA_NOTIFY:
        case MSG_DSP2MCU_RECORD_WWE_VERSION:
        case MSG_DSP2MCU_RECORD_DATA_ABORT_NOTIFY:
        case MSG_DSP2MCU_RECORD_WWD_NOTIFY:
        case MSG_DSP2MCU_PROMPT_ERROR:
        case MSG_DSP2MCU_AEQ_NOTIFY_INDEX:
        case MSG_DSP2MCU_PROMPT_DATA_REQUEST:
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_PROMPT_CONFIG):
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_COMMON_SIDETONE_STOP):
        case MSG_DSP2MCU_BT_AUDIO_DL_TIME_REPORT:
        case MSG_DSP2MCU_BT_AUDIO_DL_ALC_REQUEST:
        case MSG_DSP2MCU_BT_AUDIO_DL_LTCS_DATA_REPORT:
        case MSG_DSP2MCU_BT_AUDIO_DL_LTCS_TIMEOUT_REQUEST:
        case MSG_DSP2MCU_COMMON_REQ_GET_AUDIO_FEATURE_PARAMETER:
        case MSG_DSP2MCU_BT_AUDIO_DL_REINIT_REQUEST:
        case MSG_DSP2MCU_AVC_PARA_SEND:
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_COMMON_ANC_STOP):
        case MSG_DSP2MCU_COMMON_ANC_START_DONE:
        case AUDIO_CCNI_MESSAGE_ACK(MSG_MCU2DSP_COMMON_ANC_SET_VOLUME):
        case MSG_DSP2MCU_RECORD_LC_WZ_REPORT:
        case MSG_DSP2MCU_AUDIO_TRANSMITTER_DATA_NOTIFY:
        case MSG_DSP2MCU_AUDIO_TRANSMITTER_DATA_DIRECT:
        case MSG_DSP2MCU_COMMON_AUDIO_ANC_SWITCH:
        case MSG_DSP2MCU_COMMON_AUDIO_ANC_ADAPTIVE:
        case AUDIO_CCNI_MESSAGE_ACK(MSG_DSP2MCU_AUDIO_SYNC_DONE):
        case MSG_DSP2MCU_COMMON_UPDATE_AUDIO_NVDM_STATUS:
#ifdef MTK_SLT_AUDIO_HW
        case MSG_DSP2MCU_COMMON_AUDIO_SLT_RESULT:
#endif
            // Put into message queue
            hal_audio_message_enqueue(&dsp_controller.dsp_msg_queue, msg16, data16, msg2);

            if (audio_isr.audio_event_task_callback) {
                // Notify to task
                audio_isr.audio_event_task_callback();
            } else {
                // Process message directly
                hal_audio_dsp_message_process();
            }

            break;
    }
}

#ifdef FPGA_ENV
#include "memory_map.h"
#define DSP_START_ADDRESS lp_get_begin_address(PARTITION_DSP0)
#endif

void hal_audio_put_message_via_ccni(uint16_t message, uint16_t data16, uint32_t data32)
{
    hal_ccni_status_t ret;
    hal_ccni_message_t ccni_msg;
    uint32_t i;

    // Power on DSP at the first use
    if (!dsp_controller.dsp_power_on) {
        dsp_controller.dsp_power_on = true;
#ifdef FPGA_ENV
        uint32_t dsp_vector_base;
        dsp_vector_base = DSP_START_ADDRESS;
        log_hal_msgid_info("DSP vector base address = 0x%08x. \r\n", 1, dsp_vector_base);
        hal_dsp_core_reset(HAL_CORE_DSP0, dsp_vector_base);
#else
        hal_audio_dsp_power_on();
#endif
        log_hal_msgid_info("dsp power on\n", 0);

        // Wait DSP boot ready at the first use
        for (i = 1 ; ; i++) {
            if ((hal_core_status_read(HAL_CORE_DSP0) == HAL_CORE_ACTIVE) || (hal_core_status_read(HAL_CORE_DSP0) == HAL_CORE_SLEEP)) {
                log_hal_msgid_info("dsp ready\n", 0);
                break;
            }
#if !defined(HAL_AUDIO_DEBUG)
            assert(i < 100);
#else
            if ((i % 1000) == 0) {
                log_hal_msgid_info("Waiting msg(0x%x) CCNI_busy %ld \r\n", 2, message, i);
                //platform_assert("DSP_no_remove_CCNI",__FILE__,__LINE__);
                assert(0);
            }
#endif
            hal_audio_delay_ms(2);
        }
    }

    // Fill into ccni message
    ccni_msg.ccni_message[0] = (message << 16) | data16;
    ccni_msg.ccni_message[1] = data32;

    for (i = 1 ; (ret = hal_ccni_set_event(CCNI_CM4_TO_DSP0_RX_EVENT, &ccni_msg)) != HAL_CCNI_STATUS_OK ; i++) {
#if !defined(HAL_AUDIO_DEBUG)
        assert(i < 40);
#else
        if ((i % 1000) == 0) {
            log_hal_msgid_info("Send message(0x%x) waiting %d (%d)\r\n", 3, message, (int)i, ret);
            //platform_assert("CM4_send_DSP_CCNI_timeout",__FILE__,__LINE__);
            assert(0);
        }
#endif
        hal_audio_delay_ms(2);
    }

#if defined(HAL_AUDIO_DEBUG)
    log_hal_msgid_info("[HAL audio] Send msg %x %x, wait count %d \r\n", 3, (unsigned int)ccni_msg.ccni_message[0], (unsigned int)ccni_msg.ccni_message[1], (int)i);
#endif
}

void hal_audio_dsp_controller_send_message(uint16_t message, uint16_t data16, uint32_t data32, bool wait)
{
    uint16_t i;
    uint8_t ID_waiting = 0;

#ifndef FPGA_ENV
#ifdef HAL_DVFS_MODULE_ENABLED
    uint16_t flag_start = 0;
    uint32_t freq_result;
    freq_result = hal_dvfs_get_cpu_frequency();
    if (freq_result < (uint32_t)156000) {
        if (MSG_MCU2DSP_BT_AUDIO_DL_START == message) {
            flag_start = 1;
        } else if (MSG_MCU2DSP_BT_VOICE_UL_START == message) {
            flag_start = 1;
        } else if (MSG_MCU2DSP_BT_VOICE_DL_START == message) {
            flag_start = 1;
        } else if (MSG_MCU2DSP_PLAYBACK_START == message) {
            flag_start = 1;
        } else if (MSG_MCU2DSP_PROMPT_START == message) {
            flag_start = 1;
        } else {
            flag_start = 0;
        }
    }
#endif /*HAL_DVFS_MODULE_ENABLED*/

#ifdef HAL_DVFS_MODULE_ENABLED
    if (flag_start) {
        hal_dvfs_lock_control(HAL_DVFS_OPP_MID, HAL_DVFS_LOCK);
        //log_hal_msgid_info("frequency is risen to 1.3V", 0);
    }
#endif /*HAL_DVFS_MODULE_ENABLED*/
#endif
    if (wait) {
        switch (message) {
            case MSG_MCU2DSP_BT_AUDIO_DL_OPEN:
            case MSG_MCU2DSP_BT_AUDIO_DL_CLOSE:
            case MSG_MCU2DSP_BT_AUDIO_DL_START:
            case MSG_MCU2DSP_BT_AUDIO_DL_STOP:
                dsp_controller.waiting_A2DP = true;
                ID_waiting = 1 << 3;
                break;
            case MSG_MCU2DSP_BT_VOICE_UL_OPEN:
            case MSG_MCU2DSP_BT_VOICE_UL_CLOSE:
            case MSG_MCU2DSP_BT_VOICE_UL_START:
            case MSG_MCU2DSP_BT_VOICE_UL_STOP:
            case MSG_MCU2DSP_BT_VOICE_DL_OPEN:
            case MSG_MCU2DSP_BT_VOICE_DL_CLOSE:
            case MSG_MCU2DSP_BT_VOICE_DL_START:
            case MSG_MCU2DSP_BT_VOICE_DL_STOP:
            case MSG_MCU2DSP_BT_VOICE_UL_SUSPEND:
            case MSG_MCU2DSP_BT_VOICE_UL_RESUME:
            case MSG_MCU2DSP_BT_VOICE_DL_SUSPEND:
            case MSG_MCU2DSP_BT_VOICE_DL_RESUME:
                dsp_controller.waiting_ESCO = true;
                ID_waiting = 1 << 4;
                break;
            case MSG_MCU2DSP_RECORD_OPEN:
            case MSG_MCU2DSP_RECORD_CLOSE:
            case MSG_MCU2DSP_RECORD_START:
            case MSG_MCU2DSP_RECORD_STOP:
            case MSG_MCU2DSP_RECORD_LC_SET_PARAM:
                dsp_controller.waiting_RECORD = true;
                ID_waiting = 1 << 0;
                break;
            case MSG_MCU2DSP_PROMPT_OPEN:
            case MSG_MCU2DSP_PROMPT_CLOSE:
            case MSG_MCU2DSP_PROMPT_START:
            case MSG_MCU2DSP_PROMPT_STOP:
            case MSG_MCU2DSP_PROMPT_DUMMY_SOURCE_OPEN:
            case MSG_MCU2DSP_PROMPT_DUMMY_SOURCE_CLOSE:
            case MSG_MCU2DSP_PROMPT_DUMMY_SOURCE_START:
            case MSG_MCU2DSP_PROMPT_DUMMY_SOURCE_STOP:
                dsp_controller.waiting_VP = true;
                ID_waiting = 1 << 1;
                break;
            case MSG_MCU2DSP_COMMON_POWER_OFF_DAC_IMMEDIATELY:
                dsp_controller.waiting_DAC_OFF = true;
                ID_waiting = 1 << 6;
                break;
            default:
                dsp_controller.waiting = true;
                ID_waiting = 1 << 2;
                break;
        }
    }

    hal_audio_put_message_via_ccni(message, data16, data32);

    if (wait) {
        for (i = 0; ; i++) {
            if (ID_waiting == 0x04) {
                if (dsp_controller.waiting == false) {
#ifndef FPGA_ENV
#if defined(HAL_DVFS_MODULE_ENABLED)
                    if (flag_start) {
                        hal_dvfs_lock_control(HAL_DVFS_OPP_MID, HAL_DVFS_UNLOCK);
                        freq_result = hal_dvfs_get_cpu_frequency();
                        //log_hal_msgid_info("frequency is set back %d", 1, (int)freq_result);
                    }
#endif /*HAL_DVFS_MODULE_ENABLED*/
#endif
                    break;
                }
            } else if (ID_waiting == 0x02) {
                if (dsp_controller.waiting_VP == false) {
#ifndef FPGA_ENV
#if defined(HAL_DVFS_MODULE_ENABLED)
                    if (flag_start) {
                        hal_dvfs_lock_control(HAL_DVFS_OPP_MID, HAL_DVFS_UNLOCK);
                        freq_result = hal_dvfs_get_cpu_frequency();
                        //log_hal_msgid_info("frequency is set back %d", 1, (int)freq_result);
                    }
#endif /*HAL_DVFS_MODULE_ENABLED*/
#endif
                    break;
                }
            } else if (ID_waiting == 0x01) {
                if (dsp_controller.waiting_RECORD == false) {
#ifndef FPGA_ENV
#if defined(HAL_DVFS_MODULE_ENABLED)
                    if (flag_start) {
                        hal_dvfs_lock_control(HAL_DVFS_OPP_MID, HAL_DVFS_UNLOCK);
                        freq_result = hal_dvfs_get_cpu_frequency();
                        //log_hal_msgid_info("frequency is set back %d", 1, (int)freq_result);
                    }
#endif /*HAL_DVFS_MODULE_ENABLED*/
#endif
                    break;
                }
            } else if (ID_waiting == 0x08) {
                if (dsp_controller.waiting_A2DP == false) {
#ifndef FPGA_ENV
#if defined(HAL_DVFS_MODULE_ENABLED)
                    if (flag_start) {
                        hal_dvfs_lock_control(HAL_DVFS_OPP_MID, HAL_DVFS_UNLOCK);
                        freq_result = hal_dvfs_get_cpu_frequency();
                        //log_hal_msgid_info("frequency is set back %d", 1, (int)freq_result);
                    }
#endif /*HAL_DVFS_MODULE_ENABLED*/
#endif
                    break;
                }
            } else if (ID_waiting == 0x10) {
                if (dsp_controller.waiting_ESCO == false) {
#ifndef FPGA_ENV
#if defined(HAL_DVFS_MODULE_ENABLED)
                    if (flag_start) {
                        hal_dvfs_lock_control(HAL_DVFS_OPP_MID, HAL_DVFS_UNLOCK);
                        freq_result = hal_dvfs_get_cpu_frequency();
                        //log_hal_msgid_info("frequency is set back %d", 1, (int)freq_result);
                    }
#endif /*HAL_DVFS_MODULE_ENABLED*/
#endif
                    break;
                }
            } else if (ID_waiting == (1 << 6)) {
                if (dsp_controller.waiting_DAC_OFF == false) {
#if defined(HAL_DVFS_MODULE_ENABLED)
#if defined(HAL_DVFS_416M_SOURCE)
                    if (flag_start) {
                        hal_dvfs_lock_control(HAL_DVFS_FULL_SPEED_104M, HAL_DVFS_UNLOCK);
                        freq_result = hal_dvfs_get_cpu_frequency();
                        //log_hal_msgid_info("frequency is set back %d", 1, (int)freq_result);
                    }
#endif /*HAL_DVFS_MODULE_ENABLED*/
#endif /*HAL_DVFS_416M_SOURCE*/
                    break;
                }
            }
#if !defined(HAL_AUDIO_DEBUG)
            assert(i < 40);
#else
            if ((i % 1000) == 0) {
                log_hal_msgid_info("[HAL audio] Wait msg(0x%x) ack %d \r\n", 2, message, (int)i);
                if (i == 1000) {
                    log_hal_msgid_info("DSP_no_ack_response_to_CCNI", 0);
                    assert(0);
                }
            }
#endif

            hal_audio_delay_ms(2);
        }

#if defined(HAL_AUDIO_DEBUG)
        log_hal_msgid_info("[HAL audio] Ack %x, wait count %d \r\n", 2, message, i);
#endif

    }
}

void *hal_audio_dsp_controller_put_paramter(const void *p_param_addr, uint32_t param_size, audio_message_type_t msg_type)
{
    // Check the size of parameter
    // Copy paramter to share buffer
    if (param_size > (SHARE_BUFFER_MCU2DSP_PARAMETER_SIZE >> 2)) {
        log_hal_msgid_info("Hal_audio_put_param_over_size", 0);
        assert(0);
        return NULL;
    }

    uint32_t offset = 0;
    const uint32_t channel_index = (SHARE_BUFFER_MCU2DSP_PARAMETER_SIZE >> 2) >> 2;
    if (msg_type == AUDIO_MESSAGE_TYPE_PROMPT) {
        if (dsp_controller.waiting_VP == true) {
            log_hal_msgid_info("Hal_audio_put_param_VP_busy", 0);
            assert(0);
        }
        offset = 1 * channel_index;
    } else if (msg_type == AUDIO_MESSAGE_TYPE_RECORD) {
        if (dsp_controller.waiting_RECORD == true) {
            log_hal_msgid_info("Hal_audio_put_param_RECORD_busy", 0);
            assert(0);
        }
        offset = 2 * channel_index;
    } else if (msg_type == AUDIO_MESSAGE_TYPE_BT_AUDIO_DL) {
        if (dsp_controller.waiting_A2DP == true) {
            log_hal_msgid_info("Hal_audio_put_param_A2DP_busy", 0);
            assert(0);
        }
        offset = 3 * channel_index;
    } else if (msg_type < AUDIO_MESSAGE_TYPE_MAX) {
        if (dsp_controller.waiting == true) {
            log_hal_msgid_info("Hal_audio_put_param_AM_busy", 0);
            assert(0);
        }
        offset = 0;
    } else {
        log_hal_msgid_info("Hal_audio_put_param_msg_type_error", 0);
        assert(0);
        return NULL;
    }

    memcpy(audio_share_buffer.mcu2dsp_param + offset, p_param_addr, param_size);
    return (void *)(audio_share_buffer.mcu2dsp_param + offset);
}

//== OS task related ==
void hal_audio_set_task_notification_callback(hal_audio_notify_task_callback_t callback)
{
    audio_isr.audio_event_task_callback = callback;
}

void hal_audio_set_task_ms_delay_function(hal_audio_task_ms_delay_function_t delay_func)
{
    audio_isr.task_ms_delay_func = delay_func;
}

//== Audio Service related ==
void hal_audio_service_hook_callback(audio_message_type_t type, hal_audio_callback_t callback, void *user_data)
{
    uint32_t savedmask;

    if (type >= AUDIO_MESSAGE_TYPE_MAX) {
        log_hal_msgid_info("Hal_audio_service_hook_callback_error", 0);
        assert(0);
        return;
    }

    hal_nvic_save_and_set_interrupt_mask(&savedmask);

    audio_isr.flag |= (1 << type);
    audio_isr.callback[type] = callback;
    audio_isr.user_data[type] = user_data;

    hal_nvic_restore_interrupt_mask(savedmask);
}

void hal_audio_service_unhook_callback(audio_message_type_t type)
{
    uint32_t savedmask;

    if (type >= AUDIO_MESSAGE_TYPE_MAX) {
        log_hal_msgid_info("Hal_audio_service_unhook_unkown_msg_type", 0);
        assert(0);
        return;
    }
    if (!(audio_isr.flag & (1 << type))) {
        log_hal_msgid_info("Hal_audio_service_unhook_flag_error", 0);
        assert(0);
    }

    hal_nvic_save_and_set_interrupt_mask(&savedmask);

    audio_isr.flag &= ~(1 << type);
    audio_isr.callback[type] = NULL;
    audio_isr.user_data[type] = NULL;

    hal_nvic_restore_interrupt_mask(savedmask);
}


//== Hardware semaphore ==
#define MAX_HW_SEMA_RETRY_COUNT 100
#define HW_SEMAPHORE_AUDIO_CM4_DSP0_PLAYBACK 6

static void hal_audio_take_hw_semaphore(uint32_t *p_int_mask)
{
#ifdef HAL_HW_SEMAPHORE_MODULE_ENABLED
    uint32_t take_count = 0;

    //hal_nvic_save_and_set_interrupt_mask(p_int_mask);

    while (++take_count) {
        hal_nvic_save_and_set_interrupt_mask(p_int_mask);    /*change for System Checking*/
        if (HAL_HW_SEMAPHORE_STATUS_OK == hal_hw_semaphore_take(HW_SEMAPHORE_AUDIO_CM4_DSP0_PLAYBACK)) {
            break;
        }
        if (take_count > MAX_HW_SEMA_RETRY_COUNT) {
            hal_nvic_restore_interrupt_mask(*p_int_mask);

            //error handling
            log_hal_msgid_info("[Aud] Can not take HW Semaphore", 0);
            assert(0);
        }
        hal_nvic_restore_interrupt_mask(*p_int_mask);      /*change for System Checking*/
        if (HAL_NVIC_QUERY_EXCEPTION_NUMBER == HAL_NVIC_NOT_EXCEPTION) {
            hal_audio_delay_ms(2);
        } else {
            hal_gpt_delay_us(10);
        }
    }
#endif
}

static void hal_audio_give_hw_semaphore(uint32_t int_mask)
{
#ifdef HAL_HW_SEMAPHORE_MODULE_ENABLED
    if (HAL_HW_SEMAPHORE_STATUS_OK == hal_hw_semaphore_give(HW_SEMAPHORE_AUDIO_CM4_DSP0_PLAYBACK)) {
        hal_nvic_restore_interrupt_mask(int_mask);
    } else {
        hal_nvic_restore_interrupt_mask(int_mask);

        //error handling
        log_hal_msgid_info("[Aud] Can not give HW Semaphore", 0);
        assert(0);
    }
#endif
}

//== Buffer management related ==
uint32_t hal_audio_buf_mgm_get_data_byte_count(n9_dsp_share_info_t *p_info)
{
    uint32_t read, write, data_byte_count;
    uint32_t int_mask;

    hal_audio_take_hw_semaphore(&int_mask);

    if (p_info->bBufferIsFull) {
        hal_audio_give_hw_semaphore(int_mask);
        return p_info->length;
    }

    read = p_info->read_offset;
    write = p_info->write_offset;

    if (write >= read) {
        data_byte_count = write - read;
    } else {
        data_byte_count = p_info->length - read + write;
    }

    hal_audio_give_hw_semaphore(int_mask);

    return data_byte_count;
}

uint32_t hal_audio_buf_mgm_get_free_byte_count(n9_dsp_share_info_t *p_info)
{
    uint32_t data_byte_count, free_byte_count;

    if (p_info->bBufferIsFull) {
        return 0;
    }

    data_byte_count = hal_audio_buf_mgm_get_data_byte_count(p_info);
    free_byte_count = p_info->length - data_byte_count;

    return free_byte_count;
}

void hal_audio_buf_mgm_get_free_buffer(
    n9_dsp_share_info_t *p_info,
    uint8_t **pp_buffer,
    uint32_t *p_byte_count)
{
    uint32_t read, write, segment;
    uint32_t int_mask;

    hal_audio_take_hw_semaphore(&int_mask);

    read = p_info->read_offset;
    write = p_info->write_offset;

    if (p_info->bBufferIsFull) {
        *pp_buffer = (uint8_t *)(p_info->start_addr + write);
        *p_byte_count = 0;
    } else {
        if (write >= read) {
            segment = p_info->length - write;
        } else {
            segment = read - write;
        }

        *pp_buffer = (uint8_t *)(p_info->start_addr + write);
        *p_byte_count = segment;
    }

    hal_audio_give_hw_semaphore(int_mask);
}

void hal_audio_buf_mgm_get_data_buffer(
    n9_dsp_share_info_t *p_info,
    uint8_t **pp_buffer,
    uint32_t *p_byte_count)
{
    uint32_t read, write, segment;
    uint32_t int_mask;

    hal_audio_take_hw_semaphore(&int_mask);

    read = p_info->read_offset;
    write = p_info->write_offset;
    if ((read == write) && (p_info->bBufferIsFull == true)) {
        segment = p_info->length - read;
    } else if (write >= read) {
        segment = write - read;
    } else {
        segment = p_info->length - read;
    }

    *pp_buffer = (uint8_t *)(p_info->start_addr + read);
    *p_byte_count = segment;

    hal_audio_give_hw_semaphore(int_mask);
}

void hal_audio_buf_mgm_get_write_data_done(n9_dsp_share_info_t *p_info, uint32_t byte_count)
{
    uint32_t write;
    uint32_t int_mask;

    hal_audio_take_hw_semaphore(&int_mask);

    write = p_info->write_offset;
    write += byte_count;
    if (write >= p_info->length) {
        write -= p_info->length;
    }
    p_info->write_offset = write;

    if ((p_info->write_offset == p_info->read_offset) && (byte_count)) {
        p_info->bBufferIsFull = 1;
    }

    hal_audio_give_hw_semaphore(int_mask);
}

void hal_audio_buf_mgm_get_read_data_done(n9_dsp_share_info_t *p_info, uint32_t byte_count)
{
    uint32_t read;
    uint32_t int_mask;

    hal_audio_take_hw_semaphore(&int_mask);

    read = p_info->read_offset;
    read += byte_count;
    if (read >= p_info->length) {
        read -= p_info->length;
    }
    p_info->read_offset = read;

    if ((p_info->bBufferIsFull == 1) && (byte_count != 0)) {
        p_info->bBufferIsFull = 0;
    }

    hal_audio_give_hw_semaphore(int_mask);
}

//== Share buffer ==

#if defined(AIR_ANC_USER_UNAWARE_ENABLE) || defined(AIR_ANC_WIND_DETECTION_ENABLE) || defined(AIR_ANC_ENVIRONMENT_DETECTION_ENABLE)
uint32_t *hal_audio_query_ANC_monitor_share_info(void)
{
    return (uint32_t *)&g_ANC_monitor_share_buffer;
}
#endif

#ifdef MTK_BT_SPEAKER_ENABLE
uint8_t *hal_audio_query_fec_share_info(void)
{
    return (uint8_t *)&fec_share_buffer;
}
#endif

n9_dsp_share_info_t *hal_audio_query_bt_audio_dl_share_info(void)
{
    return &n9_dsp_share_info[SHARE_BUFFER_INFO_INDEX_BT_AUDIO_DL];
}

n9_dsp_share_info_t *hal_audio_query_bt_voice_ul_share_info(void)
{
#ifdef AVM_SHAEE_BUF_INFO
    hal_audio_set_forwarder_addr((avm_share_buf_info_t *)&n9_dsp_share_info[SHARE_BUFFER_INFO_INDEX_BT_VOICE_UL], FALSE);
#endif
    return &n9_dsp_share_info[SHARE_BUFFER_INFO_INDEX_BT_VOICE_UL];
}

n9_dsp_share_info_t *hal_audio_query_bt_voice_dl_share_info(void)
{
#ifdef AVM_SHAEE_BUF_INFO
    hal_audio_set_forwarder_addr((avm_share_buf_info_t *)&n9_dsp_share_info[SHARE_BUFFER_INFO_INDEX_BT_VOICE_DL], TRUE);
#endif
    return &n9_dsp_share_info[SHARE_BUFFER_INFO_INDEX_BT_VOICE_DL];
}

#ifdef AIR_BT_CODEC_BLE_ENABLED
n9_dsp_share_info_t *hal_audio_query_ble_audio_ul_share_info(void)
{
    return &n9_dsp_share_info[SHARE_BUFFER_INFO_INDEX_BLE_AUDIO_UL];
}

n9_dsp_share_info_t *hal_audio_query_ble_audio_dl_share_info(void)
{
    return &n9_dsp_share_info[SHARE_BUFFER_INFO_INDEX_BLE_AUDIO_DL];
}

n9_dsp_share_info_t *hal_audio_query_ble_audio_sub_ul_share_info(void)
{
    return &n9_dsp_share_info[SHARE_BUFFER_INFO_INDEX_BLE_AUDIO_SUB_UL];
}

n9_dsp_share_info_t *hal_audio_query_ble_audio_sub_dl_share_info(void)
{
    return &n9_dsp_share_info[SHARE_BUFFER_INFO_INDEX_BLE_AUDIO_SUB_DL];
}

#endif

n9_dsp_share_info_t *hal_audio_query_playback_share_info(void)
{
    //ToDo: reuse BT audio DL buffer
    return &n9_dsp_share_info[SHARE_BUFFER_INFO_INDEX_BT_AUDIO_DL];
}

n9_dsp_share_info_t *hal_audio_query_record_share_info(void)
{
    //ToDo: currently, there is not dedicated buffer for recording.
    //ToDo: reuse BT audio DL buffer
    return &n9_dsp_share_info[SHARE_BUFFER_INFO_INDEX_RECORD];
}

uint32_t *hal_audio_query_rcdc_share_info(void)
{
    return &audio_share_buffer.clk_info[0];
}

uint32_t *hal_audio_query_ull_rcdc_share_info(void)
{
    return &audio_share_buffer.ull_clk_info[0];
}

uint32_t *hal_audio_query_hfp_air_dump(void)
{
    return &audio_share_buffer.airdump[0];
}

#ifdef AIR_ADAPTIVE_EQ_ENABLE
uint32_t *hal_audio_adaptive_eq_index_share_info(void)
{
    return &audio_share_buffer.adaptive_eq_index[0];
}
#endif

AUDIO_SYNC_INFO *hal_audio_query_audio_sync_info(void)
{
    return (AUDIO_SYNC_INFO *)&audio_share_buffer.audio_sync_info[0];
}

uint32_t *hal_audio_afe_dl_buf_report(void)
{
    return (uint32_t *)&n9_dsp_share_info[SHARE_BUFFER_INFO_INDEX_BT_VOICE_DL].drift_comp_val;
}

n9_dsp_share_info_t *hal_audio_query_prompt_share_info(void)
{
    return &n9_dsp_share_info[SHARE_BUFFER_INFO_INDEX_PROMPT];
}

n9_dsp_share_info_t *hal_audio_query_nvkey_parameter_share_info(void)
{
    return &n9_dsp_share_info[SHARE_BUFFER_INFO_INDEX_NVKEY_PARAMETER];
}

n9_dsp_share_info_t *hal_audio_query_audio_transmitter_share_info(audio_transmitter_share_info_index_t index)
{
    return &audio_transmitter_share_info[index];
}

uint32_t *hal_audio_query_share_info(audio_message_type_t type)
{
    switch (type) {
        case AUDIO_MESSAGE_TYPE_BT_AUDIO_DL:
            return (uint32_t *)hal_audio_query_bt_audio_dl_share_info();
#ifdef AIR_BT_CODEC_BLE_ENABLED
        case AUDIO_MESSAGE_TYPE_BLE_AUDIO_DL:
            return (uint32_t *)hal_audio_query_ble_audio_dl_share_info();
        case AUDIO_MESSAGE_TYPE_BLE_AUDIO_UL:
            return (uint32_t *)hal_audio_query_ble_audio_ul_share_info();
        case AUDIO_MESSAGE_TYPE_BLE_AUDIO_SUB_DL:
            return (uint32_t *)hal_audio_query_ble_audio_sub_dl_share_info();
        case AUDIO_MESSAGE_TYPE_BLE_AUDIO_SUB_UL:
            return (uint32_t *)hal_audio_query_ble_audio_sub_ul_share_info();
#endif
        case AUDIO_MESSAGE_TYPE_BT_VOICE_UL:
            return (uint32_t *)hal_audio_query_bt_voice_ul_share_info();
        case AUDIO_MESSAGE_TYPE_BT_VOICE_DL:
            return (uint32_t *)hal_audio_query_bt_voice_dl_share_info();
        case AUDIO_MESSAGE_TYPE_PLAYBACK:
            return (uint32_t *)hal_audio_query_playback_share_info();
        case AUDIO_MESSAGE_TYPE_RECORD:
            return (uint32_t *)hal_audio_query_record_share_info();
        case AUDIO_MESSAGE_TYPE_PROMPT:
            return (uint32_t *)hal_audio_query_prompt_share_info();
        case AUDIO_RESERVE_TYPE_QUERY_RCDC:
            return hal_audio_query_rcdc_share_info();
        case AUDIO_RESERVE_TYPE_ULL_QUERY_RCDC:
            return hal_audio_query_ull_rcdc_share_info();
#if defined(MTK_ANC_SURROUND_MONITOR_ENABLE)
        case AUDIO_MESSAGE_TYPE_ANC_MONITOR_ADAPTIVE_ANC:
            return (uint32_t *)hal_audio_query_ANC_monitor_share_info();
#endif
#if defined(AIR_ADAPTIVE_EQ_ENABLE)
        case AUDIO_MESSAGE_TYPE_AEQ_MONITOR_ADAPTIVE_EQ:
            return (uint32_t *)hal_audio_adaptive_eq_index_share_info();
#endif
        default:
            return NULL;

    }
}
#ifdef AVM_SHAEE_BUF_INFO


#define AVM_HEADER_LEN 12
void hal_audio_set_avm_info(avm_share_buf_info_t *p_info, uint32_t buf_len,  uint16_t blksize, uint32_t sink_latnecy)
{
    uint16_t i;
    p_info->ReadIndex  = 0;
    p_info->WriteIndex = 0;
    p_info->MemBlkSize = ((blksize + AVM_HEADER_LEN + 7) >> 3) << 3;
    p_info->MemBlkNum  = (uint16_t)(buf_len / (p_info->MemBlkSize));
    p_info->SinkLatency = sink_latnecy;
    for (i = 0; i < p_info->MemBlkNum; i++) {
        memset((void *)(p_info->StartAddr + i * p_info->MemBlkSize), 0, AVM_HEADER_LEN);
    }
}

void hal_audio_set_hfp_avm_info(avm_share_buf_info_t *p_info, uint32_t buf_len,  uint16_t blksize)
{
    p_info->ReadIndex  = 0;
    p_info->WriteIndex = 0;
    p_info->MemBlkSize = blksize;
    p_info->MemBlkNum  = (uint16_t)(buf_len / (p_info->MemBlkSize));
}

void hal_audio_set_forwarder_addr(avm_share_buf_info_t *p_info, bool isRx)
{
    if (isRx == TRUE) {
        p_info->ForwarderAddr = (uint32_t)bt_pka_get_esco_forwarder_addr(SHARE_BUFFER_SYSRAM3_AUDIO_FORWARDER_BUF_SIZE);
    } else {
        p_info->ForwarderAddr = (uint32_t)bt_pka_get_esco_forwarder_addr(SHARE_BUFFER_SYSRAM3_AUDIO_FORWARDER_BUF_SIZE) + SHARE_BUFFER_SYSRAM3_RX_AUDIO_FORWARDER_BUF_SIZE;
    }
}

void hal_audio_set_gaming_mode_avm_info(avm_share_buf_info_t *p_info, uint32_t buf_len,  uint16_t blksize)
{
    p_info->ReadIndex  = 0;
    p_info->WriteIndex = 0;
    p_info->MemBlkSize = blksize;
    p_info->MemBlkNum  = (uint16_t)(buf_len / (p_info->MemBlkSize));
}
#endif

void hal_audio_set_audio_transmitter_share_info(n9_dsp_share_info_t *p_share_info, uint32_t buf_len, uint32_t max_payload_size)
{
    uint16_t blk_num, blk_size;
    uint8_t *p;
    blk_size = (max_payload_size + 4 + 3) / 4 * 4; //add 4 byte header, 4B align
    blk_num = buf_len / blk_size;
    p = (uint8_t *) & (p_share_info->sub_info.next);
    memcpy(p, (uint8_t *)&blk_size, 2);
    memcpy(p + 2, (uint8_t *)&blk_num, 2);
    p_share_info->length = blk_size * blk_num;
}

void hal_audio_a2dp_reset_share_info(n9_dsp_share_info_t *p_info)
{
    p_info->read_offset = 0;
    p_info->sub_info.next = 0;
    p_info->sampling_rate = 0;
    //p_info->length = 0;
    p_info->notify_count = 0;
    p_info->drift_comp_val = 0;
    p_info->anchor_clk = 0;

    hal_audio_dsp_controller_send_message(MSG_MCU2DSP_COMMON_SET_SYSRAM, 0, (uint32_t)&audio_share_buffer.nvkey_param[0], false);
}

void hal_audio_set_a2dp_play_en(uint32_t seq_no, uint32_t bt_clk)
{
    audio_play_en_info.sequence_number = seq_no;
    audio_play_en_info.bt_clock = bt_clk;
    hal_audio_dsp_controller_send_message(MSG_MCU2DSP_BT_AUDIO_DL_PLAY_EN_FROM_BTCON, 0, (uint32_t)&audio_play_en_info, false);
}

#ifdef AIR_BT_CODEC_BLE_ENABLED

void hal_audio_reset_le_audio_info(void)
{
    n9_dsp_share_info_t *p_info = (n9_dsp_share_info_t *)hal_audio_query_share_info(AUDIO_MESSAGE_TYPE_BLE_AUDIO_DL);
    memset((void *)p_info->start_addr, 0, SHARE_BUFFER_BLE_AUDIO_DL_SIZE);
    p_info = (n9_dsp_share_info_t *)hal_audio_query_share_info(AUDIO_MESSAGE_TYPE_BLE_AUDIO_UL);
    memset((void *)p_info->start_addr, 0, SHARE_BUFFER_BLE_AUDIO_UL_SIZE);
}
#endif

void hal_audio_reset_share_info(n9_dsp_share_info_t *p_info)
{
    p_info->write_offset = 0;
    p_info->bBufferIsFull = 0;
    hal_audio_a2dp_reset_share_info(p_info);
}

void hal_audio_set_sysram(void)
{
    hal_audio_dsp_controller_send_message(MSG_MCU2DSP_COMMON_SET_SYSRAM, 0, (uint32_t)&audio_share_buffer.nvkey_param[0], false);
}

uint32_t *hal_audio_query_ltcs_asi_buf(void)
{
    return (uint32_t *)(4 * ((n9_dsp_share_info[SHARE_BUFFER_INFO_INDEX_BT_VOICE_UL].start_addr + 3) / 4));
}

uint32_t *hal_audio_query_ltcs_min_gap_buf(void)
{
    return (uint32_t *)(4 * ((n9_dsp_share_info[SHARE_BUFFER_INFO_INDEX_BT_VOICE_DL].start_addr + 4) / 4));
}

uint32_t *hal_audio_query_ltcs_anchor_info_buf(void)
{
    return &audio_share_buffer.tx_audio_forwarder_buf[0];
}

uint32_t *hal_audio_query_race_cmd_audio_buf(void)
{
    return &audio_share_buffer.race_cmd_audio_buf[0];
}

uint32_t *hal_audio_report_bitrate_buf(void)
{
    return (uint32_t *)(4 * ((n9_dsp_share_info[SHARE_BUFFER_INFO_INDEX_BT_VOICE_UL].start_addr + n9_dsp_share_info[SHARE_BUFFER_INFO_INDEX_BT_VOICE_UL].length - 8) / 4));
}

uint32_t *hal_audio_report_lostnum_buf(void)
{
    return &(n9_dsp_share_info[SHARE_BUFFER_INFO_INDEX_BT_VOICE_UL].sub_info.next);
}

/**
 * @ Write audio drift to DSP.
 * @ val : updated drift value.
 * @ Retval HAL_AUDIO_STATUS_OK if operation is successful, others if failed.
 */
hal_audio_status_t hal_audio_write_audio_drift_val(int32_t val)
{
    ltcs_anchor_info_t *p_info = (ltcs_anchor_info_t *)hal_audio_query_ltcs_anchor_info_buf();

    p_info->drift_comp_val = val;

    return HAL_AUDIO_STATUS_OK;
}

/**
 * @ Write audio anchor to DSP.
 * @ val : updated drift value.
 * @ Retval HAL_AUDIO_STATUS_OK if operation is successful, others if failed.
 */
hal_audio_status_t hal_audio_write_audio_anchor_clk(uint32_t val)
{
    ltcs_anchor_info_t *p_info = (ltcs_anchor_info_t *)hal_audio_query_ltcs_anchor_info_buf();

    p_info->anchor_clk = val;

    return HAL_AUDIO_STATUS_OK;
}

hal_audio_status_t hal_audio_write_audio_asi_base(uint32_t val)
{
    ltcs_anchor_info_t *p_info = (ltcs_anchor_info_t *)hal_audio_query_ltcs_anchor_info_buf();

    p_info->asi_base = val;

    return HAL_AUDIO_STATUS_OK;
}

hal_audio_status_t hal_audio_write_audio_asi_cur(uint32_t val)
{
    ltcs_anchor_info_t *p_info = (ltcs_anchor_info_t *)hal_audio_query_ltcs_anchor_info_buf();

    p_info->asi_cur = val;

    return HAL_AUDIO_STATUS_OK;
}

extern HAL_DSP_PARA_AU_AFE_CTRL_t audio_nvdm_HW_config;

/********************************************* Clock CG Usage Example **************************************************
*    1. First enable power
*    if (control) {
*       2. Check the count of clock gate
*       hal_audio_status_check_clock_gate(type, CLOCK_CG, control);
*       if (!hal_audio_status_get_clock_gate_status(CLOCK_CG)) {
*          // Power on Clock CG
*          // Enable Power
*       }
*       hal_audio_status_set_clock_gate(type, CLOCK_CG, control);
*       hal_audio_status_log_clock_gate_setting(type, CLOCK_CG, control);
*    } else {
*        uint32_t count = hal_audio_status_get_clock_gate_setting(type);
*        if (count & (1UL << CLOCK_CG)) {
*            3. Check the count of clock gate
*            hal_audio_status_check_clock_gate(type, CLOCK_CG, control);
*            hal_audio_status_log_clock_gate_setting(type, CLOCK_CG, control);
*            hal_audio_status_set_clock_gate(type, CLOCK_CG, control);
*            4. Last disable power
*            if (!hal_audio_status_get_clock_gate_status(CLOCK_CG)) {
*                // Disable Power
*                // Code
*            }
*        }
*    }
***********************************************************************************************************************/

typedef enum {
    CHECK_MODE_EQUAL = 0,      /* equal */
    CHECK_MODE_LESS,           /* less than */
    CHECK_MODE_MORE,           /* more than */
    CHECK_MODE_LESS_EQUAL,     /* less than and equal */
    CHECK_MODE_MORE_EQUAL      /* more than and equal */
} rg_check_mode_t;

/**
 * @brief     This function is used to check the register value and the input value.
 * @param[in] rg_addr is to indicate register address.
 * @param[in] rg_value is input value.
 * @param[in] bit_mask indicate which bits of register are cared.
 * @param[in] mode comparison mode (equal/less than/more than/less than and equal/more than and equal).
 * @return    NONE.
 */
void hal_audio_status_check_rg_value(uint32_t rg_addr, uint32_t rg_value, uint32_t bit_mask, rg_check_mode_t mode)
{
    uint32_t value = *((volatile uint32_t *)(rg_addr));

    switch (mode) {
        case CHECK_MODE_EQUAL:
            if ((value & bit_mask) == (rg_value & bit_mask)) {
                return;
            }
            break;
        case CHECK_MODE_LESS:
            if ((value & bit_mask) < (rg_value & bit_mask)) {
                return;
            }
            break;
        case CHECK_MODE_MORE:
            if ((value & bit_mask) > (rg_value & bit_mask)) {
                return;
            }
            break;
        case CHECK_MODE_LESS_EQUAL:
            if ((value & bit_mask) <= (rg_value & bit_mask)) {
                return;
            }
            break;
        case CHECK_MODE_MORE_EQUAL:
            if ((value & bit_mask) >= (rg_value & bit_mask)) {
                return;
            }
            break;
        default:
            log_hal_msgid_error("[Audio Clock] ERROR: check mode error", 0);
            assert(0);
            break;
    }
    log_hal_msgid_error("[Audio Clock] ERROR:rg[0x%x] check fail! cur_value[0x%x] target value[0x%x] bit mask[0x%x] mode %d", 5,
                        rg_addr,
                        value,
                        rg_value,
                        bit_mask,
                        mode
                       );
    assert(0);
}

/**
 * @brief     This function is used to enable or disable the assigned clock gate when this scenario start!
 * @param[in] type is to indicate the audio scenario which will enable or disable the clock gate.
 * @param[in] cg_type is the clock gate that will be enable or disable.
 * @param[in] control enable or disable clock gate.
 * @return    NONE.
 */
static void hal_audio_status_set_clock_gate(audio_scenario_type_t type, audio_clock_setting_type_t cg_type, bool control)
{
    if ((type < 0) || (type >= AUDIO_SCENARIO_TYPE_END) || (cg_type >= AUDIO_POWER_END) || (cg_type < 0)) {
        log_hal_msgid_error("[Audio Clock] ERROR: type %d cg_type %d", 2,
                            type,
                            cg_type
                           );
        assert(0);
        return;
    }
    // get index
    uint32_t index = type / 32;
    uint32_t bit_mask = type % 32;
    if (control) {
        if (g_dsp_controller_cg[cg_type][index] & (1 << bit_mask)) {
            log_hal_msgid_error("[Audio Clock] ERROR: clock [%d] type [%d] is already enable", 2,
                                cg_type,
                                type);
            log_hal_msgid_error("[Audio Clock] CG %d: [0x%x][0x%x][0x%x][0x%x]", 5,
                                cg_type,
                                g_dsp_controller_cg[cg_type][3],
                                g_dsp_controller_cg[cg_type][2],
                                g_dsp_controller_cg[cg_type][1],
                                g_dsp_controller_cg[cg_type][0]
                               );
            assert(0);
            return;
        }
        g_dsp_controller_cg[cg_type][index] |= (1 << bit_mask);
    } else {
        if ((g_dsp_controller_cg[cg_type][index] & (1 << bit_mask)) == 0) {
            log_hal_msgid_error("[Audio Clock] ERROR: clock [%d] type [%d] is already disable", 2,
                                cg_type,
                                type);
            log_hal_msgid_error("[Audio Clock] CG %d: [0x%x][0x%x][0x%x][0x%x]", 5,
                                cg_type,
                                g_dsp_controller_cg[cg_type][3],
                                g_dsp_controller_cg[cg_type][2],
                                g_dsp_controller_cg[cg_type][1],
                                g_dsp_controller_cg[cg_type][0]
                               );
            assert(0);
            return;
        }
        g_dsp_controller_cg[cg_type][index] &= ~(1 << bit_mask);
    }
}

/**
 * @brief     This function is used to get the status of the clock gate!
 * @param[in] cg_type cg_type is the clock gate.
 * @return    Return #true when this clock gate is used by some scenario.
 *            Return #false when this clock gate is not used by any scenario.
 */
static bool hal_audio_status_get_clock_gate_status(audio_clock_setting_type_t cg_type)
{
    for (uint32_t i = 0; i <= (AUDIO_SCENARIO_TYPE_END / 32); i++) {
        if (g_dsp_controller_cg[cg_type][i] != 0) {
            return true; // means this clock gate is enabled by some scenario
        }
    }
    return false; // this clock gate is not used
}

/**
 * @brief     This function is used to watch whether this clock gate is used by this scenario!
 * @param[in] type is to indicate the audio scenario.
 * @param[in] cg_type is the clock gate.
 * @return    Return #true when this clock gate is used by this scenario.
 *            Return #false when this clock gate is not used by this scenario.
 */
static bool hal_audio_status_check_clock_gate_status(audio_scenario_type_t type, audio_clock_setting_type_t cg_type)
{
    uint32_t index = type / 32;
    uint32_t bit_mask = type % 32;
    if (g_dsp_controller_cg[cg_type][index] & (1 << bit_mask)) {
        return true;
    }
    return false; // this clock gate is not used
}

/**
 * @brief     This function is used to logging the clock setting of scenario.
 * @param[in] type is to indicate the audio scenario type.
 * @param[in] cg_type is to indicate clock setting type.
 * @param[in] control enable or disable.
 * @return    NONE.
 */
void hal_audio_status_log_clock_gate_setting(audio_scenario_type_t type, audio_clock_setting_type_t cg_type, bool control)
{
    // get the count
    uint32_t cnt = type;
    if (control) {
        g_dsp_controller_cg_control_bit[cnt] |= (1 << cg_type);
    } else {
        g_dsp_controller_cg_control_bit[cnt] &= ~(1 << cg_type);
    }
}

/**
 * @brief     This function is used to get the clock settings of scenario.
 * @param[in] type is to indicate the audio scenario type.
 * @return    The clock settings of scenario.
 */
static uint32_t hal_audio_status_get_clock_gate_setting(audio_scenario_type_t type)
{
    uint32_t cnt = type;
    return g_dsp_controller_cg_control_bit[cnt];
}

/**
 * @brief     This function is used to control the releated clock settings or power of dac by scenario.
 * @param[in] type is to indicate the audio scenario type.
 * @param[in] param is the pointer of open parameters.
 * @param[in] control enable or disable.
 * @return    NONE.
 */
void hal_audio_status_set_dac(audio_scenario_type_t type, mcu2dsp_open_param_t *param, bool control)
{
    if ((param == NULL) && (control)) {
        return;
    }
    bool vaud18_use_flag = false; // only open VAUD18 when DAC is used.
    if (control) {
        hal_audio_device_t out_device = param->stream_out_param.afe.audio_device;
        if (out_device & HAL_AUDIO_DEVICE_DAC_DUAL) {
            if (!hal_audio_status_get_clock_gate_status(AUDIO_POWER_DAC)) { // Power on DAC
                switch (audio_nvdm_HW_config.adc_dac_config.ADDA_DAC_Mode_Sel) {
                    // 0x1: PMU_Class_AB
                    case 0x1:
                        pmu_set_audio_mode(PMU_CLASSAB, PMU_ON);
                        vaud18_use_flag = true;
                        break;
                    // 0x2: PMU_Class_D
                    case 0x2:
                        pmu_set_audio_mode(PMU_CLASSD, PMU_ON);
                        vaud18_use_flag = true;
                        break;
                    // 0x3: PMU_Class_G3
                    case 0x3:
                        pmu_set_audio_mode(PMU_CLASSG3, PMU_ON);
                        vaud18_use_flag = true;
                        break;
                    // 0x0: PMU_CLASSG2
                    case 0x0:
                        pmu_set_audio_mode(PMU_CLASSG2, PMU_ON);
                        vaud18_use_flag = true;
                        break;
                    default:
                        // TODO: verify the power consumption
                        log_hal_msgid_warning("[Audio Clock] dac class nvkey invalid %d",
                                              1, audio_nvdm_HW_config.adc_dac_config.ADDA_DAC_Mode_Sel);
                        // pmu_set_audio_mode(PMU_CLASSG2, PMU_ON);
                        break;
                }
#ifdef HAL_PMU_MODULE_ENABLED
#ifndef AIR_AUDIO_EXT_DAC_ENABLE
                if (vaud18_use_flag) {
                    pmu_enable_power(PMU_BUCK_VAUD18, PMU_ON);
                }
#endif /* AIR_AUDIO_EXT_DAC_ENABLE */
#endif /* HAL_PMU_MODULE_ENABLED */
                // AMP is for AMP delay, we can only power off DAC after DSP notifying!
                hal_audio_status_set_clock_gate(AUDIO_SCENARIO_TYPE_AMP, AUDIO_POWER_DAC, true);
                hal_audio_status_log_clock_gate_setting(AUDIO_SCENARIO_TYPE_AMP, AUDIO_POWER_DAC, control);
                log_hal_msgid_info("[Audio Clock] enable dac type %d", 1,
                                   type);
            }
            hal_audio_status_set_clock_gate(type, AUDIO_POWER_DAC, true);
            hal_audio_status_log_clock_gate_setting(type, AUDIO_POWER_DAC, control);
        }
    } else {
        uint32_t count = hal_audio_status_get_clock_gate_setting(type);
        if (count & (1 << AUDIO_POWER_DAC)) {
            hal_audio_status_set_clock_gate(type, AUDIO_POWER_DAC, false);
            hal_audio_status_log_clock_gate_setting(type, AUDIO_POWER_DAC, control);
            log_hal_msgid_info("[Audio Clock] disable dac type %d", 1,
                               type);
            if (!hal_audio_status_get_clock_gate_status(AUDIO_POWER_DAC)) { // Power down DAC
                switch (audio_nvdm_HW_config.adc_dac_config.ADDA_DAC_Mode_Sel) {
                    case 0x1:
                        pmu_set_audio_mode(PMU_CLASSAB, PMU_OFF);
                        vaud18_use_flag = true;
                        break;
                    case 0x2:
                        pmu_set_audio_mode(PMU_CLASSD, PMU_OFF);
                        vaud18_use_flag = true;
                        break;
                    case 0x3:
                        pmu_set_audio_mode(PMU_CLASSG3, PMU_OFF);
                        vaud18_use_flag = true;
                        break;
                    case 0x0:
                        pmu_set_audio_mode(PMU_CLASSG2, PMU_OFF);
                        vaud18_use_flag = true;
                        break;
                    default:
                        //pmu_set_audio_mode(PMU_CLASSG2, PMU_OFF);
                        break;
                }
#ifdef HAL_PMU_MODULE_ENABLED
#ifndef AIR_AUDIO_EXT_DAC_ENABLE
                if (vaud18_use_flag) {
                    pmu_enable_power(PMU_BUCK_VAUD18, PMU_OFF);
                }
#endif /* AIR_AUDIO_EXT_DAC_ENABLE */
#endif /* HAL_PMU_MODULE_ENABLED */
            }
        }
    }
}

/**
 * @brief     This function is used to control the releated clock settings or power of micbias by scenario.
 * @param[in] type is to indicate the audio scenario type.
 * @param[in] param is the pointer of open parameters.
 * @param[in] control enable or disable.
 * @return    NONE.
 */
void hal_audio_status_set_micbias(audio_scenario_type_t type, mcu2dsp_open_param_t *param, bool control)
{
    if ((param == NULL) && (control)) {
        return;
    }
    uint32_t i = 0;
    uint32_t ldo_voltage[AUDIO_POWER_MICBIAS_END - AUDIO_POWER_MICBIAS_0 + 1];
    ldo_voltage[0] = audio_nvdm_HW_config.adc_dac_config.ADDA_Voice_Bias0_Level;
    ldo_voltage[1] = audio_nvdm_HW_config.adc_dac_config.ADDA_Voice_Bias1_Level;
    ldo_voltage[2] = audio_nvdm_HW_config.adc_dac_config.ADDA_Voice_Bias2_Level;
    uint32_t micbias_nuber = AUDIO_POWER_MICBIAS_END - AUDIO_POWER_MICBIAS_0;
    for (i = 0; i <= micbias_nuber; i++) { // micbias total number: 3
        if (control) {
            // check stream in type
            hal_audio_device_t in_device = param->stream_in_param.afe.audio_device;
            uint8_t bias_select = param->stream_in_param.afe.bias_select;
            // Only AMIC/DMIC/LINEIN will use micbias
            if ((param->param.stream_in == STREAM_IN_AFE) &&
                ((in_device & HAL_AUDIO_DEVICE_MAIN_MIC_DUAL) ||
                 (in_device & HAL_AUDIO_DEVICE_DIGITAL_MIC_DUAL))) {
                if (bias_select & (1 << i)) {
                    if (!hal_audio_status_get_clock_gate_status(AUDIO_POWER_MICBIAS_0 + i)) { // Power on micbias
                        // NOTE: We must set voltage and then power on the LDO.
                        pmu_set_micbias_vout(PMIC_MICBIAS0 + i, ldo_voltage[i]);
                        if (audio_nvdm_HW_config.adc_dac_config.ADDA_Voice_Bias012_share_LDO) {
                            pmu_enable_micbias(PMIC_MICBIAS_LDO0, PMIC_MICBIAS_ALL, PMIC_MICBIAS_NM, PMU_ON);
                        } else {
                            pmu_enable_micbias(PMIC_MICBIAS_LDO0 + i, PMIC_MICBIAS0 + i, PMIC_MICBIAS_NM, PMU_ON);
                        }
                    }
                    hal_audio_status_set_clock_gate(type, AUDIO_POWER_MICBIAS_0 + i, control); // only set bit
                    hal_audio_status_log_clock_gate_setting(type, AUDIO_POWER_MICBIAS_0 + i, control);
                }
                log_hal_msgid_info("[Audio Clock] micbias_%d type %d control %d bias_select 0x%x voltage 0x%x share_ldo0 0x%x", 6,
                                   i,
                                   type,
                                   control,
                                   bias_select,
                                   ldo_voltage[i],
                                   audio_nvdm_HW_config.adc_dac_config.ADDA_Voice_Bias012_share_LDO);
            }
        } else {
            uint32_t count = hal_audio_status_get_clock_gate_setting(type);
            if (count & (1UL << (AUDIO_POWER_MICBIAS_0 + i))) {
                hal_audio_status_log_clock_gate_setting(type, AUDIO_POWER_MICBIAS_0 + i, control);
                hal_audio_status_set_clock_gate(type, AUDIO_POWER_MICBIAS_0 + i, control);
                if (!hal_audio_status_get_clock_gate_status(AUDIO_POWER_MICBIAS_0 + i)) {
                    if (audio_nvdm_HW_config.adc_dac_config.ADDA_Voice_Bias012_share_LDO) {
                        pmu_enable_micbias(PMIC_MICBIAS_LDO0, PMIC_MICBIAS0 + i, PMIC_MICBIAS_NM, PMU_OFF); // Power down micbias
                    } else {
                        pmu_enable_micbias(PMIC_MICBIAS_LDO0 + i, PMIC_MICBIAS0 + i, PMIC_MICBIAS_NM, PMU_OFF); // Power down micbias
                    }
                }
            }
        }
    }
}

/**
 * @brief     This function is used to control the releated clock settings or power of hi-res in Uplink by scenario.
 * @param[in] type is to indicate the audio scenario type.
 * @param[in] param is the pointer of open parameters.
 * @param[in] control enable or disable.
 * @return    NONE.
 */
void hal_audio_status_set_ul_hires(audio_scenario_type_t type, mcu2dsp_open_param_t *param, bool control)
{
    if ((param == NULL) && (control)) {
        return;
    }
    bool hires_flag = false;
    if (control) {
        // check stream_in type, avoid downlink run this code!
        // Only ADC 0/1 support Hi-Res!
        if ((param->param.stream_in != STREAM_IN_AFE) || (param->stream_in_param.afe.audio_interface != HAL_AUDIO_INTERFACE_1)) {
            return;
        }
        uint32_t device_in_rate = param->stream_in_param.afe.sampling_rate;
        hal_audio_device_t in_device = param->stream_in_param.afe.audio_device;
        if (((in_device & HAL_AUDIO_DEVICE_MAIN_MIC_DUAL) && (device_in_rate > 48000)) /* ||
            (hal_audio_status_get_clock_gate_status(AUDIO_CLOCK_UPLINK)) */) {
            if (!hal_audio_status_get_clock_gate_status(AUDIO_CLOCK_UPLINK)) {
#if defined(HAL_DVFS_MODULE_ENABLED)
                hal_dvfs_lock_control(HAL_DVFS_OPP_MID, HAL_DVFS_LOCK);
                log_hal_msgid_info("[Audio Clock] ul hi-res: type %d lock frequency to 0.8V, rate %u, device 0x%x", 3,
                                   type,
                                   device_in_rate,
                                   in_device);
#endif
                // use lock dvfs to ensure clk source
                // clock_mux_sel(CLK_AUD_ULCK_SEL, 1); /* MPLL_D3, 208 MHz */
                hal_clock_enable(HAL_CLOCK_CG_AUD_UPLINK);
                dsp_controller.hi_res_on_ul = control; /* NOT USE */
            }
            hal_audio_status_set_clock_gate(type, AUDIO_CLOCK_UPLINK, control);
            hal_audio_status_log_clock_gate_setting(type, AUDIO_CLOCK_UPLINK, control);
        }
    } else {
        uint32_t count = hal_audio_status_get_clock_gate_setting(type);
        if (count & (1UL << AUDIO_CLOCK_UPLINK)) {
            hal_audio_status_log_clock_gate_setting(type, AUDIO_CLOCK_UPLINK, control);
            hal_audio_status_set_clock_gate(type, AUDIO_CLOCK_UPLINK, control);
            if (!hal_audio_status_get_clock_gate_status(AUDIO_CLOCK_UPLINK)) {
                // Check UL Hi-res mode, AFE_ADDA_UL_SRC_CON0[19:17] <= 3
                hal_audio_status_check_rg_value(AFE_ADDA_UL_SRC_CON0, 0x30000, 0xE0000, CHECK_MODE_LESS_EQUAL);
                hal_clock_disable(HAL_CLOCK_CG_AUD_UPLINK);
                dsp_controller.hi_res_on_ul = control;
#if defined(HAL_DVFS_MODULE_ENABLED)
                hal_dvfs_lock_control(HAL_DVFS_OPP_MID, HAL_DVFS_UNLOCK);
                log_hal_msgid_info("[Audio Clock] ul hi-res: type %d unlock frequency 0.8V", 1,
                                   type);
#endif
            }
        }
    }
    if (hires_flag) {
        log_hal_msgid_info("[Audio Clock] ul hi-res: type %d control %d", 2,
                           type,
                           control);
    }
}

/**
 * @brief     This function is used to control the releated clock settings or power of hi-res in Downlink by scenario.
 * @param[in] type is to indicate the audio scenario type.
 * @param[in] param is the pointer of open parameters.
 * @param[in] control enable or disable.
 * @return    NONE.
 */
void hal_audio_status_set_dl_hires(audio_scenario_type_t type, mcu2dsp_open_param_t *param, bool control)
{
    if ((param == NULL) && (control)) {
        return;
    }
    bool hires_flag = false;
    /* NOTE: We should disable dl hi-res after closing dac! */
    if (control) {
        // check stream_out type, avoid uplink run this code!
        if (param->param.stream_out != STREAM_OUT_AFE) {
            return;
        }
        uint32_t device_out_rate = param->stream_out_param.afe.sampling_rate;
        hal_audio_device_t out_device = param->stream_out_param.afe.audio_device;
        if (((out_device & HAL_AUDIO_DEVICE_DAC_DUAL) && (device_out_rate > 48000)) ||
            (hal_audio_status_get_clock_gate_status(AUDIO_CLOCK_DWLINK))) { // hi-res
            hires_flag = true;
            if (device_out_rate > 96000) {
                if (dsp_controller.clk_mux_hires_dl == DL_HIRES_96KHZ) {
                    log_hal_msgid_error("[Audio Clock] ERROR: DL Hi-Res 96KHz is already on, type %d sample rate %d device 0x%x", 3,
                                        type,
                                        device_out_rate,
                                        out_device);
                    assert(0);
                }
                dsp_controller.clk_mux_hires_dl = DL_HIRES_192KHZ;
                dsp_controller.clk_mux_hires_hwsrc = HWSRC_HIRES_192KHZ;
                /* MPLL_D3_D2, 104 MHz */
                // USE LOCK dvfs to ensure CLK source.
                // clock_mux_sel(CLK_AUD_GPSRC_SEL, 2);
            } else {
                if (dsp_controller.clk_mux_hires_dl == DL_HIRES_192KHZ) {
                    log_hal_msgid_error("[Audio Clock] ERROR: DL Hi-Res 192KHz is already on, type %d sample rate %d device 0x%x", 3,
                                        type,
                                        device_out_rate,
                                        out_device);
                    assert(0);
                }
                dsp_controller.clk_mux_hires_dl = DL_HIRES_96KHZ;
                dsp_controller.clk_mux_hires_hwsrc = HWSRC_HIRES_96KHZ;
                /* MPLL_D3_D04, 52 MHz */
                // USE LOCK dvfs to ensure CLK source.
                // clock_mux_sel(CLK_AUD_GPSRC_SEL, 1);
            }
            if (!hal_audio_status_get_clock_gate_status(AUDIO_CLOCK_DWLINK)) { // Enable Hi-Res
                if (dsp_controller.clk_mux_hires_dl == DL_HIRES_192KHZ) {
#if defined(HAL_DVFS_MODULE_ENABLED)
                    hal_dvfs_lock_control(HAL_DVFS_OPP_MID, HAL_DVFS_LOCK);
                    log_hal_msgid_info("[Audio Clock] dl hi-res: type %d lock frequency to 0.8V, rate %u, device 0x%x", 3,
                                       type,
                                       device_out_rate,
                                       out_device);
#endif
                } else if (dsp_controller.clk_mux_hires_dl == DL_HIRES_96KHZ) {
#if defined(HAL_DVFS_MODULE_ENABLED)
                    hal_dvfs_lock_control(HAL_DVFS_OPP_LOW, HAL_DVFS_LOCK);
                    log_hal_msgid_info("[Audio Clock] dl hi-res: type %d lock frequency to 0.7V, rate %u, device 0x%x", 3,
                                       type,
                                       device_out_rate,
                                       out_device);
#endif
                } else {
                    log_hal_msgid_error("[Audio Clock] ERROR: abnormal case!", 0);
                    assert(0);
                }
                dsp_controller.hi_res_on_hwsrc = control;
                log_hal_msgid_info("[Audio Clock] dl hi-res: enable hi-res hwsrc!", 0);
                /* MPLL_D5, 124.8 MHz */
                // USE LOCK dvfs to ensure CLK source.
                // clock_mux_sel(CLK_AUD_DLCK_SEL, 1);
                hal_clock_enable(HAL_CLOCK_CG_AUD_DWLINK);
                dsp_controller.hi_res_on_dl = control; /* NOT USE */
            }
            hal_audio_status_set_clock_gate(type, AUDIO_CLOCK_DWLINK, control);
            hal_audio_status_log_clock_gate_setting(type, AUDIO_CLOCK_DWLINK, control);
            if (hal_audio_status_get_clock_gate_status(AUDIO_POWER_DAC)) { // DAC is already on
                /* We should close DL Hi-res after closing DAC when DAC is alredy on! */
                if (!hal_audio_status_check_clock_gate_status(AUDIO_SCENARIO_TYPE_AMP, AUDIO_CLOCK_DWLINK)) {
                    hal_audio_status_set_clock_gate(AUDIO_SCENARIO_TYPE_AMP, AUDIO_CLOCK_DWLINK, control);
                    hal_audio_status_log_clock_gate_setting(AUDIO_SCENARIO_TYPE_AMP, AUDIO_CLOCK_DWLINK, control);
                    log_hal_msgid_info("[Audio Clock] dl hi-res: dac is already on, lock hi-res for amp!", 0);
                }
            }
        }
    } else {
        uint32_t count = hal_audio_status_get_clock_gate_setting(type);
        if (count & (1UL << AUDIO_CLOCK_DWLINK)) {
            hires_flag = true;
            hal_audio_status_log_clock_gate_setting(type, AUDIO_CLOCK_DWLINK, control);
            hal_audio_status_set_clock_gate(type, AUDIO_CLOCK_DWLINK, control);
            if (!hal_audio_status_get_clock_gate_status(AUDIO_CLOCK_DWLINK)) { // Disable Hi-Res

                // Check DL Hi-res mode, AFE_ADDA_DL_SRC2_CON0[31:28] <= 8
                hal_audio_status_check_rg_value(AFE_ADDA_DL_SRC2_CON0, 0x80000000, 0xF0000000, CHECK_MODE_LESS_EQUAL);

                if (dsp_controller.clk_mux_hires_dl == DL_HIRES_192KHZ) {
#if defined(HAL_DVFS_MODULE_ENABLED)
                    hal_dvfs_lock_control(HAL_DVFS_OPP_MID, HAL_DVFS_UNLOCK);
                    log_hal_msgid_info("[Audio Clock] dl hi-res: type %d unlock frequency 0.8V", 1,
                                       type);
#endif
                } else if (dsp_controller.clk_mux_hires_dl == DL_HIRES_96KHZ) {
#if defined(HAL_DVFS_MODULE_ENABLED)
                    hal_dvfs_lock_control(HAL_DVFS_OPP_LOW, HAL_DVFS_UNLOCK);
                    log_hal_msgid_info("[Audio Clock] dl hi-res: type %d unlock frequency 0.7V", 1,
                                       type);
#endif
                } else {
                    log_hal_msgid_error("[Audio Clock] ERROR: abnormal case!", 0);
                    assert(0);
                }
                hal_clock_disable(HAL_CLOCK_CG_AUD_DWLINK);
                dsp_controller.clk_mux_hires_dl = DL_HIRES_NONE;
                dsp_controller.hi_res_on_dl = control; /* NOT USE */
                if (dsp_controller.hi_res_on_hwsrc) {
                    /* F_FXO_CK, 26 MHz */
                    // clock_mux_sel(CLK_AUD_GPSRC_SEL, 0);
                    dsp_controller.hi_res_on_hwsrc = control;
                    log_hal_msgid_info("[Audio Clock] dl hi-res: disable hi-res hwsrc!", 0);
                }
            }
        }
    }
    if (hires_flag) {
        log_hal_msgid_info("[Audio Clock] dl hi-res: type %d control %d", 2,
                           type,
                           control);
    }
}

/**
 * @brief     This function is used to control the releated clock settings or power of APLL in Uplink by scenario.
 * @param[in] type is to indicate the audio scenario type.
 * @param[in] param is the pointer of open parameters.
 * @param[in] control enable or disable.
 * @return    NONE.
 */
void hal_audio_status_set_i2s_master_in_low_jitter_mode(audio_scenario_type_t type, mcu2dsp_open_param_t *param, bool control)
{
    if ((param == NULL) && (control)) {
        return;
    }
    if (control) {
        // check stream_in type, avoid downlink run this code!
        if (param->param.stream_in != STREAM_IN_AFE) {
            return;
        }
        bool intf0_used_flag = false;
        bool intf1_used_flag = false;
        uint32_t device_in_rate = param->stream_in_param.afe.sampling_rate;
        uint32_t i2s_in_rate[4] = {0};
        i2s_in_rate[0] = param->stream_in_param.afe.i2s_master_sampling_rate[0];
        i2s_in_rate[1] = param->stream_in_param.afe.i2s_master_sampling_rate[1];
        i2s_in_rate[2] = param->stream_in_param.afe.i2s_master_sampling_rate[2];
        i2s_in_rate[3] = param->stream_in_param.afe.i2s_master_sampling_rate[3];
        hal_audio_device_t in_device = param->stream_in_param.afe.audio_device;
        hal_audio_interface_t device_interface = param->stream_in_param.afe.audio_interface;
#ifdef AIR_AUDIO_SUPPORT_MULTIPLE_MICROPHONE
        hal_audio_device_t in_device1 = param->stream_in_param.afe.audio_device1;
        hal_audio_device_t in_device2 = param->stream_in_param.afe.audio_device2;
        hal_audio_device_t in_device3 = param->stream_in_param.afe.audio_device3;
        hal_audio_interface_t device_interface1 = param->stream_in_param.afe.audio_interface1;
        hal_audio_interface_t device_interface2 = param->stream_in_param.afe.audio_interface2;
        hal_audio_interface_t device_interface3 = param->stream_in_param.afe.audio_interface3;
#ifdef MTK_AUDIO_HW_IO_CONFIG_ENHANCE
        hal_audio_device_t in_device4 = param->stream_in_param.afe.audio_device4;
        hal_audio_device_t in_device5 = param->stream_in_param.afe.audio_device5;
        hal_audio_device_t in_device6 = param->stream_in_param.afe.audio_device6;
        hal_audio_device_t in_device7 = param->stream_in_param.afe.audio_device7;
        hal_audio_interface_t device_interface4 = param->stream_in_param.afe.audio_interface4;
        hal_audio_interface_t device_interface5 = param->stream_in_param.afe.audio_interface5;
        hal_audio_interface_t device_interface6 = param->stream_in_param.afe.audio_interface6;
        hal_audio_interface_t device_interface7 = param->stream_in_param.afe.audio_interface7;
#endif
#endif
        /* -1-  Get I2S Master Interface ---------------------------------------------------------------------------- */
        int32_t interface_number = -1; // -1 is used for error check
        hal_audio_interface_t interface[8] = {0};
#ifdef AIR_AUDIO_SUPPORT_MULTIPLE_MICROPHONE
#ifdef MTK_AUDIO_HW_IO_CONFIG_ENHANCE
        if ((device_interface7 != HAL_AUDIO_INTERFACE_NONE) &&
            ((in_device7 == HAL_AUDIO_DEVICE_I2S_MASTER)    ||
             (in_device7 == HAL_AUDIO_DEVICE_I2S_MASTER_L)   ||
             (in_device7 == HAL_AUDIO_DEVICE_I2S_MASTER_R))) {
            interface_number += 1;
            interface[interface_number] = device_interface7;
        }
        if ((device_interface6 != HAL_AUDIO_INTERFACE_NONE) &&
            ((in_device6 == HAL_AUDIO_DEVICE_I2S_MASTER)    ||
             (in_device6 == HAL_AUDIO_DEVICE_I2S_MASTER_L)   ||
             (in_device6 == HAL_AUDIO_DEVICE_I2S_MASTER_R))) {
            interface_number += 1;
            interface[interface_number] = device_interface6;
        }
        if ((device_interface5 != HAL_AUDIO_INTERFACE_NONE) &&
            ((in_device5 == HAL_AUDIO_DEVICE_I2S_MASTER)    ||
             (in_device5 == HAL_AUDIO_DEVICE_I2S_MASTER_L)   ||
             (in_device5 == HAL_AUDIO_DEVICE_I2S_MASTER_R))) {
            interface_number += 1;
            interface[interface_number] = device_interface5;
        }
        if ((device_interface4 != HAL_AUDIO_INTERFACE_NONE) &&
            ((in_device4 == HAL_AUDIO_DEVICE_I2S_MASTER)    ||
             (in_device4 == HAL_AUDIO_DEVICE_I2S_MASTER_L)   ||
             (in_device4 == HAL_AUDIO_DEVICE_I2S_MASTER_R))) {
            interface_number += 1;
            interface[interface_number] = device_interface4;
        }
#endif
        if ((device_interface3 != HAL_AUDIO_INTERFACE_NONE) &&
            ((in_device3 == HAL_AUDIO_DEVICE_I2S_MASTER)    ||
             (in_device3 == HAL_AUDIO_DEVICE_I2S_MASTER_L)   ||
             (in_device3 == HAL_AUDIO_DEVICE_I2S_MASTER_R))) {
            interface_number += 1;
            interface[interface_number] = device_interface3;
        }
        if ((device_interface2 != HAL_AUDIO_INTERFACE_NONE) &&
            ((in_device2 == HAL_AUDIO_DEVICE_I2S_MASTER)    ||
             (in_device2 == HAL_AUDIO_DEVICE_I2S_MASTER_L)   ||
             (in_device2 == HAL_AUDIO_DEVICE_I2S_MASTER_R))) {
            interface_number += 1;
            interface[interface_number] = device_interface2;
        }
        if ((device_interface1 != HAL_AUDIO_INTERFACE_NONE) &&
            ((in_device1 == HAL_AUDIO_DEVICE_I2S_MASTER)    ||
             (in_device1 == HAL_AUDIO_DEVICE_I2S_MASTER_L)   ||
             (in_device1 == HAL_AUDIO_DEVICE_I2S_MASTER_R))) {
            interface_number += 1;
            interface[interface_number] = device_interface1;
        }
#endif
        if ((device_interface != HAL_AUDIO_INTERFACE_NONE) &&
            ((in_device == HAL_AUDIO_DEVICE_I2S_MASTER)    ||
             (in_device == HAL_AUDIO_DEVICE_I2S_MASTER_L)   ||
             (in_device == HAL_AUDIO_DEVICE_I2S_MASTER_R))) {
            interface_number += 1;
            interface[interface_number] = device_interface;
        }
        if (interface_number == -1) {
            log_hal_msgid_info("[Audio Clock] low jitter mode: i2s in device is not suitable, type %d device 0x%x", 2,
                               type,
                               in_device);
            return;
        }
        for (uint32_t cnt = 0; cnt <= interface_number; cnt ++) {
            uint8_t i2s_number = 0;
            switch (interface[cnt]) {
                case HAL_AUDIO_INTERFACE_1: // I2S MASTER 0
                    i2s_number = 0;
                    break;
                case HAL_AUDIO_INTERFACE_2: // I2S MASTER 1
                    i2s_number = 1;
                    break;
                case HAL_AUDIO_INTERFACE_3: // I2S MASTER 2
                    i2s_number = 2;
                    break;
                case HAL_AUDIO_INTERFACE_4: // I2S MASTER 3
                    i2s_number = 3;
                    break;
                default:
                    log_hal_msgid_error("[Audio Clock] ERROR: low jitter mode, i2s in interface error. type %d device 0x%x interface %d", 3,
                                        type,
                                        in_device,
                                        device_interface);
                    assert(0);
                    return;
            }
            uint32_t valid_rate = i2s_in_rate[i2s_number] == 0 ? device_in_rate : i2s_in_rate[i2s_number];
            if ((dsp_controller.i2s_master_low_jitter[i2s_number] != 0) || (valid_rate > 48000)) {
                param->stream_in_param.afe.is_low_jitter[i2s_number] = true;
            }
            if (!param->stream_in_param.afe.is_low_jitter[i2s_number]) {
                continue;
            }
            log_hal_msgid_info("[Audio Clock] low jitter mode: i2s in type %d device 0x%x interface %d rate %d", 4,
                               type,
                               in_device,
                               device_interface,
                               valid_rate);
            dsp_controller.i2s_master_low_jitter[i2s_number] |= type; // log the i2s setting of each scenario
            /* Choose APLL 1/2 */
            audio_clock_setting_type_t cg_type = (afe_get_apll_by_samplerate(valid_rate) == AFE_APLL1) ?
                                                 AUDIO_CLOCK_INTF1_IN :
                                                 AUDIO_CLOCK_INTF0_IN;
            if (cg_type == AUDIO_CLOCK_INTF1_IN) {
                intf1_used_flag = true;
            } else {
                intf0_used_flag = true;
            }
            if ((!hal_audio_status_get_clock_gate_status(cg_type)) &&
                (!hal_audio_status_get_clock_gate_status(cg_type + 2))) {
                if (cg_type == AUDIO_CLOCK_INTF1_IN) {
                    clock_mux_sel(CLK_AUD_INTERFACE1_SEL, 2);
                    hal_clock_enable(HAL_CLOCK_CG_AUD_INTF1);
                } else {
                    clock_mux_sel(CLK_AUD_INTERFACE0_SEL, 3); // 3:APLL2_CK, 24.576 / 22.579 MHz
                    hal_clock_enable(HAL_CLOCK_CG_AUD_INTF0);
                }
            }
        }
        if (intf0_used_flag) {
            hal_audio_status_set_clock_gate(type, AUDIO_CLOCK_INTF0_IN, control);
            hal_audio_status_log_clock_gate_setting(type, AUDIO_CLOCK_INTF0_IN, control);
        }
        if (intf1_used_flag) {
            hal_audio_status_set_clock_gate(type, AUDIO_CLOCK_INTF1_IN, control);
            hal_audio_status_log_clock_gate_setting(type, AUDIO_CLOCK_INTF1_IN, control);
        }
    } else {
        uint32_t count = hal_audio_status_get_clock_gate_setting(type);
        for (uint32_t i = 0; i < (AUDIO_CLOCK_INTF1_IN - AUDIO_CLOCK_INTF0_IN + 1); i ++) {
            if (count & (1UL << (AUDIO_CLOCK_INTF0_IN + i))) {
                hal_audio_status_log_clock_gate_setting(type, AUDIO_CLOCK_INTF0_IN + i, control);
                hal_audio_status_set_clock_gate(type, AUDIO_CLOCK_INTF0_IN + i, control);
                // clear dsp_controller flag
                for (uint32_t j = 0; j < AUDIO_HW_I2S_MASTER_BLOCK_NUMBER; j ++) {
                    dsp_controller.i2s_master_low_jitter[j] &= ~((uint64_t)type);
                }

                if ((!hal_audio_status_get_clock_gate_status(AUDIO_CLOCK_INTF0_IN + i)) &&
                    (!hal_audio_status_get_clock_gate_status(AUDIO_CLOCK_INTF0_OUT + i))) {
                    if (i == 0) {
                        // Check APLL2 mode, AUDIO_TOP_CON0[9] = 1 && AFE_APLL2_TUNER_CFG[0] = 0
                        hal_audio_status_check_rg_value(AUDIO_TOP_CON0, 0x200, 0x200, CHECK_MODE_EQUAL);
                        hal_audio_status_check_rg_value(AFE_APLL2_TUNER_CFG, 0, 0, CHECK_MODE_EQUAL);
                        hal_clock_disable(HAL_CLOCK_CG_AUD_INTF0);
                    } else if (i == 1) {
                        // Check APLL1 mode, AUDIO_TOP_CON0[8] = 1 && AFE_APLL1_TUNER_CFG[0] = 0
                        hal_audio_status_check_rg_value(AUDIO_TOP_CON0, 0x100, 0x100, CHECK_MODE_EQUAL);
                        hal_audio_status_check_rg_value(AFE_APLL1_TUNER_CFG, 0, 0, CHECK_MODE_EQUAL);
                        hal_clock_disable(HAL_CLOCK_CG_AUD_INTF1);
                    } else {
                        log_hal_msgid_error("[Audio Clock] low jitter mode: i2s in enum error %d", 1,
                                            i);
                        assert(0);
                    }
                }
            }
        }

    }

}

/**
 * @brief     This function is used to control the releated clock settings or power of APLL in Downlink by scenario.
 * @param[in] type is to indicate the audio scenario type.
 * @param[in] param is the pointer of open parameters.
 * @param[in] control enable or disable.
 * @return    NONE.
 */
void hal_audio_status_set_i2s_master_out_low_jitter_mode(audio_scenario_type_t type, mcu2dsp_open_param_t *param, bool control)
{
    if ((param == NULL) && (control)) {
        return;
    }
    if (control) {
        // check stream_out type, avoid uplink run this code!
        if (param->param.stream_out != STREAM_OUT_AFE) {
            return;
        }
        bool intf0_used_flag = false;
        bool intf1_used_flag = false;
        uint32_t device_out_rate = param->stream_out_param.afe.sampling_rate;
        uint32_t i2s_out_rate[4] = {0};
        i2s_out_rate[0] = param->stream_out_param.afe.i2s_master_sampling_rate[0];
        i2s_out_rate[1] = param->stream_out_param.afe.i2s_master_sampling_rate[1];
        i2s_out_rate[2] = param->stream_out_param.afe.i2s_master_sampling_rate[2];
        i2s_out_rate[3] = param->stream_out_param.afe.i2s_master_sampling_rate[3];
        hal_audio_device_t out_device = param->stream_out_param.afe.audio_device;
        hal_audio_interface_t device_interface = param->stream_out_param.afe.audio_interface;
        if (type == AUDIO_SCENARIO_TYPE_SPDIF) {
            // SPDIF USE I2S0 Master out
            out_device = HAL_AUDIO_DEVICE_I2S_MASTER;
            device_interface = HAL_AUDIO_INTERFACE_1;
        }
#ifdef AIR_AUDIO_SUPPORT_MULTIPLE_MICROPHONE
        hal_audio_device_t out_device1 = param->stream_out_param.afe.audio_device1;
        hal_audio_device_t out_device2 = param->stream_out_param.afe.audio_device2;
        hal_audio_device_t out_device3 = param->stream_out_param.afe.audio_device3;
        hal_audio_interface_t device_interface1 = param->stream_out_param.afe.audio_interface1;
        hal_audio_interface_t device_interface2 = param->stream_out_param.afe.audio_interface2;
        hal_audio_interface_t device_interface3 = param->stream_out_param.afe.audio_interface3;
#ifdef MTK_AUDIO_HW_IO_CONFIG_ENHANCE
        hal_audio_device_t out_device4 = param->stream_out_param.afe.audio_device4;
        hal_audio_device_t out_device5 = param->stream_out_param.afe.audio_device5;
        hal_audio_device_t out_device6 = param->stream_out_param.afe.audio_device6;
        hal_audio_device_t out_device7 = param->stream_out_param.afe.audio_device7;
        hal_audio_interface_t device_interface4 = param->stream_out_param.afe.audio_interface4;
        hal_audio_interface_t device_interface5 = param->stream_out_param.afe.audio_interface5;
        hal_audio_interface_t device_interface6 = param->stream_out_param.afe.audio_interface6;
        hal_audio_interface_t device_interface7 = param->stream_out_param.afe.audio_interface7;
#endif
#endif
        /* -1-  Get I2S Master Interface -------------------------------------------------------------------------------- */
        int32_t interface_number = -1; // -1 is used for error check
        hal_audio_interface_t interface[8] = {0};
#ifdef AIR_AUDIO_SUPPORT_MULTIPLE_MICROPHONE
#ifdef MTK_AUDIO_HW_IO_CONFIG_ENHANCE
        if ((device_interface7 != HAL_AUDIO_INTERFACE_NONE) &&
            ((out_device7 == HAL_AUDIO_DEVICE_I2S_MASTER)   ||
             (out_device7 == HAL_AUDIO_DEVICE_I2S_MASTER_L)  ||
             (out_device7 == HAL_AUDIO_DEVICE_I2S_MASTER_R))) {
            interface_number += 1;
            interface[interface_number] = device_interface7;
        }
        if ((device_interface6 != HAL_AUDIO_INTERFACE_NONE) &&
            ((out_device6 == HAL_AUDIO_DEVICE_I2S_MASTER)   ||
             (out_device6 == HAL_AUDIO_DEVICE_I2S_MASTER_L)  ||
             (out_device6 == HAL_AUDIO_DEVICE_I2S_MASTER_R))) {
            interface_number += 1;
            interface[interface_number] = device_interface6;
        }
        if ((device_interface5 != HAL_AUDIO_INTERFACE_NONE) &&
            ((out_device5 == HAL_AUDIO_DEVICE_I2S_MASTER)   ||
             (out_device5 == HAL_AUDIO_DEVICE_I2S_MASTER_L)  ||
             (out_device5 == HAL_AUDIO_DEVICE_I2S_MASTER_R))) {
            interface_number += 1;
            interface[interface_number] = device_interface5;
        }
        if ((device_interface4 != HAL_AUDIO_INTERFACE_NONE) &&
            ((out_device4 == HAL_AUDIO_DEVICE_I2S_MASTER)   ||
             (out_device4 == HAL_AUDIO_DEVICE_I2S_MASTER_L)  ||
             (out_device4 == HAL_AUDIO_DEVICE_I2S_MASTER_R))) {
            interface_number += 1;
            interface[interface_number] = device_interface4;
        }
#endif
        if ((device_interface3 != HAL_AUDIO_INTERFACE_NONE) &&
            ((out_device3 == HAL_AUDIO_DEVICE_I2S_MASTER)   ||
             (out_device3 == HAL_AUDIO_DEVICE_I2S_MASTER_L)  ||
             (out_device3 == HAL_AUDIO_DEVICE_I2S_MASTER_R))) {
            interface_number += 1;
            interface[interface_number] = device_interface3;
        }
        if ((device_interface2 != HAL_AUDIO_INTERFACE_NONE) &&
            ((out_device2 == HAL_AUDIO_DEVICE_I2S_MASTER)   ||
             (out_device2 == HAL_AUDIO_DEVICE_I2S_MASTER_L)  ||
             (out_device2 == HAL_AUDIO_DEVICE_I2S_MASTER_R))) {
            interface_number += 1;
            interface[interface_number] = device_interface2;
        }
        if ((device_interface1 != HAL_AUDIO_INTERFACE_NONE) &&
            ((out_device1 == HAL_AUDIO_DEVICE_I2S_MASTER)   ||
             (out_device1 == HAL_AUDIO_DEVICE_I2S_MASTER_L)  ||
             (out_device1 == HAL_AUDIO_DEVICE_I2S_MASTER_R))) {
            interface_number += 1;
            interface[interface_number] = device_interface1;
        }
#endif
        if ((device_interface != HAL_AUDIO_INTERFACE_NONE) &&
            ((out_device == HAL_AUDIO_DEVICE_I2S_MASTER)   ||
             (out_device == HAL_AUDIO_DEVICE_I2S_MASTER_L)  ||
             (out_device == HAL_AUDIO_DEVICE_I2S_MASTER_R))) {
            interface_number += 1;
            interface[interface_number] = device_interface;
        }
        if (interface_number == -1) {
            log_hal_msgid_info("[Audio Clock] low jitter mode: i2s_out device is not suitable, type %d device 0x%x", 2,
                               type,
                               out_device);
            return;
        }
        for (uint32_t cnt = 0; cnt <= interface_number; cnt ++) {
            uint8_t i2s_number = 0;
            switch (interface[cnt]) {
                case HAL_AUDIO_INTERFACE_1: // I2S MASTER 0
                    i2s_number = 0;
                    break;
                case HAL_AUDIO_INTERFACE_2: // I2S MASTER 1
                    i2s_number = 1;
                    break;
                case HAL_AUDIO_INTERFACE_3: // I2S MASTER 2
                    i2s_number = 2;
                    break;
                case HAL_AUDIO_INTERFACE_4: // I2S MASTER 3
                    i2s_number = 3;
                    break;
                default:
                    log_hal_msgid_error("[Audio Clock] ERROR: low jitter mode, i2s_out interface error. type %d device 0x%x interface %d", 3,
                                        type,
                                        out_device,
                                        device_interface);
                    assert(0);
                    return;
            }
            uint32_t valid_rate = i2s_out_rate[i2s_number] == 0 ? device_out_rate : i2s_out_rate[i2s_number];
            if ((dsp_controller.i2s_master_low_jitter[i2s_number] != 0) || (valid_rate > 48000)) {
                param->stream_out_param.afe.is_low_jitter[i2s_number] = true;
            }
            if (!param->stream_out_param.afe.is_low_jitter[i2s_number]) {
                continue;
            }
            log_hal_msgid_info("[Audio Clock] low jitter mode: i2s_out type %d device 0x%x interface %d rate %d", 4,
                               type,
                               out_device,
                               device_interface,
                               valid_rate);
            dsp_controller.i2s_master_low_jitter[i2s_number] |= type; // log the i2s setting of each scenario
            /* Choose APLL 1/2 */
            audio_clock_setting_type_t cg_type = (afe_get_apll_by_samplerate(valid_rate)) == AFE_APLL1 ? AUDIO_CLOCK_INTF1_OUT : AUDIO_CLOCK_INTF0_OUT;
            if (cg_type == AUDIO_CLOCK_INTF1_OUT) {
                intf1_used_flag = true;
            } else {
                intf0_used_flag = true;
            }
            if ((!hal_audio_status_get_clock_gate_status(cg_type)) &&
                (!hal_audio_status_get_clock_gate_status(cg_type - 2))) {
                if (cg_type == AUDIO_CLOCK_INTF1_OUT) {
                    clock_mux_sel(CLK_AUD_INTERFACE1_SEL, 2);
                    hal_clock_enable(HAL_CLOCK_CG_AUD_INTF1);
                } else {
                    clock_mux_sel(CLK_AUD_INTERFACE0_SEL, 3); // 3 : APLL2_CK,    24.576 / 22.579 MHz
                    hal_clock_enable(HAL_CLOCK_CG_AUD_INTF0);
                }
            }
        }
        if (intf0_used_flag) {
            hal_audio_status_set_clock_gate(type, AUDIO_CLOCK_INTF0_OUT, control);
            hal_audio_status_log_clock_gate_setting(type, AUDIO_CLOCK_INTF0_OUT, control);
        }
        if (intf1_used_flag) {
            hal_audio_status_set_clock_gate(type, AUDIO_CLOCK_INTF1_OUT, control);
            hal_audio_status_log_clock_gate_setting(type, AUDIO_CLOCK_INTF1_OUT, control);
        }
    } else {
        uint32_t count = hal_audio_status_get_clock_gate_setting(type);
        for (uint32_t i = 0; i < (AUDIO_CLOCK_INTF1_OUT - AUDIO_CLOCK_INTF0_OUT + 1); i ++) {
            if (count & (1UL << (AUDIO_CLOCK_INTF0_OUT + i))) {
                hal_audio_status_log_clock_gate_setting(type, AUDIO_CLOCK_INTF0_OUT + i, control);
                hal_audio_status_set_clock_gate(type, AUDIO_CLOCK_INTF0_OUT + i, control);
                // clear dsp_controller flag
                for (uint32_t j = 0; j < AUDIO_HW_I2S_MASTER_BLOCK_NUMBER; j ++) {
                    dsp_controller.i2s_master_low_jitter[j] &= ~((uint64_t)type);
                }
                if ((!hal_audio_status_get_clock_gate_status(AUDIO_CLOCK_INTF0_OUT + i)) &&
                    (!hal_audio_status_get_clock_gate_status(AUDIO_CLOCK_INTF0_IN + i))) {
                    if (i == 0) {
                        // Check APLL2 mode, AUDIO_TOP_CON0[9] = 1 && AFE_APLL2_TUNER_CFG[0] = 0
                        hal_audio_status_check_rg_value(AUDIO_TOP_CON0, 0x200, 0x200, CHECK_MODE_EQUAL);
                        hal_audio_status_check_rg_value(AFE_APLL2_TUNER_CFG, 0, 0, CHECK_MODE_EQUAL);
                        hal_clock_disable(HAL_CLOCK_CG_AUD_INTF0);
                    } else if (i == 1) {
                        // Check APLL1 mode, AUDIO_TOP_CON0[8] = 1 && AFE_APLL1_TUNER_CFG[0] = 0
                        hal_audio_status_check_rg_value(AUDIO_TOP_CON0, 0x100, 0x100, CHECK_MODE_EQUAL);
                        hal_audio_status_check_rg_value(AFE_APLL1_TUNER_CFG, 0, 0, CHECK_MODE_EQUAL);
                        hal_clock_disable(HAL_CLOCK_CG_AUD_INTF1);
                    } else {
                        log_hal_msgid_error("[Audio Clock] low jitter mode: i2s out enum error %d", 1,
                                            i);
                        assert(0);
                    }
                }
            }
        }

    }
}

/**
 * @brief     This function is used to control the releated clock settings or power of audio by scenario(VOW/SPDIF .etc).
 * @param[in] type is to indicate the audio scenario type.
 * @param[in] param is the pointer of open parameters.
 * @param[in] control enable or disable.
 * @return    NONE.
 */
void hal_audio_status_set_audio_clock_gate(audio_scenario_type_t type, mcu2dsp_open_param_t *param, bool control)
{
    /* -1-  Check Common Clock Gate --------------------------------------------------------------------------------- */
    if (control) {
        /* NOTE: Special Clock Gate Setting: SPDIF & VOW !!! */
        bool spdif_flag = false;
        if (param) {
            hal_audio_device_t out_device = param->stream_out_param.afe.audio_device;
            if ((param->param.stream_out == STREAM_OUT_AFE) && (out_device == HAL_AUDIO_DEVICE_SPDIF)) {
                spdif_flag = true; // USE SPDIF
            }
        }

        if ((type == AUDIO_SCENARIO_TYPE_SPDIF) || (spdif_flag)) {
            hal_clock_enable(HAL_CLOCK_CG_SPDIF);
            hal_audio_status_set_clock_gate(type, AUDIO_CLOCK_SPDIF, control);
            hal_audio_status_log_clock_gate_setting(type, AUDIO_CLOCK_SPDIF, control);
            log_hal_msgid_info("[Audio Clock] enable spdif", 0);
        }
        if (type == AUDIO_SCENARIO_TYPE_VOW) {
            hal_clock_enable(HAL_CLOCK_CG_AUD_VOW_BUS);
            hal_audio_status_set_clock_gate(type, AUDIO_CLOCK_VOW, control);
            hal_audio_status_log_clock_gate_setting(type, AUDIO_CLOCK_VOW, control);
            log_hal_msgid_info("[Audio Clock] enable vow", 0);
        }
        /* dsp low power control */
#ifdef HAL_AUDIO_POWER_SLIM_ENABLED
        if ((type == AUDIO_SCENARIO_TYPE_ANC)
#ifdef HAL_PURE_LINEIN_PLAYBACK_ENABLE
            || (type == AUDIO_SCENARIO_TYPE_LINE_IN)
#endif /* HAL_PURE_LINEIN_PLAYBACK_ENABLE */
        ) {
            if (!hal_audio_status_get_clock_gate_status(AUDIO_DSP_SPM_STATE4)) {
                log_hal_msgid_info("[Audio Clock] lock SPM state4", 0);
                spm_audio_lowpower_setting(SPM_STATE4, SPM_ENABLE); // DSP can sleep
            }
            hal_audio_status_set_clock_gate(type, AUDIO_DSP_SPM_STATE4, control);
            hal_audio_status_log_clock_gate_setting(type, AUDIO_DSP_SPM_STATE4, control);
        } else {
            if (!hal_audio_status_get_clock_gate_status(AUDIO_DSP_SPM_STATE1)) {
                log_hal_msgid_info("[Audio Clock] lock SPM state1", 0);
                spm_audio_lowpower_setting(SPM_STATE1, SPM_ENABLE); // DSP can't sleep
            }
            /* Lock AMP for SPM state1 to avoid pop noise of ending */
            if ((hal_audio_status_get_clock_gate_status(AUDIO_POWER_DAC)) &&
                (!hal_audio_status_check_clock_gate_status(AUDIO_SCENARIO_TYPE_AMP, AUDIO_DSP_SPM_STATE1))) { // DAC is already on
                hal_audio_status_set_clock_gate(AUDIO_SCENARIO_TYPE_AMP, AUDIO_DSP_SPM_STATE1, control);
                hal_audio_status_log_clock_gate_setting(AUDIO_SCENARIO_TYPE_AMP, AUDIO_DSP_SPM_STATE1, control);
                log_hal_msgid_info("[Audio Clock] lock SPM state1 for AMP", 0);
            }
            hal_audio_status_set_clock_gate(type, AUDIO_DSP_SPM_STATE1, control);
            hal_audio_status_log_clock_gate_setting(type, AUDIO_DSP_SPM_STATE1, control);
        }
#else
        if (!hal_audio_status_get_clock_gate_status(AUDIO_DSP_SPM_STATE1)) {
            log_hal_msgid_info("[Audio Clock] lock SPM state1", 0);
            spm_audio_lowpower_setting(SPM_STATE1, SPM_ENABLE); // DSP can't sleep
        }
        /* Lock AMP for SPM state1 to avoid pop noise of ending */
        if ((hal_audio_status_get_clock_gate_status(AUDIO_POWER_DAC)) &&
            (!hal_audio_status_check_clock_gate_status(AUDIO_SCENARIO_TYPE_AMP, AUDIO_DSP_SPM_STATE1))) { // DAC is already on
            hal_audio_status_set_clock_gate(AUDIO_SCENARIO_TYPE_AMP, AUDIO_DSP_SPM_STATE1, control);
            hal_audio_status_log_clock_gate_setting(AUDIO_SCENARIO_TYPE_AMP, AUDIO_DSP_SPM_STATE1, control);
            log_hal_msgid_info("[Audio Clock] lock SPM state1 for AMP", 0);
        }
        hal_audio_status_set_clock_gate(type, AUDIO_DSP_SPM_STATE1, control);
        hal_audio_status_log_clock_gate_setting(type, AUDIO_DSP_SPM_STATE1, control);
#endif /* HAL_AUDIO_POWER_SLIM_ENABLED */
        if ((!hal_audio_status_get_clock_gate_status(AUDIO_CLOCK_GPSRC)) &&
            (!hal_audio_status_get_clock_gate_status(AUDIO_CLOCK_INT)) &&
            (!hal_audio_status_get_clock_gate_status(AUDIO_CLOCK_ENGINE))) { // Power on
            /* NOTE: We must enable INTBUS/ENGINE CG before mtcmos control!!! */
            hal_clock_enable(HAL_CLOCK_CG_AUD_INTBUS);
            hal_clock_enable(HAL_CLOCK_CG_AUD_GPSRC);
            hal_clock_enable(HAL_CLOCK_CG_AUD_ENGINE_BUS);
            // power on mt mos
#ifdef HAL_SLEEP_MANAGER_ENABLED
            spm_control_mtcmos(SPM_MTCMOS_AUDIO_SYS, SPM_MTCMOS_PWR_ENABLE);
            spm_control_mtcmos(SPM_MTCMOS_AUDIO_ANC, SPM_MTCMOS_PWR_ENABLE);
            spm_control_mtcmos(SPM_MTCMOS_AUDIO_HS,  SPM_MTCMOS_PWR_ENABLE);
#endif /* HAL_SLEEP_MANAGER_ENABLED */
            // // PDN Control
            *((volatile uint32_t*)(0xC0000000)) = 0x3FCCC344; // pdn AUDIO_TOP_CON0
            *((volatile uint32_t*)(0xC0000004)) = 0x709b000f; // PDN AUDIO_TOP_CON1
        }
        hal_audio_status_set_clock_gate(type, AUDIO_CLOCK_GPSRC, control);
        hal_audio_status_set_clock_gate(type, AUDIO_CLOCK_INT, control);
        hal_audio_status_set_clock_gate(type, AUDIO_CLOCK_ENGINE, control);

        hal_audio_status_log_clock_gate_setting(type, AUDIO_CLOCK_GPSRC, control);
        hal_audio_status_log_clock_gate_setting(type, AUDIO_CLOCK_INT, control);
        hal_audio_status_log_clock_gate_setting(type, AUDIO_CLOCK_ENGINE, control);
        if (hal_audio_status_get_clock_gate_status(AUDIO_POWER_DAC)) { // DAC is already on
            /* We should power off audio clock after closing DAC when DAC is alredy on! */
            // AMP is for AMP delay, we can only power off DAC after DSP notifying!
            if ((!hal_audio_status_check_clock_gate_status(AUDIO_SCENARIO_TYPE_AMP, AUDIO_CLOCK_GPSRC)) &&
                (!hal_audio_status_check_clock_gate_status(AUDIO_SCENARIO_TYPE_AMP, AUDIO_CLOCK_INT)) &&
                (!hal_audio_status_check_clock_gate_status(AUDIO_SCENARIO_TYPE_AMP, AUDIO_CLOCK_ENGINE))) {
                hal_audio_status_set_clock_gate(AUDIO_SCENARIO_TYPE_AMP, AUDIO_CLOCK_GPSRC, control);
                hal_audio_status_set_clock_gate(AUDIO_SCENARIO_TYPE_AMP, AUDIO_CLOCK_INT, control);
                hal_audio_status_set_clock_gate(AUDIO_SCENARIO_TYPE_AMP, AUDIO_CLOCK_ENGINE, control);

                hal_audio_status_log_clock_gate_setting(AUDIO_SCENARIO_TYPE_AMP, AUDIO_CLOCK_GPSRC, control);
                hal_audio_status_log_clock_gate_setting(AUDIO_SCENARIO_TYPE_AMP, AUDIO_CLOCK_INT, control);
                hal_audio_status_log_clock_gate_setting(AUDIO_SCENARIO_TYPE_AMP, AUDIO_CLOCK_ENGINE, control);
                log_hal_msgid_info("[Audio Clock] clock gate: dac is already on, lock common gate for amp!", 0);
            }
        }
    } else {
        /* NOTE: Special Clock Gate Setting: SPDIF & VOW !!! */
        uint32_t count = hal_audio_status_get_clock_gate_setting(type);
        if ((type == AUDIO_SCENARIO_TYPE_SPDIF) && (count & (1 << AUDIO_CLOCK_SPDIF))) {
            // Check SPDIF, AFE_SPDIFIN_CFG0[0] = 0
            hal_audio_status_check_rg_value(AFE_SPDIFIN_CFG0, 0, 0x1, CHECK_MODE_EQUAL);

            hal_clock_disable(HAL_CLOCK_CG_SPDIF);
            hal_audio_status_set_clock_gate(type, AUDIO_CLOCK_SPDIF, control);
            hal_audio_status_log_clock_gate_setting(type, AUDIO_CLOCK_SPDIF, control);
        }
        if ((type == AUDIO_SCENARIO_TYPE_VOW) && (count & (1 << AUDIO_CLOCK_VOW))) {
            // Check VOW, AFE_VOW_TOP_CON1[0] = 0 && AFE_VOW_TOP_CON2[0] = 0
            hal_audio_status_check_rg_value(AFE_VOW_TOP_CON1, 0, 0x1, CHECK_MODE_EQUAL);
            hal_audio_status_check_rg_value(AFE_VOW_TOP_CON2, 0, 0x1, CHECK_MODE_EQUAL);

            hal_clock_disable(HAL_CLOCK_CG_AUD_VOW_BUS);
            hal_audio_status_set_clock_gate(type, AUDIO_CLOCK_VOW, control);
            hal_audio_status_log_clock_gate_setting(type, AUDIO_CLOCK_VOW, control);
        }

        hal_audio_status_set_clock_gate(type, AUDIO_CLOCK_GPSRC, control);
        hal_audio_status_set_clock_gate(type, AUDIO_CLOCK_INT, control);
        hal_audio_status_set_clock_gate(type, AUDIO_CLOCK_ENGINE, control);

        hal_audio_status_log_clock_gate_setting(type, AUDIO_CLOCK_GPSRC, control);
        hal_audio_status_log_clock_gate_setting(type, AUDIO_CLOCK_INT, control);
        hal_audio_status_log_clock_gate_setting(type, AUDIO_CLOCK_ENGINE, control);
        if ((!hal_audio_status_get_clock_gate_status(AUDIO_CLOCK_GPSRC)) &&
            (!hal_audio_status_get_clock_gate_status(AUDIO_CLOCK_INT)) &&
            (!hal_audio_status_get_clock_gate_status(AUDIO_CLOCK_ENGINE))) { // Power off
            // Check AFE Power, AUDIO_TOP_CON0[2] = 1
            hal_audio_status_check_rg_value(AUDIO_TOP_CON0, 0x4, 0x4, CHECK_MODE_EQUAL);
#ifdef HAL_SLEEP_MANAGER_ENABLED
            spm_control_mtcmos(SPM_MTCMOS_AUDIO_SYS, SPM_MTCMOS_PWR_DISABLE);
            spm_control_mtcmos(SPM_MTCMOS_AUDIO_ANC, SPM_MTCMOS_PWR_DISABLE);
            spm_control_mtcmos(SPM_MTCMOS_AUDIO_HS,  SPM_MTCMOS_PWR_DISABLE);
#endif /* HAL_SLEEP_MANAGER_ENABLED */
            /* NOTE: We must disable INTBUS/ENGINE CG after mtcmos control!!! */
            hal_clock_disable(HAL_CLOCK_CG_AUD_INTBUS);
            hal_clock_disable(HAL_CLOCK_CG_AUD_GPSRC);
            hal_clock_disable(HAL_CLOCK_CG_AUD_ENGINE_BUS);
        }
        /* dsp low power control */
        if (count & (1 << AUDIO_DSP_SPM_STATE1)) {
            hal_audio_status_log_clock_gate_setting(type, AUDIO_DSP_SPM_STATE1, control);
            hal_audio_status_set_clock_gate(type, AUDIO_DSP_SPM_STATE1, control);
            if (!hal_audio_status_get_clock_gate_status(AUDIO_DSP_SPM_STATE1)) {
                log_hal_msgid_info("[Audio Clock] unlock SPM state1", 0);
                spm_audio_lowpower_setting(SPM_STATE1, SPM_DISABLE);
            }
        }
        if (count & (1 << AUDIO_DSP_SPM_STATE4)) {
            hal_audio_status_log_clock_gate_setting(type, AUDIO_DSP_SPM_STATE4, control);
            hal_audio_status_set_clock_gate(type, AUDIO_DSP_SPM_STATE4, control);
            if (!hal_audio_status_get_clock_gate_status(AUDIO_DSP_SPM_STATE4)) {
                log_hal_msgid_info("[Audio Clock] unlock SPM state4", 0);
                spm_audio_lowpower_setting(SPM_STATE4, SPM_DISABLE);
            }
        }
    }
    /* -2-  Hi-res check  ------------------------------------------------------------------------------------------- */
    hal_audio_status_set_dl_hires(type, param, control);
    hal_audio_status_set_ul_hires(type, param, control);
    /* -3-  Low jitter mode check  ---------------------------------------------------------------------------------- */
    hal_audio_status_set_i2s_master_in_low_jitter_mode(type, param, control);
    hal_audio_status_set_i2s_master_out_low_jitter_mode(type, param, control);
    /* -4-  Show log info  ------------------------------------------------------------------------------------------ */
    log_hal_msgid_info("[Audio Clock] type [%d] cg_setting [0x%x] control [0x%x]", 3,
                       type,
                       hal_audio_status_get_clock_gate_setting(type),
                       control);
    log_hal_msgid_info("[Audio Clock] DAC [0x%x][0x%x][0x%x][0x%x] INT [0x%x][0x%x][0x%x][0x%x] ENGINE [0x%x][0x%x][0x%x][0x%x] GPSRC [0x%x][0x%x][0x%x][0x%x]", 16,
                       g_dsp_controller_cg[AUDIO_POWER_DAC][3], g_dsp_controller_cg[AUDIO_POWER_DAC][2], g_dsp_controller_cg[AUDIO_POWER_DAC][1], g_dsp_controller_cg[AUDIO_POWER_DAC][0],
                       g_dsp_controller_cg[AUDIO_CLOCK_INT][3], g_dsp_controller_cg[AUDIO_CLOCK_INT][2], g_dsp_controller_cg[AUDIO_CLOCK_INT][1], g_dsp_controller_cg[AUDIO_CLOCK_INT][0],
                       g_dsp_controller_cg[AUDIO_CLOCK_ENGINE][3], g_dsp_controller_cg[AUDIO_CLOCK_ENGINE][2], g_dsp_controller_cg[AUDIO_CLOCK_ENGINE][1], g_dsp_controller_cg[AUDIO_CLOCK_ENGINE][0],
                       g_dsp_controller_cg[AUDIO_CLOCK_GPSRC][3], g_dsp_controller_cg[AUDIO_CLOCK_GPSRC][2], g_dsp_controller_cg[AUDIO_CLOCK_GPSRC][1], g_dsp_controller_cg[AUDIO_CLOCK_GPSRC][0]
                      );
    log_hal_msgid_info("[Audio Clock] UL  [0x%x][0x%x][0x%x][0x%x] DL  [0x%x][0x%x][0x%x][0x%x] SPDIF  [0x%x][0x%x][0x%x][0x%x] TEST  [0x%x][0x%x][0x%x][0x%x]", 16,
                       g_dsp_controller_cg[AUDIO_CLOCK_UPLINK][3], g_dsp_controller_cg[AUDIO_CLOCK_UPLINK][2], g_dsp_controller_cg[AUDIO_CLOCK_UPLINK][1], g_dsp_controller_cg[AUDIO_CLOCK_UPLINK][0],
                       g_dsp_controller_cg[AUDIO_CLOCK_DWLINK][3], g_dsp_controller_cg[AUDIO_CLOCK_DWLINK][2], g_dsp_controller_cg[AUDIO_CLOCK_DWLINK][1], g_dsp_controller_cg[AUDIO_CLOCK_DWLINK][0],
                       g_dsp_controller_cg[AUDIO_CLOCK_SPDIF][3], g_dsp_controller_cg[AUDIO_CLOCK_SPDIF][2], g_dsp_controller_cg[AUDIO_CLOCK_SPDIF][1], g_dsp_controller_cg[AUDIO_CLOCK_SPDIF][0],
                       g_dsp_controller_cg[AUDIO_CLOCK_TEST][3], g_dsp_controller_cg[AUDIO_CLOCK_TEST][2], g_dsp_controller_cg[AUDIO_CLOCK_TEST][1], g_dsp_controller_cg[AUDIO_CLOCK_TEST][0]
                      );
    log_hal_msgid_info("[Audio Clock] APLL2 IN [0x%x][0x%x][0x%x][0x%x] APLL1 IN [0x%x][0x%x][0x%x][0x%x] APLL2 OUT [0x%x][0x%x][0x%x][0x%x] APLL1 OUT [0x%x][0x%x][0x%x][0x%x]", 16,
                       g_dsp_controller_cg[AUDIO_CLOCK_INTF0_IN][3], g_dsp_controller_cg[AUDIO_CLOCK_INTF0_IN][2], g_dsp_controller_cg[AUDIO_CLOCK_INTF0_IN][1], g_dsp_controller_cg[AUDIO_CLOCK_INTF0_IN][0],
                       g_dsp_controller_cg[AUDIO_CLOCK_INTF1_IN][3], g_dsp_controller_cg[AUDIO_CLOCK_INTF1_IN][2], g_dsp_controller_cg[AUDIO_CLOCK_INTF1_IN][1], g_dsp_controller_cg[AUDIO_CLOCK_INTF1_IN][0],
                       g_dsp_controller_cg[AUDIO_CLOCK_INTF0_OUT][3], g_dsp_controller_cg[AUDIO_CLOCK_INTF0_OUT][2], g_dsp_controller_cg[AUDIO_CLOCK_INTF0_OUT][1], g_dsp_controller_cg[AUDIO_CLOCK_INTF0_OUT][0],
                       g_dsp_controller_cg[AUDIO_CLOCK_INTF1_OUT][3], g_dsp_controller_cg[AUDIO_CLOCK_INTF1_OUT][2], g_dsp_controller_cg[AUDIO_CLOCK_INTF1_OUT][1], g_dsp_controller_cg[AUDIO_CLOCK_INTF1_OUT][0]
                      );
    log_hal_msgid_info("[Audio Clock] Bias0 [0x%x][0x%x][0x%x][0x%x] Bias1 [0x%x][0x%x][0x%x][0x%x] Bias2 [0x%x][0x%x][0x%x][0x%x] VOW [0x%x][0x%x][0x%x][0x%x]", 16,
                       g_dsp_controller_cg[AUDIO_POWER_MICBIAS_0][3], g_dsp_controller_cg[AUDIO_POWER_MICBIAS_0][2], g_dsp_controller_cg[AUDIO_POWER_MICBIAS_0][1], g_dsp_controller_cg[AUDIO_POWER_MICBIAS_0][0],
                       g_dsp_controller_cg[AUDIO_POWER_MICBIAS_1][3], g_dsp_controller_cg[AUDIO_POWER_MICBIAS_1][2], g_dsp_controller_cg[AUDIO_POWER_MICBIAS_1][1], g_dsp_controller_cg[AUDIO_POWER_MICBIAS_1][0],
                       g_dsp_controller_cg[AUDIO_POWER_MICBIAS_2][3], g_dsp_controller_cg[AUDIO_POWER_MICBIAS_2][2], g_dsp_controller_cg[AUDIO_POWER_MICBIAS_2][1], g_dsp_controller_cg[AUDIO_POWER_MICBIAS_2][0],
                       g_dsp_controller_cg[AUDIO_CLOCK_VOW][3], g_dsp_controller_cg[AUDIO_CLOCK_VOW][2], g_dsp_controller_cg[AUDIO_CLOCK_VOW][1], g_dsp_controller_cg[AUDIO_CLOCK_VOW][0]
                      );
    log_hal_msgid_info("[Audio Clock] SPM State1 [0x%x][0x%x][0x%x][0x%x] State3 [0x%x][0x%x][0x%x][0x%x] State4 [0x%x][0x%x][0x%x][0x%x]", 12,
                       g_dsp_controller_cg[AUDIO_DSP_SPM_STATE1][3], g_dsp_controller_cg[AUDIO_DSP_SPM_STATE1][2], g_dsp_controller_cg[AUDIO_DSP_SPM_STATE1][1], g_dsp_controller_cg[AUDIO_DSP_SPM_STATE1][0],
                       g_dsp_controller_cg[AUDIO_DSP_SPM_STATE3][3], g_dsp_controller_cg[AUDIO_DSP_SPM_STATE3][2], g_dsp_controller_cg[AUDIO_DSP_SPM_STATE3][1], g_dsp_controller_cg[AUDIO_DSP_SPM_STATE3][0],
                       g_dsp_controller_cg[AUDIO_DSP_SPM_STATE4][3], g_dsp_controller_cg[AUDIO_DSP_SPM_STATE4][2], g_dsp_controller_cg[AUDIO_DSP_SPM_STATE4][1], g_dsp_controller_cg[AUDIO_DSP_SPM_STATE4][0]
                      );
}
/**
 * @brief     This function is used to control all the releated clock settings or power of audio by scenario. This is the common api for users.
 * @param[in] type is to indicate the audio scenario type.
 * @param[in] param is the pointer of open parameters.
 * @param[in] is_running enable or disable.
 * @return    NONE.
 */
void hal_audio_status_set_running_flag(audio_scenario_type_t type, mcu2dsp_open_param_t *param, bool is_running)
{
#ifndef FPGA_ENV
    /* NOTE: This API should be called by each audio user! */
    log_hal_msgid_info("[Audio Clock] scenario type = [%d] param = 0x%x enable = [%d] ", 3,
                       type,
                       param,
                       is_running);
    /* Special handle for AMP, to avoid timeing issue of AMP */
    /* We must ignore the AMP signal when DAC is still lock by some scenario excluding AMP */
    if ((!is_running) &&
        (type == AUDIO_SCENARIO_TYPE_AMP) &&
        (hal_audio_status_query_running_flag_except(AUDIO_SCENARIO_TYPE_AMP)) &&
        (g_dsp_controller_cg[AUDIO_POWER_DAC][3] ||
         g_dsp_controller_cg[AUDIO_POWER_DAC][2] ||
         g_dsp_controller_cg[AUDIO_POWER_DAC][1] ||
         (g_dsp_controller_cg[AUDIO_POWER_DAC][0] & 0xFFFFFFFD))) {
        log_hal_msgid_warning("[Audio Clock] Warning: other scenario is still on, ignore amp!", 0);
        return;
    }
    /* -1-  Check power of DAC -------------------------------------------------------------------------------------- */
    hal_audio_status_set_dac(type, param, is_running);
    /* -2-  Check power of micbias ---------------------------------------------------------------------------------- */
    hal_audio_status_set_micbias(type, param, is_running);
    /* -3-  Check power of Audio Clock Gate ------------------------------------------------------------------------- */
    hal_audio_status_set_audio_clock_gate(type, param, is_running);
#endif /* FPGA_ENV */
#if defined AIR_DUAL_CHIP_MIXING_MODE_ROLE_MASTER_ENABLE || defined AIR_DUAL_CHIP_MIXING_MODE_ROLE_SLAVE_ENABLE
    mcu_dchs_cosys_ctrl_cmd_relay(is_running ? AUDIO_UART_COSYS_DL_OPEN : AUDIO_UART_COSYS_DL_CLOSE, type, param, NULL);
#endif
}

void hal_audio_status_set_notify_flag(audio_message_type_t type, bool is_notify)
{
    if (is_notify) {
        dsp_controller.dsp_notify |= (1 << type);
    } else {
        dsp_controller.dsp_notify &= ~(1 << type);
    }
}

bool hal_audio_is_a2dp_only(audio_message_type_t type)
{
    if (((type == AUDIO_MESSAGE_TYPE_BT_AUDIO_DL) || (type == AUDIO_MESSAGE_TYPE_AFE)) && \
        ((0 == (dsp_controller.running & (~((1 << AUDIO_MESSAGE_TYPE_BT_AUDIO_DL) | (1 << AUDIO_MESSAGE_TYPE_AFE))))))) {
        return true;
    }
    return false;
}

bool hal_audio_status_query_running_flag(audio_scenario_type_t type)
{
    if (g_dsp_controller_cg_control_bit[type] != 0) {
        return true;
    } else {
        return false;
    }
}

bool hal_audio_status_query_running_flag_except(audio_scenario_type_t type)
{
    for (uint32_t i = 0; i < AUDIO_SCENARIO_TYPE_END; i++) {
        if (i == type) {
            continue;
        }
        if (g_dsp_controller_cg_control_bit[i] != 0) {
            return true;
        }
    }
    return false;
}

bool hal_audio_status_query_notify_flag(audio_message_type_t type)
{
    if (dsp_controller.dsp_notify & (1 << type)) {
        return true;
    } else {
        return false;
    }
}

uint16_t hal_audio_status_query_running_flag_value()
{
    for (uint32_t i = 0; i < AUDIO_SCENARIO_TYPE_END; i++) {
        if (g_dsp_controller_cg_control_bit[i] != 0) {
            return 1;
        }
    }
    return false;
}

//== Data path related API (codec will use) ==
hal_audio_status_t hal_audio_write_stream_out_by_type(audio_message_type_t type, const void *buffer, uint32_t size)
{
    //ToDo: limit the scope -- treat it as local playback
    n9_dsp_share_info_t *p_info;
    uint32_t free_byte_count;
    hal_audio_status_t result = HAL_AUDIO_STATUS_OK;
    uint32_t i;
    uint8_t *p_source_buf = (uint8_t *)buffer;
    bool is_notify;
    uint16_t message_ack;

    // Check buffer
    if (buffer == NULL) {
        return HAL_AUDIO_STATUS_ERROR;
    }

    // According to type to get share info
    switch (type) {
        case AUDIO_MESSAGE_TYPE_PLAYBACK:
            p_info = hal_audio_query_playback_share_info();
            message_ack = AUDIO_CCNI_MESSAGE_ACK(MSG_DSP2MCU_PLAYBACK_DATA_REQUEST);
            break;
        case AUDIO_MESSAGE_TYPE_PROMPT:
            p_info = hal_audio_query_prompt_share_info();
            message_ack = AUDIO_CCNI_MESSAGE_ACK(MSG_DSP2MCU_PROMPT_DATA_REQUEST);
            break;
        default:
            return HAL_AUDIO_STATUS_ERROR;
    }

    // Check data amount
    free_byte_count = hal_audio_buf_mgm_get_free_byte_count(p_info);
    if (size > free_byte_count) {
        if (type != AUDIO_MESSAGE_TYPE_PLAYBACK) {
            return HAL_AUDIO_STATUS_ERROR;
        } else {
            size = free_byte_count;
        }
    }

    // When free space is enough
    for (i = 0; (i < 2) && size; i++) {
        uint8_t *p_dest_buf;
        uint32_t buf_size, segment;

        hal_audio_buf_mgm_get_free_buffer(p_info, &p_dest_buf, &buf_size);
        if (size >= buf_size) {
            segment = buf_size;
        } else {
            segment = size;
        }
        memcpy(p_dest_buf, p_source_buf, segment);
        hal_audio_buf_mgm_get_write_data_done(p_info, segment);
        p_source_buf += segment;
        size -= segment;
    }
#if 0//vp debug
    log_hal_msgid_info("[VPC]p_info W(0x%x) R(0x%x) IsFull(%d)\r\n", 3, p_info->start_addr + p_info->read_offset, p_info->start_addr + p_info->write_offset, p_info->bBufferIsFull);
#endif
    // Check status and notify DSP
    is_notify = hal_audio_status_query_notify_flag(type);
    if (is_notify) {
        hal_audio_status_set_notify_flag(type, false);
        hal_audio_dsp_controller_send_message(message_ack, 0, 0, false);
    }

    return result;
}


//== AM task related API ==
//ToDo
void hal_audio_AM_register_callback()
{


}


void hal_audio_am_register_a2dp_open_callback(hal_bt_audio_dl_open_callback_t callback)
{
    audio_isr.bt_audio_dl_open_callback = callback;
}


//== Speech related parameter
//ToDo: currently, I can't confirm the structure and size of parameter
void speech_update_common(const uint16_t *common)
{

}

void speech_update_nb_param(const uint16_t *param)
{

}

void speech_update_wb_param(const uint16_t *param)
{

}

void speech_update_nb_fir(const int16_t *in_coeff, const int16_t *out_coeff)
{

}

void speech_update_wb_fir(const int16_t *in_coeff, const int16_t *out_coeff)
{

}

int32_t audio_update_iir_design(const uint32_t *parameter)
{
    return 0;
}

//== Ring buffer opeartion ==
/*@brief     circular buffer(ring buffer) implemented by mirroring, which keep an extra bit to distinguish empty and full situation. */
uint32_t ring_buffer_get_data_byte_count(ring_buffer_information_t *p_info)
{
    uint32_t buffer_byte_count = p_info->buffer_byte_count;
    uint32_t write_pointer     = p_info->write_pointer;
    uint32_t read_pointer      = p_info->read_pointer;
    uint32_t data_byte_count;
    if (write_pointer >= read_pointer) {
        data_byte_count = write_pointer - read_pointer;
    } else {
        data_byte_count = (buffer_byte_count << 1) - read_pointer + write_pointer;
    }
    return data_byte_count;
}

uint32_t ring_buffer_get_space_byte_count(ring_buffer_information_t *p_info)
{
    return p_info->buffer_byte_count - ring_buffer_get_data_byte_count(p_info);
}

void ring_buffer_get_write_information(ring_buffer_information_t *p_info, uint8_t **pp_buffer, uint32_t *p_byte_count)
{
    uint32_t buffer_byte_count = p_info->buffer_byte_count;
    uint32_t space_byte_count  = ring_buffer_get_space_byte_count(p_info);
    uint8_t *buffer_pointer    = p_info->buffer_base_pointer;
    uint32_t write_pointer     = p_info->write_pointer;
    uint32_t tail_byte_count;
    if (write_pointer < buffer_byte_count) {
        *pp_buffer = buffer_pointer + write_pointer;
        tail_byte_count = buffer_byte_count - write_pointer;
    } else {
        *pp_buffer = buffer_pointer + write_pointer - buffer_byte_count;
        tail_byte_count = (buffer_byte_count << 1) - write_pointer;
    }
    *p_byte_count = MINIMUM(space_byte_count, tail_byte_count);
    return;
}

void ring_buffer_get_read_information(ring_buffer_information_t *p_info, uint8_t **pp_buffer, uint32_t *p_byte_count)
{
    uint32_t buffer_byte_count = p_info->buffer_byte_count;
    uint32_t data_byte_count   = ring_buffer_get_data_byte_count(p_info);
    uint8_t *buffer_pointer    = p_info->buffer_base_pointer;
    uint32_t read_pointer      = p_info->read_pointer;
    uint32_t tail_byte_count;
    if (read_pointer < buffer_byte_count) {
        *pp_buffer = buffer_pointer + read_pointer;
        tail_byte_count = buffer_byte_count - read_pointer;
    } else {
        *pp_buffer = buffer_pointer + read_pointer - buffer_byte_count;
        tail_byte_count = (buffer_byte_count << 1) - read_pointer;
    }
    *p_byte_count = MINIMUM(data_byte_count, tail_byte_count);
    return;
}

void ring_buffer_write_done(ring_buffer_information_t *p_info, uint32_t write_byte_count)
{
    uint32_t buffer_byte_count = p_info->buffer_byte_count;
    uint32_t buffer_end        = buffer_byte_count << 1;
    uint32_t write_pointer     = p_info->write_pointer + write_byte_count;
    p_info->write_pointer = write_pointer >= buffer_end ? write_pointer - buffer_end : write_pointer;
    return;
}

void ring_buffer_read_done(ring_buffer_information_t *p_info, uint32_t read_byte_count)
{
    uint32_t buffer_byte_count = p_info->buffer_byte_count;
    uint32_t buffer_end        = buffer_byte_count << 1;
    uint32_t read_pointer      = p_info->read_pointer + read_byte_count;
    p_info->read_pointer = read_pointer >= buffer_end ? read_pointer - buffer_end : read_pointer;
    return;
}

#ifdef MTK_BT_A2DP_AAC_ENABLE
/*@brief     circular buffer(ring buffer) implemented by keeping one slot open. Full buffer has at most (size - 1) slots. */
uint32_t ring_buffer_get_data_byte_count_non_mirroring(ring_buffer_information_t *p_info)
{
    uint32_t buffer_byte_count = p_info->buffer_byte_count;
    uint32_t write_pointer     = p_info->write_pointer;
    uint32_t read_pointer      = p_info->read_pointer;
    uint32_t data_byte_count;

    if (write_pointer >= read_pointer) {
        data_byte_count = write_pointer - read_pointer;
    } else {
        data_byte_count = buffer_byte_count - read_pointer + write_pointer;
    }
    return data_byte_count;
}

uint32_t ring_buffer_get_space_byte_count_non_mirroring(ring_buffer_information_t *p_info)
{
    return p_info->buffer_byte_count - ring_buffer_get_data_byte_count_non_mirroring(p_info);
}

void ring_buffer_get_write_information_non_mirroring(ring_buffer_information_t *p_info, uint8_t **pp_buffer, uint32_t *p_byte_count)
{
    uint32_t buffer_byte_count = p_info->buffer_byte_count;     //buffer size
    uint32_t space_byte_count  = ring_buffer_get_space_byte_count_non_mirroring(p_info) - 2;  //space two bytes(one word) empty for DSP operation
    uint8_t *buffer_pointer    = p_info->buffer_base_pointer;
    uint32_t write_pointer     = p_info->write_pointer;
    uint32_t tail_byte_count;

    tail_byte_count = buffer_byte_count - write_pointer;
    *pp_buffer = buffer_pointer + write_pointer;
    *p_byte_count = MINIMUM(space_byte_count, tail_byte_count);
    return;
}

void ring_buffer_get_read_information_non_mirroring(ring_buffer_information_t *p_info, uint8_t **pp_buffer, uint32_t *p_byte_count)
{
    uint32_t buffer_byte_count = p_info->buffer_byte_count;
    uint32_t data_byte_count   = ring_buffer_get_data_byte_count_non_mirroring(p_info);
    uint8_t *buffer_pointer    = p_info->buffer_base_pointer;
    uint32_t read_pointer      = p_info->read_pointer;
    uint32_t tail_byte_count;

    *pp_buffer = buffer_pointer + read_pointer;
    tail_byte_count = buffer_byte_count - read_pointer;
    *p_byte_count = MINIMUM(data_byte_count, tail_byte_count);
    return;
}


void ring_buffer_write_done_non_mirroring(ring_buffer_information_t *p_info, uint32_t write_byte_count)
{
    uint32_t buffer_byte_count = p_info->buffer_byte_count;
    uint32_t write_pointer     = p_info->write_pointer + write_byte_count;
    p_info->write_pointer = write_pointer == buffer_byte_count ? write_pointer - buffer_byte_count : write_pointer;
    return;
}

void ring_buffer_read_done_non_mirroring(ring_buffer_information_t *p_info, uint32_t read_byte_count)
{
    uint32_t buffer_byte_count = p_info->buffer_byte_count;
    uint32_t read_pointer      = p_info->read_pointer + read_byte_count;
    p_info->read_pointer = read_pointer == buffer_byte_count ? read_pointer - buffer_byte_count : read_pointer;
    return;
}
#endif /*MTK_BT_A2DP_AAC_ENABLE*/

//== Audio sync ==
audio_dsp_a2dp_dl_time_param_t *hal_audio_a2dp_dl_get_time_report(void)
{
    return &audio_sync_time_info;
}


//======== Unit test code ========
#if defined(UNIT_TEST)

#define KH_TOTAL_PCM_THRESHOLE 64*1024

static uint16_t KH_Test_Buffer[10 * 1024];
static uint16_t KH_serial_number;
static uint32_t KH_total_count;
static bool KH_is_eof;
volatile static  bool KH_is_media_end;

static void hal_audio_unit_test_fill_data(void)
{
    uint32_t i, byte_count, sample_count;
    uint16_t *p_buf = KH_Test_Buffer;

    if (KH_is_eof) {
        // skip
        return;
    }

    // Query data count
    hal_audio_get_stream_out_sample_count(&byte_count);

    // Prepare data
    sample_count = byte_count / 2;
    for (i = 0; i < sample_count; i++) {
        *p_buf++ = KH_serial_number++;
    }

    // Write to HAL
    hal_audio_write_stream_out(KH_Test_Buffer, sample_count * 2);

    KH_total_count += sample_count;

    log_hal_msgid_info("\r\n CM4 UT: total data %d \r\n", 1, KH_total_count);

    if (KH_total_count >= KH_TOTAL_PCM_THRESHOLE) {
        KH_is_eof = true;
        hal_audio_dsp_controller_send_message(MSG_MCU2DSP_PLAYBACK_CONFIG, AUDIO_PLAYBACK_CONFIG_EOF, 1, false);
        log_hal_msgid_info("\r\n CM4 UT: send EOF \r\n", 0);
    }
}

static void hal_audio_unit_test_isr_handler(hal_audio_event_t event, void *data)
{
    switch (event) {
        case HAL_AUDIO_EVENT_DATA_REQUEST:
            hal_audio_unit_test_fill_data();
            break;
        case HAL_AUDIO_EVENT_END:
            KH_is_media_end = true;
            hal_audio_dsp_controller_send_message(AUDIO_CCNI_MESSAGE_ACK(MSG_DSP2MCU_PLAYBACK_ERROR), AUDIO_PLAYBACK_CONFIG_EOF, 1, false);
            break;
    }
}

void hal_audio_unit_test(void)
{

    //== PCM open & start
    // Set information
    hal_audio_set_stream_out_sampling_rate(HAL_AUDIO_SAMPLING_RATE_44_1KHZ);
    hal_audio_set_stream_out_channel_number(HAL_AUDIO_STEREO);

    // Hook callback
    hal_audio_register_stream_out_callback(hal_audio_unit_test_isr_handler, NULL);

    // Prebuffer
    hal_audio_unit_test_fill_data();

    // Start
    log_hal_msgid_info("\r\n CM4 UT: start stream out ++\r\n", 0);
    hal_audio_start_stream_out(HAL_AUDIO_PLAYBACK_MUSIC);
    log_hal_msgid_info("\r\n CM4 UT: start stream out --\r\n", 0);

    // Wait for data consume
    do {

    } while (!KH_is_media_end);

    // PCM stop & close
    log_hal_msgid_info("\r\n CM4 UT: stop stream out ++\r\n", 0);
    hal_audio_stop_stream_out();
    log_hal_msgid_info("\r\n CM4 UT: stop stream out --\r\n", 0);
}
#endif // defined(UNIT_TEST)

// for action sync mechanism
cm4_dsp_audio_sync_request_param_t *hal_audio_query_audio_sync_request_param(void)
{
    return (cm4_dsp_audio_sync_request_param_t *)&audio_sync_request_parm[0];
}
#endif /* defined(HAL_AUDIO_MODULE_ENABLED) */
