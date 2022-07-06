/* Copyright Statement:
 *
 * (C) 2019 Airoha Technology Corp. All rights reserved.
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
 * the License Agreement ("Permitted User"). If you are not a Permitted User,
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
#include "app_mcp.h"

#include "apps_debug.h"

#include "ble_mcp_discovery.h"
#include "bt_gattc_discovery.h"

#ifdef AIR_LE_OTP_ENABLE
#include "ble_otp.h"
#include "ble_otp_discovery.h"
#include "ble_mcp.h"
#endif

/**************************************************************************************************
 * Define
**************************************************************************************************/
#define LOG_TAG     "[LEA][MCP]"

typedef struct {
    bt_gattc_discovery_characteristic_t  charc[BLE_MCS_MAX_CHARC_NUMBER];
    bt_gattc_discovery_descriptor_t      descrp[BLE_MCS_MAX_CHARC_NUMBER];
} app_mcp_discovery_charc_t;

#ifdef AIR_LE_OTP_ENABLE
typedef struct {
    bt_gattc_discovery_characteristic_t  charc[BLE_OTS_MAX_CHARC_NUMBER];
    bt_gattc_discovery_descriptor_t      descrp[BLE_OTS_MAX_CHARC_NUMBER];
} app_otp_discovery_charc_t;
#endif

/**************************************************************************************************
 * Variable
**************************************************************************************************/
static bt_gattc_discovery_service_t      app_lea_mcp_service;
static app_mcp_discovery_charc_t         app_lea_mcp_charc;
static app_le_audio_mcp_callback_t       app_lea_mcp_callback = NULL;

#ifdef AIR_LE_OTP_ENABLE
static bt_gattc_discovery_service_t g_otp_included_service;
static app_otp_discovery_charc_t g_otp_charc;
static uint8_t g_current_mcs_idx;
static bool is_last = FALSE;
#endif

/**************************************************************************************************
 * Prototype
**************************************************************************************************/
extern bool bt_le_audio_sink_is_link_valid(bt_handle_t handle);

#ifdef AIR_LE_OTP_ENABLE
bt_status_t app_mcp_set_discovery_included_service_range(uint16_t start_handle, uint16_t end_handle);
#endif

/**************************************************************************************************
 * Static function
**************************************************************************************************/
static void app_le_audio_mcp_set_attribute_callback(void)
{
#ifdef AIR_LE_OTP_ENABLE
    APPS_LOG_MSGID_I(LOG_TAG" app_mcp_set_attribute_callback is_last:%d mcs_idx:%d", 2, is_last, g_current_mcs_idx);

    if (is_last && (BLE_MCP_SERVICE_INDEX_GMCS == g_current_mcs_idx)) {
        uint16_t handle;
        ble_mcp_attribute_handle_range_t attr_handle_range;

        app_mcp_set_discovery_included_service_range(0, 0);
        if (BT_GATTC_DISCOVERY_STATUS_SUCCESS == bt_gattc_discovery_get_connect_handle(&handle)) {
            if (BT_STATUS_SUCCESS != ble_mcp_get_service_attribute_range(handle, BLE_MCP_SERVICE_INDEX_GMCS, &attr_handle_range)) {
                APPS_LOG_MSGID_I(LOG_TAG" ble_mcp_get_service_attribute_range fail", 0);
            } else {
                app_mcp_set_discovery_included_service_range(attr_handle_range.start_handle, attr_handle_range.end_handle);
            }
        }
    }
#endif

    bt_gattc_discovery_event_response();
}

static bool app_le_audio_mcp_discovery_callback(bt_gattc_discovery_event_t *event, bool is_gmcs)
{
    ble_mcp_characteristic_t charc[BLE_MCS_MAX_CHARC_NUMBER];
    ble_mcp_set_service_attribute_parameter_t param;
    bt_status_t bt_status;
    uint32_t i = 0;

    if (NULL == event) {
        return FALSE;
    }

    if (!bt_le_audio_sink_is_link_valid(event->conn_handle) ||
        (BT_GATTC_DISCOVERY_EVENT_FAIL == event->event_type && !event->last_instance)) {
        return FALSE;
    }

    APPS_LOG_MSGID_I(LOG_TAG" discovery_callback, is_gmcs=%d charc_num=%d",
                     2, is_gmcs, app_lea_mcp_service.char_count_found);

    memset(&param, 0, sizeof(ble_mcp_set_service_attribute_parameter_t));

    if (BT_GATTC_DISCOVERY_EVENT_COMPLETE == event->event_type) {
        if (0 != app_lea_mcp_service.char_count_found) {
            /* Fill MCP charc table */
            i = app_lea_mcp_service.char_count_found;
            while (i > 0) {
                i--;
                (charc + i)->uuid = app_lea_mcp_service.charateristics[i].char_uuid.uuid.uuid16;
                (charc + i)->value_handle = app_lea_mcp_service.charateristics[i].value_handle;
                (charc + i)->desc_handle = app_lea_mcp_service.charateristics[i].descriptor[0].handle;
            }
            param.charc = charc;
        }
        param.start_handle = app_lea_mcp_service.start_handle;
        param.end_handle = app_lea_mcp_service.end_handle;
        param.charc_num = app_lea_mcp_service.char_count_found;
    } else {
        APPS_LOG_MSGID_E(LOG_TAG" discovery_callback, error event_type=%d", 1, event->event_type);
    }

    param.is_gmcs = is_gmcs;
    param.is_complete = event->last_instance;
    param.callback = app_le_audio_mcp_set_attribute_callback;

    bt_status = ble_mcp_set_service_attribute(event->conn_handle, &param);
    APPS_LOG_MSGID_I(LOG_TAG" discovery_callback, bt_status=0x%08X", 1, bt_status);

    if (is_gmcs) {
        app_lea_mcp_callback(event->conn_handle, (0 != app_lea_mcp_service.char_count_found));
    }

    return (bt_status == BT_STATUS_SUCCESS);
}

#ifdef AIR_LE_OTP_ENABLE
static void app_otp_set_attribute_callback_included_service(uint8_t ots_idx)
{
    APPS_LOG_MSGID_I(LOG_TAG"[OTP] set attribute done, ots service_idx:%d", 1, ots_idx);
    ble_mcp_attribute_handle_range_t attr_handle_range;
    uint16_t handle;
    uint32_t num_mcs = 0;

    app_mcp_set_discovery_included_service_range(0, 0);
    if (BT_GATTC_DISCOVERY_STATUS_SUCCESS == bt_gattc_discovery_get_connect_handle(&handle) && BT_STATUS_SUCCESS == ble_mcp_get_service_number(handle, &num_mcs)) {
        APPS_LOG_MSGID_I(LOG_TAG"[OTP] set attribute done. included service. num of mcs: %d mcs_idx:%d", 2, num_mcs, g_current_mcs_idx);
        ble_mcp_set_ots_index(handle, g_current_mcs_idx, ots_idx);
        if (num_mcs) {
            if (BLE_MCP_SERVICE_INDEX_GMCS == g_current_mcs_idx) {
                num_mcs--;
            } else {
                if (num_mcs > 2 + g_current_mcs_idx) {
                    num_mcs -= (g_current_mcs_idx + 2);
                } else {
                    num_mcs = 0;
                }
            }
        }
        while (num_mcs) {
            if (BLE_MCP_SERVICE_INDEX_GMCS == g_current_mcs_idx) {
                /* GMCS is already handled in app_mcp_discovery_gmcs_callback() */
                g_current_mcs_idx = 0;
                continue;
            } else {
                g_current_mcs_idx++;
            }
            if (BT_STATUS_SUCCESS == ble_mcp_get_service_attribute_range(handle, g_current_mcs_idx, &attr_handle_range)) {
                app_mcp_set_discovery_included_service_range(attr_handle_range.start_handle, attr_handle_range.end_handle);
                break;
            }
            num_mcs--;
        }
    }
    bt_gattc_discovery_event_response();
}

static bool app_otp_discovery_callback_included_service(bt_gattc_discovery_event_t *event)
{
    ble_otp_characteristic_t charc[BLE_OTS_MAX_CHARC_NUMBER];
    ble_otp_set_service_attribute_parameter_t param;
    bt_status_t status;
    uint32_t i = 0;
    bt_gattc_discovery_service_t *p_service;

    if (NULL == event) {
        APPS_LOG_MSGID_I(LOG_TAG"[OTP] app_otp_discovery_callback_included_service null event", 0);
        return FALSE;
    }

    if (BT_GATTC_DISCOVERY_EVENT_COMPLETE != event->event_type) {
        if (BT_GATTC_DISCOVERY_ERROR_SERVICE_NOT_FOUND == event->params.error_code) {
            app_otp_set_attribute_callback_included_service(0xFF);
            return TRUE;
        }
        return FALSE;
    }

    p_service = &g_otp_included_service;

    APPS_LOG_MSGID_I(LOG_TAG"[OTP] app_otp_discovery_callback_included_service, charc_num:%d", 1, p_service->char_count_found);

    memset(&param, 0, sizeof(ble_otp_set_service_attribute_parameter_t));

    if (0 != p_service->char_count_found) {

        /* Fill OTP charc table */
        i = p_service->char_count_found;
        while (i > 0) {
            i--;
            (charc + i)->uuid = p_service->charateristics[i].char_uuid.uuid.uuid16;
            (charc + i)->value_handle = p_service->charateristics[i].value_handle;
            (charc + i)->desc_handle = p_service->charateristics[i].descriptor[0].handle;
        }
        param.charc = charc;
    }
    param.start_handle = p_service->start_handle;
    param.end_handle = p_service->end_handle;
    param.charc_num = p_service->char_count_found;

    param.is_complete = event->last_instance;
    param.callback = app_otp_set_attribute_callback_included_service;

    status = ble_otp_set_service_attribute(event->conn_handle, &param);
    APPS_LOG_MSGID_I(LOG_TAG"[OTP] app_otp_discovery_callback_included_service, status:%x last_instance:%d", 2, status, event->last_instance);

    if (BT_STATUS_SUCCESS != status) {
        return FALSE;
    }

    return TRUE;
}

#endif

static void app_le_audio_mcp_discovery_gmcs_callback(bt_gattc_discovery_event_t *event)
{
#ifdef AIR_LE_OTP_ENABLE
    is_last = event->last_instance;
    g_current_mcs_idx = BLE_MCP_SERVICE_INDEX_GMCS;
#endif

    if (!app_le_audio_mcp_discovery_callback(event, TRUE)) {
        bt_gattc_discovery_event_response();
    }
}

static void app_le_audio_mcp_discovery_mcs_callback(bt_gattc_discovery_event_t *event)
{
#ifdef AIR_LE_OTP_ENABLE
    is_last = event->last_instance;
#endif

    if (!app_le_audio_mcp_discovery_callback(event, FALSE)) {
        bt_gattc_discovery_event_response();
    }
}

#ifdef AIR_LE_OTP_ENABLE
static void app_otp_discovery_ots_included_service_callback(bt_gattc_discovery_event_t *event)
{
    APPS_LOG_MSGID_I(LOG_TAG"[OTP] app_otp_discovery_ots_included_service_callback last:%d", 1, event->last_instance);
    is_last = event->last_instance;
    if (!app_otp_discovery_callback_included_service(event)) {
        bt_gattc_discovery_event_response();
    }
}
#endif

/**************************************************************************************************
 * Public function
**************************************************************************************************/
void app_le_audio_mcp_init(app_le_audio_mcp_callback_t callback)
{
    if (NULL != app_lea_mcp_callback) {
        return;
    }
    app_lea_mcp_callback = callback;
    memset(&app_lea_mcp_charc, 0, sizeof(app_mcp_discovery_charc_t));
    int i = BLE_MCS_MAX_CHARC_NUMBER;
    while (i > 0) {
        i--;
        app_lea_mcp_charc.charc[i].descriptor_count = 1;
        app_lea_mcp_charc.charc[i].descriptor = &app_lea_mcp_charc.descrp[i];
    }

    memset(&app_lea_mcp_service, 0, sizeof(bt_gattc_discovery_service_t));
    app_lea_mcp_service.characteristic_count = BLE_MCS_MAX_CHARC_NUMBER;
    app_lea_mcp_service.charateristics = app_lea_mcp_charc.charc;

    ble_uuid_t service_uuid = {
        .type = BLE_UUID_TYPE_16BIT,
        .uuid.uuid16 = BT_GATT_UUID16_GENERIC_MEDIA_CONTROL_SERVICE,
    };
    bt_gattc_discovery_service_register(&service_uuid, &app_lea_mcp_service, app_le_audio_mcp_discovery_gmcs_callback);
    service_uuid.uuid.uuid16 = BT_GATT_UUID16_MEDIA_CONTROL_SERVICE;
    bt_gattc_discovery_service_register(&service_uuid, &app_lea_mcp_service, app_le_audio_mcp_discovery_mcs_callback);

#ifdef AIR_LE_OTP_ENABLE
    if (BT_STATUS_SUCCESS != ble_otp_init(APP_LE_AUDIO_MAX_LINK_NUM)) {
        return BT_STATUS_FAIL;
    }

    memset(&g_otp_charc, 0, sizeof(app_otp_discovery_charc_t));

    i = BLE_OTS_MAX_CHARC_NUMBER;
    while (i > 0) {
        i--;
        g_otp_charc.charc[i].descriptor_count = 1;
        g_otp_charc.charc[i].descriptor = &g_otp_charc.descrp[i];
    }

    /* register for included service */
    memset(&g_otp_included_service, 0, sizeof(bt_gattc_discovery_service_t));
    g_otp_included_service.characteristic_count = BLE_OTS_MAX_CHARC_NUMBER;
    g_otp_included_service.charateristics = g_otp_charc.charc;
    service_uuid.uuid.uuid16 = BLE_UUID16_OBJECT_TRANSFER_SERVICE;
    g_otp_included_service.is_included_definition = TRUE;
    bt_gattc_discovery_service_register(&service_uuid, &g_otp_included_service, app_otp_discovery_ots_included_service_callback);
#endif
}

#ifdef AIR_LE_OTP_ENABLE
bt_status_t app_mcp_set_discovery_included_service_range(uint16_t start_handle, uint16_t end_handle)
{
    APPS_LOG_MSGID_I(LOG_TAG"[OTP] app_mcp_set_discovery_included_service_range: 0x%x 0x%x 0x%x 0x%x", 4, g_otp_included_service.discovery_start_handle, g_otp_included_service.discovery_end_handle, start_handle, end_handle);
    g_otp_included_service.discovery_start_handle = start_handle;
    g_otp_included_service.discovery_end_handle = end_handle;
    return BT_STATUS_SUCCESS;
}
#endif

#endif

