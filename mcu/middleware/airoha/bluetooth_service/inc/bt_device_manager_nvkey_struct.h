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

#ifndef __BT_DEVICE_MANAGER_NVKEY_STRUCT_H__
#define __BT_DEVICE_MANAGER_NVKEY_STRUCT_H__

#include "bt_device_manager_internal.h"
#include "bt_device_manager_le.h"

#ifndef PACKED
#define PACKED  __attribute__((packed))
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    bt_gap_link_key_type_t   key_type;
    bt_key_t                 key;
} PACKED bt_device_manager_db_remote_paired_info_internal_t;

/* NvkeyDefine NVID_BT_HOST_REMOTE_INFO_01 ~ NVID_BT_HOST_REMOTE_INFO_MAX */
typedef struct {
    bt_bd_addr_t    address;
    bt_device_manager_remote_info_mask_t info_valid_flag;
    bt_device_manager_db_remote_paired_info_internal_t paired_info;
    bt_device_manager_db_remote_profile_info_t profile_info;
    uint32_t        iot_id;
    uint32_t        supported_profiles;
    uint8_t         reserved[60];
} PACKED bt_device_manager_db_remote_info_t;

/* NvkeyDefine NVID_BT_HOST_DM_AWS_MCE_CONFIG */
typedef struct {
    uint8_t         aws_role;
    uint16_t        supported_feature;
    bt_bd_addr_t    peer_addr;
    uint8_t         slave_retry_count;
    bt_bd_addr_t    local_aws_addr;
    uint8_t         ls_enable;
    uint8_t         aws_key[16];
    uint8_t         speaker_fixed_addr[6];
    uint8_t         aws_mode;
    uint8_t         reserved[8];
} PACKED bt_device_manager_db_aws_local_info_t;

/* NvkeyDefine NVDM_BT_DM_ITEM_BONDED_INFO_PREFIX_01 ~  NVDM_BT_DM_ITEM_BONDED_INFO_PREFIX_08*/

typedef struct {
    bt_device_manager_le_bonded_info_t  bond_info;
    uint16_t                            flag;
    uint8_t                             reserve[19];
} PACKED bt_device_manager_le_db_bonded_info_t;


#ifdef __cplusplus
}
#endif

#endif /*__BT_DEVICE_MANAGER_NVKEY_STRUCT_H__*/


