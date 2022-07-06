/* Copyright Statement:
*
* (C) 2022 Airoha Technology Corp. All rights reserved.
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
/* MediaTek restricted information */

#ifndef __BT_ULL_LE_H__
#define __BT_ULL_LE_H__

#include "bt_type.h"
#include "bt_system.h"
#include "bt_hci.h"

#define BT_ULL_LE_AIR_CIS_CONNECT_REQUEST_IND                    (BT_MODULE_ULL | 0x0001)       /**< AIR CIS connection request indication with #bt_ull_le_air_cis_request_ind_t. */
#define BT_ULL_LE_AIR_CIS_ESTABLISHED_IND                        (BT_MODULE_ULL | 0x0002)       /**< AIR CIS established indication with #bt_ull_le_air_cis_established_ind_t. */
#define BT_ULL_LE_AIR_CIS_DISCONNECT_COMPLETE_IND                (BT_MODULE_ULL | 0x0003)       /**< AIR CIS disconnected indication with #bt_ull_le_air_cis_disconnect_t. */
#define BT_ULL_LE_AIR_CIG_PARAMS_CHANGED_IND                     (BT_MODULE_ULL | 0x0004)       /**< AIR CIG parameters changed indication with #bt_ull_le_air_cig_params_changed_ind_t. */
#define BT_ULL_LE_AIR_CIS_UPLINK_ACTIVIATED_IND                  (BT_MODULE_ULL | 0x0005)       /**< Uplink of AIR CIS activiated indication with #bt_ull_le_activiate_uplink_t.. */

#define BT_ULL_LE_SET_AIR_CIG_PARAMS_CNF                         (BT_MODULE_ULL | 0x0006)       /**< Set AIR CIG parameters confirmation with #bt_ull_le_set_air_cig_params_cnf_t. */
#define BT_ULL_LE_REMOVE_AIR_CIG_CNF                             (BT_MODULE_ULL | 0x0008)       /**< Remove AIR CIG confirmation with #bt_ull_le_remove_air_cig_cnf_t. */
#define BT_ULL_LE_CREATE_AIR_CIS_CNF                             (BT_MODULE_ULL | 0x0007)       /**< Create AIR CIS connection confirmation with NULL payload. */
#define BT_ULL_LE_ACCEPT_AIR_CIS_CONNECT_REQUEST_CNF             (BT_MODULE_ULL | 0x0009)       /**< Accept AIR CIS connection request confirmation with NULL payload. */
#define BT_ULL_LE_REJECT_AIR_CIS_CONNECT_REQUEST_CNF             (BT_MODULE_ULL | 0x000A)       /**< Reject AIR CIS connection request confirmation with NULL payload. */
#define BT_ULL_LE_SETUP_AIR_ISO_DATA_PATH_CNF                    (BT_MODULE_ULL | 0x000B)       /**< Setup AIR ISO data path confirmation with #bt_ull_le_setup_air_iso_data_path_cnf_t. */
#define BT_ULL_LE_REMOVE_AIR_ISO_DATA_PATH_CNF                   (BT_MODULE_ULL | 0x000C)       /**< Remove AIR ISO data path confirmation with #bt_ull_le_remove_air_iso_data_path_cnf_t. */
#define BT_ULL_LE_DISCONNECT_AIR_CIS_CNF                         (BT_MODULE_ULL | 0x000D)       /**< Disconnect AIR CIS confirmation with NULL payload. */
#define BT_ULL_LE_CHANGE_AIR_CIG_PARAMS_CNF                      (BT_MODULE_ULL | 0x000E)       /**< Change AIR CIG parameters confirmation with NULL payload. */
#define BT_ULL_LE_UNMUTE_AIR_CIS_CNF                             (BT_MODULE_ULL | 0x000F)       /**< Unmute AIR CIS  confirmation with NULL payload. */
#define BT_ULL_LE_ACTIVIATE_UPLINK_CNF                           (BT_MODULE_ULL | 0x0010)       /**< Activiate uplink of AIR CIS  confirmation with NULL payload. */
#define BT_ULL_LE_SET_AIR_PARAMS_TABLE_CNF                       (BT_MODULE_ULL | 0x0011)       /**< Set air parameters table confirmation with NULL payload. */

#define BT_ULL_LE_REPLY_AIR_CIS_ACTION_ACCEPT           0x00    /**< Action accept. */
#define BT_ULL_LE_REPLY_AIR_CIS_ACTION_REJECT           0x01    /**< Action reject. */
typedef uint8_t bt_ull_le_reply_air_cis_action_t;  /**< Reply AIR CIS action. */

#define BT_ULL_LE_AIR_ISO_DATA_PATH_DIRECTION_INPUT     0x00    /**< Input. Audio -> Controller*/
#define BT_ULL_LE_AIR_ISO_DATA_PATH_DIRECTION_OUTPUT    0x01    /**< Output. Controller -> Audio*/
typedef uint8_t bt_ull_le_air_iso_data_path_direction_t; /**< AIR ISO data path direction. */

#define BT_ULL_LE_AIR_ISO_DATA_PATH_ID_HCI              0x00    /**< HCI. */
#define BT_ULL_LE_AIR_ISO_DATA_PATH_ID_CHANNEL_1        0x01    /**< Channel 1. */
#define BT_ULL_LE_AIR_ISO_DATA_PATH_ID_CHANNEL_2        0x02    /**< Channel 2. */
#define BT_ULL_LE_AIR_ISO_DATA_PATH_ID_CHANNEL_3        0x03    /**< Channel 3. */
#define BT_ULL_LE_AIR_ISO_DATA_PATH_ID_CHANNEL_4        0x04    /**< Channel 4. */
#define BT_ULL_LE_AIR_ISO_DATA_PATH_ID_DISABLE          0xFF    /**< Disable. */
typedef uint8_t bt_ull_le_air_iso_data_path_id_t;  /**< AIR ISO data path ID. */


/**
 * @brief       AIR CIS disconnect command.
 */
typedef bt_hci_cmd_disconnect_t bt_ull_le_air_cis_disconnect_t;

/**
 *  @brief      Reply AIR CIS Request command.
 */
typedef struct {
    bt_ull_le_reply_air_cis_action_t action;            /**< The action for replying AIR CIS request. */
    union {
        bt_hci_le_accept_cis_request_params_t accept;   /**< The accept parameters. */
        bt_hci_le_reject_cis_request_params_t reject;   /**< The reject parameters. */
    };
} bt_ull_le_reply_air_cis_request_t;

/**
 * @brief       AIR CIS request event.
 */
BT_PACKED (
typedef struct {
    bt_handle_t       acl_connection_handle;            /**< Connection handle of the ACL. */
    bt_handle_t       cis_connection_handle;            /**< Connection handle of the AIR CIS. */
    uint8_t           cig_id;                           /**< AIR CIG ID. */
    uint8_t           cis_id;                           /**< AIR CIS ID. */
}) bt_ull_le_air_cis_request_ind_t;

/**
 * @brief       AIR CIS established event.
 */
BT_PACKED (
typedef struct {
    uint8_t           status;                           /**< The status of the AIR CIS. */
    bt_handle_t       cis_connection_handle;                /**< Connection handle of the AIR CIS. */
    uint32_t          cig_sync_delay : 24;              /**< The maximum time, in microseconds, for transmission of PDUs of all AIR CISes in a AIR CIG in an isochronous interval. */
    uint32_t          cis_sync_delay : 24;              /**< The maximum time, in microseconds, for transmission of PDUs of a AIR CIS in an isochronous interval. */
    uint32_t          transport_latency_m_to_s : 24;    /**< The maximum time, in microseconds, for transmission of SDUs of all AIR CISes in a AIR CIG from slave to master. */
    uint32_t          transport_latency_s_to_m : 24;    /**< The maximum time, in microseconds, for transmission of SDUs of all AIR CISes in a AIR CIG from master to slave. */
    uint8_t           phy_m_to_s;                       /**< The transmitter PHY of packets used from the master to slave. */
    uint8_t           phy_s_to_m;                       /**< The transmitter PHY of packets used from the slave to master. */
    bool              dl_enable;                        /**< The down link active state, ture: active, false: inactive. */
    bool              ul_enable;                        /**< The up link active state, ture: active, false: inactive. */
    uint8_t           nse;                              /**< Maximum number of subevents in each isochronous event. */
    uint8_t           share_count;                      /**< Maximum number of subevents in each isochronous event. */
    uint8_t           bn_m_to_s;                        /**< The burst number for master to slave transmission. */
    uint8_t           bn_s_to_m;                        /**< The burst number for slave to master transmission. */
    uint8_t           ft_m_to_s;                        /**< The flush timeout, in multiples of the ISO_Interval for the AIR CIS, for each payload sent from the master to the slave. */
    uint8_t           ft_s_to_m;                        /**< The flush timeout, in multiples of the ISO_Interval for the AIR CIS, for each payload sent from the slave to the master.*/
    uint16_t          max_pdu_m_to_s;                   /**< Maximum size, in octets, of the payload from master to slave. */
    uint16_t          max_pdu_s_to_m;                   /**< Maximum size, in octets, of the payload from slave to master.*/
    uint16_t          iso_interval;                     /**< The time between two consecutive AIR CIS anchor points.*/
}) bt_ull_le_air_cis_established_ind_t;

/**
 *  @brief      Disconnect complete event information.
 */
BT_PACKED (
typedef struct {
    bt_hci_status_t   status;            /**< Status. */
    bt_handle_t       cis_connection_handle; /**< AIR CIS connection handle. */
    bt_hci_disconnect_reason_t reason;   /**< Disconnect reason. */
}) bt_ull_le_air_cis_disconnect_complete_ind_t;

/**
 *  @brief      CIS parameters of  AIR CIG structure.
 */
BT_PACKED (
typedef struct {
    uint8_t cis_id;             /**< identifies a AIR CIS and is set by the master's Host and passed to the slave's Host through the Link Layers during the process of establishing a AIR CIS. */
    uint16_t max_sdu_m_to_s;    /**< The maximum size of an SDU from the master's Host. */
    uint16_t max_sdu_s_to_m;    /**< The maximum size of an SDU from the slave's Host. */
    uint8_t phy_m_to_s;         /**< identifies which PHY to use for transmission from the master to the slave. */
    uint8_t phy_s_to_m;         /**< identifies which PHY to use for transmission from the slave to the master. */
}) bt_ull_le_air_cis_params_t;

/**
 *  @brief      CIG parameters of setup AIR CIG command.
 */
BT_PACKED (
typedef struct {
    uint32_t sdu_interval_m_to_s : 24;                  /**< The interval, in microseconds, of periodic SDUs. (0x000FF to 0xFFFFF) */
    uint32_t sdu_interval_s_to_m : 24;                  /**< The interval, in microseconds, of periodic SDUs. (0x000FF to 0xFFFFF) */
    uint8_t sca;                                        /**< The worst-case sleep clock accuracy of all the slaves that will participate in the AIR CIG. */
    uint8_t max_share_count;                            /**< The maximum number of times that every CIS Data PDU should be retransmitted. */
    uint16_t iso_interval;                              /**< The time between two consecutive BIG anchor points. (Range: 0x0004 to 0x0C80, Time = N * 1.25 ms.)*/
    uint8_t max_uplink_num;                             /**< The max uplink number in the AIR CIG*/
    uint8_t ft_m_to_s;                                  /**< The flush timeout in multiples of ISO_Interval for each payload sent from the master to slave. (0x01 to 0xFF) */
    uint8_t ft_s_to_m;                                  /**< The flush timeout in multiples of ISO_Interval for each payload sent from the slave to master. (0x01 to 0xFF) */
    uint8_t cis_count;                                  /**< Total number of CISes in the CIG being added or modified. The maximum value is limited to 16. */
    uint8_t cis_list[1];                                /**< The parameters of each CIS in the AIR CIG. */
})bt_ull_le_air_cig_params_t;


/**
 *  @brief      LE Setup AIR CIG parameters command.
 */
BT_PACKED (
typedef struct {
    uint8_t cig_id;                                     /**< Identifies a CIG. This parameter is allocated by the master's Host and passed to the slave's Host through the Link Layers during the process of establishing a CIS. */
    uint8_t client_type;                                /**< The client type of connecting with local device. */
    uint8_t default_params_index;                           /**< The default lantency of air cig. */
    uint8_t list_num;                          /**< Total number of air CIGs. */
    bt_ull_le_air_cig_params_t *params_list;         /**< The parameters of each CIG in different latency. */
}) bt_ull_le_set_air_cig_params_group_t;


/**
 *  @brief      AIR CIS set parameters.
 */
BT_PACKED (
typedef struct {
    bt_handle_t cis_connection_handle;                  /**< The connection handle of the CIS to be established. */
    bt_handle_t acl_connection_handle;                  /**< The connection handle of the ACL connection associated with each CIS to be established. */
    bool        ul_enable;                          /**< The state of the  up link in the air CIS. */
}) bt_ull_le_air_cis_set_t;

/**
 *  @brief      LE create AIR CIS command.
 */
 BT_PACKED(
typedef struct {
    uint8_t cis_count;                                  /**< The total number of CISes established by this command. */
    bt_ull_le_air_cis_set_t *cis_list;                  /**< The connection handle list of the CIS to be established. */
}) bt_ull_le_create_air_cis_t;

/**
 *  @brief      LE remove AIR CIG command.
 */
BT_PACKED (
typedef struct {
    uint8_t cig_id;    /**< identifies a CIG. */
}) bt_ull_le_remove_air_cig_t;

/**
 *  @brief      LE change air parameters command.
 */
BT_PACKED(
typedef struct {
    uint8_t cig_id;                                /**< identifies a CIG. This parameter is allocated by the master's Host and passed to the slave's Host through the Link Layers during the process of establishing a CIS. */
    uint8_t cig_params_index;                      /**< Current latency of CIS in the CIG. */
}) bt_ull_le_change_cig_params_t;

/**
 *  @brief      LE set air parameters table command.
 */
BT_PACKED(
typedef struct {
    uint16_t iso_interval;                 /**< The time between two consecutive BIG anchor points. (Range: 0x0004 to 0x0C80, Time = N * 1.25 ms.)*/
    uint8_t ft_m_to_s;                      /**< The flush timeout in multiples of ISO_Interval for each payload sent from the master to slave. (0x01 to 0xFF) */
    uint8_t ft_s_to_m;                      /**< The flush timeout in multiples of ISO_Interval for each payload sent from the slave to master. (0x01 to 0xFF) */
    uint16_t max_sdu_m_to_s;               /**< The maximum size of an SDU from the master's Host. */
    uint16_t max_sdu_s_to_m;               /**< The maximum size of an SDU from the slave's Host. */
    uint8_t max_share_count;               /**< The maximum number of times that every CIS Data PDU should be retransmitted. */
    uint8_t phy_m_to_s;                    /**< identifies which PHY to use for transmission from the master to the slave. */
    uint8_t phy_s_to_m;                    /**< identifies which PHY to use for transmission from the slave to the master. */
}) bt_ull_le_air_params_t;

/**
 *  @brief      LE set air parameters table command.
 */
BT_PACKED(
typedef struct {
    uint8_t list_num;                            /**< identifies a CIG. This parameter is allocated by the master's Host and passed to the slave's Host through the Link Layers during the process of establishing a CIS. */
    bt_ull_le_air_params_t *params_list;         /**< Current latency of CIS in the CIG. */
}) bt_ull_le_set_air_params_table_t;

/**
 *  @brief      LE unmute AIR CIS  command.
 */
BT_PACKED(
typedef struct {
    bt_handle_t cis_connection_handle;        /**< The connection handle list of the CIS to be established. */
}) bt_ull_le_unmute_air_cis_t;

/**
 *  @brief      LE change air CIS  command.
 */
BT_PACKED(
typedef struct {
    bt_handle_t cis_connection_handle;        /**< The connection handle list of the CIS to be established. */
}) bt_ull_le_activiate_uplink_t;


/**
 *  @brief      LE Setup AIR ISO data path command.
 */
BT_PACKED (
typedef struct {
    bt_handle_t cis_connection_handle;                  /**< Connection handle of the CIS or BIS. Range: 0x0000 to 0x0EFF */
    bt_ull_le_air_iso_data_path_direction_t direction;  /**< The direction for which the data path is being configured. */
    bt_ull_le_air_iso_data_path_id_t data_path_id;      /**< The data transport path used. When set to a value in the range 0x01 to 0xFE, the data path shall use a vendor-specific transport interface */
    uint8_t codec_format;                               /**< The coding format used over the air.*/
    uint16_t company_id;                                /**< Company ID, see Assigned Numbers for Company Identifier. Shall be ignored if coding format is not 0xFF.*/
    uint16_t vendor_codec_id;                           /**< Vendor-defined codec ID. Shall be ignored if octet 0 is not 0xFF.*/
    uint32_t controller_delay : 24;                     /**< Controller delay in microseconds Range: 0x000000 to 0x3D0900 Time range: 0 s to 4 s */
    uint8_t codec_configuration_length;                 /**< Length of codec configuration. */
    uint8_t *codec_configuration;                       /**< Codec-specific configuration data. */
}) bt_ull_le_setup_air_iso_data_path_t;

/** @brief This structure define for #BT_ULL_LE_AIR_CIG_PARAMS_CHANGED_IND. */
BT_PACKED (
typedef struct {
    bt_hci_status_t   status;              /**< Status. */
    uint8_t           cig_id;              /**< identifies a CIG. This parameter is allocated by the master's Host and passed to the slave's Host through the Link Layers during the process of establishing a CIS. */
    uint8_t           params_index;        /**< Current latency of CIS in the CIG. */
}) bt_ull_le_air_cig_params_changed_ind_t;

/** @brief This structure define for #BT_ULL_LE_AIR_CIS_UPLINK_ACTIVIATED_IND. */
BT_PACKED (
typedef struct {
    bt_hci_status_t   status;                    /**< Status. */
    bt_handle_t       cis_connection_handle;     /**< Connection handle of the CIS or BIS. Range: 0x0000 to 0x0EFF */
    bool              dl_enable;                  /**< The dplink of CIS in the CIG is active or inactive. */
    bool              ul_enable;                  /**< The uplink of CIS in the CIG is active or inactive. */
}) bt_ull_le_air_cis_uplink_activiated_ind_t;

/**
 *  @brief      LE Remove AIR ISO data path command.
 */
typedef bt_hci_le_remove_iso_data_path_params_t bt_ull_le_remove_air_iso_data_path_t;

/**
 * @brief This structure defines the associated parameter type in the callback for #BT_ULL_LE_SETUP_AIR_ISO_DATA_PATH_CNF event.
 */
typedef bt_hci_le_setup_iso_data_path_t bt_ull_le_setup_air_iso_data_path_cnf_t;

/**
 * @brief This structure defines the associated parameter type in the callback for #BT_ULL_LE_REMOVE_AIR_ISO_DATA_PATH_CNF event.
 */
typedef bt_hci_le_remove_iso_data_path_t bt_ull_le_remove_air_iso_data_path_cnf_t;

/**
 * @brief This structure defines the associated parameter type in the callback for #BT_ULL_LE_SET_AIR_CIG_PARAMS_CNF event.
 */
typedef bt_hci_le_set_cig_params_t bt_ull_le_set_air_cig_params_cnf_t;

/**
 * @brief This structure defines the associated parameter type in the callback for #BT_ULL_LE_REMOVE_AIR_CIG_CNF event.
 */
typedef bt_hci_le_remove_cig_t bt_ull_le_remove_air_cig_cnf_t;


/**
 * @brief                                This function disconenct air cis.
 * @param[in] disconnect        is the disconnect air cis parameters #bt_ull_le_air_cis_disconnect_t.
 * @return                              #BT_STATUS_SUCCESS, the operation completed successfully.
 *                                           #BT_STATUS_FAIL, the operation has failed.
 */
bt_status_t bt_ull_le_disconnect_air_cis(bt_ull_le_air_cis_disconnect_t *disconnect);

/**
 * @brief                                This function remove air iso data path.
 * @param[in] remove_iso       is the remove air iso data path parameters #bt_ull_le_remove_air_iso_data_path_t.
 * @return                              #BT_STATUS_SUCCESS, the operation completed successfully.
 *                                           #BT_STATUS_FAIL, the operation has failed.
 */
bt_status_t bt_ull_le_remove_air_iso_data_path(bt_ull_le_remove_air_iso_data_path_t *remove_iso);

/**
 * @brief                                This function setup air iso data path.
 * @param[in] setup_iso         is the setup air iso data path parameters #bt_ull_le_setup_air_iso_data_path_t.
 * @return                              #BT_STATUS_SUCCESS, the operation completed successfully.
 *                                           #BT_STATUS_FAIL, the operation has failed.
 */
bt_status_t bt_ull_le_setup_air_iso_data_path(bt_ull_le_setup_air_iso_data_path_t *setup_iso);

/**
 * @brief                                This function reply air cis connection request.
 * @param[in] reply                is the reply air cis connection request parameters #bt_ull_le_reply_air_cis_request_t.
 * @return                              #BT_STATUS_SUCCESS, the operation completed successfully.
 *                                           #BT_STATUS_FAIL, the operation has failed.
 */
bt_status_t bt_ull_le_reply_air_cis_connect_request(bt_ull_le_reply_air_cis_request_t *reply);

/**
 * @brief                                This function set air cig parameters.
 * @param[in] cig_params_group              is the air cig parameters #bt_ull_le_set_air_cig_params_group_t.
 * @param[in] length    is the data length of bt_ull_le_set_air_cig_params_group_t
 * @return                              #BT_STATUS_SUCCESS, the operation completed successfully.
 *                                           #BT_STATUS_FAIL, the operation has failed.
 */
bt_status_t bt_ull_le_set_air_cig_parameters(bt_ull_le_set_air_cig_params_group_t *cig_params_group, uint32_t length);

/**
 * @brief                                This function remove air cig.
 * @param[in] remove_cig              is the remove air cig parameters #bt_ull_le_remove_air_cig_t.
 * @return                              #BT_STATUS_SUCCESS, the operation completed successfully.
 *                                           #BT_STATUS_FAIL, the operation has failed.
 */
bt_status_t bt_ull_le_remove_air_cig(bt_ull_le_remove_air_cig_t *remove_cig);

/**
 * @brief                                This function create air cis.
 * @param[in] create_cis              is the create air cis parameters #bt_ull_le_create_air_cis_t.
 * @return                              #BT_STATUS_SUCCESS, the operation completed successfully.
 *                                           #BT_STATUS_FAIL, the operation has failed.
 */
bt_status_t bt_ull_le_create_air_cis(bt_ull_le_create_air_cis_t *create_cis);

/**
 * @brief                                This function set air params table to controller.
 * @param[in] table                is the set air params table parameters #bt_ull_le_set_air_params_table_t.
 * @return                              #BT_STATUS_SUCCESS, the operation completed successfully.
 *                                           #BT_STATUS_FAIL, the operation has failed.
 */
bt_status_t bt_ull_le_set_air_params_table(bt_ull_le_set_air_params_table_t *table);

/**
 * @brief                                This function switch air cig latency.
 * @param[in] cig_param         is the switch air cig latency parameters #bt_ull_le_change_cig_params_t.
 * @return                              #BT_STATUS_SUCCESS, the operation completed successfully.
 *                                           #BT_STATUS_FAIL, the operation has failed.
 */
bt_status_t bt_ull_le_change_air_params(bt_ull_le_change_cig_params_t *cig_param);

/**
 * @brief                                This function unmute air cis, the controller calculate play en to dsp when reciving this cmd .
 * @param[in] unmute              is the unmute air cis parameters #bt_ull_le_unmute_air_cis_t.
 * @return                              #BT_STATUS_SUCCESS, the operation completed successfully.
 *                                           #BT_STATUS_FAIL, the operation has failed.
 */
bt_status_t bt_ull_le_unmute_air_cis(bt_ull_le_unmute_air_cis_t *unmute);

/**
 * @brief                                This function change uplink path.
 * @param[in] params              is the change uplink path parameters #bt_ull_le_activiate_uplink_t.
 * @return                              #BT_STATUS_SUCCESS, the operation completed successfully.
 *                                           #BT_STATUS_FAIL, the operation has failed.
 */
bt_status_t bt_ull_le_activiate_uplink(bt_ull_le_activiate_uplink_t *params);

/**
 * @brief                                This function init bt ull profile.
 */
bt_status_t bt_ull_le_init(void);

/**
 * @brief                                This function deinit bt ull profile.
 */
bt_status_t bt_ull_le_deinit(void);

#endif

