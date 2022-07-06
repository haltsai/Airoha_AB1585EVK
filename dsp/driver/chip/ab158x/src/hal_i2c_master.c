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

/* Includes ------------------------------------------------------------------*/
#include "hal.h"
#include "hal_platform.h"
#include "hal_i2c_master.h"
#if defined(HAL_I2C_MASTER_MODULE_ENABLED)
#include "hal_clock.h"
#include "hal_i2c_master_internal.h"


static i2c_private_info_t  s_priv_info[HAL_I2C_MASTER_MAX];


static hal_i2c_status_t    _hal_i2c_start_transfer(hal_i2c_port_t i2c_port, i2c_transfer_config_t *config)
{
    /* clear status & fifo */
    i2c_op_ioctl(i2c_port, I2C_IOCTRL_CLR_IRQ_STAT, 0);
    i2c_config_fifo(i2c_port, I2C_FIFO_OP_CLR, 0, 0);

    /* config transfer */
    if (config->speed != HAL_I2C_FREQUENCY_MAX) {
        i2c_config_speed(i2c_port, i2c_speed_enum_to_dec(config->speed), 0);
    }
    if (config->trans_mode == I2C_TRANSFER_MODE_DMA) {
        int ret = 0;
        i2c_pdma_deinit(i2c_port);
        if ((ret = i2c_pdma_init(i2c_port, config)) < 0) {
            log_hal_msgid_error("[hal][i2c%d] start transfer fail: pdma err(%d) ", 2, i2c_port, ret);
            return HAL_I2C_STATUS_ERROR;
        }
    } else {
        if (config->trans_type == I2C_TRANSFER_TYPE_TX_RX || config->trans_type == I2C_TRANSFER_TYPE_TX) {
            i2c_config_fifo(i2c_port, I2C_FIFO_OP_WR, config->send_buff, config->send_size); /* write fifo if trans type is tx */
        }
    }
    i2c_config_transfer(i2c_port, config);

    /* start transfer */
    i2c_op_ioctl(i2c_port, I2C_IOCTRL_START, 0);
    return HAL_I2C_STATUS_OK;
}


static void    _hal_i2c_interrupt_handle(hal_nvic_irq_t irq_number)
{
    uint32_t  temp = 0;
    i2c_transfer_config_t  *pconfig = NULL;
    i2c_private_info_t     *priv_info = NULL;
    hal_i2c_port_t          i2c_port;
    i2c_irq_status_t        irq_status;
    int                     q_stat;
    uint8_t                 slv_addr = 0;

    i2c_port = i2c_get_port_by_nvic_id(irq_number);
    if (i2c_port >= HAL_I2C_MASTER_MAX) {
        log_hal_msgid_error("[hal][i2c%d] irq handle err: port invalid", 1, i2c_port);
        return;
    }

    /* process cureent transfer */
    priv_info = &s_priv_info[i2c_port];
    irq_status = i2c_get_irq_status(i2c_port);
    slv_addr   = i2c_op_ioctl(i2c_port, I2C_IOCTRL_GET_SLV_ADDR, 0);
    if ((q_stat = queue_pop(&(priv_info->i2c_queue), (int *)&temp)) < 0) {
        log_hal_msgid_error("[hal][i2c%d] irq handle err: pop q fail(%d)", 1, q_stat);
        return;
    }
    pconfig = (i2c_transfer_config_t *)temp;
    i2c_op_ioctl(i2c_port, I2C_IOCTRL_CLR_IRQ_STAT, 0);
    log_hal_msgid_info("[hal][i2c%d] irq handle occured, irq status %x", 2, i2c_port, irq_status);
    /* call user callback */
    if (pconfig->call_back) {
        pconfig->call_back(pconfig->slv_addr, irq_status, pconfig->user_data);
    } else if (priv_info->call_back) {
        priv_info->call_back(slv_addr, irq_status, priv_info->user_data);
    }
    pconfig->priv_data &= ~(I2C_ITEM_FLAG_IS_USED); ; /* clear item flag */
    hal_gpt_delay_us(50);
    /* if queue is not empty then start next transfer, else deinit pdma and unlock sleep */
    if (queue_top(&(priv_info->i2c_queue), (int *)&temp) < 0) {
        /* clear hw busy flag */
        priv_info->hw_state &= ~I2C_HW_STATE_DEV_BUSY_MASK;
        /* deinit dma if no tranfser need process*/
        if (pconfig->trans_mode == I2C_TRANSFER_MODE_DMA) {
            i2c_pdma_deinit(i2c_port);
        }
        /* if user call non-block api, need deinit hw */
        if ((priv_info->hw_state & I2C_HW_STATE_USED_MASK) == I2C_HW_STATE_USED_NB) {
            hal_i2c_master_deinit(i2c_port);
        }
        i2c_op_ioctl(i2c_port, I2C_IOCTRL_LOCK_SLEEP, false);
    } else {
        pconfig    = (i2c_transfer_config_t *)temp;
        _hal_i2c_start_transfer(i2c_port, pconfig);
    }
}





static hal_i2c_status_t _hal_i2c_master_send_and_receive_dma_with_nb(hal_i2c_port_t i2c_port, hal_i2c_send_to_receive_config_ex_no_busy_t *config)
{
    uint32_t                i = 0;
    int                     temp = 0;
    i2c_private_info_t     *priv_info = NULL;
    i2c_transfer_config_t  *pconfig = NULL;
    hal_i2c_status_t        status  = HAL_I2C_STATUS_OK;

    /* step 0: parameter check & init para */
    if (HAL_I2C_MASTER_MAX <= i2c_port) {
        return HAL_I2C_STATUS_INVALID_PORT_NUMBER;
    }
    if (NULL == config || (config->i2c_send_to_receive_config_ex.receive_bytes_in_one_packet == 0 &&
                           config->i2c_send_to_receive_config_ex.send_bytes_in_one_packet == 0)) {
        return HAL_I2C_STATUS_INVALID_PARAMETER;
    }

    /*step 1: find free item */
    i2c_op_ioctl(i2c_port, I2C_IOCTRL_DISABLE_IRQ, true);
    priv_info = &s_priv_info[i2c_port];
    for (i = 0; i < MAX_FIFO_SIZE; i++) {
        if (priv_info->cfg_item[i].priv_data & I2C_ITEM_FLAG_IS_USED) {
            continue;
        } else {
            priv_info->cfg_item[i].priv_data |= I2C_ITEM_FLAG_IS_USED;
            pconfig = &(priv_info->cfg_item[i]);
            break;
        }
    }
    i2c_op_ioctl(i2c_port, I2C_IOCTRL_DISABLE_IRQ, false);

    if (pconfig == NULL) {
        log_hal_msgid_error("[hal][i2c%d] dma send recv err 0: none item avail", 0);
        return HAL_I2C_STATUS_ERROR;
    }
    i2c_op_ioctl(i2c_port, I2C_IOCTRL_LOCK_SLEEP, true); /* lock sleep */

    /* step 2: initial i2c transfer config parameter */
    pconfig->speed      = config->i2c_config.frequency;
    pconfig->slv_addr   = config->i2c_send_to_receive_config_ex.slave_address;
    pconfig->trans_mode = I2C_TRANSFER_MODE_DMA;
    pconfig->send_buff  = (uint8_t *)config->i2c_send_to_receive_config_ex.send_data;
    pconfig->send_size  = config->i2c_send_to_receive_config_ex.send_bytes_in_one_packet;
    pconfig->recv_buff  = config->i2c_send_to_receive_config_ex.receive_buffer;
    pconfig->recv_size  = config->i2c_send_to_receive_config_ex.receive_bytes_in_one_packet;
    pconfig->send_pack_size = config->i2c_send_to_receive_config_ex.send_packet_length;
    pconfig->recv_pack_size = config->i2c_send_to_receive_config_ex.receive_packet_length;
    pconfig->call_back   = config->i2c_callback;
    pconfig->user_data  = config->user_data;
    if (pconfig->recv_size && pconfig->send_size) {
        pconfig->trans_type = I2C_TRANSFER_TYPE_TX_RX;
    } else if (pconfig->recv_size == 0) {
        pconfig->trans_type = I2C_TRANSFER_TYPE_TX;
    } else {
        pconfig->trans_type = I2C_TRANSFER_TYPE_RX;
    }

    /* step 3: if hw is busy then push config to queue, else start transfer */
    i2c_op_ioctl(i2c_port, I2C_IOCTRL_DISABLE_IRQ, true);
    if ((temp = queue_push(&priv_info->i2c_queue, (uint32_t) pconfig))  < 0) {
        log_hal_msgid_error("[hal][i2c%d] dma send recv err: q push fail(%d)", 1, (int) temp);
        status = HAL_I2C_STATUS_ERROR;
    } else if (i2c_op_ioctl(i2c_port, I2C_IOCTRL_GET_BUSY_STAT, 0) == 0 && (priv_info->hw_state & I2C_HW_STATE_DEV_BUSY_MASK) == 0) {
        i2c_config_irq(i2c_port, _hal_i2c_interrupt_handle);
        status =  _hal_i2c_start_transfer(i2c_port, pconfig);
        priv_info->hw_state |= I2C_HW_STATE_DEV_BUSY_MASK;
    }
    i2c_op_ioctl(i2c_port, I2C_IOCTRL_DISABLE_IRQ, false);
    return status;
}


#ifdef HAL_SLEEP_MANAGER_ENABLED
static  void    hal_i2c_backup_all_register()
{
    i2c_private_info_t     *priv_info = NULL;
    uint32_t                i2c_port = 0;

    for (i2c_port = 0; i2c_port < HAL_I2C_MASTER_MAX; i2c_port++) {
        priv_info  = &s_priv_info[i2c_port];
        if (priv_info->hw_state & I2C_HW_STATE_USED_MASK) {
            i2c_sleep_register_backup(i2c_port, (uint32_t *) &priv_info->i2c_queue.data, MAX_FIFO_SIZE);
        }
    }
}

static void  hal_i2c_restore_all_register()
{
    i2c_private_info_t     *priv_info = NULL;
    uint32_t                i2c_port = 0;

    for (i2c_port = 0; i2c_port < HAL_I2C_MASTER_MAX; i2c_port++) {
        priv_info  = &s_priv_info[i2c_port];
        if (priv_info->hw_state & I2C_HW_STATE_USED_MASK) {
            i2c_sleep_register_restore(i2c_port, (uint32_t *) &priv_info->i2c_queue.data, MAX_FIFO_SIZE);
        }
        memset(&priv_info->i2c_queue, 0, sizeof(airo_queue_t));
    }
}
#endif

hal_i2c_status_t    hal_i2c_master_init(hal_i2c_port_t i2c_port, hal_i2c_config_t *i2c_config)
{
    static bool initialized = false;
    uint32_t saved_mask;


    if (initialized == false) {
        memset(s_priv_info, 0, sizeof(s_priv_info));
        initialized = true;
    }
    /* parameter check */
    if (HAL_I2C_MASTER_MAX <= i2c_port) {
        log_hal_msgid_error("[hal][i2c%d] init: port invalid", 1, i2c_port);
        return HAL_I2C_STATUS_INVALID_PORT_NUMBER;
    }
    if (HAL_I2C_FREQUENCY_MAX <= i2c_config->frequency) {
        log_hal_msgid_error("[hal][i2c%d] init: para invalid", 1, i2c_port);
        return HAL_I2C_STATUS_INVALID_PARAMETER;
    }
    hal_nvic_save_and_set_interrupt_mask(&saved_mask);
    if (s_priv_info[i2c_port].hw_state & I2C_HW_STATE_USED_MASK) {
        log_hal_msgid_error("[hal][i2c%d] init: dev busy", 1, i2c_port);
        hal_nvic_restore_interrupt_mask(saved_mask);
        return HAL_I2C_STATUS_ERROR;
    }
    s_priv_info[i2c_port].hw_state = (I2C_HW_STATE_USED_NORMAL << I2C_HW_STATE_USED_OFFSET);
    i2c_op_ioctl(i2c_port, I2C_IOCTRL_ENABLE_CLOCK, true);
    i2c_config_io(i2c_port, true);
    i2c_config_speed(i2c_port, i2c_speed_enum_to_dec(i2c_config->frequency), 0);
    hal_nvic_restore_interrupt_mask(saved_mask);

#ifdef HAL_SLEEP_MANAGER_ENABLED
    sleep_management_register_suspend_callback(SLEEP_BACKUP_RESTORE_I2C1, (sleep_management_suspend_callback_t)hal_i2c_backup_all_register, NULL);
    sleep_management_register_resume_callback(SLEEP_BACKUP_RESTORE_I2C1, (sleep_management_resume_callback_t) hal_i2c_restore_all_register, NULL);
#endif
    queue_init(&(s_priv_info[i2c_port].i2c_queue));
    return HAL_I2C_STATUS_OK;
}


hal_i2c_status_t hal_i2c_master_set_io_config(hal_i2c_port_t i2c_port, hal_i2c_io_config_t io)
{
    i2c_private_info_t     *priv_info = NULL;

    /* parameter check */
    if (HAL_I2C_MASTER_MAX <= i2c_port) {
        return HAL_I2C_STATUS_INVALID_PORT_NUMBER;
    }

    priv_info  = &s_priv_info[i2c_port];
    priv_info->hw_state &= ~(I2C_HW_STATE_IO_CFG_MASK);
    if (io == HAL_I2C_IO_OPEN_DRAIN) {
        i2c_config_io(i2c_port, true);
    } else {
        i2c_config_io(i2c_port, false);
        priv_info->hw_state |= I2C_HW_STATE_IO_PUSHPULL;
    }
    return HAL_I2C_STATUS_OK;
}


hal_i2c_status_t hal_i2c_master_register_callback(hal_i2c_port_t i2c_port, hal_i2c_callback_t i2c_callback, void *user_data)
{
    i2c_private_info_t     *priv_info = NULL;

    /* parameter check */
    if (HAL_I2C_MASTER_MAX <= i2c_port) {
        return HAL_I2C_STATUS_INVALID_PORT_NUMBER;
    }
    priv_info  = &s_priv_info[i2c_port];
    priv_info->call_back = i2c_callback;
    priv_info->user_data = user_data;
    return HAL_I2C_STATUS_OK;
}


hal_i2c_status_t    hal_i2c_master_deinit(hal_i2c_port_t i2c_port)
{
    uint32_t saved_mask;

    /* parameter check */
    if (HAL_I2C_MASTER_MAX <= i2c_port) {
        return HAL_I2C_STATUS_INVALID_PORT_NUMBER;
    }
    hal_nvic_save_and_set_interrupt_mask(&saved_mask);
    s_priv_info[i2c_port].hw_state = 0;
    i2c_op_ioctl(i2c_port, I2C_IOCTRL_ENABLE_CLOCK, false);
    hal_nvic_restore_interrupt_mask(saved_mask);
    return HAL_I2C_STATUS_OK;
}



hal_i2c_status_t    hal_i2c_master_send_to_receive_polling(hal_i2c_port_t i2c_port, hal_i2c_send_to_receive_config_t *i2c_send_to_receive_config)
{
    i2c_transfer_config_t   config;
    uint32_t saved_mask;
    i2c_irq_status_t status;
    uint32_t  temp;

    /* step 0: parameter check & init para */
    if (HAL_I2C_MASTER_MAX <= i2c_port) {
        log_hal_msgid_error("[hal][i2c%d][poll]:0 invalid port\r\n", 1, i2c_port);
        return HAL_I2C_STATUS_INVALID_PORT_NUMBER;
    }
    if (NULL == i2c_send_to_receive_config) {
        log_hal_msgid_error("[hal][i2c%d][poll]:0 invalid parameter\r\n", 1, i2c_port);
        return HAL_I2C_STATUS_INVALID_PARAMETER;
    }
    if ((s_priv_info[i2c_port].hw_state & I2C_HW_STATE_USED_MASK) == 0) {
        log_hal_msgid_error("[hal][i2c%d][poll]: not initialized\r\n", 1, i2c_port);
        return HAL_I2C_STATUS_ERROR;
    }
    memset(&config, 0, sizeof(config));
    config.speed      = HAL_I2C_FREQUENCY_MAX;
    config.slv_addr   = i2c_send_to_receive_config->slave_address;
    config.trans_mode = I2C_TRANSFER_MODE_FIFO;
    config.send_buff  = (uint8_t *)i2c_send_to_receive_config->send_data;
    config.send_size  = i2c_send_to_receive_config->send_length;
    config.recv_buff  = i2c_send_to_receive_config->receive_buffer;
    config.recv_size  = i2c_send_to_receive_config->receive_length;
    config.recv_pack_size = 1;
    config.send_pack_size = 1;
    if (config.recv_size == 0 && config.send_size == 0) {
        log_hal_msgid_error("[hal][i2c%d][poll]:1 invalid parameter\r\n", 1, i2c_port);
        return HAL_I2C_STATUS_INVALID_PARAMETER;
    }

    if (config.recv_size && config.send_size) {
        config.trans_type = I2C_TRANSFER_TYPE_TX_RX;
    } else if (config.recv_size == 0) {
        config.trans_type = I2C_TRANSFER_TYPE_TX;
    } else {
        config.trans_type = I2C_TRANSFER_TYPE_RX;
    }

    /* step 1: wait i2c hw idle */
    while (1) {
        if (i2c_wait_idle_with_timeout(i2c_port, 5000) < 0) {
            log_hal_msgid_error("[hal][i2c%d][poll]:0 wait hw idle timeout\r\n", 1, i2c_port);
            return HAL_I2C_STATUS_ERROR_BUSY;
        }
        hal_nvic_save_and_set_interrupt_mask(&saved_mask);
        if (i2c_op_ioctl(i2c_port, I2C_IOCTRL_GET_BUSY_STAT, 0)) {
            hal_nvic_restore_interrupt_mask(saved_mask);
        } else {
            break;
        }
    }
    i2c_op_ioctl(i2c_port, I2C_IOCTRL_LOCK_SLEEP, true); /* lock sleep */

    /* step 2: help read data and status if other had use hw */
    if (queue_pop(&(s_priv_info[i2c_port].i2c_queue), (int *)&temp) >= 0) {
        i2c_transfer_config_t   *pcfg;

        pcfg = (i2c_transfer_config_t *)temp;
        if (pcfg != NULL) {
            if (pcfg->trans_type == I2C_TRANSFER_TYPE_RX || pcfg->trans_type == I2C_TRANSFER_TYPE_TX_RX) {
                i2c_config_fifo(i2c_port, I2C_FIFO_OP_RD, pcfg->recv_buff, pcfg->recv_size);
            }
            pcfg->priv_data = i2c_get_irq_status(i2c_port) - 1;
        }
    }
    queue_push(&(s_priv_info[i2c_port].i2c_queue), (int)&config);

    /* step 3: config i2c hw */
    if (s_priv_info[i2c_port].hw_state & I2C_HW_STATE_IO_PUSHPULL) {
        i2c_config_io(i2c_port, false);
    } else {
        i2c_config_io(i2c_port, true);
    }
    _hal_i2c_start_transfer(i2c_port, &config);
    hal_nvic_restore_interrupt_mask(saved_mask);

    /* step 4: wait i2c hw idle */
    while (1) {
        if (i2c_wait_idle_with_timeout(i2c_port, 5000) < 0) {
            log_hal_msgid_error("[hal][i2c%d][poll]:1 wait hw idle timeout\r\n", 1, i2c_port);
            i2c_op_ioctl(i2c_port, I2C_IOCTRL_LOCK_SLEEP, false); /* unlock sleep */
            return HAL_I2C_STATUS_ERROR_BUSY;
        }
        hal_nvic_save_and_set_interrupt_mask(&saved_mask);
        if (i2c_op_ioctl(i2c_port, I2C_IOCTRL_GET_BUSY_STAT, 0)) {
            hal_nvic_restore_interrupt_mask(saved_mask);
        } else {
            break;
        }
    }

    /* step 5: get status or read fifo */
    if (config.priv_data == 0) {
        status = i2c_get_irq_status(i2c_port);
        if (status == I2C_IRQ_STAT_SUCCESS && config.trans_type >= I2C_TRANSFER_TYPE_RX) {
            i2c_config_fifo(i2c_port, I2C_FIFO_OP_RD, config.recv_buff, config.recv_size);
        }
        queue_pop(&(s_priv_info[i2c_port].i2c_queue), NULL);
    } else {
        status = config.priv_data + 1;
    }
    hal_nvic_restore_interrupt_mask(saved_mask);
    i2c_op_ioctl(i2c_port, I2C_IOCTRL_LOCK_SLEEP, false); /* unlock sleep */
    if (status != I2C_IRQ_STAT_SUCCESS) {
        log_hal_msgid_error("[airo][i2c%d][poll]: error %d, intr stat 0x%x \r\n", 3, i2c_port, status, i2c_op_ioctl(i2c_port, I2C_IOCTRL_GET_IRQ_STAT, 0));
        return HAL_I2C_STATUS_ERROR;
    } else {
        return HAL_I2C_STATUS_OK;
    }
}




hal_i2c_status_t    hal_i2c_master_send_polling(hal_i2c_port_t i2c_port, uint8_t slave_address, const uint8_t *data, uint32_t size)
{
    hal_i2c_send_to_receive_config_t i2c_snd_recv_cfg;

    memset(&i2c_snd_recv_cfg, 0, sizeof(i2c_snd_recv_cfg));
    i2c_snd_recv_cfg.send_data     = data;
    i2c_snd_recv_cfg.send_length   = size;
    i2c_snd_recv_cfg.slave_address = slave_address;

    return hal_i2c_master_send_to_receive_polling(i2c_port, &i2c_snd_recv_cfg);
}


hal_i2c_status_t    hal_i2c_master_receive_polling(hal_i2c_port_t i2c_port, uint8_t slave_address, uint8_t *buffer, uint32_t size)
{
    hal_i2c_send_to_receive_config_t i2c_snd_recv_cfg;

    memset(&i2c_snd_recv_cfg, 0, sizeof(i2c_snd_recv_cfg));
    i2c_snd_recv_cfg.receive_buffer = buffer;
    i2c_snd_recv_cfg.receive_length = size;
    i2c_snd_recv_cfg.slave_address  = slave_address;

    return hal_i2c_master_send_to_receive_polling(i2c_port, &i2c_snd_recv_cfg);
}



hal_i2c_status_t hal_i2c_master_send_dma(hal_i2c_port_t i2c_port, uint8_t slave_address, const uint8_t *data, uint32_t size)
{
    hal_i2c_send_to_receive_config_ex_no_busy_t    config;

    memset(&config, 0, sizeof(config));
    config.i2c_callback = NULL;
    config.i2c_config.frequency = HAL_I2C_FREQUENCY_MAX;
    config.i2c_send_to_receive_config_ex.send_data = data;
    config.i2c_send_to_receive_config_ex.send_bytes_in_one_packet = size;
    config.i2c_send_to_receive_config_ex.send_packet_length = 1;
    config.i2c_send_to_receive_config_ex.slave_address = slave_address;
    config.user_data = NULL;

    return _hal_i2c_master_send_and_receive_dma_with_nb(i2c_port, &config);
}

hal_i2c_status_t hal_i2c_master_receive_dma(hal_i2c_port_t i2c_port, uint8_t slave_address, uint8_t  *buffer, uint32_t size)
{
    hal_i2c_send_to_receive_config_ex_no_busy_t    config;

    memset(&config, 0, sizeof(config));
    config.i2c_callback = NULL;
    config.i2c_config.frequency = HAL_I2C_FREQUENCY_MAX;
    config.i2c_send_to_receive_config_ex.receive_buffer             = buffer;
    config.i2c_send_to_receive_config_ex.receive_bytes_in_one_packet = size;
    config.i2c_send_to_receive_config_ex.receive_packet_length      = 1;
    config.i2c_send_to_receive_config_ex.slave_address = slave_address;
    config.user_data = NULL;

    return _hal_i2c_master_send_and_receive_dma_with_nb(i2c_port, &config);
}

hal_i2c_status_t hal_i2c_master_send_to_receive_dma(hal_i2c_port_t i2c_port, hal_i2c_send_to_receive_config_t *i2c_send_to_recv_config)
{
    hal_i2c_send_to_receive_config_ex_no_busy_t    config;

    memset(&config, 0, sizeof(config));
    config.i2c_callback = NULL;
    config.i2c_config.frequency = HAL_I2C_FREQUENCY_MAX;
    config.i2c_send_to_receive_config_ex.receive_buffer             = i2c_send_to_recv_config->receive_buffer;
    config.i2c_send_to_receive_config_ex.receive_bytes_in_one_packet = i2c_send_to_recv_config->receive_length;
    config.i2c_send_to_receive_config_ex.receive_packet_length      = 1;
    config.i2c_send_to_receive_config_ex.send_data                  = i2c_send_to_recv_config->send_data;
    config.i2c_send_to_receive_config_ex.send_bytes_in_one_packet   = i2c_send_to_recv_config->send_length;
    config.i2c_send_to_receive_config_ex.send_packet_length         = 1;
    config.i2c_send_to_receive_config_ex.slave_address              = i2c_send_to_recv_config->slave_address;
    config.user_data = NULL;

    return _hal_i2c_master_send_and_receive_dma_with_nb(i2c_port, &config);
}


#ifdef HAL_I2C_MASTER_FEATURE_EXTENDED_DMA

hal_i2c_status_t hal_i2c_master_send_dma_ex(hal_i2c_port_t i2c_port, hal_i2c_send_config_t *i2c_send_config)
{
    hal_i2c_send_to_receive_config_ex_no_busy_t    config;

    memset(&config, 0, sizeof(config));
    config.i2c_callback = NULL;
    config.i2c_config.frequency = HAL_I2C_FREQUENCY_MAX;
    config.i2c_send_to_receive_config_ex.send_data                  = i2c_send_config->send_data;
    config.i2c_send_to_receive_config_ex.send_bytes_in_one_packet   = i2c_send_config->send_bytes_in_one_packet;
    config.i2c_send_to_receive_config_ex.send_packet_length         = i2c_send_config->send_packet_length;
    config.i2c_send_to_receive_config_ex.slave_address              = i2c_send_config->slave_address;
    config.user_data = NULL;

    return _hal_i2c_master_send_and_receive_dma_with_nb(i2c_port, &config);
}

hal_i2c_status_t hal_i2c_master_receive_dma_ex(hal_i2c_port_t i2c_port, hal_i2c_receive_config_t *i2c_receive_config)
{
    hal_i2c_send_to_receive_config_ex_no_busy_t    config;

    memset(&config, 0, sizeof(config));
    config.i2c_callback = NULL;
    config.i2c_config.frequency = HAL_I2C_FREQUENCY_MAX;
    config.i2c_send_to_receive_config_ex.receive_buffer             = i2c_receive_config->receive_buffer;
    config.i2c_send_to_receive_config_ex.receive_bytes_in_one_packet = i2c_receive_config->receive_bytes_in_one_packet;
    config.i2c_send_to_receive_config_ex.receive_packet_length      = i2c_receive_config->receive_packet_length;
    config.i2c_send_to_receive_config_ex.slave_address              = i2c_receive_config->slave_address;
    config.user_data = NULL;
    return _hal_i2c_master_send_and_receive_dma_with_nb(i2c_port, &config);
}

hal_i2c_status_t hal_i2c_master_send_to_receive_dma_ex(hal_i2c_port_t i2c_port, hal_i2c_send_to_receive_config_ex_t *i2c_send_to_rcv_config_ex)
{
    hal_i2c_send_to_receive_config_ex_no_busy_t    config;

    memset(&config, 0, sizeof(config));
    config.i2c_callback = NULL;
    config.i2c_config.frequency = HAL_I2C_FREQUENCY_MAX;
    config.i2c_send_to_receive_config_ex.receive_buffer             = i2c_send_to_rcv_config_ex->receive_buffer;
    config.i2c_send_to_receive_config_ex.receive_bytes_in_one_packet = i2c_send_to_rcv_config_ex->receive_bytes_in_one_packet;
    config.i2c_send_to_receive_config_ex.receive_packet_length      = i2c_send_to_rcv_config_ex->receive_packet_length;
    config.i2c_send_to_receive_config_ex.send_data                  = i2c_send_to_rcv_config_ex->send_data;
    config.i2c_send_to_receive_config_ex.send_bytes_in_one_packet   = i2c_send_to_rcv_config_ex->send_bytes_in_one_packet;
    config.i2c_send_to_receive_config_ex.send_packet_length         = i2c_send_to_rcv_config_ex->send_packet_length;
    config.i2c_send_to_receive_config_ex.slave_address              = i2c_send_to_rcv_config_ex->slave_address;
    config.user_data = NULL;

    return _hal_i2c_master_send_and_receive_dma_with_nb(i2c_port, &config);
}
#endif


hal_i2c_status_t hal_i2c_master_send_to_receive_dma_ex_none_blocking(hal_i2c_port_t i2c_port, hal_i2c_send_to_receive_config_ex_no_busy_t *config)
{
    uint32_t temp;

    if (HAL_I2C_MASTER_MAX <= i2c_port) {
        log_hal_msgid_error("[hal][i2c%d] dma nb send recv fail: port invalid", 1, i2c_port);
        return HAL_I2C_STATUS_INVALID_PORT_NUMBER;
    }
    if (HAL_I2C_FREQUENCY_MAX <= config->i2c_config.frequency) {
        log_hal_msgid_error("[hal][i2c%d] dma nb send recv fail: para invalid", 1, i2c_port);
        return HAL_I2C_STATUS_INVALID_PARAMETER;
    }

    i2c_op_ioctl(i2c_port, I2C_IOCTRL_DISABLE_IRQ, true);
    temp = s_priv_info[i2c_port].hw_state & I2C_HW_STATE_USED_MASK;
    if (temp) {
        i2c_op_ioctl(i2c_port, I2C_IOCTRL_DISABLE_IRQ, false);
        log_hal_msgid_error("[hal][i2c%d] dma nb send recv fail: dev busy(%d)", 2, i2c_port, temp);
        return HAL_I2C_STATUS_ERROR;
    }

    /* init i2c hardware */
    hal_i2c_master_init(i2c_port, &config->i2c_config);
    s_priv_info[i2c_port].hw_state &= ~(I2C_HW_STATE_USED_MASK);
    s_priv_info[i2c_port].hw_state |= (I2C_HW_STATE_USED_NB << I2C_HW_STATE_USED_OFFSET);
    i2c_op_ioctl(i2c_port, I2C_IOCTRL_DISABLE_IRQ, false);

    return _hal_i2c_master_send_and_receive_dma_with_nb(i2c_port, config);
}

#endif /* #if defined(HAL_I2C_MASTER_MODULE_ENABLED) */

