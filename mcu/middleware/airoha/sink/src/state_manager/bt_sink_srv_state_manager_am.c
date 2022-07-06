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

#ifdef AIR_BT_SINK_SRV_STATE_MANAGER_DUMMY_DEIVCE_ENABLE
 #define BT_SINK_SRV_STATE_MANAGER_IS_SINK_SRV_DEVICE(type) \
     ((type) == AUDIO_SRC_SRV_PSEUDO_DEVICE_A2DP ||      \
      (type) == AUDIO_SRC_SRV_PSEUDO_DEVICE_AWS_A2DP ||  \
      (type) == AUDIO_SRC_SRV_PSEUDO_DEVICE_HFP ||       \
      (type) == AUDIO_SRC_SRV_PSEUDO_DEVICE_BLE ||       \
      (type) == AUDIO_SRC_SRV_PSEUDO_DEVICE_DUMMY)
#else
#define BT_SINK_SRV_STATE_MANAGER_IS_SINK_SRV_DEVICE(type) \
    ((type) == AUDIO_SRC_SRV_PSEUDO_DEVICE_A2DP ||      \
     (type) == AUDIO_SRC_SRV_PSEUDO_DEVICE_AWS_A2DP ||  \
     (type) == AUDIO_SRC_SRV_PSEUDO_DEVICE_HFP ||       \
     (type) == AUDIO_SRC_SRV_PSEUDO_DEVICE_BLE)
#endif

#define BT_SINK_SRV_STATE_MANAGER_IS_LE_CALL_DEVICE(device) \
    ((NULL != (device) && BT_SINK_SRV_STATE_MANAGER_DEVICE_TYPE_LE == (device)->type) && \
     (0 != ((BT_SINK_SRV_STATE_INCOMING | BT_SINK_SRV_STATE_OUTGOING | BT_SINK_SRV_STATE_ACTIVE) & (device)->call_state)))

#define BT_SINK_SRV_STATE_MANAGER_IS_CALL_DEVICE(device, psedev) \
    ((AUDIO_SRC_SRV_PSEUDO_DEVICE_HFP == (psedev)->type) ||     \
     (NULL != (device) && (psedev) == (device)->call_device) || \
     (BT_SINK_SRV_STATE_MANAGER_IS_LE_CALL_DEVICE(device)))

#define BT_SINK_SRV_STATE_MANAGER_IS_MEDIA_DEVICE(device, psedev) \
    ((AUDIO_SRC_SRV_PSEUDO_DEVICE_A2DP == (psedev)->type) ||        \
     (AUDIO_SRC_SRV_PSEUDO_DEVICE_AWS_A2DP == (psedev)->type) ||    \
     (NULL != (device) && (psedev) == (device)->media_device))

#ifdef AIR_BT_SINK_SRV_STATE_MANAGER_DUMMY_DEIVCE_ENABLE
#define BT_SINK_SRV_STATE_MANAGER_IS_DUMMY_DEVICE(device, psedev) \
    ((AUDIO_SRC_SRV_PSEUDO_DEVICE_DUMMY == (psedev)->type) || \
     (NULL != (device) && (psedev) == (device)->dummy_device))
#endif

extern bt_sink_srv_allow_result_t bt_customer_config_allow_play(
        bt_sink_srv_device_state_t *current,
        bt_sink_srv_device_state_t *coming);
static bt_sink_srv_allow_result_t default_bt_customer_config_allow_play(
        bt_sink_srv_device_state_t *current,
        bt_sink_srv_device_state_t *coming);

#if _MSC_VER >= 1500
#pragma comment(linker, "/alternatename:_bt_customer_config_allow_play=_default_bt_customer_config_allow_play")
#elif defined(__GNUC__) || defined(__ICCARM__) || defined(__ARMCC_VERSION) || defined(__CC_ARM)
#pragma weak bt_customer_config_allow_play = default_bt_customer_config_allow_play
#else
#error "Unsupported Platform"
#endif

bt_sink_srv_allow_result_t bt_sink_srv_state_manager_psedev_compare(
        audio_src_srv_handle_t *current,
        audio_src_srv_handle_t *coming)
{
    bt_sink_srv_allow_result_t result = BT_SINK_SRV_ALLOW_RESULT_BYPASS;
    bt_sink_srv_state_manager_context_t *context = bt_sink_srv_state_manager_get_context();

    bt_sink_srv_state_manager_device_t *current_device = NULL;
    bt_sink_srv_state_manager_device_t *coming_device = NULL;
    bt_sink_srv_device_state_t current_device_state = BT_SINK_SRV_STATE_MANAGER_INVALID_DEVICE_STATE;
    bt_sink_srv_device_state_t coming_device_state = BT_SINK_SRV_STATE_MANAGER_INVALID_DEVICE_STATE;

    /* 1. Check paramters. */
    if (current == NULL || !BT_SINK_SRV_STATE_MANAGER_IS_SINK_SRV_DEVICE(current->type)) {
        return result;
    }

    if (coming == NULL || !BT_SINK_SRV_STATE_MANAGER_IS_SINK_SRV_DEVICE(current->type)) {
        return result;
    }

    /* 2. Get device state. */
    current_device = bt_sink_srv_state_manager_get_device_by_psedev(context, current);
    coming_device = bt_sink_srv_state_manager_get_device_by_psedev(context, coming);

    bt_sink_srv_report_id("[Sink][StaMgr]psedev compare, current: 0x%x coming: 0x%x",
                          2, current, coming);

    bt_sink_srv_report_id("[Sink][StaMgr]psedev compare, current_device: 0x%x coming_device: 0x%x",
                          2, current_device, coming_device);

    if (NULL != current_device) {
        current_device_state.call_state = current_device->call_state;
        current_device_state.sco_state = current_device->call_audio_state;
        current_device_state.music_state = current_device->media_state;
        bt_sink_srv_memcpy(current_device_state.address, current_device->address, sizeof(bt_bd_addr_t));
    }

    if (NULL != coming_device) {
        coming_device_state.call_state = coming_device->call_state;
        coming_device_state.sco_state = coming_device->call_audio_state;
        coming_device_state.music_state = coming_device->media_state;
        bt_sink_srv_memcpy(coming_device_state.address, coming_device->address, sizeof(bt_bd_addr_t));
    } else {
        if (AUDIO_SRC_SRV_PSEUDO_DEVICE_A2DP == coming->type ||
            AUDIO_SRC_SRV_PSEUDO_DEVICE_AWS_A2DP == coming->type) {
            bt_sink_srv_music_device_t *music_device
                    = bt_sink_srv_music_get_device(BT_SINK_SRV_MUSIC_DEVICE_PSE_HD, coming);

            if (NULL != music_device) {
                coming_device_state.music_state = BT_SINK_SRV_STATE_STREAMING;
                bt_sink_srv_memcpy(coming_device_state.address, music_device->dev_addr, sizeof(bt_bd_addr_t));
            }
        }
    }

    /* 3. Get customer config policy. */
    const audio_src_srv_handle_t *running_psedev = audio_src_srv_get_runing_pseudo_device();
    if (NULL != running_psedev && running_psedev == current) {
        result = bt_customer_config_allow_play(&current_device_state, &coming_device_state);
        bt_sink_srv_report_id("[Sink][StaMgr]psedev compare, customer result: 0x%x", 1, result);
    } else {
        bt_sink_srv_report_id("[Sink][StaMgr]psedev compare, customer compare not allowed, running:0x%x", 1, running_psedev);
    }

    /* 4. Execute Sink Service default policy. */
    bool is_current_call = BT_SINK_SRV_STATE_MANAGER_IS_CALL_DEVICE(current_device, current);
    bool is_coming_call = BT_SINK_SRV_STATE_MANAGER_IS_CALL_DEVICE(coming_device, coming);
#ifdef AIR_BT_SINK_SRV_STATE_MANAGER_DUMMY_DEIVCE_ENABLE
    bool is_current_dummy = BT_SINK_SRV_STATE_MANAGER_IS_DUMMY_DEVICE(current_device, current);
    bool is_coming_dummy = BT_SINK_SRV_STATE_MANAGER_IS_DUMMY_DEVICE(coming_device, coming);
#endif

    if (BT_SINK_SRV_ALLOW_RESULT_BYPASS == result) {
        if (is_coming_call) {
            if (is_current_call
#ifdef AIR_BT_SINK_SRV_STATE_MANAGER_DUMMY_DEIVCE_ENABLE
                || is_current_dummy
#endif
            ) {
                if (NULL != coming_device && context->focus_call_device == coming_device) {
#ifdef AIR_BT_SINK_SRV_STATE_MANAGER_DUMMY_DEIVCE_ENABLE
                    if (current_device == coming_device && coming == coming_device->dummy_device) {
                        result = BT_SINK_SRV_ALLOW_RESULT_DISALLOW;
                    } else {
                        result = BT_SINK_SRV_ALLOW_RESULT_ALLOW;
                    }
#else
                    result = BT_SINK_SRV_ALLOW_RESULT_ALLOW;
#endif
                } else {
                    result = BT_SINK_SRV_ALLOW_RESULT_DISALLOW; /* Call cannot interrupt call. */
                }
            } else {
                result = BT_SINK_SRV_ALLOW_RESULT_ALLOW; /* Call can interrupt media. */
            }
        } else {
            if (is_current_call
#ifdef AIR_BT_SINK_SRV_STATE_MANAGER_DUMMY_DEIVCE_ENABLE
                || is_current_dummy
#endif
            ) {
                result = BT_SINK_SRV_ALLOW_RESULT_DISALLOW; /* Media cannot interrupt call. */
            } else {
                result = BT_SINK_SRV_ALLOW_RESULT_ALLOW; /* Media can interrupt media. */
            }
        }
    }

    /* 5. Update focus device. */
    if (BT_SINK_SRV_ALLOW_RESULT_ALLOW == result) {
        if (NULL != coming_device) {
            if (is_coming_call
#ifdef AIR_BT_SINK_SRV_STATE_MANAGER_DUMMY_DEIVCE_ENABLE
                || is_coming_dummy
#endif
            ) {
                bt_sink_srv_state_manager_set_focus_call_device(context, coming_device, true);
            } else {
                bt_sink_srv_state_manager_set_focus_media_device(context, coming_device, true);
            }
        }

        bt_sink_srv_event_callback(
                BT_SINK_SRV_EVENT_PLAYING_DEVICE_CHANGE,
                &coming_device_state,
                sizeof(bt_sink_srv_device_state_t));
    }

    if (AUDIO_SRC_SRV_PSEUDO_DEVICE_HFP == current->type &&
        AUDIO_SRC_SRV_PSEUDO_DEVICE_HFP == coming->type) {
        result = BT_SINK_SRV_ALLOW_RESULT_DISALLOW;
    }

    bt_sink_srv_report_id("[Sink][StaMgr]psedev compare, final result: 0x%x", 1, result);
    return result;
}

void bt_sink_srv_state_manager_running_psedev_change(audio_src_srv_handle_t *running)
{
    bt_sink_srv_state_manager_device_t *device = NULL;
    bt_sink_srv_state_manager_context_t *context = bt_sink_srv_state_manager_get_context();

    if (NULL == running || !BT_SINK_SRV_STATE_MANAGER_IS_SINK_SRV_DEVICE(running->type)) {
        return;
    }

    /* 1. Get device. */
    device = bt_sink_srv_state_manager_get_device_by_psedev(context, running);
    bt_sink_srv_report_id("[Sink][StaMgr]running psedev change, device: 0x%x", 1, device);

#if defined(MTK_AWS_MCE_ENABLE) && defined(AIR_MULTI_POINT_ENABLE)
        bt_aws_mce_role_t role = bt_connection_manager_device_local_info_get_aws_role();
        bt_aws_mce_srv_link_type_t link = bt_aws_mce_srv_get_link_type();
        bt_sink_srv_report_id("[Sink][StaMgr]running psedev change, role: 0x%x link_type: 0x%x", 2, role, link);
    
        if (BT_AWS_MCE_ROLE_PARTNER == role && BT_AWS_MCE_SRV_LINK_NORMAL == link) {
            return;
        }
#endif

    /* 2. Swap state. */
    if (NULL != device) {
        bt_sink_srv_state_manager_update_state(context, device, running, false);
    }
}

bt_status_t bt_sink_srv_get_playing_device_state(bt_sink_srv_device_state_t *device_state)
{
    bt_sink_srv_state_manager_device_t *device = NULL;
    const audio_src_srv_handle_t *psedev = audio_src_srv_get_runing_pseudo_device();
    bt_sink_srv_state_manager_context_t *context = bt_sink_srv_state_manager_get_context();

    if (NULL != psedev && BT_SINK_SRV_STATE_MANAGER_IS_SINK_SRV_DEVICE(psedev->type)) {
        device = bt_sink_srv_state_manager_get_device_by_psedev(context, (audio_src_srv_handle_t *)psedev);

        if (NULL != device) {
            device_state->call_state = device->call_state;
            device_state->sco_state = device->call_audio_state;
            device_state->music_state = device->media_state;
            bt_sink_srv_memcpy(device_state->address, device->address, sizeof(bt_bd_addr_t));

            return BT_STATUS_SUCCESS;
        }
    }

    return BT_STATUS_FAIL;
}

static bt_sink_srv_allow_result_t default_bt_customer_config_allow_play(
        bt_sink_srv_device_state_t *current,
        bt_sink_srv_device_state_t *coming)
{
    return BT_SINK_SRV_ALLOW_RESULT_BYPASS;
}

