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

#include "app_le_audio_adv_mgr.h"

#include "app_le_audio_conn_mgr.h"
#include "app_le_audio_event.h"

#include "ble_ascs_def.h"
#include "ble_bass_def.h"
#include "ble_cas_def.h"
#include "ble_csis.h"
#include "ble_pacs.h"

#include "bt_type.h"
#include "bt_gap_le_audio.h"

#include "bt_sink_srv.h"
#include "bt_sink_srv_le.h"
#include "bt_sink_srv_le_cap_audio_manager.h"
#if defined(MTK_BT_A2DP_VENDOR_2_ENABLE) || defined(MTK_BT_A2DP_VENDOR_CODEC_SUPPORT)
#include "bt_sink_srv_a2dp.h"
#endif

#ifdef MTK_AWS_MCE_ENABLE
#include "bt_aws_mce_srv.h"
#include "apps_aws_sync_event.h"
#endif

#include "apps_debug.h"
#include "apps_events_event_group.h"
#include "apps_events_bt_event.h"
#include "apps_config_features_dynamic_setting.h"
#include "multi_ble_adv_manager.h"

#include "bt_connection_manager.h"
#include "bt_customer_config.h"
#include "bt_device_manager.h"
#include "ui_shell_manager.h"
#include "FreeRTOS.h"

#if defined(AIR_LE_AUDIO_ENABLE) && (defined(AIR_MULTI_POINT_ENABLE) || defined(AIR_LE_AUDIO_MULTIPOINT_ENABLE))
#include "app_bt_conn_manager.h"
#include "bt_sink_srv_ami.h"
#endif

#define LOG_TAG     "[LEA][ADV]"

#define APP_LEA_IS_EMPTY_ADDR(X, EMPTY)        (memcmp((X), (EMPTY), BT_BD_ADDR_LEN) == 0)

typedef struct {
    uint8_t     mode;
    uint32_t    timeout;
} app_lea_adv_param_t;

static bool         app_lea_adv_enabled = FALSE;
static uint32_t     app_lea_adv_timeout = APP_LE_AUDIO_ADV_TIME;   /* 0 - always */

static uint16_t     app_lea_adv_interval_min = APP_LE_AUDIO_ADV_INTERVAL_MIN_S;
static uint16_t     app_lea_adv_interval_max = APP_LE_AUDIO_ADV_INTERVAL_MAX_S;

#define APP_LEA_MAX_TARGET_NUM        APP_LEA_MAX_BOND_NUM

typedef enum {
    APP_LEA_ADV_TIMEOUT_ACTION_STOP                 = 0,
    APP_LEA_ADV_TIMEOUT_ACTION_DIRECT_BOND_INFO,
} app_lea_adv_timeout_action_t;

static uint8_t      app_lea_adv_mode = APP_LEA_ADV_MODE_NONE;
static uint8_t      app_lea_adv_target_addr_num = 0;
static bt_addr_t    app_lea_adv_target_addr[APP_LEA_MAX_TARGET_NUM];



/**================================================================================*/
/**                                   Internal API                                 */
/**================================================================================*/
extern bool app_le_audio_is_enabled(void);
extern bt_status_t bt_gatts_service_set_le_audio_device_name(const uint8_t *device_name, uint16_t length);
static void app_le_audio_clear_target_addr(void);
static void app_le_audio_do_stop_advertising(void);

static uint8_t app_le_audio_convert_addr_type(bt_addr_type_t addr_type)
{
    uint8_t direct_adv_addr_type = 0xFF;       // 0xFF is invalid value
    if (addr_type == BT_ADDR_PUBLIC || addr_type == BT_ADDR_PUBLIC_IDENTITY) {
        direct_adv_addr_type = 0;
    } else if (addr_type == BT_ADDR_RANDOM || addr_type == BT_ADDR_RANDOM_IDENTITY) {
        direct_adv_addr_type = 1;
    }
    return direct_adv_addr_type;
}

static bool app_lea_adv_mgr_is_esco_ongoing(void)
{
    bool ret = FALSE;
    bt_sink_srv_device_state_t state_list[3] = {0};
    bt_sink_srv_get_device_state(NULL, state_list, 3);
    for (int i = 0; i < 3; i++) {
        if (state_list[i].sco_state == BT_SINK_SRV_SCO_CONNECTION_STATE_CONNECTED) {
            ret = true;
            break;
        }
    }
    return ret;
}

#if 0
static void app_le_audio_get_scan_response(bt_gap_le_set_ext_scan_response_data_t *scan_rsp)
{
    uint16_t device_name_len = 0;
    char device_name[BT_GAP_LE_MAX_DEVICE_NAME_LENGTH] = {0};
    bt_bd_addr_t *local_addr = bt_device_manager_get_local_address();

    snprintf(device_name, BT_GAP_LE_MAX_DEVICE_NAME_LENGTH, "LEA-H_%.2X%.2X%.2X%.2X%.2X%.2X",
             (*local_addr)[5], (*local_addr)[4], (*local_addr)[3],
             (*local_addr)[2], (*local_addr)[1], (*local_addr)[0]);
    device_name_len = strlen((char *)device_name);

    /* scan_rsp: AD_TYPE_NAME_COMPLETE */
    scan_rsp->data[0] = device_name_len + 1;
    scan_rsp->data[1] = BT_GAP_LE_AD_TYPE_NAME_COMPLETE;
    memcpy(&scan_rsp->data[2], device_name, device_name_len);
    scan_rsp->data_length = device_name_len + 2;

    //APPS_LOG_MSGID_I(LOG_TAG" get_scan_response, LEA device_name_len=%d", 1, device_name_len);

    /* set GATT GAP service device name */
    bt_gatts_service_set_le_audio_device_name((const uint8_t *)device_name, device_name_len);
}
#endif

static uint32_t app_le_audio_get_adv_data_internal(multi_ble_adv_info_t *adv_data)
{
    /* SCAN RSP */
    if (NULL != adv_data->scan_rsp) {
#if 0
        app_le_audio_get_scan_response(adv_data->scan_rsp);
#else
        adv_data->scan_rsp->data_length = 0;
#endif
    }

    /* ADV DATA */
    if ((NULL != adv_data->adv_data) && (NULL != adv_data->adv_data->data)) {
        uint16_t sink_conent, source_conent;
        uint8_t rsi[6];
        uint8_t len = 0;

        adv_data->adv_data->data[len] = 2;
        adv_data->adv_data->data[len + 1] = BT_GAP_LE_AD_TYPE_FLAG;
        adv_data->adv_data->data[len + 2] = BT_GAP_LE_AD_FLAG_BR_EDR_NOT_SUPPORTED | BT_GAP_LE_AD_FLAG_GENERAL_DISCOVERABLE;
        len += 3;

        /* adv_data: RSI */
        adv_data->adv_data->data[len] = 7;
        adv_data->adv_data->data[len + 1] = 0x2E;
        ble_csis_get_rsi(rsi);
        memcpy(&adv_data->adv_data->data[len + 2], rsi, sizeof(rsi));
        len += 8;

        /* adv_data: AD_TYPE_SERVICE_DATA (BAP)*/
        adv_data->adv_data->data[len] = 9;
        adv_data->adv_data->data[len + 1] = BT_GAP_LE_AD_TYPE_SERVICE_DATA;
        adv_data->adv_data->data[len + 2] = (BT_GATT_UUID16_ASCS_SERVICE & 0x00FF);
        adv_data->adv_data->data[len + 3] = ((BT_GATT_UUID16_ASCS_SERVICE & 0xFF00) >> 8);
        adv_data->adv_data->data[len + 4] = ANNOUNCEMENT_TYPE_GENERAL;
        ble_pacs_get_available_audio_contexts(&sink_conent, &source_conent);
        memcpy(&adv_data->adv_data->data[len + 5], &sink_conent, 2);
        memcpy(&adv_data->adv_data->data[len + 7], &source_conent, 2);
        adv_data->adv_data->data[len + 9] = 0x00; /* Length of the Metadata field = 0 */
        len += 10;

        /* adv_data: TX_POWER (BAP)*/
        adv_data->adv_data->data[len] = 2;
        adv_data->adv_data->data[len + 1] = BT_GAP_LE_AD_TYPE_TX_POWER;
        adv_data->adv_data->data[len + 2] = 0x7F;
        len += 3;

        /* adv_data: AD_TYPE_APPEARANCE (TMAP) */
        adv_data->adv_data->data[len] = 3;
        adv_data->adv_data->data[len + 1] = BT_GAP_LE_AD_TYPE_APPEARANCE;
        /* value: 2 bytes */
        adv_data->adv_data->data[len + 2] = 0x41;
        adv_data->adv_data->data[len + 3] = 0x09;
        len += 4;

        /* adv_data: AD_TYPE_SERVICE_DATA (BASS)*/
        adv_data->adv_data->data[len] = 3;
        adv_data->adv_data->data[len + 1] = BT_GAP_LE_AD_TYPE_SERVICE_DATA;
        /* BASS UUID: 2 bytes */
        adv_data->adv_data->data[len + 2] = (BT_SIG_UUID16_BASS & 0x00FF);
        adv_data->adv_data->data[len + 3] = ((BT_SIG_UUID16_BASS & 0xFF00) >> 8);
        len += 4;

        /* adv_data: AD_TYPE_SERVICE_DATA (CAS)*/
        adv_data->adv_data->data[len] = 4;
        adv_data->adv_data->data[len + 1] = BT_GAP_LE_AD_TYPE_SERVICE_DATA;
        /* CAS UUID: 2 bytes */
        adv_data->adv_data->data[len + 2] = (BT_SIG_UUID16_CAS & 0x00FF);
        adv_data->adv_data->data[len + 3] = ((BT_SIG_UUID16_CAS & 0xFF00) >> 8);
        adv_data->adv_data->data[len + 4] = ANNOUNCEMENT_TYPE_GENERAL;
        len += 5;

        uint16_t device_name_len = 0;

        char device_name[BT_GAP_LE_MAX_DEVICE_NAME_LENGTH] = {0};
        bt_bd_addr_t *local_addr = bt_device_manager_get_local_address();

        snprintf(device_name, BT_GAP_LE_MAX_DEVICE_NAME_LENGTH, "LEA-H_%.2X%.2X%.2X%.2X%.2X%.2X",
                 (*local_addr)[5], (*local_addr)[4], (*local_addr)[3],
                 (*local_addr)[2], (*local_addr)[1], (*local_addr)[0]);

        device_name_len = strlen((char *)device_name);

        /* scan_rsp: AD_TYPE_NAME_COMPLETE*/
        adv_data->adv_data->data[len] = device_name_len + 1;
        adv_data->adv_data->data[len + 1] = BT_GAP_LE_AD_TYPE_NAME_COMPLETE;
        memcpy(&adv_data->adv_data->data[len + 2], device_name, device_name_len);

        /*set GASTT GAP service device name*/
        bt_gatts_service_set_le_audio_device_name((const uint8_t *)device_name, device_name_len);

        adv_data->adv_data->data_length = len + 2 + device_name_len;

        //APPS_LOG_MSGID_I(LOG_TAG"[APP] data_length:%d", 1, adv_data->adv_data->data_length);
    }

    if (NULL != adv_data->adv_param) {
        adv_data->adv_param->advertising_event_properties = BT_HCI_ADV_EVT_PROPERTIES_MASK_CONNECTABLE;
        //| BT_HCI_ADV_EVT_PROPERTIES_MASK_SCANNABLE
        //| BT_HCI_ADV_EVT_PROPERTIES_MASK_LEGACY_PDU;
        /* Interval should be no larger than 100ms when discoverable */
        adv_data->adv_param->primary_advertising_interval_min = app_lea_adv_interval_min;
        adv_data->adv_param->primary_advertising_interval_max = app_lea_adv_interval_max;
        adv_data->adv_param->primary_advertising_channel_map = 0x07;
        adv_data->adv_param->own_address_type = BT_ADDR_RANDOM;
        adv_data->adv_param->advertising_filter_policy = 0;
        adv_data->adv_param->advertising_tx_power = 0x7F;
        adv_data->adv_param->primary_advertising_phy = BT_HCI_LE_ADV_PHY_1M;
        adv_data->adv_param->secondary_advertising_phy = BT_HCI_LE_ADV_PHY_1M;
    }
    return 0;
}

static uint32_t app_le_audio_get_adv_data(multi_ble_adv_info_t *adv_data, uint8_t target_index)
{
    uint32_t ret = app_le_audio_get_adv_data_internal(adv_data);

    if (app_lea_adv_mode >= APP_LEA_ADV_MODE_DIRECT && adv_data->adv_param != NULL) {
        if (app_lea_adv_target_addr_num == 0) {
            APPS_LOG_MSGID_E(LOG_TAG" get_adv_data, error DIRECT mode but target_num==0", 0);
        } else if (target_index >= app_lea_adv_target_addr_num) {
            APPS_LOG_MSGID_E(LOG_TAG" get_adv_data, error target_index(%d) > target_num(%d)",
                             2, target_index, app_lea_adv_target_addr_num);
        } else {
            uint8_t direct_adv_addr_type = app_le_audio_convert_addr_type(app_lea_adv_target_addr[target_index].type);
            adv_data->adv_param->advertising_event_properties |= BT_HCI_ADV_EVT_PROPERTIES_MASK_DIRECTED;
            adv_data->adv_param->advertising_event_properties &= (~BT_HCI_ADV_EVT_PROPERTIES_MASK_SCANNABLE);
            adv_data->adv_param->peer_address.type = direct_adv_addr_type;
            memcpy(adv_data->adv_param->peer_address.addr, app_lea_adv_target_addr[target_index].addr, sizeof(bt_bd_addr_t));
            adv_data->adv_param->own_address_type = BT_ADDR_RANDOM_IDENTITY;

            if (adv_data->adv_data != NULL) {
                adv_data->adv_data->data_length = 0;
            }
            if (adv_data->scan_rsp != NULL) {
                adv_data->scan_rsp->data_length = 0;
            }
        }
    }
    return ret;
}

static uint32_t app_le_audio_get_adv_data1(multi_ble_adv_info_t *adv_data)
{
    return app_le_audio_get_adv_data(adv_data, 0);
}

static uint32_t app_le_audio_get_adv_data2(multi_ble_adv_info_t *adv_data)
{
    return app_le_audio_get_adv_data(adv_data, 1);
}

static uint32_t app_le_audio_get_adv_data3(multi_ble_adv_info_t *adv_data)
{
    return app_le_audio_get_adv_data(adv_data, 2);
}

static uint32_t app_le_audio_get_adv_data4(multi_ble_adv_info_t *adv_data)
{
    return app_le_audio_get_adv_data(adv_data, 3);
}

static void app_lea_audio_update_multi_adv(void)
{
    multi_ble_adv_manager_remove_ble_adv(MULTI_ADV_INSTANCE_NOT_RHO, app_le_audio_get_adv_data1);
    multi_ble_adv_manager_remove_ble_adv(MULTI_ADV_INSTANCE_NOT_RHO, app_le_audio_get_adv_data2);
    multi_ble_adv_manager_remove_ble_adv(MULTI_ADV_INSTANCE_NOT_RHO, app_le_audio_get_adv_data3);
    multi_ble_adv_manager_remove_ble_adv(MULTI_ADV_INSTANCE_NOT_RHO, app_le_audio_get_adv_data4);
    multi_ble_adv_manager_add_ble_adv(MULTI_ADV_INSTANCE_NOT_RHO, app_le_audio_get_adv_data1, 1);
    if (app_lea_adv_mode > APP_LEA_ADV_MODE_DIRECT) {
        if (app_lea_adv_target_addr_num == 2) {
            multi_ble_adv_manager_add_ble_adv(MULTI_ADV_INSTANCE_NOT_RHO, app_le_audio_get_adv_data1, 1);
        } else if (app_lea_adv_target_addr_num == 3) {
            multi_ble_adv_manager_add_ble_adv(MULTI_ADV_INSTANCE_NOT_RHO, app_le_audio_get_adv_data2, 1);
            multi_ble_adv_manager_add_ble_adv(MULTI_ADV_INSTANCE_NOT_RHO, app_le_audio_get_adv_data3, 1);
        } else if (app_lea_adv_target_addr_num == 4) {
            multi_ble_adv_manager_add_ble_adv(MULTI_ADV_INSTANCE_NOT_RHO, app_le_audio_get_adv_data2, 1);
            multi_ble_adv_manager_add_ble_adv(MULTI_ADV_INSTANCE_NOT_RHO, app_le_audio_get_adv_data3, 1);
            multi_ble_adv_manager_add_ble_adv(MULTI_ADV_INSTANCE_NOT_RHO, app_le_audio_get_adv_data4, 1);
        }
    }
    multi_ble_adv_manager_notify_ble_adv_data_changed(MULTI_ADV_INSTANCE_NOT_RHO);
}

static void app_le_audio_do_start_advertising(uint8_t mode, uint32_t timeout)
{
    APPS_LOG_MSGID_I(LOG_TAG" do_start_advertising, start adv_mode=%d timeout=%d", 2, mode, timeout);
    uint8_t adv_timeout_action = APP_LEA_ADV_TIMEOUT_ACTION_STOP;
    app_lea_adv_mode = mode;
    if (mode == APP_LEA_ADV_MODE_DIRECT) {
#if (APP_LE_AUDIO_ADV_CONN_MODE == APP_LE_AUDIO_MODE_PHONE_SWITCH)
        adv_timeout_action = APP_LEA_ADV_TIMEOUT_ACTION_DIRECT_BOND_INFO;
#endif
    } else if (mode == APP_LEA_ADV_MODE_DIRECT_BOND_INFO) {
        app_le_audio_clear_target_addr();
        uint8_t empty_addr[BT_BD_ADDR_LEN] = {0};
        app_lea_bond_info_t *bond_info = app_lea_conn_mgr_get_bond_info();
        for (int i = 0; i < APP_LEA_MAX_TARGET_NUM; i++) {
            uint8_t addr_type = bond_info[i].addr_type;
            const uint8_t *addr = bond_info[i].bond_addr;
            if (bond_info[i].used && !APP_LEA_IS_EMPTY_ADDR(addr, empty_addr)) {
                APPS_LOG_MSGID_I(LOG_TAG" do_start_advertising, bond_info[%d] type=%d addr=%02X:%02X:%02X:%02X:%02X:%02X",
                                 8, i, bond_info[i].addr_type, addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);
                app_lea_adv_mgr_update_target_addr(APP_LEA_TARGET_ADD_ADDR, addr_type, addr);
            } else {
                APPS_LOG_MSGID_I(LOG_TAG" do_start_advertising, bond_info[%d] empty", 1, i);
            }
        }

        if (app_lea_adv_target_addr_num == 0) {
            APPS_LOG_MSGID_E(LOG_TAG" do_start_advertising, bond_info is empty", 0);
            app_le_audio_do_stop_advertising();
            return;
        }
    }

    uint8_t cur_conn_num = app_lea_conn_mgr_get_conn_num();
    uint8_t support_max_conn_num = app_lea_conn_mgr_get_support_max_conn_num();
    if (!app_le_audio_is_enabled()) {
        APPS_LOG_MSGID_E(LOG_TAG" do_start_advertising, error LC3 disabled", 0);
        return;
    } else if (apps_config_features_is_mp_test_mode()) {
        APPS_LOG_MSGID_E(LOG_TAG" do_start_advertising, error MP TEST Mode", 0);
        return;
    } else if (cur_conn_num == APP_LEA_MAX_CONN_NUM) {
        APPS_LOG_MSGID_E(LOG_TAG" do_start_advertising, error max link %d==%d", 2, cur_conn_num, APP_LEA_MAX_CONN_NUM);
        return;
    } else if (cur_conn_num == support_max_conn_num) {
        APPS_LOG_MSGID_E(LOG_TAG" do_start_advertising, error max support link %d==%d", 2, cur_conn_num, support_max_conn_num);
        return;
    } else if (app_lea_adv_mode == APP_LEA_ADV_MODE_NONE) {
        APPS_LOG_MSGID_E(LOG_TAG" do_start_advertising, error ADV mode", 0);
        return;
    } else if (app_lea_adv_mode > APP_LEA_ADV_MODE_DIRECT && app_lea_adv_target_addr_num == 0) {
        APPS_LOG_MSGID_E(LOG_TAG" do_start_advertising, error DIRECT but no target", 0);
        return;
    }
#if defined(AIR_LE_AUDIO_ENABLE) && (defined(AIR_MULTI_POINT_ENABLE) || defined(AIR_LE_AUDIO_MULTIPOINT_ENABLE))
    if (!app_bt_conn_manager_allow_le_audio()) {
        APPS_LOG_MSGID_E(LOG_TAG" do_start_advertising, error APP_CONN disallow", 0);
        return;
    }
#endif

#ifdef AIR_LE_AUDIO_HEADSET_ENABLE
    extern bool app_le_audio_is_wired_audio(void);
    if (app_le_audio_is_wired_audio()) {
        APPS_LOG_MSGID_E(LOG_TAG" do_start_advertising, error Wired Audio", 0);
        return;
    }
#endif

    /* LE-Audio ADV is long interval when the AWS link is disconnected. */
    if (bt_sink_srv_get_state() >= BT_SINK_SRV_STATE_STREAMING
        || app_lea_adv_mgr_is_esco_ongoing()
#ifdef MTK_AWS_MCE_ENABLE
        || BT_AWS_MCE_SRV_LINK_NONE == bt_aws_mce_srv_get_link_type()
#endif
       ) {
        app_lea_adv_interval_min = APP_LE_AUDIO_ADV_INTERVAL_MIN_L;
        app_lea_adv_interval_max = APP_LE_AUDIO_ADV_INTERVAL_MAX_L;
    } else {
        app_lea_adv_interval_min = APP_LE_AUDIO_ADV_INTERVAL_MIN_S;
        app_lea_adv_interval_max = APP_LE_AUDIO_ADV_INTERVAL_MAX_S;
    }

    app_lea_audio_update_multi_adv();

    if (app_lea_adv_enabled) {
        if (app_lea_adv_timeout == 0 && timeout > 0) {
            // Keep timeout=0 always advertising
        } else {
            app_lea_adv_timeout = timeout;
        }
    } else {
        app_lea_adv_timeout = timeout;
    }

    ui_shell_remove_event(EVENT_GROUP_UI_SHELL_LE_AUDIO, EVENT_ID_LE_AUDIO_ADV_TIMER);
    if (app_lea_adv_timeout > 0) {
        ui_shell_send_event(FALSE, EVENT_PRIORITY_MIDDLE,
                            EVENT_GROUP_UI_SHELL_LE_AUDIO, EVENT_ID_LE_AUDIO_ADV_TIMER,
                            (void *)(int)adv_timeout_action, 0, NULL, app_lea_adv_timeout);
    }

    app_lea_adv_enabled = TRUE;
    APPS_LOG_MSGID_I(LOG_TAG" do_start_advertising, success adv_mode=%d timeout=%d interval=0x%04X 0x%04X",
                     4, app_lea_adv_mode, app_lea_adv_timeout,
                     app_lea_adv_interval_min, app_lea_adv_interval_max);
}

static void app_le_audio_do_stop_advertising(void)
{
    if (apps_config_features_is_mp_test_mode()) {
        APPS_LOG_MSGID_E(LOG_TAG"do_stop_advertising fail, MP TEST Mode", 0);
        return;
    }

    ui_shell_remove_event(EVENT_GROUP_UI_SHELL_LE_AUDIO, EVENT_ID_LE_AUDIO_ADV_TIMER);

    multi_ble_adv_manager_remove_ble_adv(MULTI_ADV_INSTANCE_NOT_RHO, app_le_audio_get_adv_data1);
    multi_ble_adv_manager_remove_ble_adv(MULTI_ADV_INSTANCE_NOT_RHO, app_le_audio_get_adv_data2);
    multi_ble_adv_manager_remove_ble_adv(MULTI_ADV_INSTANCE_NOT_RHO, app_le_audio_get_adv_data3);
    multi_ble_adv_manager_remove_ble_adv(MULTI_ADV_INSTANCE_NOT_RHO, app_le_audio_get_adv_data4);
    multi_ble_adv_manager_notify_ble_adv_data_changed(MULTI_ADV_INSTANCE_NOT_RHO);

    app_lea_adv_enabled = FALSE;
    app_lea_adv_timeout = 0;
    app_lea_adv_mode = APP_LEA_ADV_MODE_NONE;

    app_le_audio_clear_target_addr();
    APPS_LOG_MSGID_I(LOG_TAG" do_stop_advertising", 0);
}

static void app_le_audio_do_update_advertising_param(void)
{
    uint8_t cur_conn_num = app_lea_conn_mgr_get_conn_num();
    uint8_t support_max_conn_num = app_lea_conn_mgr_get_support_max_conn_num();
    if (!app_le_audio_is_enabled()) {
        APPS_LOG_MSGID_E(LOG_TAG" update_advertising_param, error LC3 disabled", 0);
        return;
    } else if (apps_config_features_is_mp_test_mode()) {
        APPS_LOG_MSGID_E(LOG_TAG" update_advertising_param, error MP TEST Mode", 0);
        return;
    } else if (!app_lea_adv_enabled) {
        APPS_LOG_MSGID_E(LOG_TAG" update_advertising_param, error already stopped", 0);
        return;
    } else if (cur_conn_num == APP_LEA_MAX_CONN_NUM) {
        APPS_LOG_MSGID_E(LOG_TAG" update_advertising_param, error max link %d==%d", 2, cur_conn_num, APP_LEA_MAX_CONN_NUM);
        return;
    } else if (cur_conn_num == support_max_conn_num) {
        APPS_LOG_MSGID_E(LOG_TAG" update_advertising_param, error max support link %d==%d", 2, cur_conn_num, support_max_conn_num);
        return;
    } else if (app_lea_adv_mode == APP_LEA_ADV_MODE_NONE) {
        APPS_LOG_MSGID_E(LOG_TAG" update_advertising_param, error ADV mode", 0);
        return;
    } else if (app_lea_adv_mode > APP_LEA_ADV_MODE_DIRECT && app_lea_adv_target_addr_num == 0) {
        APPS_LOG_MSGID_E(LOG_TAG" update_advertising_param, error DIRECT but no target", 0);
        return;
    }
#if defined(AIR_LE_AUDIO_ENABLE) && (defined(AIR_MULTI_POINT_ENABLE) || defined(AIR_LE_AUDIO_MULTIPOINT_ENABLE))
    if (!app_bt_conn_manager_allow_le_audio()) {
        APPS_LOG_MSGID_E(LOG_TAG" update_advertising_param, error APP_CONN disallow", 0);
        return;
    }
#endif

    app_lea_audio_update_multi_adv();

    app_lea_adv_enabled = TRUE;

    APPS_LOG_MSGID_I(LOG_TAG" update_advertising_param, adv_mode=%d timeout=%d interval=0x%04X 0x%04X",
                     4, app_lea_adv_mode, app_lea_adv_timeout,
                     app_lea_adv_interval_min, app_lea_adv_interval_max);
}

static void app_le_audio_clear_target_addr(void)
{
    bt_aws_mce_role_t role = bt_connection_manager_device_local_info_get_aws_role();
    APPS_LOG_MSGID_I(LOG_TAG" [%02X] clear_target_addr", 1, role);
    app_lea_adv_target_addr_num = 0;
    memset(app_lea_adv_target_addr, 0, sizeof(bt_addr_t) * APP_LEA_MAX_TARGET_NUM);
}

static void app_le_audio_print_target_addr(void)
{
    bt_aws_mce_role_t role = bt_connection_manager_device_local_info_get_aws_role();
    for (int i = 0; i < APP_LEA_MAX_TARGET_NUM; i++) {
        uint8_t addr_type = app_lea_adv_target_addr[i].type;
        const uint8_t *addr = app_lea_adv_target_addr[i].addr;
        APPS_LOG_MSGID_I(LOG_TAG" printf_target_addr, [%02X][%d] num=%d addr_type=%d addr=%02X:%02X:%02X:%02X:%02X:%02X",
                         10, role, i, app_lea_adv_target_addr_num,
                         addr_type, addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);
    }
}

bool app_lea_adv_mgr_update_target_addr_imp(app_le_audio_update_target_mode_t mode,
                                            bt_addr_type_t addr_type,
                                            const uint8_t *addr)
{
    bool ret = FALSE;
    uint8_t empty_addr[BT_BD_ADDR_LEN] = {0};
    bt_aws_mce_role_t role = bt_connection_manager_device_local_info_get_aws_role();

    APPS_LOG_MSGID_I(LOG_TAG" update_target_addr, [%02X] mode=%d addr_type=%d addr=0x%08X",
                     4, role, mode, addr_type, addr);
    app_le_audio_print_target_addr();

    if (mode == APP_LEA_TARGET_ADD_ADDR && app_lea_adv_target_addr_num == APP_LEA_MAX_TARGET_NUM) {
        APPS_LOG_MSGID_E(LOG_TAG" update_target_addr, [%02X] error target_addr_list full", 1, role);
        goto exit;
    } else if ((mode == APP_LEA_TARGET_ADD_ADDR || mode == APP_LEA_TARGET_SET_UNIQUE_ADDR || mode == APP_LEA_TARGET_REMOVE_ADDR)
               && APP_LEA_IS_EMPTY_ADDR(addr, empty_addr)) {
        APPS_LOG_MSGID_E(LOG_TAG" update_target_addr, [%02X] error addr", 1, role);
        goto exit;
    } else if (mode == APP_LEA_TARGET_REMOVE_ONE && app_lea_adv_target_addr_num == 0) {
        APPS_LOG_MSGID_E(LOG_TAG" update_target_addr, [%02X] error target_addr_list empty", 1, role);
        goto exit;
    }

    switch (mode) {
        case APP_LEA_TARGET_ADD_ADDR: {
            for (int i = 0; i < APP_LEA_MAX_TARGET_NUM; i++) {
                uint8_t *target_addr = (uint8_t *)app_lea_adv_target_addr[i].addr;
                if (APP_LEA_IS_EMPTY_ADDR(target_addr, empty_addr)) {
                    app_lea_adv_target_addr[i].type = addr_type;
                    memcpy(target_addr, addr, BT_BD_ADDR_LEN);
                    app_lea_adv_target_addr_num += 1;
                    ret = TRUE;
                    break;
                } else if (memcmp(target_addr, addr, BT_BD_ADDR_LEN) == 0) {
                    ret = TRUE;
                    break;
                }
            }
            break;
        }
        case APP_LEA_TARGET_SET_UNIQUE_ADDR: {
            for (int i = 0; i < APP_LEA_MAX_TARGET_NUM; i++) {
                uint8_t *target_addr = (uint8_t *)app_lea_adv_target_addr[i].addr;
                if (i == 0) {
                    app_lea_adv_target_addr[0].type = addr_type;
                    memcpy(target_addr, addr, BT_BD_ADDR_LEN);
                } else {
                    app_lea_adv_target_addr[i].type = 0;
                    memset(target_addr, 0, BT_BD_ADDR_LEN);
                }
            }
            app_lea_adv_target_addr_num = 1;
            ret = TRUE;
            break;
        }
        case APP_LEA_TARGET_REMOVE_ONE: {
            for (int i = APP_LEA_MAX_TARGET_NUM - 1; i >= 0; i--) {
                uint8_t *target_addr = (uint8_t *)app_lea_adv_target_addr[i].addr;
                if (!APP_LEA_IS_EMPTY_ADDR(target_addr, empty_addr)) {
                    app_lea_adv_target_addr[i].type = 0;
                    memset(target_addr, 0, BT_BD_ADDR_LEN);
                    app_lea_adv_target_addr_num -= 1;
                    ret = TRUE;
                    break;
                }
            }
            break;
        }
        case APP_LEA_TARGET_REMOVE_ADDR: {
            for (int i = 0; i < APP_LEA_MAX_TARGET_NUM; i++) {
                uint8_t *target_addr = (uint8_t *)app_lea_adv_target_addr[i].addr;
                if (memcmp(addr, target_addr, BT_BD_ADDR_LEN) == 0) {
                    APPS_LOG_MSGID_I(LOG_TAG" update_target_addr, [%02X] clear index=%d", 2, role, i);
                    app_lea_adv_target_addr[i].type = 0;
                    memset(target_addr, 0, BT_BD_ADDR_LEN);
                    app_lea_adv_target_addr_num -= 1;
                    ret = TRUE;
                    break;
                }
            }
            break;
        }
        case APP_LEA_TARGET_REMOVE_ALL: {
            app_le_audio_clear_target_addr();
            ret = TRUE;
            break;
        }
    }

exit:
    configASSERT(app_lea_adv_target_addr_num <= APP_LEA_MAX_TARGET_NUM);

    if (ret) {
        app_le_audio_print_target_addr();
    } else {
        APPS_LOG_MSGID_E(LOG_TAG" update_target_addr, [%02X] error mode=%d", 2, role, mode);
    }
    return ret;
}



/**================================================================================*/
/**                                 APP Event Handler                              */
/**================================================================================*/
static void app_lea_adv_mgr_bt_sink_event_group(uint32_t event_id, void *extra_data, size_t data_len)
{
    switch (event_id) {
        case BT_SINK_SRV_EVENT_STATE_CHANGE: {
            bt_sink_srv_state_change_t *param = (bt_sink_srv_state_change_t *)extra_data;
            if (param == NULL) {
                break;
            }

            if (param->previous != BT_SINK_SRV_STATE_STREAMING && param->current == BT_SINK_SRV_STATE_STREAMING) {
                APPS_LOG_MSGID_I(LOG_TAG" Music Start", 0);
                app_lea_adv_interval_min = APP_LE_AUDIO_ADV_INTERVAL_MIN_L;
                app_lea_adv_interval_max = APP_LE_AUDIO_ADV_INTERVAL_MAX_L;
                app_le_audio_do_update_advertising_param();
            } else if (param->previous == BT_SINK_SRV_STATE_STREAMING && param->current != BT_SINK_SRV_STATE_STREAMING) {
                APPS_LOG_MSGID_I(LOG_TAG" Music Stop", 0);
                app_lea_adv_interval_min = APP_LE_AUDIO_ADV_INTERVAL_MIN_S;
                app_lea_adv_interval_max = APP_LE_AUDIO_ADV_INTERVAL_MAX_S;
                app_le_audio_do_update_advertising_param();
            }
            break;
        }

        case BT_SINK_SRV_EVENT_HF_SCO_STATE_UPDATE: {
            bt_sink_srv_sco_state_update_t *esco_state = (bt_sink_srv_sco_state_update_t *)extra_data;
            if (esco_state == NULL) {
                break;
            }

            if (esco_state->state == BT_SINK_SRV_SCO_CONNECTION_STATE_CONNECTED) {
                APPS_LOG_MSGID_I(LOG_TAG" eSCO Connected", 0);
                app_lea_adv_interval_min = APP_LE_AUDIO_ADV_INTERVAL_MIN_L;
                app_lea_adv_interval_max = APP_LE_AUDIO_ADV_INTERVAL_MAX_L;
                app_le_audio_do_update_advertising_param();
            } else if (esco_state->state == BT_SINK_SRV_SCO_CONNECTION_STATE_DISCONNECTED) {
                APPS_LOG_MSGID_I(LOG_TAG" eSCO Disconnected", 0);
                app_lea_adv_interval_min = APP_LE_AUDIO_ADV_INTERVAL_MIN_S;
                app_lea_adv_interval_max = APP_LE_AUDIO_ADV_INTERVAL_MAX_S;
                app_le_audio_do_update_advertising_param();
            }
            break;
        }
        default:
            break;
    }
}

static void app_lea_adv_mgr_bt_dm_event_group(uint32_t event_id, void *extra_data, size_t data_len)
{
    bt_device_manager_power_event_t evt;
    bt_device_manager_power_status_t status;
    bt_event_get_bt_dm_event_and_status(event_id, &evt, &status);
    switch (evt) {
        case BT_DEVICE_MANAGER_POWER_EVT_STANDBY_COMPLETE: {
            if (BT_DEVICE_MANAGER_POWER_RESET_TYPE_NORMAL == status) {
                APPS_LOG_MSGID_I(LOG_TAG" BT_DM POWER OFF", 0);
                app_le_audio_clear_target_addr();
            }
            break;
        }
    }
}

static void app_lea_adv_mgr_lea_event_group(uint32_t event_id, void *extra_data, size_t data_len)
{
    switch (event_id) {
        case EVENT_ID_LE_AUDIO_ADV_TIMER: {
            uint8_t action = (uint8_t)(int)extra_data;
            APPS_LOG_MSGID_I(LOG_TAG" LE Audio event, ADV timeout action=%d", 1, action);
            if (action == APP_LEA_ADV_TIMEOUT_ACTION_DIRECT_BOND_INFO) {
                app_le_audio_do_start_advertising(APP_LEA_ADV_MODE_DIRECT_BOND_INFO, APP_LE_AUDIO_ADV_TIME);
            } else {
                app_le_audio_do_stop_advertising();
            }
            break;
        }
        case EVENT_ID_LE_AUDIO_START_ADV: {
            app_lea_adv_param_t *adv_param = (app_lea_adv_param_t *)extra_data;
            APPS_LOG_MSGID_I(LOG_TAG" LE Audio event, start ADV", 0);
            app_le_audio_do_start_advertising(adv_param->mode, adv_param->timeout);
            break;
        }
        case EVENT_ID_LE_AUDIO_STOP_ADV: {
            APPS_LOG_MSGID_I(LOG_TAG" LE Audio event, stop ADV", 0);
            app_le_audio_do_stop_advertising();
            break;
        }
    }
}

#ifdef MTK_AWS_MCE_ENABLE
static void app_lea_adv_mgr_proc_aws_data(void *extra_data, size_t data_len)
{
    uint32_t aws_event_group;
    uint32_t aws_event_id;
    void *p_extra_data = NULL;
    uint32_t extra_data_len = 0;
    bt_aws_mce_role_t role = bt_connection_manager_device_local_info_get_aws_role();
    bt_aws_mce_report_info_t *aws_data_ind = (bt_aws_mce_report_info_t *)extra_data;

    if (aws_data_ind == NULL || aws_data_ind->module_id != BT_AWS_MCE_REPORT_MODULE_APP_ACTION) {
        return;
    }

    apps_aws_sync_event_decode_extra(aws_data_ind, &aws_event_group, &aws_event_id,
                                     &p_extra_data, &extra_data_len);
    if (aws_event_group == EVENT_GROUP_UI_SHELL_LE_AUDIO
        && aws_event_id == EVENT_ID_LE_AUDIO_START_ADV) {
        app_lea_adv_param_t *adv_param = (app_lea_adv_param_t *)p_extra_data;
        APPS_LOG_MSGID_I(LOG_TAG" AWS_DATA event, [%02X] start ADV", 1, role);
        app_le_audio_do_start_advertising(adv_param->mode, adv_param->timeout);
    } else if (aws_event_group == EVENT_GROUP_UI_SHELL_LE_AUDIO
               && aws_event_id == EVENT_ID_LE_AUDIO_STOP_ADV) {
        APPS_LOG_MSGID_I(LOG_TAG" AWS_DATA event, [%02X] stop ADV", 1, role);
        app_le_audio_do_stop_advertising();
    }
}
#endif

static void app_le_audio_mgr_proc_bt_cm_group(uint32_t event_id, void *extra_data, size_t data_len)
{
    switch (event_id) {
        case BT_CM_EVENT_REMOTE_INFO_UPDATE: {
            bt_cm_remote_info_update_ind_t *remote_update = (bt_cm_remote_info_update_ind_t *)extra_data;
            if (NULL == remote_update) {
                break;
            }
#if defined(MTK_AWS_MCE_ENABLE) || defined(MTK_BT_A2DP_VENDOR_2_ENABLE) || defined(MTK_BT_A2DP_VENDOR_CODEC_SUPPORT)
            bt_aws_mce_role_t role = bt_connection_manager_device_local_info_get_aws_role();
#endif

#ifdef MTK_AWS_MCE_ENABLE
            if (!(BT_CM_PROFILE_SERVICE_MASK(BT_CM_PROFILE_SERVICE_AWS) & remote_update->pre_connected_service) &&
                (BT_CM_PROFILE_SERVICE_MASK(BT_CM_PROFILE_SERVICE_AWS) & remote_update->connected_service)) {
                APPS_LOG_MSGID_I(LOG_TAG" BT_CM event, [%02X] AWS Connected reason=0x%02X", 2, role, remote_update->reason);

#if defined(MTK_BT_A2DP_VENDOR_2_ENABLE) || defined(MTK_BT_A2DP_VENDOR_CODEC_SUPPORT)
                if (BT_AWS_MCE_ROLE_PARTNER == role && bt_sink_srv_a2dp_get_vendor_codec_config()) {
                    if (BT_AWS_MCE_SRV_LINK_NORMAL == bt_aws_mce_srv_get_link_type()) {
                        APPS_LOG_MSGID_I(LOG_TAG"[vendor_codec] BT_CM event, [%02X] AWS Connected normal link stop ADV.", 1, role);
                        app_lea_adv_mgr_stop_advertising(FALSE);
                    }
                }
#endif
                /* LE-Audio ADV resume to the short interval when the AWS link connected. */
                if (BT_SINK_SRV_STATE_STREAMING > bt_sink_srv_get_state()
                    || !app_lea_adv_mgr_is_esco_ongoing()) {
                    app_lea_adv_interval_min = APP_LE_AUDIO_ADV_INTERVAL_MIN_S;
                    app_lea_adv_interval_max = APP_LE_AUDIO_ADV_INTERVAL_MAX_S;
                    app_le_audio_do_update_advertising_param();
                }
            } else if ((BT_CM_PROFILE_SERVICE_MASK(BT_CM_PROFILE_SERVICE_AWS) & remote_update->pre_connected_service)
                       && !(BT_CM_PROFILE_SERVICE_MASK(BT_CM_PROFILE_SERVICE_AWS) & remote_update->connected_service)) {
                APPS_LOG_MSGID_I(LOG_TAG" BT_CM event, [%02X] AWS Disconnected reason=0x%02X", 2, role, remote_update->reason);

                /* LE-Audio ADV resume to the long interval when the AWS link disconnected. */
                if (app_lea_adv_interval_min != APP_LE_AUDIO_ADV_INTERVAL_MIN_L
                    || app_lea_adv_interval_max != APP_LE_AUDIO_ADV_INTERVAL_MAX_L) {
                    app_lea_adv_interval_min = APP_LE_AUDIO_ADV_INTERVAL_MIN_L;
                    app_lea_adv_interval_max = APP_LE_AUDIO_ADV_INTERVAL_MAX_L;
                    app_le_audio_do_update_advertising_param();
                }
#if defined(MTK_BT_A2DP_VENDOR_2_ENABLE) || defined(MTK_BT_A2DP_VENDOR_CODEC_SUPPORT)
                if (BT_AWS_MCE_ROLE_PARTNER == role) {
                    APPS_LOG_MSGID_I(LOG_TAG"[vendor_codec] BT_CM event, [%02X] AWS disconnected resume ADV.", 1, role);
                    app_lea_adv_mgr_start_advertising(APP_LEA_ADV_MODE_UNDIRECT, FALSE, 0);
                }
#endif
            }
#endif /* MTK_AWS_MCE_ENABLE*/

            if ((BT_CM_ACL_LINK_CONNECTED <= remote_update->pre_acl_state) &&
                (BT_CM_ACL_LINK_CONNECTED > remote_update->acl_state) &&
                (0 == bt_cm_get_connected_devices(~BT_CM_PROFILE_SERVICE_MASK(BT_CM_PROFILE_SERVICE_AWS), NULL, 0))) {
#if defined(MTK_BT_A2DP_VENDOR_2_ENABLE) || defined(MTK_BT_A2DP_VENDOR_CODEC_SUPPORT)
#ifdef MTK_AWS_MCE_ENABLE
                if (BT_AWS_MCE_ROLE_AGENT == role)
#endif
                {
                    APPS_LOG_MSGID_I(LOG_TAG"[vendor_codec] BT_CM event, [%02X] ACL disconnected start ADV", 1, role);
                    app_lea_adv_mgr_start_advertising(APP_LEA_ADV_MODE_UNDIRECT, FALSE, 0);
                }
            } else if (BT_CM_ACL_LINK_CONNECTED > remote_update->pre_acl_state
                       && BT_CM_ACL_LINK_CONNECTED <= remote_update->acl_state
                       && 0 != memcmp(remote_update->address, bt_device_manager_get_local_address(), sizeof(bt_bd_addr_t))) {
#ifdef MTK_AWS_MCE_ENABLE
                if (BT_AWS_MCE_ROLE_AGENT == role)
#endif
                {
                    bool codec_config = bt_sink_srv_a2dp_get_vendor_codec_config();
                    APPS_LOG_MSGID_I(LOG_TAG"[vendor_codec] BT_CM event, [%02X] ACL connected & codec_config=%d stop ADV",
                                     2, role, codec_config);
                    if (codec_config) {
                        app_lea_adv_mgr_stop_advertising(FALSE);
                    }
                }
#endif
            }

            break;
        }
        default:
            break;
    }
}



/**================================================================================*/
/**                                     Public API                                 */
/**================================================================================*/
void app_lea_adv_mgr_start_advertising(uint8_t mode, bool sync, uint32_t timeout)
{
    APPS_LOG_MSGID_I(LOG_TAG" start_advertising, mode=%d sync=%d timeout=%d",
                     3, mode, sync, timeout);
    app_lea_adv_param_t *adv_param = (app_lea_adv_param_t *)pvPortMalloc(sizeof(app_lea_adv_param_t));
    if (adv_param == NULL) {
        APPS_LOG_MSGID_E(LOG_TAG" start_advertising, malloc error", 0);
        return;
    } else if (mode == APP_LEA_ADV_MODE_NONE) {
        APPS_LOG_MSGID_E(LOG_TAG" start_advertising, none mode", 0);
        if (adv_param != NULL) {
            vPortFree(adv_param);
        }
        return;
    }
#ifndef AIR_LE_AUDIO_DIRECT_ADV
    if (mode == APP_LEA_ADV_MODE_DIRECT || mode == APP_LEA_ADV_MODE_DIRECT_BOND_INFO) {
        if (adv_param != NULL) {
            vPortFree(adv_param);
        }
        APPS_LOG_MSGID_E(LOG_TAG" start_advertising, disable DIRECT ADV mode", 0);
        return;
    }
#endif

    adv_param->mode = mode;
    adv_param->timeout = timeout;
    ui_shell_send_event(FALSE, EVENT_PRIORITY_HIGH,
                        EVENT_GROUP_UI_SHELL_LE_AUDIO, EVENT_ID_LE_AUDIO_START_ADV,
                        (void *)adv_param, sizeof(app_lea_adv_param_t), NULL, 0);

#ifdef MTK_AWS_MCE_ENABLE
    bt_aws_mce_srv_link_type_t aws_link_type = bt_aws_mce_srv_get_link_type();
    if (sync && aws_link_type != BT_AWS_MCE_SRV_LINK_NONE) {
        bt_status_t bt_status = apps_aws_sync_event_send_extra(EVENT_GROUP_UI_SHELL_LE_AUDIO,
                                                               EVENT_ID_LE_AUDIO_START_ADV,
                                                               adv_param,
                                                               sizeof(app_lea_adv_param_t));
        if (bt_status != BT_STATUS_SUCCESS) {
            APPS_LOG_MSGID_E(LOG_TAG" start_advertising, error AWS bt_status=0x%08X", 1, bt_status);
        }
    }
#endif
}

void app_lea_adv_mgr_stop_advertising(bool sync)
{
    APPS_LOG_MSGID_I(LOG_TAG" stop_advertising, sync=%d", 1, sync);
    ui_shell_send_event(FALSE, EVENT_PRIORITY_HIGH,
                        EVENT_GROUP_UI_SHELL_LE_AUDIO, EVENT_ID_LE_AUDIO_STOP_ADV,
                        NULL, 0, NULL, 0);

#ifdef MTK_AWS_MCE_ENABLE
    bt_aws_mce_srv_link_type_t aws_link_type = bt_aws_mce_srv_get_link_type();
    if (sync && aws_link_type != BT_AWS_MCE_SRV_LINK_NONE) {
        bt_status_t bt_status = apps_aws_sync_event_send(EVENT_GROUP_UI_SHELL_LE_AUDIO,
                                                         EVENT_ID_LE_AUDIO_STOP_ADV);
        if (bt_status != BT_STATUS_SUCCESS) {
            APPS_LOG_MSGID_E(LOG_TAG" stop_advertising, error AWS bt_status=0x%08X", 1, bt_status);
        }
    }
#endif
}

bool app_lea_adv_mgr_update_adv_interval(uint16_t interval_min, uint16_t interval_max)
{
    uint8_t sink_state = bt_sink_srv_get_state();
    bool esco_ongoing = app_lea_adv_mgr_is_esco_ongoing();
    bool turn_down = (interval_min < app_lea_adv_interval_min && interval_max < app_lea_adv_interval_max);
    APPS_LOG_MSGID_I(LOG_TAG" update_adv_interval, adv_enabled=%d sink_state=%d esco_ongoing=%d min=%d->%d max=%d->%d",
                     7, app_lea_adv_enabled, sink_state, esco_ongoing,
                     app_lea_adv_interval_min, interval_min,
                     app_lea_adv_interval_max, interval_max);

    if (!app_lea_adv_enabled) {
        APPS_LOG_MSGID_E(LOG_TAG" update_adv_interval, not advertising", 0);
        return FALSE;
    } else if (app_lea_adv_interval_min == interval_min && app_lea_adv_interval_max == interval_max) {
        APPS_LOG_MSGID_E(LOG_TAG" update_adv_interval, same parameter", 0);
        return FALSE;
    } else if ((interval_min < app_lea_adv_interval_min && interval_max > app_lea_adv_interval_max)
               || (interval_min > app_lea_adv_interval_min && interval_max < app_lea_adv_interval_max)) {
        APPS_LOG_MSGID_E(LOG_TAG" update_adv_interval, not invalid parameter", 0);
        return FALSE;
    } else if (turn_down) {
        if (sink_state >= BT_SINK_SRV_STATE_STREAMING || esco_ongoing) {
            APPS_LOG_MSGID_E(LOG_TAG" update_adv_interval, not turn_down when Audio ongoing", 0);
            return FALSE;
        }
#ifdef MTK_AWS_MCE_ENABLE
        if (BT_AWS_MCE_SRV_LINK_NONE == bt_aws_mce_srv_get_link_type()) {
            APPS_LOG_MSGID_E(LOG_TAG" update_adv_interval, not turn_down when AWS not connected", 0);
            return FALSE;
        }
#endif
    }

    app_lea_adv_interval_min = interval_min;
    app_lea_adv_interval_max = interval_max;
    app_le_audio_do_update_advertising_param();
    APPS_LOG_MSGID_I(LOG_TAG" update_adv_interval, success min=%d max=%d",
                     2, app_lea_adv_interval_min, app_lea_adv_interval_max);
    return TRUE;
}

void app_lea_adv_mgr_get_adv_info(uint8_t *mode, bt_addr_t *target_addr, uint32_t *timeout)
{
    if (mode != NULL) {
        *mode = app_lea_adv_mode;
    }
    if (target_addr != NULL) {
        memcpy(target_addr, &app_lea_adv_target_addr[0], sizeof(bt_addr_t));
    }
    if (timeout != NULL) {
        *timeout = app_lea_adv_timeout;
    }
}

bool app_lea_adv_mgr_update_target_addr(app_le_audio_update_target_mode_t mode,
                                        bt_addr_type_t addr_type,
                                        const uint8_t *addr)
{
    return app_lea_adv_mgr_update_target_addr_imp(mode, addr_type, addr);
}

void app_lea_adv_mgr_init(void)
{

}

void app_lea_adv_mgr_proc_ui_shell_event(uint32_t event_group,
                                         uint32_t event_id,
                                         void *extra_data,
                                         size_t data_len)
{
    switch (event_group) {
        case EVENT_GROUP_UI_SHELL_BT_SINK:
            app_lea_adv_mgr_bt_sink_event_group(event_id, extra_data, data_len);
            break;
        case EVENT_GROUP_UI_SHELL_BT_DEVICE_MANAGER:
            app_lea_adv_mgr_bt_dm_event_group(event_id, extra_data, data_len);
            break;
        case EVENT_GROUP_UI_SHELL_LE_AUDIO:
            app_lea_adv_mgr_lea_event_group(event_id, extra_data, data_len);
            break;
#ifdef MTK_AWS_MCE_ENABLE
        case EVENT_GROUP_UI_SHELL_AWS_DATA:
            app_lea_adv_mgr_proc_aws_data(extra_data, data_len);
            break;
#endif
        case EVENT_GROUP_UI_SHELL_BT_CONN_MANAGER:
            app_le_audio_mgr_proc_bt_cm_group(event_id, extra_data, data_len);
            break;
    }
}

#endif  /* AIR_LE_AUDIO_ENABLE */

