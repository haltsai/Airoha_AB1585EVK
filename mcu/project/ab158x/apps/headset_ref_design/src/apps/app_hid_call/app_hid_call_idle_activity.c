/* Copyright Statement:
 *
 * (C) 2021  Airoha Technology Corp. All rights reserved.
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
/* Airoha restricted information */
#include "app_hid_call_idle_activity.h"
#include "ui_shell_activity.h"
#include "apps_config_event_list.h"
#include "apps_config_state_list.h"
#include "apps_debug.h"
#include "apps_events_event_group.h"
#include "FreeRTOS.h"
#ifdef AIR_AIRO_KEY_ENABLE
#include "airo_key_event.h"
#endif
#include "apps_events_key_event.h"
#include "apps_events_interaction_event.h"
#include "apps_config_key_remapper.h"
#include "apps_config_led_manager.h"
#include "apps_config_led_index_list.h"
#include "usbaudio_drv.h"

#include "timers.h"

/**************************************************************************************************
* Define
**************************************************************************************************/

#define APP_HID_CALL_MAX_CALL_NUM      1

#define APP_HID_CALL_CMD_LEN_ACCEPT_CALL    0x02
#define APP_HID_CALL_CMD_LEN_REJECT_CALL    0x02
#define APP_HID_CALL_CMD_LEN_END_CALL       0x02
#define APP_HID_CALL_CMD_LEN_HOLD_CALL      0x02

#define APP_HID_CALL_REPORT_ID_IDX          0
#define APP_HID_CALL_CALL_STATE_IDX         0x01
#define APP_HID_CALL_CALL_STATE_REPORT_ID   0x05
#define APP_HID_CALL_CALL_STATE_REPORT_LEN  0x02


#define APP_HID_CALL_RX_CALL_FLAG_OFF_HOOK  0x01
#define APP_HID_CALL_RX_CALL_FLAG_MUTE      0x02
#define APP_HID_CALL_RX_CALL_FLAG_RING      0x04
#define APP_HID_CALL_RX_CALL_FLAG_HOLD      0x08
#define APP_HID_CALL_RX_CALL_FLAG_MIC       0x10
#define APP_HID_CALL_RX_CALL_FLAG_LINE      0x20
#define APP_HID_CALL_RX_CALL_FLAG_RINGER    0x40


#define APP_HID_CALL_TX_CALL_FLAG_NONE              0x00
#define APP_HID_CALL_TX_CALL_FLAG_HOOK_SWITCH       0x01
#define APP_HID_CALL_TX_CALL_FLAG_PHONE_MUTE        0x02
#define APP_HID_CALL_TX_CALL_FLAG_REDIAL            0x04
#define APP_HID_CALL_TX_CALL_FLAG_FLASH             0x08
#define APP_HID_CALL_TX_CALL_FLAG_LINE_BUSY_TONE    0x10
#define APP_HID_CALL_TX_CALL_FLAG_LINE              0x20
#define APP_HID_CALL_TX_CALL_FLAG_SPEED_DIAL        0x40
#define APP_HID_CALL_TX_CALL_FLAG_HEADSET_ARRAY     0x80

#define APP_HID_CALL_CALL_STATUS_IDLE               0
#define APP_HID_CALL_CALL_STATUS_INCOMING           0x01
#define APP_HID_CALL_CALL_STATUS_OUTGOING           0x02
#define APP_HID_CALL_CALL_STATUS_ACCEPT             0x03
#define APP_HID_CALL_CALL_STATUS_ACTIVATE           0x04
#define APP_HID_CALL_CALL_STATUS_HELD               0x05
#define APP_HID_CALL_CALL_STATUS_ENDING             0x06
#define APP_HID_CALL_CALL_STATUS_ENDING2            0x07
#define APP_HID_CALL_CALL_STATUS_END                0x08
#define APP_HID_CALL_CALL_STATUS_MULTICALL          0x80    /* a bit mask for note multicall scenario*/
typedef uint8_t app_hid_call_status;

#define APP_HID_CALL_CALL_SET_MULTICALL(status)    (status |= APP_HID_CALL_CALL_STATUS_MULTICALL)
#define APP_HID_CALL_CALL_RESET_MULTICALL(status)  (status &= ~APP_HID_CALL_CALL_STATUS_MULTICALL)
#define APP_HID_CALL_CALL_IS_MULTICALL(status)     (status & APP_HID_CALL_CALL_STATUS_MULTICALL)

#define APP_HID_CALL_CALL_END_DELAY                 (2500)
#define APP_HID_CALL_CALL_CHECK_TERMIATE_DELAY      (100)
#define APP_HID_CALL_CALL_MUTE_DELAY                (8)
#define APP_HID_CALL_CALL_TERMINATE_DELAY           (3)
#define APP_HID_CALL_CALL_ACCEPT_DELAY              (10)
#define APP_HID_CALL_CALL_REJECT_DELAY              (3)
#define APP_HID_CALL_CALL_HOLD_DELAY                (3)
#define APP_HID_CALL_CALL_UNHOLD_DELAY              (3)

#define TAG "[APP HID CALL] "

typedef enum {
    USB_HID_TIMER_CHECK_TERMINATE,
    USB_HID_TIMER_CHECK_TERMINATE_CASE3,
    USB_HID_TIMER_CHECK_REMOTE_HOLD,
    USB_HID_TIMER_UNHOLD_TERMINATE,
    USB_HID_TIMER_TERMINATE_DELAY,
    USB_HID_TIMER_ACCEPT_DELAY,
    USB_HID_TIMER_REJECT_DELAY,
    USB_HID_TIMER_HOLD_DELAY,
    USB_HID_TIMER_UNHOLD_DELAY,
    USB_HID_TIMER_MUTE,
    USB_HID_TIMER_MAX
} app_hid_call_timer_type_t;

/**************************************************************************************************
* Structure
**************************************************************************************************/
typedef struct {
    app_hid_call_status call_status;
} app_hid_call_usb_hid_info;

typedef struct {
    app_hid_call_status next_status;
    uint8_t wait_event;
} app_hid_call_wait_event;

/**************************************************************************************************
* Variable
**************************************************************************************************/
static app_hid_call_usb_hid_info g_usb_hid_info[APP_HID_CALL_MAX_CALL_NUM];
static uint8_t g_usb_hid_call_event = 0;
static uint8_t g_usb_hid_call_need_terminate_idx = 0;
static TimerHandle_t g_usb_hid_timer[USB_HID_TIMER_MAX] = {0};
static app_hid_call_wait_event g_call_wait_event = {0};
static uint8_t g_current_usb_hid_state = APP_HID_CALL_TX_CALL_FLAG_NONE;
static app_hid_call_status g_termiate_bk_state = APP_HID_CALL_CALL_STATUS_IDLE;
static apps_config_key_action_t s_key_action_preparse_result = KEY_ACTION_INVALID;

/**************************************************************************************************
* Prototype
**************************************************************************************************/
static void app_usb_hid_send_call_cmd(uint8_t cmd, uint8_t length);
static void app_usb_hid_send_self_call_cmd(uint8_t *cmd, uint8_t length, uint32_t delay_ms);
static void app_hid_call_terminate_call(uint8_t call_idx);

/**************************************************************************************************
* Static Functions
**************************************************************************************************/
static uint32_t _get_timer_type(TimerHandle_t timer_id)
{
    uint32_t i = 0;
    for (i = 0; i < USB_HID_TIMER_MAX; i++) {
        if (g_usb_hid_timer[i] == timer_id) {
            return i;
        }
    }
    return USB_HID_TIMER_MAX;
}

static void app_hid_call_handle_timeout(TimerHandle_t timer_id)
{
    app_hid_call_timer_type_t timer_type = _get_timer_type(timer_id);
    APPS_LOG_MSGID_I(TAG"timeout! type %d", 1, timer_type);

    switch (timer_type) {
        case USB_HID_TIMER_CHECK_TERMINATE: {
            APPS_LOG_MSGID_I(TAG"timeout! CALL END FAILED", 0);
            app_hid_call_usb_hid_info *p_info = &g_usb_hid_info[0];
            p_info->call_status = g_termiate_bk_state;
            APPS_LOG_MSGID_I(TAG"timeout! CALL END FAILED, set state back to %d", 1, g_termiate_bk_state);
            break;
        }
        case USB_HID_TIMER_CHECK_TERMINATE_CASE3: {
            /* Call end */
            ui_shell_send_event(true,
                                EVENT_PRIORITY_HIGH,
                                EVENT_GROUP_UI_SHELL_USB_HID_CALL,
                                APP_HID_CALL_EVENT_USB_HID_CALL_ENDED,
                                NULL, 0, NULL, 0);
            // off-hook 1 means there is still call existing, not it as multicall
            APP_HID_CALL_CALL_SET_MULTICALL(g_call_wait_event.next_status);
            APPS_LOG_MSGID_I(TAG"timeout! CALL END with MULTI", 0);
            break;
        }
        case USB_HID_TIMER_CHECK_REMOTE_HOLD: {
            /* Call remotely hold */
            ui_shell_send_event(true,
                                EVENT_PRIORITY_HIGH,
                                EVENT_GROUP_UI_SHELL_USB_HID_CALL,
                                APP_HID_CALL_EVENT_USB_HID_CALL_REMOTE_HOLD,
                                NULL, 0, NULL, 0);
            break;
        }
        case USB_HID_TIMER_UNHOLD_TERMINATE: {
            APPS_LOG_MSGID_I(TAG"timeout! terminate call %d", 1, g_usb_hid_call_need_terminate_idx);
            if (g_usb_hid_call_need_terminate_idx != 0) {
                app_hid_call_terminate_call(g_usb_hid_call_need_terminate_idx);
                g_usb_hid_call_need_terminate_idx = 0;
            }
            break;
        }
        case USB_HID_TIMER_ACCEPT_DELAY: {
            app_usb_hid_send_call_cmd(APP_HID_CALL_TX_CALL_FLAG_NONE, APP_HID_CALL_CMD_LEN_ACCEPT_CALL);
            break;
        }
        case USB_HID_TIMER_TERMINATE_DELAY:
        case USB_HID_TIMER_REJECT_DELAY: {
            app_usb_hid_send_call_cmd(APP_HID_CALL_TX_CALL_FLAG_NONE, APP_HID_CALL_CMD_LEN_END_CALL);
            break;
        }
        case USB_HID_TIMER_HOLD_DELAY: {
            app_usb_hid_send_call_cmd(APP_HID_CALL_TX_CALL_FLAG_HOOK_SWITCH, APP_HID_CALL_CMD_LEN_HOLD_CALL);
            break;
        }
        case USB_HID_TIMER_UNHOLD_DELAY: {
            app_usb_hid_send_call_cmd(APP_HID_CALL_TX_CALL_FLAG_NONE, APP_HID_CALL_CMD_LEN_HOLD_CALL);
            break;
        }
        case USB_HID_TIMER_MUTE: {
            app_usb_hid_send_call_cmd((g_usb_hid_call_event & APP_HID_CALL_RX_CALL_FLAG_OFF_HOOK), 2);
            break;
        }
    }
}

static void app_hid_call_start_timer(uint32_t type, uint32_t timeout_ms)
{
    if (type >= USB_HID_TIMER_MAX) {
        return;
    }

    if (!g_usb_hid_timer[type]) {
        g_usb_hid_timer[type] = xTimerCreate("g_usb_hid_timer",
                                             (timeout_ms * portTICK_PERIOD_MS),
                                             pdFALSE, /* Repeat timer */
                                             NULL,
                                             app_hid_call_handle_timeout);
    }

    if (!g_usb_hid_timer[type]) {
        APPS_LOG_MSGID_I(TAG"start_timer %d, create timer failed!", 1, type);
        return;
    }

    if (pdFALSE == xTimerIsTimerActive(g_usb_hid_timer[type])) {
        xTimerStart(g_usb_hid_timer[type], 0);
        APPS_LOG_MSGID_I(TAG"start_timer %d for %d ms, done", 2, type, timeout_ms);
    }
}

static bool app_hid_call_stop_timer(uint32_t type)
{
    if (type >= USB_HID_TIMER_MAX) {
        return false;
    }

    if (!g_usb_hid_timer[type]) {
        return false;
    }

    if (pdTRUE != xTimerIsTimerActive(g_usb_hid_timer[type])) {
        return false;
    }

    xTimerStop(g_usb_hid_timer[type], 0);
    APPS_LOG_MSGID_I(TAG"stop_timer %d, done", 1, type);
    return true;
}


static app_hid_call_usb_hid_info *app_hid_call_get_call_info_by_state(app_hid_call_status call_status)
{
    uint8_t i;

    for (i = 0; i < APP_HID_CALL_MAX_CALL_NUM; i++) {
        if (call_status == g_usb_hid_info[i].call_status) {
            return &g_usb_hid_info[i];
        }
    }

    return NULL;
}

static app_hid_call_usb_hid_info *app_hid_call_get_call_info_by_index(uint8_t call_idx)
{
    return &g_usb_hid_info[0];
}

/**************************************************************************************************
* Public Functions
**************************************************************************************************/
static bool app_hid_call_handle_incoming_call(void)
{
    app_hid_call_usb_hid_info *p_info;

    if (NULL == (p_info = app_hid_call_get_call_info_by_state(APP_HID_CALL_CALL_STATUS_IDLE))) {
        APPS_LOG_MSGID_I(TAG"handle_incoming_call ERROR, call full!", 0);
        return false;
    }

    memset(p_info, 0, sizeof(app_hid_call_usb_hid_info));
    p_info->call_status = APP_HID_CALL_CALL_STATUS_INCOMING;

    //TODO, check
    /*
    if (BLE_TBS_INVALID_CALL_INDEX == (call_idx = bt_le_audio_source_call_set_incoming_call(ble_tbs_get_gtbs_service_idx(), uri, 8, NULL, 0))) {
        APPS_LOG_MSGID_I(TAG"handle_incoming_call ERROR, set call fail!", 0);
        p_info->call_status = APP_HID_CALL_CALL_STATUS_IDLE;
        return true;
    }
    */

    APPS_LOG_MSGID_I(TAG"handle_incoming_call", 0);

    //TODO, check app_le_audio_ucst_start();
    return true;
}

static bool app_hid_call_handle_outgoing_call(void)
{
    app_hid_call_usb_hid_info *p_info;

    if (NULL == (p_info = app_hid_call_get_call_info_by_state(APP_HID_CALL_CALL_STATUS_IDLE))) {
        APPS_LOG_MSGID_I(TAG"handle_outgoing_call ERROR, call full!", 0);
        return false;
    }

    memset(p_info, 0, sizeof(app_hid_call_usb_hid_info));
    p_info->call_status = APP_HID_CALL_CALL_STATUS_OUTGOING;

    //TODO, check
    /*
    if (BLE_TBS_INVALID_CALL_INDEX == (call_idx = bt_le_audio_source_call_originate(ble_tbs_get_gtbs_service_idx(), uri, 8))) {
        APPS_LOG_MSGID_I(TAG"handle_outgoing_call ERROR, originate fail!", 0);
        p_info->call_status = APP_HID_CALL_CALL_STATUS_IDLE;
        return false;
    }
    */

    APPS_LOG_MSGID_I(TAG"handle_outgoing_call", 0);

    //TODO, check
    /*
    bt_le_audio_source_call_accept_t alerting = {
        .service_index = ble_tbs_get_gtbs_service_idx(),
        .call_index = call_idx
    };
    */

    //TODO, check
    /*
    if (BT_STATUS_SUCCESS != bt_le_audio_source_send_action(BT_LE_AUDIO_SOURCE_ACTION_CALL_ALERTING, &alerting)) {
        APPS_LOG_MSGID_I(TAG"handle_outgoing_call ERROR, alerting fail!", 0);
        p_info->call_status = APP_HID_CALL_CALL_STATUS_IDLE;
        return false;
    }
    */

    return true;
}

static bool app_hid_call_handle_call_active(void)
{
    app_hid_call_usb_hid_info *p_info = NULL;
    uint8_t i;

    for (i = 0; i < APP_HID_CALL_MAX_CALL_NUM; i++) {
        if (APP_HID_CALL_CALL_STATUS_IDLE == g_usb_hid_info[i].call_status) {
            /* PC will send USB HID event ACTIVATE without incoming stage when
               1. outgoing call
               2. switch active device during call
               Recognize this situation as a very short Outgoing stage and then transit to Active stage
             */
            APPS_LOG_MSGID_I(TAG"handle_call_active, transit to OUTGOING", 0);
            app_hid_call_handle_outgoing_call();
        }

        if ((APP_HID_CALL_CALL_STATUS_INCOMING <= g_usb_hid_info[i].call_status) &&
            (APP_HID_CALL_CALL_STATUS_ACCEPT >= g_usb_hid_info[i].call_status)) {
            p_info = &g_usb_hid_info[i];
        }
    }

    if (NULL == p_info) {
        APPS_LOG_MSGID_I(TAG"handle_call_active ERROR, call not found!", 0);
        return false;
    }

    APPS_LOG_MSGID_I(TAG"handle_call_active, call_status:%x", 1, p_info->call_status);

    switch (p_info->call_status) {
        case APP_HID_CALL_CALL_STATUS_INCOMING: {
            p_info->call_status = APP_HID_CALL_CALL_STATUS_ACCEPT;

            //TODO, check
            /*
            bt_le_audio_source_call_accept_t param;
            param.service_index = ble_tbs_get_gtbs_service_idx();
            param.call_index = p_info->call_idx;
            ret = bt_le_audio_source_send_action(BT_LE_AUDIO_SOURCE_ACTION_CALL_ACCEPT, &param);
            */
            //break;
        }
        /* fall through*/
        case APP_HID_CALL_CALL_STATUS_OUTGOING:
        case APP_HID_CALL_CALL_STATUS_ACCEPT: {
            p_info->call_status = APP_HID_CALL_CALL_STATUS_ACTIVATE;
            //TODO, check
            /*
            bt_le_audio_source_call_active_t param;
            param.service_index = ble_tbs_get_gtbs_service_idx();
            param.call_index = p_info->call_idx;
            ret = bt_le_audio_source_send_action(BT_LE_AUDIO_SOURCE_ACTION_CALL_ACTIVE, &param);
            */
            break;
        }
        default:
            break;
    }

    //APPS_LOG_MSGID_I(TAG"handle_call_active, ret:%x call_status:%x", 2, ret, p_info->call_status);

    return true;
}

static bool app_hid_call_handle_call_end(void)
{
    app_hid_call_usb_hid_info *p_info = NULL;
    uint8_t i;

    for (i = 0; i < APP_HID_CALL_MAX_CALL_NUM; i++) {
        if ((APP_HID_CALL_CALL_STATUS_INCOMING <= g_usb_hid_info[i].call_status) &&
            (APP_HID_CALL_CALL_STATUS_END >= g_usb_hid_info[i].call_status)) {
            p_info = &g_usb_hid_info[i];
        }
    }

    if (NULL == p_info) {
        APPS_LOG_MSGID_I(TAG"handle_call_end ERROR, call not found!", 0);
        return false;
    }

    APPS_LOG_MSGID_I(TAG"handle_call_end, call_status:%x", 1, p_info->call_status);

    if (APP_HID_CALL_CALL_STATUS_ENDING == p_info->call_status || APP_HID_CALL_CALL_STATUS_ENDING2 == p_info->call_status) {
        //TODO, check
        p_info->call_status = APP_HID_CALL_CALL_STATUS_IDLE;
        /*
        bt_le_audio_source_call_ended_t le_param;
        uint8_t call_idx;
        call_idx = p_info->call_idx;
        le_param.service_index = ble_tbs_get_gtbs_service_idx();
        le_param.call_index = call_idx;
        ret = bt_le_audio_source_send_action(BT_LE_AUDIO_SOURCE_ACTION_CALL_ENDED, &le_param);
        */
    } else {
        APPS_LOG_MSGID_I(TAG"handle_call_end, pre status is %d", 1, p_info->call_status);
        p_info->call_status = APP_HID_CALL_CALL_STATUS_IDLE;
        //TODO, check
        /*
        bt_le_audio_source_call_terminate_t param;
        param.service_index = ble_tbs_get_gtbs_service_idx();
        param.call_index = p_info->call_idx;
        param.reason = BLE_TBS_TERMINATION_REASON_REMOTE_PARTY_END_CALL;
        ret = bt_le_audio_source_send_action(BT_LE_AUDIO_SOURCE_ACTION_CALL_TERMINATE, &param);
        */
    }

    //APPS_LOG_MSGID_I(TAG"handle_call_end, ret:%x call_status:%x", 2, ret, p_info->call_status);

    return true;
}

static bool app_hid_call_handle_call_hold(void)
{
    app_hid_call_usb_hid_info *p_info = NULL;
    uint8_t i;

    for (i = 0; i < APP_HID_CALL_MAX_CALL_NUM; i++) {
        if (APP_HID_CALL_CALL_STATUS_ACTIVATE == g_usb_hid_info[i].call_status) {
            p_info = &g_usb_hid_info[i];
        }
    }

    if (NULL == p_info) {
        APPS_LOG_MSGID_I(TAG"handle_call_hold ERROR, call not found!", 0);
        return false;
    }

    APPS_LOG_MSGID_I(TAG"handle_call_hold, call_status:%x", 1, p_info->call_status);

    //TODO, check
    p_info->call_status = APP_HID_CALL_CALL_STATUS_HELD;
    /*
    bt_le_audio_source_call_hold_t param;
    param.service_index = ble_tbs_get_gtbs_service_idx();
    param.call_index = p_info->call_idx;
    ret = bt_le_audio_source_send_action(BT_LE_AUDIO_SOURCE_ACTION_CALL_HOLD, &param);
    */
    //APPS_LOG_MSGID_I(TAG"handle_call_active, ret:%x call_status:%x", 2, ret, p_info->call_status);

    //app_usb_hid_send_call_cmd(APP_HID_CALL_TX_CALL_FLAG_LINE_BUSY_TONE | APP_HID_CALL_TX_CALL_FLAG_LINE, APP_HID_CALL_CMD_LEN_HOLD_CALL);

    return true;
}

static bool app_hid_call_handle_call_unhold(void)
{
    app_hid_call_usb_hid_info *p_info = NULL;
    uint8_t i;

    for (i = 0; i < APP_HID_CALL_MAX_CALL_NUM; i++) {
        if (APP_HID_CALL_CALL_STATUS_HELD == g_usb_hid_info[i].call_status) {
            p_info = &g_usb_hid_info[i];
        }
    }

    if (NULL == p_info) {
        APPS_LOG_MSGID_I(TAG"handle_call_unhold ERROR, call not found!", 0);
        return false;
    }

    APPS_LOG_MSGID_I(TAG"handle_call_unhold, call_status:%x", 1, p_info->call_status);

    //TODO, check
    p_info->call_status = APP_HID_CALL_CALL_STATUS_ACTIVATE;
    /*
    bt_le_audio_source_call_unhold_t param;
    param.service_index = ble_tbs_get_gtbs_service_idx();
    param.call_index = p_info->call_idx;
    ret = bt_le_audio_source_send_action(BT_LE_AUDIO_SOURCE_ACTION_CALL_UNHOLD, &param);
    */
    //APPS_LOG_MSGID_I(TAG"handle_call_active, ret:%x call_status:%x", 2, ret, p_info->call_status);

    //app_usb_hid_send_call_cmd(APP_HID_CALL_TX_CALL_FLAG_HOOK_SWITCH | APP_HID_CALL_TX_CALL_FLAG_LINE_BUSY_TONE | APP_HID_CALL_TX_CALL_FLAG_LINE, APP_HID_CALL_CMD_LEN_HOLD_CALL);

    return true;
}

static void app_hid_call_rx_callback(uint8_t *p_data, uint32_t size)
{
    uint8_t call_event;

    if (NULL == p_data) {
        APPS_LOG_MSGID_I(TAG"rx_callback ERROR, invalid param", 0);
        return;
    }

    if ((APP_HID_CALL_CALL_STATE_REPORT_LEN != size) || (APP_HID_CALL_CALL_STATE_REPORT_ID != p_data[APP_HID_CALL_REPORT_ID_IDX])) {
        APPS_LOG_MSGID_I(TAG"rx_callback ERROR, invalid report_id:%x size:%x", 2, p_data[APP_HID_CALL_REPORT_ID_IDX], size);
        return;
    }

    APPS_LOG_MSGID_I(TAG"rx_callback, data:%x %x %x call_event:%x, next: %x, wait: %x", 6,
                     p_data[0], p_data[1], p_data[2], g_usb_hid_call_event, g_call_wait_event.next_status, g_call_wait_event.wait_event);

    call_event = p_data[APP_HID_CALL_CALL_STATE_IDX];

    if (APP_HID_CALL_CALL_STATUS_ENDING == g_call_wait_event.next_status) {
        APPS_LOG_MSGID_I(TAG"ending1", 0);

        app_hid_call_stop_timer(USB_HID_TIMER_CHECK_TERMINATE);

        switch (call_event) {
            case 0:
                /* Terminate case I*/
                ui_shell_send_event(true,
                                    EVENT_PRIORITY_HIGH,
                                    EVENT_GROUP_UI_SHELL_USB_HID_CALL,
                                    APP_HID_CALL_EVENT_USB_HID_CALL_ENDED,
                                    NULL, 0, NULL, 0);
                g_call_wait_event.next_status = APP_HID_CALL_CALL_STATUS_IDLE;
                g_call_wait_event.wait_event = APP_HID_CALL_TX_CALL_FLAG_NONE;
                break;
            case 1:
                /* Terminate case II or III, wait if there is 05 00 00 */
                g_call_wait_event.next_status = APP_HID_CALL_CALL_STATUS_ENDING2;
                g_call_wait_event.wait_event = APP_HID_CALL_TX_CALL_FLAG_NONE;
                app_hid_call_start_timer(USB_HID_TIMER_CHECK_TERMINATE_CASE3, APP_HID_CALL_CALL_CHECK_TERMIATE_DELAY);
                break;
            default:
                break;
        }
    } else if (APP_HID_CALL_CALL_STATUS_ENDING2 == g_call_wait_event.next_status) {
        APPS_LOG_MSGID_I(TAG"ending2", 0);
        switch (call_event) {
            case 0: {
                app_hid_call_stop_timer(USB_HID_TIMER_CHECK_TERMINATE_CASE3);
                /* Terminate case II*/
                ui_shell_send_event(true,
                                    EVENT_PRIORITY_HIGH,
                                    EVENT_GROUP_UI_SHELL_USB_HID_CALL,
                                    APP_HID_CALL_EVENT_USB_HID_CALL_ENDED,
                                    NULL, 0, NULL, 0);
                g_call_wait_event.next_status = APP_HID_CALL_CALL_STATUS_IDLE;
                g_call_wait_event.wait_event = APP_HID_CALL_TX_CALL_FLAG_NONE;
                break;
            }
            case 1: {
                if (app_hid_call_stop_timer(USB_HID_TIMER_CHECK_TERMINATE_CASE3)) {
                    /* terminate failed */
                    APPS_LOG_MSGID_I(TAG"timeout! CALL END FAILED", 0);
                    g_call_wait_event.next_status = APP_HID_CALL_CALL_STATUS_IDLE;
                    g_call_wait_event.wait_event = APP_HID_CALL_TX_CALL_FLAG_NONE;
                }
                break;
            }
        }
    }

#if 0
    if ((g_usb_hid_call_event & APP_HID_CALL_RX_CALL_FLAG_OFF_HOOK) == (p_data[APP_HID_CALL_CALL_STATE_IDX]&APP_HID_CALL_RX_CALL_FLAG_OFF_HOOK)) {
        if (APP_HID_CALL_CALL_STATUS_END == g_call_wait_event.next_status) {
            /* Call end */
            ui_shell_send_event(true,
                                EVENT_PRIORITY_HIGH,
                                EVENT_GROUP_UI_SHELL_USB_HID_CALL,
                                APP_HID_CALL_EVENT_USB_HID_CALL_ENDED,
                                NULL, 0, NULL, 0);
            // off-hook 1 means there is still call existing, not it as multicall
            APP_HID_CALL_CALL_SET_MULTICALL(g_call_wait_event.next_status);
            APPS_LOG_MSGID_I(TAG"rx_callback CALL END with MULTI", 0);
            return;
        }
    }
#endif

    if (g_usb_hid_call_event == p_data[APP_HID_CALL_CALL_STATE_IDX]) {
        return;
    }

    switch (g_call_wait_event.next_status) {
        case APP_HID_CALL_CALL_STATUS_ACCEPT: {
            if (call_event == g_call_wait_event.wait_event) {
                app_usb_hid_send_call_cmd(APP_HID_CALL_TX_CALL_FLAG_HOOK_SWITCH, APP_HID_CALL_CMD_LEN_ACCEPT_CALL);
            }
            g_call_wait_event.next_status = APP_HID_CALL_CALL_STATUS_IDLE;
            return;
        }
        default:
            break;
    }

    if (!(APP_HID_CALL_RX_CALL_FLAG_OFF_HOOK & call_event) && (APP_HID_CALL_RX_CALL_FLAG_OFF_HOOK & g_usb_hid_call_event)) {
        APPS_LOG_MSGID_I(TAG"rx_callback off-hook 1->0", 0);
        /* Off-hook from 1 to 0, send 05 00 00 */
        app_usb_hid_send_call_cmd(APP_HID_CALL_TX_CALL_FLAG_NONE, 2);
    } else if ((APP_HID_CALL_RX_CALL_FLAG_OFF_HOOK & call_event) && !(APP_HID_CALL_RX_CALL_FLAG_OFF_HOOK & g_usb_hid_call_event)) {
        APPS_LOG_MSGID_I(TAG"rx_callback off-hook 0->1", 0);
        /* Off-hook from 0 to 1, send 05 01 00 */
        app_usb_hid_send_call_cmd(APP_HID_CALL_TX_CALL_FLAG_HOOK_SWITCH, 2);
    }

    if ((call_event & APP_HID_CALL_RX_CALL_FLAG_RING) &&
        (!(g_usb_hid_call_event & APP_HID_CALL_RX_CALL_FLAG_RING))) {

        /* Inoming call */
        ui_shell_send_event(true,
                            EVENT_PRIORITY_HIGH,
                            EVENT_GROUP_UI_SHELL_USB_HID_CALL,
                            APP_HID_CALL_EVENT_USB_HID_INCOMING_CALL,
                            NULL, 0, NULL, 0);

    } else if ((call_event & APP_HID_CALL_RX_CALL_FLAG_OFF_HOOK) &&
               (!(g_usb_hid_call_event & APP_HID_CALL_RX_CALL_FLAG_OFF_HOOK) ||
                (!((call_event ^ g_usb_hid_call_event)&APP_HID_CALL_RX_CALL_FLAG_MUTE) && APP_HID_CALL_CALL_IS_MULTICALL(g_call_wait_event.next_status)))) {

        if (app_hid_call_stop_timer(USB_HID_TIMER_CHECK_TERMINATE)) {
            /* Call unhold */
            ui_shell_send_event(true,
                                EVENT_PRIORITY_HIGH,
                                EVENT_GROUP_UI_SHELL_USB_HID_CALL,
                                APP_HID_CALL_EVENT_USB_HID_CALL_UNHOLD,
                                NULL, 0, NULL, 0);
        } else {
            if (APP_HID_CALL_TX_CALL_FLAG_HOOK_SWITCH != (g_current_usb_hid_state & APP_HID_CALL_TX_CALL_FLAG_HOOK_SWITCH)) {
                app_usb_hid_send_call_cmd(APP_HID_CALL_TX_CALL_FLAG_HOOK_SWITCH, 2);
            }

            /* Call active */
            ui_shell_send_event(true,
                                EVENT_PRIORITY_HIGH,
                                EVENT_GROUP_UI_SHELL_USB_HID_CALL,
                                APP_HID_CALL_EVENT_USB_HID_CALL_ACTIVATE,
                                NULL, 0, NULL, 0);

            if ((APP_HID_CALL_CALL_STATUS_MULTICALL | APP_HID_CALL_CALL_STATUS_ACCEPT) == g_call_wait_event.next_status) {
                app_hid_call_start_timer(USB_HID_TIMER_ACCEPT_DELAY, 50);
            }
        }
    } else if (!(call_event & APP_HID_CALL_RX_CALL_FLAG_HOLD) && (g_usb_hid_call_event & APP_HID_CALL_RX_CALL_FLAG_HOLD)) {
        /* Call local unhold */
        ui_shell_send_event(true,
                            EVENT_PRIORITY_HIGH,
                            EVENT_GROUP_UI_SHELL_USB_HID_CALL,
                            APP_HID_CALL_EVENT_USB_HID_CALL_UNHOLD,
                            NULL, 0, NULL, 0);
    } else if (call_event & APP_HID_CALL_RX_CALL_FLAG_HOLD) {
        if ((call_event & APP_HID_CALL_RX_CALL_FLAG_OFF_HOOK)
            && !(g_usb_hid_call_event & APP_HID_CALL_RX_CALL_FLAG_HOLD)) {
            app_hid_call_start_timer(USB_HID_TIMER_CHECK_REMOTE_HOLD, 50);
        } else {
            app_hid_call_stop_timer(USB_HID_TIMER_CHECK_REMOTE_HOLD);
            /* Call local hold */
            ui_shell_send_event(true,
                                EVENT_PRIORITY_HIGH,
                                EVENT_GROUP_UI_SHELL_USB_HID_CALL,
                                APP_HID_CALL_EVENT_USB_HID_CALL_HOLD,
                                NULL, 0, NULL, 0);
        }
    } else if ((call_event & APP_HID_CALL_RX_CALL_FLAG_OFF_HOOK) &&
               (call_event & APP_HID_CALL_RX_CALL_FLAG_MUTE) &&
               (!(g_usb_hid_call_event & APP_HID_CALL_RX_CALL_FLAG_MUTE))) {

        /* MIC MUTE  */
        ui_shell_send_event(true,
                            EVENT_PRIORITY_HIGH,
                            EVENT_GROUP_UI_SHELL_USB_HID_CALL,
                            APP_HID_CALL_EVENT_USB_HID_MIC_MUTE,
                            NULL, 0, NULL, 0);

    } else if ((call_event & APP_HID_CALL_RX_CALL_FLAG_OFF_HOOK) &&
               (!(call_event & APP_HID_CALL_RX_CALL_FLAG_MUTE)) &&
               (g_usb_hid_call_event & APP_HID_CALL_RX_CALL_FLAG_MUTE)) {

        /* MIC UNMUTE  */
        ui_shell_send_event(true,
                            EVENT_PRIORITY_HIGH,
                            EVENT_GROUP_UI_SHELL_USB_HID_CALL,
                            APP_HID_CALL_EVENT_USB_HID_MIC_UNMUTE,
                            NULL, 0, NULL, 0);
    } else if (call_event == 0) {
        if (APP_HID_CALL_CALL_IS_MULTICALL(g_call_wait_event.next_status)) {
            g_call_wait_event.next_status = APP_HID_CALL_CALL_STATUS_IDLE;
            APPS_LOG_MSGID_I(TAG"rx_callback CALL END no MULTI, back to IDLE", 0);
        } else if (g_call_wait_event.next_status == APP_HID_CALL_CALL_STATUS_IDLE) {
            /* Call end */
            ui_shell_send_event(true,
                                EVENT_PRIORITY_HIGH,
                                EVENT_GROUP_UI_SHELL_USB_HID_CALL,
                                APP_HID_CALL_EVENT_USB_HID_CALL_ENDED,
                                NULL, 0, NULL, 0);
        }
#if 0
        else {
            if (g_usb_hid_call_event & (APP_HID_CALL_RX_CALL_FLAG_HOLD | APP_HID_CALL_RX_CALL_FLAG_OFF_HOOK | APP_HID_CALL_RX_CALL_FLAG_RING)) {
                /* Need to wait for several time to make sure there is no hook-switch */
                app_hid_call_start_timer(USB_HID_TIMER_CHECK_TERMINATE, APP_HID_CALL_CALL_CHECK_TERMIATE_DELAY);
            } else {

                if (APP_HID_CALL_TX_CALL_FLAG_NONE != (g_current_usb_hid_state & APP_HID_CALL_TX_CALL_FLAG_HOOK_SWITCH)) {
                    app_usb_hid_send_call_cmd(APP_HID_CALL_TX_CALL_FLAG_NONE, 2);
                }

                /* Call end */
                ui_shell_send_event(true,
                                    EVENT_PRIORITY_HIGH,
                                    EVENT_GROUP_UI_SHELL_USB_HID_CALL,
                                    APP_HID_CALL_EVENT_USB_HID_CALL_ENDED,
                                    NULL, 0, NULL, 0);
            }
        }
#endif
    }

    g_usb_hid_call_event = call_event;
}

static void app_usb_hid_send_call_cmd(uint8_t cmd, uint8_t length)
{
    USB_Audio_HID_t ret;
    uint8_t buf[5] = {APP_HID_CALL_CALL_STATE_REPORT_ID, 0, 0, 0, 0};

    buf[1] = cmd;
    ret = USB_Audio_HID_TX(buf, length);

    APPS_LOG_MSGID_I(TAG"send_call_cmd, cmd:%x length:%x ret:%x", 3, cmd, length, ret);
    g_current_usb_hid_state = cmd;
}

static void app_usb_hid_send_self_call_cmd(uint8_t *cmd, uint8_t length, uint32_t delay_ms)
{
    ui_shell_send_event(false,
                        EVENT_PRIORITY_HIGH,
                        EVENT_GROUP_UI_SHELL_USB_HID_CALL,
                        APP_HID_CALL_EVENT_SELF_CTRL,
                        cmd, length, NULL, delay_ms);
}

static void app_hid_call_accept_call(uint8_t call_idx)
{
    app_hid_call_usb_hid_info *p_info = NULL;

    if (NULL == (p_info = app_hid_call_get_call_info_by_index(call_idx))) {
        APPS_LOG_MSGID_I(TAG"accept_call ERROR, call not exist!", 0);
        return;
    }

    APPS_LOG_MSGID_I(TAG"accept_call, call_status:%x", 1, p_info->call_status);

    if (APP_HID_CALL_CALL_STATUS_INCOMING == p_info->call_status) {
        p_info->call_status = APP_HID_CALL_CALL_STATUS_ACCEPT;

        // if multicall, use different method to accept call
        if (APP_HID_CALL_CALL_IS_MULTICALL(g_call_wait_event.next_status)) {
            /* 05 08 00 -> 10ms -> 05 00 00*/
            g_call_wait_event.next_status = APP_HID_CALL_CALL_STATUS_ACCEPT;
            APP_HID_CALL_CALL_SET_MULTICALL(g_call_wait_event.next_status);
            g_call_wait_event.wait_event =  APP_HID_CALL_RX_CALL_FLAG_RING | APP_HID_CALL_RX_CALL_FLAG_OFF_HOOK;

            app_usb_hid_send_call_cmd(APP_HID_CALL_TX_CALL_FLAG_FLASH, APP_HID_CALL_CMD_LEN_ACCEPT_CALL);
            app_hid_call_start_timer(USB_HID_TIMER_ACCEPT_DELAY, APP_HID_CALL_CALL_ACCEPT_DELAY);
        } else {
            /* 05 01 00 */
            g_call_wait_event.next_status = APP_HID_CALL_CALL_STATUS_ACCEPT;
            g_call_wait_event.wait_event =  APP_HID_CALL_RX_CALL_FLAG_RING | APP_HID_CALL_RX_CALL_FLAG_OFF_HOOK;
            app_usb_hid_send_call_cmd(APP_HID_CALL_TX_CALL_FLAG_HOOK_SWITCH, APP_HID_CALL_CMD_LEN_ACCEPT_CALL);
        }
    }
}

static void app_hid_call_hold_call(uint8_t call_idx)
{
    app_hid_call_usb_hid_info *p_info = NULL;

    if (NULL == (p_info = app_hid_call_get_call_info_by_index(call_idx))) {
        APPS_LOG_MSGID_I(TAG"hold_call ERROR, call not exist!", 0);
        return;
    }

    APPS_LOG_MSGID_I(TAG"hold_call, call_status:%x", 1, p_info->call_status);

    if (APP_HID_CALL_CALL_STATUS_ACTIVATE == p_info->call_status) {
        /* 05 09, 05 01, (wait 05 09 -> 05 08), 05 00 */
        app_usb_hid_send_call_cmd(APP_HID_CALL_TX_CALL_FLAG_HOOK_SWITCH | APP_HID_CALL_TX_CALL_FLAG_FLASH, APP_HID_CALL_CMD_LEN_HOLD_CALL);
        app_hid_call_start_timer(USB_HID_TIMER_HOLD_DELAY, APP_HID_CALL_CALL_HOLD_DELAY);
    }
}

static void app_hid_call_unhold_call(uint8_t call_idx)
{
    app_hid_call_usb_hid_info *p_info = NULL;

    if (NULL == (p_info = app_hid_call_get_call_info_by_index(call_idx))) {
        APPS_LOG_MSGID_I(TAG"unhold_call ERROR, call not exist!", 0);
        return;
    }

    APPS_LOG_MSGID_I(TAG"unhold_call, call_status:%x", 1, p_info->call_status);

    if (APP_HID_CALL_CALL_STATUS_HELD == p_info->call_status) {
        /* 05 08, 05 00, (wait 05 00 -> 05 01), 05 01 */
        app_usb_hid_send_call_cmd(APP_HID_CALL_TX_CALL_FLAG_FLASH, APP_HID_CALL_CMD_LEN_HOLD_CALL);
        app_hid_call_start_timer(USB_HID_TIMER_UNHOLD_DELAY, APP_HID_CALL_CALL_UNHOLD_DELAY);
    }
}

static void app_hid_call_terminate_call(uint8_t call_idx)
{
    app_hid_call_usb_hid_info *p_info = NULL;

    if (NULL == (p_info = app_hid_call_get_call_info_by_index(call_idx))) {
        APPS_LOG_MSGID_I(TAG"terminate_call ERROR, call not exist!", 0);
        return;
    }

    APPS_LOG_MSGID_I(TAG"terminate_call, call_status:%x", 1, p_info->call_status);

    if (APP_HID_CALL_CALL_STATUS_HELD == p_info->call_status) {
        APPS_LOG_MSGID_I(TAG"call held, unhold firstly", 0);
        /* unhold firstly and then terminate */
        g_usb_hid_call_need_terminate_idx = call_idx;
        app_hid_call_unhold_call(call_idx);
        app_hid_call_start_timer(USB_HID_TIMER_UNHOLD_TERMINATE, APP_HID_CALL_CALL_END_DELAY);
        return;
    } else if (APP_HID_CALL_CALL_STATUS_END == p_info->call_status) {
        //TOOD, check
        p_info->call_status = APP_HID_CALL_CALL_STATUS_IDLE;
        /*
        bt_le_audio_source_call_ended_t le_param;
        le_param.service_index = ble_tbs_get_gtbs_service_idx();
        le_param.call_index = p_info->call_idx;
        p_info->call_idx = 0;
        bt_le_audio_source_send_action(BT_LE_AUDIO_SOURCE_ACTION_CALL_ENDED, &le_param);
        */
        return;
    }

    app_hid_call_status call_status;


    call_status = p_info->call_status;
    g_termiate_bk_state = p_info->call_status;
    p_info->call_status = APP_HID_CALL_CALL_STATUS_ENDING;

    if (APP_HID_CALL_CALL_STATUS_ACTIVATE <= call_status) {
        /* if dongle hook-switch status:
           0: send 05 01 00 -> 500ms -> 05 00 00
           1: send 05 00 00
         */
        if ((g_current_usb_hid_state & APP_HID_CALL_TX_CALL_FLAG_HOOK_SWITCH) && !APP_HID_CALL_CALL_IS_MULTICALL(g_call_wait_event.next_status)) {
            /* Terminate Call case I, II, III(1st time): normal case, use 05 00 00 to terminate call */
            app_usb_hid_send_call_cmd(APP_HID_CALL_TX_CALL_FLAG_NONE, APP_HID_CALL_CMD_LEN_END_CALL);
            app_hid_call_start_timer(USB_HID_TIMER_CHECK_TERMINATE, 500);
        } else {
            /* Terminate Call case III(2nd time): if multidevice, use 05 01 00--500ms-->05 00 00 to terminate call */
            app_usb_hid_send_call_cmd(APP_HID_CALL_TX_CALL_FLAG_HOOK_SWITCH, APP_HID_CALL_CMD_LEN_END_CALL);
            app_hid_call_start_timer(USB_HID_TIMER_TERMINATE_DELAY, 500);
        }
        g_call_wait_event.next_status = APP_HID_CALL_CALL_STATUS_ENDING;
        g_call_wait_event.wait_event =  APP_HID_CALL_TX_CALL_FLAG_NONE;
    } else {
        app_usb_hid_send_call_cmd(APP_HID_CALL_TX_CALL_FLAG_HEADSET_ARRAY, APP_HID_CALL_CMD_LEN_REJECT_CALL);
        app_hid_call_start_timer(USB_HID_TIMER_REJECT_DELAY, APP_HID_CALL_CALL_REJECT_DELAY);
        app_hid_call_start_timer(USB_HID_TIMER_CHECK_TERMINATE, 500);
    }
}

static void app_hid_call_mute_mic(void)
{
    if (g_usb_hid_call_event & APP_HID_CALL_RX_CALL_FLAG_HOLD) {
        APPS_LOG_MSGID_I(TAG"call held, do not change mic mute state", 0);
        return;
    }
    app_usb_hid_send_call_cmd((g_usb_hid_call_event & APP_HID_CALL_RX_CALL_FLAG_OFF_HOOK) | APP_HID_CALL_TX_CALL_FLAG_PHONE_MUTE, 2);
    app_hid_call_start_timer(USB_HID_TIMER_MUTE, APP_HID_CALL_CALL_MUTE_DELAY);
}

static void app_hid_call_toggle_play(void)
{
    USB_Audio_HID_PlayPause();
}

static void app_hid_call_handle_busy_call(bool busy)
{
    uint8_t i;

    for (i = 0; i < APP_HID_CALL_MAX_CALL_NUM; i++) {
        if (busy) {
            switch (g_usb_hid_info[i].call_status) {
                case APP_HID_CALL_CALL_STATUS_INCOMING:
                    /* auto reject */
                    APPS_LOG_MSGID_I(TAG"auto reject", 0);
                    app_hid_call_terminate_call(0);
                    break;
                case APP_HID_CALL_CALL_STATUS_OUTGOING:
                case APP_HID_CALL_CALL_STATUS_ACTIVATE:
                    APPS_LOG_MSGID_I(TAG"auto hold", 0);
                    /* auto hold */
                    app_hid_call_hold_call(0);
                    break;
                default:
                    break;
            }
        } else {
            switch (g_usb_hid_info[i].call_status) {
                case APP_HID_CALL_CALL_STATUS_HELD:
                    APPS_LOG_MSGID_I(TAG"auto unhold", 0);
                    /* auto hold */
                    app_hid_call_unhold_call(0);
                    break;
                default:
                    break;
            }
        }
    }
}

bool app_hid_call_existing(void)
{
    if (NULL == app_hid_call_get_call_info_by_state(APP_HID_CALL_CALL_STATUS_IDLE)) {
        return true;
    }
    return false;
}

static void app_hid_call_init(void)
{
    int32_t ret = 0;

    memset(&g_usb_hid_info[0], 0, sizeof(app_hid_call_usb_hid_info) * APP_HID_CALL_MAX_CALL_NUM);

    ret = usb_hid_handler_rx_register(APP_HID_CALL_CALL_STATE_REPORT_ID, APP_HID_CALL_CALL_STATE_REPORT_LEN, (usb_hid_handler_callback *)app_hid_call_rx_callback);
    APPS_LOG_MSGID_I(TAG"init, usb_hid_handler_rx_register:%x", 1, ret);
}

static bool _proc_ui_shell_group(struct _ui_shell_activity *self, uint32_t event_id, void *extra_data, size_t data_len)
{
    /* UI shell internal event must process by this activity, so default is true */
    bool ret = true;
    switch (event_id) {
        case EVENT_ID_SHELL_SYSTEM_ON_CREATE: {
            APPS_LOG_MSGID_I(TAG"create", 0);
            app_hid_call_init();
            break;
        }
        default:
            break;
    }
    return ret;
}

static bool app_hid_call_handle_hid_event(ui_shell_activity_t *self, uint32_t event_id, void *extra_data, size_t data_len)
{
    switch (event_id) {
        case APP_HID_CALL_EVENT_USB_HID_INCOMING_CALL: {
            APPS_LOG_MSGID_I(TAG"hid_call_evt: INCOMING_CALL", 0);
            app_hid_call_handle_incoming_call();
            /* If other call is activate, reject new calls. */
            //app_hid_call_handle_busy_call(true);
            break;
        }
        case APP_HID_CALL_EVENT_USB_HID_CALL_ACTIVATE: {
            APPS_LOG_MSGID_I(TAG"hid_call_evt: CALL_ACTIVATE", 0);
            app_hid_call_handle_call_active();
            break;
        }
        case APP_HID_CALL_EVENT_USB_HID_CALL_REMOTE_HOLD: {
            APPS_LOG_MSGID_I(TAG"hid_call_evt: CALL_REMOTE_HOLD", 0);
            //app_hid_call_handle_call_remotely_hold();
            break;
        }
        case APP_HID_CALL_EVENT_USB_HID_CALL_HOLD: {
            APPS_LOG_MSGID_I(TAG"hid_call_evt: CALL_HOLD", 0);
            app_hid_call_handle_call_hold();
            break;
        }
        case APP_HID_CALL_EVENT_USB_HID_CALL_UNHOLD: {
            APPS_LOG_MSGID_I(TAG"hid_call_evt: CALL_UNHOLD", 0);
            app_hid_call_handle_call_unhold();
            break;
        }
        case APP_HID_CALL_EVENT_USB_HID_CALL_ENDED: {
            APPS_LOG_MSGID_I(TAG"hid_call_evt: CALL_ENDED", 0);
            app_hid_call_handle_call_end();
            break;
        }
        case APP_HID_CALL_EVENT_USB_HID_MIC_MUTE: {
            APPS_LOG_MSGID_I(TAG"hid_call_evt: MIC_MUTE", 0);
            //TODO, mute usb audio
            break;
        }
        case APP_HID_CALL_EVENT_USB_HID_MIC_UNMUTE: {
            APPS_LOG_MSGID_I(TAG"hid_call_evt: MIC_UNMUTE", 0);
            //TODO, unmute usb audio
            break;
        }
        case APP_HID_CALL_EVENT_SELF_CTRL: {
            APPS_LOG_MSGID_I(TAG"hid_call_evt: self command, len = %d", 1, data_len);
            if (data_len == 0) {
                break;
            }
            USB_Audio_HID_t ret;
            uint8_t buf[5] = {0};

            memcpy(buf, extra_data, data_len);
            ret = USB_Audio_HID_TX(buf, data_len);

            APPS_LOG_MSGID_I(TAG"send_call_cmd, length:%x ret:%x", 2, data_len, ret);
            break;
        }
    }
    return false;
}

static bool _proc_key_event(struct _ui_shell_activity *self, uint32_t event_id, void *extra_data, size_t data_len)
{
    bool ret = false;
    apps_config_state_t mmi_sta = apps_config_key_get_mmi_state();
    if (mmi_sta >= APP_HFP_INCOMING && mmi_sta <= APP_HFP_MULTITPART_CALL) {
        ;/* The key event will remap by app_hid_call_coexist_hfp in hfp activity. */
    } else {
        app_hid_call_coexist_hfp(event_id);
    }

    if (s_key_action_preparse_result != KEY_ACTION_INVALID) {
        APPS_LOG_MSGID_I(TAG"_proc_key_event, preparse_result=%d", 1, s_key_action_preparse_result);
    }

    switch (s_key_action_preparse_result) {
        case KEY_CANCEL_OUT_GOING_CALL:
        case KEY_END_CALL:
        case KEY_REJCALL:
            app_hid_call_terminate_call(0);
            ret = true;
            break;
        case KEY_ACCEPT_CALL:
            app_hid_call_accept_call(0);
            ret = true;
            break;
        case KEY_ONHOLD_CALL:
            if (g_usb_hid_info[0].call_status != APP_HID_CALL_CALL_STATUS_HELD) {
                app_hid_call_hold_call(0);
            } else {
                app_hid_call_unhold_call(0);
            }
            ret = true;
            break;
    }
    s_key_action_preparse_result = KEY_ACTION_INVALID;

    apps_config_key_action_t action = *(uint16_t *)extra_data;
    switch (action) {
        case KEY_MUTE_MIC:
            app_hid_call_mute_mic();
            break;
    }

    return ret;
}

bool app_hid_call_idle_activity_proc(struct _ui_shell_activity *self,
                                     uint32_t event_group,
                                     uint32_t event_id,
                                     void *extra_data,
                                     size_t data_len)
{

    bool ret = false;

    switch (event_group) {
        case EVENT_GROUP_UI_SHELL_SYSTEM:
            /* UI Shell internal events, please refer to doc/Airoha_IoT_SDK_UI_Framework_Developers_Guide.pdf. */
            ret = _proc_ui_shell_group(self, event_id, extra_data, data_len);
            break;
        case EVENT_GROUP_UI_SHELL_USB_HID_CALL:
            ret = app_hid_call_handle_hid_event(self, event_id, extra_data, data_len);
            break;
        case EVENT_GROUP_UI_SHELL_KEY:
            /* key event. */
            ret = _proc_key_event(self, event_id, extra_data, data_len);
            break;
        case EVENT_GROUP_UI_SHELL_APP_INTERACTION: {
            if (event_id == APPS_EVENTS_INTERACTION_USB_PLUG_STATE) {
                bool plug_in = (bool)extra_data;
                APPS_LOG_MSGID_I(TAG"APPS_EVENTS_INTERACTION_USB_PLUG_STATE received, usb state=%d.", 1, plug_in);
                if (!plug_in) {
                    g_usb_hid_call_event = 0;
                    g_usb_hid_info[0].call_status = 0;
                    g_usb_hid_call_need_terminate_idx = 0;
                    g_current_usb_hid_state = APP_HID_CALL_TX_CALL_FLAG_NONE;
                    s_key_action_preparse_result = KEY_ACTION_INVALID;
                }
            }
            break;
        }
    }

    return ret;
}

apps_config_key_action_t app_hid_call_coexist_hfp(uint32_t event_id)
{
    apps_config_key_action_t hfp_action = KEY_ACTION_INVALID;

    apps_config_state_t mmi_sta = apps_config_key_get_mmi_state();
    apps_config_state_t remap_mmi_sta = apps_config_key_get_mmi_state();
    app_hid_call_status hid_call_sta = g_usb_hid_info[0].call_status;

    /* remap mmi state */
    if (mmi_sta >= APP_HFP_INCOMING && mmi_sta <= APP_HFP_MULTITPART_CALL) {
        switch (mmi_sta) {
            case APP_HFP_INCOMING:
            case APP_HFP_TWC_INCOMING:
                if (hid_call_sta != APP_HID_CALL_CALL_STATUS_IDLE) {
                    remap_mmi_sta = APP_HFP_TWC_INCOMING;
                }
                break;
            case APP_HFP_OUTGOING:
            case APP_HFP_TWC_OUTGOING:
                if (hid_call_sta != APP_HID_CALL_CALL_STATUS_IDLE) {
                    remap_mmi_sta = APP_HFP_TWC_OUTGOING;
                }
                break;
            case APP_STATE_HELD_ACTIVE:
            case APP_HFP_CALLACTIVE:
            case APP_HFP_CALLACTIVE_WITHOUT_SCO:
            case APP_HFP_MULTITPART_CALL:
                switch (hid_call_sta) {
                    case APP_HID_CALL_CALL_STATUS_IDLE:
                        break;
                    case APP_HID_CALL_CALL_STATUS_INCOMING:
                        remap_mmi_sta = APP_HFP_TWC_INCOMING;
                        break;
                    case APP_HID_CALL_CALL_STATUS_OUTGOING:
                        remap_mmi_sta = APP_HFP_TWC_OUTGOING;
                        break;
                    default:
                        remap_mmi_sta = APP_HFP_MULTITPART_CALL;
                        break;
                }
                break;
        }
    } else {
        switch (hid_call_sta) {
            case APP_HID_CALL_CALL_STATUS_IDLE:
                break;
            case APP_HID_CALL_CALL_STATUS_INCOMING:
                remap_mmi_sta = APP_HFP_INCOMING;
                break;
            case APP_HID_CALL_CALL_STATUS_OUTGOING:
                remap_mmi_sta = APP_HFP_TWC_OUTGOING;
                break;
            default:
                remap_mmi_sta = APP_HFP_CALLACTIVE;
                break;
        }
    }

    /* remap key action by remaped mmi state. */
    uint8_t key_id;
    airo_key_event_t key_event;
    app_event_key_event_decode(&key_id, &key_event, event_id);
    apps_config_key_action_t remap_action = apps_config_key_event_remapper_map_action_in_temp_state(key_id, key_event, remap_mmi_sta);
    APPS_LOG_MSGID_I(TAG"app_hid_call_coexist_hfp mmi_sta=%d, hid_call_sta=%d, remap_mmi=%d, remap_key_action=%d", 4,
                     mmi_sta, hid_call_sta, remap_mmi_sta, remap_action);

    /* Decide hfp call and hid call action. */
    switch (remap_action) {
        /* Just one call exist, find which one. */
        case KEY_REDIAL_LAST_CALL:
        case KEY_CANCEL_OUT_GOING_CALL:
        case KEY_REJCALL:
        case KEY_ACCEPT_CALL:
        /* Common case. */
        case KEY_VOICE_UP:
        case KEY_VOICE_DN:
        case KEY_MUTE_MIC:
        /* If just one call exist, find which one. If at least two call exist, hfp call is higher prority. */
        case KEY_END_CALL:
        case KEY_ONHOLD_CALL:
            if (mmi_sta >= APP_HFP_INCOMING && mmi_sta <= APP_HFP_MULTITPART_CALL) {
                hfp_action = remap_action;
            } else if (hid_call_sta != APP_HID_CALL_CALL_STATUS_IDLE) {
                s_key_action_preparse_result = remap_action;
            } else {
                APPS_LOG_MSGID_E(TAG"app_hid_call_coexist_hfp, unexpected status.", 0);
            }
            break;
        /* If just one call exist, find which one. If at least two call exist, hid call is higher prority. */
        case KEY_3WAY_HOLD_ACTIVE_ACCEPT_OTHER:
            if (hid_call_sta == APP_HID_CALL_CALL_STATUS_INCOMING) {
                s_key_action_preparse_result = KEY_ACCEPT_CALL;
                if (mmi_sta == APP_HFP_INCOMING) {
                    hfp_action = KEY_REJCALL;
                } else if (mmi_sta > APP_HFP_INCOMING) {
                    hfp_action = KEY_END_CALL;
                }
            } else if (mmi_sta == APP_HFP_INCOMING) {
                hfp_action = KEY_ACCEPT_CALL;
                if (hid_call_sta == APP_HID_CALL_CALL_STATUS_INCOMING) {
                    s_key_action_preparse_result = KEY_REJCALL;
                } else if (hid_call_sta > APP_HID_CALL_CALL_STATUS_INCOMING) {
                    s_key_action_preparse_result = KEY_END_CALL;
                }
            } else {
                APPS_LOG_MSGID_E(TAG"app_hid_call_coexist_hfp, unexpected status in multipart call case.", 0);
            }
            break;
        /* hid call is higher prority. */
        case KEY_REJCALL_SECOND_PHONE:
            if (mmi_sta == APP_HFP_INCOMING) {
                hfp_action = KEY_REJCALL;
            } else if (hid_call_sta == APP_HID_CALL_CALL_STATUS_INCOMING) {
                s_key_action_preparse_result = KEY_REJCALL;
            } else {
                APPS_LOG_MSGID_E(TAG"app_hid_call_coexist_hfp, unexpected status in twc incoming call case.", 0);
            }
            break;
    }

    APPS_LOG_MSGID_I(TAG"app_hid_call_coexist_hfp, return hfp_action=%d, pre parse key action=%d.", 2,
                     hfp_action, s_key_action_preparse_result);
    return hfp_action;
}
