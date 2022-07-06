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

#ifndef __SYSLOG_PORT_H__
#define __SYSLOG_PORT_H__

#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#include "syslog.h"

extern uint32_t PORT_SYSLOG_SHARE_VARIABLE_BEGIN;
extern uint32_t PORT_SYSLOG_SDK_VERSION_BEGIN;
extern uint32_t PORT_SYSLOG_BUILD_TIME_BEGIN;
extern uint32_t PORT_SYSLOG_INIT_STAGE_PORT;
extern uint32_t PORT_SYSLOG_RUNNING_STAGE_PORT;
extern uint32_t PORT_SYSLOG_EXCEPTION_NUMBER;

extern print_level_t g_cpu_level[2];

typedef struct {
    uint8_t *p_buf;     /**<  syslog buffer start address*/
    uint32_t buf_size;  /**<  syslog buffer lenght*/
} syslog_buffer_t;

/* for syslog debug */
#define SYSLOG_I(fmt, arg...)               syslog_port_log_info(fmt, ##arg)
#define SYSLOG_W(fmt, arg...)               syslog_port_log_warning(fmt, ##arg)
#define SYSLOG_E(fmt, arg...)               syslog_port_log_error(fmt, ##arg)
#define SYSLOG_MSGID_I(fmt, cnt, arg...)    syslog_port_log_info(fmt, ##arg); //syslog_port_log_msgid_info(fmt, cnt, ##arg)
#define SYSLOG_MSGID_W(fmt, cnt, arg...)    syslog_port_log_warning(fmt, ##arg) //syslog_port_log_msgid_warning(fmt, cnt, ##arg)
#define SYSLOG_MSGID_E(fmt, cnt, arg...)    syslog_port_log_error(fmt, ##arg); //syslog_port_log_msgid_error(fmt, cnt, ##arg)

/* syslog must internal API */
extern uint32_t query_syslog_port(void);
extern bool syslog_query_one_wire_log_active(void);
extern void syslog_config_one_wire_log_active(void);
extern void race_tx_protocol_callback();
extern void race_rx_protocol_callback();
/* syslog self debug log API */
void syslog_port_log_info(const char *message, ...);
void syslog_port_log_warning(const char *message, ...);
void syslog_port_log_error(const char *message, ...);
void syslog_port_log_msgid_info(const char *message, uint32_t arg_cnt, ...);
void syslog_port_log_msgid_warning(const char *message, uint32_t arg_cnt, ...);
void syslog_port_log_msgid_error(const char *message, uint32_t arg_cnt, ...);
/* necessary parameter */
uint32_t syslog_port_query_exception_number(void);
uint32_t syslog_port_query_os_task_number(void);
uint32_t syslog_port_query_nest_count(uint32_t handle);
bool syslog_port_query_user_name(uint32_t handle, const char **user_name);
bool syslog_port_user_handle(uint32_t port, const char *user_name, uint32_t *p_handle);
uint32_t syslog_port_query_current_timestamp(void);
uint32_t syslog_port_memview_dsp0_to_mcu(uint32_t address);
uint32_t syslog_port_query_uart_baudrate(void);
bool syslog_port_uart_is_xoff(uint32_t port);
void syslog_port_uart_disable_flowcontrol(uint32_t port);
void syslog_port_uart_disable_irq(uint32_t port);
void syslog_port_uart_send_polling(uint32_t port, const uint8_t *data, uint32_t size);
int32_t syslog_port_nvic_restore_interrupt_mask(uint32_t mask);
int32_t syslog_port_nvic_save_and_set_interrupt_mask(uint32_t *mask);
uint32_t syslog_port_parse_port_number(syslog_port_type_t port_type, uint8_t port_index);
void *syslog_port_malloc(uint32_t size);
void syslog_port_free(void *pdata);
uint32_t syslog_port_read_setting(char *name, uint8_t *p_setting, uint32_t size);
bool syslog_port_save_setting(char *name, uint8_t *p_setting, uint32_t size);
/* tool command */
uint32_t syslog_port_receive(uint32_t handle, uint8_t *p_buf, uint32_t size);
bool syslog_port_is_spp_port(uint32_t port);
bool syslog_port_is_usb_port(uint32_t port);
bool syslog_port_cmd_event_is_vaild(uint8_t event);
void syslog_port_send_daemo_message(void);
void syslog_port_send_daemo_message_assert(void);
/* syslog init */
void syslog_port_early_init(void);
uint32_t syslog_port_init(uint32_t port);
void syslog_port_post_init(uint32_t port, uint32_t *handle, void *user_cb_handler);
void syslog_port_1wire_port_init(uint32_t syslog_port);
bool syslog_port_mux_exception_init(uint32_t handle);
void syslog_port_excetpion_1wire_check(uint32_t port);
bool syslog_port_core_is_exception(void);
bool syslog_port_exception_send(uint32_t handle, uint8_t *buffers, uint32_t buffers_counter);
bool syslog_port_mux_tx(uint32_t handle, uint8_t *buffers, uint32_t buffers_counter, uint32_t *send_done_data_len);
uint32_t syslog_port_cmd_query_tx_buffer_free(uint32_t port);
void syslog_port_cmd_clean_tx_buffer(uint32_t port);


#endif

