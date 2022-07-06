
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
 * File: app_le_audio_conn_mgr.c
 *
 * Description: This file provides API for LE Audio connection manage.
 *
 */

#ifdef AIR_LE_AUDIO_ENABLE

#include "app_le_audio_conn_mgr.h"

#include "app_le_audio_aird_client.h"

#include "app_ccp.h"
#include "app_mcp.h"
#include "apps_debug.h"
#include "apps_events_bt_event.h"
#include "apps_events_event_group.h"

#include "bt_aws_mce_srv.h"
#include "bt_customer_config.h"
#include "bt_device_manager.h"
#include "bt_device_manager_le.h"
#include "bt_sink_srv_le.h"

#include "multi_ble_adv_manager.h"
#include "nvkey.h"
#include "nvkey_id_list.h"
#include "ui_shell_manager.h"
#ifdef AIR_MULTI_POINT_ENABLE
#include "app_bt_emp_service.h"
#endif

#define LOG_TAG             "[LEA][CONN]"



typedef struct {
    bt_handle_t     conn_handle;
    uint8_t         peer_random_addr[BT_BD_ADDR_LEN];
    uint8_t         peer_type;
    uint8_t         peer_id_addr[BT_BD_ADDR_LEN];

    uint8_t         discover_gtbs   : 1;
    uint8_t         discover_gmcs   : 1;
    uint8_t         support_gtbs    : 1;
    uint8_t         support_gmcs    : 1;
    uint8_t         reserved        : 4;
} PACKED app_lea_conn_info_t;

static app_lea_conn_info_t                 app_lea_conn_info_list[APP_LEA_MAX_CONN_NUM];

#define APP_LEA_BOND_INFO_LENGTH           (sizeof(app_lea_bond_info_t) * APP_LEA_MAX_BOND_NUM)
static app_lea_bond_info_t                 app_lea_bond_info_list[APP_LEA_MAX_BOND_NUM];

static app_lea_conn_mgr_connection_cb_t    app_lea_connection_cb = NULL;



/**================================================================================*/
/**                                   Internal API                                 */
/**================================================================================*/
#define APP_LEA_IS_EMPTY_ADDR(X, EMPTY)        (memcmp((X), (EMPTY), BT_BD_ADDR_LEN) == 0)

static uint8_t app_lea_conn_mgr_get_link_count(void)
{
    uint8_t conn_num = 0;
    for (int i = 0; i < APP_LEA_MAX_CONN_NUM; i++) {
        if (app_lea_conn_info_list[i].conn_handle != BT_HANDLE_INVALID) {
            conn_num++;
        }
    }
    return conn_num;
}

static bool app_lea_conn_mgr_disconnect_by_handle(bt_handle_t handle,
                                                  bt_hci_disconnect_reason_t reason)
{
    bt_hci_cmd_disconnect_t disconnect_para;
    disconnect_para.connection_handle = handle;
    disconnect_para.reason = reason;

    bt_status_t bt_status = bt_gap_le_disconnect(&disconnect_para);
    APPS_LOG_MSGID_I(LOG_TAG" disconnect_by_handle, handle=0x%04X reason=0x%02X bt_status=0x%08X",
                     3, handle, reason, bt_status);
    return (bt_status == BT_STATUS_SUCCESS);
}

static bt_status_t app_lea_conn_mgr_check_profile_supported(bt_handle_t handle)
{
    bt_status_t result = BT_STATUS_FAIL;
    uint8_t index = app_lea_conn_mgr_get_index(handle);
    if (index == APP_LEA_MAX_CONN_NUM) {
        result = BT_STATUS_FAIL;
        goto exit;
    }

    if (!app_lea_conn_info_list[index].discover_gtbs || !app_lea_conn_info_list[index].discover_gmcs) {
        result = BT_STATUS_BUSY;
        goto exit;
    }

    if (!app_lea_conn_info_list[index].support_gtbs && !app_lea_conn_info_list[index].support_gmcs) {
        APPS_LOG_MSGID_E(LOG_TAG" check_profile_supported, unsupported", 0);
        result = BT_STATUS_UNSUPPORTED;
    }

exit:
    // APPS_LOG_MSGID_I(LOG_TAG" check_profile_supported, result=0x%08X", 1, result);
    return result;
}

static void app_lea_conn_mgr_discover_ccp_callback(bt_handle_t handle, bool support_gtbs)
{
    uint8_t index = app_lea_conn_mgr_get_index(handle);
    if (index == APP_LEA_MAX_CONN_NUM) {
        return;
    }

    app_lea_conn_info_list[index].discover_gtbs = TRUE;
    app_lea_conn_info_list[index].support_gtbs = (support_gtbs ? TRUE : FALSE);

    if (BT_STATUS_UNSUPPORTED == app_lea_conn_mgr_check_profile_supported(handle)) {
        //app_lea_conn_mgr_disconnect_by_handle(handle BT_HCI_STATUS_CONNECTION_TERMINATED_BY_LOCAL_HOST);
    }
}

static void app_lea_conn_mgr_discover_mcp_callback(bt_handle_t handle, bool support_gmcs)
{
    uint8_t index = app_lea_conn_mgr_get_index(handle);
    if (index == APP_LEA_MAX_CONN_NUM) {
        return;
    }

    app_lea_conn_info_list[index].discover_gmcs = TRUE;
    app_lea_conn_info_list[index].support_gmcs = (support_gmcs ? TRUE : FALSE);

    if (BT_STATUS_UNSUPPORTED == app_lea_conn_mgr_check_profile_supported(handle)) {
        //app_lea_conn_mgr_disconnect_by_handle(handle BT_HCI_STATUS_CONNECTION_TERMINATED_BY_LOCAL_HOST);
    }
}

static void app_lea_conn_mgr_clear_conn_info(void)
{
    APPS_LOG_MSGID_I(LOG_TAG" clear_conn_info", 0);
    memset(&app_lea_conn_info_list[0], 0, APP_LEA_MAX_CONN_NUM * sizeof(app_lea_conn_info_t));
    for (int i = 0; i < APP_LEA_MAX_CONN_NUM; i++) {
        app_lea_conn_info_list[i].conn_handle = BT_HANDLE_INVALID;
    }
}

static void app_lea_conn_mgr_print_bond_info(bool now)
{
    for (int i = 0; i < APP_LEA_MAX_BOND_NUM; i++) {
        const uint8_t *addr = app_lea_bond_info_list[i].bond_addr;
        if (now) {
            APPS_LOG_MSGID_I(LOG_TAG" update_bond_info, [now][%d] used=%d addr_type=%d bond_addr=%02X:%02X:%02X:%02X:%02X:%02X",
                             9, i, app_lea_bond_info_list[i].used, app_lea_bond_info_list[i].addr_type,
                             addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);
        } else {
            APPS_LOG_MSGID_I(LOG_TAG" update_bond_info, [before][%d] used=%d addr_type=%d bond_addr=%02X:%02X:%02X:%02X:%02X:%02X",
                             9, i, app_lea_bond_info_list[i].used, app_lea_bond_info_list[i].addr_type,
                             addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);
        }
    }
}

static void app_lea_conn_mgr_update_bond_info(uint8_t addr_type, uint8_t *bd_addr)
{
    bool duplicate = FALSE;
    uint8_t empty_addr[BT_BD_ADDR_LEN] = {0};

    if (APP_LEA_IS_EMPTY_ADDR(bd_addr, empty_addr) || addr_type > BT_ADDR_RANDOM_IDENTITY) {
        APPS_LOG_MSGID_I(LOG_TAG" update_bond_info, error addr/type", 0);
        return;
    }

    app_lea_conn_mgr_print_bond_info(FALSE);
    for (int i = 0; i < APP_LEA_MAX_BOND_NUM; i++) {
        const uint8_t *addr = app_lea_bond_info_list[i].bond_addr;
        if (memcmp(addr, bd_addr, BT_BD_ADDR_LEN) == 0) {
            duplicate = TRUE;
            break;
        }
    }

    if (duplicate) {
        APPS_LOG_MSGID_I(LOG_TAG" update_bond_info, duplicate addr", 0);
        return;
    }

    for (int i = (APP_LEA_MAX_BOND_NUM - 1 - 1); i >= 0; i--) {
        if (app_lea_bond_info_list[i].used) {
            memcpy(&app_lea_bond_info_list[i + 1], &app_lea_bond_info_list[i], sizeof(app_lea_bond_info_t));
        }
    }
    app_lea_bond_info_list[0].used = TRUE;
    app_lea_bond_info_list[0].addr_type = addr_type;
    memcpy(app_lea_bond_info_list[0].bond_addr, bd_addr, BT_BD_ADDR_LEN);

    app_lea_conn_mgr_print_bond_info(TRUE);
}

static void app_lea_conn_mgr_save_bond_info(void)
{
    nvkey_status_t status = nvkey_write_data(NVID_APP_LE_AUDIO_CONN_FLAG,
                                             (const uint8_t *)&app_lea_bond_info_list[0],
                                             APP_LEA_BOND_INFO_LENGTH);
    APPS_LOG_MSGID_I(LOG_TAG" save_bond_info, status=%d", 1, status);
}

static void app_lea_conn_mgr_restore_bond_info(void)
{
    uint32_t size = APP_LEA_BOND_INFO_LENGTH;
    nvkey_status_t status = nvkey_read_data(NVID_APP_LE_AUDIO_CONN_FLAG,
                                            (uint8_t *)&app_lea_bond_info_list[0],
                                            &size);
    APPS_LOG_MSGID_I(LOG_TAG" restore_bond_info, read status=%d", 1, status);
    if (status == NVKEY_STATUS_OK) {
        app_lea_conn_mgr_print_bond_info(TRUE);
    } else if (status == NVKEY_STATUS_ITEM_NOT_FOUND) {
        memset(&app_lea_bond_info_list[0], 0, APP_LEA_BOND_INFO_LENGTH);
        nvkey_write_data(NVID_APP_LE_AUDIO_CONN_FLAG,
                         (const uint8_t *)&app_lea_bond_info_list[0],
                         APP_LEA_BOND_INFO_LENGTH);
    } else {
        memset(&app_lea_bond_info_list[0], 0, APP_LEA_BOND_INFO_LENGTH);
    }
}





/**================================================================================*/
/**                                 APP Event Handler                              */
/**================================================================================*/
static void app_lea_conn_mgr_bt_dm_event_group(uint32_t event_id, void *extra_data, size_t data_len)
{
    bt_device_manager_power_event_t evt;
    bt_device_manager_power_status_t status;
    bt_event_get_bt_dm_event_and_status(event_id, &evt, &status);
    switch (evt) {
        case BT_DEVICE_MANAGER_POWER_EVT_ACTIVE_COMPLETE: {
            if (BT_DEVICE_MANAGER_POWER_STATUS_SUCCESS == status) {
                APPS_LOG_MSGID_I(LOG_TAG" BT_DM Power ON", 0);
                app_lea_conn_mgr_clear_conn_info();
            }
            break;
        }

        case BT_DEVICE_MANAGER_POWER_EVT_STANDBY_COMPLETE: {
            if (BT_DEVICE_MANAGER_POWER_RESET_TYPE_NORMAL == status) {
                APPS_LOG_MSGID_I(LOG_TAG" BT_DM POWER OFF", 0);
                app_lea_conn_mgr_clear_conn_info();
                app_lea_conn_mgr_save_bond_info();
            }
            break;
        }
    }
}

static void app_lea_conn_mgr_bt_sink_event_group(uint32_t event_id, void *extra_data, size_t data_len)
{

}

static void app_lea_conn_mgr_bt_event_group(uint32_t event_id, void *extra_data, size_t data_len)
{
    apps_bt_event_data_t *bt_event_data = (apps_bt_event_data_t *)extra_data;
    if (bt_event_data == NULL) {
        APPS_LOG_MSGID_E(LOG_TAG" BT event, bt_event_data is NULL", 0);
        return;
    }

    switch (event_id) {
        case BT_GAP_LE_CONNECT_IND: {
            bt_gap_le_connection_ind_t *conn_ind = (bt_gap_le_connection_ind_t *)bt_event_data->buffer;
            if (conn_ind == NULL || BT_HANDLE_INVALID == conn_ind->connection_handle) {
                break;
            }

            uint8_t *local_addr = (uint8_t *)conn_ind->local_addr.addr;
            bt_bd_addr_t adv_addr = {0};
            if (multi_ble_adv_manager_get_random_addr_and_adv_handle(MULTI_ADV_INSTANCE_NOT_RHO, &adv_addr, NULL)
                && memcmp(&adv_addr, local_addr, BT_BD_ADDR_LEN) == 0) {
                // le audio link
            } else {
                APPS_LOG_MSGID_I(LOG_TAG" BT_LE_CONNECT_IND, not le audio link", 0);
                break;
            }

            bt_handle_t conn_handle = conn_ind->connection_handle;
            uint8_t *addr = (uint8_t *)conn_ind->peer_addr.addr;
            bt_addr_type_t peer_type = conn_ind->peer_addr.type;

            int index = -1;
            for (int i = 0; i < APP_LEA_MAX_CONN_NUM; i++) {
                if (app_lea_conn_info_list[i].conn_handle == BT_HANDLE_INVALID) {
                    APPS_LOG_MSGID_I(LOG_TAG" BT_LE_CONNECT_IND, lea_conn_info[%d] handle=0x%04X peer_addr=%02X:%02X:%02X:%02X:%02X:%02X peer_type=%d",
                                     9, i, conn_handle, addr[0], addr[1], addr[2], addr[3], addr[4], addr[5], peer_type);
                    app_lea_conn_info_list[i].conn_handle = conn_handle;
                    if (peer_type == BT_ADDR_RANDOM) {
                        memcpy(app_lea_conn_info_list[i].peer_random_addr, addr, BT_BD_ADDR_LEN);
                        memset(app_lea_conn_info_list[i].peer_id_addr, 0, BT_BD_ADDR_LEN);
                        app_lea_conn_info_list[i].peer_type = BT_ADDR_RANDOM;
                    } else {
                        memset(app_lea_conn_info_list[i].peer_random_addr, 0, BT_BD_ADDR_LEN);
                        memcpy(app_lea_conn_info_list[i].peer_id_addr, addr, BT_BD_ADDR_LEN);
                        app_lea_conn_info_list[i].peer_type = peer_type;
                    }
                    index = i;

                    if (app_lea_connection_cb != NULL) {
                        app_lea_connection_cb(TRUE, index, peer_type, addr, 0x00);
                    }
                    break;
                }
            }
            if (index == -1) {
                APPS_LOG_MSGID_E(LOG_TAG" BT_LE_CONNECT_IND, lea_conn_info full", 0);
            }
            break;
        }

        case BT_GAP_LE_DISCONNECT_IND: {
            bt_hci_evt_disconnect_complete_t *disconn_ind = (bt_hci_evt_disconnect_complete_t *)bt_event_data->buffer;
            if (disconn_ind == NULL || BT_HANDLE_INVALID == disconn_ind->connection_handle) {
                break;
            }

            bt_handle_t conn_handle = disconn_ind->connection_handle;
            APPS_LOG_MSGID_I(LOG_TAG" BT_LE_DISCONNECT_IND, lea_conn_info handle=0x%04X", 1, conn_handle);
            int index = -1;
            for (int i = 0; i < APP_LEA_MAX_CONN_NUM; i++) {
                if (app_lea_conn_info_list[i].conn_handle == conn_handle) {
                    APPS_LOG_MSGID_I(LOG_TAG" BT_LE_DISCONNECT_IND, lea_conn_info[%d] handle=0x%04X reason=0x%02X",
                                     3, i, conn_handle, disconn_ind->reason);
                    if (app_lea_connection_cb != NULL) {
                        uint8_t peer_type = app_lea_conn_info_list[i].peer_type;
                        const uint8_t *peer_addr = NULL;
                        if (peer_type == BT_ADDR_RANDOM) {
                            peer_addr = app_lea_conn_info_list[i].peer_random_addr;
                        } else {
                            peer_addr = app_lea_conn_info_list[i].peer_id_addr;
                        }
                        app_lea_connection_cb(FALSE, i, peer_type, peer_addr, disconn_ind->reason);
                    }
                    memset(&app_lea_conn_info_list[i], 0, sizeof(app_lea_conn_info_t));
                    app_lea_conn_info_list[i].conn_handle = BT_HANDLE_INVALID;
                    index = i;
                    break;
                }
            }
            if (index == -1) {
                APPS_LOG_MSGID_I(LOG_TAG" BT_LE_DISCONNECT_IND, no match item", 0);
            }
            break;
        }

        case BT_GAP_LE_BONDING_COMPLETE_IND: {
            bt_gap_le_bonding_complete_ind_t *ind = (bt_gap_le_bonding_complete_ind_t *)bt_event_data->buffer;
            if (ind == NULL || BT_HANDLE_INVALID == ind->handle) {
                break;
            }

            bt_handle_t conn_handle = ind->handle;
            APPS_LOG_MSGID_I(LOG_TAG" BT_BONDING_COMPLETE_IND, handle=%d", 1, conn_handle);
            for (int i = 0; i < APP_LEA_MAX_CONN_NUM; i++) {
                if (app_lea_conn_info_list[i].conn_handle == conn_handle) {
                    uint8_t *addr = app_lea_conn_info_list[i].peer_random_addr;
                    APPS_LOG_MSGID_I(LOG_TAG" BT_BONDING_COMPLETE_IND, lea_conn_info[%d] handle=0x%04X",
                                     2, i, conn_handle);
                    bt_device_manager_le_bonded_info_t *bonded_info = bt_device_manager_le_get_bonding_info_by_addr_ext((bt_bd_addr_t *)addr);
                    if (bonded_info == NULL) {
                        break;
                    }

                    uint8_t type = (uint8_t)bonded_info->info.identity_addr.address.type;
                    uint8_t *bd_addr = (uint8_t *)bonded_info->info.identity_addr.address.addr;
                    APPS_LOG_MSGID_I(LOG_TAG" BT_BONDING_COMPLETE_IND, lea_conn_info[%d] type=%d bd_addr=%02X:%02X:%02X:%02X:%02X:%02X",
                                     8, i, type, bd_addr[0], bd_addr[1], bd_addr[2], bd_addr[3], bd_addr[4], bd_addr[5]);

                    uint8_t empty_addr[BT_BD_ADDR_LEN] = {0};
                    if (type == 0xFF || APP_LEA_IS_EMPTY_ADDR(bd_addr, empty_addr)) {
                        // Not RPA addr, cannot get IDA
                        bd_addr = app_lea_conn_info_list[i].peer_random_addr;
                        type = app_lea_conn_info_list[i].peer_type;
                        memset(app_lea_conn_info_list[i].peer_id_addr, 0, BT_BD_ADDR_LEN);
                        APPS_LOG_MSGID_I(LOG_TAG" BT_BONDING_COMPLETE_IND, NON-RPA use type=%d addr=%02X:%02X:%02X:%02X:%02X:%02X",
                                         7, type, bd_addr[0], bd_addr[1], bd_addr[2], bd_addr[3], bd_addr[4], bd_addr[5]);
                    } else {
                        memcpy(app_lea_conn_info_list[i].peer_id_addr, bd_addr, BT_BD_ADDR_LEN);
                        app_lea_conn_info_list[i].peer_type = type;
                    }

                    app_lea_conn_mgr_update_bond_info(type, bd_addr);
                    break;
                }
            }
            break;
        }
    }
}



/**================================================================================*/
/**                                     Public API                                 */
/**================================================================================*/
void app_lea_conn_mgr_register_connection_cb(app_lea_conn_mgr_connection_cb_t cb)
{
    app_lea_connection_cb = cb;
}

bool app_lea_conn_mgr_is_ever_connected(void)
{
    bool ret = FALSE;
    for (int i = 0; i < APP_LEA_MAX_BOND_NUM; i++) {
        if (app_lea_bond_info_list[i].used) {
            ret = TRUE;
            break;
        }
    }
    return ret;
}

bool app_lea_conn_mgr_is_connected(const uint8_t *addr)
{
    bool ret = FALSE;
    for (int i = 0; i < APP_LEA_MAX_CONN_NUM; i++) {
        if (app_lea_conn_info_list[i].conn_handle != BT_HANDLE_INVALID
            && (memcmp(app_lea_conn_info_list[i].peer_random_addr, addr, BT_BD_ADDR_LEN) == 0
                || memcmp(app_lea_conn_info_list[i].peer_id_addr, addr, BT_BD_ADDR_LEN) == 0)) {
            ret = TRUE;
            break;
        }
    }
    return ret;
}

bt_handle_t app_lea_conn_mgr_get_handle(uint8_t index)
{
    return (index < APP_LEA_MAX_CONN_NUM ? app_lea_conn_info_list[index].conn_handle : BT_HANDLE_INVALID);
}

uint8_t app_lea_conn_mgr_get_index(bt_handle_t handle)
{
    int i = 0;
    for (i = 0; i < APP_LEA_MAX_CONN_NUM; i++) {
        if (app_lea_conn_info_list[i].conn_handle == handle && handle != BT_HANDLE_INVALID) {
            break;
        }
    }
    return i;
}

uint8_t app_lea_conn_mgr_get_conn_num(void)
{
    return app_lea_conn_mgr_get_link_count();
}

void app_lea_conn_mgr_get_conn_info(uint8_t *num, bt_addr_t *bdaddr)
{
    if (num == NULL || bdaddr == NULL) {
        APPS_LOG_MSGID_E(LOG_TAG" get_conn_info, error num=0x%08X bdaddr=0x%08X", 2, num, bdaddr);
        return;
    }

    uint8_t empty_addr[BT_BD_ADDR_LEN] = {0};
    int conn_num = app_lea_conn_mgr_get_conn_num();
    if (conn_num == 0) {
        APPS_LOG_MSGID_I(LOG_TAG" get_conn_info, num=0", 0);
        return;
    }

    int index = 0;
    for (int i = 0; i < APP_LEA_MAX_CONN_NUM; i++) {
        if (app_lea_conn_info_list[i].conn_handle != BT_HANDLE_INVALID) {
            uint8_t peer_type = app_lea_conn_info_list[i].peer_type;
            const uint8_t *peer_addr = NULL;
            if (peer_type == BT_ADDR_RANDOM) {
                peer_addr = app_lea_conn_info_list[i].peer_random_addr;
            } else {
                peer_addr = app_lea_conn_info_list[i].peer_id_addr;
            }

            if (!APP_LEA_IS_EMPTY_ADDR(peer_addr, empty_addr)) {
                bdaddr[index].type = peer_type;
                memcpy(bdaddr[index].addr, peer_addr, BT_BD_ADDR_LEN);
                index++;
                *num = index;

                APPS_LOG_MSGID_I(LOG_TAG" get_conn_info, lea_num=%d addr_type=%d %02X:%02X:%02X:%02X:%02X:%02X",
                                 8, *num, peer_type,
                                 peer_addr[0], peer_addr[1], peer_addr[2], peer_addr[3], peer_addr[4], peer_addr[5]);
                if (index == APP_LEA_MAX_CONN_NUM) {
                    break;
                }
            }
        }
    }
}

uint8_t app_lea_conn_mgr_get_support_max_conn_num(void)
{
#if (APP_LE_AUDIO_ADV_CONN_MODE == APP_LE_AUDIO_MODE_DONGLE_ONE)
    return 1;
#else
    bool support_emp = FALSE;
#ifdef AIR_MULTI_POINT_ENABLE
    support_emp = app_bt_emp_is_enable();
#endif

    if (support_emp) {
        return APP_LEA_MAX_CONN_NUM;
    } else {
        return 1;
    }
#endif
}

app_lea_bond_info_t *app_lea_conn_mgr_get_bond_info(void)
{
    return &app_lea_bond_info_list[0];
}

void app_lea_conn_mgr_reset_bond_info(void)
{
    APPS_LOG_MSGID_I(LOG_TAG" reset_bond_info", 0);
    memset(&app_lea_bond_info_list[0], 0, APP_LEA_BOND_INFO_LENGTH);
    app_lea_conn_mgr_save_bond_info();
}

bool app_lea_conn_mgr_is_le_bond(const uint8_t *addr)
{
    bt_device_manager_le_bonded_info_t *bonded_info = bt_device_manager_le_get_bonding_info_by_addr_ext((bt_bd_addr_t *)addr);
    return (bonded_info != NULL);
}

bool app_lea_conn_mgr_get_ida(const uint8_t *random_addr, uint8_t *type, uint8_t *id_addr)
{
    bool ret = FALSE;
    if (type == NULL || id_addr == NULL) {
        APPS_LOG_MSGID_E(LOG_TAG" get_ida, error type=0x%08X id_addr=0x%08X", 2, type, id_addr);
        return ret;
    }

    bt_device_manager_le_bonded_info_t *bonded_info = bt_device_manager_le_get_bonding_info_by_addr_ext((bt_bd_addr_t *)random_addr);
    if (bonded_info != NULL) {
        *type = (uint8_t)bonded_info->info.identity_addr.address.type;
        uint8_t *bd_addr = (uint8_t *)bonded_info->info.identity_addr.address.addr;
        memcpy(id_addr, bd_addr, BT_BD_ADDR_LEN);
        ret = TRUE;
    }

    return ret;
}

// if addr==NULL, disconnect all
void app_lea_conn_mgr_disconnect(const uint8_t *addr, uint8_t reason)
{
    if (addr != NULL) {
        APPS_LOG_MSGID_I(LOG_TAG" disconnect, addr=%02X:%02X:%02X:%02X:%02X:%02X reason=0x%02X",
                         7, addr[0], addr[1], addr[2], addr[3], addr[4], addr[5], reason);
        uint8_t empty_addr[BT_BD_ADDR_LEN] = {0};
        if (APP_LEA_IS_EMPTY_ADDR(addr, empty_addr)) {
            APPS_LOG_MSGID_E(LOG_TAG" disconnect, empty addr", 0);
            return;
        }

        for (int i = 0; i < APP_LEA_MAX_CONN_NUM; i++) {
            if (memcmp(addr, app_lea_conn_info_list[i].peer_id_addr, BT_BD_ADDR_LEN) == 0
                || memcmp(addr, app_lea_conn_info_list[i].peer_random_addr, BT_BD_ADDR_LEN) == 0) {
                app_lea_conn_mgr_disconnect_by_handle(app_lea_conn_info_list[i].conn_handle,
                                                      reason);
                break;
            }
        }
    } else {
        APPS_LOG_MSGID_I(LOG_TAG" disconnect all, reason=0x%02X", 1, reason);
        for (int i = 0; i < APP_LEA_MAX_CONN_NUM; i++) {
            if (BT_HANDLE_INVALID != app_lea_conn_info_list[i].conn_handle) {
                app_lea_conn_mgr_disconnect_by_handle(app_lea_conn_info_list[i].conn_handle,
                                                      reason);
            }
        }
    }
}

void app_lea_conn_mgr_disconnect_dongle(void)
{
    uint8_t disconnect_num = 0;
    for (int i = 0; i < APP_LEA_MAX_CONN_NUM; i++) {
        if (BT_HANDLE_INVALID != app_lea_conn_info_list[i].conn_handle) {
            if (app_le_audio_aird_client_is_support_hid_call(app_lea_conn_info_list[i].conn_handle)) {
                app_lea_conn_mgr_disconnect_by_handle(app_lea_conn_info_list[i].conn_handle,
                                                      BT_HCI_STATUS_CONNECTION_TERMINATED_BY_LOCAL_HOST);
                disconnect_num++;
            }
        }
    }
    APPS_LOG_MSGID_I(LOG_TAG" disconnect_dongle, num=%d", 1, disconnect_num);
}

void app_lea_conn_mgr_enable_multi_conn(bool enable)
{
#ifdef AIR_LE_AUDIO_MULTIPOINT_ENABLE
    if (enable) {
        multi_ble_adv_manager_set_le_connection_max_count(MULTI_ADV_INSTANCE_NOT_RHO, APP_LEA_MAX_CONN_NUM);
    } else {
        multi_ble_adv_manager_set_le_connection_max_count(MULTI_ADV_INSTANCE_NOT_RHO, 1);
    }
#else
    if (enable) {
        APPS_LOG_MSGID_E(LOG_TAG" enable_multi_conn, cannot support multi point feature", 0);
    }
    multi_ble_adv_manager_set_le_connection_max_count(MULTI_ADV_INSTANCE_NOT_RHO, 1);
#endif
    APPS_LOG_MSGID_I(LOG_TAG" enable_multi_conn, enable=%d", 1, enable);
}

void app_lea_conn_mgr_init(void)
{
    APPS_LOG_MSGID_I(LOG_TAG" init", 0);
    app_le_audio_ccp_init(app_lea_conn_mgr_discover_ccp_callback);
    app_le_audio_mcp_init(app_lea_conn_mgr_discover_mcp_callback);

    app_lea_conn_mgr_clear_conn_info();
    app_lea_conn_mgr_restore_bond_info();
}

void app_lea_conn_mgr_proc_ui_shell_event(uint32_t event_group,
                                          uint32_t event_id,
                                          void *extra_data,
                                          size_t data_len)
{
    switch (event_group) {
        case EVENT_GROUP_UI_SHELL_BT_DEVICE_MANAGER:
            app_lea_conn_mgr_bt_dm_event_group(event_id, extra_data, data_len);
            break;
        case EVENT_GROUP_UI_SHELL_BT_SINK:
            app_lea_conn_mgr_bt_sink_event_group(event_id, extra_data, data_len);
            break;
        case EVENT_GROUP_UI_SHELL_BT:
            app_lea_conn_mgr_bt_event_group(event_id, extra_data, data_len);
            break;
    }
}

#endif /* AIR_LE_AUDIO_ENABLE */
