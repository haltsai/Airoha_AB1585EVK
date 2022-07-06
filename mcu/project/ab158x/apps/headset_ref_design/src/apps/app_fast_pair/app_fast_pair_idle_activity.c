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

/**
 * File: app_fast_pair_idle_activity.c
 *
 * Description: This file could initialize fast pair, and receive events to update fast pair status.
 *
 * Note: See doc/Airoha_IoT_SDK_Application_Developers_Guide.pdf for fast pair APP.
 *
 */

#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"
#include "portable.h"
#include "syslog.h"
#include "bt_fast_pair.h"
#include "apps_events_event_group.h"
#include "apps_config_features_dynamic_setting.h"
#include "app_bt_state_service.h"
#ifdef AIR_MULTI_POINT_ENABLE
#include "app_bt_emp_service.h"
#endif
#include "app_fast_pair.h"
#include "app_fast_pair_nvkey_struct.h"
#include "bt_sink_srv.h"
#include "ui_shell_manager.h"
#include "bt_app_common.h"
#include "bt_device_manager_le.h"
#include "apps_debug.h"
#include "bt_init.h"
#include "bt_gap_le.h"
#include "bt_callback_manager.h"
#include "apps_events_bt_event.h"
#include "multi_ble_adv_manager.h"
#include "multi_va_event_id.h"
#include "app_bt_takeover_service.h"
#include "nvkey.h"
#include "nvdm.h"
#include "nvkey_id_list.h"
#ifdef MTK_AWS_MCE_ENABLE
#include "bt_aws_mce_report.h"
#include "bt_sink_srv_ami.h"
#include "bt_aws_mce_srv.h"
#endif
#include "bt_sink_srv_a2dp.h"
#ifdef RACE_FIND_ME_ENABLE
#include "app_fm_activity.h"
#endif
#include "apps_aws_sync_event.h"
#ifdef AIR_SMART_CHARGER_ENABLE
#include "app_smcharger_idle_activity.h"
#else
#if defined(MTK_BATTERY_MANAGEMENT_ENABLE)
#include "app_battery_transient_activity.h"
#endif
#endif
#include "apps_config_key_remapper.h"
#include "apps_events_battery_event.h"
#if defined(MTK_BATTERY_MANAGEMENT_ENABLE)
#include "battery_management.h"
#endif
#include "nvdm_id_list.h"
#include "nvkey_id_list.h"
#include "bt_customer_config.h"
#include "verno.h"
#include "atci.h"

#define LOG_TAG     "[app_fast_pair] "
#define APP_ONLY_ALLOW_TAKE_OVER_BY_FAST_PAIR       (false) /* When it's true, only allow BT take over by fast pair flow. */

#define APP_FAST_PAIR_DISCOVER_MODE_ADV_INTERVAL    (0xA0)  /* The adv interval when BT is discoverable. 0xA0 means 100ms */
#define APP_FAST_PAIR_NONDISCOVER_MODE_ADV_INTERVAL (0x190) /* The adv interval when BT is not discoverable. 0x190 means 250ms */
#define REMAINING_BATTERY_TIME_WHEN_FULL            (240)   /* The remaining battery time in minutes when battery is 100%. */

#define FAST_PAIR_PRIVATE_PROTECTED     /* If defined, the module id is encrypted and stored in NVKEY. */
#ifdef FAST_PAIR_PRIVATE_PROTECTED

#ifdef AIR_BT_FAST_PAIR_SASS_ENABLE
const static bt_bd_addr_t empty_addr = { 0 };
#endif

/* Encrypt and decrypt key, it's must as same as the "Private Key" in Page "Encryption_NvKey" of config Tool.
 * The config tool while use the same key to encrypt fast pair related nvdm. FW can use the same private key to decrypt it.
*/
const char app_fast_pair_nvdm_key[32] = {0x12, 0x34, 0x56, 0x78, 0x12, 0x34, 0x56, 0x78, \
                                         0x12, 0x34, 0x56, 0x78, 0x12, 0x34, 0x56, 0x78, \
                                         0x12, 0x34, 0x56, 0x78, 0x12, 0x34, 0x56, 0x78, \
                                         0x12, 0x34, 0x56, 0x78, 0x12, 0x34, 0x56, 0x78
                                        };
#endif

#ifdef MTK_AWS_MCE_ENABLE
#define APP_FAST_PAIR_COMPONENT_NUM                 (3)         /* 3 component means left, right and charger case */
#define APP_FAST_PAIR_AWS_CONTEXT_SYNC_ID           (0xE0)      /* The event id of event group EVENT_GROUP_UI_SHELL_BT_FAST_PAIR */
#else
#define APP_FAST_PAIR_COMPONENT_NUM                 (1)         /* 1 component means headset itself. */
#endif

/* If Fast pair related nvkey is invalid, will use below config. */
#define APP_FAST_PAIR_DEFAULT_MAX_ACCOUNT           (5)         /* Maximum google account. */
#define APP_FAST_PAIR_DEFAULT_TX_POWER_LEVEL        (-20)       /* TX Power level set in fast pair adv data. -20 means -20db. */
#define APP_FAST_PAIR_AIROHA_TEST_MODEL_ID          (0xB2DC3E)  /* Default model ID. It's useful if customer not set any model set in config tool. */
#ifdef MTK_AWS_MCE_ENABLE
#define APP_FAST_PAIR_PERSONALIZED_NAME             "Airoha_fast_pair_earbuds"      /* Default name. Because we use EDR name as default, it's not useful now. */
#else
#define APP_FAST_PAIR_PERSONALIZED_NAME             "Airoha_fast_pair_headset"      /* Default name. Because we use EDR name as default, it's not useful now. */
#endif

/* When there is not set any model id, use the default private key. */
#define APP_FAST_PAIR_AIROHA_TEST_PRIVATE_KEY       {0XDD, 0XE2, 0X5B, 0X48, 0X20, 0X8B, 0X6D, 0X34, \
                                                     0X6E, 0X42, 0X76, 0X91, 0X9E, 0X9D, 0X17, 0XC8, \
                                                     0X6C, 0X40, 0X99, 0XA0, 0XAA, 0XBF, 0X4B, 0X4F, \
                                                     0X99, 0XD7, 0X86, 0XB3, 0X0B, 0X04, 0X1B, 0X80}

#define APP_FAST_PAIR_COMPONENT_LEFT                (0) /* In fast pair spec, the battery value of left earbud is bit 0. */
#define APP_FAST_PAIR_COMPONENT_RIGHT               (1) /* In fast pair spec, the battery value of right earbud is bit 1. */
#define APP_FAST_PAIR_COMPONENT_CASE                (2) /* In fast pair spec, the battery value of charger case is bit 2. */

#ifdef AIR_BT_FAST_PAIR_SASS_ENABLE
#define FAST_PAIR_SASS_DEFAULT_ENABLED              (1)
#define FAST_PAIR_SASS_VERSION_CODE                 (0x0101)    /* Spec is 0x0101, but the format of Fast pair is big-endian. */
#ifdef AIR_MULTI_POINT_ENABLE
#define FAST_PAIR_SASS_MULTI_POINT                  (1)
#else
#define FAST_PAIR_SASS_MULTI_POINT                  (0)
#endif
#ifdef MTK_IN_EAR_FEATURE_ENABLE
#define FAST_PAIR_SASS_ON_HEAD_DETECTION            (1)
#else
#define FAST_PAIR_SASS_ON_HEAD_DETECTION            (0)
#endif
#endif /* #ifdef AIR_BT_FAST_PAIR_SASS_ENABLE */

typedef struct {
    uint8_t tone;
#ifdef MTK_AWS_MCE_ENABLE
    uint8_t peer_tone;
#endif
} app_fast_pair_find_me_state_t;

/**
 *  @brief This structure defines the format of the context of the activity.
 */
typedef struct {
    bool                            is_vis_on;          /* When it's true, smart phone should display the notification. */
    app_fast_pair_nvkey_t           nvkey;              /* The configuration in NVID_APP_FAST_PAIR_CONFIGURE, refer to app_fast_pair_nvkey_t. */
    bt_fast_pair_battery_t          battery;            /* The battery status, refer to bt_fast_pair_battery_t. */
    bt_fast_pair_account_key_list_t *account_key_list;  /* The paired google account. */
    bt_gap_le_advertising_handle_t  adv_handle;         /* The value indicate which advertising handle is for fast pair */
#if APP_ONLY_ALLOW_TAKE_OVER_BY_FAST_PAIR
    bool                            waiting_connect;    /* When it's true, means waiting EDR connection */
#endif
#ifdef AIR_TILE_ENABLE
    bool                            is_classic_bt_on;      /* When it's true, gfp can broadcast adv */
#endif
#ifdef AIR_BT_FAST_PAIR_SASS_ENABLE
    bt_bd_addr_t                    drop_target;
#ifdef AIR_MULTI_POINT_ENABLE
    bt_bd_addr_t                    allow_switch_audio_addr;        /* Record the address of last allow switch audio resource. */
    bt_bd_addr_t                    temp_allow_switch_audio_addr;   /* Record the address of device just connected BT. */
    bt_bd_addr_t                    switch_done_target_addr;        /* Record the address of target address of switch audio source done. */
    bt_fast_pair_sass_multipoint_audio_src_switch_reason_t switch_reason; /* Record the switch reason of switch audio source done. */
#endif
    uint8_t                         connection_switch_back_event;   /* record last time switch back event. */
    uint8_t                         sass_switch_prefer_flags; /* The SASS multipoint switching preference flag */
#endif
    uint32_t                        model_id;           /* The model id for fast pair. */
} app_fast_pair_local_context_t;

#define APP_FAST_PAIR_PROTECTED_CRY_LEN             (sizeof(app_fast_pair_protected_t)) /* The size to read NVkey. */

static app_fast_pair_local_context_t g_app_fast_pair_context;   /* The variable records context */

#ifdef RACE_FIND_ME_ENABLE
static app_fast_pair_find_me_state_t s_app_fast_pair_find_me_state;
#endif

#ifdef FAST_PAIR_PRIVATE_PROTECTED
/**
* @brief      This function decrypt data by a private key.
* @param[in]  p_key, the private key.
* @param[in]  p_in, the input data, it is encrypted.
* @param[in]  len, the length of p_in. Note: the buffer of the p_out should be larger than (len - 4). 4 is the size of CRC.
* @param[in]  p_out, the buffer of decrypted output data.
* @return     -1 means CRC fail, > 0 means the output length.
*/
extern int  nvkey_decrypt(unsigned char *p_key, unsigned char *p_in, int len, unsigned char *p_out);
#endif

/**
 * @brief      The ATCI cmd handler, refer to at_cmd_hdlr_fp.
 * @param[in]  parse_cmd, The value is defined in #atci_parse_cmd_param_t. This parameter is given by the ATCI
 *             parser to indicate the input command data to be transferred to the command handler.
 * @return     ATCI_STATUS_OK means success, otherwise means fail.
 */
static atci_status_t _fast_pair_test_cmd_handler(atci_parse_cmd_param_t *parse_cmd)
{
    atci_response_t response = {{0}, 0};
    response.response_flag = ATCI_RESPONSE_FLAG_APPEND_ERROR;
    char *cmd_params = parse_cmd->string_ptr + parse_cmd->name_len + 1; /* At the position behind '='. */
    switch (parse_cmd->mode) {
        case ATCI_CMD_MODE_EXECUTION: {
            if (0 == memcmp(cmd_params, "Silence_mode,", strlen("Silence_mode,"))) {
                bool enable = *(cmd_params + strlen("Silence_mode,")) == '1' ? true : false;
                bt_fast_pair_set_silence_mode(NULL, enable);
            }
            response.response_flag = ATCI_RESPONSE_FLAG_APPEND_OK;
            break;
        }
        default:
            break;
    }
    response.response_len = strlen((char *)response.response_buf);
    atci_send_response(&response);
    return ATCI_STATUS_OK;
}

/* The atci command table. */
static atci_cmd_hdlr_item_t fast_pair_test_atci_cmd[] = {
    {
        .command_head = "AT+FASTPAIRTEST",    /**< Test fast pair */
        .command_hdlr = _fast_pair_test_cmd_handler,
        .hash_value1 = 0,
        .hash_value2 = 0,
    },
};

/**
* @brief      This function read nvkey to get fast pair configuration.
* @param[in]  context, the context of the app.
* @param[in]  context, the pointer of the a fast_pair_protected data buffer.
* @return     0 means success, -1 means parameter error, -2 means read NVkey fail.
*/
static int8_t app_fast_pair_load_configure_info(app_fast_pair_local_context_t *context, app_fast_pair_protected_t *protected_set)
{
    nvkey_status_t status;
    if (NULL == context) {
        APPS_LOG_MSGID_E(LOG_TAG" wrongly parameter : 0x%x", 1, context);
        return -1;
    }

    uint32_t size = sizeof(app_fast_pair_nvkey_t);
    if (NVKEY_STATUS_OK != (status = nvkey_read_data(NVID_APP_FAST_PAIR_CONFIGURE, (uint8_t *) & (context->nvkey), &size))) {
        APPS_LOG_MSGID_E(LOG_TAG" load configure data failed : %d", 1, status);
        return -2;
    }
    APPS_LOG_MSGID_I(LOG_TAG" load configure data size %d", 1, size);
    APPS_LOG_DUMP_I(LOG_TAG" load configure data", &(context->nvkey), size);

    size = APP_FAST_PAIR_PERSONALIZED_NAME_SIZE;
    if (NVDM_STATUS_OK == nvdm_read_data_item(NVDM_GROUP_FAST_PAIR_APP, NVDM_FAST_PAIR_PERSONALIZED_NAME,
                                              (uint8_t *)context->nvkey.personalized_name, &size)) {
        APPS_LOG_MSGID_I(LOG_TAG"read name from PERSONALIZED_NAME", 0);
    }

    uint8_t *cry = (uint8_t *)pvPortMalloc(APP_FAST_PAIR_PROTECTED_CRY_LEN);
    if (cry) {
        memset(cry, 0, APP_FAST_PAIR_PROTECTED_CRY_LEN);
    }
    size = APP_FAST_PAIR_PROTECTED_CRY_LEN;
    if (NULL == cry || NVKEY_STATUS_OK != (status = nvkey_read_data(NVID_APP_FAST_PAIR_PROTECTED, cry, &size))) {
        APPS_LOG_MSGID_E(LOG_TAG" load protected data failed : %d", 1, status);
        if (cry) {
            vPortFree(cry);
        }
        return -2;
    }
    APPS_LOG_MSGID_I(LOG_TAG" load protected data size %d", 1, size);
#ifdef FAST_PAIR_PRIVATE_PROTECTED
    /* Decrypt the model set. */
    int ret = nvkey_decrypt((unsigned char *)app_fast_pair_nvdm_key, cry, APP_FAST_PAIR_PROTECTED_CRY_LEN, (unsigned char *) protected_set);
    APPS_LOG_MSGID_I(LOG_TAG" load configure decry ret %d", 1, ret);
#else
    memcpy(protected_set, cry, sizeof(app_fast_pair_protected_t));
#endif
    vPortFree(cry);
    return 0;
}

/**
* @brief      This function read nvkey to get paired accounts.
* @param[in]  context, the context of the app.
*/
static void app_fast_pair_load_account_key(app_fast_pair_local_context_t *context)
{
    nvdm_status_t status;
    uint32_t idx = 0;
    uint8_t saved_max_number = context->nvkey.max_account;
    uint32_t size = sizeof(bt_fast_pair_account_key_list_t) +
                    sizeof(bt_fast_pair_account_key_t) * saved_max_number - sizeof(bt_fast_pair_account_key_t);
    memset(context->account_key_list, 0, size);
    status = nvdm_read_data_item(NVDM_GROUP_FAST_PAIR_APP, NVDM_FAST_PAIR_ACCOUNT_KEY, (uint8_t *)(context->account_key_list), &size);
    if (NVDM_STATUS_OK != status) {
        context->account_key_list->max_key_number = context->nvkey.max_account;
        APPS_LOG_MSGID_E(LOG_TAG" account key load failed : %d", 1, status);
        return;
    }
    for (idx = 0; idx < context->account_key_list->max_key_number; idx++) {
        if (context->account_key_list->account_key_list[idx][0]) {
            APPS_LOG_DUMP_I("account key:", &context->account_key_list->account_key_list[idx][0], 16);
        }
    }
    APPS_LOG_MSGID_I(LOG_TAG" account key load num %d, need num %d", 2, context->account_key_list->max_key_number, saved_max_number);
}

/**
* @brief      This function write nvkey to update paired accounts.
* @param[in]  account_key, the array of account_key.
*/
static void app_fast_pair_store_account_key(bt_fast_pair_account_key_list_t *account_key)
{
    nvdm_status_t status;
    uint32_t size = sizeof(bt_fast_pair_account_key_list_t) +
                    sizeof(bt_fast_pair_account_key_t) * account_key->max_key_number - sizeof(bt_fast_pair_account_key_t);

    APPS_LOG_MSGID_E(LOG_TAG" Account key write nvdm size: %d", 1, size);
    status = nvdm_write_data_item(NVDM_GROUP_FAST_PAIR_APP, NVDM_FAST_PAIR_ACCOUNT_KEY, NVDM_DATA_ITEM_TYPE_RAW_DATA,
                                  (uint8_t *)(account_key), size);
    if (NVDM_STATUS_OK != status) {
        APPS_LOG_MSGID_E(LOG_TAG" Account key write nvdm failed : %x", 1, status);
        return;
    }
}

/**
* @brief      This function is the callback function to implement, refer to get_ble_adv_data_func_t.
* @param[in]  adv_data, a structure need to fill data in the callback function.
*/
static uint32_t app_fast_pair_get_adv_data(multi_ble_adv_info_t *adv_data)
{
    bt_fast_pair_advertising_data_t type;
    bt_fast_pair_status_t ret;
    uint16_t interval;

    if (g_app_fast_pair_context.is_vis_on) {
        type = BT_FAST_PAIR_ADVERTISING_DATA_MODEL_ID;
        interval = APP_FAST_PAIR_DISCOVER_MODE_ADV_INTERVAL;
    } else {
        type = BT_FAST_PAIR_ADVERTISING_DATA_ACCOUNT_AND_BATTERY;
        if (g_app_fast_pair_context.battery.ui_show) {
            interval = APP_FAST_PAIR_NONDISCOVER_MODE_ADV_INTERVAL;
        } else {
            interval = APP_FAST_PAIR_DISCOVER_MODE_ADV_INTERVAL;
        }
    }

    if (adv_data->adv_param) {
        bt_app_common_generate_default_adv_data(adv_data->adv_param, NULL, NULL, NULL, 0);
        adv_data->adv_param->primary_advertising_interval_min = interval;
        /* Interval should be no larger than 100ms when discoverable */
        adv_data->adv_param->primary_advertising_interval_max = interval;
        bt_bd_addr_t peer_addr[1] = {
            {0, 0, 0, 0, 0, 0}
        };
        uint8_t count = 1;
        bt_device_manager_le_get_bonded_list(peer_addr, &count);
        if (0 == count) {
            bt_gap_le_bonding_info_t bond_info;
            bt_app_common_generate_random_address(peer_addr[0]);
            memset(&bond_info, 0, sizeof(bond_info));
            bond_info.identity_addr.address.type = BT_ADDR_RANDOM;
            memcpy(bond_info.identity_addr.address.addr, peer_addr, sizeof(peer_addr));
            bond_info.key_security_mode = BT_GAP_LE_SECURITY_BONDED_MASK;
            memcpy(bond_info.local_key.identity_info.irk, bt_app_common_get_ble_local_irk(), sizeof(bond_info.local_key.identity_info.irk));
            memset(&bond_info.identity_info.irk, 0xFF, sizeof(bond_info.identity_info.irk));
            bt_device_manager_le_set_bonding_info_by_addr(&(bond_info.identity_addr.address), &bond_info);
            adv_data->adv_param->peer_address = bond_info.identity_addr.address;
            adv_data->adv_param->own_address_type = BT_ADDR_RANDOM_IDENTITY;
            APPS_LOG_MSGID_I(LOG_TAG" create RPA address %02X:%02X:%02X:%02X:%02X:%02X", 6,
                             bond_info.identity_addr.address.addr[0],
                             bond_info.identity_addr.address.addr[1],
                             bond_info.identity_addr.address.addr[2],
                             bond_info.identity_addr.address.addr[3],
                             bond_info.identity_addr.address.addr[4],
                             bond_info.identity_addr.address.addr[5]);
        } else {
            bt_gap_le_bonding_info_t *bonded_info = NULL;
            bonded_info = bt_device_manager_le_get_bonding_info_by_addr(&peer_addr[0]);
            if (bonded_info != NULL) {
                adv_data->adv_param->peer_address = bonded_info->identity_addr.address;
                adv_data->adv_param->own_address_type = BT_ADDR_RANDOM_IDENTITY;
                APPS_LOG_MSGID_I(LOG_TAG" use RPA address %02X:%02X:%02X:%02X:%02X:%02X", 6,
                                 bonded_info->identity_addr.address.addr[0],
                                 bonded_info->identity_addr.address.addr[1],
                                 bonded_info->identity_addr.address.addr[2],
                                 bonded_info->identity_addr.address.addr[3],
                                 bonded_info->identity_addr.address.addr[4],
                                 bonded_info->identity_addr.address.addr[5]);
            }
        }
    }

    if (adv_data->adv_data) {
        uint8_t data_len = adv_data->adv_data->data_length > 0xFF ? 0xFF : adv_data->adv_data->data_length;
        /* Call API in middleware to get adv data. */
        ret = bt_fast_pair_get_advertising_data(adv_data->adv_data->data, &data_len, type);
        if (ret != BT_FAST_PAIR_STATUS_SUCCESS) {
            APPS_LOG_MSGID_E(LOG_TAG" adv get data failed %d", 1, ret);
            return 0;
        } else {
            adv_data->adv_data->data_length = data_len;
        }
    }
    if (adv_data->scan_rsp) {
        adv_data->scan_rsp->data_length = 0;
    }

    return 0;
}

/**
* @brief      This function update the fast pair adv.
* @param[in]  local_context, the context of the app.
*/
static void app_fast_pair_trigger_advertising(app_fast_pair_local_context_t *local_context)
{
#ifdef AIR_TILE_ENABLE
    if (!g_app_fast_pair_context.is_classic_bt_on) {
        APPS_LOG_MSGID_I(LOG_TAG"check classic bt on status before trigger adv %d", 1, g_app_fast_pair_context.is_classic_bt_on);
        return;
    }
#endif
    /* For re-start ADV if connected by unexpected devices */
    APPS_LOG_MSGID_I(LOG_TAG" Trigger advertising visibility status: %d", 1, g_app_fast_pair_context.is_vis_on);

    if (apps_config_features_is_mp_test_mode()) {
        return;
    }
#if 0
    if (bt_sink_srv_get_connected_devices_number() >= 1) {
        /* Once connected one ACL link, then don't start ADV to avoid
         * conflict with other BLE module. */
        APPS_LOG_MSGID_W("[APP_FAST_PAIR]ACL link already exited: %d", 1, is_vis_on);
        return;
    }
#endif
    multi_ble_adv_manager_remove_ble_adv(MULTI_ADV_INSTANCE_FAST_PAIR, app_fast_pair_get_adv_data);
    multi_ble_adv_manager_add_ble_adv(MULTI_ADV_INSTANCE_FAST_PAIR, app_fast_pair_get_adv_data, 1);
    multi_ble_adv_manager_notify_ble_adv_data_changed(MULTI_ADV_INSTANCE_FAST_PAIR);
}

#ifdef AIR_SPOT_ENABLE
static uint32_t app_fast_pair_spot_get_adv_data(multi_ble_adv_info_t *adv_data)
{
    uint16_t interval;

    if (g_app_fast_pair_context.is_vis_on) {
        interval = APP_FAST_PAIR_DISCOVER_MODE_ADV_INTERVAL;
    } else {
        interval = APP_FAST_PAIR_NONDISCOVER_MODE_ADV_INTERVAL;
    }

    if (adv_data->adv_param) {
        bt_app_common_generate_default_adv_data(adv_data->adv_param, NULL, NULL, NULL, 0);
        adv_data->adv_param->primary_advertising_interval_min = interval;
        /* Interval should be no larger than 100ms when discoverable */
        adv_data->adv_param->primary_advertising_interval_max = interval;
    }

    if (adv_data->adv_data != NULL) {
        uint8_t *buf = adv_data->adv_data->data;
        uint16_t *len = &adv_data->adv_data->data_length;

        bool ret = bt_fast_pair_get_spot_adv_data(FALSE, buf, len);
        if (!ret) {
            APPS_LOG_MSGID_E(LOG_TAG"[SPOT] gen adv data failed", 1, 0);
            adv_data->adv_data->data_length = 0;
        }

        APPS_LOG_MSGID_I(LOG_TAG"[SPOT] get adv data, data_length=%d eid=%02X:%02X:%02X:%02X:%02X",
                         6, adv_data->adv_data->data_length, buf[8], buf[9], buf[10], buf[11], buf[12]);
    }
    if (adv_data->scan_rsp) {
        adv_data->scan_rsp->data_length = 0;
    }

    return 0;
}

static void app_fast_pair_spot_adv_callback(bt_fast_pair_spot_adv_event event)
{
    APPS_LOG_MSGID_I(LOG_TAG"[SPOT] adv_callback, event=0x%04X", 1, event);
    // Need to switch to APP task
    ui_shell_send_event(false, EVENT_PRIORITY_MIDDLE, EVENT_GROUP_UI_SHELL_BT_FAST_PAIR,
                        event, NULL, 0, NULL, 0);
}

static void app_fast_pair_spot_enable_advertising(bool enable)
{
    APPS_LOG_MSGID_I(LOG_TAG"[SPOT] enable_advertising, enable=%d", 1, enable);
    if (enable) {
        multi_ble_adv_manager_remove_ble_adv(MULTI_ADV_INSTANCE_FAST_PAIR, app_fast_pair_spot_get_adv_data);
        multi_ble_adv_manager_add_ble_adv(MULTI_ADV_INSTANCE_FAST_PAIR, app_fast_pair_spot_get_adv_data, 1);
        multi_ble_adv_manager_notify_ble_adv_data_changed(MULTI_ADV_INSTANCE_FAST_PAIR);

#ifdef MTK_AWS_MCE_ENABLE
        if (bt_connection_manager_device_local_info_get_aws_role() != BT_AWS_MCE_ROLE_AGENT) {
            return;
        }
        uint32_t size = 32;
        uint8_t buf[32] = {0};
        nvkey_status_t sta = nvkey_read_data(NVID_APP_FPS_SPOT_EIK, buf, &size);
        apps_aws_sync_event_send_extra(EVENT_GROUP_UI_SHELL_BT_FAST_PAIR,
                                       BT_FAST_PAIR_SPOT_EVENT_START_ADV,
                                       buf, size);
        APPS_LOG_MSGID_I(LOG_TAG"[SPOT] sync eph key to partner, len=%d, nv_sta=%d", 2, size, sta);
#endif
    } else {
        multi_ble_adv_manager_remove_ble_adv(MULTI_ADV_INSTANCE_FAST_PAIR, app_fast_pair_spot_get_adv_data);
        multi_ble_adv_manager_notify_ble_adv_data_changed(MULTI_ADV_INSTANCE_FAST_PAIR);
#ifdef MTK_AWS_MCE_ENABLE
        if (bt_connection_manager_device_local_info_get_aws_role() != BT_AWS_MCE_ROLE_AGENT) {
            return;
        }
        apps_aws_sync_event_send_extra(EVENT_GROUP_UI_SHELL_BT_FAST_PAIR, BT_FAST_PAIR_SPOT_EVENT_STOP_ADV, NULL, 0);
#endif
    }
}

uint32_t bt_fast_pair_spot_get_eid_clock_callback()
{
    /* Notice!!!
     * The customer should implement this function! If your implement support RTC timer, return the RTC timer
     * with seconds. And your RTC timer must support power loss recover. It's means you should backup the
     * RTC timer to the NVDM every a few hours. If your device power loss and the RTC timer cleared, you should
     * use the value in the NVDM to recover it.
     * If you implement it by the system clock, you should keep the clock is count-up. It's means that you
     * should add the system clock to a base counter, you could set the base counter when the product leaves
     * the factory. And storage the sum of base counter and current system clock to the NVDM every time the
     * system power off and read it as the new base counter when the system boot up again.
     */
    static uint32_t last_reboot_value = 0;
    static uint32_t last_value = 0;
    if (last_reboot_value == 0) {
        uint32_t size = sizeof(uint32_t);
        nvkey_status_t sta = nvkey_read_data(NVID_APP_FPS_SPOT_EID_COUNTER, (uint8_t *)&last_reboot_value, &size);
        if (sta != NVKEY_STATUS_OK) {
            last_reboot_value = 1; /* to void reading again. */
        }
        last_value = last_reboot_value;
    }

    uint32_t new_value = last_reboot_value + xTaskGetTickCount() / portTICK_PERIOD_MS / 1000;
    /* update every 5hours */
    if (new_value - last_value >= 60 * 60 * 5) {
        nvkey_write_data(NVID_APP_FPS_SPOT_EID_COUNTER, (uint8_t *)&new_value, sizeof(uint32_t));
        last_value = new_value;
    }
    return new_value;
}


static void app_fast_pair_spot_find_me(bool l_ring, bool r_ring, uint32_t ring_time)
{
    APPS_LOG_MSGID_I(LOG_TAG" app_fast_pair_spot_find_me R:%d, L:%d, duration:%d",
                     3, r_ring, l_ring, ring_time);
#ifdef RACE_FIND_ME_ENABLE
    app_find_me_param_struct *find_self_param = (app_find_me_param_struct *)pvPortMalloc(sizeof(app_find_me_param_struct));
    if (find_self_param == NULL) {
        APPS_LOG_MSGID_I(LOG_TAG" app_fast_pair_spot_find_me malloc fail", 0);
        return;
    }
    find_self_param->blink = 0;
    find_self_param->duration_seconds = ring_time;
#ifdef MTK_AWS_MCE_ENABLE
    app_find_me_param_struct peer_param = {
        .blink = 0,
        .duration_seconds = ring_time
    };
    /* Because in the fast pair spec, the component bit define of ring request is different from the bit define of battery value,
       must convert the left and right. */
    uint8_t battery_item = (AUDIO_CHANNEL_R == ami_get_audio_channel() ? APP_FAST_PAIR_COMPONENT_RIGHT : APP_FAST_PAIR_COMPONENT_LEFT);
    if (APP_FAST_PAIR_COMPONENT_RIGHT == battery_item) {
        find_self_param->tone = l_ring;
        peer_param.tone = r_ring;
    } else {
        find_self_param->tone = r_ring;
        peer_param.tone = l_ring;
    }

    apps_aws_sync_event_send_extra(EVENT_GROUP_UI_SHELL_APP_INTERACTION,
                                   APPS_EVENTS_INTERACTION_FIND_ME_APP_SYNC_FIND_ME_ACTION, &peer_param, sizeof(peer_param));
#else
    find_self_param->tone = l_ring;
#endif
    ui_shell_send_event(false, EVENT_PRIORITY_MIDDLE, EVENT_GROUP_UI_SHELL_FINDME,
                        APP_FIND_ME_EVENT_ID_TRIGGER, find_self_param, sizeof(app_find_me_param_struct), NULL, 0);
#endif
}

static void app_fast_spot_stop_find_me()
{
#ifdef RACE_FIND_ME_ENABLE
    app_find_me_param_struct *find_self_param = (app_find_me_param_struct *)pvPortMalloc(sizeof(app_find_me_param_struct));
    if (find_self_param) {
        find_self_param->blink = 0;
        find_self_param->duration_seconds = 0;
        find_self_param->tone = 0;
#ifdef MTK_AWS_MCE_ENABLE
        apps_aws_sync_event_send_extra(EVENT_GROUP_UI_SHELL_APP_INTERACTION,
                                       APPS_EVENTS_INTERACTION_FIND_ME_APP_SYNC_FIND_ME_ACTION, find_self_param, sizeof(app_find_me_param_struct));
#endif
        ui_shell_send_event(false, EVENT_PRIORITY_MIDDLE, EVENT_GROUP_UI_SHELL_FINDME,
                            APP_FIND_ME_EVENT_ID_TRIGGER, find_self_param, sizeof(app_find_me_param_struct), NULL, 0);
    }
#else
    APPS_LOG_MSGID_E(LOG_TAG"FIND ME NOT ENABLED!", 0);
#endif
}

static void app_fast_pair_ring_request_handle(bt_fast_pair_ring_request_t *ring);
/* !Notice: this function just for reference, please implement it by your ring design. */
bt_fast_pair_status_t bt_fast_pair_spot_ring_callback(bt_fast_pair_spot_ring_req_type_t req_type, uint8_t *data, uint16_t *remain_time)
{
    static bt_fast_pair_spot_ring_type_t s_current_ring_type = BT_FAST_PAIR_SPOT_RING_TYPE_NONE;
    static uint16_t s_remain_time = 0;

    APPS_LOG_MSGID_I(LOG_TAG"[SPOT] ring req, %d, data: %d, remain_time: %d", 3, req_type, *data, *remain_time);
    switch (req_type) {
        case BT_FAST_PAIR_SPOT_RING_REQ_RING:
            /* Ring req, the *data is ring type */
            s_current_ring_type = *data;
            /* Notice! the time is deciseconds. */
            s_remain_time = ((*remain_time << 8) & 0xFF00) | ((*remain_time >> 8) & 0xFF);
            if (s_current_ring_type == BT_FAST_PAIR_SPOT_RING_TYPE_NONE) {
                /* BT_FAST_PAIR_SPOT_RING_TYPE_NONE means stop ring. */
                s_remain_time = 0;
                /* Every time ringing stops, must call bt_fast_pair_spot_ring_state_update to udpate state.
                 * Because the ringing is stopped by SPOT req, so the state should be BT_FAST_PAIR_SPOT_RING_STOPPED_BY_GATT.
                 * If the ringing is stopped by other reasons, must call this api too.
                 */
                bt_fast_pair_spot_ring_state_update(BT_FAST_PAIR_SPOT_RING_STOPPED_BY_GATT, s_current_ring_type, 0);
                app_fast_spot_stop_find_me();
            } else {
#ifdef AIR_RACE_FIND_ME_ENABLE
                ui_shell_remove_event(EVENT_GROUP_UI_SHELL_FINDME, APP_FIND_ME_EVENT_ID_TRIGGER);
#endif
                app_fast_pair_spot_find_me(true, true, s_remain_time / 10);
            }
            /* Every time ringing starts, must call bt_fast_pair_spot_ring_state_update to udpate state. */
            bt_fast_pair_spot_ring_state_update(BT_FAST_PAIR_SPOT_RING_STA_STARTED, s_current_ring_type, s_remain_time);
            break;
        case BT_FAST_PAIR_SPOT_RING_REQ_GET_RING_CAPABLE:
            /* Get the ring capable. */
            *data = BT_FAST_PAIR_SPOT_RING_CAPABLE_SINGLE;
            break;
        case BT_FAST_PAIR_SPOT_RING_REQ_GET_RING_TYPE:
            /* Get the current ring type and remain time, if current not in ringing, the s_remain_time must be zero. */
            *data = s_current_ring_type;
            *remain_time = s_remain_time;
            break;
    }
    return BT_FAST_PAIR_STATUS_SUCCESS;
}
#endif

/**
* @brief      This function update the battery information.
* @param[in]  context, the context of the app.
* @param[in]  battery, the new battery information.
*/
static void app_fast_pair_update_battery(app_fast_pair_local_context_t *context, bt_fast_pair_battery_t *battery)
{
    if (NULL == context || NULL == battery) {
        return;
    }
    bool need_update = false;
    uint8_t lowest_battery_value = 0xFF; /* The device which is not charging and battery level is lowest. */
    bt_fast_pair_battery_t *cur_cntx = &(context->battery);

    /* When data changed */
    if (cur_cntx->ui_show != battery->ui_show ||
        cur_cntx->battery[APP_FAST_PAIR_COMPONENT_RIGHT].charging != battery->battery[APP_FAST_PAIR_COMPONENT_RIGHT].charging ||
        cur_cntx->battery[APP_FAST_PAIR_COMPONENT_LEFT].charging != battery->battery[APP_FAST_PAIR_COMPONENT_LEFT].charging ||
        cur_cntx->battery[APP_FAST_PAIR_COMPONENT_CASE].charging != battery->battery[APP_FAST_PAIR_COMPONENT_CASE].charging ||
        cur_cntx->battery[APP_FAST_PAIR_COMPONENT_RIGHT].battery_value != battery->battery[APP_FAST_PAIR_COMPONENT_RIGHT].battery_value ||
        cur_cntx->battery[APP_FAST_PAIR_COMPONENT_LEFT].battery_value != battery->battery[APP_FAST_PAIR_COMPONENT_LEFT].battery_value ||
        cur_cntx->battery[APP_FAST_PAIR_COMPONENT_CASE].battery_value != battery->battery[APP_FAST_PAIR_COMPONENT_CASE].battery_value) {
        need_update = true;
        if (0 == battery->battery[APP_FAST_PAIR_COMPONENT_RIGHT].charging) {
            lowest_battery_value = battery->battery[APP_FAST_PAIR_COMPONENT_RIGHT].battery_value;
        }
        if (0 == battery->battery[APP_FAST_PAIR_COMPONENT_LEFT].charging && lowest_battery_value > battery->battery[APP_FAST_PAIR_COMPONENT_LEFT].battery_value) {
            lowest_battery_value = battery->battery[APP_FAST_PAIR_COMPONENT_LEFT].battery_value;
        }
        if (lowest_battery_value != 0xFF) {
            battery->remaining_time = REMAINING_BATTERY_TIME_WHEN_FULL * lowest_battery_value / 100;
        } else {
            battery->remaining_time = BT_FAST_PAIR_REMAINING_TIME_INVALID_VALUE;
        }
    }
    if (true == need_update) {
        context->battery = *battery;
        /* Call middleware API to updata battery information. */
        bt_fast_pair_update_battery(cur_cntx);
        /* Because the battery information is contained in adv data, must update adv once. */
        app_fast_pair_trigger_advertising(context);
    }
}

#ifdef RACE_FIND_ME_ENABLE
static void app_fast_pair_notify_ring_state_change(void)
{
    uint8_t message_buffer[sizeof(bt_fast_pair_message_stream_t)];
    bt_fast_pair_message_stream_t *cont = (bt_fast_pair_message_stream_t *)&message_buffer;
    cont->group_ID = BT_FAST_PAIR_MESSAGE_GROUP_DEVICE_ACTION_EVENT;
    cont->code_ID = BT_FAST_PAIR_MESSAGE_CODE_DEVICE_ACTION_EVENT_RING;
    cont->data_length = 0x01;
    cont->data[0] = 0;
#ifdef MTK_AWS_MCE_ENABLE
    bt_aws_mce_role_t aws_role = bt_connection_manager_device_local_info_get_aws_role();
    if ((BT_AWS_MCE_ROLE_PARTNER | BT_AWS_MCE_ROLE_CLINET) & aws_role) {
        return;
    }
    if (AUDIO_CHANNEL_R == ami_get_audio_channel()) {
        cont->data[0] = (s_app_fast_pair_find_me_state.tone & 0x1) | ((s_app_fast_pair_find_me_state.peer_tone & 0x1) << 1);
    } else {
        cont->data[0] = (s_app_fast_pair_find_me_state.peer_tone & 0x1) | ((s_app_fast_pair_find_me_state.tone & 0x1) << 1);
    }
#else
    cont->data[0] = s_app_fast_pair_find_me_state.tone & 0x1;
#endif
    APPS_LOG_MSGID_I(LOG_TAG" Ringing state change : %x.", 1, cont->data[0]);
    bt_fast_pair_send_message_stream(NULL, cont);
}
#endif

#ifdef AIR_SMART_CHARGER_ENABLE
/**
* @brief      This function send event to stop find me.
*/
static void app_fast_pair_stop_find_me(void)
{
#ifdef RACE_FIND_ME_ENABLE
    app_find_me_param_struct *find_self_param = (app_find_me_param_struct *)pvPortMalloc(sizeof(app_find_me_param_struct));
    if (find_self_param) {
        find_self_param->blink = 0;
        find_self_param->duration_seconds = 0;
        find_self_param->tone = 0;
        ui_shell_send_event(false, EVENT_PRIORITY_MIDDLE, EVENT_GROUP_UI_SHELL_FINDME,
                            APP_FIND_ME_EVENT_ID_TRIGGER, find_self_param, sizeof(app_find_me_param_struct), NULL, 0);
    }
#endif
}
#endif

/**
* @brief      This function process the fast pair ring request from smart phone.
* @param[in]  ring, the structure of ring request.
*/
static void app_fast_pair_ring_request_handle(bt_fast_pair_ring_request_t *ring)
{
    APPS_LOG_MSGID_I(LOG_TAG" Recived ring request from remote device R:%d, L:%d, duration:%d",
                     3, ring->ring_status[0], ring->ring_status[1], ring->ring_duration);
#ifdef RACE_FIND_ME_ENABLE
    app_find_me_param_struct *find_self_param = (app_find_me_param_struct *)pvPortMalloc(sizeof(app_find_me_param_struct));
    if (ring->ring_duration == 0xFF) {
        ring->ring_duration = 0;
    }
    if (NULL == find_self_param) {
        APPS_LOG_MSGID_E(LOG_TAG" Allocate ring req buffer fail", 0);
        return;
    }
    find_self_param->blink = 0;
    find_self_param->duration_seconds = ring->ring_duration;
#ifdef MTK_AWS_MCE_ENABLE
    app_find_me_param_struct peer_param = {
        .blink = 0,
        .duration_seconds = ring->ring_duration
    };
    /* Because in the fast pair spec, the component bit define of ring request is different from the bit define of battery value,
       must convert the left and right. */
    uint8_t battery_item = (AUDIO_CHANNEL_R == ami_get_audio_channel() ? APP_FAST_PAIR_COMPONENT_RIGHT : APP_FAST_PAIR_COMPONENT_LEFT);
    if (APP_FAST_PAIR_COMPONENT_RIGHT == battery_item) {
        find_self_param->tone = ring->ring_status[APP_FAST_PAIR_COMPONENT_LEFT];
        peer_param.tone = ring->ring_status[APP_FAST_PAIR_COMPONENT_RIGHT];
    } else {
        find_self_param->tone = ring->ring_status[APP_FAST_PAIR_COMPONENT_RIGHT];
        peer_param.tone = ring->ring_status[APP_FAST_PAIR_COMPONENT_LEFT];
    }
    s_app_fast_pair_find_me_state.peer_tone = peer_param.tone;
    apps_aws_sync_event_send_extra(EVENT_GROUP_UI_SHELL_BT_FAST_PAIR,
                                   BT_FAST_PAIR_APP_EVENT_RING_REQUEST, &peer_param, sizeof(peer_param));
#else
    find_self_param->tone = ring->ring_status[0];
#endif
    s_app_fast_pair_find_me_state.tone = find_self_param->tone;
    ui_shell_send_event(false, EVENT_PRIORITY_MIDDLE, EVENT_GROUP_UI_SHELL_FINDME,
                        APP_FIND_ME_EVENT_ID_TRIGGER, find_self_param, sizeof(app_find_me_param_struct), NULL, 0);
#endif
}

/**
* @brief      This function process additional data from smart phone.
* @param[in]  local_context, context, the context of the app.
* @param[in]  data, the content of the additional data.
*/
static void app_fast_pair_additional_data_handle(app_fast_pair_local_context_t *local_context, bt_fast_pair_additional_data_t *data, uint32_t data_len)
{
    nvdm_status_t status = 0;
    APPS_LOG_MSGID_I(LOG_TAG" Additional data recived data_id %d, data_length %d.", 2, data->data_id, data->data_length);
    /* If the additional data is changing personalized name. */
    if (BT_FAST_PAIR_ADDITIONAL_DATA_ID_PERSONALIZED_NAME == data->data_id) {
        if (data->data_length > sizeof(g_app_fast_pair_context.nvkey.personalized_name) - 1) {
            data->data_length = sizeof(g_app_fast_pair_context.nvkey.personalized_name) - 1;
        }
        memcpy(g_app_fast_pair_context.nvkey.personalized_name, data->data, data->data_length);
        g_app_fast_pair_context.nvkey.personalized_name[data->data_length] = 0;
        if (NVDM_STATUS_OK != (status = nvdm_write_data_item(NVDM_GROUP_FAST_PAIR_APP, NVDM_FAST_PAIR_PERSONALIZED_NAME,
                                                             NVDM_DATA_ITEM_TYPE_RAW_DATA,
                                                             (uint8_t *) & (g_app_fast_pair_context.nvkey.personalized_name),
                                                             APP_FAST_PAIR_PERSONALIZED_NAME_SIZE))) {
            APPS_LOG_MSGID_E(LOG_TAG" PERSONALIZED_NAME write failed : 0x%x", 1, status);
        }
    }
#ifdef MTK_AWS_MCE_ENABLE
    apps_aws_sync_event_send_extra(EVENT_GROUP_UI_SHELL_BT_FAST_PAIR,
                                   BT_FAST_PAIR_APP_EVENT_ADDITIONAL_DATA, data, data_len);
#endif
}

#ifdef AIR_BT_FAST_PAIR_SASS_ENABLE
static bool app_fast_pair_app_event_proc_sass_msg(bt_bd_addr_t *addr, bt_fast_pair_message_stream_t *msg_data)
{
    bool need_send_ack = false;
    bt_fast_pair_acknowledgements_t ack = {
        .action = BT_FAST_PAIR_MESSAGE_CODE_ACKNOWLEDGEMENT_ACK,
        .groupID = BT_FAST_PAIR_MESSAGE_GROUP_SMART_AUDIO_SOURCE_SWITCHING,
        .codeID = msg_data->code_ID,
    };
    switch (msg_data->code_ID) {
        case BT_FAST_PAIR_MESSAGE_CODE_SASS_GET_CAPABILITY: {
            /* Processed by middleare. */
            break;
        }
        case BT_FAST_PAIR_MESSAGE_CODE_SASS_NOTIFY_CAPABILITY: {
            /* No use. */
            break;
        }
        case BT_FAST_PAIR_MESSAGE_CODE_SASS_SET_MULTIPOINT_STATE: {
            /* No Support. */
            break;
        }
        case BT_FAST_PAIR_MESSAGE_CODE_SASS_SET_SWITCHING_PREFERENCE: {
            bt_fast_pair_sass_set_switching_preference_data_t *set_preference = (bt_fast_pair_sass_set_switching_preference_data_t *) & (msg_data->data);
            APPS_LOG_MSGID_I(LOG_TAG" SASS_SET_SWITCHING_PREFERENCE: 0x%x -> 0x%x", 2, g_app_fast_pair_context.sass_switch_prefer_flags, set_preference->switching_preference_flags);
            g_app_fast_pair_context.sass_switch_prefer_flags = set_preference->switching_preference_flags;
            bt_fast_pair_sass_notify_switching_preference_data_t notify_preference = {
                .switching_preference_flags = set_preference->switching_preference_flags,
                .advanced_switching_settings = set_preference->advanced_switching_settings,
            };
            bt_fast_pair_sass_update_multipoint_switching_preference(&notify_preference, false);
            break;
        }
        case BT_FAST_PAIR_MESSAGE_CODE_SASS_GET_SWITCHING_PREFERENCE: {
            /* Processed by middleare. */
            break;
        }
        case BT_FAST_PAIR_MESSAGE_CODE_SASS_NOTIFY_SWITCHING_PREFERENCE: {
            /* No use. */
            break;
        }
#ifdef AIR_MULTI_POINT_ENABLE
        case BT_FAST_PAIR_MESSAGE_CODE_SASS_SWITCH_ACTIVE_AUDIO_SOURCE: {
            if (addr == NULL || msg_data == NULL) {
                break;
            }
            need_send_ack = true;
            bt_fast_pair_switch_active_audio_source_bit_t switch_audio_flags = *(bt_fast_pair_switch_active_audio_source_bit_t *) & (msg_data->data);
            APPS_LOG_MSGID_I(LOG_TAG" BT_FAST_PAIR_MESSAGE_CODE_SASS_SWITCH_ACTIVE_AUDIO_SOURCE flag 0x%X, addr:[%02X:%02X:%02X:%02X:%02X:%02X]", 7,
                             switch_audio_flags,
                             (*addr)[0], (*addr)[1], (*addr)[2], (*addr)[3], (*addr)[4], (*addr)[5]);
            bt_bd_addr_t connected_devices[3];
            uint32_t connected_devices_count = 3;
            connected_devices_count = app_bt_state_service_get_connected_disinclude_aws(connected_devices, connected_devices_count);
            bt_sink_srv_device_state_t device_state;
            bt_status_t bt_ret = bt_sink_srv_get_playing_device_state(&device_state);
            bt_bd_addr_t *switch_to_addr = NULL;
            bt_bd_addr_t *switch_away_addr = NULL;
            bt_bd_addr_t *the_other_src_addr = NULL;

            for (uint32_t i = 0; i < connected_devices_count; i++) {
                if (memcmp(connected_devices[i], *addr, sizeof(bt_bd_addr_t)) != 0) {
                    the_other_src_addr = &connected_devices[i];
                    break;
                }
            }

            if ((1 << BT_FAST_PAIR_SASS_SWITCH_ACTIVE_AUDIO_SOURCE_SWITCH_TO_THIS_BIT) == switch_audio_flags) {
                APPS_LOG_MSGID_I(LOG_TAG" switch to this, means accept switch audio source", 0);
                memcpy(g_app_fast_pair_context.allow_switch_audio_addr, *addr, sizeof(bt_bd_addr_t));
                bt_sink_srv_a2dp_int_user_conf_set(1, *addr);
                ui_shell_remove_event(EVENT_GROUP_UI_SHELL_APP_INTERACTION,
                                      APPS_EVENTS_INTERACTION_FAST_PAIR_CLEAR_ACTIVE_SWITCH_RECORD);
                ui_shell_send_event(false, EVENT_PRIORITY_MIDDLE,
                                    EVENT_GROUP_UI_SHELL_APP_INTERACTION,
                                    APPS_EVENTS_INTERACTION_FAST_PAIR_CLEAR_ACTIVE_SWITCH_RECORD,
                                    NULL, 0, NULL, 1000);

                break;
            }
            if ((1 << BT_FAST_PAIR_SASS_SWITCH_ACTIVE_AUDIO_SOURCE_SWITCH_TO_THIS_BIT) & switch_audio_flags) {
                switch_to_addr = addr;
                switch_away_addr = the_other_src_addr;
            } else {
                switch_to_addr = the_other_src_addr;
                switch_away_addr = addr;
            }

            if (BT_STATUS_SUCCESS == bt_ret) {
                if (memcmp(switch_to_addr, device_state.address, sizeof(bt_bd_addr_t)) == 0) {
                    /* Already playing. */
                    APPS_LOG_MSGID_I(LOG_TAG" already playing on addr [%02X:%02X:%02X:%02X:%02X:%02X]", 6,
                             (*switch_to_addr)[0], (*switch_to_addr)[1],
                             (*switch_to_addr)[2], (*switch_to_addr)[3],
                             (*switch_to_addr)[4], (*switch_to_addr)[5]);
                    ack.action = BT_FAST_PAIR_MESSAGE_CODE_ACKNOWLEDGEMENT_NAK;
                    ack.reason = BT_FAST_PAIR_ACKNOWLEDGEMENT_NAK_REASON_REDUNDANT_DEVICE_ACTION;
                } else {
                    if ((1 << BT_FAST_PAIR_SASS_SWITCH_ACTIVE_AUDIO_SOURCE_REJECT_SCO) & switch_audio_flags) {
                        if (BT_SINK_SRV_STATE_NONE != device_state.call_state) {
                            bt_sink_srv_send_action(BT_SINK_SRV_ACTION_SWITCH_AUDIO_PATH, switch_away_addr);
                        }
                    }
                    if ((1 << BT_FAST_PAIR_SASS_SWITCH_ACTIVE_AUDIO_SOURCE_RESUME_PLAYING_BIT) & switch_audio_flags) {
                        bt_sink_srv_send_action(BT_SINK_SRV_ACTION_PLAY, switch_to_addr);
                    }
                }
            }
            APPS_LOG_MSGID_I(LOG_TAG" switch to play on addr [%02X:%02X:%02X:%02X:%02X:%02X]", 6,
                             (*switch_to_addr)[0], (*switch_to_addr)[1],
                             (*switch_to_addr)[2], (*switch_to_addr)[3],
                             (*switch_to_addr)[4], (*switch_to_addr)[5]);

            if ((1 << BT_FAST_PAIR_SASS_SWITCH_ACTIVE_AUDIO_SOURCE_DISCONNECT) & switch_audio_flags) {
                bt_cm_connect_t connect_param = {
                    .profile = BT_CM_PROFILE_SERVICE_MASK_ALL,
                };
                memcpy(connect_param.address, *switch_away_addr, sizeof(bt_bd_addr_t));
                APPS_LOG_MSGID_I(LOG_TAG" disconnect addr [%02X:%02X:%02X:%02X:%02X:%02X]", 6,
                                 (*switch_away_addr)[0], (*switch_away_addr)[1],
                                 (*switch_away_addr)[2], (*switch_away_addr)[3],
                                 (*switch_away_addr)[4], (*switch_away_addr)[5]);
                bt_cm_disconnect(&connect_param);
            }
            break;
        }
#endif
        case BT_FAST_PAIR_MESSAGE_CODE_SASS_SWITCH_BACK: {
            uint8_t switch_back_event = msg_data->data[0];
            bt_bd_addr_t *takeover_addr = app_bt_takeover_service_get_last_takeover_device();
            APPS_LOG_MSGID_I(LOG_TAG" BT_FAST_PAIR_MESSAGE_CODE_SASS_SWITCH_BACK disconnected_record:[%02X:%02X:%02X:%02X:%02X:%02X].", 6,
                             (*takeover_addr)[0], (*takeover_addr)[1],
                             (*takeover_addr)[2], (*takeover_addr)[3],
                             (*takeover_addr)[4], (*takeover_addr)[5]);
            APPS_LOG_MSGID_I(LOG_TAG" BT_FAST_PAIR_MESSAGE_CODE_SASS_SWITCH_BACK current addr:[%02X:%02X:%02X:%02X:%02X:%02X].", 6,
                             (*addr)[0], (*addr)[1], (*addr)[2], (*addr)[3], (*addr)[4], (*addr)[5]);
            bt_cm_connect_t connect_param;
            connect_param.profile = BT_CM_PROFILE_SERVICE_MASK_ALL;
            memcpy(connect_param.address, *addr, sizeof(bt_bd_addr_t));
            bt_cm_disconnect(&connect_param);
#ifdef AIR_MULTI_POINT_ENABLE
            bt_bd_addr_t connected_devices[BT_MAX_LINK_NUM];
            uint32_t connected_devices_count = BT_MAX_LINK_NUM;
            uint32_t i;
            connected_devices_count = app_bt_state_service_get_connected_disinclude_aws(connected_devices, connected_devices_count);
            for (i = 0; i < connected_devices_count; i++) {
                if (memcmp(connected_devices[i], *addr, sizeof(bt_bd_addr_t)) != 0) {
                    break;
                }
            }
            if (i < connected_devices_count) {
                APPS_LOG_MSGID_I(LOG_TAG" BT_FAST_PAIR_MESSAGE_CODE_SASS_SWITCH_BACK another connected device:[%02X:%02X:%02X:%02X:%02X:%02X].", 6,
                             connected_devices[i][0], connected_devices[i][1], connected_devices[i][2], 
                             connected_devices[i][3], connected_devices[i][4], connected_devices[i][5]);
                if (switch_back_event & BT_FAST_PAIR_SASS_SWITCH_BACK_EVENT_RESUME_PLAYING) {
                    memcpy(g_app_fast_pair_context.allow_switch_audio_addr, *addr, sizeof(bt_bd_addr_t));
                    ui_shell_remove_event(EVENT_GROUP_UI_SHELL_APP_INTERACTION,
                                          APPS_EVENTS_INTERACTION_FAST_PAIR_CLEAR_ACTIVE_SWITCH_RECORD);
                    ui_shell_send_event(false, EVENT_PRIORITY_MIDDLE,
                                        EVENT_GROUP_UI_SHELL_APP_INTERACTION,
                                        APPS_EVENTS_INTERACTION_FAST_PAIR_CLEAR_ACTIVE_SWITCH_RECORD,
                                        NULL, 0, NULL, 1000);
                    bt_sink_srv_send_action(BT_SINK_SRV_ACTION_PLAY, &(connected_devices[i]));
                }
            }
#endif
            if (0 != memcmp((*takeover_addr), empty_addr, sizeof(bt_bd_addr_t))) {
                connect_param.profile = BT_CM_PROFILE_SERVICE_MASK(BT_CM_PROFILE_SERVICE_HFP) | BT_CM_PROFILE_SERVICE_MASK(BT_CM_PROFILE_SERVICE_A2DP_SINK);
                memcpy(connect_param.address, *takeover_addr, sizeof(bt_bd_addr_t));
                bt_cm_connect(&connect_param);
                g_app_fast_pair_context.connection_switch_back_event = switch_back_event;
            }
            break;
        }
        case BT_FAST_PAIR_MESSAGE_CODE_SASS_NOTIFY_MULTIPOINT_SWITCH: {
            /* Only can be from provider to seeker.*/
            break;
        }
        case BT_FAST_PAIR_MESSAGE_CODE_SASS_GET_CONNECTION_STATUS: {
            /* Processed by middleare. */
            break;
        }
        case BT_FAST_PAIR_MESSAGE_CODE_SASS_NOTIFY_CONNECTION_STATUS: {
            /* Only can be from provider to seeker.*/
            break;
        }
        case BT_FAST_PAIR_MESSAGE_CODE_SASS_NOTIFY_SASS_INITIATED_CONNECTION: {
            /* To-Do. Don't know usage. */
            break;
        }
        case BT_FAST_PAIR_MESSAGE_CODE_SASS_INDICATE_IN_USE_ACCOUNT_KEY: {
            /* Processed by middleare. */
            APPS_LOG_MSGID_I(LOG_TAG" INDICATE_IN_USE_ACCOUNT_KEY", 0);
            app_fast_pair_connection_state_change();
            break;
        }
        case BT_FAST_PAIR_MESSAGE_CODE_SASS_SEND_CUSTOM_DATA: {
            /* Processed by middleare. */
            break;
        }
        case BT_FAST_PAIR_MESSAGE_CODE_SASS_SET_DROP_CONNECTION_TARGET: {
            if (addr) {
                memcpy(g_app_fast_pair_context.drop_target, *addr, sizeof(bt_bd_addr_t));
            }
            break;
        }
        default:
            break;
    }

    if (need_send_ack) {
        if (ack.action == BT_FAST_PAIR_MESSAGE_CODE_ACKNOWLEDGEMENT_ACK) {
            bt_fast_pair_send_acknowledgements(addr, &ack, msg_data->data, msg_data->data_length - sizeof(bt_fast_pair_sass_mac_verify_data_t));
        } else {
            bt_fast_pair_send_acknowledgements(addr, &ack, NULL, 0);
        }
    }

    return true;
}
#endif

/**
* @brief      This function process the events of EVENT_GROUP_UI_SHELL_BT_FAST_PAIR group.
* @param[in]  self, the context pointer of the activity.
* @param[in]  event_id, the current event ID to be handled.
* @param[in]  extra_data, extra data pointer of the current event, NULL means there is no extra data.
* @param[in]  data_len, the length of the extra data. 0 means extra_data is NULL.
* @return     If return true, the current event cannot be handle by the next activity.
*/
static bool app_fast_pair_app_event_callback(struct _ui_shell_activity *self, uint32_t event_id, void *extra_data, size_t data_len)
{
    assert(NULL != self);
    app_bt_event_fast_pair_callback_data_t *format_data = (app_bt_event_fast_pair_callback_data_t *)extra_data;
    APPS_LOG_MSGID_I(LOG_TAG" Fast pair app event id:0x%x, len:%d.", 2, event_id, data_len);
    switch (event_id) {
        case BT_FAST_PAIR_APP_EVENT_NEED_STORE_ACCOUNT_KEY: {
            bt_fast_pair_account_key_list_t *real_key_list = g_app_fast_pair_context.account_key_list;
            bt_fast_pair_account_key_list_t *key_list = (bt_fast_pair_account_key_list_t *)extra_data;
            assert(key_list == real_key_list);
            APPS_LOG_MSGID_I(LOG_TAG" Account key update change fast pair data.", 0);
            if (!g_app_fast_pair_context.is_vis_on) {
                app_fast_pair_trigger_advertising(&g_app_fast_pair_context);
            }
            app_fast_pair_store_account_key(key_list);
#ifdef MTK_AWS_MCE_ENABLE
            uint32_t size_account = sizeof(bt_fast_pair_account_key_list_t) +
                                    sizeof(bt_fast_pair_account_key_t) * (g_app_fast_pair_context.account_key_list->max_key_number - 1);
            apps_aws_sync_event_send_extra(EVENT_GROUP_UI_SHELL_BT_FAST_PAIR,
                                           BT_FAST_PAIR_APP_EVENT_NEED_STORE_ACCOUNT_KEY, g_app_fast_pair_context.account_key_list, size_account);
#endif
            break;
        }
#if APP_ONLY_ALLOW_TAKE_OVER_BY_FAST_PAIR
        case BT_FAST_PAIR_APP_EVENT_PAIR_COMPLETE: {
            g_app_fast_pair_context.waiting_connect = false;
            break;
        }
#endif
        case BT_FAST_PAIR_APP_EVENT_SERVICE_CONNECTED: {
            APPS_LOG_MSGID_I(LOG_TAG" Spp service connected.", 0);
            break;
        }
        case BT_FAST_PAIR_APP_EVENT_SERVICE_DISCONNECTED: {
            APPS_LOG_MSGID_I(LOG_TAG" Spp service disconnected.", 0);
            break;
        }
        case BT_FAST_PAIR_APP_EVENT_RING_REQUEST: {
            if (NULL == format_data || data_len <= sizeof(app_bt_event_fast_pair_callback_data_t)) {
                break;
            }
            app_fast_pair_ring_request_handle((bt_fast_pair_ring_request_t *)format_data->data);
            break;
        }
        case BT_FAST_PAIR_APP_EVENT_MESSAGE_STREAM: {
            APPS_LOG_MSGID_I(LOG_TAG" Message stream received.", 0);
            if (NULL != format_data && data_len >= sizeof(app_bt_event_fast_pair_callback_data_t)) {
                bt_fast_pair_message_stream_t *msg = (bt_fast_pair_message_stream_t *)(format_data->data);
                if (msg->group_ID == BT_FAST_PAIR_MESSAGE_GROUP_DEVICE_ACTION_EVENT
                    && msg->code_ID == BT_FAST_PAIR_MESSAGE_CODE_DEVICE_ACTION_EVENT_RING) {
                    bt_fast_pair_acknowledgements_t ack = {
                        .action = BT_FAST_PAIR_MESSAGE_CODE_ACKNOWLEDGEMENT_ACK,
                        .groupID = BT_FAST_PAIR_MESSAGE_GROUP_DEVICE_ACTION_EVENT,
                        .codeID = BT_FAST_PAIR_MESSAGE_CODE_DEVICE_ACTION_EVENT_RING,
                    };
#ifdef RACE_FIND_ME_ENABLE
                    bt_fast_pair_send_acknowledgements(&(format_data->addr), &ack, msg->data, msg->data_length);
#else
                    ack.action = BT_FAST_PAIR_MESSAGE_CODE_ACKNOWLEDGEMENT_NAK;
                    ack.reason = BT_FAST_PAIR_ACKNOWLEDGEMENT_NAK_REASON_NOT_SUPPORTED;
                    uint8_t current_state[1] = { 0 };
                    bt_fast_pair_send_acknowledgements(&(format_data->addr), &ack, current_state, sizeof(current_state));
#endif
                }
#ifdef AIR_BT_FAST_PAIR_SASS_ENABLE
                if (msg->group_ID == BT_FAST_PAIR_MESSAGE_GROUP_SMART_AUDIO_SOURCE_SWITCHING) {
                    app_fast_pair_app_event_proc_sass_msg(&(format_data->addr), msg);
                }
#endif
            }
            break;
        }
        case BT_FAST_PAIR_APP_EVENT_ADDITIONAL_DATA: {
            if (NULL == format_data || data_len <= sizeof(app_bt_event_fast_pair_callback_data_t)) {
                break;
            }
            app_fast_pair_additional_data_handle(&g_app_fast_pair_context, (bt_fast_pair_additional_data_t *)format_data->data, data_len - sizeof(bt_bd_addr_t));
            break;
        }
        case BT_FAST_PAIR_APP_EVENT_ACTIVE_COMPONENTS_REQ: {
            if (NULL == format_data && data_len <= sizeof(app_bt_event_fast_pair_callback_data_t)) {
                break;
            }
            bt_bd_addr_t *bt_addr = &(format_data->addr);
            uint8_t message_buffer[sizeof(bt_fast_pair_message_stream_t)] = { 0 };
            uint8_t components_active_state = 0x01; /* For headset, 0x01 means active. */
#ifdef MTK_AWS_MCE_ENABLE
            components_active_state = 0;
            if (0 == g_app_fast_pair_context.battery.battery[APP_FAST_PAIR_COMPONENT_RIGHT].charging) {
                components_active_state |= 0x1;
            }
            if (0 == g_app_fast_pair_context.battery.battery[APP_FAST_PAIR_COMPONENT_LEFT].charging) {
                components_active_state |= 0x1 << 1;
            }
#endif

            bt_fast_pair_message_stream_t *cont = (bt_fast_pair_message_stream_t *)&message_buffer;
            cont->group_ID = BT_FAST_PAIR_MESSAGE_GROUP_DEVICE_INFO_EVENT;
            cont->code_ID = BT_FAST_PAIR_MESSAGE_CODE_DEVICE_INFO_EVENT_ACTIVE_COMPONENTS_RSP;
            cont->data_length = 0x01;
            cont->data[0] = components_active_state;
            APPS_LOG_MSGID_I(LOG_TAG" ACTIVE COMPONENTS_RSP : %x.", 1, cont->data[0]);
            bt_fast_pair_send_message_stream(bt_addr, cont);
            break;
        }
#if APP_ONLY_ALLOW_TAKE_OVER_BY_FAST_PAIR
        case BT_FAST_PAIR_APP_EVENT_WAIT_CONNECTION: {
            g_app_fast_pair_context.waiting_connect = true;
            break;
        }
#endif
#ifdef AIR_BT_FAST_PAIR_SASS_ENABLE
        case BT_FAST_PAIR_APP_EVENT_SASS_UPDATE_IN_USE_ACCOUNT: {
            const bt_fast_pair_sass_recorded_data_t *p_recorded_data;
            bt_fast_pair_sass_get_internal_data(NULL, &p_recorded_data);
#ifdef MTK_AWS_MCE_ENABLE
            apps_aws_sync_event_send_extra(EVENT_GROUP_UI_SHELL_BT_FAST_PAIR,
                                           BT_FAST_PAIR_APP_EVENT_SASS_UPDATE_IN_USE_ACCOUNT,
                                           &p_recorded_data->inused_account_key_id,
                                           sizeof(p_recorded_data->inused_account_key_id));
#endif
            break;
        }
        case BT_FAST_PAIR_APP_EVENT_SASS_UPDATE_CONNECTED_RECORD: {
            const bt_fast_pair_sass_recorded_data_t *p_recorded_data;
            bt_fast_pair_sass_get_internal_data(NULL, &p_recorded_data);
#ifdef MTK_AWS_MCE_ENABLE
            apps_aws_sync_event_send_extra(EVENT_GROUP_UI_SHELL_BT_FAST_PAIR,
                                           BT_FAST_PAIR_APP_EVENT_SASS_UPDATE_CONNECTED_RECORD,
                                           p_recorded_data->connected_record_array,
                                           sizeof(bt_bd_addr_t) * p_recorded_data->connected_record_length);
#endif
            break;
        }
#endif
#ifdef AIR_SPOT_ENABLE
        case BT_FAST_PAIR_SPOT_EVENT_START_ADV: {
            // Use last old EID
            app_fast_pair_spot_enable_advertising(TRUE);
            break;
        }

        case BT_FAST_PAIR_SPOT_EVENT_STOP_ADV: {
            app_fast_pair_spot_enable_advertising(FALSE);
            break;
        }

        case BT_FAST_PAIR_SPOT_EVENT_EID_READY: {
            // Use new EID when first generate EID successfully
            app_fast_pair_spot_enable_advertising(TRUE);
            break;
        }

        case BT_FAST_PAIR_SPOT_EVENT_EID_UPDATED: {
            // Use new EID every 1024 sec
            app_fast_pair_spot_enable_advertising(TRUE);
            break;
        }
#endif
        default:
            break;
    }
    return false;
}

#ifdef AIR_BT_FAST_PAIR_SASS_ENABLE
uint8_t bt_fast_pair_sass_get_connection_state(void)
{
    uint8_t fast_pair_state = BT_FAST_PAIR_CONNECTION_STATE_NO_CONNECTION;
    switch (apps_config_key_get_mmi_state()) {
        case APP_BT_OFF:
            fast_pair_state = BT_FAST_PAIR_CONNECTION_STATE_NO_CONNECTION;
            break;
        case APP_CONNECTABLE:
        case APP_DISCONNECTED:
            if (0 < bt_cm_get_connected_devices(~BT_CM_PROFILE_SERVICE_MASK(BT_CM_PROFILE_SERVICE_AWS), NULL, 0)) {
                fast_pair_state = BT_FAST_PAIR_CONNECTION_STATE_NO_CONNECTION;
            } else {
                if (0 < bt_cm_get_connecting_devices(~BT_CM_PROFILE_SERVICE_MASK(BT_CM_PROFILE_SERVICE_AWS), NULL, 0)) {
                    fast_pair_state = BT_FAST_PAIR_CONNECTION_STATE_PAGING;
                }
            }
            break;
        case APP_CONNECTED:
            fast_pair_state = BT_FAST_PAIR_CONNECTION_STATE_CONNECTED;
            break;
        case APP_HFP_INCOMING:
        case APP_HFP_OUTGOING:
        case APP_HFP_CALLACTIVE:
        case APP_HFP_CALLACTIVE_WITHOUT_SCO:
        case APP_HFP_TWC_INCOMING:
        case APP_HFP_TWC_OUTGOING:
        case APP_HFP_MULTITPART_CALL:
        case APP_STATE_HELD_ACTIVE:
            fast_pair_state = BT_FAST_PAIR_CONNECTION_STATE_HFP;
            break;
        case APP_A2DP_PLAYING:
            if (bt_sink_srv_get_music_state() == BT_SINK_SRV_MUSIC_PLAYBACK_STATE_STOPPED) {
                fast_pair_state = BT_FAST_PAIR_CONNECTION_STATE_A2DP_STREAMING;
            } else {
                fast_pair_state = BT_FAST_PAIR_CONNECTION_STATE_A2DP_STREAMING_WITH_AVRCP;
            }
            break;
        case APP_STATE_FIND_ME:
        case APP_STATE_VA:
            fast_pair_state = BT_FAST_PAIR_CONNECTION_STATE_NON_AUDIO_DATA_TRANSFERRING;
            break;
        default:
            break;
    }

    /* Connection flags. */
#ifdef MTK_IN_EAR_FEATURE_ENABLE
    fast_pair_state |= (app_in_ear_get_ohd_state() <= APP_IN_EAR_OHD_NONE_DETECTED ? 0 : 1) << BT_FAST_PAIR_CONNECTION_FLAGS_ON_HEAD_DETECTION;
#else
    fast_pair_state |= 0 << BT_FAST_PAIR_CONNECTION_FLAGS_ON_HEAD_DETECTION;
#endif
    uint32_t connected_count = app_bt_state_service_get_connected_disinclude_aws(NULL, 0);
    uint32_t max_connect = 1;
#ifdef AIR_MULTI_POINT_ENABLE
    if (app_bt_emp_is_enable()) {
#ifdef MTK_AWS_MCE_ENABLE
        max_connect = BT_MAX_LINK_NUM - 1;
#else
        max_connect = BT_MAX_LINK_NUM;
#endif
    }
#endif
    APPS_LOG_MSGID_I(LOG_TAG" connected_count: %d, max_connect: %d", 2, connected_count, max_connect);
    fast_pair_state |= (connected_count >= max_connect ? 0 : 1) << BT_FAST_PAIR_CONNECTION_FLAGS_CONNECTION_AVALIABILITY;
    fast_pair_state |= 0 << BT_FAST_PAIR_CONNECTION_FLAGS_FOCUS_MODE;
    fast_pair_state |= 0 << BT_FAST_PAIR_CONNECTION_FLAGS_AUTO_RECONNECTED;
    return fast_pair_state;
}
#endif

#ifdef AIR_SMART_CHARGER_ENABLE
/**
* @brief      This function process the events of EVENT_GROUP_UI_SHELL_CHARGER_CASE group.
* @param[in]  self, the context pointer of the activity.
* @param[in]  event_id, the current event ID to be handled.
* @param[in]  extra_data, extra data pointer of the current event, NULL means there is no extra data.
* @param[in]  data_len, the length of the extra data. 0 means extra_data is NULL.
* @return     If return true, the current event cannot be handle by the next activity.
*/
static bool app_fast_pair_proc_charger_case_group(ui_shell_activity_t *self,
                                                  uint32_t event_id,
                                                  void *extra_data,
                                                  size_t data_len)
{
    bool ret = false;
    uint8_t battery_item = (AUDIO_CHANNEL_R == ami_get_audio_channel() ? APP_FAST_PAIR_COMPONENT_RIGHT : APP_FAST_PAIR_COMPONENT_LEFT);
    bt_fast_pair_battery_t update_battery = g_app_fast_pair_context.battery;

    switch (event_id) {
        case SMCHARGER_EVENT_NOTIFY_ACTION: {
            app_smcharger_public_event_para_t *event_para = (app_smcharger_public_event_para_t *)extra_data;
            if (event_para->action == SMCHARGER_CHARGER_IN_ACTION) {
                APPS_LOG_MSGID_I(LOG_TAG", SMCharger Charger_in action", 0);
                update_battery.battery[battery_item].charging = 1;
                app_fast_pair_stop_find_me();
            } else if (event_para->action == SMCHARGER_OPEN_LID_ACTION) {
                APPS_LOG_MSGID_I(LOG_TAG", SMCharger Open_lid action", 0);
                update_battery.ui_show = true;
            } else if (event_para->action == SMCHARGER_CLOSE_LID_ACTION) {
                APPS_LOG_MSGID_I(LOG_TAG", SMCharger Close_lid action", 0);
                update_battery.ui_show = false;
            } else if (event_para->action == SMCHARGER_CHARGER_OUT_ACTION) {
                APPS_LOG_MSGID_I(LOG_TAG", SMCharger Charger_out action", 0);
                update_battery.ui_show = true;
                update_battery.battery[battery_item].charging = 0;
                app_fast_pair_stop_find_me();
            } else if (event_para->action == SMCHARGER_CASE_BATTERY_REPORT_ACTION) {
                uint8_t case_battery = (uint8_t)((uint32_t)event_para->data);
                APPS_LOG_MSGID_I(LOG_TAG", BATTERY_LEVEL_REPORT=%d", 1, case_battery);
                update_battery.battery[APP_FAST_PAIR_COMPONENT_CASE].charging = ((case_battery & 0x80) ? 1 : 0);
                update_battery.battery[APP_FAST_PAIR_COMPONENT_CASE].battery_value = (case_battery & 0x7F);
            }
        }
        break;

        default:
            return ret;
    }
    app_fast_pair_update_battery(&g_app_fast_pair_context, &update_battery);
    return ret;
}
#endif

#ifdef MTK_AWS_MCE_ENABLE
#ifdef AIR_SMART_CHARGER_ENABLE
/**
* @brief      This function process the aws data of BT_AWS_MCE_REPORT_MODULE_SMCHARGER.
* @param[in]  context, the context of the app.
* @param[in]  report, the aws report data.
*/
static void app_fast_pair_aws_mce_data_smart_charge_callback(app_fast_pair_local_context_t *context,  bt_aws_mce_report_info_t *report)
{
#if 0
    received_charger_exist = (smcharger_context.peer_smcharger_state != STATE_SMCHARGER_OUT_OF_CASE) ? 1 : 0;
    context->battery.battery[battery_item].charging = received_charger_exist;
    if (!(context->battery.battery[APP_FAST_PAIR_COMPONENT_RIGHT].charging) &&
        !(context->battery.battery[APP_FAST_PAIR_COMPONENT_LEFT].charging)) {
        context->battery.ui_show = false;
    } else {
        context->battery.ui_show = true;
    }
    bt_fast_pair_update_battery(&(context->battery));
    app_fast_pair_trigger_advertising(context);
#endif
}
#endif

/**
* @brief      This function process the AWS data of BT_AWS_MCE_REPORT_MODULE_BATTERY.
* @param[in]  context, the context of the app.
* @param[in]  report, the AWS report data.
*/
static void app_fast_pair_aws_mce_data_battery_callback(app_fast_pair_local_context_t *context, bt_aws_mce_report_info_t *report)
{
#if defined(MTK_BATTERY_MANAGEMENT_ENABLE)
    uint8_t received_charger_exist = 0;
    uint8_t received_bat_percent = 0;
    uint8_t battery_item = (AUDIO_CHANNEL_R == ami_get_audio_channel() ? APP_FAST_PAIR_COMPONENT_LEFT : APP_FAST_PAIR_COMPONENT_RIGHT);
    bt_fast_pair_battery_t update_battery = context->battery;

#ifdef AIR_SMART_CHARGER_ENABLE
    app_smcharger_context_t smcharger_context;
    memcpy(&smcharger_context.peer_battery_percent, report->param, sizeof(smcharger_context.peer_battery_percent));
    APPS_LOG_MSGID_I(LOG_TAG", [Agent] Received partner_bat_percent = %d", 1, smcharger_context.peer_battery_percent);
    received_bat_percent = smcharger_context.peer_battery_percent & (~ PARTNER_BATTERY_CHARGING);
    received_charger_exist = (smcharger_context.peer_battery_percent & PARTNER_BATTERY_CHARGING) ? 1 : 0;    /* The bit 7 is charger state */
#else
    battery_local_context_type_t battery_context;
    memcpy(&battery_context.partner_battery_percent, report->param, sizeof(battery_context.partner_battery_percent));
    APPS_LOG_MSGID_I(LOG_TAG", [Agent] Received partner_bat_percent = %d", 1, battery_context.partner_battery_percent);
    received_bat_percent = battery_context.partner_battery_percent & (~ PARTNER_BATTERY_CHARGING);
    received_charger_exist = (battery_context.partner_battery_percent & PARTNER_BATTERY_CHARGING) ? 1 : 0;      /* The bit 7 is charger state */
#endif
    update_battery.battery[battery_item].charging = received_charger_exist;
    update_battery.battery[battery_item].battery_value = received_bat_percent;
    app_fast_pair_update_battery(context, &update_battery);
#endif
}

/**
* @brief      This function process the AWS data of BT_AWS_MCE_REPORT_MODULE_APP_ACTION, it's fast pair internal AWS events.
* @param[in]  context, the context of the app.
* @param[in]  report, the AWS report data.
*/
static void app_fast_pair_aws_mce_data_app_action_callback(app_fast_pair_local_context_t *context, bt_aws_mce_report_info_t *report)
{
    uint32_t    event_group = 0, event_id = 0, extra_aws_len = 0;
    void        *extra_aws = NULL;
    apps_aws_sync_event_decode_extra(report, &event_group, &event_id, &extra_aws, &extra_aws_len);
    if (EVENT_GROUP_UI_SHELL_BT_FAST_PAIR != event_group || NULL == extra_aws) {
        return;
    }
    switch (event_id) {
        case BT_FAST_PAIR_APP_EVENT_NEED_STORE_ACCOUNT_KEY: {
            /* Received store account key request from agent. */
            bt_fast_pair_account_key_list_t *new_account = (bt_fast_pair_account_key_list_t *)extra_aws;
            if (NULL == new_account || NULL == context->account_key_list ||
                context->account_key_list->max_key_number != new_account->max_key_number
                || extra_aws_len != sizeof(bt_fast_pair_account_key_list_t) +
                sizeof(bt_fast_pair_account_key_t) * (g_app_fast_pair_context.account_key_list->max_key_number - 1)) {
                APPS_LOG_MSGID_E(LOG_TAG", Agent(%d) & Partner(%d) max key number are not same! Stop update flow!", 2,
                                 context->account_key_list ? context->account_key_list->max_key_number : 0,
                                 new_account ? new_account->max_key_number : 0);
                return;
            }
            if (memcmp(context->account_key_list, new_account, extra_aws_len)) {
                memcpy(context->account_key_list, new_account, extra_aws_len);
                app_fast_pair_store_account_key(context->account_key_list);
            }
            break;
        }
        case BT_FAST_PAIR_APP_EVENT_RING_REQUEST: {
#ifdef RACE_FIND_ME_ENABLE
            /* Received ring request from agent. */
            if (extra_aws_len == sizeof(app_find_me_param_struct)) {
                app_find_me_param_struct *find_me_param =
                    (app_find_me_param_struct *)pvPortMalloc(sizeof(app_find_me_param_struct));
                if (find_me_param) {
                    memcpy(find_me_param, extra_aws, sizeof(app_find_me_param_struct));
                    ui_shell_send_event(false, EVENT_PRIORITY_MIDDLE,
                                        EVENT_GROUP_UI_SHELL_FINDME,
                                        APP_FIND_ME_EVENT_ID_TRIGGER,
                                        find_me_param,
                                        sizeof(app_find_me_param_struct),
                                        NULL, 0);
                }
            }
#endif
            break;
        }
        case BT_FAST_PAIR_APP_EVENT_ADDITIONAL_DATA: {
            /* Received process additional data from agent. */
            bt_fast_pair_additional_data_t *data = (bt_fast_pair_additional_data_t *)extra_aws;
            APPS_LOG_MSGID_I(LOG_TAG" Aws packet additional data received data_id %d, data_length %d.", 2, data->data_id, data->data_length);
            if (BT_FAST_PAIR_ADDITIONAL_DATA_ID_PERSONALIZED_NAME == data->data_id) {
                nvdm_status_t status = 0;
                bt_fast_pair_update_parameters_t params;
                if (data->data_length > sizeof(context->nvkey.personalized_name) - 1) {
                    data->data_length = sizeof(context->nvkey.personalized_name) - 1;
                }
                memcpy(context->nvkey.personalized_name, data->data, data->data_length);
                context->nvkey.personalized_name[data->data_length] = 0;
                params.tx_power_level = NULL;
                if (0 != context->nvkey.tx_power_available) {
                    params.tx_power_level = &(context->nvkey.tx_power_level);
                }
                params.addr_type = BT_ADDR_RANDOM;
                params.personalized_name = context->nvkey.personalized_name;
                if (NVDM_STATUS_OK != (status = nvdm_write_data_item(NVDM_GROUP_FAST_PAIR_APP, NVDM_FAST_PAIR_PERSONALIZED_NAME,
                                                                     NVDM_DATA_ITEM_TYPE_RAW_DATA,
                                                                     (uint8_t *) & (g_app_fast_pair_context.nvkey.personalized_name),
                                                                     APP_FAST_PAIR_PERSONALIZED_NAME_SIZE))) {
                    APPS_LOG_MSGID_E(LOG_TAG" PERSONALIZED_NAME write failed : 0x%x", 1, status);
                }
                bt_fast_pair_update_parameters(&params);
            }
            break;
        }
        case APP_FAST_PAIR_AWS_CONTEXT_SYNC_ID: {
            app_fast_pair_nvkey_t *nvkey_cntx = (app_fast_pair_nvkey_t *)extra_aws;
            APPS_LOG_MSGID_I(LOG_TAG" Aws cntext sync data received selected_set %d, component_num %d.", 2, nvkey_cntx->seleceted_set, nvkey_cntx->component_num);
            memcpy(&(context->nvkey), nvkey_cntx, (sizeof(context->nvkey) - sizeof(context->nvkey.reserved)));
        }
        break;
#ifdef AIR_BT_FAST_PAIR_SASS_ENABLE
        case BT_FAST_PAIR_APP_EVENT_SASS_UPDATE_IN_USE_ACCOUNT: {
            APPS_LOG_MSGID_I(LOG_TAG" Aws cntext sync data received IN_USE_ACCOUNT, length: %d", 1, extra_aws_len);
            if (extra_aws_len == sizeof(uint8_t)) {
                uint8_t in_use_account_id = *(uint8_t *)extra_aws;
                bt_fast_pair_sass_set_in_used_account_key_id(in_use_account_id);
            }
            break;
        }
        case BT_FAST_PAIR_APP_EVENT_SASS_UPDATE_CONNECTED_RECORD: {
            APPS_LOG_MSGID_I(LOG_TAG" Aws cntext sync data received UPDATE_CONNECTED_RECORD, length: %d", 1, extra_aws_len);
            uint8_t array_len = extra_aws_len / sizeof(bt_bd_addr_t);
            bt_bd_addr_t *array = array_len > 0 ? extra_aws : NULL;
            bt_fast_pair_sass_set_connected_record(array_len, array);
        }
#endif
#ifdef AIR_SPOT_ENABLE
        case BT_FAST_PAIR_SPOT_EVENT_START_ADV: {
            nvkey_status_t sta = nvkey_write_data(NVID_APP_FPS_SPOT_EIK, extra_aws, extra_aws_len);
            APPS_LOG_MSGID_I(LOG_TAG"[SPOT] partner update eik, nv_sta=%d", 1, sta);
            bt_fast_pair_spot_set_advertising(true);
            break;
        }
        case BT_FAST_PAIR_SPOT_EVENT_STOP_ADV: {
            uint8_t buf[32] = {0};
            nvkey_status_t sta = nvkey_write_data(NVID_APP_FPS_SPOT_EIK, buf, 32);
            APPS_LOG_MSGID_I(LOG_TAG"[SPOT] partner update eik, nv_sta=%d", 1, sta);
            bt_fast_pair_spot_set_advertising(false);
            break;
        }
#endif
        default:
            break;
    }
}

/**
* @brief      This function process the events of EVENT_GROUP_UI_SHELL_AWS_DATA group.
* @param[in]  self, the context pointer of the activity.
* @param[in]  event_id, the current event ID to be handled.
* @param[in]  extra_data, extra data pointer of the current event, NULL means there is no extra data.
* @param[in]  data_len, the length of the extra data. 0 means extra_data is NULL.
* @return     If return true, the current event cannot be handle by the next activity.
*/
static bool app_fast_pair_aws_mce_data_callback(struct _ui_shell_activity *self,  uint32_t event_id, void *extra_data, size_t data_len)
{
    assert(NULL != self);
    bt_aws_mce_report_info_t *report = (bt_aws_mce_report_info_t *)extra_data;
    if (NULL == report) {
        APPS_LOG_MSGID_E(LOG_TAG", report data or context is NULL", 0);
        return false;
    }
    switch (report->module_id) {
        case BT_AWS_MCE_REPORT_MODULE_BATTERY:
            app_fast_pair_aws_mce_data_battery_callback(&g_app_fast_pair_context, report);
            break;
#ifdef AIR_SMART_CHARGER_ENABLE
        case BT_AWS_MCE_REPORT_MODULE_SMCHARGER:
            app_fast_pair_aws_mce_data_smart_charge_callback(&g_app_fast_pair_context, report);
            break;
#endif
        case BT_AWS_MCE_REPORT_MODULE_APP_ACTION:
            app_fast_pair_aws_mce_data_app_action_callback(&g_app_fast_pair_context, report);
            break;
        default:
            APPS_LOG_MSGID_I(LOG_TAG", report moudle id:0x%x not support.", 1, report->module_id);
            return false;
    }
    return false;
}
#endif

/**
* @brief      This function process the events of EVENT_GROUP_UI_SHELL_MULTI_VA group.
* @param[in]  self, the context pointer of the activity.
* @param[in]  event_id, the current event ID to be handled.
* @param[in]  extra_data, extra data pointer of the current event, NULL means there is no extra data.
* @param[in]  data_len, the length of the extra data. 0 means extra_data is NULL.
* @return     If return true, the current event cannot be handle by the next activity.
*/
static bool app_fast_pair_multi_va_event_proc(ui_shell_activity_t *self,  uint32_t event_id, void *extra_data, size_t data_len)
{
    bool ret = false;
    switch (event_id) {
#ifdef MTK_AWS_MCE_ENABLE
        case MULTI_VA_EVENT_NOTIFY_BLE_ADDR_CHANGED: {
            bt_bd_addr_t random_addr;
            if (multi_ble_adv_manager_get_random_addr_and_adv_handle(MULTI_ADV_INSTANCE_FAST_PAIR, &random_addr, NULL)) {
                APPS_LOG_MSGID_I(LOG_TAG", ble address changed : %x:%x:%x:%x:%x:%x", 6,
                                 random_addr[0], random_addr[1], random_addr[2],
                                 random_addr[3], random_addr[4], random_addr[5]);
                bt_fast_pair_set_random_ble_address(false, &random_addr);
            }
            break;
        }
#endif
        default:
            break;
    }

    return ret;
}


/**
* @brief      This function to init fast pair.
* @param[in]  self, the context pointer of the activity.
* @return     initialized success or not.
*/
static bool app_fast_pair_init(ui_shell_activity_t *self)
{
    bt_fast_pair_status_t           ret;
    uint32_t    app_fast_pair_model_id = APP_FAST_PAIR_AIROHA_TEST_MODEL_ID;
    uint8_t     app_fast_pair_private_key[32] = APP_FAST_PAIR_AIROHA_TEST_PRIVATE_KEY;
    app_fast_pair_protected_t       *protect_sets;
    bt_bd_addr_t random_addr;
    atci_register_handler(fast_pair_test_atci_cmd, sizeof(fast_pair_test_atci_cmd) / sizeof(atci_cmd_hdlr_item_t));
    protect_sets = (app_fast_pair_protected_t *)pvPortMalloc(sizeof(app_fast_pair_protected_t));
    if (protect_sets != NULL && 0 == app_fast_pair_load_configure_info(&g_app_fast_pair_context, protect_sets)) {
        if (g_app_fast_pair_context.nvkey.seleceted_set >= APP_FAST_PAIR_MODEL_SET_MAX_NUMBER) {
            g_app_fast_pair_context.nvkey.seleceted_set = APP_FAST_PAIR_MODEL_SET_MAX_NUMBER - 1;
        }
        app_fast_pair_model_id = protect_sets->sets[g_app_fast_pair_context.nvkey.seleceted_set].model_id;
        memcpy(app_fast_pair_private_key, &(protect_sets->sets[g_app_fast_pair_context.nvkey.seleceted_set].private_key), 32 * sizeof(uint8_t));
        vPortFree(protect_sets);
        APPS_LOG_MSGID_W(LOG_TAG" init model id 0x%x, max account %d, tx_power_exist %d, tx_power %d, component num %d select %d!",
                         6, app_fast_pair_model_id, g_app_fast_pair_context.nvkey.max_account, g_app_fast_pair_context.nvkey.tx_power_available,
                         g_app_fast_pair_context.nvkey.tx_power_level, g_app_fast_pair_context.nvkey.component_num, g_app_fast_pair_context.nvkey.seleceted_set);
        if (!g_app_fast_pair_context.nvkey.fps_enable) {
            APPS_LOG_MSGID_W(LOG_TAG" init disable!", 0);
            return false;
        }
    } else {
        /* Load fail must stop init process */
        g_app_fast_pair_context.nvkey.fps_enable = 1;
        g_app_fast_pair_context.nvkey.max_account = APP_FAST_PAIR_DEFAULT_MAX_ACCOUNT;
        g_app_fast_pair_context.nvkey.seleceted_set = 0;
        g_app_fast_pair_context.nvkey.tx_power_available = 1;
        g_app_fast_pair_context.nvkey.tx_power_level = APP_FAST_PAIR_DEFAULT_TX_POWER_LEVEL;
        g_app_fast_pair_context.nvkey.component_num = APP_FAST_PAIR_COMPONENT_NUM;
        /* memset(g_app_fast_pair_context.nvkey.personalized_name, 0, sizeof(g_app_fast_pair_context.nvkey.personalized_name)); */
    }
    g_app_fast_pair_context.model_id = app_fast_pair_model_id;
    g_app_fast_pair_context.account_key_list = (bt_fast_pair_account_key_list_t *)pvPortMalloc(sizeof(bt_fast_pair_account_key_list_t) +
                                                                                               (g_app_fast_pair_context.nvkey.max_account - 1) * sizeof(bt_fast_pair_account_key_t));
    if (NULL == g_app_fast_pair_context.account_key_list) {
        APPS_LOG_MSGID_E(LOG_TAG" Init allocate failed", 0);
        return false;
    }
    app_fast_pair_load_account_key(&g_app_fast_pair_context);
    APPS_LOG_MSGID_I(LOG_TAG" Init account ", 0);
    bt_fast_pair_init_parameters_t init_param;
    memset(&init_param, 0, sizeof(init_param));
    init_param.model_id = app_fast_pair_model_id;
    init_param.private_key = app_fast_pair_private_key;
    init_param.tx_power_level = NULL;
    init_param.addr_type = BT_ADDR_RANDOM;
    init_param.component_num = g_app_fast_pair_context.nvkey.component_num;
    init_param.personalized_name = g_app_fast_pair_context.nvkey.personalized_name;
    init_param.firmware_revision = MTK_FW_VERSION;
#ifdef AIR_BT_FAST_PAIR_SASS_ENABLE
    g_app_fast_pair_context.sass_switch_prefer_flags = (0 << BT_FAST_PAIR_SASS_MULTIPOINT_SWITCH_A_A_BIT)
                                                       | (1 << BT_FAST_PAIR_SASS_MULTIPOINT_SWITCH_H_A_BIT);
    init_param.sass_param.sass_version_code = FAST_PAIR_SASS_VERSION_CODE;
    init_param.sass_param.sass_capability_flags = (FAST_PAIR_SASS_DEFAULT_ENABLED << BT_FAST_PAIR_SASS_CAPABILITY_STATE_BIT)
                                                  | (0 << BT_FAST_PAIR_SASS_CAPABILITY_MULTIPOINT_CONFIG_BIT)
                                                  | (FAST_PAIR_SASS_MULTI_POINT << BT_FAST_PAIR_SASS_CAPABILITY_MULTIPOINT_STATE_BIT)
#ifdef MTK_IN_EAR_FEATURE_ENABLE
                                                  | ((app_in_ear_get_ohd_state() == APP_IN_EAR_OHD_DISABLED ? 0 : 1) << BT_FAST_PAIR_SASS_CAPABILITY_ON_HEAD_DETECTION_BIT)
                                                  | ((app_in_ear_get_ohd_state() <= APP_IN_EAR_OHD_NONE_DETECTED ? 0 : 1) << BT_FAST_PAIR_SASS_CAPABILITY_ON_HEAD_DETECTION_CURRENT_BIT)
#else
                                                  | (0 << BT_FAST_PAIR_SASS_CAPABILITY_ON_HEAD_DETECTION_BIT)
                                                  | (0 << BT_FAST_PAIR_SASS_CAPABILITY_ON_HEAD_DETECTION_CURRENT_BIT)
#endif
                                                  ;
    init_param.sass_param.switching_preference_flags = g_app_fast_pair_context.sass_switch_prefer_flags;
#endif
    if (0 != g_app_fast_pair_context.nvkey.tx_power_available) {
        init_param.tx_power_level = &(g_app_fast_pair_context.nvkey.tx_power_level);
    }
    ret = bt_fast_pair_init(&init_param);
    if (multi_ble_adv_manager_get_random_addr_and_adv_handle(MULTI_ADV_INSTANCE_FAST_PAIR,
                                                             &random_addr,
                                                             &(g_app_fast_pair_context.adv_handle))) {
        bt_fast_pair_set_random_ble_address(false, &random_addr);
    }

    g_app_fast_pair_context.battery.ui_show = false;
    g_app_fast_pair_context.battery.component_num = APP_FAST_PAIR_COMPONENT_NUM;
    g_app_fast_pair_context.battery.remaining_time = BT_FAST_PAIR_REMAINING_TIME_INVALID_VALUE;
    for (uint8_t i = 0; i < APP_FAST_PAIR_COMPONENT_NUM; i++) {
        g_app_fast_pair_context.battery.battery[i].charging = 0;
        g_app_fast_pair_context.battery.battery[i].battery_value = 0xFF;
    }
    if (ret != BT_FAST_PAIR_STATUS_SUCCESS) {
        APPS_LOG_MSGID_E(LOG_TAG" fast pair service init failed %d", 1, ret);
        vPortFree(g_app_fast_pair_context.account_key_list);
        g_app_fast_pair_context.account_key_list = NULL;
        return false;
    }
    return true;
}

uint32_t app_fast_pair_get_model_id(void)
{
    return g_app_fast_pair_context.model_id;
}


static bool app_fast_pair_bt_takeover_allow(const bt_bd_addr_t device_addr)
{
#ifdef AIR_BT_FAST_PAIR_SASS_ENABLE
    if (memcmp(g_app_fast_pair_context.drop_target, empty_addr, sizeof(bt_bd_addr_t)) != 0) {
        if (memcmp(g_app_fast_pair_context.drop_target, device_addr, sizeof(bt_bd_addr_t)) == 0) {
            return true;
        } else {
            return false;
        }
    }
#endif
#if APP_ONLY_ALLOW_TAKE_OVER_BY_FAST_PAIR
    return g_app_fast_pair_context.waiting_connect;
#else
    return true;
#endif
}

/**
* @brief      This function process the events of EVENT_GROUP_UI_SHELL_SYSTEM group.
* @param[in]  self, the context pointer of the activity.
* @param[in]  event_id, the current event ID to be handled.
* @param[in]  extra_data, extra data pointer of the current event, NULL means there is no extra data.
* @param[in]  data_len, the length of the extra data. 0 means extra_data is NULL.
* @return     If return true, the current event cannot be handle by the next activity.
*/
static bool app_proc_fast_pair_shell_group(ui_shell_activity_t *self, uint32_t event_id, void *extra_data, size_t data_len)
{
    bool ret = true; /* UI shell internal event must process by this activity, so default is true. */
    switch (event_id) {
        case EVENT_ID_SHELL_SYSTEM_ON_CREATE: {
#ifdef AIR_SPOT_ENABLE
            bt_fast_pair_spot_set_adv_callback(app_fast_pair_spot_adv_callback);
#endif
            memset(&g_app_fast_pair_context, 0, sizeof(app_fast_pair_local_context_t));
            app_bt_takeover_service_user_register(APP_BT_TAKEOVER_ID_FASTPAIR, app_fast_pair_bt_takeover_allow);
            ret = app_fast_pair_init(self);
            if (ret == true) {
                /* BT event is not suitable for UI shell scheme.
                * Instead, choose bt callback manager to handle bt related event. */
#if MTK_AWS_MCE_ENABLE
                uint8_t battery_item = (AUDIO_CHANNEL_R == ami_get_audio_channel() ? APP_FAST_PAIR_COMPONENT_RIGHT : APP_FAST_PAIR_COMPONENT_LEFT);
#else
                uint8_t battery_item = 0;
#endif
#if defined(MTK_BATTERY_MANAGEMENT_ENABLE)
                bt_fast_pair_battery_t update_battery = g_app_fast_pair_context.battery;
                update_battery.battery[battery_item].charging = battery_management_get_battery_property(BATTERY_PROPERTY_CHARGER_EXIST);
                update_battery.battery[battery_item].battery_value = battery_management_get_battery_property(BATTERY_PROPERTY_CAPACITY);
#ifdef AIR_SMART_CHARGER_ENABLE
                uint8_t case_battery = app_smcharger_read_case_battery_nvkey();
                update_battery.battery[APP_FAST_PAIR_COMPONENT_CASE].charging = ((case_battery & 0x80) ? 1 : 0);
                update_battery.battery[APP_FAST_PAIR_COMPONENT_CASE].battery_value = (case_battery & 0x7F);
#endif
                app_fast_pair_update_battery(&g_app_fast_pair_context, &update_battery);
#endif

                extern void bt_fast_pair_at_cmd_init();
                bt_fast_pair_at_cmd_init();
            }
            APPS_LOG_MSGID_I(LOG_TAG" activity: %d", 1, ret);
            break;
        }
        case EVENT_ID_SHELL_SYSTEM_ON_DESTROY: {
            if (g_app_fast_pair_context.account_key_list) {
                vPortFree(g_app_fast_pair_context.account_key_list);
                g_app_fast_pair_context.account_key_list = NULL;
            }
            APPS_LOG_MSGID_I(LOG_TAG" destory", 0);
            break;
        }
        case EVENT_ID_SHELL_SYSTEM_ON_RESUME: {
            break;
        }
        case EVENT_ID_SHELL_SYSTEM_ON_PAUSE: {
            break;
        }
        case EVENT_ID_SHELL_SYSTEM_ON_REFRESH: {
            break;
        }
        case EVENT_ID_SHELL_SYSTEM_ON_RESULT: {
            break;
        }
        default:
            break;
    }
    return ret;
}

/**
* @brief      This function process the events of EVENT_GROUP_UI_SHELL_BATTERY group.
* @param[in]  self, the context pointer of the activity.
* @param[in]  event_id, the current event ID to be handled.
* @param[in]  extra_data, extra data pointer of the current event, NULL means there is no extra data.
* @param[in]  data_len, the length of the extra data. 0 means extra_data is NULL.
* @return     If return true, the current event cannot be handle by the next activity.
*/
static bool app_fast_pair_proc_battery_group(ui_shell_activity_t *self, uint32_t event_id, void *extra_data, size_t data_len)
{
    bool ret = false;
    assert(NULL != self);
#ifdef MTK_AWS_MCE_ENABLE
    uint8_t battery_item = (AUDIO_CHANNEL_R == ami_get_audio_channel() ? APP_FAST_PAIR_COMPONENT_RIGHT : APP_FAST_PAIR_COMPONENT_LEFT);
#else
    uint8_t battery_item = 0;
#endif
    bt_fast_pair_battery_t update_battery = g_app_fast_pair_context.battery;
    switch (event_id) {
        case APPS_EVENTS_BATTERY_PERCENT_CHANGE: {
            APPS_LOG_MSGID_I(LOG_TAG"APPS_EVENTS_BATTERY_PERCENT_CHANGE : %d", 1, (uint32_t)extra_data);
            update_battery.battery[battery_item].battery_value = (uint32_t)extra_data;
            break;
        }
        case APPS_EVENTS_BATTERY_CHARGER_EXIST_CHANGE: {
#if 0
            APPS_LOG_MSGID_I(LOG_TAG"APPS_EVENTS_BATTERY_CHARGER_EXIST_CHANGE : %d", 1, (uint32_t)extra_data);
            g_app_fast_pair_context.battery.battery[battery_item].charging = (uint32_t)extra_data;
            if (!(g_app_fast_pair_context.battery.battery[APP_FAST_PAIR_COMPONENT_RIGHT].charging) &&
                !(g_app_fast_pair_context.battery.battery[APP_FAST_PAIR_COMPONENT_LEFT].charging)) {
                g_app_fast_pair_context.battery.ui_show = false;
            } else {
                g_app_fast_pair_context.battery.ui_show = true;
            }
#endif
        }
        return ret;
        default:
            return ret;
    }
    app_fast_pair_update_battery(&g_app_fast_pair_context, &update_battery);
    return ret;
}

static bool app_fast_pair_proc_bt_group(ui_shell_activity_t *self, uint32_t event_id, void *extra_data, size_t data_len)
{
    bool ret = false;

    switch (event_id) {
        case BT_GAP_LE_RPA_ROTAION_IND: {
            bt_gap_le_rpa_rotation_ind_t *rotation = NULL;
            apps_bt_event_data_t *bt_event_data = (apps_bt_event_data_t *)extra_data;
            if (bt_event_data != NULL) {
                rotation = (bt_gap_le_rpa_rotation_ind_t *)bt_event_data->buffer;
            }
            if (rotation != NULL) {
                bt_gap_le_advertising_handle_t adv_handle = 0;
                multi_ble_adv_manager_get_random_addr_and_adv_handle(MULTI_ADV_INSTANCE_FAST_PAIR,
                                                                     NULL,
                                                                     &adv_handle);
                APPS_LOG_MSGID_W(LOG_TAG"BT_GAP_LE_RPA_ROTAION_IND, handle %d?=%d, %x:%x:%x:%x:%x:%x", 8,
                                 rotation->handle, adv_handle,
                                 rotation->address[0], rotation->address[1], rotation->address[2],
                                 rotation->address[3], rotation->address[4], rotation->address[5]);
                if (rotation->handle == adv_handle) {
                    bt_fast_pair_set_random_ble_address(true, &(rotation->address));
                }
            }
            break;
        }
        default:
            break;
    }
    return ret;
}

/**
* @brief      This function process the events of EVENT_GROUP_UI_SHELL_BT_SINK group.
* @param[in]  self, the context pointer of the activity.
* @param[in]  event_id, the current event ID to be handled.
* @param[in]  extra_data, extra data pointer of the current event, NULL means there is no extra data.
* @param[in]  data_len, the length of the extra data. 0 means extra_data is NULL.
* @return     If return true, the current event cannot be handle by the next activity.
*/
static bool app_fast_pair_proc_bt_sink_group(struct _ui_shell_activity *self, uint32_t event_id, void *extra_data, size_t data_len)
{
    bool ret = false;
    switch (event_id) {
#if defined AIR_BT_FAST_PAIR_SASS_ENABLE && defined AIR_MULTI_POINT_ENABLE
        case BT_SINK_SRV_EVENT_PLAYING_DEVICE_CHANGE:
            if (extra_data != NULL) {
                bt_sink_srv_device_state_t *device_state = (bt_sink_srv_device_state_t *)extra_data;
                bt_bd_addr_t *addr = &(device_state->address);
                bt_fast_pair_sass_multipoint_audio_src_switch_reason_t switch_reason;

                if (device_state->music_state == BT_SINK_SRV_STATE_STREAMING) {
                    switch_reason = BT_FAST_PAIR_SASS_MULTIPOINT_AUDIO_SRC_SWITCH_REASON_A2DP;
                } else if (device_state->call_state >= BT_SINK_SRV_STATE_INCOMING) {
                    switch_reason = BT_FAST_PAIR_SASS_MULTIPOINT_AUDIO_SRC_SWITCH_REASON_HFP;
                } else {
                    switch_reason = BT_FAST_PAIR_SASS_MULTIPOINT_AUDIO_SRC_SWITCH_REASON_UNSPECIFIED;
                }
                APPS_LOG_MSGID_I(LOG_TAG"BT_SINK_SRV_EVENT_PLAYING_DEVICE_CHANGE reason : %d", 1, switch_reason);
                /* bt_fast_pair_sass_update_multipoint_audio_source_switch(addr, switch_reason); */
                g_app_fast_pair_context.switch_reason = switch_reason;
                memcpy(g_app_fast_pair_context.switch_done_target_addr, *addr, sizeof(bt_bd_addr_t));

            }
            break;
#endif
        default:
            break;
    }
    return ret;
}

/**
* @brief      This function process the events of EVENT_GROUP_UI_SHELL_BT_CONN_MANAGER group.
* @param[in]  self, the context pointer of the activity.
* @param[in]  event_id, the current event ID to be handled.
* @param[in]  extra_data, extra data pointer of the current event, NULL means there is no extra data.
* @param[in]  data_len, the length of the extra data. 0 means extra_data is NULL.
* @return     If return true, the current event cannot be handle by the next activity.
*/
static bool app_fast_pair_proc_bt_cm_group(struct _ui_shell_activity *self, uint32_t event_id, void *extra_data, size_t data_len)
{
    bool ret = false;
    /* APPS_LOG_MSGID_I("[APP_FAST_PAIR], bt cm event_id : 0x%x", 1, event_id); */
#ifdef MTK_AWS_MCE_ENABLE
    /* Only handle bt cm related event in Agent role */
    bt_aws_mce_role_t aws_role = bt_connection_manager_device_local_info_get_aws_role();
    if ((BT_AWS_MCE_ROLE_PARTNER | BT_AWS_MCE_ROLE_CLINET) & aws_role) {
        APPS_LOG_MSGID_W("[APP_FAST_PAIR] Don't init on wrongly role: %d", 1, aws_role);
        return ret;
    }
#endif
    switch (event_id) {
        case BT_CM_EVENT_VISIBILITY_STATE_UPDATE: {
            bt_cm_visibility_state_update_ind_t *visible_update = (bt_cm_visibility_state_update_ind_t *)extra_data;
            g_app_fast_pair_context.is_vis_on = visible_update->visibility_state;
            APPS_LOG_MSGID_I("[APP_FAST_PAIR] Visibilty update chage fast pair data.", 0);
            app_fast_pair_trigger_advertising(&g_app_fast_pair_context);
#ifdef AIR_SPOT_ENABLE
            bt_fast_pair_spot_update_pairing_mode_state(visible_update->visibility_state);
#endif
        }
        break;
        case BT_CM_EVENT_REMOTE_INFO_UPDATE: {
            bt_cm_remote_info_update_ind_t *remote_update = (bt_cm_remote_info_update_ind_t *)extra_data;
            bt_cm_profile_service_mask_t profile_srv = BT_CM_PROFILE_SERVICE_MASK(BT_CM_PROFILE_SERVICE_HFP) | BT_CM_PROFILE_SERVICE_MASK(BT_CM_PROFILE_SERVICE_A2DP_SINK);
            if ((remote_update->pre_connected_service & profile_srv) && !(remote_update->connected_service & profile_srv)) {
                /* For fast pair service add workaround, if HFP and A2DP all disconnected, then we need disconnected fast pair service actively. */
                /* bt_fast_pair_terminate(&(remote_update->address)); */
                bt_cm_connect_t disconnect_param;
                memcpy(disconnect_param.address, remote_update->address, sizeof(bt_bd_addr_t));
                disconnect_param.profile = BT_CM_PROFILE_SERVICE_MASK(BT_CM_PROFILE_SERVICE_CUSTOMIZED_FAST_PAIR);
                bt_cm_disconnect(&disconnect_param);
            }
#ifdef MTK_AWS_MCE_ENABLE
            if (BT_CM_ACL_LINK_CONNECTED <= remote_update->acl_state &&
                !(remote_update->pre_connected_service & BT_CM_PROFILE_SERVICE_MASK(BT_CM_PROFILE_SERVICE_AWS)) &&
                (remote_update->connected_service & BT_CM_PROFILE_SERVICE_MASK(BT_CM_PROFILE_SERVICE_AWS))) {
                uint32_t size_account = sizeof(bt_fast_pair_account_key_list_t) +
                                        sizeof(bt_fast_pair_account_key_t) * (g_app_fast_pair_context.account_key_list->max_key_number - 1);
                /* Only agent role will enable this module, so don't need judge the role
                           * in event handle. */
                APPS_LOG_MSGID_I("[APP_FAST_PAIR]AWS attached.", 0);
                apps_aws_sync_event_send_extra(EVENT_GROUP_UI_SHELL_BT_FAST_PAIR,
                                               BT_FAST_PAIR_APP_EVENT_NEED_STORE_ACCOUNT_KEY, g_app_fast_pair_context.account_key_list, size_account);
                apps_aws_sync_event_send_extra(EVENT_GROUP_UI_SHELL_BT_FAST_PAIR,
                                               APP_FAST_PAIR_AWS_CONTEXT_SYNC_ID, &(g_app_fast_pair_context.nvkey), sizeof(g_app_fast_pair_context.nvkey) - sizeof(g_app_fast_pair_context.nvkey.reserved));

#ifdef AIR_BT_FAST_PAIR_SASS_ENABLE
                const bt_fast_pair_sass_recorded_data_t *p_recorded_data;
                bt_fast_pair_sass_get_internal_data(NULL, &p_recorded_data);
#ifdef MTK_AWS_MCE_ENABLE
                apps_aws_sync_event_send_extra(EVENT_GROUP_UI_SHELL_BT_FAST_PAIR,
                                               BT_FAST_PAIR_APP_EVENT_SASS_UPDATE_IN_USE_ACCOUNT,
                                               &p_recorded_data->inused_account_key_id,
                                               sizeof(p_recorded_data->inused_account_key_id));
                apps_aws_sync_event_send_extra(EVENT_GROUP_UI_SHELL_BT_FAST_PAIR,
                                               BT_FAST_PAIR_APP_EVENT_SASS_UPDATE_CONNECTED_RECORD,
                                               p_recorded_data->connected_record_array,
                                               sizeof(bt_bd_addr_t) * p_recorded_data->connected_record_length);
#endif
#endif
            } else if ((remote_update->pre_connected_service & BT_CM_PROFILE_SERVICE_MASK(BT_CM_PROFILE_SERVICE_AWS)) &&
                       !(remote_update->connected_service & BT_CM_PROFILE_SERVICE_MASK(BT_CM_PROFILE_SERVICE_AWS))) {
                uint8_t peer_battery_item = (AUDIO_CHANNEL_R != ami_get_audio_channel() ? APP_FAST_PAIR_COMPONENT_RIGHT : APP_FAST_PAIR_COMPONENT_LEFT);
                bt_fast_pair_battery_t update_battery = g_app_fast_pair_context.battery;
                update_battery.battery[peer_battery_item].battery_value = 0xFF;
                app_fast_pair_update_battery(&g_app_fast_pair_context, &update_battery);
            }
#endif
#ifdef AIR_BT_FAST_PAIR_SASS_ENABLE
            /*Delete the drop_target. */
            if (BT_CM_ACL_LINK_CONNECTED <= remote_update->pre_acl_state && BT_CM_ACL_LINK_CONNECTED > remote_update->acl_state) {
                if (memcmp(remote_update->address, g_app_fast_pair_context.drop_target, sizeof(g_app_fast_pair_context.drop_target)) == 0) {
                    memset(g_app_fast_pair_context.drop_target, 0, sizeof(g_app_fast_pair_context.drop_target));
                    APPS_LOG_MSGID_I(LOG_TAG"[APP_FAST_PAIR] ACL disconnect, remove drop target.", 0);
                }
#if defined AIR_MULTI_POINT_ENABLE
                if (0 == memcmp(g_app_fast_pair_context.temp_allow_switch_audio_addr, remote_update->address, sizeof(bt_bd_addr_t))) {
                    APPS_LOG_MSGID_I(LOG_TAG"ACL disconnect, remove temp_allow_switch_audio_addr.", 0);
                }
#endif
                if (app_bt_state_service_get_connected_disinclude_aws(NULL, 0) > 0) {
                    app_fast_pair_connection_state_change();
                }
            } else if (BT_CM_ACL_LINK_DISCONNECTED == remote_update->pre_acl_state && BT_CM_ACL_LINK_CONNECTED <= remote_update->acl_state) {
#if defined AIR_MULTI_POINT_ENABLE
                APPS_LOG_MSGID_I(LOG_TAG"ACL connect, and connected from SP, remember the temp_allow_switch_audio_addr.", 0);
                memcpy(g_app_fast_pair_context.temp_allow_switch_audio_addr, remote_update->address, sizeof(bt_bd_addr_t));
                ui_shell_send_event(false, EVENT_PRIORITY_MIDDLE, EVENT_GROUP_UI_SHELL_APP_INTERACTION,
                                    APPS_EVENTS_INTERACTION_FAST_PAIR_SASS_CLEAR_TEMP_ALLOW, NULL, 0, NULL, 5000);
#endif
                if (app_bt_state_service_get_connected_disinclude_aws(NULL, 0) >= 2) {
                    app_fast_pair_connection_state_change();
                }
            }
            if ((~remote_update->pre_connected_service) & remote_update->connected_service
                & (BT_CM_PROFILE_SERVICE_MASK(BT_CM_PROFILE_SERVICE_A2DP_SINK) | BT_CM_PROFILE_SERVICE_MASK(BT_CM_PROFILE_SERVICE_AVRCP))
                && remote_update->connected_service & BT_CM_PROFILE_SERVICE_MASK(BT_CM_PROFILE_SERVICE_A2DP_SINK)
                && remote_update->connected_service & BT_CM_PROFILE_SERVICE_MASK(BT_CM_PROFILE_SERVICE_AVRCP)) {
                APPS_LOG_MSGID_I(LOG_TAG"A2DP/AVRCP connected, need resume music event : %d", 1, g_app_fast_pair_context.connection_switch_back_event);
                if (g_app_fast_pair_context.connection_switch_back_event == BT_FAST_PAIR_SASS_SWITCH_BACK_EVENT_RESUME_PLAYING) {
                    bt_sink_srv_action_t sink_action = BT_SINK_SRV_ACTION_PLAY;
                    bt_sink_srv_send_action(sink_action, NULL);
                    g_app_fast_pair_context.connection_switch_back_event = 0;
                }
#if defined AIR_MULTI_POINT_ENABLE
                if (0 == memcmp(g_app_fast_pair_context.temp_allow_switch_audio_addr, remote_update->address, sizeof(bt_bd_addr_t))) {
                    APPS_LOG_MSGID_I(LOG_TAG"A2DP and AVRCP connected. update temp_allow_switch_audio_addr timer", 0);
                    ui_shell_remove_event(EVENT_GROUP_UI_SHELL_APP_INTERACTION,
                                          APPS_EVENTS_INTERACTION_FAST_PAIR_SASS_CLEAR_TEMP_ALLOW);
                    ui_shell_send_event(false, EVENT_PRIORITY_MIDDLE, EVENT_GROUP_UI_SHELL_APP_INTERACTION,
                                        APPS_EVENTS_INTERACTION_FAST_PAIR_SASS_CLEAR_TEMP_ALLOW, NULL, 0, NULL, 2000);
                }
#endif
            }
#endif
            break;
        }
        default:
            break;
    }
    return ret;
}

/**
* @brief      This function process the events of EVENT_GROUP_UI_SHELL_BT_CONN_MANAGER group.
* @param[in]  self, the context pointer of the activity.
* @param[in]  event_id, the current event ID to be handled.
* @param[in]  extra_data, extra data pointer of the current event, NULL means there is no extra data.
* @param[in]  data_len, the length of the extra data. 0 means extra_data is NULL.
* @return     If return true, the current event cannot be handle by the next activity.
*/
static bool app_fast_pair_proc_bt_dm_group(struct _ui_shell_activity *self, uint32_t event_id, void *extra_data, size_t data_len)
{
    bool ret = false;

    bt_device_manager_power_event_t evt;
    bt_device_manager_power_status_t status;
    bt_event_get_bt_dm_event_and_status(event_id, &evt, &status);
    switch (evt) {
        case BT_DEVICE_MANAGER_POWER_EVT_ACTIVE_COMPLETE: {
            if (BT_DEVICE_MANAGER_POWER_STATUS_SUCCESS == status && !apps_config_features_is_mp_test_mode()) {
                APPS_LOG_MSGID_I("[APP_FAST_PAIR] Power on trigger fast pair advertising.", 0);
#ifdef AIR_TILE_ENABLE
                g_app_fast_pair_context.is_classic_bt_on = true;
                APPS_LOG_MSGID_I(LOG_TAG"classic bt on %d", 1, g_app_fast_pair_context.is_classic_bt_on);
#endif
                app_fast_pair_trigger_advertising(&g_app_fast_pair_context);
            }
            break;
        }
#ifdef AIR_TILE_ENABLE
        case BT_DEVICE_MANAGER_POWER_EVT_CLASSIC_ACTIVE_COMPLETE: {
            if (BT_DEVICE_MANAGER_POWER_STATUS_SUCCESS == status && !apps_config_features_is_mp_test_mode()) {
                g_app_fast_pair_context.is_classic_bt_on = true;
                APPS_LOG_MSGID_I(LOG_TAG"Classic power on trigger fast pair advertising, classic bt on %d", 1, g_app_fast_pair_context.is_classic_bt_on);
                app_fast_pair_trigger_advertising(&g_app_fast_pair_context);
            }
            break;
        }
#endif
        case BT_DEVICE_MANAGER_POWER_EVT_STANDBY_COMPLETE: {
            if (BT_DEVICE_MANAGER_POWER_RESET_TYPE_NORMAL == status && !apps_config_features_is_mp_test_mode()) {
                g_app_fast_pair_context.is_vis_on = false;
                multi_ble_adv_manager_remove_ble_adv(MULTI_ADV_INSTANCE_FAST_PAIR, app_fast_pair_get_adv_data);
            }
            break;
        }
        default:
            break;
    }

    return ret;
}

#ifdef MTK_AWS_MCE_ENABLE
/**
* @brief      This function process the events of EVENT_GROUP_UI_SHELL_AWS group.
* @param[in]  self, the context pointer of the activity.
* @param[in]  event_id, the current event ID to be handled.
* @param[in]  extra_data, extra data pointer of the current event, NULL means there is no extra data.
* @param[in]  data_len, the length of the extra data. 0 means extra_data is NULL.
* @return     If return true, the current event cannot be handle by the next activity.
*/
bool app_fast_pair_proc_aws_event(ui_shell_activity_t *self, uint32_t event_id, void *extra_data, size_t data_len)
{
    bool ret = false;

    switch (event_id) {
        case BT_AWS_MCE_SRV_EVENT_AIR_PAIRING_COMPLETE:
            break;
        default:
            break;
    }
    return ret;
}
#endif


/**
* @brief      This function process the events of EVENT_GROUP_UI_SHELL_APP_INTERACTION.
* @param[in]  self, the context pointer of the activity.
* @param[in]  event_id, the current event ID to be handled.
* @param[in]  extra_data, extra data pointer of the current event, NULL means there is no extra data.
* @param[in]  data_len, the length of the extra data. 0 means extra_data is NULL.
* @return     If return true, the current event cannot be handle by the next activity.
*/

bool app_fast_pair_proc_app_interaction_event(ui_shell_activity_t *self, uint32_t event_id, void *extra_data, size_t data_len)
{
    bool ret = false;

    switch (event_id) {
#ifdef AIR_TILE_ENABLE
        case APPS_EVENTS_INTERACTION_STOP_FAST_PAIR_ADV: {
            app_fast_pair_stop_advertising();
#ifdef AIR_TILE_ENABLE
            g_app_fast_pair_context.is_classic_bt_on = false;
            APPS_LOG_MSGID_I(LOG_TAG"APPS_EVENTS_INTERACTION_STOP_FAST_PAIR_ADV, classic bt on %d", 1, g_app_fast_pair_context.is_classic_bt_on);
#endif
            break;
        }
#endif
#ifdef RACE_FIND_ME_ENABLE
        case APPS_EVENTS_INTERACTION_NOTIFY_FIND_ME_STATE: {
            const app_find_me_context_t *fm_context = app_find_me_idle_activity_get_context();
            if (fm_context->tone != s_app_fast_pair_find_me_state.tone
#ifdef MTK_AWS_MCE_ENABLE
                || fm_context->peer_tone != s_app_fast_pair_find_me_state.peer_tone
#endif
               ) {
                s_app_fast_pair_find_me_state.tone = fm_context->tone;
#ifdef MTK_AWS_MCE_ENABLE
                s_app_fast_pair_find_me_state.peer_tone = fm_context->peer_tone;
#endif
                app_fast_pair_notify_ring_state_change();
            }
            break;
        }
#endif
#ifdef AIR_BT_FAST_PAIR_SASS_ENABLE
#if defined AIR_MULTI_POINT_ENABLE
        case APPS_EVENTS_INTERACTION_FAST_PAIR_CLEAR_ACTIVE_SWITCH_RECORD: {
            memset(g_app_fast_pair_context.allow_switch_audio_addr, 0, sizeof(bt_bd_addr_t));
            break;
        }
        case APPS_EVENTS_INTERACTION_FAST_PAIR_SASS_CLEAR_TEMP_ALLOW: {
            memset(g_app_fast_pair_context.temp_allow_switch_audio_addr, 0, sizeof(bt_bd_addr_t));
            break;
        }
#endif
        case APPS_EVENTS_INTERACTION_FAST_PAIR_SASS_UPDATE_CONNECTION_STATUS: {
            void app_fast_pair_connection_state_change(void);
            break;
        }
#ifdef MTK_IN_EAR_FEATURE_ENABLE
        case APPS_EVENTS_INTERACTION_IN_EAR_UPDATE_STA: {
            app_in_ear_sta_info_t *sta_info = (app_in_ear_sta_info_t *)extra_data;
            APPS_LOG_MSGID_I(LOG_TAG", [IN_EAR_UPDATE] %d->%d", 2, sta_info->previous, sta_info->current);
            if ((sta_info->previous == APP_IN_EAR_STA_BOTH_OUT && sta_info->current != APP_IN_EAR_STA_BOTH_OUT)
                || (sta_info->previous != APP_IN_EAR_STA_BOTH_OUT && sta_info->current == APP_IN_EAR_STA_BOTH_OUT)) {
                app_fast_pair_connection_state_change();
            }
            break;
        }
#endif
#endif
        default:
            break;
    }
    return ret;
}

bool app_fast_pair_idle_activity_proc(struct _ui_shell_activity *self, uint32_t event_group, uint32_t event_id, void *extra_data, size_t data_len)
{
    bool ret = false;
    /* APPS_LOG_MSGID_I("[APP_FAST_PAIR]APP_FAST_PAIR_IDLE_ACT, event_group : %x, evt: %x", 2, event_group, event_id); */
    switch (event_group) {
        case EVENT_GROUP_UI_SHELL_SYSTEM: {
            ret = app_proc_fast_pair_shell_group(self, event_id, extra_data, data_len);
            break;
        }
        case EVENT_GROUP_UI_SHELL_BATTERY: {
            ret = app_fast_pair_proc_battery_group(self, event_id, extra_data, data_len);
            break;
        }
        case EVENT_GROUP_UI_SHELL_BT:
            ret = app_fast_pair_proc_bt_group(self, event_id, extra_data, data_len);
            break;
        case EVENT_GROUP_UI_SHELL_BT_SINK: {
            ret = app_fast_pair_proc_bt_sink_group(self, event_id, extra_data, data_len);
            break;
        }
        case EVENT_GROUP_UI_SHELL_BT_CONN_MANAGER: {
            /* Trigger fast pair behavior */
            ret = app_fast_pair_proc_bt_cm_group(self, event_id, extra_data, data_len);
            break;
        }
        case EVENT_GROUP_UI_SHELL_BT_DEVICE_MANAGER: {
            /* Trigger fast pair behavior */
            ret = app_fast_pair_proc_bt_dm_group(self, event_id, extra_data, data_len);
            break;
        }
#ifdef MTK_AWS_MCE_ENABLE
        case EVENT_GROUP_UI_SHELL_AWS:
            ret = app_fast_pair_proc_aws_event(self, event_id, extra_data, data_len);
            break;
#endif
#ifdef AIR_SMART_CHARGER_ENABLE
        case EVENT_GROUP_UI_SHELL_CHARGER_CASE:
            ret = app_fast_pair_proc_charger_case_group(self, event_id, extra_data, data_len);
            break;
#endif
        case EVENT_GROUP_UI_SHELL_BT_FAST_PAIR: {
            ret = app_fast_pair_app_event_callback(self, event_id, extra_data, data_len);
            break;
        }
#ifdef MTK_AWS_MCE_ENABLE
        case EVENT_GROUP_UI_SHELL_AWS_DATA: {
            /* Just group currently */
            ret = app_fast_pair_aws_mce_data_callback(self, event_id, extra_data, data_len);
            break;
        }
#endif
        case EVENT_GROUP_UI_SHELL_MULTI_VA:
            ret = app_fast_pair_multi_va_event_proc(self, event_id, extra_data, data_len);
            break;

        case EVENT_GROUP_UI_SHELL_APP_INTERACTION:
            ret = app_fast_pair_proc_app_interaction_event(self, event_id, extra_data, data_len);
            break;

        default:
            break;
    }

    /* APPS_LOG_MSGID_I("[APP_FAST_PAIR]APP_FAST_PAIR_IDLE_ACT, ret : %x", 1, ret); */
    return ret;
}

/**
 * @brief     The implement of weak symbol function that returns the account key list buffer to fast pair module.
 * @return    A pointer to the account key buffer containing the account key buffer numbers and account key buffer. The pointer should not be NULL and should point to a global variable.
 */
bt_fast_pair_account_key_list_t *bt_fast_pair_get_account_key_list()
{
    return g_app_fast_pair_context.account_key_list;
}

void app_fast_pair_set_tx_power_level(int8_t tx_power)
{
    nvkey_status_t status = 0;
    app_fast_pair_nvkey_t temp_nvkey;
    uint32_t nvkey_size = sizeof(app_fast_pair_nvkey_t);
    if (NVKEY_STATUS_OK != (status = nvkey_read_data(NVID_APP_FAST_PAIR_CONFIGURE,
                                                     (uint8_t *)&temp_nvkey, &nvkey_size))) {
        APPS_LOG_MSGID_E("load configure failed : 0x%x", 1, status);
        return;
    }
    g_app_fast_pair_context.nvkey.tx_power_level = tx_power;
    temp_nvkey.tx_power_level = tx_power;
    if (NVKEY_STATUS_OK != (status = nvkey_write_data(NVID_APP_FAST_PAIR_CONFIGURE,
                                                      (uint8_t *)&temp_nvkey, sizeof(app_fast_pair_nvkey_t)))) {
        APPS_LOG_MSGID_E("load configure write failed : 0x%x", 1, status);
    }
}

void app_fast_pair_stop_advertising(void)
{
    APPS_LOG_MSGID_I(LOG_TAG" stop_advertising", 0);

    if (apps_config_features_is_mp_test_mode()) {
        return;
    }

    multi_ble_adv_manager_remove_ble_adv(MULTI_ADV_INSTANCE_FAST_PAIR, app_fast_pair_get_adv_data);
    multi_ble_adv_manager_notify_ble_adv_data_changed(MULTI_ADV_INSTANCE_FAST_PAIR);
}


bool app_fast_pair_get_is_waiting_connect(void)
{
#if APP_ONLY_ALLOW_TAKE_OVER_BY_FAST_PAIR
    return g_app_fast_pair_context.waiting_connect;
#else
    return true;
#endif
}

#ifdef AIR_BT_FAST_PAIR_SASS_ENABLE
void app_fast_pair_connection_state_change(void)
{
#if defined AIR_MULTI_POINT_ENABLE
    bool need_notify_switch = false;
    if (memcmp(g_app_fast_pair_context.switch_done_target_addr, empty_addr, sizeof(bt_bd_addr_t)) != 0) {
        if (g_app_fast_pair_context.switch_reason == BT_FAST_PAIR_SASS_MULTIPOINT_AUDIO_SRC_SWITCH_REASON_A2DP) {
            if (apps_config_key_get_mmi_state() == APP_A2DP_PLAYING
                || apps_config_key_get_mmi_state() == APP_ULTRA_LOW_LATENCY_PLAYING
                || apps_config_key_get_mmi_state() == APP_LE_AUDIO_BIS_PLAYING) {
                need_notify_switch = true;
            }
        } else if (g_app_fast_pair_context.switch_reason == BT_FAST_PAIR_SASS_MULTIPOINT_AUDIO_SRC_SWITCH_REASON_HFP) {
            if ((apps_config_key_get_mmi_state() >= APP_HFP_INCOMING && apps_config_key_get_mmi_state() <= APP_HFP_MULTITPART_CALL)
                || apps_config_key_get_mmi_state() == APP_STATE_HELD_ACTIVE) {
                need_notify_switch = true;
            }
        } else {
            if (apps_config_key_get_mmi_state() >= APP_CONNECTED) {
                need_notify_switch = true;
            }
        }
        if (need_notify_switch) {
            bt_fast_pair_sass_update_multipoint_audio_source_switch(&g_app_fast_pair_context.switch_done_target_addr, g_app_fast_pair_context.switch_reason);
            memset(g_app_fast_pair_context.switch_done_target_addr, 0, sizeof(bt_bd_addr_t));
        }
    }
#endif
    bt_fast_pair_sass_update_connection_state(true);
    if (!g_app_fast_pair_context.is_vis_on) {
        app_fast_pair_trigger_advertising(&g_app_fast_pair_context);
    }
}

#ifdef AIR_BT_SINK_SRV_STATE_MANAGER_ENABLE
bt_sink_srv_allow_result_t app_fast_pair_multi_link_get_allow_play(bt_sink_srv_device_state_t *current_device, bt_sink_srv_device_state_t *coming_device)
{
    bt_sink_srv_allow_result_t allow = BT_SINK_SRV_ALLOW_RESULT_BYPASS;
#ifdef AIR_MULTI_POINT_ENABLE
    if (current_device != NULL && coming_device != NULL) {
        uint8_t current_account_id = bt_fast_pair_sass_get_account_id_by_addr(&current_device->address);
        uint8_t coming_account_id = bt_fast_pair_sass_get_account_id_by_addr(&coming_device->address);
        APPS_LOG_MSGID_I(LOG_TAG" app_fast_pair_multi_link_get_allow_play current addr: %x:%x:%x:%x:%x:%x", 6,
                         current_device->address[0], current_device->address[1], current_device->address[2],
                         current_device->address[3], current_device->address[4], current_device->address[5]);
        APPS_LOG_MSGID_I(LOG_TAG" app_fast_pair_multi_link_get_allow_play coming addr: %x:%x:%x:%x:%x:%x", 6,
                         coming_device->address[0], coming_device->address[1], coming_device->address[2],
                         coming_device->address[3], coming_device->address[4], coming_device->address[5]);
        APPS_LOG_MSGID_I(LOG_TAG" app_fast_pair_multi_link_get_allow_play account_id %d - %d, prefer_flags : 0x%x", 3,
                         current_account_id, coming_account_id, g_app_fast_pair_context.sass_switch_prefer_flags);
        if (current_account_id == 0xFF && coming_account_id == 0xFF) {
            allow = BT_SINK_SRV_ALLOW_RESULT_BYPASS;
        } else {
            allow = BT_SINK_SRV_ALLOW_RESULT_ALLOW;
            if (current_device->music_state == BT_SINK_SRV_STATE_STREAMING && coming_device->music_state == BT_SINK_SRV_STATE_STREAMING) {
                if (!(g_app_fast_pair_context.sass_switch_prefer_flags & (1 << BT_FAST_PAIR_SASS_MULTIPOINT_SWITCH_A_A_BIT))) {
                    allow = BT_SINK_SRV_ALLOW_RESULT_ALLOW; /* Already allowed by switch active device message. */
                }
            } else if (current_device->call_state >= BT_SINK_SRV_STATE_INCOMING && coming_device->call_state >= BT_SINK_SRV_STATE_INCOMING) {
                if (!(g_app_fast_pair_context.sass_switch_prefer_flags & (1 << BT_FAST_PAIR_SASS_MULTIPOINT_SWITCH_H_H_BIT))) {
                    allow = BT_SINK_SRV_ALLOW_RESULT_DISALLOW;
                }
            } else if (current_device->call_state >= BT_SINK_SRV_STATE_INCOMING && coming_device->music_state == BT_SINK_SRV_STATE_STREAMING) {
                if (!(g_app_fast_pair_context.sass_switch_prefer_flags & (1 << BT_FAST_PAIR_SASS_MULTIPOINT_SWITCH_A_H_BIT))) {
                    allow = BT_SINK_SRV_ALLOW_RESULT_DISALLOW;
                }
            } else if (current_device->music_state == BT_SINK_SRV_STATE_STREAMING && coming_device->call_state >= BT_SINK_SRV_STATE_INCOMING) {
                if (!(g_app_fast_pair_context.sass_switch_prefer_flags & (1 << BT_FAST_PAIR_SASS_MULTIPOINT_SWITCH_H_A_BIT))) {
                    allow = BT_SINK_SRV_ALLOW_RESULT_DISALLOW;
                }
            }
        }
        if (allow == BT_SINK_SRV_ALLOW_RESULT_ALLOW) {
            ui_shell_remove_event(EVENT_GROUP_UI_SHELL_APP_INTERACTION,
                                  APPS_EVENTS_INTERACTION_FAST_PAIR_SASS_UPDATE_CONNECTION_STATUS);
            ui_shell_send_event(false, EVENT_PRIORITY_MIDDLE, EVENT_GROUP_UI_SHELL_APP_INTERACTION,
                                APPS_EVENTS_INTERACTION_FAST_PAIR_SASS_UPDATE_CONNECTION_STATUS, NULL, 0, NULL, 100);
        }
        if (allow == BT_SINK_SRV_ALLOW_RESULT_ALLOW) {
            bt_fast_pair_sass_multipoint_audio_src_switch_reason_t switch_reason = BT_FAST_PAIR_SASS_MULTIPOINT_AUDIO_SRC_SWITCH_REASON_UNSPECIFIED;
            if (coming_device->music_state == BT_SINK_SRV_STATE_STREAMING) {
                switch_reason = BT_FAST_PAIR_SASS_MULTIPOINT_AUDIO_SRC_SWITCH_REASON_A2DP;
            } else if (coming_device->music_state >= BT_SINK_SRV_STATE_INCOMING) {
                switch_reason = BT_SINK_SRV_STATE_INCOMING;
            }
            bt_fast_pair_sass_update_multipoint_audio_source_switch(&coming_device->address, switch_reason);
        }
    }
#endif
    APPS_LOG_MSGID_I(LOG_TAG" app_fast_pair_multi_link_get_allow_play result %d", 1, allow);
    return allow;
}
#endif

bool bt_fast_pair_sass_get_active_device_address(bt_bd_addr_t *addr)
{
    bool ret = false;
#ifdef AIR_MULTI_POINT_ENABLE
    bt_sink_srv_device_state_t device_state;
    bt_status_t bt_ret = bt_sink_srv_get_playing_device_state(&device_state);
    if (BT_STATUS_SUCCESS == bt_ret) {
        ret = true;
        memcpy(*addr, device_state.address, sizeof(bt_bd_addr_t));
    } else {
        ret = true;
        memset(*addr, 0, sizeof(bt_bd_addr_t));
    }
    APPS_LOG_MSGID_I(LOG_TAG" bt_fast_pair_sass_get_active_address bt_ret: 0x%x, addr: %x:%x:%x:%x:%x:%x", 7, bt_ret,
                     (*addr)[0], (*addr)[1], (*addr)[2], (*addr)[3], (*addr)[4], (*addr)[5]);
#else
    bt_bd_addr_t connected_devices[BT_MAX_LINK_NUM];
    uint32_t connected_devices_count = BT_MAX_LINK_NUM;
    uint32_t i;
    connected_devices_count = app_bt_state_service_get_connected_disinclude_aws(connected_devices, connected_devices_count);
    if (connected_devices_count > 0 && apps_config_key_get_mmi_state() > APP_CONNECTED) {
        memcpy(*addr, connected_devices[0], sizeof(bt_bd_addr_t));
    } else {
        memset(*addr, 0, sizeof(bt_bd_addr_t));
    }
#endif
    return ret;
}

#ifdef AIR_MULTI_POINT_ENABLE
int32_t bt_sink_srv_a2dp_int_user_conf_get(bt_bd_addr_ptr_t coming_addr, bt_bd_addr_ptr_t cur_addr)
{
    if (!(g_app_fast_pair_context.sass_switch_prefer_flags & (1 << BT_FAST_PAIR_SASS_MULTIPOINT_SWITCH_A_A_BIT))) {
        uint8_t account_coming = bt_fast_pair_sass_get_account_id_by_addr(coming_addr);
        uint8_t account_cur = bt_fast_pair_sass_get_account_id_by_addr(cur_addr);
        APPS_LOG_MSGID_I(LOG_TAG" 2 SP connected SASS use the same account key? account id : %d, %d, may need set", 1, account_coming, account_cur);
        if (0 == memcmp(g_app_fast_pair_context.temp_allow_switch_audio_addr, coming_addr, sizeof(bt_bd_addr_t))) {
            memset(g_app_fast_pair_context.temp_allow_switch_audio_addr, 0, sizeof(bt_bd_addr_t));
            ui_shell_remove_event(EVENT_GROUP_UI_SHELL_APP_INTERACTION, APPS_EVENTS_INTERACTION_FAST_PAIR_SASS_CLEAR_TEMP_ALLOW);
            return BT_SINK_SRV_A2DP_INT_USER_CONF_ALLOW;
        } else if (account_coming == 0xFF && account_cur == 0xFF) {
            return BT_SINK_SRV_A2DP_INT_USER_CONF_ALLOW;
        } else if (account_coming == account_cur) {
            if (0 == memcmp(g_app_fast_pair_context.allow_switch_audio_addr, coming_addr, sizeof(bt_bd_addr_t))) {
                return BT_SINK_SRV_A2DP_INT_USER_CONF_ALLOW;
            } else {
                return BT_SINK_SRV_A2DP_INT_USER_CONF_PENDING;
            }
        } else {
            if (account_cur == 0xFF
                && 0 == memcmp(g_app_fast_pair_context.allow_switch_audio_addr, coming_addr, sizeof(bt_bd_addr_t))) {
                /* Special case, sometimes the account key of cur_addr is 0xFF when SPP disconnected in switching back flow. */
                return BT_SINK_SRV_A2DP_INT_USER_CONF_ALLOW;
            } else {
                return BT_SINK_SRV_A2DP_INT_USER_CONF_DISALLOW;
            }
        }
    } else {
        return BT_SINK_SRV_A2DP_INT_USER_CONF_ALLOW;
    }
}
#endif

#endif
