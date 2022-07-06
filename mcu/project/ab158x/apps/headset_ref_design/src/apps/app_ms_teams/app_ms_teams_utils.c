
/* Copyright Statement:
 *
 * (C) 2019  Airoha Technology Corp. All rights reserved.
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

/**
 * File: app_ms_teams_utils.c
 *
 * Description:
 * This file is used to provide the common function for ms_teams
 *
 */

#include "app_ms_teams_utils.h"
#include "ui_shell_activity.h"
#include "ui_shell_manager.h"
#ifdef MTK_RACE_CMD_ENABLE
#include "race_cmd.h"
#include "race_cmd_relay_cmd.h"
#include "race_noti.h"
#include "race_bt.h"
#endif
#include "app_ms_teams_idle_activity.h"
#include "ms_teams_sys_mem.h"
#include <string.h>

#define TAG "[MS TEAMS] utils "

static app_teams_telemetry_info s_telemetry_info = {
    NULL,     //endpoint fw
    (uint8_t*)"v1.0.0",   //base fw
    true,   //don to setting
    NULL,     //endpoint model id
    NULL,     //endpoint sn
    (uint8_t*)"SN202202150001",   //base sn
    0,      //side tone level
    1,      //audio codec
    0x17,    //dsp effect
    0x0,    //mute lock
    0xff,   // headset worn
    0x1,    //battery level
    0x0,    //device ready
    0x1,    //radio link quality
    (uint8_t*)"None", // error msg
    NULL,     // btn press info
    0xFF,    //wired device changed, set to 1 only when the connected device changed (device A->B)
    0x0,    //local conference count
    0x0,    //voice mute
    0x1,    //dual purpose btn
};

typedef struct {
    uint8_t key;
    uint8_t val;
} ms_teams_telemetry_key_value_uint8;
typedef struct {
    uint8_t key;
    uint16_t val;
} ms_teams_telemetry_key_value_uint16;
typedef struct {
    uint8_t key;
    uint32_t val;
} ms_teams_telemetry_key_value_uint32;

#define MAX_TELEMETRY_LIST_INT8_MAX_NUMS 16
static ms_teams_telemetry_key_value_uint8 s_telemetry_dt_list[MAX_TELEMETRY_LIST_INT8_MAX_NUMS] = {{0}};
#define MAX_TELEMETRY_LIST_INT16_MAX_NUMS 8
static ms_teams_telemetry_key_value_uint16 s_telemetry_dt_list1[MAX_TELEMETRY_LIST_INT16_MAX_NUMS] = {{0}};
#define MAX_TELEMETRY_LIST_INT32_MAX_NUMS 2
static ms_teams_telemetry_key_value_uint32 s_telemetry_dt_list2[MAX_TELEMETRY_LIST_INT32_MAX_NUMS] = {{0}};

void ms_teams_telemetry_report_uint8_value(uint8_t key, uint8_t value, bool sync)
{
    uint32_t idx = 0;
    for (idx = 0; idx < MAX_TELEMETRY_LIST_INT8_MAX_NUMS; idx++) {
        if (s_telemetry_dt_list[idx].key == 0 || s_telemetry_dt_list[idx].key == key) {
            s_telemetry_dt_list[idx].key = key;
            s_telemetry_dt_list[idx].val = value;
            break;
        }
    }

    if (idx == MAX_TELEMETRY_LIST_INT8_MAX_NUMS) {
        APPS_LOG_MSGID_E(TAG"no enough memory for uint8 report.", 0);
    }

    if (sync) {
        ms_teams_btn_press_type_t type = MS_TEAMS_BTN_PRESS_TYPE_NONE;
        ms_teams_error_code_t ret = ms_teams_send_action(MS_TEAMS_ACTION_TEAMS_BTN_INVOKE, &type, sizeof(ms_teams_btn_press_type_t));
        type = MS_TEAMS_BTN_PRESS_TYPE_NONE;
        ret = ms_teams_send_action(MS_TEAMS_ACTION_TEAMS_BTN_RELEASE, &type, sizeof(ms_teams_btn_press_type_t));
        APPS_LOG_MSGID_I(TAG"report uint8 item, key=%d, ret=%d.", 2, key, ret);
    }
}

void ms_teams_telemetry_report_uint16_value(uint8_t key, uint16_t value, bool sync)
{
    uint32_t idx = 0;
    for (idx = 0; idx < MAX_TELEMETRY_LIST_INT16_MAX_NUMS; idx++) {
        if (s_telemetry_dt_list1[idx].key == 0 || s_telemetry_dt_list1[idx].key == key) {
            s_telemetry_dt_list1[idx].key = key;
            s_telemetry_dt_list1[idx].val = value;
            break;
        }
    }

    if (idx == MAX_TELEMETRY_LIST_INT16_MAX_NUMS) {
        APPS_LOG_MSGID_E(TAG"no enough memory for uint16 report.", 0);
    }

    if (sync) {
        ms_teams_btn_press_type_t type = MS_TEAMS_BTN_PRESS_TYPE_NONE;
        ms_teams_error_code_t ret = ms_teams_send_action(MS_TEAMS_ACTION_TEAMS_BTN_INVOKE, &type, sizeof(ms_teams_btn_press_type_t));
        type = MS_TEAMS_BTN_PRESS_TYPE_NONE;
        ret = ms_teams_send_action(MS_TEAMS_ACTION_TEAMS_BTN_RELEASE, &type, sizeof(ms_teams_btn_press_type_t));
        APPS_LOG_MSGID_I(TAG"report uint16 item, key=%d, value=%d, ret=%d.", 3, key, value, ret);
    }
}

#if 0
void ms_teams_telemetry_report_initial_value()
{
    ms_teams_btn_press_type_t type = MS_TEAMS_BTN_PRESS_TYPE_NONE;
    ms_teams_error_code_t ret = ms_teams_send_action(MS_TEAMS_ACTION_TEAMS_BTN_INVOKE, &type, sizeof(ms_teams_btn_press_type_t));
    type = MS_TEAMS_BTN_PRESS_TYPE_NONE;
    ret = ms_teams_send_action(MS_TEAMS_ACTION_TEAMS_BTN_RELEASE, &type, sizeof(ms_teams_btn_press_type_t));
}
#endif

void ms_teams_telemetry_report_uint32_value(uint8_t key, uint32_t value, bool sync)
{
    uint32_t idx = 0;
    for (idx = 0; idx < MAX_TELEMETRY_LIST_INT32_MAX_NUMS; idx++) {
        if (s_telemetry_dt_list2[idx].key == 0 || s_telemetry_dt_list2[idx].key == key) {
            s_telemetry_dt_list2[idx].key = key;
            s_telemetry_dt_list2[idx].val = value;
            break;
        }
    }

    if (idx == MAX_TELEMETRY_LIST_INT32_MAX_NUMS) {
        APPS_LOG_MSGID_E(TAG"no enough memory for uint32 report.", 0);
    }

    if (sync) {
        ms_teams_btn_press_type_t type = MS_TEAMS_BTN_PRESS_TYPE_NONE;
        ms_teams_error_code_t ret = ms_teams_send_action(MS_TEAMS_ACTION_TEAMS_BTN_INVOKE, &type, sizeof(ms_teams_btn_press_type_t));
        type = MS_TEAMS_BTN_PRESS_TYPE_NONE;
        ret = ms_teams_send_action(MS_TEAMS_ACTION_TEAMS_BTN_RELEASE, &type, sizeof(ms_teams_btn_press_type_t));
        APPS_LOG_MSGID_I(TAG"report uint32 item, key=%d, ret=%d.", 2, key, ret);
    }
}

void ms_teams_update_product_info(uint8_t *info, uint32_t total_len)
{
    static uint8_t *product_info = NULL;
    if (product_info != NULL) {
        ms_teams_free(product_info);
        product_info = NULL;
    }
    product_info = ms_teams_malloc(total_len);
    if (product_info == NULL) {
        APPS_LOG_MSGID_E(TAG"no enough memory for FW.", 0);
        return;
    }
    ms_teams_memcpy(product_info, info, total_len);
    s_telemetry_info.endpoint_fw = product_info;
    s_telemetry_info.endpoint_mode_id = &product_info[strnlen((const char*)s_telemetry_info.endpoint_fw, total_len) + 1];
    s_telemetry_info.endpoint_sn = &product_info[strnlen((const char*)s_telemetry_info.endpoint_mode_id, total_len) + strnlen((const char*)s_telemetry_info.endpoint_fw, total_len) + 2];
}

static void ms_teams_event_handler(ms_teams_event_t ev, uint32_t sub_event, uint8_t *data, uint32_t data_len)
{
    uint32_t shell_event = 0;
    uint8_t *data_copy = NULL;
    ui_shell_status_t ui_ret = UI_SHELL_STATUS_OK;

    APPS_LOG_MSGID_I(TAG"teams event callback: 0x%x, 0x%x, 0x%x, 0x%x.", 4, ev, sub_event, data, data_len);
#ifdef MTK_AWS_MCE_ENABLE
    /* TODO: send event and data to partner. */
#endif
    shell_event = ((ev << 16) & 0xFFFF0000) | (sub_event & 0xFFFF);
    if (data != NULL && data_len != 0) {
        data_copy = (uint8_t *)pvPortMalloc(data_len);
        if (data_copy == NULL) {
            APPS_LOG_MSGID_I(TAG"teams event handler malloc mem failed.", 0);
            return;
        }
        memcpy(data_copy, data, data_len);
    } else {
        data_len = 0;
    }

    ui_ret = ui_shell_send_event(true, EVENT_PRIORITY_MIDDLE, EVENT_GROUP_UI_SHELL_MS_TEAMS,
                                 shell_event,
                                 data_copy,
                                 data_len,
                                 NULL, 0);
    if (ui_ret != UI_SHELL_STATUS_OK && data_len != 0) {
        APPS_LOG_MSGID_E(TAG"teams event handler send event fail.", 0);
        vPortFree(data_copy);
    }
}

/* TODO, update every telemetry item */
static void ms_teams_telemetry_handler(ms_teams_telemetry_type_t *req_list, uint32_t req_len, uint8_t **data, uint32_t *data_len)
{
    static uint8_t invalid_str = 0x0;
    static uint8_t telemetry_rsp_buf[20] = {0};
    uint32_t rsp_len = 0;
    for (uint32_t idx = 0; idx < req_len; idx++) {
        APPS_LOG_MSGID_I(TAG"teams telemetry callbackm type=0x%x.", 1, req_list[idx]);
        if (idx > 0) {
            /* Teams core will fill the telemetry for the first request by default, the usr should fill the follow request in multiple request. */
            telemetry_rsp_buf[rsp_len] = req_list[idx];
            rsp_len += 1;
        }
        switch (req_list[idx]) {
            case MS_TEAMS_TELEMETRY_ENDPOINT_FW:
                if (s_telemetry_info.endpoint_fw != NULL) {
                    *data = s_telemetry_info.endpoint_fw;
                    *data_len = strnlen((const char*)s_telemetry_info.endpoint_fw, 32);
                } else {
                    *data = &invalid_str;
                    *data_len = 1;
                }
                return;
            case MS_TEAMS_TELEMETRY_FW:
                if (s_telemetry_info.base_fw != NULL) {
                    *data = s_telemetry_info.base_fw;
                    *data_len = strnlen((const char*)s_telemetry_info.base_fw, 32);
                } else {
                    *data = &invalid_str;
                    *data_len = 1;
                }
                return;
            case MS_TEAMS_TELEMETRY_DON_TO_ANS_SETTING:
                telemetry_rsp_buf[rsp_len] = s_telemetry_info.don_to_seeting;
                rsp_len += 1;
                break;
            case MS_TEAMS_TELEMETRY_ENDPOINT_MODEL_ID:
                if (s_telemetry_info.endpoint_mode_id != NULL) {
                    *data = s_telemetry_info.endpoint_mode_id;
                    *data_len = strnlen((const char*)s_telemetry_info.endpoint_mode_id, 32);
                } else {
                    *data = &invalid_str;
                    *data_len = 1;
                }
                return;
            case MS_TEAMS_TELEMETRY_ENDPOINT_SN:
                if (s_telemetry_info.endpoint_sn != NULL) {
                    *data = s_telemetry_info.endpoint_sn;
                    *data_len = strnlen((const char*)s_telemetry_info.endpoint_sn, 32);
                } else {
                    *data = &invalid_str;
                    *data_len = 1;
                }
                return;
            case MS_TEAMS_TELEMETRY_SN:
                if (s_telemetry_info.base_sn != NULL) {
                    *data = s_telemetry_info.base_sn;
                    *data_len = strnlen((const char*)s_telemetry_info.base_sn, 32);
                } else {
                    *data = &invalid_str;
                    *data_len = 1;
                }
                return;
            case MS_TEAMS_TELEMETRY_SIDETONE_LEVEL: {
                static uint8_t side_tone_level[] = {'0'};
                *data = side_tone_level;
                *data_len = 1;
            }
            return;
            case MS_TEAMS_TELEMETRY_AUDIO_CODEC_USED:
                telemetry_rsp_buf[rsp_len] = s_telemetry_info.audio_codec;
                rsp_len += 1;
                break;
            case MS_TEAMS_TELEMETRY_DSP_EFFECTS_ENABLED: {
                telemetry_rsp_buf[rsp_len] = s_telemetry_info.dsp_effect & 0xFF;
                telemetry_rsp_buf[rsp_len + 1] = (s_telemetry_info.dsp_effect >> 8) & 0xFF;
                telemetry_rsp_buf[rsp_len + 2] = (s_telemetry_info.dsp_effect >> 16) & 0xFF;
                telemetry_rsp_buf[rsp_len + 3] = (s_telemetry_info.dsp_effect >> 24) & 0xFF;
                rsp_len += 4;
            }
            break;
            case MS_TEAMS_TELEMETRY_MUTE_LOCK:
                telemetry_rsp_buf[rsp_len] = s_telemetry_info.mute_lock;
                rsp_len += 1;
                break;
            case MS_TEAMS_TELEMETRY_HEADSET_WORN:
                telemetry_rsp_buf[rsp_len] = s_telemetry_info.headset_worn;
                rsp_len += 1;
                break;
            case MS_TEAMS_TELEMETRY_BATTERY_LEVEL:
                telemetry_rsp_buf[rsp_len] = s_telemetry_info.battery_level;
                rsp_len += 1;
                break;
            case MS_TEAMS_TELEMETRY_DEVICE_READY:
                telemetry_rsp_buf[rsp_len] = s_telemetry_info.device_ready;
                rsp_len += 1;
                break;
            case MS_TEAMS_TELEMETRY_RADIO_LINK_QUALITY:
                telemetry_rsp_buf[rsp_len] = s_telemetry_info.radio_link_quality;
                rsp_len += 1;
                break;
            case MS_TEAMS_TELEMETRY_ERR_MSG:
                break;
            case MS_TEAMS_TELEMETRY_BTN_PRESS_INFO:
                break;
            case MS_TEAMS_TELEMETRY_WIREL_DEVICE_CHANGE:
                telemetry_rsp_buf[rsp_len] = s_telemetry_info.wired_device_changed;
                rsp_len += 1;
                break;
#if 0
            case MS_TEAMS_TELEMETRY_PEOPLE_CNT:
                break
#endif
            case MS_TEAMS_TELEMETRY_LOCAL_CONFERENCE_CNT:
                telemetry_rsp_buf[rsp_len] = s_telemetry_info.local_conference_cnt & 0xFF;
                telemetry_rsp_buf[rsp_len + 1] = (s_telemetry_info.local_conference_cnt >> 8) & 0xFF;
                telemetry_rsp_buf[rsp_len + 2] = (s_telemetry_info.local_conference_cnt >> 16) & 0xFF;
                telemetry_rsp_buf[rsp_len + 3] = (s_telemetry_info.local_conference_cnt >> 24) & 0xFF;
                rsp_len += 4;
                break;
#if 1
            case MS_TEAMS_TELEMETRY_DEVICE_MIC_STA:
            case MS_TEAMS_TELEMETRY_DEVICE_SPK_STA:
            case MS_TEAMS_TELEMETRY_DEVICE_AUD_STREAM_STA:
            case MS_TEAMS_TELEMETRY_DEVICE_VIDEO_STREAM_STA:
            case MS_TEAMS_TELEMETRY_AI_MODEL_AND_VERSION:
            case MS_TEAMS_TELEMETRY_DEVICE_RESOURCE_USAGE:
                *data = &invalid_str;
                *data_len = 1;
                return;
#endif
#if 0
            /* DT item only. */
            case MS_TEAMS_TELEMETRY_VOICE_MUTE_ACTIVITY:
                telemetry_rsp_buf[rsp_len] = s_telemetry_info.voice_mute;
                rsp_len += 1;
                break;
#endif
            case MS_TEAMS_TELEMETRY_DUAL_PURPOS_TEAMS_BTN:
                telemetry_rsp_buf[rsp_len] = s_telemetry_info.dual_purpose_btn;
                rsp_len += 1;
                break;
        }
    }

    if (rsp_len != 0) {
        *data = telemetry_rsp_buf;
        *data_len = rsp_len;
    } else if (req_len == 0) {
        APPS_LOG_MSGID_I(TAG"teams telemetry callback, query context.", 0);
        uint32_t idx = 0;
        for (idx = 0; idx < MAX_TELEMETRY_LIST_INT16_MAX_NUMS; idx++) {
            if (s_telemetry_dt_list1[idx].key != 0) {
                telemetry_rsp_buf[0] = s_telemetry_dt_list1[idx].key;
                s_telemetry_dt_list1[idx].key = 0;
                telemetry_rsp_buf[1] = (s_telemetry_dt_list1[idx].val >> 8) & 0xFF;
                telemetry_rsp_buf[2] = (s_telemetry_dt_list1[idx].val) & 0xFF;
                *data = telemetry_rsp_buf;
                *data_len = 3;
                //APPS_LOG_MSGID_I(TAG"teams telemetry callback, query context=%d.", 1, telemetry_rsp_buf[0]);
                return;
            }
        }
        uint32_t uint8_nums = 0;
        for (idx = 0; idx < MAX_TELEMETRY_LIST_INT8_MAX_NUMS; idx++) {
            if (s_telemetry_dt_list[idx].key != 0) {
                telemetry_rsp_buf[uint8_nums] = s_telemetry_dt_list[idx].key;
                s_telemetry_dt_list[idx].key = 0;
                telemetry_rsp_buf[uint8_nums + 1] = s_telemetry_dt_list[idx].val;
                uint8_nums += 2;
            }
        }
        if (uint8_nums != 0) {
            *data = telemetry_rsp_buf;
            *data_len = uint8_nums;
            return;
        }
        for (idx = 0; idx < MAX_TELEMETRY_LIST_INT32_MAX_NUMS; idx++) {
            if (s_telemetry_dt_list2[idx].key != 0) {
                telemetry_rsp_buf[0] = s_telemetry_dt_list2[idx].key;
                s_telemetry_dt_list2[idx].key = 0;
                telemetry_rsp_buf[1] = s_telemetry_dt_list2[idx].val & 0xff;
                telemetry_rsp_buf[2] = (s_telemetry_dt_list2[idx].val >> 8) & 0xff;
                telemetry_rsp_buf[3] = (s_telemetry_dt_list2[idx].val >> 16) & 0xff;
                telemetry_rsp_buf[4] = (s_telemetry_dt_list2[idx].val >> 24) & 0xff;
                *data = telemetry_rsp_buf;
                *data_len = 5;
                return;
            }
        }
    }
}

app_teams_telemetry_info *app_ms_teams_get_telemetry_info()
{
    return &s_telemetry_info;
}

void app_ms_teams_init()
{
    ms_teams_error_code_t ret = MS_TEAMS_ERROR_CODE_NONE;
    ms_teams_config_t config = {
#ifndef ULL_DONGLE
        MS_TEAMS_BT_SRV | MS_TEAMS_USB_SRV,
#else
        MS_TEAMS_USB_PROXY
#endif
        ms_teams_event_handler,
        ms_teams_telemetry_handler,
        0x02
    };

    ret = ms_teams_init(&config);
    APPS_LOG_MSGID_I(TAG"init result: %d", 1, ret);
}

void app_ms_teams_set_mmi_state(ms_teams_call_state_sub_event_t ev)
{
    apps_config_state_t mmi_sta = APP_TOTAL_STATE_NO;
    /* TODO: handle the state update logic with HFP */
    switch (ev) {
        case MS_TEAMS_CALL_EVENT_END_CALL:
            ui_shell_send_event(false, EVENT_PRIORITY_HIGNEST, EVENT_GROUP_UI_SHELL_APP_INTERACTION,
                                APPS_EVENTS_INTERACTION_UPDATE_MMI_STATE, NULL, 0, NULL, 0);
            ui_shell_send_event(false, EVENT_PRIORITY_HIGNEST, EVENT_GROUP_UI_SHELL_APP_INTERACTION,
                                APPS_EVENTS_INTERACTION_UPDATE_LED_BG_PATTERN, NULL, 0,
                                NULL, 0);
            break;
        case MS_TEAMS_CALL_EVENT_OUTGOING_CALL:
            mmi_sta = APP_HFP_OUTGOING;
            apps_config_set_background_led_pattern(LED_INDEX_OUTGOING_CALL, true, APPS_CONFIG_LED_AWS_SYNC_PRIO_MIDDLE);
            break;
        case MS_TEAMS_CALL_EVENT_INCOMING_CALL:
            mmi_sta = APP_HFP_INCOMING;
            apps_config_set_background_led_pattern(LED_INDEX_INCOMING_CALL, true, APPS_CONFIG_LED_AWS_SYNC_PRIO_MIDDLE);
            break;
        case MS_TEAMS_CALL_EVENT_CALL_IN_PROCESS:
            mmi_sta = APP_HFP_CALLACTIVE;
            apps_config_set_background_led_pattern(LED_INDEX_CALL_ACTIVE, true, APPS_CONFIG_LED_AWS_SYNC_PRIO_MIDDLE);
            break;
        case MS_TEAMS_CALL_EVENT_CALLER_ID:
            break;
        case MS_TEAMS_CALL_EVENT_MUTE_STATE_UPDATE:
            break;
        case MS_TEAMS_CALL_EVENT_HOLD_CALL:
            mmi_sta = APP_STATE_HELD_ACTIVE;
            apps_config_set_background_led_pattern(LED_INDEX_CALL_ACTIVE, true, APPS_CONFIG_LED_AWS_SYNC_PRIO_MIDDLE);
            break;
    }

    if (mmi_sta != APP_TOTAL_STATE_NO) {
        apps_config_key_set_mmi_state(mmi_sta);
    }

    APPS_LOG_MSGID_I(TAG"mmi update to 0x%x with 0x%x", 2, mmi_sta, ev);
}

bool app_ms_teams_is_dongle_connected()
{
#ifdef AIR_LE_AUDIO_ENABLE
    APPS_LOG_MSGID_I(TAG"dongle connected, le-audio case", 0);
    return true;
#else
#if defined(MTK_RACE_CMD_ENABLE)
    bt_bd_addr_t addr_list[1];
    uint32_t list_num = 1;
    list_num = bt_cm_get_connected_devices(BT_CM_PROFILE_SERVICE_MASK(BT_CM_PROFILE_SERVICE_CUSTOMIZED_ULL),
                                           addr_list, list_num);
    APPS_LOG_MSGID_I(TAG"dongle connected debug %d,0x%x", 2, list_num, bt_cm_get_connected_profile_services(addr_list[0]));
    if (list_num > 0 &&
        BT_CM_PROFILE_SERVICE_MASK(BT_CM_PROFILE_SERVICE_AIR) & bt_cm_get_connected_profile_services(addr_list[0])) {
        APPS_LOG_MSGID_I(TAG"dongle connected", 0);
        return true;
    }
#endif
    return false;
#endif
}

#if 0
void app_ms_teams_send_race_data_to_dongle(uint8_t type, uint8_t *data, uint32_t data_len)
{
#if defined(MTK_RACE_CMD_ENABLE)
    uint8_t channel_id = race_get_channel_id_by_port_type(RACE_SERIAL_PORT_TYPE_SPP);
    ms_teams_race_pkg_t *pkg = RACE_ClaimPacket((uint8_t)RACE_TYPE_COMMAND_WITHOUT_RSP,
                                                (uint16_t)MS_TEAMS_RACE_SET_DATA,
                                                (uint16_t)(sizeof(uint8_t) + data_len),
                                                channel_id);
    pkg->type = type;
    if (data_len > 0) {
        memcpy(&pkg->data, data, data_len);
    }
    RACE_ERRCODE race_notify_ret = race_noti_send(pkg, channel_id, false);
    APPS_LOG_MSGID_I(TAG"set race data to dongle ret=%d", 1, race_notify_ret);
    if (race_notify_ret != RACE_ERRCODE_SUCCESS) {
        RACE_FreePacket((void *)pkg);
    }
#endif
}

static void *apps_ms_teams_race_handler(ptr_race_pkt_t p_race_package, uint16_t length, uint8_t channel_id)
{
    typedef struct {
        RACE_COMMON_HDR_STRU cmdhdr;
        uint8_t data[0];
    } PACKED CMD1;
    CMD1 *cmd = (CMD1 *)p_race_package;
    APPS_LOG_MSGID_I(TAG"receive data from headset, type=%d, len=%d", 2, cmd->data[0], cmd->cmdhdr.length);
    if (cmd->data[0] == 0) {
        ms_teams_btn_press_type_t type = MS_TEAMS_BTN_PRESS_TYPE_LONG_PRESS;
        ms_teams_error_code_t ret = ms_teams_send_action(MS_TEAMS_ACTION_TEAMS_BTN_INVOKE, &type, sizeof(ms_teams_btn_press_type_t));
        APPS_LOG_MSGID_I(TAG"call btn invoke, ret=%d.", 1, ret);
    } else {
        ms_teams_btn_press_type_t type = MS_TEAMS_BTN_PRESS_TYPE_NONE;
        ms_teams_error_code_t ret = ms_teams_send_action(MS_TEAMS_ACTION_TEAMS_BTN_RELEASE, &type, sizeof(ms_teams_btn_press_type_t));
        APPS_LOG_MSGID_I(TAG"call btn invoke, ret=%d.", 1, ret);
    }
    return NULL;
}

void app_ms_teams_event_init()
{
#if defined(MTK_RACE_CMD_ENABLE)
    RACE_HANDLER handler = {
        .id_start = MS_TEAMS_RACE_RACE_ID_START,
        .id_end = MS_TEAMS_RACE_RACE_ID_END,
        .handler = apps_ms_teams_race_handler
    };
    race_status_t ret = RACE_Register_Handler(&handler);
    APPS_LOG_MSGID_I(TAG"init race ret=%d", 1, ret);
#endif
}
#endif

