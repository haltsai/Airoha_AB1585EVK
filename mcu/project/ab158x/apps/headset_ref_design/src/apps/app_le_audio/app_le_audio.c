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

#ifdef AIR_LE_AUDIO_ENABLE

#include "app_le_audio.h"

#include "app_le_audio_aird_client.h"
#include "app_le_audio_adv_mgr.h"
#include "app_le_audio_conn_mgr.h"
#include "app_le_audio_event.h"
#ifdef AIR_LE_AUDIO_MTK_HFP_AT_CMD
#include "app_le_audio_hfp_at_cmd.h"
#endif

#include "bt_type.h"
#include "bt_gap_le_audio.h"
#include "bt_gap_le.h"

#include "bt_sink_srv.h"
#include "bt_sink_srv_le.h"
#include "bt_sink_srv_le_cap_audio_manager.h"

#ifdef MTK_AWS_MCE_ENABLE
#include "bt_aws_mce_report.h"
#include "bt_aws_mce_srv.h"
#include "apps_aws_sync_event.h"
#endif
#include "apps_debug.h"

#include "multi_ble_adv_manager.h"
#include "apps_events_event_group.h"
#include "apps_events_interaction_event.h"
#include "apps_events_bt_event.h"
#include "apps_config_features_dynamic_setting.h"
#include "bt_callback_manager.h"
#include "bt_connection_manager.h"
#include "bt_customer_config.h"
#include "bt_device_manager.h"
#include "bt_init.h"
#include "ui_shell_manager.h"
#include "bt_aws_mce_role_recovery.h"
#include "bt_gattc.h"
#include "bt_le_audio_sink.h"
#include "ble_pbp.h"

#include "nvkey.h"
#include "nvkey_id_list.h"

#ifdef MTK_AWS_MCE_ENABLE
#include "app_mps.h"
#endif

#ifdef APPS_SLEEP_AFTER_NO_CONNECTION
#include "app_power_save_utils.h"
#endif
#ifdef AIR_SMART_CHARGER_ENABLE
#include "app_smcharger.h"
#endif
#ifdef AIR_LE_AUDIO_BIS_ENABLE
#include "app_le_audio_bis.h"
#endif



/**************************************************************************************************
* Define
**************************************************************************************************/
#define LOG_TAG     "[LEA][APP]"

/* LE Audio feature state */
#define APP_LE_AUDIO_FEATURE_DISABLE    0
#define APP_LE_AUDIO_FEATURE_ENABLE     1

/**************************************************************************************************
 * Enum
**************************************************************************************************/
#ifdef AIR_LE_AUDIO_HEADSET_ENABLE
typedef enum {
    APP_LE_AUDIO_WIRED_LINE_IN = 0,
    APP_LE_AUDIO_WIRED_USB_IN,
} app_le_audio_wired_state_t;

#define APP_LE_AUDIO_WIRED_MASK(wired_type)    (0x01U << (wired_type))

static uint8_t app_le_audio_wired_state = 0x00;
#endif

/**************************************************************************************************
 * Variable
**************************************************************************************************/
#ifdef AIR_LE_AUDIO_CIS_ENABLE
static bool lea_local_cis_connected = FALSE;
#ifdef MTK_AWS_MCE_ENABLE
static bool lea_peer_cis_connected = FALSE;
#endif
#endif

static uint8_t app_lea_feature_state = APP_LE_AUDIO_FEATURE_ENABLE;

/**************************************************************************************************
 * Prototype
**************************************************************************************************/
extern void app_le_audio_dhss_sdp_init(void);
extern void bt_app_common_sync_le_audio_info(void);

/**************************************************************************************************
 * Static Functions
**************************************************************************************************/
#ifdef MTK_AWS_MCE_ENABLE
static void app_le_audio_send_battery_level_notification(void)
{
    for (int i = 0; i < APP_LE_AUDIO_MAX_LINK_NUM; i++) {
        bt_handle_t handle = app_lea_conn_mgr_get_handle(i);
        if (handle != BT_HANDLE_INVALID) {
            app_le_audio_mps_send_battery(handle);
        }
    }
}
#endif

static bt_status_t app_le_audio_set_sniff(bool enable)
{
    bt_bd_addr_t aws_device;
    bt_status_t status = BT_STATUS_SUCCESS;
    uint32_t num = bt_cm_get_connected_devices(BT_CM_PROFILE_SERVICE_MASK(BT_CM_PROFILE_SERVICE_AWS), &aws_device, 1);

    if (num > 0) {
        bt_gap_connection_handle_t gap_handle = bt_cm_get_gap_handle(aws_device);
        bt_gap_link_policy_setting_t setting;
        APPS_LOG_MSGID_I(LOG_TAG" set_sniff, enable=%d handle=0x%08X", 2, enable, gap_handle);
        if (!enable) {
            status = bt_gap_exit_sniff_mode(gap_handle);
        }

        setting.sniff_mode = enable ? BT_GAP_LINK_POLICY_ENABLE : BT_GAP_LINK_POLICY_DISABLE;
        status = bt_gap_write_link_policy(gap_handle, &setting);
    }
    return status;
}

static bt_status_t app_le_audio_event_callback(bt_msg_type_t msg, bt_status_t status, void *buff)
{
    switch (msg) {
        case BT_GAP_LE_CIS_ESTABLISHED_IND: {
            bt_gap_le_cis_established_ind_t *ind = (bt_gap_le_cis_established_ind_t *) buff;
            APPS_LOG_MSGID_I(LOG_TAG" BT_GAP_LE_CIS_ESTABLISHED_IND, handle=0x%04X", 1, ind->connection_handle);
            //app_le_audio_set_sniff(FALSE);
            break;
        }

        case BT_GAP_LE_CIS_TERMINATED_IND: {
            bt_gap_le_cis_terminated_ind_t *ind = (bt_gap_le_cis_terminated_ind_t *) buff;
            APPS_LOG_MSGID_I(LOG_TAG" BT_GAP_LE_CIS_TERMINATED_IND, handle=0x%04X", 1, ind->connection_handle);
            app_le_audio_set_sniff(TRUE);
            break;
        }

        case BT_GAP_LE_EXT_ADVERTISING_REPORT_IND: {

            if (status == BT_STATUS_SUCCESS)
            {
                bt_gap_le_ext_advertising_report_ind_t *ind = (bt_gap_le_ext_advertising_report_ind_t *)buff;
                uint16_t total_len = ind->data_length;
                uint8_t *data = ind->data;
                uint16_t uuid;
                uint16_t idx = 0;

                while (idx < total_len)
                {
                    if (data[idx] >= 3 && data[idx + 1] == BT_GAP_LE_AD_TYPE_SERVICE_DATA)
                    {
                        uuid = (uint16_t)data[idx + 3] << 8 | data[idx + 2];

                        if (uuid == BT_PBP_UUID16_PUBLIC_BROADCAST_ANNOUNCEMENTS)
                        {
                           APPS_LOG_MSGID_I(LOG_TAG"[PBP] PUBLIC_BROADCAST_ANNOUNCEMENTS find UUID = %x SID = %x type:%d, addr:%02X:%02X:%02X:%02X:%02X:%02X feature = %x", 10,
                            uuid, ind->advertising_SID, ind->address.type, ind->address.addr[5], ind->address.addr[4], ind->address.addr[3],
                            ind->address.addr[2], ind->address.addr[1], ind->address.addr[0], data[idx + 4]);
                        }
                    }
                    else if (data[idx + 1] == BT_PBP_BROADCAST_NAME)
                    {
                        uint8_t val = data[idx + data[idx] + 1];

                        data[idx + data[idx] + 1] = 0;
                        APPS_LOG_I(LOG_TAG"[PBP] BROADCAST_NAME = [%s]", &data[idx + 2]);
                        data[idx + data[idx] + 1] = val;
                    }

                    idx += (data[idx] + 1);
                }
             }

            break;
        }

        case BT_GATTC_DISCOVER_PRIMARY_SERVICE:
        case BT_GATTC_DISCOVER_PRIMARY_SERVICE_BY_UUID:
        case BT_GATTC_FIND_INCLUDED_SERVICES:
        case BT_GATTC_DISCOVER_CHARC:
        case BT_GATTC_DISCOVER_CHARC_DESCRIPTOR:
        case BT_GATTC_READ_CHARC:
        case BT_GATTC_READ_LONG_CHARC:
        case BT_GATTC_READ_USING_CHARC_UUID:
        case BT_GATTC_READ_MULTI_CHARC_VALUES:
        case BT_GATTC_WRITE_CHARC:
        case BT_GATTC_WRITE_LONG_CHARC:
        case BT_GATTC_RELIABLE_WRITE_CHARC:
        case BT_GATTC_CHARC_VALUE_NOTIFICATION: {
            app_le_audio_aird_client_event_handler(msg, status, buff);
            break;
        }
        default:
            break;
    }
    return BT_STATUS_SUCCESS;
}

static bool app_le_audio_proc_bt_cm_group(struct _ui_shell_activity *self,
                                          uint32_t event_id,
                                          void *extra_data,
                                          size_t data_len)
{
    switch (event_id) {
        case BT_CM_EVENT_REMOTE_INFO_UPDATE: {
            bt_cm_remote_info_update_ind_t *remote_update = (bt_cm_remote_info_update_ind_t *)extra_data;
            if (NULL == remote_update) {
                break;
            }

#if defined(AIR_LE_AUDIO_MTK_HFP_AT_CMD) || defined(MTK_AWS_MCE_ENABLE)
            bt_aws_mce_role_t role = bt_connection_manager_device_local_info_get_aws_role();
#endif

#ifdef AIR_LE_AUDIO_MTK_HFP_AT_CMD
            if ((BT_CM_PROFILE_SERVICE_MASK(BT_CM_PROFILE_SERVICE_HFP) & remote_update->pre_connected_service)
                != (BT_CM_PROFILE_SERVICE_MASK(BT_CM_PROFILE_SERVICE_HFP) & remote_update->connected_service)) {
                APPS_LOG_MSGID_I(LOG_TAG"[HF] BT_CM event, [%02X] HFP service=0x%08X", 2, role, remote_update->connected_service);
                if (role == BT_AWS_MCE_ROLE_AGENT) {
                    if ((BT_CM_PROFILE_SERVICE_MASK(BT_CM_PROFILE_SERVICE_HFP) & remote_update->connected_service)) {
                        app_le_audio_hfp_start_battery_timer(TRUE);
                        app_le_audio_hfp_send_battery_info();
                    } else {
                        app_le_audio_hfp_start_battery_timer(FALSE);
                    }
                }
            }
#endif

#ifdef MTK_AWS_MCE_ENABLE
            if (!(BT_CM_PROFILE_SERVICE_MASK(BT_CM_PROFILE_SERVICE_AWS) & remote_update->pre_connected_service) &&
                (BT_CM_PROFILE_SERVICE_MASK(BT_CM_PROFILE_SERVICE_AWS) & remote_update->connected_service)) {
                APPS_LOG_MSGID_I(LOG_TAG" BT_CM event, [%02X] AWS Connected reason=0x%02X", 2, role, remote_update->reason);
                app_le_audio_mps_set_battery(APP_MPS_CHANNEL_PEER, APP_MPS_BATTERY_LEVEL_VALUE_MAX);
                app_le_audio_send_battery_level_notification();

#ifdef AIR_LE_AUDIO_CIS_ENABLE
                bt_status_t bt_status = apps_aws_sync_event_send_extra(EVENT_GROUP_UI_SHELL_LE_AUDIO,
                                                                       EVENT_ID_LE_AUDIO_CIS_SYNC_STATE,
                                                                       &lea_local_cis_connected,
                                                                       sizeof(uint8_t));
                APPS_LOG_MSGID_I(LOG_TAG" BT_CM event, AWS Connected, sync_cis_connected=%d bt_status=0x%08X",
                                 2, lea_local_cis_connected, bt_status);
#endif
            } else if ((BT_CM_PROFILE_SERVICE_MASK(BT_CM_PROFILE_SERVICE_AWS) & remote_update->pre_connected_service)
                       && !(BT_CM_PROFILE_SERVICE_MASK(BT_CM_PROFILE_SERVICE_AWS) & remote_update->connected_service)) {
                APPS_LOG_MSGID_I(LOG_TAG" BT_CM event, [%02X] AWS Disconnected reason=0x%02X", 2, role, remote_update->reason);
                if (remote_update->reason != BT_HCI_STATUS_ROLE_SWITCH_PENDING && remote_update->reason != BT_HCI_STATUS_SUCCESS) {
                    app_le_audio_mps_set_battery(APP_MPS_CHANNEL_PEER, APP_MPS_BATTERY_LEVEL_VALUE_MIN);
                    app_le_audio_send_battery_level_notification();
                }
#ifdef AIR_LE_AUDIO_CIS_ENABLE
                lea_peer_cis_connected = FALSE;
#ifdef APPS_SLEEP_AFTER_NO_CONNECTION
                app_power_save_utils_notify_mode_changed(FALSE, NULL);
#endif
#endif
            }
#endif
            break;
        }
    }
    return FALSE;
}

static bool app_le_audio_proc_bt_dm_group(struct _ui_shell_activity *self,
                                          uint32_t event_id,
                                          void *extra_data,
                                          size_t data_len)
{
    bt_device_manager_power_event_t evt;
    bt_device_manager_power_status_t status;
    bt_event_get_bt_dm_event_and_status(event_id, &evt, &status);
    switch (evt) {
        case BT_DEVICE_MANAGER_POWER_EVT_ACTIVE_COMPLETE:
            if (BT_DEVICE_MANAGER_POWER_STATUS_SUCCESS == status) {
                bt_sink_srv_cap_am_init();
#ifdef AIR_LE_AUDIO_MTK_HFP_AT_CMD
                app_le_audio_hfp_at_cmd_register(TRUE);
#endif
            }
            break;
        case BT_DEVICE_MANAGER_POWER_EVT_STANDBY_COMPLETE:
            if (BT_DEVICE_MANAGER_POWER_RESET_TYPE_NORMAL == status) {
                bt_sink_srv_cap_am_deinit();
#ifdef AIR_LE_AUDIO_MTK_HFP_AT_CMD
                app_le_audio_hfp_at_cmd_register(FALSE);
#endif
            }
            break;
        default:
            break;
    }
    return FALSE;
}

static bool app_le_audio_proc_bt_sink_group(struct _ui_shell_activity *self,
                                            uint32_t event_id,
                                            void *extra_data,
                                            size_t data_len)
{
    switch (event_id) {
        case LE_SINK_SRV_EVENT_REMOTE_INFO_UPDATE: {
#ifdef AIR_LE_AUDIO_CIS_ENABLE
            bt_le_sink_srv_event_remote_info_update_t *update_ind = (bt_le_sink_srv_event_remote_info_update_t *)extra_data;
            if (update_ind == NULL) {
                break;
            }
            bool old_local_cis_connected = lea_local_cis_connected;
            if (update_ind->pre_state == BT_BLE_LINK_DISCONNECTED
                && update_ind->state == BT_BLE_LINK_CONNECTED) {
                lea_local_cis_connected = TRUE;
            } else if (update_ind->pre_state == BT_BLE_LINK_CONNECTED
                       && update_ind->state == BT_BLE_LINK_DISCONNECTED) {
#ifdef AIR_LE_AUDIO_MULTIPOINT_ENABLE
                bt_handle_t le_handle = bt_sink_srv_cap_check_links_state(BT_SINK_SRV_CAP_STATE_CONNECTED);
                APPS_LOG_MSGID_I(LOG_TAG"[Power_Saving] LEA_BT_SINK, Disconnect remain le_handle=0x%04X", 1, le_handle);
                if (BT_HANDLE_INVALID == le_handle) {
                    lea_local_cis_connected = FALSE;
                }
#else
                lea_local_cis_connected = FALSE;
#endif
            }

            APPS_LOG_MSGID_I(LOG_TAG"[Power_Saving] LEA_BT_SINK, lea_local_cis_connected=%d->%d",
                             2, old_local_cis_connected, lea_local_cis_connected);
            if (old_local_cis_connected != lea_local_cis_connected) {
#ifdef APPS_SLEEP_AFTER_NO_CONNECTION
                app_power_save_utils_notify_mode_changed(FALSE, NULL);
#endif
#ifdef MTK_AWS_MCE_ENABLE
                bt_aws_mce_srv_link_type_t aws_link_type = bt_aws_mce_srv_get_link_type();
                bt_status_t bt_status = BT_STATUS_SUCCESS;
                if (aws_link_type != BT_AWS_MCE_SRV_LINK_NONE) {
                    bt_status = apps_aws_sync_event_send_extra(EVENT_GROUP_UI_SHELL_LE_AUDIO,
                                                               EVENT_ID_LE_AUDIO_CIS_SYNC_STATE,
                                                               &lea_local_cis_connected,
                                                               sizeof(uint8_t));
                    APPS_LOG_MSGID_I(LOG_TAG"[Power_Saving] LEA_BT_SINK, sync_cis_connected=%d bt_status=0x%08X",
                                     2, lea_local_cis_connected, bt_status);
                }
#endif
#endif
            } else if (lea_local_cis_connected) {
#ifdef APPS_SLEEP_AFTER_NO_CONNECTION
                /* To notify power save refresh time. */
                app_power_save_utils_refresh_waiting_time();
#endif
            }
            break;
        }
    }
    return FALSE;
}

static bool app_le_audio_proc_bt_group(struct _ui_shell_activity *self,
                                       uint32_t event_id,
                                       void *extra_data,
                                       size_t data_len)
{
    apps_bt_event_data_t *bt_event_data = (apps_bt_event_data_t *)extra_data;
    if (bt_event_data == NULL) {
        APPS_LOG_MSGID_E(LOG_TAG" BT event, bt_event_data is NULL", 0);
        return FALSE;
    }
    switch (event_id) {
        case BT_POWER_ON_CNF: {
            /* Restart LE_Audio ADV if ever_connected */
            if (app_lea_conn_mgr_is_ever_connected()) {
#if (APP_LE_AUDIO_ADV_CONN_MODE == APP_LE_AUDIO_MODE_DEFAULT)
                app_le_audio_start_advertising(TRUE, FALSE, 0);
                APPS_LOG_MSGID_I(LOG_TAG" BT_POWER_ON_CNF, (DEFAULT) restart LEA ADV due to ever_connected", 0);
#elif (APP_LE_AUDIO_ADV_CONN_MODE == APP_LE_AUDIO_MODE_DONGLE_ONE)
                app_lea_bond_info_t *bond_info = app_lea_conn_mgr_get_bond_info();
                uint8_t empty_addr[BT_BD_ADDR_LEN] = {0};
                if (bond_info != NULL && bond_info[0].used
                    && memcmp(bond_info->bond_addr, empty_addr, BT_BD_ADDR_LEN) != 0) {
                    app_lea_adv_mgr_update_target_addr(APP_LEA_TARGET_SET_UNIQUE_ADDR, bond_info[0].addr_type, bond_info->bond_addr);
                    app_le_audio_start_advertising(FALSE, FALSE, APP_LE_AUDIO_DONGLE_ONE_ADV_TIME);
                    APPS_LOG_MSGID_I(LOG_TAG" BT_POWER_ON_CNF, (DONGLE_ONE) restart LEA ADV when ever_connected", 0);
                } else {
                    APPS_LOG_MSGID_E(LOG_TAG" BT_POWER_ON_CNF, (DONGLE_ONE) restart LEA ADV fail - ever_connected but bond_info error", 0);
                }
#endif
            } else {
#if (APP_LE_AUDIO_ADV_CONN_MODE == APP_LE_AUDIO_MODE_DONGLE_ONE)
                app_le_audio_start_advertising(TRUE, FALSE, APP_LE_AUDIO_DONGLE_ONE_ADV_TIME);
                APPS_LOG_MSGID_I(LOG_TAG" BT_POWER_ON_CNF, (DONGLE_ONE) start general adv", 0);
#else
                APPS_LOG_MSGID_I(LOG_TAG" BT_POWER_ON_CNF, no ever_connected", 0);
#endif
            }
            break;
        }
    }
    return FALSE;
}

#ifdef MTK_AWS_MCE_ENABLE
static bool app_le_audio_proc_aws_data_group(struct _ui_shell_activity *self,
                                             uint32_t event_id,
                                             void *extra_data,
                                             size_t data_len)
{
    uint32_t aws_event_group;
    uint32_t aws_event_id;
    void *p_extra_data = NULL;
    uint32_t extra_data_len = 0;
    bt_aws_mce_report_info_t *aws_data_ind = (bt_aws_mce_report_info_t *)extra_data;

    if (!aws_data_ind || aws_data_ind->module_id != BT_AWS_MCE_REPORT_MODULE_APP_ACTION) {
        return FALSE;
    }

    apps_aws_sync_event_decode_extra(aws_data_ind, &aws_event_group, &aws_event_id,
                                     &p_extra_data, &extra_data_len);
    if (aws_event_group == EVENT_GROUP_UI_SHELL_LE_AUDIO
        && aws_event_id == EVENT_ID_LE_AUDIO_CIS_SYNC_STATE) {
#ifdef AIR_LE_AUDIO_CIS_ENABLE
        bool peer_cis_connected = *((bool *)p_extra_data);
        APPS_LOG_MSGID_I(LOG_TAG"[Power_Saving] AWS_DATA, peer_cis_connected=%d->%d",
                         2, lea_peer_cis_connected, peer_cis_connected);
        if (peer_cis_connected != lea_peer_cis_connected) {
            lea_peer_cis_connected = peer_cis_connected;
#ifdef APPS_SLEEP_AFTER_NO_CONNECTION
            app_power_save_utils_notify_mode_changed(FALSE, NULL);
#endif
        }
#endif
    }
    return FALSE;
}
#endif

#ifdef AIR_SMART_CHARGER_ENABLE
static bool app_le_audio_proc_smcharger_group(struct _ui_shell_activity *self,
                                              uint32_t event_id,
                                              void *extra_data,
                                              size_t data_len)
{
    switch (event_id) {
        case SMCHARGER_EVENT_NOTIFY_BOTH_CHANGED: {
#ifdef AIR_LE_AUDIO_MTK_HFP_AT_CMD
            bt_aws_mce_role_t role = bt_connection_manager_device_local_info_get_aws_role();
            if (role == BT_AWS_MCE_ROLE_AGENT) {
                app_le_audio_hfp_send_battery_info();
            }
#endif
            break;
        }

        case SMCHARGER_EVENT_NOTIFY_ACTION: {
#if (APP_LE_AUDIO_ADV_CONN_MODE == APP_LE_AUDIO_MODE_DONGLE_ONE)
            app_smcharger_public_event_para_t *event_para = (app_smcharger_public_event_para_t *)extra_data;
            if (event_para->action != SMCHARGER_OPEN_LID_ACTION) {
                break;
            } else if (bt_cm_power_get_state() != BT_CM_POWER_STATE_ON) {
                // Ignore OPEN_LID_ACTION, start ADV in BT_POWER_ON_CNF
                APPS_LOG_MSGID_I(LOG_TAG" OPEN_LID_ACTION, BT not ON", 0);
                break;
            }

            if (app_lea_conn_mgr_is_ever_connected()) {
                app_lea_bond_info_t *bond_info = app_lea_conn_mgr_get_bond_info();
                uint8_t empty_addr[BT_BD_ADDR_LEN] = {0};
                if (bond_info != NULL && bond_info[0].used
                    && memcmp(bond_info->bond_addr, empty_addr, BT_BD_ADDR_LEN) != 0) {
                    app_lea_adv_mgr_update_target_addr(APP_LEA_TARGET_SET_UNIQUE_ADDR, bond_info[0].addr_type, bond_info->bond_addr);
                    app_le_audio_start_advertising(FALSE, FALSE, APP_LE_AUDIO_DONGLE_ONE_ADV_TIME);
                    APPS_LOG_MSGID_I(LOG_TAG" OPEN_LID_ACTION, (DONGLE_ONE) restart LEA ADV when ever_connected", 0);
                } else {
                    APPS_LOG_MSGID_E(LOG_TAG" OPEN_LID_ACTION, (DONGLE_ONE) restart LEA ADV fail - ever_connected but bond_info error", 0);
                }
            } else {
                app_le_audio_start_advertising(TRUE, FALSE, APP_LE_AUDIO_DONGLE_ONE_ADV_TIME);
                APPS_LOG_MSGID_I(LOG_TAG" OPEN_LID_ACTION, (DONGLE_ONE) start general adv", 0);
            }
#endif
            break;
        }
        default:
            break;
    }
    return FALSE;
}
#endif

static bool app_le_audio_proc_lea_group(struct _ui_shell_activity *self,
                                        uint32_t event_id,
                                        void *extra_data,
                                        size_t data_len)
{
    switch (event_id) {
#ifdef AIR_LE_AUDIO_MTK_HFP_AT_CMD
        case EVENT_ID_LE_AUDIO_NOTIFY_BATTERY: {
            app_le_audio_hfp_send_battery_info();
            break;
        }
#endif
        default:
            break;
    }
    return TRUE;
}

void app_le_audio_connection_callback(bool conn_or_disconnect, int index, uint8_t addr_type, const uint8_t *addr, bt_hci_disconnect_reason_t reason)
{
    bt_aws_mce_role_t role = bt_connection_manager_device_local_info_get_aws_role();
    if (conn_or_disconnect) {
        APPS_LOG_MSGID_I(LOG_TAG" connection_callback, connected [%02X] index=%d", 2, role, index);
    } else {
        APPS_LOG_MSGID_I(LOG_TAG" connection_callback, disconnected [%02X] index=%d addr=%d-%02X:%02X:%02X:%02X:%02X:%02X reason=0x%02X",
                         10, role, index, addr_type, addr[0], addr[1], addr[2], addr[3], addr[4], addr[5], reason);
    }

    if (conn_or_disconnect) {
        uint8_t cur_conn_num = app_lea_conn_mgr_get_conn_num();
        uint8_t support_max_conn_num = app_lea_conn_mgr_get_support_max_conn_num();
        if (cur_conn_num == support_max_conn_num) {
            APPS_LOG_MSGID_I(LOG_TAG" connection_callback, [%02X] stop ADV when LEA MAX LINK %d",
                             2, role, support_max_conn_num);
            app_le_audio_stop_advertising(FALSE);
        }
    } else {
#if (APP_LE_AUDIO_ADV_CONN_MODE == APP_LE_AUDIO_MODE_DEFAULT)
        app_le_audio_start_advertising(TRUE, FALSE, 0);
#elif (APP_LE_AUDIO_ADV_CONN_MODE == APP_LE_AUDIO_MODE_DONGLE_ONE)
        if (reason != BT_HCI_STATUS_CONNECTION_TERMINATED_BY_LOCAL_HOST
            && reason != BT_HCI_STATUS_LIMIT_REACHED) {
            app_lea_adv_mgr_update_target_addr(APP_LEA_TARGET_SET_UNIQUE_ADDR, addr_type, addr);
            app_le_audio_start_advertising(FALSE, FALSE, 0);
        }
#elif (APP_LE_AUDIO_ADV_CONN_MODE == APP_LE_AUDIO_MODE_PHONE_SWITCH)
        bool exist_connected_edr = app_bt_conn_manager_exist_connected_edr(addr);
        if (reason == BT_HCI_STATUS_CONNECTION_TERMINATED_BY_LOCAL_HOST
            || reason == BT_HCI_STATUS_LIMIT_REACHED) {
            APPS_LOG_MSGID_I(LOG_TAG" disconnect_callback, disconnect difference LEA successfully", 0);
            ui_shell_remove_event(EVENT_GROUP_UI_SHELL_LE_AUDIO, EVENT_ID_LE_AUDIO_SYNC_ADV_ACTION);
            ui_shell_send_event(FALSE, EVENT_PRIORITY_HIGH,
                                EVENT_GROUP_UI_SHELL_LE_AUDIO, EVENT_ID_LE_AUDIO_SYNC_ADV_ACTION,
                                NULL, 0, NULL, 0);
        } else if (!exist_connected_edr) {
            app_lea_adv_mgr_update_target_addr(APP_LEA_TARGET_SET_UNIQUE_ADDR, addr_type, addr);
            app_le_audio_start_advertising(FALSE, FALSE, 0);
        }
#endif
        app_le_audio_aird_client_reset_info(index);
    }
}



/**************************************************************************************************
 * Public function
**************************************************************************************************/
#if 0
void bt_sink_srv_le_media_state_change_callback(uint16_t event_id, bt_handle_t handle, bool is_resume)
{
    // Use BT_SINK_SRV LE Audio Callback directly, not switch to APP task for low latency
    APPS_LOG_MSGID_I(LOG_TAG" le_media_state_change_callback, event_id=0x%04X handle=0x%04X is_resume=%d",
                     3, event_id, handle, is_resume);
    if (event_id == BT_LE_AUDIO_SINK_EVENT_MEDIA_SUSPEND) {
        app_le_audio_aird_client_notify_dongle_media_state(TRUE, handle, NULL, 0);
    } else if (event_id == BT_LE_AUDIO_SINK_EVENT_MEDIA_RESUME) {
        app_le_audio_aird_client_notify_dongle_media_state(FALSE, handle, NULL, 0);
    }
}
#else
void bt_sink_srv_edr_state_change_callback(bt_sink_srv_state_t previous, bt_sink_srv_state_t now)
{
    APPS_LOG_MSGID_I(LOG_TAG" edr_state_change, bt_sink_state %04X->%04X", 2, previous, now);
    if (((!(previous & BT_SINK_SRV_STATE_INCOMING)) && (now & BT_SINK_SRV_STATE_INCOMING)) ||
        ((!(previous & BT_SINK_SRV_STATE_OUTGOING)) && (now & BT_SINK_SRV_STATE_OUTGOING)) ||
        //((previous & BT_SINK_SRV_STATE_HELD_REMAINING) && (!(now & BT_SINK_SRV_STATE_HELD_REMAINING))) ||
        ((!(previous & BT_SINK_SRV_STATE_ACTIVE)) && (now & BT_SINK_SRV_STATE_ACTIVE)) ||
        ((!(previous & BT_SINK_SRV_STATE_STREAMING)) && (now & BT_SINK_SRV_STATE_STREAMING))) {
        /* HFP incoming call */
        /* HFP outgoing call */
        /* HFP cal unheld */
        /* HFP call active */
        /* A2DP Streaming start */
        ui_shell_remove_event(EVENT_GROUP_UI_SHELL_LE_AUDIO, EVENT_ID_LE_AUDIO_RESET_DONGLE_BUSY_EVENT);
        app_le_audio_aird_client_notify_dongle_media_state(TRUE, NULL, 0);
    } else if (((previous & BT_SINK_SRV_STATE_INCOMING) && (!(now & BT_SINK_SRV_STATE_INCOMING))) ||
               ((previous & BT_SINK_SRV_STATE_OUTGOING) && (!(now & BT_SINK_SRV_STATE_OUTGOING))) ||
               //((!(previous & BT_SINK_SRV_STATE_HELD_REMAINING)) && (now & BT_SINK_SRV_STATE_HELD_REMAINING)) ||
               ((previous & BT_SINK_SRV_STATE_ACTIVE) && (!(now & BT_SINK_SRV_STATE_ACTIVE))) ||
               ((previous & BT_SINK_SRV_STATE_STREAMING) && (!(now & BT_SINK_SRV_STATE_STREAMING)))) {
        /* HFP reject call (incoming) */
        /* HFP reject call (outgoing) */
        /* HFP cal held */
        /* HFP call end */
        /* A2DP Streaming stop */
        ui_shell_remove_event(EVENT_GROUP_UI_SHELL_LE_AUDIO, EVENT_ID_LE_AUDIO_RESET_DONGLE_BUSY_EVENT);
        ui_shell_send_event(FALSE, EVENT_PRIORITY_HIGH,
                            EVENT_GROUP_UI_SHELL_LE_AUDIO, EVENT_ID_LE_AUDIO_RESET_DONGLE_BUSY_EVENT,
                            NULL, 0, NULL, APP_LE_AUDIO_RESET_DEVICE_BUSY_TIMEOUT);
    }
}
#endif

#ifdef AIR_LE_AUDIO_HEADSET_ENABLE
bool app_le_audio_is_wired_audio(void)
{
    bool ret = FALSE;
    if ((app_le_audio_wired_state & APP_LE_AUDIO_WIRED_MASK(APP_LE_AUDIO_WIRED_LINE_IN))
        || (app_le_audio_wired_state & APP_LE_AUDIO_WIRED_MASK(APP_LE_AUDIO_WIRED_USB_IN))) {
        ret = TRUE;
    }
    return ret;
}

static bool app_le_audio_proc_interaction_group(ui_shell_activity_t *self,
                                                uint32_t event_id,
                                                void *extra_data,
                                                size_t data_len)
{
    switch (event_id) {
#ifdef APPS_LINE_IN_SUPPORT
        case APPS_EVENTS_INTERACTION_LINE_IN_PLUG_STATE: {
            bool plug_in = (bool)extra_data;
            bool local_line_in_state = (app_le_audio_wired_state & APP_LE_AUDIO_WIRED_MASK(APP_LE_AUDIO_WIRED_LINE_IN));
            APPS_LOG_MSGID_I(LOG_TAG" interaction event, line_in=%d->%d", 2, local_line_in_state, plug_in);
            if (plug_in != local_line_in_state) {
                if (plug_in) {
                    app_le_audio_wired_state |= APP_LE_AUDIO_WIRED_MASK(APP_LE_AUDIO_WIRED_LINE_IN);
                    app_le_audio_stop_advertising(FALSE);
                    app_lea_conn_mgr_disconnect_dongle();
                } else {
                    app_le_audio_wired_state &= ~APP_LE_AUDIO_WIRED_MASK(APP_LE_AUDIO_WIRED_LINE_IN);
                    if (app_le_audio_wired_state == 0) {
                        app_le_audio_start_advertising(TRUE, FALSE, 0);
                    }
                }
            }
            break;
        }
#endif
#ifdef APPS_USB_AUDIO_SUPPORT
        case APPS_EVENTS_INTERACTION_USB_PLUG_STATE: {
            bool plug_in = (bool)extra_data;
            bool local_usb_in_state = (app_le_audio_wired_state & APP_LE_AUDIO_WIRED_MASK(APP_LE_AUDIO_WIRED_USB_IN));
            APPS_LOG_MSGID_I(LOG_TAG" interaction event, usb_in=%d->%d", 2, local_usb_in_state, plug_in);
            if (plug_in != local_usb_in_state) {
                if (plug_in) {
                    app_le_audio_wired_state |= APP_LE_AUDIO_WIRED_MASK(APP_LE_AUDIO_WIRED_USB_IN);
                    app_le_audio_stop_advertising(FALSE);
                    app_lea_conn_mgr_disconnect_dongle();
                } else {
                    app_le_audio_wired_state &= ~APP_LE_AUDIO_WIRED_MASK(APP_LE_AUDIO_WIRED_USB_IN);
                    if (app_le_audio_wired_state == 0) {
                        app_le_audio_start_advertising(TRUE, FALSE, 0);
                    }
                }
            }
            break;
        }
#endif
        default:
            break;
    }

    return FALSE;
}
#endif

bool app_le_audio_idle_activity_proc(struct _ui_shell_activity *self, uint32_t event_group, uint32_t event_id, void *extra_data, size_t data_len)
{
    bool ret = FALSE;
    switch (event_group) {
        case EVENT_GROUP_UI_SHELL_BT_CONN_MANAGER: {
            ret = app_le_audio_proc_bt_cm_group(self, event_id, extra_data, data_len);
            break;
        }
        case EVENT_GROUP_UI_SHELL_BT_SINK: {
            ret = app_le_audio_proc_bt_sink_group(self, event_id, extra_data, data_len);
            break;
        }
        case EVENT_GROUP_UI_SHELL_BT: {
            ret = app_le_audio_proc_bt_group(self, event_id, extra_data, data_len);
            break;
        }
#ifdef MTK_AWS_MCE_ENABLE
        /* UI Shell BT AWS_DATA events. */
        case EVENT_GROUP_UI_SHELL_AWS_DATA:
            ret = app_le_audio_proc_aws_data_group(self, event_id, extra_data, data_len);
            break;
#endif
#ifdef AIR_SMART_CHARGER_ENABLE
        /* APP SmartCharger events. */
        case EVENT_GROUP_UI_SHELL_CHARGER_CASE:
            ret = app_le_audio_proc_smcharger_group(self, event_id, extra_data, data_len);
            break;
#endif
        case EVENT_GROUP_UI_SHELL_LE_AUDIO: {
            ret = app_le_audio_proc_lea_group(self, event_id, extra_data, data_len);
            break;
        }
        case EVENT_GROUP_UI_SHELL_BT_DEVICE_MANAGER: {
            ret = app_le_audio_proc_bt_dm_group(self, event_id, extra_data, data_len);
            break;
        }
#ifdef AIR_LE_AUDIO_HEADSET_ENABLE
        case EVENT_GROUP_UI_SHELL_APP_INTERACTION: {
            ret = app_le_audio_proc_interaction_group(self, event_id, extra_data, data_len);
            break;
        }
#endif
        default:
            break;
    }

    app_lea_conn_mgr_proc_ui_shell_event(event_group, event_id, extra_data, data_len);
    app_lea_adv_mgr_proc_ui_shell_event(event_group, event_id, extra_data, data_len);
    app_le_audio_aird_client_proc_ui_shell_event(event_group, event_id, extra_data, data_len);
#ifdef AIR_LE_AUDIO_BIS_ENABLE
    app_le_audio_bis_proc_ui_shell_event(self, event_group, event_id, extra_data, data_len);
#endif
    return ret;
}

void app_le_audio_enable(bool enable)
{
    if (app_lea_feature_state == enable) {
        APPS_LOG_MSGID_I(LOG_TAG" app_le_audio_enable, same enable %d", 1, enable);
        return;
    }

    APPS_LOG_MSGID_I(LOG_TAG" app_le_audio_enable, enable=%d", 1, enable);
}

bool app_le_audio_is_enabled(void)
{
    return (app_lea_feature_state == APP_LE_AUDIO_FEATURE_ENABLE);
}

void app_le_audio_reset(void)
{
    APPS_LOG_MSGID_I(LOG_TAG" reset", 0);
#if (APP_LE_AUDIO_ADV_CONN_MODE == APP_LE_AUDIO_MODE_DONGLE_ONE)
    app_le_audio_stop_advertising(FALSE);
    app_lea_conn_mgr_disconnect(NULL, BT_HCI_STATUS_CONNECTION_TERMINATED_BY_LOCAL_HOST);
    app_lea_conn_mgr_reset_bond_info();
    app_le_audio_start_advertising(TRUE, FALSE, 0);
#endif
}

uint8_t app_le_audio_get_feature_state(void)
{
    return app_lea_feature_state;
}

void app_le_audio_enable_multi_conn(bool enable)
{
    app_lea_conn_mgr_enable_multi_conn(enable);
}

void app_le_audio_start_advertising(bool general, bool sync, uint32_t timeout)
{
    app_lea_adv_mgr_start_advertising((general ? APP_LEA_ADV_MODE_UNDIRECT : APP_LEA_ADV_MODE_DIRECT),
                                      sync, timeout);
}

void app_le_audio_stop_advertising(bool sync)
{
    app_lea_adv_mgr_stop_advertising(sync);
}

void app_le_audio_update_adv_interval(uint16_t interval_min, uint16_t interval_max)
{
    app_lea_adv_mgr_update_adv_interval(interval_min, interval_max);
}

void app_le_audio_init(void)
{
    APPS_LOG_MSGID_I(LOG_TAG" app_le_audio_init", 0);
#if defined(AIR_MULTI_POINT_ENABLE)
    // config in app_bt_emp_service
#else
    app_lea_conn_mgr_enable_multi_conn(FALSE);
#endif

#ifdef MTK_AWS_MCE_ENABLE
    app_le_audio_dhss_sdp_init();
#endif

    le_sink_srv_init(APP_LE_AUDIO_MAX_LINK_NUM);
#ifdef AIR_LE_AUDIO_HEADSET_ENABLE
    le_audio_set_device_type(LE_AUDIO_DEVICE_TYPE_HEADSET);
#endif

    bt_callback_manager_register_callback(bt_callback_type_app_event,
                                          MODULE_MASK_GAP | MODULE_MASK_SYSTEM | MODULE_MASK_GATT,
                                          (void *)app_le_audio_event_callback);

    app_le_audio_aird_client_init();

    app_lea_adv_mgr_init();
    app_lea_conn_mgr_init();
    app_lea_conn_mgr_register_connection_cb(app_le_audio_connection_callback);

#ifdef AIR_LE_AUDIO_BIS_ENABLE
    app_le_audio_bis_init();
#endif

#ifdef MTK_AWS_MCE_ENABLE
    app_le_audio_mps_init();
#endif
}

bool app_le_audio_is_connected(void)
{
    bool ret = FALSE;
#ifdef AIR_LE_AUDIO_CIS_ENABLE
    ret = lea_local_cis_connected;
#ifdef MTK_AWS_MCE_ENABLE
    if (!ret) {
        ret = lea_peer_cis_connected;
    }
#endif
#endif
    //APPS_LOG_MSGID_I(LOG_TAG"[Power_Saving] is_connected=%d", 1, ret);
    return ret;
}

#endif  /* AIR_LE_AUDIO_ENABLE */

