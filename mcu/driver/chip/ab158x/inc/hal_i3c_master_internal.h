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

#include "hal.h"
#include "hal_platform.h"

#ifndef __HAL_I3C_MASTER_INTERNAL_H__
#define __HAL_I3C_MASTER_INTERNAL_H__

#include "hal_i3c_master.h"

#ifdef HAL_I3C_MASTER_MODULE_ENABLED

/* Define printf for FPGA DVT logging */
#define     I3C_PLAIN_LOG_ENABLE

#ifdef HAL_NVIC_MODULE_ENABLED
#define     I3C_NVIC_ENABLED
#endif

#ifdef I3C_PLAIN_LOG_ENABLE
    #define log_hal_i3c_info(_message, cnt, ...)        printf(_message, ##__VA_ARGS__)
    #define log_hal_i3c_warning(_message, cnt, ...)     printf(_message, ##__VA_ARGS__)
    #define log_hal_i3c_error(_message, cnt, ...)       printf(_message, ##__VA_ARGS__)
#else
    #define log_hal_i3c_info(_message, cnt, ...)        log_hal_msgid_info(_message,cnt,    ##__VA_ARGS__)
    #define log_hal_i3c_warning(_message, cnt, ...)     log_hal_msgid_warning(_message,cnt, ##__VA_ARGS__)
    #define log_hal_i3c_error(_message,cnt, ...)        log_hal_msgid_error(_message,cnt,   ##__VA_ARGS__)
#endif

#define     I3C_SPECIAL_ADDRESS_7E      0x7E
/////////////////////////////////////////////////////////////////////////////////////////////////////
typedef enum {
    I3C_FIFO_WRITE = 0,
    I3C_FIFO_READ,
    I3C_FIFO_CLEAR
} i3c_fifo_op_t;

typedef enum {
    I3C_IO_SCL = 0,
    I3C_IO_SDA,
} hal_i3c_io_t;

typedef enum{
    I3C_IOCTL_START = 0,
    I3C_IOCTL_GET_BUSY_STAT,
    I3C_IOCTL_GET_IRQ_STAT,
    I3C_IOCTL_CLR_IRQ_STAT,
    I3C_IOCTL_SET_IRQ_MASK,
    I3C_IOCTL_SET_SLV_RADDR,
    I3C_IOCTL_SET_SLV_WADDR,
    I3C_IOCTL_GET_SLV_ADDR,
    I3C_IOCTL_GET_FIFO_STAT,
    I3C_IOCTL_GET_ERROR_STAT,
    I3C_IOCTL_ENABLE_CLOCK,
    I3C_IOCTL_SET_DEF_ADDR,
    I3C_IOCTL_GET_ERR_ADDR,
    I3C_IOCTL_LOCK_SLEEP,
}i3c_ioctl_cmd_t;

enum {
    I3C_OP_STATE_INIT_MASK = (1 << 0),
};

enum {
    I3C_TRANSFER_TYPE_SEND_TO_RECEIVE = 0,
    I3C_TRANSFER_TYPE_SEND,
    I3C_TRANSFER_TYPE_RECEIVE,
    I3C_TRANSFER_TYPE_DAA
};

typedef struct {
    volatile uint32_t           seq_id;
    uint32_t                    op_state;
    hal_i3c_callback_t          call_back;
    void                       *user_data;
    hal_i3c_transfer_config_t   cfg_buff[6];
} i3c_private_info_t;



typedef struct {
    int      head;
    int      tail;
    uint8_t *buff;
    uint32_t sz_buff;
}air_queue_t;

typedef enum {
    QUEUE_STATUS_OK                 =  0,
    QUEUE_STATUS_ERROR_INVALID_PARA = -1,
    QUEUE_STATUS_ERROR_FULL         = -2,
    QUEUE_STATUS_ERROR_EMPTY        = -3,
    QUEUE_STATUS_ERROR_NOT_ENOUGH         = -4,
}airo_queue_status_t;

enum {
    I3C_DA_STATE_IDLE = 0,
    I3C_DA_STATE_DAA_1ST = 1,
    I3C_DA_STATE_DAA_2ND,
    I3C_DA_STATE_NONE
};


/* For I3C API*/
//uint32_t                i3c_enum2num(hal_i2c_frequency_t freq);
hal_i3c_master_status_t i3c_config_i2c_speed(hal_i3c_master_port_t  i3c_port, uint32_t i3c_speed, uint32_t i3c_hs_speed);
hal_i3c_master_status_t i3c_config_i3c_speed(hal_i3c_master_port_t  i3c_port, uint32_t i3c_speed, uint32_t i3c_hs_speed);
hal_i3c_master_status_t i3c_config_transfer(hal_i3c_master_port_t  i3c_port, hal_i3c_transfer_config_t *config);
void                    i3c_config_timing(hal_i3c_master_port_t  i2c_port, uint32_t t_min_timing_ns);
void                    i3c_config_io(hal_i3c_master_port_t  i2c_port, hal_i3c_io_t io, hal_i3c_io_config_t config);
void                    i3c_config_start(hal_i3c_master_port_t  i3c_port, uint32_t option);
int                     i3c_config_fifo(hal_i3c_master_port_t  i2c_port, i3c_fifo_op_t  op, uint8_t *buff, uint32_t size);
void                    i3c_hardware_reset(hal_i3c_master_port_t  i2c_port);
void                    i3c_config_irq(hal_i3c_master_port_t  i3c_port, hal_nvic_isr_t callback);
int                     i3c_opt_ioctl(hal_i3c_master_port_t  i3c_port, uint32_t cmd, uint32_t option);
int                     i3c_get_port_by_nvic_id(hal_nvic_irq_t irq_number);
void                    i3c_dump_register(hal_i3c_master_port_t  i2c_port);


int                     air_queue_init(air_queue_t *pque,  uint8_t *buff, uint32_t size);
int                     air_queue_push(air_queue_t *pque,  uint8_t *buff, uint32_t size);
int                     air_queue_pop(air_queue_t *pque,    uint8_t *buff, uint32_t size);
int                     air_queue_top(air_queue_t *pque,    uint8_t *buff, uint32_t size);



#endif /* HAL_I3C_MODULE_ENBALED */
#endif /* __HAL_I3C_MASTER_INTERNAL_H__ */


