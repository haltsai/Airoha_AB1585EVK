/* Copyright Statement:
 *
 * (C) 2017  Airoha Technology Corp. All rights reserved.
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

#include <string.h>
#include <stdio.h>
#include <stdint.h>

#include "hal_uart.h"
#include "hal_uart_internal.h"
#include "hal_nvic.h"
#include "hal_gpt_internal.h"
#include "hal_core_status.h"
#include "hal_hw_semaphore.h"

#include "syslog.h"
#include "syslog_port.h"
#include "syslog_nvkey_struct.h"

#include "mux.h"
#include "mux_port_common.h"

#include "serial_port_assignment.h"
#include "hal_resource_assignment.h"

/* nvdm for filter */
#ifdef MTK_NVDM_ENABLE
#include "nvdm.h"
#include "nvdm_id_list.h"
#endif

/* 1-wire log feature fucntion */
#if defined (AIR_1WIRE_ENABLE)
#include "smchg_1wire.h"
#endif

#if defined(MTK_NVDM_ENABLE) && !defined(AG3335)
#include "nvkey.h"
#include "nvkey_id_list.h"
#endif

#ifdef SYSTEM_DAEMON_TASK_ENABLE
#include "system_daemon.h"
#endif

#ifdef FREERTOS_ENABLE
#include "FreeRTOS.h"
#include "task.h"
#endif


/* create for self debug module */
log_create_module(syslog, PRINT_LEVEL_INFO);
/* create common debug module */
log_create_module(common, PRINT_LEVEL_INFO);
log_create_module(MPLOG, PRINT_LEVEL_WARNING);
log_create_module(printf, PRINT_LEVEL_INFO);

uint32_t PORT_SYSLOG_SHARE_VARIABLE_BEGIN   = HW_SYSRAM_PRIVATE_MEMORY_SYSLOG_VAR_START;
uint32_t PORT_SYSLOG_SDK_VERSION_BEGIN      = HW_SYSRAM_PRIVATE_MEMORY_SYSLOG_VERSION_VAR_START;
uint32_t PORT_SYSLOG_BUILD_TIME_BEGIN       = HW_SYSRAM_PRIVATE_MEMORY_SYSLOG_BUILD_TIME_VAR_START;
#ifndef AIR_UART1_EARLY_LOG_ENABLE
uint32_t PORT_SYSLOG_INIT_STAGE_PORT        = CONFIG_SYSLOG_INIT_STAGE_PORT;
#else
uint32_t PORT_SYSLOG_INIT_STAGE_PORT        = HAL_UART_1;
#endif
uint32_t PORT_SYSLOG_RUNNING_STAGE_PORT     = CONFIG_SYSLOG_RUNNING_STAGE_PORT;

extern uint32_t mux_query_nest_count(mux_handle_t handle);

#define SYSLOG_PORT_UNUSED(a) (void)a

#ifdef AIR_SYSLOG_BUFFER_EXPAND_ENABLE
#define AIR_SYSLOG_TX_BUFFER_SIZE       (32 * 1024)
#define AIR_SYSLOG_RX_BUFFER_SIZE       (1024)
#else
#define AIR_SYSLOG_TX_BUFFER_SIZE       (16 * 1024)
#define AIR_SYSLOG_RX_BUFFER_SIZE       (1024)
#endif

#define PORT_SYSLOG_MAX_ONE_PLAIN_LOG_SIZE (256)

/* hal uart internal API for */
extern UART_REGISTER_T *const g_uart_regbase[];
hal_uart_baudrate_t g_uart_baudrate = CONFIG_SYSLOG_BAUDRATE;

#if !defined(MTK_DEBUG_LEVEL_NONE)
void syslog_port_log_info(const char *message, ...)
{
    va_list ap;
    va_start(ap, message);
    vprint_module_log(&LOG_CONTROL_BLOCK_SYMBOL(syslog), __FUNCTION__, __LINE__, PRINT_LEVEL_INFO, message, ap);
    va_end(ap);
}

void syslog_port_log_warning(const char *message, ...)
{
    va_list ap;
    va_start(ap, message);
    vprint_module_log(&LOG_CONTROL_BLOCK_SYMBOL(syslog), __FUNCTION__, __LINE__, PRINT_LEVEL_WARNING, message, ap);
    va_end(ap);
}

void syslog_port_log_error(const char *message, ...)
{
    va_list ap;
    va_start(ap, message);
    vprint_module_log(&LOG_CONTROL_BLOCK_SYMBOL(syslog), __FUNCTION__, __LINE__, PRINT_LEVEL_ERROR, message, ap);
    va_end(ap);
}

void syslog_port_log_msgid_info(const char *message, uint32_t arg_cnt, ...)
{
    va_list ap;
    va_start(ap, arg_cnt);
    log_print_msgid(&LOG_CONTROL_BLOCK_SYMBOL(syslog), PRINT_LEVEL_INFO, message, arg_cnt, ap);
    va_end(ap);
}

void syslog_port_log_msgid_warning(const char *message, uint32_t arg_cnt, ...)
{
    va_list ap;
    va_start(ap, arg_cnt);
    log_print_msgid(&LOG_CONTROL_BLOCK_SYMBOL(syslog), PRINT_LEVEL_WARNING, message, arg_cnt, ap);
    va_end(ap);
}

void syslog_port_log_msgid_error(const char *message, uint32_t arg_cnt, ...)
{
    va_list ap;
    va_start(ap, arg_cnt);
    log_print_msgid(&LOG_CONTROL_BLOCK_SYMBOL(syslog), PRINT_LEVEL_ERROR, message, arg_cnt, ap);
    va_end(ap);
}
#else
void syslog_port_log_info(const char *message, ...) {}
void syslog_port_log_warning(const char *message, ...) {}
void syslog_port_log_error(const char *message, ...) {}
void syslog_port_log_msgid_info(const char *message, uint32_t arg_cnt, ...) {}
void syslog_port_log_msgid_warning(const char *message, uint32_t arg_cnt, ...) {}
void syslog_port_log_msgid_error(const char *message, uint32_t arg_cnt, ...) {}
#endif

uint32_t syslog_port_query_current_timestamp(void)
{
    uint32_t count = 0;
    uint64_t count64 = 0;

    hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_32K, &count);
    count64 = ((uint64_t)count) * 1000 / 32768;

    return (uint32_t)count64;
}

uint32_t syslog_port_memview_dsp0_to_mcu(uint32_t address)
{
    return hal_memview_dsp0_to_mcu(address);
}

uint32_t syslog_port_query_uart_baudrate(void)
{
    return g_uart_baudrate;
}

bool syslog_port_uart_is_xoff(uint32_t port)
{
    if (port >= HAL_UART_MAX) {
        return false;
    }

    if ((g_uart_regbase[port]->MCR_UNION.MCR & UART_MCR_XOFF_STATUS_MASK) != 0) {
        return true;
    }

    return false;
}

void syslog_port_uart_disable_flowcontrol(uint32_t port)
{
    if (port < HAL_UART_MAX) {
        hal_uart_disable_flowcontrol(port);
    }
}

void syslog_port_uart_disable_irq(uint32_t port)
{
    if (port < HAL_UART_MAX) {
        uart_disable_irq(port);
    }
}

void syslog_port_uart_send_polling(uint32_t port, const uint8_t *data, uint32_t size)
{
    if (port < HAL_UART_MAX) {
        hal_uart_send_polling(port, data, size);
    }
}

int32_t syslog_port_nvic_save_and_set_interrupt_mask(uint32_t *mask)
{
    return hal_nvic_save_and_set_interrupt_mask(mask);
}

int32_t syslog_port_nvic_restore_interrupt_mask(uint32_t mask)
{
    return hal_nvic_restore_interrupt_mask(mask);
}

uint32_t syslog_port_parse_port_number(syslog_port_type_t port_type, uint8_t port_index)
{
    if (port_type == SYSLOG_PORT_TYPE_UART) {
        return MUX_UART_BEGIN + port_index;
    } else if (port_type == SYSLOG_PORT_TYPE_USB) {
        return MUX_USB_BEGIN + port_index;
    }
    return PORT_SYSLOG_RUNNING_STAGE_PORT;
}

/* memory free */
void *syslog_port_malloc(uint32_t size)
{
    return malloc(size);
}

/* memory malloc */
void syslog_port_free(void *pdata)
{
    free(pdata);
}

/* syslog setting read from flash */
uint32_t syslog_port_read_setting(char *name, uint8_t *p_setting, uint32_t size)
{
#ifdef MTK_NVDM_ENABLE
    uint32_t max_size;
    nvdm_status_t status;

    if (name == NULL) {
        return false;
    }

    max_size = size;
    if (strcmp(name, "cpu_filter") == 0) {
        status = nvdm_read_data_item(NVDM_GROUP_SYSLOG, NVDM_SYSLOG_ITEM_CPU_FILTER, p_setting, &max_size);
    } else if (strcmp(name, "module_filter") == 0) {
        status = nvdm_read_data_item(NVDM_GROUP_SYSLOG, NVDM_SYSLOG_ITEM_MODULE_FILTER, p_setting, &max_size);
    } else {
        status = NVDM_STATUS_ERROR;
    }

    if (status == NVDM_STATUS_OK) {
        return true;
    }
#endif

    return false;
}

/* syslog setting write to flash */
bool syslog_port_save_setting(char *name, uint8_t *p_setting, uint32_t size)
{
#ifdef MTK_NVDM_ENABLE
    nvdm_status_t status;

    if (name == NULL) {
        return false;
    }

    if (strcmp(name, "cpu_filter") == 0) {
        status = nvdm_write_data_item(NVDM_GROUP_SYSLOG, NVDM_SYSLOG_ITEM_CPU_FILTER, NVDM_DATA_ITEM_TYPE_RAW_DATA, p_setting, size);
    } else if (strcmp(name, "module_filter") == 0) {
        status = nvdm_write_data_item(NVDM_GROUP_SYSLOG, NVDM_SYSLOG_ITEM_MODULE_FILTER, NVDM_DATA_ITEM_TYPE_RAW_DATA, p_setting, size);
    } else {
        status = NVDM_STATUS_ERROR;
    }

    if (status == NVDM_STATUS_OK) {
        return true;
    }

#endif

    return false;
}

uint32_t syslog_port_receive(uint32_t handle, uint8_t *p_buf, uint32_t size)
{
    mux_buffer_t buffer;
    uint32_t len;
    buffer.p_buf = p_buf;
    buffer.buf_size = size;
    mux_rx(handle, &buffer, &len);
    return len;
}

bool syslog_port_cmd_event_is_vaild(uint8_t event)
{
    if (event != MUX_EVENT_READY_TO_READ) {
        return false;
    }

    return true;
}

bool syslog_port_is_spp_port(uint32_t port)
{
    if (port == MUX_AIRAPP_0) {
        return true;
    }

    return false;
}

bool syslog_port_is_usb_port(uint32_t port)
{
    if ((port == MUX_USB_COM_1) || (port == MUX_USB_COM_2)) {
        return true;
    }

    return false;
}

void syslog_port_send_daemo_message(void)
{
#ifdef SYSTEM_DAEMON_TASK_ENABLE
    system_daemon_send_message(SYSTEM_DAEMON_ID_LOGGING_TO_FLASH, NULL);
#endif
}

void syslog_port_send_daemo_message_assert(void)
{
#ifdef SYSTEM_DAEMON_TASK_ENABLE
    system_daemon_send_message(SYSTEM_DAEMON_ID_LOGGING_TO_ASSERT, NULL);
#endif
}

uint32_t syslog_port_query_exception_number(void)
{
    return HAL_NVIC_QUERY_EXCEPTION_NUMBER;
}

uint32_t syslog_port_query_os_task_number(void)
{
#ifdef FREERTOS_ENABLE
    TaskStatus_t pxTaskStatus;

    if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) {
        vTaskGetInfo(xTaskGetCurrentTaskHandle(), &pxTaskStatus, pdFALSE, eInvalid);
        return pxTaskStatus.xTaskNumber;
    } else {
        return 0;
    }
#else
    return 0;
#endif

}

uint32_t syslog_port_query_nest_count(uint32_t handle)
{
    return mux_query_nest_count(handle);
}

bool syslog_port_query_user_name(uint32_t handle, const char **user_name)
{
    mux_status_t status;
    const char *name = NULL;

    status = mux_query_user_name(handle, &name);
    if (status != MUX_STATUS_OK) {
        return false;
    }

    *user_name = name;

    return true;
}

bool syslog_port_mux_exception_init(uint32_t handle)
{
    mux_status_t status;

    status = mux_exception_init(handle);
    if (status != MUX_STATUS_OK) {
        return false;
    }

    return true;
}

bool syslog_port_user_handle(uint32_t port, const char *user_name, uint32_t *p_handle)
{
    mux_status_t status;

    status = mux_query_user_handle(port, user_name, p_handle);
    if (status != MUX_STATUS_OK) {
        return false;
    }

    return true;
}

bool syslog_port_exception_send(uint32_t handle, uint8_t *buffers, uint32_t buffers_counter)
{
    mux_status_t status;

    status = mux_exception_send(handle, (mux_buffer_t *)buffers, buffers_counter);
    if (status != MUX_STATUS_OK) {
        return false;
    }

    return true;
}

bool syslog_port_mux_tx(uint32_t handle, uint8_t *buffers, uint32_t buffers_counter, uint32_t *send_done_data_len)
{
    mux_status_t status;

    status = mux_tx(handle, (mux_buffer_t *)buffers, buffers_counter, send_done_data_len);
    if (status != MUX_STATUS_OK) {
        return false;
    }

    return true;
}

uint32_t syslog_port_cmd_query_tx_buffer_free(uint32_t port)
{
    mux_status_t status;
    uint32_t tx_free_size;

    status = mux_control(port, MUX_CMD_GET_TX_AVAIL, (mux_ctrl_para_t *)&tx_free_size);
    if (status != MUX_STATUS_OK) {
        tx_free_size = 0;
    }

    return tx_free_size;
}

void syslog_port_cmd_clean_tx_buffer(uint32_t port)
{
    if ((port == MUX_USB_COM_1) || (port == MUX_USB_COM_2)) {
        mux_control(port, MUX_CMD_CLEAN, NULL);
    }
}

void syslog_port_excetpion_1wire_check(uint32_t port)
{
#if defined (AIR_1WIRE_ENABLE)
    if (syslog_query_one_wire_log_active() == true) {
        mux_control(port, MUX_CMD_UART_TX_ENABLE, NULL);
        syslog_port_uart_disable_flowcontrol(port);
    }
#endif
}

bool syslog_port_core_is_exception(void)
{
    if (hal_core_status_read(HAL_CORE_MCU) == HAL_CORE_EXCEPTION) {
        return true;
    }

    return false;
}

//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------

#if defined (AIR_1WIRE_ENABLE)
extern void smchg_1wire_mux_tx_protocol_cb(mux_handle_t handle, const mux_buffer_t payload[], uint32_t buffers_counter, mux_buffer_t *head, mux_buffer_t *tail, void *user_data);
extern void smchg_1wire_mux_rx_protocol_cb(mux_handle_t *handle, mux_buffer_t buffers[], uint32_t buffers_counter, uint32_t *consume_len, uint32_t *package_len, void *user_data);
#endif

void syslog_port_early_init(void)
{
    hal_uart_config_t uart_config;
    uart_config.baudrate        = syslog_port_query_uart_baudrate();
    uart_config.parity          = HAL_UART_PARITY_NONE;
    uart_config.stop_bit        = HAL_UART_STOP_BIT_1;
    uart_config.word_length     = HAL_UART_WORD_LENGTH_8;
    hal_uart_init(PORT_SYSLOG_INIT_STAGE_PORT, &uart_config);
#ifndef MTK_DEBUG_PLAIN_LOG_ENABLE
    hal_uart_set_software_flowcontrol(PORT_SYSLOG_INIT_STAGE_PORT, 0x11, 0x13, 0x77);
#endif
}

#if defined(MTK_NVDM_ENABLE) && !defined(AG3335)
bool syslog_port_form_nvkey_init(uint32_t *port)
{
    syslog_nvkey_cfg_t syslog_nvkey_data;
    uint32_t nvkey_item_size;
    uint8_t  syslog_port;
    nvkey_status_t nvkey_status;

    if (port == NULL) {
        return false;
    }

    /* read port setting from nvkey */
    nvkey_item_size = sizeof(syslog_nvkey_data);
    memset((uint8_t *)(&syslog_nvkey_data), 0, nvkey_item_size);

    nvkey_status = nvkey_read_data(NVID_PERI_LOG_SETTING, (uint8_t *)(&syslog_nvkey_data), &nvkey_item_size);
    if ((nvkey_status != NVKEY_STATUS_OK) || (nvkey_item_size != 4) || (syslog_nvkey_data.magic_number != 0x5A)) {
        SYSLOG_MSGID_E("Logging: Read NVKEY status=%d size=%d magic_num=0x%02X", 3, nvkey_status, nvkey_item_size, syslog_nvkey_data.magic_number);
        return false;
    }

    SYSLOG_MSGID_I("Logging: Read NVKEY, status=%d [0]=0x%x [1]=0x%x [2]=0x%x [3]=0x%02x", 5, nvkey_status,
                   syslog_nvkey_data.syslog_port, syslog_nvkey_data.syslog_level, syslog_nvkey_data.baudrate, syslog_nvkey_data.magic_number);

    g_cpu_level[0] = syslog_nvkey_data.syslog_level.cpu0_level;
    g_cpu_level[1] = syslog_nvkey_data.syslog_level.cpu1_level;

    if (syslog_nvkey_data.baudrate.high_br_enable != 0) {
        // TODO: 1wire not support high baudrate.
#ifdef HAL_UART_FEATURE_6M_BAUDRATE
        g_uart_baudrate = HAL_UART_BAUDRATE_6000000;
        SYSLOG_MSGID_I("Logging: Read NVKEY Logging uart enabe 6M baudrate", 0);
#endif
    }

    /* 1wire setting init */
#if defined (AIR_1WIRE_ENABLE)
    smchg_nvkey_init();
    smchg_1wire_gpio_init(); //1wire gpio config

    if (smchg_cfg.one_wire_log) {
        syslog_config_one_wire_log_active();
        if ((smchg_cfg.uart_sel == SMCHG_UART0_2GPIO) || (smchg_cfg.uart_sel == SMCHG_UART0_1GPIO)) {
            SYSLOG_MSGID_I("1wire, log UART0", 0);
            syslog_nvkey_data.syslog_port.port = 0; //UART0
        } else {
            SYSLOG_MSGID_I("1wire, log UART1", 0);
            syslog_nvkey_data.syslog_port.port = 1; //UART1
        }
    }
#endif

    /* load syslog port parameter*/
    syslog_port = syslog_nvkey_data.syslog_port.port;

    if (syslog_port == 0) {
        *port = MUX_UART_0;
        SYSLOG_MSGID_I("Logging: Read NVKEY Logging port: UART_0", 0);
    } else if (syslog_port == 1) {
        *port = MUX_UART_1;
        SYSLOG_MSGID_I("Logging: Read NVKEY Logging port: UART_1", 0);
    } else if (syslog_port == 2) {
        *port = MUX_UART_2;
        SYSLOG_MSGID_I("Logging: Read NVKEY Logging port: UART_2", 0);
    } else if (syslog_port == 3) {
        *port = MUX_USB_COM_1;
        SYSLOG_MSGID_I("Logging: Read NVKEY Logging port: MUX_USB_COM_1/USB_CDC_1", 0);
    } else if (syslog_port == 4) {
        *port = MUX_USB_COM_2;
        SYSLOG_MSGID_I("Logging: Read NVKEY Logging port: MUX_USB_COM_2/USB_CDC_2", 0);
    } else if (syslog_port == 5) {
        *port = MUX_AIRAPP_0;
        SYSLOG_MSGID_I("Logging: Read NVKEY Logging port: AIR_SPP", 0);
    } else if (syslog_port == 6) {
        /* reserve for flash port */
        // *port = MUX_FLASH;
        SYSLOG_MSGID_W("Logging: Read NVKEY Logging port: MUX_FLASH, no use", 0);
        return false;
    } else if (syslog_port == 7) {
        *port = MUX_USB_COM_2;
        SYSLOG_MSGID_I("Logging: Read NVKEY Logging port: MUX_USB_COM_2/USB_HID", 0);
    } else {
        return false;
    }

    return true;
}
#endif

uint32_t syslog_port_init(uint32_t port)
{
    bool nvkey_status = false;
    uint32_t syslog_port;
    mux_status_t status;
    mux_port_setting_t syslog_setting;
    mux_protocol_t protocol_callback;
    mux_port_buffer_t query_port_buffer;

    syslog_port = port;
    SYSLOG_MSGID_I("Logging: Default runtime log port:%d", 1, syslog_port);

#if defined(MTK_NVDM_ENABLE) && !defined(AG3335)
    nvkey_status = syslog_port_form_nvkey_init(&syslog_port);
    SYSLOG_MSGID_I("Logging: Read nvkey runtime log port:%d", 1, syslog_port);
#endif /* MTK_NVDM_ENABLE && !AG3335 */

    /* default syslog port setting */
    syslog_setting.tx_buffer_size = AIR_SYSLOG_TX_BUFFER_SIZE;
    syslog_setting.rx_buffer_size = AIR_SYSLOG_RX_BUFFER_SIZE;
    syslog_setting.dev_setting.uart.uart_config.baudrate    = syslog_port_query_uart_baudrate();
    syslog_setting.dev_setting.uart.uart_config.word_length = HAL_UART_WORD_LENGTH_8;
    syslog_setting.dev_setting.uart.uart_config.stop_bit    = HAL_UART_STOP_BIT_1;
    syslog_setting.dev_setting.uart.uart_config.parity      = HAL_UART_PARITY_NONE;
    syslog_setting.dev_setting.uart.flowcontrol_type        = MUX_UART_SW_FLOWCONTROL;
    syslog_setting.portLinkRegAddr = 0x0;

    if ((nvkey_status == true) && (syslog_port == MUX_AIRAPP_0)) {
        SYSLOG_MSGID_I("Logging: nvkey = MUX_AIRAPP_0", 0);
        status = mux_query_port_numbers_from_nvdm("SYSLOG", (mux_port_buffer_t *)&query_port_buffer);
        if ((status == MUX_STATUS_OK) && (query_port_buffer.count == 1)) {
            SYSLOG_MSGID_I("Logging: query syslog port from nvdm ok, port=%d", 1, query_port_buffer.buf[0]);
            if (query_port_buffer.buf[0] != MUX_AIRAPP_0) {
                mux_close_delete_from_nvdm(query_port_buffer.buf[0], "SYSLOG");
            }
        }
    } else {
        query_port_buffer.count = 0;
        status = mux_query_user_port_numbers_from_nvdm("SYSLOG", (uint32_t *) & (query_port_buffer.count));
        SYSLOG_MSGID_I("Logging: query syslog port from nvdm, status:%d quantity:%d", 2, status, query_port_buffer.count);
        if ((status == MUX_STATUS_OK) && (query_port_buffer.count > 1)) {
            SYSLOG_MSGID_I("Logging: syslog port must <= 1", 0);
            assert(0); // syslog port must <= 1
        }
        status = mux_query_port_numbers_from_nvdm("SYSLOG", (mux_port_buffer_t *)&query_port_buffer);
        if ((status == MUX_STATUS_OK) && (query_port_buffer.count == 1)) {
            SYSLOG_MSGID_I("Logging: query syslog port from nvdm ok, port=%d", 1, query_port_buffer.buf[0]);
            if (syslog_query_one_wire_log_active() == true) {
                if (query_port_buffer.buf[0] != syslog_port) {
                    mux_close_delete_from_nvdm(query_port_buffer.buf[0], "SYSLOG");
                }
                SYSLOG_MSGID_I("Logging: one-wire log is avtive, use 1-wire port=%d", 1, syslog_port);
            } else {
                syslog_port = query_port_buffer.buf[0];
                SYSLOG_MSGID_I("Logging: one-wire is invalid, use nvdm port=%d", 1, syslog_port);
            }
        } else {
            SYSLOG_MSGID_I("Logging: query syslog port from nvdm fail, use default port=%d", 1, syslog_port);
        }
    }

    if ((syslog_query_one_wire_log_active() == true) && ((syslog_port == MUX_UART_1) || (syslog_port == MUX_UART_0))) {
#if defined (AIR_1WIRE_ENABLE)
        syslog_setting.dev_setting.uart.flowcontrol_type = MUX_UART_NONE_FLOWCONTROL;
        protocol_callback.rx_protocol_callback = smchg_1wire_mux_rx_protocol_cb;
        protocol_callback.tx_protocol_callback = smchg_1wire_mux_tx_protocol_cb;
#endif
        protocol_callback.user_data = NULL;
    } else {
        if (syslog_port == MUX_AIRAPP_0) {
            protocol_callback.rx_protocol_callback = NULL;
            protocol_callback.tx_protocol_callback = NULL;
            protocol_callback.user_data = NULL;
        } else {
            protocol_callback.rx_protocol_callback = race_rx_protocol_callback; //race_rx_no_packed_callback;
            protocol_callback.tx_protocol_callback = race_tx_protocol_callback; //race_tx_no_packed_callback;
            protocol_callback.user_data = NULL;
        }
    }

    SYSLOG_MSGID_I("Logging: begin mux_init syslog port:%d tx_buf_size:%d rx_buf_size:%d", 3,
                   syslog_port, syslog_setting.tx_buffer_size, syslog_setting.rx_buffer_size);

    /* mask log port uart irq */
    if (syslog_port < MUX_UART_END) {
        syslog_port_uart_disable_irq(syslog_port);
    }

    if ((syslog_port == MUX_USB_COM_1) || (syslog_port == MUX_USB_COM_2)) {
        syslog_debug_uart_init(HAL_UART_0);
        debug_printf("syslog debug uart init port 0 ");
    } else if (syslog_query_one_wire_log_active() == true) {
        if (syslog_port == 0) {
            syslog_debug_uart_init(HAL_UART_1);
            debug_printf("syslog debug uart init port 1 ");
        } else if (syslog_port == 1) {
            syslog_debug_uart_init(HAL_UART_0);
            debug_printf("syslog debug uart init port 0 ");
        }
    }

    status = mux_init(syslog_port, &syslog_setting, &protocol_callback);

    if ((syslog_port != MUX_AIRAPP_0) && (syslog_query_one_wire_log_active() == false)) {
        mux_open_save_to_nvdm(syslog_port, "SYSLOG");
    }

    return syslog_port;
}

void syslog_port_1wire_port_init(uint32_t syslog_port)
{
#if defined (AIR_1WIRE_ENABLE)
    /* check charger 5v exist */
    if (smchg_1wire_chg_exist()) {
        smchg_1wire_set_mode_status(SMCHG_1WIRE_CHG);
        mux_control(syslog_port, MUX_CMD_UART_TX_RX_DISABLE, NULL);
    } else {
        mux_ctrl_para_t port_config_parm;
        smchg_1wire_set_mode_status(SMCHG_1WIRE_LOG);
        port_config_parm.mux_set_config_uart_param.baudrate = smchg_cfg.log_mode_baud_rate;
        mux_control(syslog_port, MUX_CMD_CHANGE_UART_PARAM, &port_config_parm);
        mux_control(syslog_port, MUX_CMD_UART_TX_ENABLE, NULL);
    }
    hal_uart_disable_flowcontrol(syslog_port);
#else
    SYSLOG_PORT_UNUSED(syslog_port);
#endif
}

void syslog_port_post_init(uint32_t port, uint32_t *handle, void *user_cb_handler)
{
    mux_status_t status;
    if (port == MUX_AIRAPP_0) {
        status = mux_open(port, "SYSLOG", (mux_handle_t *)handle, NULL, NULL);
        if (status != MUX_STATUS_OK) {
            assert(0);
        }
    } else {
        status = mux_open(port, "SYSLOG", (mux_handle_t *)handle, user_cb_handler, NULL);
        if (status != MUX_STATUS_OK) {
            assert(0);
        }
    }
}

#if !defined(MTK_DEBUG_LEVEL_NONE)
#if !defined(MTK_DEBUG_LEVEL_PRINTF)
/* For printf() */
int __wrap_printf(const char *format, ...)
{
    va_list ap;

    /* module fun line level format ap */
    va_start(ap, format);

#ifdef MTK_DEBUG_PLAIN_LOG_ENABLE
    vprint_module_log(&LOG_CONTROL_BLOCK_SYMBOL(printf), NULL, 0, 0, format, ap);
#else
    const char *fun = __FUNCTION__;
    vprint_module_log(&LOG_CONTROL_BLOCK_SYMBOL(printf), fun, __LINE__, PRINT_LEVEL_INFO, format, ap);
#endif

    va_end(ap);

    return 0;
}
#else /* !MTK_DEBUG_LEVEL_PRINTF */
int __wrap_printf(const char *format, ...)
{
    va_list ap;
    int32_t log_size;
    uint32_t irq_mask;
    char frame_header[PORT_SYSLOG_MAX_ONE_PLAIN_LOG_SIZE];

    va_start(ap, format);
    log_size = vsnprintf(frame_header, sizeof(frame_header), format, ap);
    if (log_size < 0) {
        return -1;
    }
    if ((uint32_t)log_size >= (sizeof(frame_header))) {
        log_size = sizeof(frame_header) - 1;
    }
    va_end(ap);

    syslog_port_nvic_save_and_set_interrupt_mask(&irq_mask);
#ifndef MTK_SINGLE_CPU_ENV
    while (hal_hw_semaphore_take(HW_SEMAPHORE_SYSLOG) != HAL_HW_SEMAPHORE_STATUS_OK);
#endif
    syslog_port_uart_send_polling(query_syslog_port(), (uint8_t *)frame_header, log_size);
#ifndef MTK_SINGLE_CPU_ENV
    while (hal_hw_semaphore_give(HW_SEMAPHORE_SYSLOG) != HAL_HW_SEMAPHORE_STATUS_OK);
#endif
    syslog_port_nvic_restore_interrupt_mask(irq_mask);

    return log_size;
}
#endif /* MTK_DEBUG_LEVEL_PRINTF */
#else  /* !MTK_DEBUG_LEVEL_NONE */
int __wrap_printf(const char *format, ...)
{
    SYSLOG_PORT_UNUSED(format);

    return 0;
}
#endif /* MTK_DEBUG_LEVEL_NONE */
