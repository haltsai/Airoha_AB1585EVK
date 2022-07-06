/* Copyright Statement:
 *
 * (C) 2018  Airoha Technology Corp. All rights reserved.
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

#include "FreeRTOS.h"
#include "timers.h"
#include "mux.h"
#include "smartcharger.h"
#include "hal_gpio.h"
#include "hal_uart_internal.h"
#include "hal_eint.h"
#include "hal_gpio.h"
#include "hal_log.h"
#include "chargersmartcase.h"
#include "battery_management.h"
#include "hal.h"
#include "hal_dvfs_internal.h"
#include "hal_sleep_manager_platform.h"
#include "battery_management_core.h"
#include "battery_management.h"
#include "memory_attribute.h"
#include "bt_sink_srv_ami.h"
#include "serial_port_assignment.h"
#include "serial_port.h"
#include "race_xport.h"
#include "nvkey.h"
#include "nvkey_id_list.h"
#include "hal_resource_assignment.h"

#if defined(AIR_BTA_IC_PREMIUM_G3)
#if defined (AIR_BTA_PMIC_HP)
#include "hal_pmu_internal_hp.h"
#include "hal_pmu_charger_hp.h"
#elif defined(AIR_BTA_PMIC_LP)
#include "hal_pmu_internal_lp.h"
#include "hal_pmu_charger_lp.h"
#endif
#endif

#if defined(AIR_BTA_IC_PREMIUM_G2)
#if defined (AIR_BTA_PMIC_G2_HP)
#elif defined(AIR_BTA_PMIC_G2_LP)
#include "hal_pmu_charger_2565.h"
#include "hal_pmu_internal_2565.h"
#endif
#endif
#include "smchg_1wire_config.h"
#include "smchg_1wire.h"
#include "ept_gpio_var.h"


#define log_hal_msgid_info(_message,arg_cnt,...)          LOG_MSGID_I(hal,_message,arg_cnt,##__VA_ARGS__)



#if defined(AIR_1WIRE_ENABLE)

/* create mux debug module */
log_create_module(SM_CHG, PRINT_LEVEL_INFO);

#if defined(AIR_BTA_IC_PREMIUM_G3)
#if defined (AIR_BTA_PMIC_HP)
#define HP_1WIRE_G3
#elif defined(AIR_BTA_PMIC_LP)
#define LP_1WIRE_G3
#endif
#elif defined(AIR_BTA_IC_PREMIUM_G2)
#if defined (AIR_BTA_PMIC_G2_HP)
#define HP_1WIRE_G2
#elif defined(AIR_BTA_PMIC_G2_LP)
#define LP_1WIRE_G2
#endif
#else
#endif


#define RACE_CH                 0
#define RACE_TYPE               1
#define LENGTH_L                2
#define LENGTH_H                3
#define CMD_ID                  6
#define EARBUD_ID               7
#define DATA                    8
#define BAT_LV                  8
#define BAUDRATE                8
#define KEY_ID                  8

#define CASE_LID_OPEN           0x2
#define CASE_LID_CLOSE_DONE     0x3
#define CASE_CHARGER_OFF        0x4
#define CASE_CHARGER_KEY        0x5
#define CASE_BATTER_LEVEL       0x6
#define CASE_LID_CLOSE          0x8
#define CASE_LOG_ENABLE         0xE
#define CASE_RACE_ENABLE        0xF

#define SMCHG_MUX_RX_DATA_SIZE  32
#define SMCHG_MUX_TX_BUFF_SIZE  1024
#define SMCHG_MUX_RX_BUFF_SIZE  1024
#define SMCHG_MUX_RX_HANDLE_TIMER_MS (1)
#define SMCHG_MUX_TX_QUERY_MAX_TIME (1500)

const uint8_t smchg_app_table[] = {
    0,
    0,
    SMCHG_LID_OPEN,
    SMCHG_LID_CLOSE_DONE,
    SMCHG_CHG_OFF,
    SMCHG_CHG_KEY,
    0,
    0,
    SMCHG_LID_CLOSE
};

static uint8_t raceEvt[] = {
    0x05,      //race header
    0x5B,      //race type
    0x05,      //lengthL
    0x00,      //lengthH
    0x00,      //cmd type
    0x20,      //cmd tpye
    0x00,      //event
    0x00,      //R or L
    0x00,      //parameter
};

typedef struct {
    mux_buffer_t *mux_buffer;
    uint32_t offset;
    uint32_t idx;
    uint32_t counter;
    uint32_t left_data_len;
    uint32_t total_length;
} one_wire_mux_buffer_stru;

typedef struct {
    mux_buffer_t *mux_buffer;
    uint32_t offset;
    uint32_t idx;
    uint32_t counter;
    uint32_t left_data_len;
    uint32_t total_length;
} smchg_mux_buffer_t;

one_wire_mux_buffer_stru mux_buffer;
mux_handle_t smchg_mux_handle;
smchg_1wire_gpio_t smchg_1wire_gpio_cfg;
TimerHandle_t pSmartChargerOutTimer;
TimerHandle_t pSmartChargerRxTimer;

hal_uart_port_t smchg_uart_port = SERIAL_PORT_DEV_UART_1;

uint8_t pre_cmd = 0;
uint32_t pre_data = 0;
uint16_t pre_data_len = 0;
uint8_t last_cmd = 0;
uint8_t boot_Flag = 0;
uint8_t lock_sleep_flag = 0;
uint8_t chg_mode_flag = 0;
uint8_t race_mode_flag = 0;
bool    is_rx_timer_start = FALSE;
uint32_t g_rx_data_len = 0;
void    *g_rx_user_data = NULL;

static uint32_t g_com_mode_chk_cnt = 0;



static volatile smchg_1wire_mode_t *p_1wire_mode = (volatile smchg_1wire_mode_t *)HW_SYSRAM_PRIVATE_MEMORY_1WIRE_START;

static void smchg_1wire_rx_handle(uint32_t user_data_len, void *user_data);


/* sleep lock control */
void smchg_1wire_lock_sleep(void)
{
    //log_hal_msgid_info("1wire, lock_sleep, flag[%d]", 1, lock_sleep_flag);

    if (!lock_sleep_flag) {
        hal_sleep_manager_lock_sleep(SLEEP_LOCK_CHARGER_CASE);
        lock_sleep_flag = 1;
    }
}

void smchg_1wire_unlock_sleep(void)
{
    //log_hal_msgid_info("1wire, unlock_sleep, flag[%d]", 1, lock_sleep_flag);

    if (lock_sleep_flag) {
        hal_sleep_manager_unlock_sleep(SLEEP_LOCK_CHARGER_CASE);
        lock_sleep_flag = 0;
    }
}

smchg_1wire_mode_t smchg_1wire_get_mode_status(void)
{
    return *p_1wire_mode;
}

smchg_status_t smchg_1wire_set_mode_status(smchg_1wire_mode_t mode)
{
    if (mode >= SMCHG_1WIRE_MAX) {
        return SMCHG_STATUS_INVALID_PARAMETER;
    }

    *p_1wire_mode = mode;

    return SMCHG_STATUS_OK;
}


void smchg_1wire_app_cmd(uint8_t cmd)
{
    switch (cmd) {
        case SMCHG_LID_OPEN:
            log_hal_msgid_info("1wire, app_cmd[lid_open]", 0);
            break;
        case SMCHG_LID_CLOSE:
            log_hal_msgid_info("1wire, app_cmd[lid_close]", 0);
            break;
        case SMCHG_LID_CLOSE_DONE:
            log_hal_msgid_info("1wire, app_cmd[lid_close_done]", 0);
            break;
        case SMCHG_CHG_OFF:
            log_hal_msgid_info("1wire, app_cmd[chg_off]", 0);
            break;
        case SMCHG_CHG_KEY:
            log_hal_msgid_info("1wire, app_cmd[chg_key]", 0);
            break;
        case SMCHG_USER_DATA:
            log_hal_msgid_info("1wire, app_cmd[user_data]", 0);
            break;
        case SMCHG_CHG_IN:
            log_hal_msgid_info("1wire, app_cmd[chg_in]", 0);
            break;
        case SMCHG_CHG_OUT:
            log_hal_msgid_info("1wire, app_cmd[chg_out]", 0);
            break;
        case SMCHG_CHG_COMPL:
            log_hal_msgid_info("1wire, app_cmd[chg_compl]", 0);
            break;
        case SMCHG_CHG_RECHG:
            log_hal_msgid_info("1wire, app_cmd[chg_rechg]", 0);
            break;

        default:
            log_hal_msgid_error("1wire, app_cmd[unknow]", 0);
            break;
    }
}


bool smchg_1wire_vbat_over_thrd(uint32_t thrd)
{
#if defined(AIR_BTA_PMIC_LP) || defined(AIR_BTA_PMIC_G2_LP)
    uint32_t vbat = battery_management_get_battery_property(BATTERY_PROPERTY_VOLTAGE);
    log_hal_msgid_info("1wire, vbat[%d]", 1, vbat);
    if (vbat > thrd) {
        return TRUE;
    } else {
        return FALSE;
    }
#elif defined(AIR_BTA_PMIC_HP)
    return TRUE;
#else
    #error "please check if it needs to check the battery state or not"
#endif
}

bool smchg_1wire_vchg_less_than_thrd(uint32_t thrd)
{
    uint32_t data;

    data = pmu_1wire_get_vbus_uart_volt();
    //log_hal_msgid_info("1wire, vbus_uart[%d], thrd[%d]", 2, data, thrd);

    if (data < thrd) {
        return TRUE;
    } else {
        return FALSE;
    }
}

int32_t smchg_1wire_chg_exist(void)
{
    return battery_management_get_battery_property(BATTERY_PROPERTY_CHARGER_EXIST);
}

static void smchg_1wire_com_mode_exist(uint8_t *ret)
{
    *ret = 1;
    mux_status_t  mux_status = MUX_STATUS_ERROR_BUSY;
    bool is_com_mode = TRUE;
    mux_status = mux_control((mux_port_t)smchg_uart_port, MUX_CMD_GET_TRX_PORT_IDLE, NULL);
    if (mux_status != MUX_STATUS_OK) {
        g_com_mode_chk_cnt = 0;
        log_hal_msgid_warning("1wire, uart_busy_1, uart_status[%d]", mux_status);
        return ;
    }


    if (smchg_1wire_vchg_less_than_thrd(smchg_cfg.out_of_case_vbus_uart_thrd)) {
        is_com_mode = FALSE;
    } else {
        g_com_mode_chk_cnt = 0;
    }



    mux_status = mux_control((mux_port_t)smchg_uart_port, MUX_CMD_GET_TRX_PORT_IDLE, NULL);
    if (mux_status != MUX_STATUS_OK) {
        g_com_mode_chk_cnt = 0;
        log_hal_msgid_warning("1wire, uart_busy_2, uart_status[%d]", mux_status);
        return ;
    }


    if (!is_com_mode) {
        g_com_mode_chk_cnt++;
        log_hal_msgid_warning("1wire, com_mod_chk, cnt[%d]", 1, g_com_mode_chk_cnt);
        if (g_com_mode_chk_cnt > 2) {
            g_com_mode_chk_cnt = 0;
            *ret = 0;
        }
    }

    return;
}

static void smchg_1wire_init_to_out_of_case(void)
{
    bool is_vbat_norm = smchg_1wire_vbat_over_thrd(smchg_cfg.com_mode_vbat_thrd);

    log_hal_msgid_info("1wire, init_to_out_of_case, vbat_state[%d]", 1, is_vbat_norm);
    if (is_vbat_norm) {
        pmu_1wire_cfg(PMU_1WIRE_INIT_TO_OUT_OF_CASE_NORM_BAT);
    } else {
        pmu_1wire_cfg(PMU_1WIRE_INIT_TO_OUT_OF_CASE_LOW_BAT);
    }
}

static void smchg_1wire_init_to_chg_mode(void)
{
    pmu_1wire_cfg(PMU_1WIRE_INIT_TO_CHG_IN);
}


void smchg_1wire_init_to_log_mode(void)
{
    bool is_vbat_norm = smchg_1wire_vbat_over_thrd(smchg_cfg.com_mode_vbat_thrd);

    log_hal_msgid_info("1wire, intr_to_log_mode, vbat_state[%d]", 1, is_vbat_norm);
    if (is_vbat_norm) {
        pmu_1wire_cfg(PMU_1WIRE_CHG_IN_TO_COM_NORM_BAT);
    } else {
        pmu_1wire_cfg(PMU_1WIRE_CHG_IN_TO_COM_LOW_BAT);
    }
}

static void smchg_1wire_intr_to_chg_mode(void)
{
    smchg_1wire_mode_t smchg_1wire_status = smchg_1wire_get_mode_status();
    bool is_vbat_norm = smchg_1wire_vbat_over_thrd(smchg_cfg.com_mode_vbat_thrd);

    log_hal_msgid_info("1wire, intr_to_chg_mode, mode[%d], vbat_state[%d]", 2, smchg_1wire_status, is_vbat_norm);
    switch (smchg_1wire_status)
    {
        case SMCHG_1WIRE_LOG:
        case SMCHG_1WIRE_COM:
        case SMCHG_1WIRE_RACE:
        case SMCHG_1WIRE_ATCI: {
            if (is_vbat_norm) {
                pmu_1wire_cfg(PMU_1WIRE_COM_TO_CHG_IN_NORM_BAT);
            } else {
                pmu_1wire_cfg(PMU_1WIRE_COM_TO_CHG_IN_LOW_BAT);
            }
        }
        break;
        case SMCHG_1WIRE_OUT:
        case SMCHG_1WIRE_CHG: {
            if (is_vbat_norm) {
                pmu_1wire_cfg(PMU_1WIRE_OUT_OF_CASE_TO_CHG_IN_NORM_BAT);
            } else {
                pmu_1wire_cfg(PMU_1WIRE_OUT_OF_CASE_TO_CHG_IN_LOW_BAT);
            }
        }
        break;
        default:
            if (is_vbat_norm) {
                pmu_1wire_cfg(PMU_1WIRE_OUT_OF_CASE_TO_CHG_IN_NORM_BAT);
            } else {
                pmu_1wire_cfg(PMU_1WIRE_OUT_OF_CASE_TO_CHG_IN_LOW_BAT);
            }
        break;
    }
}



static void smchg_1wire_intr_to_com_mode(void)
{
    bool is_vbat_norm = smchg_1wire_vbat_over_thrd(smchg_cfg.com_mode_vbat_thrd);

    log_hal_msgid_info("1wire, intr_to_com_mode, vbat_state[%d]", 1, is_vbat_norm);
    if (is_vbat_norm) {
        pmu_1wire_cfg(PMU_1WIRE_CHG_IN_TO_COM_NORM_BAT);
    } else {
        pmu_1wire_cfg(PMU_1WIRE_CHG_IN_TO_COM_LOW_BAT);
    }
}

static smchg_status_t smchg_1wire_com_mode_to_out_of_case(void)
{
    smchg_status_t status = SMCHG_STATUS_OK;
    uint8_t ret;

    smchg_1wire_com_mode_exist(&ret);
    if (ret) {
        //log_hal_msgid_info("1wire, timer_to_out_of_case, keep com mode", 0);
        status = SMCHG_STATUS_ERROR;
    } else {
        pmu_1wire_cfg(PMU_1WIRE_COM_TO_OUT_OF_CASE);
    }
    return status;
}



static void smchg_1wire_com_mode_to_pwr_save_mode(void)
{
    pmu_1wire_cfg(PMU_1WIRE_COM_TO_PWR_SAVE);
}



void smchg_1wire_com_mode_tx_done(void)
{
    uint32_t time_s = 0, time_e = 0, time_dur = 0;
    mux_status_t  mux_status = MUX_STATUS_ERROR_BUSY;
    hal_gpt_status_t gpt_status = HAL_GPT_STATUS_ERROR;

    hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M, &time_s);

    do {
        mux_status = mux_control((mux_port_t)smchg_uart_port, MUX_CMD_GET_TX_PORT_IDLE, NULL);
        //log_hal_msgid_warning("1wire, com_mode_to_race_mode, mux_status[%d]", 1, mux_status);
        gpt_status = hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M, &time_e);
        if (gpt_status!= HAL_GPT_STATUS_OK) {
            log_hal_msgid_error("1wire, com_mode_tx_done, get time_e gpt error, status[%d]", 1, gpt_status);
        }

        gpt_status = hal_gpt_get_duration_count(time_s, time_e, &time_dur);
        if (gpt_status != HAL_GPT_STATUS_OK) {
            log_hal_msgid_error("1wire, com_mode_tx_done, get gpt dur error, status[%d]", 1, gpt_status);
        }
        /*if (time_dur > SMCHG_MUX_TX_QUERY_MAX_TIME) {
            break;
        }*/
    } while (mux_status != MUX_STATUS_OK);

    log_hal_msgid_warning("1wire, com_mode_tx_done, time[%dus]", 1, time_dur);
}




/*     1wire mode     */
static void smchg_1wire_race_mode(hal_uart_baudrate_t baudrate)
{
    mux_ctrl_para_t port_config_parm;

    log_hal_msgid_info("1wire, race_mode, baudrate[%d]", 1, smchg_1wire_baudrate_report(baudrate));

    if (xTimerChangePeriodFromISR(pSmartChargerOutTimer, pdMS_TO_TICKS(smchg_cfg.race_mode_chk_timer), 0) == pdFAIL) {
        log_hal_msgid_error("1wire, race_mode, change race_mode_timer fail", 0);
    }

    smchg_1wire_set_mode_status(SMCHG_1WIRE_RACE);

    port_config_parm.mux_set_config_uart_param.baudrate = baudrate;
    mux_control(smchg_mux_handle, MUX_CMD_CHANGE_UART_PARAM, &port_config_parm);
    port_config_parm.mux_set_config_uart_param.int_enable = TRUE;
    mux_control(smchg_mux_handle, MUX_CMD_CHANGE_UART_TX_INT, &port_config_parm);
    g_com_mode_chk_cnt = 0;
    race_1wire_init(smchg_uart_port, baudrate);
}

void smchg_1wire_leave_race_mode(void)
{
    //mux_ctrl_para_t port_config_parm;

    log_hal_msgid_info("1wire, leave_race_mode", 0);
    /*if(xTimerStop(pSmartChargerOutTimer, 0) == pdFAIL)
    {
        log_hal_msgid_error("1wire, leave_race_mode, stop timer fail", 0);
    }
    race_1wire_deinit();*/
}

void smchg_1wire_log_mode(void)
{
    mux_ctrl_para_t port_config_parm;

    if (smchg_1wire_get_mode_status() == SMCHG_1WIRE_LOG) {
        log_hal_msgid_warning("1wire, already in log_mode", 0);
        return ;
    }

    log_hal_msgid_info("1wire, log_mode, baudrate[%d]", 1, smchg_1wire_baudrate_report(smchg_cfg.log_mode_baud_rate));

    if (xTimerStopFromISR(pSmartChargerOutTimer, 0) == pdFAIL) {
        log_hal_msgid_error("1wire, log_mode, stop timer fail", 0);
    }

    port_config_parm.mux_set_config_uart_param.int_enable = FALSE;
    mux_control(smchg_mux_handle, MUX_CMD_CHANGE_UART_TX_INT, &port_config_parm);
    port_config_parm.mux_set_config_uart_param.baudrate = smchg_cfg.log_mode_baud_rate;
    mux_control(smchg_mux_handle, MUX_CMD_CHANGE_UART_PARAM, &port_config_parm);
    mux_control((mux_port_t)smchg_uart_port, MUX_CMD_UART_TX_ENABLE, NULL);

    /* BTA-24062 must set behand pin mux setting. */
    smchg_1wire_set_mode_status(SMCHG_1WIRE_LOG);

    smchg_1wire_unlock_sleep();
}

static void smchg_1wire_chg_mode(void)
{
    mux_ctrl_para_t port_config_parm;

    if (smchg_1wire_get_mode_status() == SMCHG_1WIRE_CHG) {
        log_hal_msgid_warning("1wire, already in chg_mode", 0);
        return ;
    }

    log_hal_msgid_info("1wire, chg_mode", 0);

    if (xTimerStop(pSmartChargerOutTimer, 0) == pdFAIL) {
        log_hal_msgid_error("1wire, chg_mode, stop timer fail", 0);
    }

    smchg_1wire_set_mode_status(SMCHG_1WIRE_CHG);

    port_config_parm.mux_set_config_uart_param.int_enable = FALSE;
    mux_control(smchg_mux_handle, MUX_CMD_CHANGE_UART_TX_INT, &port_config_parm);
    mux_control((mux_port_t)smchg_uart_port, MUX_CMD_UART_TX_RX_DISABLE, NULL);


    chg_mode_flag = 1;
    g_com_mode_chk_cnt = 0;

    smchg_1wire_unlock_sleep();
}

static void smchg_1wire_com_mode(void)
{
    mux_ctrl_para_t port_config_parm;

    log_hal_msgid_info("1wire, com_mode, baudrate[%d]", 1, smchg_1wire_baudrate_report(smchg_cfg.com_mode_baud_rate));

    if (xTimerChangePeriodFromISR(pSmartChargerOutTimer, pdMS_TO_TICKS(smchg_cfg.com_mode_chk_timer), 0) == pdFAIL) {
        log_hal_msgid_error("1wire, com_mode, change com_mode_timer fail", 0);
    }

    smchg_1wire_set_mode_status(SMCHG_1WIRE_COM);

    port_config_parm.mux_set_config_uart_param.baudrate = smchg_cfg.com_mode_baud_rate;
    mux_control(smchg_mux_handle, MUX_CMD_CHANGE_UART_PARAM, &port_config_parm);
    mux_control((mux_port_t)smchg_uart_port, MUX_CMD_UART_RX_ENABLE, NULL);
    port_config_parm.mux_set_config_uart_param.int_enable = TRUE;
    mux_control(smchg_mux_handle, MUX_CMD_CHANGE_UART_TX_INT, &port_config_parm);

    smchg_1wire_lock_sleep();
}

static void smchg_1wire_out_of_case(void)
{
    mux_ctrl_para_t port_config_parm;

    log_hal_msgid_info("1wire, out_of_case", 0);

    if (xTimerStop(pSmartChargerOutTimer, 0) == pdFAIL) {
        log_hal_msgid_error("1wire, out_of_case, stop timer fail", 0);
    }

    smchg_1wire_set_mode_status(SMCHG_1WIRE_OUT);

    port_config_parm.mux_set_config_uart_param.int_enable = FALSE;
    mux_control(smchg_mux_handle, MUX_CMD_CHANGE_UART_TX_INT, &port_config_parm);
    mux_control((mux_port_t)smchg_uart_port, MUX_CMD_UART_TX_RX_DISABLE, NULL);

    chg_mode_flag = 0;

    smchg_1wire_unlock_sleep();
}


static void smchg_1wire_pwr_save_mode(void)
{
    mux_ctrl_para_t port_config_parm;

    log_hal_msgid_info("1wire, pwr_save_mode", 0);

    if(xTimerStop(pSmartChargerOutTimer, 0) == pdFAIL)
    {
        log_hal_msgid_error("1wire, pwr_save_mode, stop timer fail", 0);
    }

    smchg_1wire_set_mode_status(SMCHG_1WIRE_OUT);

    port_config_parm.mux_set_config_uart_param.int_enable = FALSE;
    mux_control(smchg_mux_handle, MUX_CMD_CHANGE_UART_TX_INT, &port_config_parm);
    mux_control((mux_port_t)smchg_uart_port, MUX_CMD_UART_TX_RX_DISABLE, NULL);

    chg_mode_flag = 0;

    smchg_1wire_unlock_sleep();

}

smchg_status_t smchg_1wire_enter_pwr_save_mode(void)
{
    smchg_1wire_com_mode_to_pwr_save_mode();
    smchg_1wire_pwr_save_mode();

    return SMCHG_STATUS_OK;
}


static void smchg_1wire_send_to_app(uint8_t cmd, uint32_t data, uint16_t data_len)
{
    if (last_cmd == cmd) {
        if (cmd == SMCHG_LID_OPEN ||
            cmd == SMCHG_LID_CLOSE_DONE ||
            cmd == SMCHG_LID_CLOSE) {
            log_hal_msgid_warning("1wire, repeat app_cmd[%d]", 1, cmd);
            return;
        }
    }

    last_cmd = cmd;

    if (cmd) {
        smartcharger_callback_t callback_handler = ChargerSmartCase_GetSmartCaseHandle();
        if (callback_handler) {
            smchg_1wire_app_cmd(cmd);
            callback_handler(cmd, 1, data, data_len);
            //log_hal_msgid_info("1wire, send_to_app, cmd[%d], data[%d], data_len[%d]", 3, cmd, data, data_len);
        } else {
            pre_cmd = cmd;
            pre_data = data;
            pre_data_len = data_len;
        }
    }
}

void smchg_1wire_pre_handle(void)
{
    if (pre_cmd) {
        smchg_1wire_send_to_app(pre_cmd, pre_data, pre_data_len);
        pre_cmd = 0;
        pre_data = 0;
        pre_data_len = 0;
    }
}

static void smchg_1wire_com_mode_timer_cb(TimerHandle_t pxExpiredTimer)
{
    if (smchg_1wire_com_mode_to_out_of_case() != SMCHG_STATUS_OK) {
        return;
    }
    //log_hal_msgid_info("1wire, out_of_case ok", 0);

    if (race_mode_flag) {
        smchg_1wire_leave_race_mode();
        race_mode_flag = 0;
    }

    if (smchg_cfg.one_wire_log) {
        smchg_1wire_log_mode();
    } else {
        smchg_1wire_out_of_case();
    }
    smchg_1wire_send_to_app(SMCHG_CHG_OUT, 0, 0);
}


static void smchg_1wire_rx_handle_timer_cb(TimerHandle_t pxExpiredTimer)
{
    smchg_1wire_rx_handle(g_rx_data_len, g_rx_user_data);
    is_rx_timer_start = FALSE;
    g_rx_data_len     = 0;
    g_rx_user_data    = NULL;
}



static void smchg_1wire_rx_timer_start(uint32_t data_len, void *user_data)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    if (is_rx_timer_start == FALSE) {

        if (xTimerStartFromISR(pSmartChargerRxTimer, &xHigherPriorityTaskWoken) != pdPASS) {
            log_hal_msgid_info("1wire, rx timer start from isr fail", 0);
        }
        else {
            is_rx_timer_start = TRUE;
            g_rx_data_len = data_len;
            g_rx_user_data = user_data;
            log_hal_msgid_info("1wire, rx timer start", 0);
        }
    }
    else {
        log_hal_msgid_info("1wire, rx timer restart", 0);
    }

    if (is_rx_timer_start && xHigherPriorityTaskWoken != pdFALSE) {
        // Actual macro used here is port specific.
        portYIELD_FROM_ISR(pdTRUE);
    }
}





static void smchg_1wire_chg_isr_cb(battery_management_event_t event, const void *data)
{
    int32_t chg_exist = smchg_1wire_chg_exist();

    if (event != BATTERY_MANAGEMENT_EVENT_CHARGER_EXIST_UPDATE) {
        return;
    }
    log_hal_msgid_info("1wire, chg_isr_cb, chg_exist[%d], bat_evt[%d]", 2, chg_exist, event);

    if (chg_exist) { //any mode to chg mode
        smchg_1wire_intr_to_chg_mode();

        if (race_mode_flag) {
            smchg_1wire_leave_race_mode();
            race_mode_flag = 0;
        }
        smchg_1wire_chg_mode();
        smchg_1wire_send_to_app(SMCHG_CHG_IN, 0, 0);
    } else if (smchg_1wire_get_mode_status() == SMCHG_1WIRE_CHG) { //any mode to com mode, without enter out of case directly
        smchg_1wire_intr_to_com_mode();
        smchg_1wire_com_mode();
        xTimerStart(pSmartChargerOutTimer, 0);
    }
}


static void smchg_1wire_rx_handle(uint32_t user_data_len, void *user_data)
{
    uint8_t raceCmd[SMCHG_MUX_RX_DATA_SIZE] = {0};
    mux_buffer_t buffer;
    buffer.buf_size = SMCHG_MUX_RX_DATA_SIZE;
    buffer.p_buf = raceCmd;
    uint32_t data_size = 0;;
    uint32_t rcv_size = 0;

    mux_rx(smchg_mux_handle, &buffer, &data_size);
    rcv_size = (raceCmd[LENGTH_H]<<8) | raceCmd[LENGTH_L] + 4;
    log_hal_msgid_info("1wire, rx_handle, data_size[%d], rcv_size[%d]", 2, data_size, rcv_size);


    if (xTimerReset(pSmartChargerOutTimer, 0) == pdFAIL) {
          log_hal_msgid_error("1wire, rx_handle, reset com_mode_timer fail", 0);
    }

    if ((data_size >= 8) && (rcv_size == data_size)) {

        audio_channel_t earbud_ch = ami_get_audio_channel();

        log_hal_msgid_info("1wire, rx_handle, race_ch[0x%X], race_type[0x%X], cmd_id[0x%X], earbud_id[%d], data[%d], earbud_ch[%d]", 6,
                           raceCmd[RACE_CH], raceCmd[RACE_TYPE], raceCmd[CMD_ID], raceCmd[EARBUD_ID], raceCmd[DATA], earbud_ch);

        raceEvt[EARBUD_ID] = earbud_ch;

        if ((raceCmd[RACE_CH] == 0x5) && (raceCmd[RACE_TYPE] == 0x5A) && (raceCmd[CMD_ID])) {
            uint32_t data = 0;
            uint16_t data_len = 0;

            if (raceCmd[CMD_ID] == CASE_LID_OPEN || raceCmd[CMD_ID] == CASE_CHARGER_OFF) {
                data = raceCmd[BAT_LV];
                data_len = 1;
            } else if (raceCmd[CMD_ID] == CASE_CHARGER_KEY) {
                data = raceCmd[KEY_ID];
                data_len = 1;
            }

            if (raceCmd[CMD_ID] <= CASE_LID_CLOSE) {
                smchg_1wire_send_to_app(smchg_app_table[raceCmd[CMD_ID]], data, data_len);
            }

            if (raceCmd[EARBUD_ID] == earbud_ch) {
                raceEvt[CMD_ID] = raceCmd[CMD_ID];
                raceEvt[EARBUD_ID] = earbud_ch;
                raceEvt[BAT_LV] = battery_management_get_battery_property(BATTERY_PROPERTY_CAPACITY);

                mux_control((mux_port_t)smchg_uart_port, MUX_CMD_UART_TX_ENABLE, NULL);
                uint32_t size;
                buffer.buf_size = sizeof(raceEvt);
                buffer.p_buf = raceEvt;
                mux_tx(smchg_mux_handle, &buffer, 1, &size);
                log_hal_msgid_info("1wire, rx_handle, response, evt_type[0x%X], evt_id[0x%X], earbud_id[%d], data[%d], size[%d]", 5,
                                   raceEvt[RACE_TYPE], raceEvt[CMD_ID], raceEvt[EARBUD_ID], raceEvt[DATA], size);

                if (raceCmd[CMD_ID] == CASE_CHARGER_OFF) {
                    mux_control((mux_port_t)smchg_uart_port, MUX_CMD_UART_TX_ENABLE, NULL);
                }

                if (raceCmd[CMD_ID] == CASE_LOG_ENABLE) {
                    smchg_1wire_com_mode_tx_done();
                    if (smchg_cfg.one_wire_log) {
                        smchg_1wire_init_to_log_mode();
                        smchg_1wire_log_mode();
                    }
                }

                if (raceCmd[CMD_ID] == CASE_RACE_ENABLE) {
                    if (race_mode_flag) {
                        log_hal_msgid_error("1wire, rx_handle, race_mode already exist", 0);
                        return;
                    }
                    smchg_1wire_com_mode_tx_done();
                    race_mode_flag = 1;
                    smchg_1wire_race_mode(raceCmd[BAUDRATE]);
                }
            }
        }
    } else {
        log_hal_msgid_warning("1wire, rx_handle, unknown_data[0x%X 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X]", 8,
                              raceCmd[0], raceCmd[1], raceCmd[2], raceCmd[3], raceCmd[4], raceCmd[5], raceCmd[6], raceCmd[7]);
    }
}

void smchg_1wire_uart_cb(mux_handle_t handle, mux_event_t event, uint32_t data_len, void *user_data)
{
    //log_hal_msgid_info("1wire, uart_cb, handle[0x%X], mux_event[%d], data_len[%d]", 3, handle, event, data_len);
    if (event == MUX_EVENT_READY_TO_READ) {
        log_hal_msgid_info("1wire, uart_cb, rx_done", 0);
        g_com_mode_chk_cnt = 0;
        smchg_1wire_rx_timer_start(data_len, user_data);
    } else if (event == MUX_EVENT_TRANSMISSION_DONE) {
        log_hal_msgid_info("1wire, uart_cb, tx_done", 0);
        g_com_mode_chk_cnt = 0;
        mux_control((mux_port_t)smchg_uart_port, MUX_CMD_UART_RX_ENABLE, NULL);
    }
}

smchg_mux_buffer_t smchg_mux_buffer;
void smchg_1wire_mux_buffer_fetch_init(mux_buffer_t buffers[], uint32_t buffers_counter)
{
    uint32_t i;

    smchg_mux_buffer.mux_buffer = buffers;
    smchg_mux_buffer.offset = 0;
    smchg_mux_buffer.idx = 0;
    smchg_mux_buffer.counter = buffers_counter;
    smchg_mux_buffer.left_data_len = 0;
    for (i = 0; i < buffers_counter; i++) {
        smchg_mux_buffer.left_data_len += buffers[i].buf_size;
    }
    smchg_mux_buffer.total_length = smchg_mux_buffer.left_data_len;
    /*log_hal_msgid_info("1wire, mux_buffer_featch_init, buf_size[%d], left_data_len[%d], total_length[%d]", 3,
        buffers[0].buf_size, smchg_mux_buffer.left_data_len, smchg_mux_buffer.total_length);*/
}

bool smchg_1wire_mux_header_fetch(uint8_t *out_buf, uint32_t out_len)
{
    uint32_t i;

    if (smchg_mux_buffer.idx >= smchg_mux_buffer.counter) {
        log_hal_msgid_error("1wire, mux_header_featch counter fail, idx[%d], counter[%d]", 2,
                            smchg_mux_buffer.idx, smchg_mux_buffer.counter);
        return false;
    }

    if (smchg_mux_buffer.left_data_len < out_len) {
        log_hal_msgid_error("1wire, mux_header_featch length fail, left_data_len[%d], out_len[%d]", 2,
                            smchg_mux_buffer.left_data_len, out_len);
        return false;
    }

    for (i = 0; i < out_len; i++, smchg_mux_buffer.left_data_len--, smchg_mux_buffer.offset++) {
        if (smchg_mux_buffer.offset >= smchg_mux_buffer.mux_buffer[smchg_mux_buffer.idx].buf_size) {
            smchg_mux_buffer.idx++;
            smchg_mux_buffer.offset = 0;
            if (smchg_mux_buffer.idx >= smchg_mux_buffer.counter) {
                log_hal_msgid_error("1wire, mux_header_featch counter fail, idx[%d], counter[%d]", 2,
                                    smchg_mux_buffer.idx, smchg_mux_buffer.counter);
                assert(0);
            }
        }
        *(out_buf + i) = *(smchg_mux_buffer.mux_buffer[smchg_mux_buffer.idx].p_buf + smchg_mux_buffer.offset);
        //RACE_LOG_MSGID_I("1wire, mux_header_featch, data[0x%x]", 1, *(out_buf+i));
    }
    return true;
}

void smchg_1wire_mux_tx_protocol_cb(mux_handle_t handle, const mux_buffer_t payload[], uint32_t buffers_counter, mux_buffer_t *head, mux_buffer_t *tail, void *user_data)
{
    const char *user_name = NULL;
    uint32_t i = 0, race_id = 0, total_size = 0;

    mux_status_t status = mux_query_user_name(handle, &user_name);
    smchg_1wire_mode_t smchg_1wire_status = smchg_1wire_get_mode_status();
    if ((status == MUX_STATUS_OK) && (user_name != NULL)) {
        if (smchg_1wire_status == SMCHG_1WIRE_NORM) {
            if (strcmp(user_name, "ATCI") == 0) {
                /* [Special] Need to fill the head buffer */
                race_id = 0x0F92;
            } else {
                /* SYSLOG/RACE/other */
                head->p_buf = NULL;
                tail->p_buf = NULL;
                head->buf_size = 0;
                tail->buf_size = 0;
                return;
            }
        } else if (smchg_1wire_status == SMCHG_1WIRE_LOG) {
            if (strcmp(user_name, "SYSLOG") == 0) {
                head->p_buf = NULL;
                tail->p_buf = NULL;
                head->buf_size = 0;
                tail->buf_size = 0;
                return;
            } else {
                /* Ignore tx except syslog */
                head->p_buf[TX_PROTOCOL_CB_HEAD_BUFFER_MAX_LEN + 0] = 0x65;
                head->p_buf[TX_PROTOCOL_CB_HEAD_BUFFER_MAX_LEN + 1] = 0x72;
                head->p_buf[TX_PROTOCOL_CB_HEAD_BUFFER_MAX_LEN + 2] = 0x69;
                head->p_buf[TX_PROTOCOL_CB_HEAD_BUFFER_MAX_LEN + 3] = 0x57;

                tail->p_buf[TX_PROTOCOL_CB_HEAD_BUFFER_MAX_LEN + 0] = 0x65;
                tail->p_buf[TX_PROTOCOL_CB_HEAD_BUFFER_MAX_LEN + 1] = 0x72;
                tail->p_buf[TX_PROTOCOL_CB_HEAD_BUFFER_MAX_LEN + 2] = 0x69;
                tail->p_buf[TX_PROTOCOL_CB_HEAD_BUFFER_MAX_LEN + 3] = 0x57;

                head->p_buf = NULL;
                tail->p_buf = NULL;
                head->buf_size = 0;
                tail->buf_size = 0;
                return;
            }
        } else if (smchg_1wire_status == SMCHG_1WIRE_COM) {
            if (strcmp(user_name, "SM_CHG") == 0) {
                head->p_buf = NULL;
                tail->p_buf = NULL;
                head->buf_size = 0;
                tail->buf_size = 0;
                return;
            } else {
                /* Ignore tx except CHG CMD */
                head->p_buf[TX_PROTOCOL_CB_HEAD_BUFFER_MAX_LEN + 0] = 0x65;
                head->p_buf[TX_PROTOCOL_CB_HEAD_BUFFER_MAX_LEN + 1] = 0x72;
                head->p_buf[TX_PROTOCOL_CB_HEAD_BUFFER_MAX_LEN + 2] = 0x69;
                head->p_buf[TX_PROTOCOL_CB_HEAD_BUFFER_MAX_LEN + 3] = 0x57;

                tail->p_buf[TX_PROTOCOL_CB_HEAD_BUFFER_MAX_LEN + 0] = 0x65;
                tail->p_buf[TX_PROTOCOL_CB_HEAD_BUFFER_MAX_LEN + 1] = 0x72;
                tail->p_buf[TX_PROTOCOL_CB_HEAD_BUFFER_MAX_LEN + 2] = 0x69;
                tail->p_buf[TX_PROTOCOL_CB_HEAD_BUFFER_MAX_LEN + 3] = 0x57;

                head->p_buf = NULL;
                tail->p_buf = NULL;
                head->buf_size = 0;
                tail->buf_size = 0;
                return;
            }
        } else if (smchg_1wire_status == SMCHG_1WIRE_RACE) {
            if (strcmp(user_name, "RACE_CMD") == 0) {
                head->p_buf = NULL;
                tail->p_buf = NULL;
                head->buf_size = 0;
                tail->buf_size = 0;
                return;
            } else {
                /* Ignore tx except race CMD */
                head->p_buf[TX_PROTOCOL_CB_HEAD_BUFFER_MAX_LEN + 0] = 0x65;
                head->p_buf[TX_PROTOCOL_CB_HEAD_BUFFER_MAX_LEN + 1] = 0x72;
                head->p_buf[TX_PROTOCOL_CB_HEAD_BUFFER_MAX_LEN + 2] = 0x69;
                head->p_buf[TX_PROTOCOL_CB_HEAD_BUFFER_MAX_LEN + 3] = 0x57;

                tail->p_buf[TX_PROTOCOL_CB_HEAD_BUFFER_MAX_LEN + 0] = 0x65;
                tail->p_buf[TX_PROTOCOL_CB_HEAD_BUFFER_MAX_LEN + 1] = 0x72;
                tail->p_buf[TX_PROTOCOL_CB_HEAD_BUFFER_MAX_LEN + 2] = 0x69;
                tail->p_buf[TX_PROTOCOL_CB_HEAD_BUFFER_MAX_LEN + 3] = 0x57;

                head->p_buf = NULL;
                tail->p_buf = NULL;
                head->buf_size = 0;
                tail->buf_size = 0;
                return;
            }
        } else if ((smchg_1wire_status == SMCHG_1WIRE_CHG) || (smchg_1wire_status == SMCHG_1WIRE_OUT)) {
            /* Ignore all tx */
            head->p_buf[TX_PROTOCOL_CB_HEAD_BUFFER_MAX_LEN + 0] = 0x65;
            head->p_buf[TX_PROTOCOL_CB_HEAD_BUFFER_MAX_LEN + 1] = 0x72;
            head->p_buf[TX_PROTOCOL_CB_HEAD_BUFFER_MAX_LEN + 2] = 0x69;
            head->p_buf[TX_PROTOCOL_CB_HEAD_BUFFER_MAX_LEN + 3] = 0x57;

            tail->p_buf[TX_PROTOCOL_CB_HEAD_BUFFER_MAX_LEN + 0] = 0x65;
            tail->p_buf[TX_PROTOCOL_CB_HEAD_BUFFER_MAX_LEN + 1] = 0x72;
            tail->p_buf[TX_PROTOCOL_CB_HEAD_BUFFER_MAX_LEN + 2] = 0x69;
            tail->p_buf[TX_PROTOCOL_CB_HEAD_BUFFER_MAX_LEN + 3] = 0x57;

            head->p_buf = NULL;
            tail->p_buf = NULL;
            head->buf_size = 0;
            tail->buf_size = 0;
            return;
        } else {
            /* mode error */
            head->p_buf = NULL;
            tail->p_buf = NULL;
            head->buf_size = 0;
            tail->buf_size = 0;
            return;
        }
    } else {
        head->p_buf = NULL;
        tail->p_buf = NULL;
        head->buf_size = 0;
        tail->buf_size = 0;
        return;
    }

    total_size = 0;
    for (i = 0; i < buffers_counter; i++) {
        total_size += payload[i].buf_size;
    }

    /* Insert the race header here */
    head->p_buf[0] = 0x05;
    head->p_buf[1] = RACE_TYPE_NOTIFICATION;
    head->p_buf[2] = (uint8_t)((total_size + 2) & 0xFF);
    head->p_buf[3] = (uint8_t)(((total_size + 2) >> 8) & 0xFF);
    head->p_buf[4] = (uint8_t)(race_id & 0xFF);
    head->p_buf[5] = (uint8_t)((race_id >> 8) & 0xFF);
    head->buf_size = 6;

    tail->p_buf = NULL;
    tail->buf_size = 0;
}

void smchg_1wire_mux_rx_protocol_cb(mux_handle_t *handle, mux_buffer_t buffers[], uint32_t buffers_counter,
                                    uint32_t *consume_len, uint32_t *package_len, void *user_data)
{
    uint8_t race_ch, race_type;
    uint16_t race_length, race_id;
    uint32_t collected_length;
    mux_handle_t p_handle;

    //log_hal_msgid_info("1wire, mux_rx_protocol_cb, buffers_counter[%d]", 1, buffers_counter);

    race_length = 0;
    collected_length = 0;
    *package_len = 0;
    *consume_len = 0;

    smchg_1wire_mode_t smchg_1wire_status = smchg_1wire_get_mode_status();

    smchg_1wire_mux_buffer_fetch_init(buffers, buffers_counter);

    /* 1wire sm check */
    if ((smchg_1wire_status == SMCHG_1WIRE_LOG) || (smchg_1wire_status == SMCHG_1WIRE_CHG) || (smchg_1wire_status == SMCHG_1WIRE_OUT)) {
        /* ignore all rx */
        *handle = 0x0;
        *package_len = 0;
        *consume_len = smchg_mux_buffer.total_length;
        return;
    } else if ((smchg_1wire_status == SMCHG_1WIRE_NORM) || (smchg_1wire_status == SMCHG_1WIRE_COM) || (smchg_1wire_status == SMCHG_1WIRE_RACE) || (smchg_1wire_status == SMCHG_1WIRE_ATCI)) {
        /* allow rx handle */
    } else {
        /* error */
        *handle = 0x0;
        *package_len = 0;
        *consume_len = smchg_mux_buffer.total_length;
        return;
    }

    if (smchg_1wire_mux_header_fetch(&race_ch, 1) == false) {
        *package_len = 0;
        *consume_len = smchg_mux_buffer.total_length - smchg_mux_buffer.left_data_len;
        log_hal_msgid_error("1wire, mux_rx_protocol_cb, race_ch fetch fail", 0);
        return;
    }

    if ((race_ch & 0x07) == 0x05) {
        if (smchg_1wire_mux_header_fetch(&race_type, 1) == false) {
            *package_len = 0;
            *consume_len = smchg_mux_buffer.total_length - smchg_mux_buffer.left_data_len;
            log_hal_msgid_error("1wire, mux_rx_protocol_cb, race_type fetch fail", 0);
            return;
        }

        if ((race_type != RACE_TYPE_COMMAND) && (race_type != RACE_TYPE_COMMAND_WITHOUT_RSP)) {
            *package_len = 0;
            *consume_len = smchg_mux_buffer.total_length - smchg_mux_buffer.left_data_len;
            log_hal_msgid_error("1wire, mux_rx_protocol_cb, race_type[0x%X]", 1, race_type);
            return;
        }

        if (smchg_1wire_mux_header_fetch((uint8_t *)&race_length, 2) == false) {
            *package_len = 0;
            *consume_len = smchg_mux_buffer.total_length - smchg_mux_buffer.left_data_len;
            log_hal_msgid_error("1wire, mux_rx_protocol_cb, race_length fetch fail", 0);
            return;
        }

        if (smchg_1wire_mux_header_fetch((uint8_t *)&race_id, 2) == false) {
            *package_len = 0;
            *consume_len = smchg_mux_buffer.total_length - smchg_mux_buffer.left_data_len;
            log_hal_msgid_error("1wire, mux_rx_protocol_cb, race_id fetch fail", 0);
            return;
        }

        *handle = 0;
        collected_length = smchg_mux_buffer.total_length - smchg_mux_buffer.left_data_len; //6
        if (race_id == 0x0F92) {
            if (mux_query_user_handle(smchg_uart_port, "ATCI", &p_handle) == MUX_STATUS_OK) {
                /* ATCI race command */
                *handle = p_handle;
                /* RACE cmd format: 05 5D length(2 byte) 0f92 at+test=? */
                *package_len = race_length - 2;
                *consume_len = 6;
            } else {
                *package_len = 0;
                *consume_len = race_length + 4;
                //return;
            }
        } else if (race_id == 0x2000) {
            if (smchg_1wire_status != SMCHG_1WIRE_COM) {
                *package_len = 0;
                *consume_len = collected_length;
                //return;
            } else if (mux_query_user_handle((mux_port_t)smchg_uart_port, "SM_CHG", &p_handle) == MUX_STATUS_OK) {
                /* 1wire cmd */
                if (smchg_mux_buffer.left_data_len >= (race_length - 2)) {
                    *package_len = collected_length + race_length - 2;
                    *consume_len = 0;
                    *handle = p_handle;
                } else {
                    *package_len = collected_length + smchg_mux_buffer.left_data_len;
                    *consume_len = 0;
                    *handle = p_handle;
                    log_hal_msgid_error("1wire, mux_rx_protocol_cb fail, data len not enough", 0);
                }

                g_com_mode_chk_cnt = 0;
            } else {
                *package_len = 0;
                *consume_len = collected_length;
                //return;
            }
        } else {
            if ((mux_query_user_handle((mux_port_t)smchg_uart_port, "RACE_CMD", &p_handle) == MUX_STATUS_OK) &&
                (smchg_1wire_status == SMCHG_1WIRE_RACE)) {
                //race cmd
                *handle = p_handle;
                *package_len = race_length + 4;
                *consume_len = 0;
                g_com_mode_chk_cnt = 0;
            } else {
                *package_len = 0;
                *consume_len = race_length + 4;
                log_hal_msgid_error("1wire, mux_rx_protocol_cb fail, no_race_handle", 0);
            }
        }
        //log_hal_msgid_info("1wire, mux_rx_protocol_cb ok, race_ch[0x%X], race_type[0x%X], race_length[%d], race_id[0x%X] handle[0x%X]", 5,
        //race_ch, race_type, race_length, race_id, *handle);
    } else {
        *package_len = 0;
        *consume_len = smchg_mux_buffer.total_length - smchg_mux_buffer.left_data_len;
        log_hal_msgid_error("1wire, mux_rx_protocol_cb fail, race_ch[0x%X]", 1, race_ch);
    }
}

void smchg_1wire_tx_gpio_driving(hal_gpio_driving_current_t driving)
{
    switch (smchg_cfg.uart_sel) {
        case SMCHG_UART0_2GPIO:
        case SMCHG_UART1_2GPIO:
            hal_gpio_set_driving_current(smchg_1wire_gpio_cfg.tx_pin, driving);
            break;

        case SMCHG_UART0_1GPIO:
        case SMCHG_UART1_1GPIO:
            hal_gpio_set_driving_current(smchg_1wire_gpio_cfg.trx_pin, driving);
            break;

        default:
            break;
    }
}

void smchg_1wire_gpio_init(void)
{
    switch (smchg_cfg.uart_sel) {
        case SMCHG_UART0_2GPIO: {
            if ((HAL_UART0_TXD_PIN | HAL_UART0_TXD_PIN_M_GPIO | HAL_UART0_TXD_PIN_M_UART0_TXD |
                 HAL_UART0_RXD_PIN | HAL_UART0_RXD_PIN_M_GPIO | HAL_UART0_RXD_PIN_M_UART0_RXD) == 0xFF) {
                assert(0);
            }

            smchg_1wire_gpio_cfg.tx_pin  = HAL_UART0_TXD_PIN;
            smchg_1wire_gpio_cfg.tx_gpio = HAL_UART0_TXD_PIN_M_GPIO;
            smchg_1wire_gpio_cfg.tx_uart = HAL_UART0_TXD_PIN_M_UART0_TXD;
            smchg_1wire_gpio_cfg.rx_pin  = HAL_UART0_RXD_PIN;
            smchg_1wire_gpio_cfg.rx_gpio = HAL_UART0_RXD_PIN_M_GPIO;
            smchg_1wire_gpio_cfg.rx_uart = HAL_UART0_RXD_PIN_M_UART0_RXD;
        }
        break;

        case SMCHG_UART1_2GPIO: {
            if ((HAL_UART1_TXD_PIN | HAL_UART1_TXD_PIN_M_GPIO | HAL_UART1_TXD_PIN_M_UART1_TXD |
                 HAL_UART1_RXD_PIN | HAL_UART1_RXD_PIN_M_GPIO | HAL_UART1_RXD_PIN_M_UART1_RXD) == 0xFF) {
                assert(0);
            }

            smchg_1wire_gpio_cfg.tx_pin  = HAL_UART1_TXD_PIN;
            smchg_1wire_gpio_cfg.tx_gpio = HAL_UART1_TXD_PIN_M_GPIO;
            smchg_1wire_gpio_cfg.tx_uart = HAL_UART1_TXD_PIN_M_UART1_TXD;
            smchg_1wire_gpio_cfg.rx_pin  = HAL_UART1_RXD_PIN;
            smchg_1wire_gpio_cfg.rx_gpio = HAL_UART1_RXD_PIN_M_GPIO;
            smchg_1wire_gpio_cfg.rx_uart = HAL_UART1_RXD_PIN_M_UART1_RXD;
        }
        break;
#if defined (AIR_BTA_IC_PREMIUM_G3)
        case SMCHG_UART0_1GPIO: {
            if ((HAL_UART0_RXD_PIN | HAL_UART0_RXD_PIN_M_GPIO |
                 HAL_UART0_RXD_PIN_M_UART0_TXD | HAL_UART0_RXD_PIN_M_UART0_RXD) == 0xFF) {
                assert(0);
            }

            smchg_1wire_gpio_cfg.trx_pin  = HAL_UART0_RXD_PIN;
            smchg_1wire_gpio_cfg.trx_gpio = HAL_UART0_RXD_PIN_M_GPIO;
            smchg_1wire_gpio_cfg.tx_uart  = HAL_UART0_RXD_PIN_M_UART0_TXD;
            smchg_1wire_gpio_cfg.rx_uart  = HAL_UART0_RXD_PIN_M_UART0_RXD;
        }
        break;

        case SMCHG_UART1_1GPIO: {
            if ((HAL_UART1_RXD_PIN | HAL_UART1_RXD_PIN_M_GPIO |
                 HAL_UART1_RXD_PIN_M_UART1_TXD | HAL_UART1_RXD_PIN_M_UART1_RXD) == 0xFF) {
                assert(0);
            }

            smchg_1wire_gpio_cfg.trx_pin  = HAL_UART1_RXD_PIN;
            smchg_1wire_gpio_cfg.trx_gpio = HAL_UART1_RXD_PIN_M_GPIO;
            smchg_1wire_gpio_cfg.tx_uart  = HAL_UART1_RXD_PIN_M_UART1_TXD;
            smchg_1wire_gpio_cfg.rx_uart  = HAL_UART1_RXD_PIN_M_UART1_RXD;
        }
        break;
#endif
        default:
            log_hal_msgid_error("1wire, gpio_init fail, uart_sel[%d]", 1, smchg_cfg.uart_sel);
            break;
    }
    smchg_1wire_tx_gpio_driving(HAL_GPIO_DRIVING_CURRENT_4MA);
}

static void smchg_1wire_uart_init(void)
{
    mux_port_setting_t setting;
    mux_protocol_t protocol_callback_1wire = {smchg_1wire_mux_tx_protocol_cb, smchg_1wire_mux_rx_protocol_cb, NULL};
    mux_status_t mux_init_status;

    memset(&setting, 0, sizeof(setting));
    setting.dev_setting.uart.uart_config.baudrate    = smchg_cfg.com_mode_baud_rate;
    setting.dev_setting.uart.uart_config.word_length = HAL_UART_WORD_LENGTH_8;
    setting.dev_setting.uart.uart_config.stop_bit    = HAL_UART_STOP_BIT_1;
    setting.dev_setting.uart.uart_config.parity      = HAL_UART_PARITY_NONE;
    setting.dev_setting.uart.flowcontrol_type        = MUX_UART_NONE_FLOWCONTROL;
    setting.portLinkRegAddr                          = 0;

    setting.tx_buffer_size = SMCHG_MUX_TX_BUFF_SIZE;
    setting.rx_buffer_size = SMCHG_MUX_RX_BUFF_SIZE;
    mux_init_status = mux_init((mux_port_t)smchg_uart_port, &setting, &protocol_callback_1wire);
    if (mux_init_status != MUX_STATUS_OK) {
        log_hal_msgid_warning("1wire, mux_init fail[%d]", 1, mux_init_status);
    }
    mux_init_status = mux_open((mux_port_t)smchg_uart_port, "SM_CHG", &smchg_mux_handle, smchg_1wire_uart_cb, NULL);
    if (mux_init_status != MUX_STATUS_OK) {
        log_hal_msgid_error("1wire, mux_open fail[%d]", 1, mux_init_status);
        assert(0);
    }

    log_hal_msgid_info("1wire, uart_init, baudrate[%d]", 1,
                       smchg_1wire_baudrate_report(setting.dev_setting.uart.uart_config.baudrate));

    if ((boot_Flag == 0) && (pmu_get_power_off_reason() == 0x8)) {
        log_hal_msgid_info("1wire, boot_evt send", 0);
        mux_control((mux_port_t)smchg_uart_port, MUX_CMD_UART_TX_ENABLE, NULL);
        raceEvt[RACE_TYPE] = 0x5D;
        mux_buffer_t buf = {raceEvt, sizeof(raceEvt)};
        uint32_t size;
        mux_tx(smchg_mux_handle, &buf, 1, &size);
        smchg_1wire_com_mode_tx_done();
        raceEvt[RACE_TYPE] = 0x5B;
        boot_Flag = 1;
    }
}

void smchg_1wire_init(void)
{
    log_hal_msgid_info("1wire, init enter", 0);

    if (smchg_cfg.uart_sel > SMCHG_UART1_1GPIO) {
        log_hal_msgid_error("1wire, init fail, uart_sel[%d]", 1, smchg_cfg.uart_sel);
        //assert(0);
    } else if ((smchg_cfg.uart_sel == SMCHG_UART0_1GPIO) || (smchg_cfg.uart_sel == SMCHG_UART0_2GPIO)) {
        smchg_uart_port = SERIAL_PORT_DEV_UART_0;
    }

    last_cmd = SMCHG_CHG_OUT;
    pSmartChargerOutTimer = xTimerCreate("Smart Charger", pdMS_TO_TICKS(smchg_cfg.com_mode_chk_timer), pdTRUE, NULL, smchg_1wire_com_mode_timer_cb);
    if (!pSmartChargerOutTimer) {
        log_hal_msgid_error("1wire, init com_mode_timer_cb fail", 0);
        assert(0);
    }

    pSmartChargerRxTimer = xTimerCreate("Smart Charger", pdMS_TO_TICKS(SMCHG_MUX_RX_HANDLE_TIMER_MS), pdFALSE, NULL, smchg_1wire_rx_handle_timer_cb);
    if (!pSmartChargerRxTimer) {
        log_hal_msgid_error("1wire, init rx_handle_timer_cb fail", 0);
        assert(0);
    }

    smchg_1wire_uart_init();


    if (battery_management_register_callback(smchg_1wire_chg_isr_cb) != BATTERY_MANAGEMENT_STATUS_OK) {
        log_hal_msgid_error("1wire, init chg_isr_cb fail", 0);
        return;
    }


    int32_t chg_exist = smchg_1wire_chg_exist();
    log_hal_msgid_info("1wire, init chg_exist[%d]", 1, chg_exist);

    if (chg_exist) {
        smchg_1wire_init_to_chg_mode();
        smchg_1wire_chg_mode();
    } else {
        if (smchg_cfg.one_wire_log) {
            smchg_1wire_init_to_log_mode();
            smchg_1wire_log_mode();
        } else {
            smchg_1wire_init_to_out_of_case();
            smchg_1wire_out_of_case();
        }
    }
}
#endif

