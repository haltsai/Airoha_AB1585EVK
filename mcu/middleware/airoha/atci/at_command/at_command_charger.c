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

#include "hal_feature_config.h"
#ifdef HAL_PMU_MODULE_ENABLED


#include "FreeRTOS.h"
#include "task.h"
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include "at_command.h"
#include "syslog.h"
#ifdef MTK_BATTERY_MANAGEMENT_ENABLE
#include "battery_management.h"
#include "battery_management_core.h"
#endif
#if (IC_CONFIG == ab155x)
#include "hal_charger.h"
#endif
log_create_module(atci_charger, PRINT_LEVEL_INFO);
#define MAX_CHAR_NUM 6
#include "hal_pmu.h"
int task_timer = 1000;
int count = 0;
static TaskHandle_t charger_task_handle = NULL;
int pmu_debug_index = 0;
#ifdef MTK_BATTERY_MANAGEMENT_ENABLE

#if defined(AIR_BTA_PMIC_HP) || defined(AIR_BTA_PMIC_G2_HP)
const char *charger_type[10] = { "", "SDP", "CDP", "DCP", "SS", "IPAD2_IPAD4", "IPHONE_5V_1A", "NON-STD", "DP/DM_Floating", "UNABLE_TO_IDENTIFY"};
const char *charger_state[8] = { "CHARGER OFF", "PRECC", "CC", "EXTENSION", "EOC", "THERMAL", "VBAT_OVP", "SAFETY TIMER"};
#endif
#if defined(AIR_BTA_PMIC_LP) || defined(AIR_BTA_PMIC_G2_LP)
const char *charger_type[10] = { "", "SDP", "CDP", "DCP", "SS", "IPAD2_IPAD4", "IPHONE_5V_1A", "NON-STD", "DP/DM_Floating", "UNABLE_TO_IDENTIFY"};
const char *charger_state[8] = { "CHARGER OFF", "TRICKLE", "CC1", "CC2", "CV_INT", "CV", "EOC", "RECHG"};
#endif
#endif
#ifdef MTK_FUEL_GAUGE
#include "battery_meter.h"
#endif
void pmu_dump_auxadc(void)
{
#if defined(AIR_BTA_PMIC_HP) || defined(AIR_BTA_PMIC_G2_HP)
    LOG_MSGID_I(common, "========================[PMU_AUXADC]=============================", 0);
    LOG_MSGID_I(common, "[PMU_AUXADC][CH0]PMU_AUX_PN_ZCV==%d", 1, pmu_auxadc_get_channel_value(PMU_AUX_PN_ZCV));
    LOG_MSGID_I(common, "[PMU_AUXADC][CH0]PMU_AUX_WK_ZCV==%d", 1, pmu_auxadc_get_channel_value(PMU_AUX_WK_ZCV));
    LOG_MSGID_I(common, "[PMU_AUXADC][CH0]PMU_AUX_BATSNS==%d", 1, pmu_auxadc_get_channel_value(PMU_AUX_BATSNS));
    LOG_MSGID_I(common, "[PMU_AUXADC][CH0]PMU_AUX_BAT_RECHARGER==%d", 1, pmu_auxadc_get_channel_value(PMU_AUX_BAT_RECHARGER));
    LOG_MSGID_I(common, "[PMU_AUXADC][CH1]PMU_AUX_VBUS_UART==%d", 1, pmu_auxadc_get_channel_value(PMU_AUX_VBUS_UART));
    LOG_MSGID_I(common, "[PMU_AUXADC][CH2]PMU_AUX_VBUS==%d", 1, pmu_auxadc_get_channel_value(PMU_AUX_VBUS));
    LOG_MSGID_I(common, "[PMU_AUXADC][CH3]PMU_AUX_CHR_THM==%d", 1, pmu_auxadc_get_channel_value(PMU_AUX_CHR_THM));
    LOG_MSGID_I(common, "[PMU_AUXADC][CH3]PMU_AUX_HW_JEITA==%d", 1, pmu_auxadc_get_channel_value(PMU_AUX_HW_JEITA));
    LOG_MSGID_I(common, "[PMU_AUXADC][CH4]PMU_AUX_PMIC_AP==%d", 1, pmu_auxadc_get_channel_value(PMU_AUX_PMIC_AP));
#endif
}

void pmu_dump_battery(void)
{
#ifdef MTK_BATTERY_MANAGEMENT_ENABLE
    int32_t charger_status;
    charger_status = battery_management_get_battery_property(BATTERY_PROPERTY_CHARGER_EXIST);
    if (charger_status) {
        LOG_MSGID_I(common, "[BM]Battery capacity = %d(%%) ", 1, (int)battery_management_get_battery_property(BATTERY_PROPERTY_CAPACITY));
        LOG_MSGID_I(common, "[BM]Charger status = %d ", 1, (int)charger_status);
        LOG_I(common, "[BM]Charger type = %s", charger_type[battery_management_get_battery_property(BATTERY_PROPERTY_CHARGER_TYPE)]);
        LOG_MSGID_I(common, "[BM]Battery temperature JEITA CH3 = %d(Celsius degrees)", 1, (int)battery_management_get_battery_property(BATTERY_PROPERTY_TEMPERATURE));
        LOG_MSGID_I(common, "[BM]Battery temperature PMIC CH4 = %d(Celsius degrees)", 1, (int)battery_management_get_battery_property(BATTERY_PROPERTY_PMIC_TEMPERATURE));
        LOG_MSGID_I(common, "[BM]Battery voltage = %d(mV)\n", 1, (int) battery_management_get_battery_property(BATTERY_PROPERTY_VOLTAGE));
        LOG_I(common, "[BM]Charger State = %s\n", charger_state[battery_management_get_battery_property(BATTERY_PROPERTY_CHARGER_STATE)]);
    } else {
        LOG_MSGID_I(common, "[BM]Battery capacity = %d(%%)", 1, (int) battery_management_get_battery_property(BATTERY_PROPERTY_CAPACITY));
        LOG_MSGID_I(common, "[BM]Charger status = %d(0:charge not exist; 1:charger exist)", 1, (int) charger_status);
        LOG_MSGID_I(common, "[BM]Battery temperature JEITA CH3 = %d(Celsius degrees)", 1, (int)battery_management_get_battery_property(BATTERY_PROPERTY_TEMPERATURE));
        LOG_MSGID_I(common, "[BM]Battery temperature PMIC CH4 = %d(Celsius degrees)", 1, (int)battery_management_get_battery_property(BATTERY_PROPERTY_PMIC_TEMPERATURE));
        LOG_MSGID_I(common, "[BM]Battery voltage = %d(mV)\n", 1, (int)battery_management_get_battery_property(BATTERY_PROPERTY_VOLTAGE));
    }
#endif
}

void pmu_charger_test(void)
{
#ifdef MTK_BATTERY_MANAGEMENT_ENABLE
    if (battery_management_get_battery_property(BATTERY_PROPERTY_CHARGER_STATE) == CHARGER_STATE_CHR_OFF) {
        LOG_MSGID_I(common, "Charger_disable", 0);
        pmu_enable_charger(0);
    } else {
        LOG_MSGID_I(common, "Charger_enable", 0);
        pmu_enable_charger(1);
    }
#endif
}

int ctoi(char s[])
{
    int i = 0;
    int n = 0;
    for (; (s[i] >= '0' && s[i] <= '9'); ++i) {
        n = 10 * n + (s[i] - '0');
    }
    return n;
}
int htoi(char s[])
{
    int i;
    int n = 0;
    if (s[0] == '0' && (s[1] == 'x' || s[1] == 'X')) {
        i = 2;
    } else {
        i = 0;
    }
    for (; (s[i] >= '0' && s[i] <= '9') || (s[i] >= 'a' && s[i] <= 'z') || (s[i] >= 'A' && s[i] <= 'Z'); ++i) {
        if (tolower((int)s[i]) > '9') {
            n = 16 * n + (10 + tolower((int)s[i]) - 'a');
        } else {
            n = 16 * n + (tolower((int)s[i]) - '0');
        }
    }
    return n;
}

static void charger_task(void *pvParameters)
{
#ifdef MTK_BATTERY_MANAGEMENT_ENABLE

    for (;;) {
        const TickType_t xDelay = task_timer / portTICK_PERIOD_MS;
        vTaskDelay(xDelay);
        switch (pmu_debug_index) {
            case 0:
                pmu_dump_battery();
                break;
            case 1:
                pmu_dump_auxadc();
                break;
            case 2:
                pmu_charger_test();
                break;
        }
    }
#endif
}

atci_status_t atci_cmd_hdlr_charger(atci_parse_cmd_param_t *parse_cmd)
{
    char *str  = NULL;
    atci_response_t response = { {0}};
    atci_response_heavy_data_t dumpall_response;
    uint8_t str_size = strlen(parse_cmd->string_ptr);
    dumpall_response.response_buf = pvPortMalloc(str_size + 1);
    if (dumpall_response.response_buf == NULL) {
        LOG_MSGID_I(common, "Command is null", 0);
        return ATCI_STATUS_OK;
    }
    str = (char *)dumpall_response.response_buf;
    switch (parse_cmd->mode) {
        case ATCI_CMD_MODE_EXECUTION: /* AT+ECHAR=SQC... */
            if (strncmp(parse_cmd->string_ptr, "AT+ECHAR=SQC,", 12) == 0) {
                char *mid_pos = NULL;
                int input_domain = 0;
                int input_value = 0;
                char *end_pos = NULL;
                mid_pos = strchr(parse_cmd->string_ptr, ',');
                mid_pos++;
                end_pos = strchr(mid_pos, ',');
                if ((strlen(mid_pos) - strlen(end_pos)) < str_size) {
                    memcpy(str, mid_pos, strlen(mid_pos) - strlen(end_pos));
                    input_domain = htoi(mid_pos);
                    end_pos++;
                    input_value = htoi(end_pos);
                    task_timer = ctoi(end_pos);
                    mid_pos = NULL;
                    end_pos = NULL;
                    LOG_MSGID_I(common, "input_value1:%d, input_value2:%d\r\n", 2, input_domain, input_value);
                    switch (input_domain) {
                        case 0:
                            vTaskDelete(charger_task_handle);
                            charger_task_handle = NULL;
                            count = 0;
                            break;

                        case 1:
                            pmu_debug_index = 0;
                            if (charger_task_handle == NULL) {
                                xTaskCreate(charger_task, "charger_task", 512, (void *) &input_value, tskIDLE_PRIORITY, &charger_task_handle);
                            }
                            break;

                        case 2:
                            vTaskSuspend(charger_task_handle);
                            break;

                        case 3:
                            vTaskResume(charger_task_handle);
                            break;
                        case 4:
                            pmu_debug_index = 1;
                            if (charger_task_handle == NULL) {
                                xTaskCreate(charger_task, "charger_task", 512, (void *) &input_value, tskIDLE_PRIORITY, &charger_task_handle);
                            }
                            break;
                        case 5:
                            pmu_debug_index = 2;
                            if (charger_task_handle == NULL) {
                                xTaskCreate(charger_task, "charger_task", 512, (void *) &input_value, tskIDLE_PRIORITY, &charger_task_handle);
                            }
                            break;
                    }
                } else {
                    LOG_MSGID_I(common, "command lengh error", 0);
                }
            } else if (strncmp(parse_cmd->string_ptr, "AT+ECHAR=ICL,", 12) == 0) {
                int input_addr = 0;
                char *end_pos = NULL;
                end_pos = strchr(parse_cmd->string_ptr, ',');
                end_pos++;
                input_addr = htoi(end_pos);
                end_pos = NULL;
                LOG_MSGID_I(common, "Input leve : %d\r\n", 1, input_addr);
#ifdef AIR_BTA_PMIC_HP
                pmu_set_charger_current_limit(input_addr);
#endif
            }
#ifdef MTK_FUEL_GAUGE
            else if (strncmp(parse_cmd->string_ptr, "AT+ECHAR=FG_DEPUTY_TEMP,", 23) == 0) {
                            char *mid_pos = NULL;
                            int input_domain = 0;
                            int input_value = 0;
                            char *end_pos = NULL;
                            mid_pos = strchr(parse_cmd->string_ptr, ',');
                            mid_pos++;
                            end_pos = strchr(mid_pos, ',');
                            if ((strlen(mid_pos) - strlen(end_pos)) < str_size) {
                                memcpy(str, mid_pos, strlen(mid_pos) - strlen(end_pos));
                                input_domain = ctoi(mid_pos);
                                end_pos++;
                                input_value = ctoi(end_pos);
                                mid_pos = NULL;
                                end_pos = NULL;
                                LOG_MSGID_I(common, "input_value1:%d, input_value2:%d\r\n", 2, input_domain, input_value);
#ifdef FUEL_GAUGE_DEPUTY_TABLE_ENABLE
                                battery_set_deputy_temp(input_domain,input_value);
#endif
                            }
            }
            else if (strncmp(parse_cmd->string_ptr, "AT+ECHAR=FG_DUMP,", 12) == 0) {
                            int input_addr = 0;
                            char *end_pos = NULL;
                            end_pos = strchr(parse_cmd->string_ptr, ',');
                            end_pos++;
                            input_addr = htoi(end_pos);
                            end_pos = NULL;
                            LOG_MSGID_I(common, "AT+ECHAR=FG_DUMP\r\n", 1, input_addr);
                            switch(input_addr){
                            case 0:
                                battery_dump_zcv_table();
                                break;
                            }
            }
#endif
            else if (strncmp(parse_cmd->string_ptr, "AT+ECHAR=DEBUG,", 12) == 0) {
                int input_addr = 0;
                char *end_pos = NULL;
                end_pos = strchr(parse_cmd->string_ptr, ',');
                end_pos++;
                input_addr = htoi(end_pos);
                end_pos = NULL;
                LOG_MSGID_I(common, "Input leve : %d\r\n", 1, input_addr);
            }
#ifdef MTK_BATTERY_MANAGEMENT_ENABLE
            else if (strncmp(parse_cmd->string_ptr, "AT+ECHAR=CHARGER,", 12) == 0) {
                char *end_pos = NULL;
                int input_value = 0;
                end_pos = strchr(parse_cmd->string_ptr, ',');
                end_pos++;
                input_value = htoi(end_pos);
                end_pos = NULL;
                log_hal_msgid_info("Setting : %d\r\n", 1, (int) input_value);

                battery_set_enable_charger(input_value);
                if (input_value) {
                    snprintf((char *) response.response_buf, ATCI_UART_TX_FIFO_BUFFER_SIZE, "Charger Enable [%d][%s]\r\n", input_value,
                             charger_state[battery_management_get_battery_property(BATTERY_PROPERTY_CHARGER_STATE)]);
                } else {
                    snprintf((char *) response.response_buf, ATCI_UART_TX_FIFO_BUFFER_SIZE, "Charger Disable [%d][%s]\r\n", input_value,
                             charger_state[battery_management_get_battery_property(BATTERY_PROPERTY_CHARGER_STATE)]);
                }
            } else if (strncmp(parse_cmd->string_ptr, "AT+ECHAR=STA", 12) == 0) {
                snprintf((char *) response.response_buf, ATCI_UART_TX_FIFO_BUFFER_SIZE, "STA:%s\r\n", charger_state[battery_management_get_battery_property(BATTERY_PROPERTY_CHARGER_STATE)]);
            } else if (strncmp(parse_cmd->string_ptr, "AT+ECHAR=SOC", 12) == 0) {
                snprintf((char *) response.response_buf, ATCI_UART_TX_FIFO_BUFFER_SIZE, "SOC:%d\r\n", (int) battery_management_get_battery_property(BATTERY_PROPERTY_CAPACITY));
            } else if (strncmp(parse_cmd->string_ptr, "AT+ECHAR=VBAT", 12) == 0) {
                snprintf((char *) response.response_buf, ATCI_UART_TX_FIFO_BUFFER_SIZE, "VBAT:%d\r\n", (int) battery_management_get_battery_property(BATTERY_PROPERTY_VOLTAGE));
            } else if (strncmp(parse_cmd->string_ptr, "AT+ECHAR=CHG_OPTION,", 12) == 0) {
                int input_addr = 0;
                char *end_pos = NULL;
                end_pos = strchr(parse_cmd->string_ptr, ',');
                end_pos++;
                input_addr = htoi(end_pos);
                end_pos = NULL;
                LOG_MSGID_I(common, "Input leve : %d\r\n", 1, input_addr);
#ifdef AIR_BTA_PMIC_HP
                battery_switch_charger_option(input_addr);
#endif
            }
#endif
            else {
                response.response_flag = 0 | ATCI_RESPONSE_FLAG_APPEND_ERROR;
                atci_send_response(&response);
                break;
            }
            response.response_flag = 0 | ATCI_RESPONSE_FLAG_APPEND_OK;
            response.response_len = strlen((const char *) response.response_buf);
            atci_send_response(&response);
            break;
    }
    vPortFree(dumpall_response.response_buf);
    return ATCI_STATUS_OK;
}


#endif /* HAL_CHARGER_MODULE_ENABLED */

