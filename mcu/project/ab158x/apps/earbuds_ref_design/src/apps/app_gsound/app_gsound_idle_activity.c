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
 * File: app_gsound_idle_activity.c
 *
 * Description: This file is the activity to handle the action from APP layer.
 */

#ifdef AIR_GSOUND_ENABLE

#include "app_gsound_idle_activity.h"

#include "gsound_api.h"
#include "app_gsound_battery_ohd.h"
#include "app_gsound_device_action.h"
#include "app_gsound_event.h"
#include "app_gsound_service.h"
#include "app_gsound_multi_va.h"

#include "apps_aws_sync_event.h"
#include "app_bt_takeover_service.h"
#include "apps_config_key_remapper.h"
#include "apps_control_touch_key_status.h"
#include "apps_events_battery_event.h"
#include "apps_events_key_event.h"
#ifdef AIR_SMART_CHARGER_ENABLE
#include "app_smcharger.h"
#endif
#ifdef AIR_PROMPT_SOUND_ENABLE
#include "voice_prompt_api.h"
#include "apps_config_vp_index_list.h"
#endif

#ifdef MTK_ANC_ENABLE
#include "anc_control_api.h"
#endif
#ifdef MTK_BATTERY_MANAGEMENT_ENABLE
#include "battery_management.h"
#endif
#ifdef MTK_AWS_MCE_ENABLE
#include "bt_aws_mce_srv.h"
#include "bt_aws_mce_report.h"
#endif
#include "bt_sink_srv_ami.h"
#ifndef MULTI_VA_SUPPORT_COMPETITION
#include "multi_va_manager.h"
#endif


#define LOG_TAG "[GS][APP][ACTIVITY]"

/**
 * @brief State of GSound key.
 */
enum {
    KEY_NOT_PREPARED, /**< key state none */
    KEY_PREPARED,     /**< key state press */
    KEY_RELEASE,      /**< key state release */
    KEY_PTT,          /**< key state Push To Talk */
};

static uint32_t             app_gsound_is_key_prepared;

static bool app_gsound_takeover_service_allow(const bt_bd_addr_t remote_addr)
{
    bool working = gsound_is_voice_working();
    GSOUND_LOG_I(LOG_TAG" takeover_service_allow, allow=%d", 1, !working);
    return (!working);
}

static bool gsound_idle_proc_ui_shell_group(ui_shell_activity_t *self, uint32_t event_id, void *extra_data, size_t data_len)
{
    bool ret = TRUE;
    switch (event_id) {
        case EVENT_ID_SHELL_SYSTEM_ON_CREATE: {
            GSOUND_LOG_I(LOG_TAG" CREATE", 0);
            app_gsound_srv_init();
            app_gsound_battery_init();
            app_bt_takeover_service_user_register(APP_BT_TAKEOVER_ID_BISTO, app_gsound_takeover_service_allow);
            break;
        }
        default:
            break;
    }
    return ret;
}

static bool app_gsound_map_and_notify_action(apps_config_key_action_t key_event)
{
    bool ret = FALSE;
    bt_sink_srv_am_type_t scenario = bt_sink_srv_ami_get_current_scenario();
    switch (key_event) {
        case KEY_GSOUND_PRESS:
            if (scenario != HFP) {
                app_gsound_is_key_prepared = KEY_PREPARED;
                ret = TRUE;
            }
            break;
        case KEY_GSOUND_VOICE_QUERY:
            if (app_gsound_is_key_prepared == KEY_PREPARED) {
                gsound_send_action_event((GSOUND_TARGET_ACTION_GA_VOICE_PREPARE | GSOUND_TARGET_ACTION_GA_FETCH_PREPARE), NULL);
                gsound_send_action_event(GSOUND_TARGET_ACTION_GA_VOICE_PTT_OR_FETCH | GSOUND_TARGET_ACTION_GA_WILL_PAUSE, NULL);
                app_gsound_is_key_prepared = KEY_PTT;
                ret = TRUE;
            }
            break;
        case KEY_GSOUND_ENDPOINTING:
            if (app_gsound_is_key_prepared == KEY_RELEASE) {
                gsound_send_action_event(GSOUND_TARGET_ACTION_GA_STOP_ASSISTANT | GSOUND_TARGET_ACTION_GA_START_ENDPOINTING_QUERY | GSOUND_TARGET_ACTION_TOGGLE_PLAY_PAUSE, NULL);
                ret = TRUE;
            }
            break;
        case KEY_GSOUND_NOTIFY:
            break;
        case KEY_GSOUND_RELEASE:
            if (app_gsound_is_key_prepared == KEY_PREPARED) {
                app_gsound_is_key_prepared = KEY_RELEASE;
                ret = TRUE;
            } else if (app_gsound_is_key_prepared == KEY_PTT) {
                gsound_send_action_event(GSOUND_TARGET_ACTION_GA_VOICE_DONE, NULL);
                app_gsound_is_key_prepared = KEY_NOT_PREPARED;
                ret = TRUE;
            }
            break;
        case KEY_GSOUND_CANCEL:
        case KEY_AVRCP_PLAY:
        case KEY_AVRCP_PAUSE: {
            gsound_send_action_event(GSOUND_TARGET_ACTION_GA_VOICE_PREPARE | GSOUND_TARGET_ACTION_GA_FETCH_PREPARE, NULL);
            gsound_send_action_event(GSOUND_TARGET_ACTION_GA_VOICE_DONE, NULL);
            gsound_send_action_event(GSOUND_TARGET_ACTION_GA_WILL_PAUSE, NULL);
            if (gsound_send_action_event(GSOUND_TARGET_ACTION_GA_STOP_ASSISTANT | GSOUND_TARGET_ACTION_TOGGLE_PLAY_PAUSE, NULL)) {
                ret = TRUE;
            }
            break;
        }
        case KEY_AVRCP_FORWARD: {
            gsound_send_action_event(GSOUND_TARGET_ACTION_GA_VOICE_PREPARE | GSOUND_TARGET_ACTION_GA_FETCH_PREPARE, NULL);
            gsound_send_action_event(GSOUND_TARGET_ACTION_GA_VOICE_DONE, NULL);
            gsound_send_action_event(GSOUND_TARGET_ACTION_GA_WILL_PAUSE, NULL);
            if (gsound_send_action_event(GSOUND_TARGET_ACTION_NEXT_TRACK, NULL)) {
                ret = TRUE;
            }
            break;
        }
        case KEY_AVRCP_BACKWARD: {
            gsound_send_action_event(GSOUND_TARGET_ACTION_GA_VOICE_PREPARE | GSOUND_TARGET_ACTION_GA_FETCH_PREPARE, NULL);
            gsound_send_action_event(GSOUND_TARGET_ACTION_GA_VOICE_DONE, NULL);
            gsound_send_action_event(GSOUND_TARGET_ACTION_GA_WILL_PAUSE, NULL);
            if (gsound_send_action_event(GSOUND_TARGET_ACTION_PREV_TRACK, NULL)) {
                ret = TRUE;
            }
            break;
        }
        default:
            break;
    }
    GSOUND_LOG_I(LOG_TAG" map_key_to_action, scenario=%d is_key_prepared=%d key_event=0x%04X ret=%d",
                 4, scenario, app_gsound_is_key_prepared, key_event, ret);
    return ret;
}

#ifdef AIRO_KEY_EVENT_ENABLE
static bool app_gsound_proc_key_group(ui_shell_activity_t *self, uint32_t event_id, void *extra_data, size_t data_len)
{
    bool ret = FALSE;
    uint8_t key_id = 0;
    airo_key_event_t key_event = AIRO_KEY_INVALID;
    app_event_key_event_decode(&key_id, &key_event, event_id);
    apps_config_key_action_t action;
    if (extra_data) {
        action = *(uint16_t *)extra_data;
    } else {
        action = apps_config_key_event_remapper_map_action(key_id, key_event);
    }

    //GSOUND_LOG_I(LOG_TAG" KEY Event, [%02X] action=0x%04X scenario=%d", 3, role, action, scenario);

    if ((action >= KEY_GSOUND_PRESS && action <= KEY_GSOUND_CANCEL)
        || action == KEY_AVRCP_PLAY || action == KEY_AVRCP_PAUSE
        || action == KEY_AVRCP_FORWARD || action == KEY_AVRCP_BACKWARD) {
//        if (!app_gsound_srv_is_connected()) {
//            GSOUND_LOG_I(LOG_TAG" KEY Event, ignore - GSound not connected", 0);
//            return FALSE;
//        }
        if (gsound_is_ld_suspend()) {
            GSOUND_LOG_E(LOG_TAG" KEY Event, ignore - GSound LD Suspending", 0);
            return TRUE;
        }

#ifdef MTK_AWS_MCE_ENABLE
        bt_aws_mce_role_t role = bt_connection_manager_device_local_info_get_aws_role();
        if (role == BT_AWS_MCE_ROLE_PARTNER) {
            bool sync_to_agent = FALSE;
            bt_aws_mce_srv_link_type_t aws_link_type = bt_aws_mce_srv_get_link_type();
            if (aws_link_type != BT_AWS_MCE_SRV_LINK_NONE) {
                bt_status_t bt_status = apps_aws_sync_event_send(EVENT_GROUP_UI_SHELL_KEY, action);
                sync_to_agent = (bt_status == BT_STATUS_SUCCESS);
            }
            if (sync_to_agent) {
                GSOUND_LOG_I(LOG_TAG" KEY Event, Partner sync_to_agent Pass", 0);
            } else {
                voice_prompt_param_t vp = {0};
                vp.vp_index = VP_INDEX_FAILED;
                voice_prompt_play(&vp, NULL);
                //apps_config_set_vp(VP_INDEX_FAILED, FALSE, 100, VOICE_PROMPT_PRIO_MEDIUM, FALSE, NULL);
                GSOUND_LOG_I(LOG_TAG" KEY Event, Partner sync_to_agent Fail", 0);
            }
        } else
#endif
        {
            bool ret = app_gsound_map_and_notify_action(action);
            if (ret) {
                //GSOUND_LOG_I(LOG_TAG" KEY Event, Agent map_and_notify_action Pass", 0);
            } else {
                GSOUND_LOG_I(LOG_TAG" KEY Event, Agent map_and_notify_action Fail", 0);
                if (KEY_AVRCP_PLAY == action || KEY_AVRCP_PAUSE == action) {
                    app_gsound_srv_reject_action(GSOUND_TARGET_ACTION_TOGGLE_PLAY_PAUSE);
                }
            }
        }
        ret = TRUE;
    }
    return ret;
}
#endif

#ifdef MTK_AWS_MCE_ENABLE
static bool app_gsound_proc_aws_group(void *extra_data, size_t data_len)
{
    bool ret = FALSE;
    bt_aws_mce_report_info_t *aws_data_ind = (bt_aws_mce_report_info_t *)extra_data;

    switch (aws_data_ind->module_id) {
        case BT_AWS_MCE_REPORT_MODULE_APP_ACTION: {
            uint32_t event_group = 0;
            uint32_t event_id = 0;
            uint32_t aws_event_extra_data_len = 0;
            void *aws_event_extra_data = NULL;
            bt_aws_mce_role_t role = bt_connection_manager_device_local_info_get_aws_role();
            apps_aws_sync_event_decode_extra(aws_data_ind, &event_group, &event_id, &aws_event_extra_data, &aws_event_extra_data_len);
            switch (event_group) {
                case EVENT_GROUP_UI_SHELL_KEY:
                    GSOUND_LOG_I(LOG_TAG" [AWS_DATA] KEY Event, [%02X] role=%d is_key_prepared=%d",
                                 4, role, event_id, app_gsound_is_key_prepared);
                    if ((event_id >= KEY_GSOUND_PRESS && event_id <= KEY_GSOUND_CANCEL)
                        || event_id == KEY_AVRCP_PLAY || event_id == KEY_AVRCP_PAUSE
                        || event_id == KEY_AVRCP_FORWARD || event_id == KEY_AVRCP_BACKWARD) {
                        bool ret = app_gsound_map_and_notify_action(event_id);
                        if (ret) {
                            GSOUND_LOG_I(LOG_TAG" [AWS_DATA] KEY Event, Agent map_and_notify_action Pass", 0);
                        } else {
                            GSOUND_LOG_I(LOG_TAG" [AWS_DATA] KEY Event, Agent map_and_notify_action Fail", 0);
                            if (event_id == KEY_AVRCP_PLAY || event_id == KEY_AVRCP_PAUSE) {
                                app_gsound_srv_reject_action(GSOUND_TARGET_ACTION_TOGGLE_PLAY_PAUSE);
                            }
                        }
                        ret = TRUE;
                    }
                    break;

                case EVENT_GROUP_UI_SHELL_GSOUND: {
                    uint8_t enable_state = app_gsound_srv_get_enable_state();
                    switch (event_id) {
                        case APPS_EVENT_GSOUND_PARTNER_ENABLE: {
                            GSOUND_LOG_I(LOG_TAG" [AWS_DATA] GSOUND Event, [%02X] PARTNER_ENABLE enable_state=%d",
                                         2, role, enable_state);
                            if (BT_AWS_MCE_ROLE_PARTNER == role
                                && APP_GSOUND_SRV_STATE_DISABLED == enable_state) {
                                app_gsound_srv_enable();
                                app_gsound_srv_set_connected(TRUE);
                            }
                            break;
                        }
                        case APPS_EVENT_GSOUND_PARTNER_DISABLE: {
                            GSOUND_LOG_I(LOG_TAG" [AWS_DATA] GSOUND Event, [%02X] PARTNER_DISABLE enable_state=%d",
                                         2, role, enable_state);
                            if (BT_AWS_MCE_ROLE_PARTNER == role
                                && APP_GSOUND_SRV_STATE_ENABLED == enable_state) {
                                app_gsound_srv_disable();
                                app_gsound_srv_set_connected(FALSE);
                            }
                            break;
                        }
                        case APPS_EVENT_GSOUND_CONNECTED:
                            GSOUND_LOG_I(LOG_TAG" [AWS_DATA] GSOUND Event, [%02X] CONNECTED", 1, role);
                            if (BT_AWS_MCE_ROLE_PARTNER == role) {
                                app_gsound_srv_set_connected(TRUE);
                            }
                            break;
                        case APPS_EVENT_GSOUND_DISCONNECTED:
                            GSOUND_LOG_I(LOG_TAG" [AWS_DATA] GSOUND Event, [%02X] DISCONNECTED", 1, role);
                            if (BT_AWS_MCE_ROLE_PARTNER == role) {
                                app_gsound_srv_set_connected(FALSE);
                            }
                            break;

                        case APPS_EVENT_GSOUND_POWER_OFF_SYNC: {
                            GSOUND_LOG_I(LOG_TAG" [AWS_DATA] GSOUND Event, [%02X] POWER_OFF_SYNC", 1, role);
                            bool *need_rho = (bool *)pvPortMalloc(sizeof(bool));
                            if (need_rho == NULL) {
                                GSOUND_LOG_E(LOG_TAG" [AWS_DATA] GSOUND Event, POWER_OFF_SYNC malloc failed", 0);
                                break;
                            }
                            *need_rho = FALSE;
                            ui_shell_status_t status = ui_shell_send_event(FALSE, EVENT_PRIORITY_HIGNEST,
                                                                           EVENT_GROUP_UI_SHELL_APP_INTERACTION,
                                                                           APPS_EVENTS_INTERACTION_REQUEST_POWER_OFF,
                                                                           (void *)need_rho, sizeof(bool),
                                                                           NULL, 0);
                            if (status != UI_SHELL_STATUS_OK) {
                                vPortFree(need_rho);
                                GSOUND_LOG_E(LOG_TAG" [AWS_DATA] GSOUND Event, POWER_OFF_SYNC send failed", 0);
                            }
                            break;
                        }
                    }
                }
            }
            break;
        }

        case BT_AWS_MCE_REPORT_MODULE_BATTERY: {
            uint8_t peer_battery = 0;
            memcpy(&peer_battery, aws_data_ind->param, sizeof(uint8_t));
            app_gsound_battery_handle_peer_battery(peer_battery);
            break;
        }
    }
    return ret;
}
#endif

#ifdef AIR_SMART_CHARGER_ENABLE
static bool app_gsound_proc_charger_case_group(struct _ui_shell_activity *self, uint32_t event_id, void *extra_data, size_t data_len)
{
    bool ret = FALSE;
    switch (event_id) {
        case SMCHARGER_EVENT_NOTIFY_ACTION: {
            app_smcharger_public_event_para_t *event_para = (app_smcharger_public_event_para_t *)extra_data;
            switch (event_para->action) {
                case SMCHARGER_CHARGER_IN_ACTION: {
                    GSOUND_LOG_I(LOG_TAG" CHARGER_IN_ACTION", 0);
                    app_gsound_battery_set_info(APP_GSOUND_BATTERY_INFO_LOCAL_IS_CHARGING, TRUE);
                    break;
                }
                case SMCHARGER_CHARGER_OUT_ACTION: {
                    GSOUND_LOG_I(LOG_TAG" CHARGER_OUT_ACTION", 0);
                    app_gsound_battery_set_info(APP_GSOUND_BATTERY_INFO_LOCAL_IS_CHARGING, FALSE);
                    break;
                }
                case SMCHARGER_CASE_BATTERY_REPORT_ACTION: {
                    uint8_t case_percent = (uint8_t)(uint32_t)event_para->data;
                    GSOUND_LOG_I(LOG_TAG" BATTERY_REPORT_ACTION, case_percent=%d", 1, case_percent);
                    app_gsound_battery_set_info(APP_GSOUND_BATTERY_INFO_CASE_PERCENT, (case_percent & 0x7F));
                    app_gsound_battery_set_info(APP_GSOUND_BATTERY_INFO_CASE_IS_CHARGING, (case_percent & 0x80));
                    break;
                }
            }
        }
        break;
    }
    return ret;
}
#endif

static bool app_gsound_proc_battery_group(ui_shell_activity_t *self, uint32_t event_id, void *extra_data, size_t data_len)
{
    switch (event_id) {
        case APPS_EVENTS_BATTERY_PERCENT_CHANGE:
            app_gsound_battery_set_info(APP_GSOUND_BATTERY_INFO_LOCAL_PERCENT, (uint32_t)extra_data);
            break;
    }
    return FALSE;
}

static bool app_gsound_proc_gsound_group(ui_shell_activity_t *self, uint32_t event_id, void *extra_data, size_t data_len)
{
    switch (event_id) {
        case APPS_EVENT_GSOUND_ENABLED: {
#ifdef MTK_AWS_MCE_ENABLE
            bt_status_t bt_status = apps_aws_sync_event_send(EVENT_GROUP_UI_SHELL_GSOUND,
                                                             APPS_EVENT_GSOUND_PARTNER_ENABLE);
            GSOUND_LOG_I(LOG_TAG" GSound AWS send PARTNER_ENABLE, bt_status=0x%08X", 8, bt_status);
#endif
            app_gsound_multi_va_notify_connected();
            app_gsound_srv_set_connected(TRUE);
            break;
        }

        case APPS_EVENT_GSOUND_CONNECTED: {
#ifdef MTK_AWS_MCE_ENABLE
            bt_status_t bt_status = apps_aws_sync_event_send(EVENT_GROUP_UI_SHELL_GSOUND,
                                                             APPS_EVENT_GSOUND_CONNECTED);
            GSOUND_LOG_I(LOG_TAG" GSound AWS send CONNECTED, bt_status=0x%08X", 1, bt_status);
#endif
            app_gsound_multi_va_notify_connected();
            app_gsound_srv_set_connected(TRUE);
            break;
        }

        case APPS_EVENT_GSOUND_DISABLED: {
#ifdef MTK_AWS_MCE_ENABLE
            bt_status_t bt_status = apps_aws_sync_event_send(EVENT_GROUP_UI_SHELL_GSOUND,
                                                             APPS_EVENT_GSOUND_PARTNER_DISABLE);
            GSOUND_LOG_I(LOG_TAG" GSound AWS send PARTNER_DISABLE, bt_status=0x%08X", 1, bt_status);
#endif
            app_gsound_multi_va_notify_disconnected(TRUE);
            app_gsound_srv_set_connected(FALSE);
            break;
        }

        case APPS_EVENT_GSOUND_DISCONNECTED: {
#ifdef MTK_AWS_MCE_ENABLE
            bt_status_t bt_status = apps_aws_sync_event_send(EVENT_GROUP_UI_SHELL_GSOUND,
                                                             APPS_EVENT_GSOUND_DISCONNECTED);
            GSOUND_LOG_I(LOG_TAG" GSound AWS send DISCONNECTED, bt_status=0x%08X", 1, bt_status);
#endif
            app_gsound_multi_va_notify_disconnected(FALSE);
            app_gsound_srv_set_connected(FALSE);
            break;
        }

        default:
            break;
    }
    return TRUE;
}

static bool app_gsound_proc_bt_cm_group(ui_shell_activity_t *self, uint32_t event_id, void *extra_data, size_t data_len)
{

    switch (event_id) {
        case BT_CM_EVENT_REMOTE_INFO_UPDATE: {
#ifdef MTK_AWS_MCE_ENABLE
            bt_cm_remote_info_update_ind_t *remote_update = (bt_cm_remote_info_update_ind_t *)extra_data;
//            uint8_t *addr = (uint8_t *)remote_update->address;
//            APPS_LOG_MSGID_I(LOG_TAG", bt_info_update addr=%02X:%02X:%02X:%02X:%02X:%02X acl=%d->%d srv=0x%04X->0x%04X",
//                             10, addr[0], addr[1], addr[2], addr[3], addr[4], addr[5],
//                             remote_update->pre_acl_state, remote_update->acl_state,
//                             remote_update->pre_connected_service, remote_update->connected_service);

            bt_aws_mce_role_t role = bt_connection_manager_device_local_info_get_aws_role();
            if (!(BT_CM_PROFILE_SERVICE_MASK(BT_CM_PROFILE_SERVICE_AWS) & remote_update->pre_connected_service)
                && (BT_CM_PROFILE_SERVICE_MASK(BT_CM_PROFILE_SERVICE_AWS) & remote_update->connected_service)) {
                bool is_connected = app_gsound_srv_is_connected();
                GSOUND_LOG_I(LOG_TAG" BT_CM, [%2X] AWS Connected is_connected=%d", 2, role, is_connected);
                if (role == BT_AWS_MCE_ROLE_AGENT) {
                    bt_status_t status = BT_STATUS_FAIL;
                    if (is_connected) {
                        status = apps_aws_sync_event_send(EVENT_GROUP_UI_SHELL_GSOUND, APPS_EVENT_GSOUND_CONNECTED);
                    } else {
                        status = apps_aws_sync_event_send(EVENT_GROUP_UI_SHELL_GSOUND, APPS_EVENT_GSOUND_DISCONNECTED);
                    }
                    if (status != BT_STATUS_SUCCESS) {
                        GSOUND_LOG_E(LOG_TAG" BT_CM, [%2X] AWS send fail 0x%08X", 2, role, status);
                    }
                }
            }
#endif
            break;
        }

        default:
            break;
    }
    return FALSE;
}



/**================================================================================*/
/**                                 Idle Activity API                              */
/**================================================================================*/
bool app_gsound_idle_activity_proc(struct _ui_shell_activity *self,
                                   uint32_t event_group,
                                   uint32_t event_id,
                                   void *extra_data,
                                   size_t data_len)
{
    bool ret = FALSE;
    switch (event_group) {
        case EVENT_GROUP_UI_SHELL_SYSTEM: {
            ret = gsound_idle_proc_ui_shell_group(self, event_id, extra_data, data_len);
            break;
        }

        case EVENT_GROUP_UI_SHELL_APP_INTERACTION: {
#ifdef MTK_IN_EAR_FEATURE_ENABLE
            if (event_id == APPS_EVENTS_INTERACTION_IN_EAR_UPDATE_STA) {
                gsound_ohd_set_state(app_gsound_ohd_get_state());
            }
#endif
            break;
        }
#ifdef AIRO_KEY_EVENT_ENABLE
        case EVENT_GROUP_UI_SHELL_KEY: {
            ret = app_gsound_proc_key_group(self, event_id, extra_data, data_len);
            break;
        }
#endif
#ifdef MTK_AWS_MCE_ENABLE
        case EVENT_GROUP_UI_SHELL_AWS_DATA: {
            ret = app_gsound_proc_aws_group(extra_data, data_len);
            break;
        }
#endif

#ifdef AIR_SMART_CHARGER_ENABLE
        case EVENT_GROUP_UI_SHELL_CHARGER_CASE:
            ret = app_gsound_proc_charger_case_group(self, event_id, extra_data, data_len);
            break;
#endif
        case EVENT_GROUP_UI_SHELL_BATTERY: {
            ret = app_gsound_proc_battery_group(self, event_id, extra_data, data_len);
            break;
        }

#ifdef MTK_ANC_ENABLE
        case EVENT_GROUP_UI_SHELL_AUDIO_ANC: {
            if (event_id == AUDIO_ANC_CONTROL_EVENT_ON
                || event_id == AUDIO_ANC_CONTROL_EVENT_OFF) {
                GSOUND_LOG_I(LOG_TAG" ANC update %d", 1, event_id);
                app_gsound_device_action_request_state();
            }
            break;
        }
#endif

        case EVENT_GROUP_UI_SHELL_GSOUND: {
            ret = app_gsound_proc_gsound_group(self, event_id, extra_data, data_len);
            break;
        }

        case EVENT_GROUP_UI_SHELL_TOUCH_KEY: {
            if (event_id == APPS_TOUCH_KEY_STATUS_UPDATE) {
                GSOUND_LOG_I(LOG_TAG" touch_key update", 0);
                app_gsound_device_action_request_state();
            }
            break;
        }

        case EVENT_GROUP_UI_SHELL_BT_CONN_MANAGER: {
            ret = app_gsound_proc_bt_cm_group(self, event_id, extra_data, data_len);
            break;
        }

        default:
            break;
    }
    return ret;
}

#endif /* AIR_GSOUND_ENABLE */
