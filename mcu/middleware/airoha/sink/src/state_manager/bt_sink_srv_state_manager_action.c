/* Copyright Statement:
 *
 * (C) 2022  Airoha Technology Corp. All rights reserved.
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

#include "bt_sink_srv_state_manager.h"
#include "bt_sink_srv_state_manager_internal.h"

#define BT_SINK_SRV_STATE_MANAGER_CALL_ACTION_MASK \
    (SINK_MODULE_MASK_HFP | SINK_MODULE_MASK_HSP)

#define BT_SINK_SRV_STATE_MANAGER_MEDIA_ACTION_MASK \
    (SINK_MODULE_MASK_A2DP | SINK_MODULE_MASK_AVRCP)

#define BT_SINK_SRV_STATE_MANAGER_DISPATCH_TABLE_SIZE \
    (sizeof(g_bt_sink_srv_state_manager_dispatch_table) / \
     sizeof(bt_sink_srv_state_manager_dispatch_table_t))

#define BT_SINK_SRV_STATE_MANAGER_EDR_ACTION_TABLE_SIZE \
    (sizeof(bt_sink_srv_action_callback_table) / \
     sizeof(bt_sink_srv_action_callback_table_t))

#define BT_SINK_SRV_STATE_MANAGER_IS_CALL_ACTION(mask) \
    ((mask) & BT_SINK_SRV_STATE_MANAGER_CALL_ACTION_MASK)

#define BT_SINK_SRV_STATE_MANAGER_IS_MEDIA_ACTION(mask) \
    ((mask) & BT_SINK_SRV_STATE_MANAGER_MEDIA_ACTION_MASK)

extern const bt_sink_srv_action_callback_table_t bt_sink_srv_action_callback_table[BT_SINK_SRV_MAX_ACTION_TABLE_SIZE];

static const bt_sink_srv_state_manager_dispatch_table_t g_bt_sink_srv_state_manager_dispatch_table[] = {
    /* INCOMING + OUTGOING. */
    {
        BT_SINK_SRV_STATE_INCOMING,
        BT_SINK_SRV_STATE_OUTGOING,
        BT_SINK_SRV_ACTION_3WAY_HOLD_ACTIVE_ACCEPT_OTHER,
        BT_SINK_SRV_ACTION_ANSWER,
        0,
        false
    },
    {
        BT_SINK_SRV_STATE_INCOMING,
        BT_SINK_SRV_STATE_OUTGOING,
        BT_SINK_SRV_ACTION_3WAY_RELEASE_ALL_HELD,
        BT_SINK_SRV_ACTION_REJECT,
        0,
        true
    },
    {
        BT_SINK_SRV_STATE_INCOMING,
        BT_SINK_SRV_STATE_OUTGOING,
        BT_SINK_SRV_ACTION_HANG_UP,
        0,
        BT_SINK_SRV_ACTION_HANG_UP,
        false
    },
    /* INCOMING + HELD_REMAINING. */
    {
        BT_SINK_SRV_STATE_INCOMING,
        BT_SINK_SRV_STATE_HELD_REMAINING,
        BT_SINK_SRV_ACTION_3WAY_HOLD_ACTIVE_ACCEPT_OTHER,
        BT_SINK_SRV_ACTION_ANSWER,
        0,
        false
    },
    {
        BT_SINK_SRV_STATE_INCOMING,
        BT_SINK_SRV_STATE_HELD_REMAINING,
        BT_SINK_SRV_ACTION_3WAY_RELEASE_ALL_HELD,
        BT_SINK_SRV_ACTION_REJECT,
        0,
        true
    },
    /* OUTGOING + INCOMING. */
    {
        BT_SINK_SRV_STATE_OUTGOING,
        BT_SINK_SRV_STATE_INCOMING,
        BT_SINK_SRV_ACTION_3WAY_HOLD_ACTIVE_ACCEPT_OTHER,
        BT_SINK_SRV_ACTION_HANG_UP,
        BT_SINK_SRV_ACTION_ANSWER,
        true
    },
    {
        BT_SINK_SRV_STATE_OUTGOING,
        BT_SINK_SRV_STATE_INCOMING,
        BT_SINK_SRV_ACTION_ANSWER,
        0,
        BT_SINK_SRV_ACTION_ANSWER,
        true
    },
    {
        BT_SINK_SRV_STATE_OUTGOING,
        BT_SINK_SRV_STATE_INCOMING,
        BT_SINK_SRV_ACTION_3WAY_RELEASE_ALL_HELD,
        0,
        BT_SINK_SRV_ACTION_REJECT,
        false
    },
    {
        BT_SINK_SRV_STATE_OUTGOING,
        BT_SINK_SRV_STATE_INCOMING,
        BT_SINK_SRV_ACTION_REJECT,
        0,
        BT_SINK_SRV_ACTION_REJECT,
        false
    },
    /* OUTGOING + HELD_REMAINING. */
    {
        BT_SINK_SRV_STATE_OUTGOING,
        BT_SINK_SRV_STATE_HELD_REMAINING,
        BT_SINK_SRV_ACTION_3WAY_RELEASE_ALL_HELD,
        0,
        BT_SINK_SRV_ACTION_3WAY_RELEASE_ALL_HELD,
        false
    },
    /* ACTIVE + INCOMING. */
    {
        BT_SINK_SRV_STATE_ACTIVE,
        BT_SINK_SRV_STATE_INCOMING,
        BT_SINK_SRV_ACTION_3WAY_HOLD_ACTIVE_ACCEPT_OTHER,
        BT_SINK_SRV_ACTION_3WAY_HOLD_ACTIVE_ACCEPT_OTHER,
        BT_SINK_SRV_ACTION_ANSWER,
        true
    },
    {
        BT_SINK_SRV_STATE_ACTIVE,
        BT_SINK_SRV_STATE_INCOMING,
        BT_SINK_SRV_ACTION_3WAY_RELEASE_ALL_HELD,
        0,
        BT_SINK_SRV_ACTION_REJECT,
        false
    },
    {
        BT_SINK_SRV_STATE_ACTIVE,
        BT_SINK_SRV_STATE_INCOMING,
        BT_SINK_SRV_ACTION_ANSWER,
        0,
        BT_SINK_SRV_ACTION_ANSWER,
        true
    },
    {
        BT_SINK_SRV_STATE_ACTIVE,
        BT_SINK_SRV_STATE_INCOMING,
        BT_SINK_SRV_ACTION_REJECT,
        0,
        BT_SINK_SRV_ACTION_REJECT,
        false
    },
    /* ACTIVE + HELD_REMAINING. */
    {
        BT_SINK_SRV_STATE_ACTIVE,
        BT_SINK_SRV_STATE_HELD_REMAINING,
        BT_SINK_SRV_ACTION_3WAY_HOLD_ACTIVE_ACCEPT_OTHER,
        BT_SINK_SRV_ACTION_3WAY_HOLD_ACTIVE_ACCEPT_OTHER,
        BT_SINK_SRV_ACTION_3WAY_HOLD_ACTIVE_ACCEPT_OTHER,
        true
    },
    {
        BT_SINK_SRV_STATE_ACTIVE,
        BT_SINK_SRV_STATE_HELD_REMAINING,
        BT_SINK_SRV_ACTION_3WAY_RELEASE_ALL_HELD,
        0,
        BT_SINK_SRV_ACTION_3WAY_RELEASE_ALL_HELD,
        false
    },
    /* HELD_REMAINING + INCOMING. */
    {
        BT_SINK_SRV_STATE_HELD_REMAINING,
        BT_SINK_SRV_STATE_INCOMING,
        BT_SINK_SRV_ACTION_3WAY_HOLD_ACTIVE_ACCEPT_OTHER,
        0,
        BT_SINK_SRV_ACTION_ANSWER,
        true
    },
    {
        BT_SINK_SRV_STATE_HELD_REMAINING,
        BT_SINK_SRV_STATE_INCOMING,
        BT_SINK_SRV_ACTION_3WAY_RELEASE_ALL_HELD,
        0,
        BT_SINK_SRV_ACTION_REJECT,
        false
    },
    /* HELD_REMAINING + OUTGOING. */
    {
        BT_SINK_SRV_STATE_HELD_REMAINING,
        BT_SINK_SRV_STATE_OUTGOING,
        BT_SINK_SRV_ACTION_HANG_UP,
        0,
        BT_SINK_SRV_ACTION_HANG_UP,
        false
    }
};

static bt_status_t bt_sink_srv_state_manager_action_handler_internal(
        bt_sink_srv_state_manager_context_t *context,
        bt_sink_srv_action_t action,
        void *parameter);
static bt_status_t bt_sink_srv_state_manager_call_action_handler(
        bt_sink_srv_state_manager_context_t *context,
        bt_sink_srv_action_t action,
        void *parameter);
static bt_status_t bt_sink_srv_state_manager_media_action_handler(
        bt_sink_srv_state_manager_context_t *context,
        bt_sink_srv_action_t action,
        void *parameter);
static bt_status_t bt_sink_srv_state_manager_le_action_handler(
        bt_sink_srv_state_manager_context_t *context,
        bt_sink_srv_action_t action,
        void *parameter);
static void bt_sink_srv_state_manager_sync_focus(
        bt_sink_srv_state_manager_context_t *context,
        bt_sink_srv_state_manager_focus_device_t type,
        bt_sink_srv_state_manager_device_t *device);

bt_status_t bt_sink_srv_state_manager_action_handler(bt_sink_srv_action_t action, void *parameter)
{
    bt_status_t status = BT_STATUS_FAIL;
    bt_sink_module_mask_t mask = SINK_MODULE_MASK_OFFSET(action);
    bt_sink_srv_state_manager_context_t *context = bt_sink_srv_state_manager_get_context();

    if (BT_SINK_SRV_STATE_MANAGER_IS_CALL_ACTION(mask)) {
        status = bt_sink_srv_state_manager_call_action_handler(context, action, parameter);
    } else if (BT_SINK_SRV_STATE_MANAGER_IS_MEDIA_ACTION(mask)) {
        status = bt_sink_srv_state_manager_media_action_handler(context, action, parameter);
    } else {
        status = bt_sink_srv_state_manager_action_handler_internal(context, action, parameter);
    }

    return status;
}

static bt_status_t bt_sink_srv_state_manager_action_handler_internal(
        bt_sink_srv_state_manager_context_t *context,
        bt_sink_srv_action_t action,
        void *parameter)
{
    bt_status_t status = bt_sink_srv_state_manager_le_action_handler(context, action, parameter);

    if (BT_STATUS_SUCCESS != status) {
        status = bt_sink_srv_state_manager_edr_action_handler(context, action, parameter);
    }

    return status;
}

static bt_status_t bt_sink_srv_state_manager_call_action_handler(
        bt_sink_srv_state_manager_context_t *context,
        bt_sink_srv_action_t action,
        void *parameter)
{
    bool swap_focus = false;
    bt_status_t status = BT_STATUS_FAIL;

    bt_sink_srv_state_manager_device_t *other_device = NULL;
    bt_sink_srv_state_t focus_state = BT_SINK_SRV_STATE_NONE;
    bt_sink_srv_state_t other_state = BT_SINK_SRV_STATE_NONE;

    /* 1. If no device is playing, trigger LE device at first. */
    if (NULL == context->focus_call_device) {
        return bt_sink_srv_state_manager_action_handler_internal(context, action, parameter);
    }

    /* 2. Get focus state & other state. */
    focus_state = context->focus_call_device->call_state;

    other_device = bt_sink_srv_state_manager_get_device_by_call_state(
            context,
            BT_SINK_SRV_STATE_MANAGER_CALL_STATE,
            context->focus_call_device);

    if (NULL != other_device) {
        other_state = other_device->call_state;
    }

    /* 3. Handle action by dispatch table. */
    for (uint32_t i = 0; i < BT_SINK_SRV_STATE_MANAGER_DISPATCH_TABLE_SIZE; i++) {
        if (g_bt_sink_srv_state_manager_dispatch_table[i].action == action &&
            g_bt_sink_srv_state_manager_dispatch_table[i].focus_state == focus_state &&
            g_bt_sink_srv_state_manager_dispatch_table[i].other_state == other_state) {
            /* 3.1. Check if need to swap focus device. */
            bt_sink_srv_report_id(
                    "[Sink][StaMgr]call action handler, focus_action:0x%x other_action:0x%x",
                    2,
                    g_bt_sink_srv_state_manager_dispatch_table[i].focus_action,
                    g_bt_sink_srv_state_manager_dispatch_table[i].other_action);

            if (g_bt_sink_srv_state_manager_dispatch_table[i].swap_focus) {
                swap_focus = true;

                if (NULL != other_device && BT_SINK_SRV_STATE_MANAGER_DEVICE_TYPE_LE == other_device->type) {
                    bt_sink_srv_state_manager_sync_focus(
                            context,
                            BT_SINK_SRV_STATE_MANAGER_FOCUS_DEVICE_CALL,
                            other_device);
                }
            }

            /* 3.2. If focus device & other device type are same, send action to focus device. */
            if (context->focus_call_device->type == other_device->type) {
                break;
            }

            /* 3.3. Send focus action & other action. */
            if (BT_SINK_SRV_STATE_MANAGER_DEVICE_TYPE_EDR == context->focus_call_device->type) {
                status = bt_sink_srv_state_manager_edr_action_handler(
                        context,
                        g_bt_sink_srv_state_manager_dispatch_table[i].focus_action,
                        parameter);
            } else {
                status = bt_sink_srv_state_manager_le_action_handler(
                        context,
                        g_bt_sink_srv_state_manager_dispatch_table[i].focus_action,
                        parameter);
            }

            if (BT_SINK_SRV_STATE_MANAGER_DEVICE_TYPE_EDR == other_device->type) {
                status = bt_sink_srv_state_manager_edr_action_handler(
                        context,
                        g_bt_sink_srv_state_manager_dispatch_table[i].other_action,
                        parameter);
            } else {
                status = bt_sink_srv_state_manager_le_action_handler(
                        context,
                        g_bt_sink_srv_state_manager_dispatch_table[i].other_action,
                        parameter);
            }

            if (swap_focus) {
                bt_sink_srv_state_manager_set_focus_call_device(context, other_device, true);
            }

            return status;
        }
    }

    if (BT_SINK_SRV_STATE_MANAGER_DEVICE_TYPE_EDR == context->focus_call_device->type) {
        status = bt_sink_srv_state_manager_edr_action_handler(context, action, parameter);
    } else {
        status = bt_sink_srv_state_manager_le_action_handler(context, action, parameter);
    }

    if (swap_focus) {
        bt_sink_srv_state_manager_set_focus_call_device(context, other_device, true);
    }

    return status;
}

static bt_status_t bt_sink_srv_state_manager_media_action_handler(
        bt_sink_srv_state_manager_context_t *context,
        bt_sink_srv_action_t action,
        void *parameter)
{
    bt_status_t status = BT_STATUS_FAIL;

    /* 1. If no device is playing, trigger LE device at first. */
    if (NULL == context->focus_media_device) {
        return bt_sink_srv_state_manager_action_handler_internal(context, action, parameter);
    }

    /* 2. Trigger the playing device. */
    if (BT_SINK_SRV_STATE_MANAGER_DEVICE_TYPE_EDR == context->focus_media_device->type) {
        status = bt_sink_srv_state_manager_edr_action_handler(context, action, parameter);
    } else {
        status = bt_sink_srv_state_manager_le_action_handler(context, action, parameter);
    }

    return status;
}

bt_status_t bt_sink_srv_state_manager_edr_action_handler(
        bt_sink_srv_state_manager_context_t *context,
        bt_sink_srv_action_t action,
        void *parameter)
{
    bt_status_t status = BT_STATUS_FAIL;
    bt_sink_module_mask_t mask = SINK_MODULE_MASK_OFFSET(action);
    bt_sink_srv_report_id("[Sink][StaMgr]edr action handler, action: 0x%x", 1, action);

    if (0 == action) {
        return BT_STATUS_SUCCESS;
    }

    for (uint32_t i = 0; i < BT_SINK_SRV_STATE_MANAGER_EDR_ACTION_TABLE_SIZE; i++) {
        if ((bt_sink_srv_action_callback_table[i].module & mask) &&
            (NULL != bt_sink_srv_action_callback_table[i].callback)) {
            status = bt_sink_srv_action_callback_table[i].callback(action, parameter);
        }
    }

    return status;
}

static bt_status_t bt_sink_srv_state_manager_le_action_handler(
        bt_sink_srv_state_manager_context_t *context,
        bt_sink_srv_action_t action,
        void *parameter)
{
    bt_status_t status = BT_STATUS_FAIL;
    bt_sink_srv_report_id("[Sink][StaMgr]le action handler, action: 0x%x", 1, action);

    if (0 == action) {
        return BT_STATUS_SUCCESS;
    }

#ifdef AIR_LE_AUDIO_ENABLE
    status = le_sink_srv_send_action(action, parameter);
#endif
    return status;
}

static void bt_sink_srv_state_manager_sync_focus(
        bt_sink_srv_state_manager_context_t *context,
        bt_sink_srv_state_manager_focus_device_t type,
        bt_sink_srv_state_manager_device_t *device)
{
#ifdef MTK_AWS_MCE_ENABLE
    bt_status_t status = BT_STATUS_FAIL;
    uint8_t buffer[sizeof(bt_sink_srv_state_manager_sync_focus_t)] = {0};
    bt_sink_srv_state_manager_sync_focus_t *sync_focus = (bt_sink_srv_state_manager_sync_focus_t *)&buffer;

    bt_aws_mce_report_info_t report_info = {0};
    bt_aws_mce_role_t role = bt_connection_manager_device_local_info_get_aws_role();

    /* 1. Check parameters. */
    if (NULL == device || BT_SINK_SRV_STATE_MANAGER_DEVICE_TYPE_EDR == device->type) {
        bt_sink_srv_report_id("[Sink][StaMgr]sync focus, NULL device or EDR type", 0);
        return;
    }

    /* 2. Fill data. */
    sync_focus->header.type = BT_SINK_SRV_STATE_MANAGER_SYNC_TYPE_FOCUS;
    sync_focus->header.length = sizeof(buffer);

    if (BT_AWS_MCE_ROLE_AGENT == role) {
        sync_focus->header.direction = BT_SINK_SRV_STATE_MANAGER_SYNC_DIRECTION_PARTNER;
    } else {
        sync_focus->header.direction = BT_SINK_SRV_STATE_MANAGER_SYNC_DIRECTION_AGENT;
    }

    sync_focus->focus_type = type;
    sync_focus->device_type = device->type;
    bt_sink_srv_memcpy(&sync_focus->address, &device->address, sizeof(bt_bd_addr_t));

    /* 3. Fill report info. */
    report_info.module_id = BT_AWS_MCE_REPORT_MODULE_SINK_STAMGR;
    report_info.param = (void *)&buffer;
    report_info.param_len = sizeof(buffer);

    status = bt_aws_mce_report_send_urgent_event(&report_info);
    bt_sink_srv_report_id("[Sink][StaMgr]sync focus, status: 0x%x", 1, status);
#endif
}

