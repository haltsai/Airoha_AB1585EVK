/* Copyright Statement:
 *
 * (C) 2005-2016  MediaTek Inc. All rights reserved.
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. ("MediaTek") and/or its licensors.
 * Without the prior written permission of MediaTek and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 * You may only use, reproduce, modify, or distribute (as applicable) MediaTek Software
 * if you have agreed to and been bound by the applicable license agreement with
 * MediaTek ("License Agreement") and been granted explicit permission to do so within
 * the License Agreement ("Permitted User").  If you are not a Permitted User,
 * please cease any access or use of MediaTek Software immediately.
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT MEDIATEK SOFTWARE RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES
 * ARE PROVIDED TO RECEIVER ON AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL
 * WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 */

#ifdef MTK_BT_AT_COMMAND_ENABLE
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "at_command.h"
#include "hal_gpt.h"
#include "hal_log.h"
#include "hal_irrx_internal.h"
#include "nvkey_id_list.h"
#include "hal_clock_internal.h"

#define AT_EFUSE_CAPID0_MASK            0x000001FF

#if defined(MTK_NVDM_ENABLE)
#include "nvkey.h"
#define HAL_NVDM_XO_26M_TABLE_STRING "2045"


typedef struct {
    uint8_t trim_spec;
    uint16_t cap_value;
    uint8_t reserved;
} __attribute__((packed)) xo_info_t;
#endif /* MTK_CAPID_IN_NVDM_AT_COMMAND_ENABLE && MTK_NVDM_ENABLE */


extern bool bt_enable_relay_mode(hal_uart_port_t port);

atci_status_t atci_cmd_hdlr_capid(atci_parse_cmd_param_t *parse_cmd)
{
    atci_response_t resonse = {{0}};

    char *param1 = NULL;
    char *param2 = NULL;
    uint8_t things = 0;
    uint8_t port = 0;
    int32_t result = 0;
    uint32_t debug_capid = 0;
    //uint32_t read_efuse_capid=0;

    log_hal_msgid_info("atci_cmd_hdlr_capid \r\n", 0);

    resonse.response_flag = 0; /*    Command Execute Finish.  */

    switch (parse_cmd->mode) {
        case ATCI_CMD_MODE_TESTING:
            strncpy((char *)resonse.response_buf, "+CAPID=99\r\nOK\r\n", strlen("+CAPID=99\r\nOK\r\n"));
            atci_send_response(&resonse);
            break;

        case ATCI_CMD_MODE_EXECUTION:
            resonse.response_len = 0;
            //atci_send_response(&resonse);
            if (strncmp(parse_cmd->string_ptr, "AT+CAPID=", strlen("AT+CAPID=")) == 0) {
                param1 = (char *)strtok(parse_cmd->string_ptr, "=");
                param1 = (char *)strtok(NULL, ",");
                things = atoi(param1);
                log_hal_msgid_info("things=%d\r\n", 1, things);

                if (0 == things) { //AT+CAPID=0,2   enter relay mode on uart2
                    param2 = (char *)strtok(NULL, ",");
                    port = atoi(param2);
                    log_hal_msgid_info("things=%d,port=%d\r\n", 2, things, port);
                    result = bt_enable_relay_mode((hal_uart_port_t)port);
                    if (true == result) {
                        resonse.response_flag = ATCI_RESPONSE_FLAG_APPEND_OK;
                    } else {
                        resonse.response_flag = ATCI_RESPONSE_FLAG_APPEND_ERROR;
                    }
                } else if (1 == things) { //AT+CAPID=1,420   modify capid=420 dynamic.
                    /* set CAPID register */
                    param2 = (char *)strtok(NULL, ",");
                    debug_capid = atoi(param2);
#if defined(AIR_BTA_IC_PREMIUM_G3)
                    DCXO_PCON_REG.DCXO_CAPID_EFUSE = debug_capid;
                    DCXO_PCON_REG.DCXO_CAPID_EFUSE_SEL = 0x1;
#else
                    set_capid_rg(debug_capid);   /* set CAPID register */
#endif /* chip_define */
                    hal_gpt_delay_ms(1); /* waiting for signal stable */
                    log_hal_msgid_info("Set %u to CAPID register.\r\n", 1, (int)debug_capid);

                    resonse.response_flag = ATCI_RESPONSE_FLAG_APPEND_OK;
                } else if (2 == things) { //AT+CAPID=2,404,2  save capid to efuse CAPID2
                    /* write capid to nvdm */
                    /* also write to register */
                    param2 = (char *)strtok(NULL, ",");
                    debug_capid = atoi(param2) & AT_EFUSE_CAPID0_MASK;
#if defined(AIR_BTA_IC_PREMIUM_G3)
                    uint8_t nvkey[4] = {0};
                    uint32_t size = sizeof(xo_info_t);
                    if (((nvkey_status_t)nvkey_read_data(NVID_CAL_XO_26M_CRTSTAL_TRIM, (uint8_t *)nvkey, &size)) == NVKEY_STATUS_OK) {
                        (((xo_info_t *)nvkey) -> cap_value) = debug_capid;
                        result = nvdm_write_data_item("AB15", HAL_NVDM_XO_26M_TABLE_STRING, NVDM_DATA_ITEM_TYPE_RAW_DATA, (uint8_t *)nvkey, size);
                        if (result == NVDM_STATUS_OK) {
                            DCXO_PCON_REG.DCXO_CAPID_EFUSE = debug_capid;
                            DCXO_PCON_REG.DCXO_CAPID_EFUSE_SEL = 0x1;
                        }
                        LOG_MSGID_I(common, "CAPID item in NVDM(nvkey) is available(0x%x).\r\n", 1, (((xo_info_t *)nvkey) -> cap_value));
                    } else {
                        LOG_MSGID_E(common, "CAPID item in  NVDM(nvkey) is empty, use default value.\r\n", 0);
                        return ATCI_STATUS_OK;
                    }
#else
                    result = set_capid_nvdm(debug_capid); /* Write CAPID to NVDM */
                    if (result == NVDM_STATUS_OK) {
                        set_capid_rg(debug_capid);   /* set CAPID register */
                    }
#endif

                    resonse.response_flag = ATCI_RESPONSE_FLAG_APPEND_OK;

                } else if (3 == things) { //AT+CAPID=3  read efuse capid
                    /* Read CAPID NVKey, but don't set to register */
                    /* result : the read capid value from NVKey */
#if defined(AIR_BTA_IC_PREMIUM_G3)
                    /* Read NVKey, but don't set to register */
                    uint32_t size = 0;
                    uint8_t nvkey[4] = {0};
                    size = sizeof(xo_info_t);
                    if (nvkey_read_data(NVID_CAL_XO_26M_CRTSTAL_TRIM, nvkey, &size) == NVKEY_STATUS_OK) {
                        result = (((xo_info_t *)nvkey) -> cap_value) & AT_EFUSE_CAPID0_MASK;
                        LOG_MSGID_I(common, "CAPID item in NVDM(nvkey) is available(0x%x).\r\n", 1, result);
                    } else {
                        LOG_MSGID_E(common, "CAPID item in  NVDM(nvkey) is empty, use default value.\r\n", 0);
                    }
#else
                    result = get_capid_nvdm();
#endif /* chip version */
                    resonse.response_flag = ATCI_RESPONSE_FLAG_APPEND_OK;
                } else if (4 == things) { //AT+CAPID=4  read current capid
                    /* Read CAPID value, and save in result variable */
#if defined(AIR_BTA_IC_PREMIUM_G3)
                    result = DCXO_PCON_REG.DCXO_CAPID_EFUSE;
#else
                    result = get_capid_rg();
#endif
                    log_hal_msgid_info("CAPID register = %d\r\n", 1, (int)result);
                    resonse.response_flag = ATCI_RESPONSE_FLAG_APPEND_OK;
                } else if (5 == things) { //AT+CAPID=5  read current capid
                    /* legacy capid at command (To be removed) */
#if defined(MTK_CAPID_IN_NVDM_AT_COMMAND_ENABLE) && defined(MTK_NVDM_ENABLE)
                    log_hal_msgid_info("Support at_command to calibration capid\r\n", 0);
#if defined(AIR_BTA_IC_PREMIUM_G3)
                    snprintf((char *)resonse.response_buf, sizeof(resonse.response_buf), "XO CAPID = 0x%x\r\n", DCXO_PCON_REG.DCXO_CAPID_EFUSE);
#else
                    snprintf((char *)resonse.response_buf, sizeof(resonse.response_buf), "XO CAPID = 0x%x\r\n", (unsigned int)get_capid_rg());
#endif
                    resonse.response_flag = ATCI_RESPONSE_FLAG_APPEND_OK;

                    resonse.response_len = strlen((char *)resonse.response_buf);
                    atci_send_response(&resonse);
                    return ATCI_STATUS_OK;
#else
                    log_hal_msgid_info("Not support at_command to calibration capid\r\n", 0);
#endif /* MTK_CAPID_IN_NVDM_AT_COMMAND_ENABLE && MTK_NVDM_ENABLE */
                }

            }

            snprintf((char *)resonse.response_buf, sizeof(resonse.response_buf), "+CAPID:%d,%ld\r\n", things, result);
            resonse.response_len = strlen((char *)resonse.response_buf);
            atci_send_response(&resonse);


            break;
        default :
            /* others are invalid command format */
            strncpy((char *)resonse.response_buf, "ERROR\r\n", (strlen("ERROR\r\n") + 1));
            resonse.response_len = strlen((char *)resonse.response_buf);
            atci_send_response(&resonse);
            break;
    }
    return ATCI_STATUS_OK;
}

#endif
/*---ATA Command handler Function End---*/
