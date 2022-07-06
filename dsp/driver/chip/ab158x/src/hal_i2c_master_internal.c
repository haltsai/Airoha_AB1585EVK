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
#include "hal_platform.h"
#include "hal.h"
#include "hal_i2c_master_internal.h"

#if defined(HAL_I2C_MASTER_MODULE_ENABLED)
#include "hal_i2c_master.h"
#include "hal_pdma_internal.h"
#include "hal_sleep_manager.h"
#include "hal_sleep_manager_platform.h"

#define     I2C_MASTER_SCL_DUTY       (40)
#define     I2C_REG_FMT(x)            (x)

static volatile  I2C_REGISTER_T      *const     s_i2c_master_reg_ao   = (I2C_REGISTER_T *)I2C_REG_FMT(I2C_AO_BASE);
static volatile  airo_i2c_register_t *const     s_i2c_master_reg_airo[HAL_I2C_MASTER_MAX] = { (airo_i2c_register_t *) I2C_REG_FMT(I2C_0_BASE), (airo_i2c_register_t *) I2C_REG_FMT(I2C_1_BASE),
                                                                                              (airo_i2c_register_t *) I2C_REG_FMT(I2C_2_BASE), NULL
                                                                                            };
static volatile I2C_PDMA_REGISTER_T  *const     s_i2c_pdma_register[HAL_I2C_MASTER_MAX]   = { (I2C_PDMA_REGISTER_T *) I2C_REG_FMT(I2C0_PDMA_BASE), (I2C_PDMA_REGISTER_T *) I2C_REG_FMT(I2C1_PDMA_BASE),
                                                                                              (I2C_PDMA_REGISTER_T *) I2C_REG_FMT(I2C2_PDMA_BASE)
                                                                                            };
static const  hal_clock_cg_id                   s_i2c_clk_pdn_table[HAL_I2C_MASTER_MAX]   = {HAL_CLOCK_CG_END, HAL_CLOCK_CG_I2C1, HAL_CLOCK_CG_END, HAL_CLOCK_CG_END};
static const  hal_nvic_irq_t                    s_i2c_nvic_irq_num[HAL_I2C_MASTER_MAX]    = {IRQ_NUMBER_MAX, I2C1_IRQn, IRQ_NUMBER_MAX, IRQ_NUMBER_MAX};
#ifdef HAL_SLEEP_MANAGER_ENABLED
static volatile const sleep_management_lock_request_t    s_i2c_sleep_handle[HAL_I2C_MASTER_MAX] = {SLEEP_LOCK_INVALID_ID, SLEEP_LOCK_I2C1, SLEEP_LOCK_INVALID_ID, SLEEP_LOCK_INVALID_ID};
#endif
static volatile const pdma_channel_t                     s_i2c_pdma_chnl[HAL_I2C_MASTER_MAX][2] = { {        0xFF,         0xFF},
    {PDMA_I2C1_TX, PDMA_I2C1_RX},
    {        0xFF,         0xFF},
    {        0xFF,         0xFF},
};
////////////////////////////////////////////////static api ////////////////////////////////////////////////
static bool    _i2c_calc_speed(uint32_t speed,
                               uint8_t   duty,
                               uint32_t *clk_div,
                               uint32_t *sample_cnt_div,
                               uint32_t *step_cnt_div)
{
    uint32_t  clock_div   = 0;  /* 5bit:0x1F */
    uint32_t  smp_cnt_div = 0;  /* 3bit:0x07 */
    uint32_t  stp_cnt_div = 0;  /* 6bit:0x3F */
    uint32_t temp;

    for (clock_div = 0; clock_div < 0x1F; clock_div++) {
        temp = I2C_CLOCK_FREQUENCY / ((clock_div + 1) * speed);
        temp = (temp * duty) / 100;
        for (smp_cnt_div = 0; smp_cnt_div < 0x7; smp_cnt_div++) {
            stp_cnt_div = temp / (smp_cnt_div + 1);
            if (stp_cnt_div < 0x3f) {
                break;
            }
        }
        if (stp_cnt_div < 0x3f) {
            break;
        }
    }
    if (clock_div >= 0x1f || smp_cnt_div >= 0x7 || stp_cnt_div >= 0x3f) {
        return false;
    } else {
        *clk_div        = clock_div;
        *sample_cnt_div = smp_cnt_div;
        *step_cnt_div   = stp_cnt_div;
        return true;
    }
}



void    _i2c_config_timing(hal_i2c_port_t  i2c_port, uint32_t t_susta_ns, uint32_t t_hdsta_ns, uint32_t t_susto_ns)
{
#define     I3C_SOURCE_CLOCK_PERIOD    19.2//9.17 for 109Mhz, 19.2 for 52M

    volatile airo_i2c_register_t *i2c_reg = NULL;
    uint32_t tm_max = 0;
    uint32_t ext_tm, hs_ext_tm;
    uint32_t clk_unit;

    i2c_reg = s_i2c_master_reg_airo[i2c_port];
    if (t_susta_ns == 0 || t_hdsta_ns == 0 || t_susto_ns == 0) {
        i2c_reg->EXT_CONF = (4 << I3C_EXT_CONFIG_EXT_TIME_OFFSET) | I3C_EXT_CONFIG_EXT_EN_MASK;
        return;
    }
    clk_unit = 1000000000 / (I2C_CLOCK_FREQUENCY / (i2c_reg->CLOCK_DIV + 1));


    tm_max = (t_susta_ns > t_hdsta_ns) ? t_susta_ns : t_hdsta_ns;
    tm_max = (tm_max > t_susto_ns) ? tm_max : t_susto_ns;

    ext_tm = (uint32_t)(tm_max / clk_unit);
    ext_tm = 10;
    log_hal_i2c_info("[hal][i2c] config timing %d = %d/%d\r\n", 3, ext_tm, tm_max, clk_unit);
    if (ext_tm > 0xFF) {
        ext_tm = 0xFF;
    }

    hs_ext_tm = (uint32_t)(tm_max / clk_unit);
    if (hs_ext_tm > 0x7F) {
        hs_ext_tm = 0x7F;
    }
    i2c_reg->EXT_CONF = ((ext_tm << I3C_EXT_CONFIG_EXT_TIME_OFFSET)& I3C_EXT_CONFIG_EXT_TIME_MASK) | \
                        (I3C_EXT_CONFIG_EXT_EN_MASK);
    //((hs_ext_tm<<I3C_EXT_CONFIG_HS_EXT_TIME_OFFSET)& I3C_EXT_CONFIG_HS_EXT_TIME_MASK);

}






/////////////////////////////////////////////// API For Old I2C HW Design /////////////////////////////////////////////////

void i2c_config_speed_v1(uint32_t i2c_port, uint32_t frequency_hz)
{
    uint32_t    frequency = 0;
    uint32_t    step_cnt_div;
    uint32_t    sample_cnt_div;
    uint32_t    temp;

    (void)i2c_port;
    /* set clock div to 0 */
    s_i2c_master_reg_ao->CLOCK_DIV = 0;
    if (frequency_hz <= 50000) {
        s_i2c_master_reg_ao->CLOCK_DIV = 1;
    }
    frequency = frequency_hz;
    /***********************************************************
    * Note: according to datasheet
    *  speed = source_clock/(clock_div+1)/(2*(step_cnt_div+1)*(sample_cnt_div+1))
    ************************************************************/
    /* Fast Mode Speed */
    for (sample_cnt_div = 1; sample_cnt_div <= 8; sample_cnt_div++) {
        temp = (frequency * 2 * sample_cnt_div);
        step_cnt_div = (I2C_AO_CLOCK_FREQUENCY + temp - 1) / temp;  /* cast the <1 part */

        if (step_cnt_div <= 64) {
            break;
        }
    }
    sample_cnt_div--;
    step_cnt_div--;
    /* write timing register */
    s_i2c_master_reg_ao->TIMING_UNION.TIMING_CELLS.SAMPLE_CNT_DIV = sample_cnt_div;
    s_i2c_master_reg_ao->TIMING_UNION.TIMING_CELLS.STEP_CNT_DIV = step_cnt_div;
}


void i2c_config_io_v1(uint32_t i2c_port, bool is_opendrain)
{
    (void)i2c_port;
    if (is_opendrain) {
        s_i2c_master_reg_ao->IO_CONFIG = 0x0003;
        s_i2c_master_reg_ao->CONTROL2_UNION.CONTROL2_CELLS.CLK_EXT_EN = 1;
    } else {
        s_i2c_master_reg_ao->IO_CONFIG = 0x0;
        s_i2c_master_reg_ao->CONTROL2_UNION.CONTROL2_CELLS.CLK_EXT_EN = 0;
    }
}



hal_i2c_status_t i2c_config_transfer_v1(uint32_t i2c_port, i2c_transfer_config_t *config)
{
    (void)i2c_port;
    /*clear fifo*/
    s_i2c_master_reg_ao->CONTROL_UNION.CONTROL = 0;
    s_i2c_master_reg_ao->CONTROL2_UNION.CONTROL2_CELLS.ACKERR_DET_EN = 1;

    switch (config->trans_type) {
        case I2C_TRANSFER_TYPE_TX: {
            s_i2c_master_reg_ao->TRANSAC_LEN      = config->send_pack_size;
            s_i2c_master_reg_ao->TRANSFER_LEN     = config->send_size;
            s_i2c_master_reg_ao->TRANSFER_LEN_AUX = 0;

            s_i2c_master_reg_ao->SLAVE_ADDR       = (config->slv_addr << 1) & 0xFF;
        }
        break;
        case I2C_TRANSFER_TYPE_RX: {
            s_i2c_master_reg_ao->TRANSAC_LEN      = config->recv_pack_size;
            s_i2c_master_reg_ao->TRANSFER_LEN     = config->recv_size;
            s_i2c_master_reg_ao->TRANSFER_LEN_AUX = 0;

            s_i2c_master_reg_ao->SLAVE_ADDR       = ((config->slv_addr << 1) | 0x1) & 0xFF;
        }
        break;
        case I2C_TRANSFER_TYPE_TX_RX: {
            s_i2c_master_reg_ao->TRANSAC_LEN      = 2;
            s_i2c_master_reg_ao->TRANSFER_LEN     = config->send_size;
            s_i2c_master_reg_ao->TRANSFER_LEN_AUX = config->recv_size;
            s_i2c_master_reg_ao->CONTROL_UNION.CONTROL_CELLS.RS_STOP = 1;
            s_i2c_master_reg_ao->CONTROL_UNION.CONTROL_CELLS.DIR_CHANGE = 1;
            s_i2c_master_reg_ao->CONTROL_UNION.CONTROL_CELLS.TRANSFER_LEN_CHANGE = 1;

            s_i2c_master_reg_ao->SLAVE_ADDR       = (config->slv_addr << 1) & 0xFF;
        }
        break;
        default:
            break;
    }
    if (config->trans_mode == I2C_TRANSFER_MODE_DMA) {
        s_i2c_master_reg_ao->CONTROL_UNION.CONTROL_CELLS.DMA_EN = 1;
    }
    return 0;
}




int i2c_fifo_control_v1(uint32_t i2c_port, uint8_t op_code, uint8_t *buffer, uint32_t buffer_sz)
{
    int i = 0, buff_sz = 0;

    (void)i2c_port;
    buff_sz = (int) buffer_sz;
    if (buffer == NULL) {
        return -1;
    }
    switch (op_code) {
        case I2C_FIFO_OP_WR: {
            for (i = 0; i < buff_sz; i++) {
                if (s_i2c_master_reg_ao->FIFO_STAT_UNION.FIFO_STAT & 0x02) {
                    log_hal_msgid_error("[hal][i2c%d] wr fifo fail: full(wr %dB)", 2, i2c_port, i);
                    break;
                }
                s_i2c_master_reg_ao->DATA_PORT = buffer[i];
            }
        }
        break;
        case I2C_FIFO_OP_RD: {
            for (i = 0; i < buff_sz; i++) {
                if (s_i2c_master_reg_ao->FIFO_STAT_UNION.FIFO_STAT & 0x01) {
                    log_hal_msgid_error("[hal][i2c%d] rd fifo fail: empty(rd %dB)", 2, i2c_port, i);
                    break;
                }
                buffer[i] = s_i2c_master_reg_ao->DATA_PORT;
            }
        }
        break;
        case I2C_FIFO_OP_CLR: {
            s_i2c_master_reg_ao->FIFO_ADDR_CLR = 0x1;
        }
        break;
        default:
            i = -1;
            break;
    }

    return i;
}

i2c_irq_status_t    i2c_get_irq_status_v1(uint32_t i2c_port)
{
    uint32_t                 register_value;
    i2c_irq_status_t         return_status = I2C_IRQ_STAT_UNKNOWN_ERROR;

    (void)i2c_port;
    register_value = s_i2c_master_reg_ao->INTR_STAT;
    log_hal_msgid_info("[i2c%d] intr stat %x", 2, (int)i2c_port, (int)register_value);
    if (0 != (register_value & (0x1 << 8))) {
        return_status = I2C_IRQ_STAT_ACK_ERROR;
    } else if (0 != (register_value & (0x1 << 16))) {
        return_status = I2C_IRQ_STAT_NACK_ERROR;
    } else if (0x1 == register_value) {
        return_status = I2C_IRQ_STAT_SUCCESS;
    }
    return return_status;
}


/////////////////////////////////////////////// API For New I2C HW Design /////////////////////////////////////////////////

hal_i2c_status_t i2c_config_speed_v2(hal_i2c_port_t  i2c_port, uint32_t i2c_speed, uint32_t i2c_hs_speed)
{
    volatile airo_i2c_register_t *i2c_reg = NULL;
    uint32_t  clock_div   = 0;
    uint32_t  smp_cnt_div = 0;  /* 3bit:0x07 */
    uint32_t  stp_cnt_div = 0;  /* 6bit:0x3F */
    bool     res;

    i2c_reg = s_i2c_master_reg_airo[i2c_port];
    if (i2c_speed >= I2C_CLOCK_FREQUENCY || i2c_hs_speed >= I2C_CLOCK_FREQUENCY) {
        log_hal_i2c_error("[hal][i2c%d] freq invalid(freq:%dHz, hs_freq:%dHz)\r\n", 2, i2c_port, i2c_speed, i2c_hs_speed);
        return HAL_I2C_STATUS_INVALID_PARAMETER;
    }
    i2c_reg->CLOCK_DIV = 0;
    if (i2c_speed != 0) {
        /* clear bits*/
        i2c_reg->CLOCK_DIV &= ~I3C_CLOCK_DIV_SMP_DIV_MASK;
        i2c_reg->L_TIMING  &= ~(I3C_LTIMING_LSMAPLE_CNT_DIV_MASK | I3C_LTIMING_LSTEP_CNT_DIV_MASK);
        i2c_reg->H_TIMING  &= ~(I3C_HTIMING_HSAMPLE_CNT_DIV_MASK | I3C_HTIMING_HSTEP_CNT_DIV_MASK);

        /* config scl low */
        res = _i2c_calc_speed(i2c_speed, (100 - I2C_MASTER_SCL_DUTY), &clock_div, &smp_cnt_div, &stp_cnt_div);
        if (res == false) {
            log_hal_i2c_error("[hal][i2c%d] freq invalid(l):%dHz\r\n", 2, i2c_port, i2c_speed);
            return HAL_I2C_STATUS_INVALID_PARAMETER;
        }
        i2c_reg->CLOCK_DIV |= ((clock_div << I3C_CLOCK_DIV_SMP_DIV_OFFFSET) & I3C_CLOCK_DIV_SMP_DIV_MASK);
        i2c_reg->L_TIMING  |= ((smp_cnt_div << I3C_LTIMING_LSMAPLE_CNT_DIV_OFFSET) | (stp_cnt_div << I3C_LTIMING_LSTEP_CNT_DIV_OFFSET));
        log_hal_i2c_info("[hal][i2c%d] scl low: clk_idv(%d), sample cnt(%d), step_cnt(%d)\r\n", 3, i2c_port, clock_div, smp_cnt_div, stp_cnt_div);

        /* config scl high */
        res = _i2c_calc_speed(i2c_speed, I2C_MASTER_SCL_DUTY, &clock_div, &smp_cnt_div, &stp_cnt_div);
        if (res == false) {
            log_hal_i2c_error("[hal][i2c%d] freq invalid(h):%dHz\r\n", 2, i2c_port, i2c_speed);
            return HAL_I2C_STATUS_INVALID_PARAMETER;
        }
        i2c_reg->H_TIMING |= ((smp_cnt_div << I3C_HTIMING_HSAMPLE_CNT_DIV_OFFSET) | (stp_cnt_div << I3C_HTIMING_HSTEP_CNT_DIV_OFFSET));
        log_hal_i2c_info("[hal][i2c%d] scl high: clk_idv(%d), sample cnt(%d), step_cnt(%d)\r\n", 3, i2c_port, clock_div, smp_cnt_div, stp_cnt_div);
        log_hal_i2c_info("==============clock div:%x\r\n", 1, i2c_reg->CLOCK_DIV);
    }

    if (i2c_hs_speed != 0) {
        /* clear bits*/
        i2c_reg->L_TIMING  &= ~(I3C_LTIMING_LHS_SAMPLE_CNT_DIV_MASK | I3C_LTIMING_LHS_STEP_CNT_DIV_MASK);
        i2c_reg->HS        &= ~(I3C_HS_SAMPLE_CNT_DIV_MASK | I3C_HS_STEP_CNT_DIV_MASK);

        /* config scl low */
        res = _i2c_calc_speed(i2c_hs_speed, (100 - I2C_MASTER_SCL_DUTY), &clock_div, &smp_cnt_div, &stp_cnt_div);
        if (res == false || smp_cnt_div >= 0x7 || stp_cnt_div >= 0x7) {
            log_hal_i2c_error("[hal][i2c%d] hs freq invalid(l):%dHz\r\n", 2, i2c_port, i2c_hs_speed);
            return HAL_I2C_STATUS_INVALID_PARAMETER;
        }
        i2c_reg->CLOCK_DIV |= (clock_div << I3C_CLOCK_DIV_HS_DIV_OFFFSET) & I3C_CLOCK_DIV_HS_DIV_MASK;
        i2c_reg->L_TIMING  |= ((smp_cnt_div << I3C_LTIMING_LHS_SAMPLE_CNT_DIV_OFFSET) | (stp_cnt_div << I3C_LTIMING_LSTEP_CNT_DIV_OFFSET));
        /* config scl high */
        res = _i2c_calc_speed(i2c_hs_speed, I2C_MASTER_SCL_DUTY, &clock_div, &smp_cnt_div, &stp_cnt_div);
        if (res == false || smp_cnt_div >= 0x7 || stp_cnt_div >= 0x7) {
            log_hal_i2c_error("[hal][i3c%d] hs freq invalid(h):%dHz\r\n", 2, i2c_port, i2c_hs_speed);
            return HAL_I2C_STATUS_INVALID_PARAMETER;
        }
        i2c_reg->HS |= ((smp_cnt_div << I3C_HS_SAMPLE_CNT_DIV_OFFSET) | (stp_cnt_div << I3C_HS_STEP_CNT_DIV_OFFSET));
    }
    return HAL_I2C_STATUS_OK;
}

void    i2c_config_io_v2(hal_i2c_port_t  i2c_port, bool is_opendrain)
{

    volatile airo_i2c_register_t *i2c_reg = s_i2c_master_reg_airo[i2c_port];

    i2c_reg->IO_CONFIG = 0x0;

    if (is_opendrain) {
        i2c_reg->IO_CONFIG |= I3C_IO_CONFIG_SCL_MASK;
        i2c_reg->IO_CONFIG |= I3C_IO_CONFIG_SDA_MASK;
        i2c_reg->CONTROL   |= I3C_CONTROL_CLK_EXT_EN_MASK;
    } else {
        i2c_reg->IO_CONFIG &= ~(I3C_IO_CONFIG_SCL_MASK | I3C_IO_CONFIG_SDA_MASK);
        i2c_reg->CONTROL   &= ~I3C_CONTROL_CLK_EXT_EN_MASK;
    }
}

#define     I2C_DEFAULT_EXT_TIMING     0x32

void    i2c_config_transfer_v2(hal_i2c_port_t  i2c_port, i2c_transfer_config_t *i2c_buff)
{
    volatile airo_i2c_register_t *i2c_reg = s_i2c_master_reg_airo[i2c_port];
    //hal_i2c_status_t              result  = HAL_I2C_STATUS_OK;

    i2c_reg->START            = 0;
    i2c_reg->CONTROL          = I3C_CONTROL_ACKERR_DET_EN_MASK | I3C_CONTROL_RS_STOP_MASK; /* enable ackerr detect */
    i2c_reg->TRANSFER_LEN     = 0;
    i2c_reg->TRANSFER_LEN_AUX = 0;
    i2c_reg->DELAY_LEN        = 0x1C;
    i2c_reg->EXT_CONF         = (I2C_DEFAULT_EXT_TIMING << 8) | (I2C_DEFAULT_EXT_TIMING << 1) | 0x1;
    switch (i2c_buff->trans_type) {
        case I2C_TRANSFER_TYPE_TX_RX: {
            i2c_reg->CONTROL         |= (I3C_CONTROL_DIR_CHANGE_MASK | I3C_CONTROL_TRANSFER_LEN_CHANGE_MASK); /* direction change enable */
            i2c_reg->TRANSFER_LEN     = i2c_buff->send_size;
            i2c_reg->TRANSFER_LEN_AUX = i2c_buff->recv_size;
            i2c_reg->TRANSAC_LEN      = 2;

            i2c_reg->SLAVE_ADDR       = (i2c_buff->slv_addr << 1) & 0xFF;
        }
        break;
        case I2C_TRANSFER_TYPE_TX: {
            i2c_reg->TRANSFER_LEN     = i2c_buff->send_size;
            i2c_reg->TRANSFER_LEN_AUX = 0;
            i2c_reg->TRANSAC_LEN      = i2c_buff->send_pack_size;

            i2c_reg->SLAVE_ADDR       = (i2c_buff->slv_addr << 1) & 0xFF;
        }
        break;
        case I2C_TRANSFER_TYPE_RX: {
            i2c_reg->TRANSFER_LEN     = i2c_buff->recv_size;
            i2c_reg->TRANSFER_LEN_AUX = 0;
            i2c_reg->TRANSAC_LEN      = i2c_buff->recv_pack_size;
            i2c_reg->SLAVE_ADDR       = ((i2c_buff->slv_addr << 1) | 0x1) & 0xFF;
        }
        break;
        default: {
            i2c_reg->TRANSAC_LEN  = 1;
        }
        break;
    }
    if (i2c_buff->trans_mode == I2C_TRANSFER_MODE_DMA) {
        i2c_reg->CONTROL |= I3C_CONTROL_DMA_EN_MASK;
    }
}

int i2c_fifo_control_v2(uint32_t i2c_port, uint8_t op_code, uint8_t *buff, uint32_t size)
{
    volatile airo_i2c_register_t *i2c_reg = s_i2c_master_reg_airo[i2c_port];
    uint32_t i = 0;

    /* if op != I3C_FIFO_CLEAR, need check the buffer and size*/
    if (op_code != I2C_FIFO_OP_CLR && (buff == NULL || size == 0)) {
        log_hal_i2c_error("[hal][i2c%d] i2c_setting_fifo: err para\r\n", 1, i2c_port);
        return -1;
    }
    switch (op_code) {
        case I2C_FIFO_OP_WR: {
            for (i = 0; i < size; i++) {
                i2c_reg->DATA_PORT = buff[i];
            }
        }
        break;

        case I2C_FIFO_OP_RD: {
            for (i = 0; i < size; i++) {
                buff[i] = i2c_reg->DATA_PORT;
            }
        }
        break;
        case I2C_FIFO_OP_CLR: {
            i2c_reg->FIFO_ADDR_CLR = 0x3;
        }
        break;
    }
    return i;
}



i2c_irq_status_t    i2c_get_irq_status_v2(uint32_t i2c_port)
{
    uint32_t                 register_value;
    i2c_irq_status_t         return_status = I2C_IRQ_STAT_UNKNOWN_ERROR;

    register_value = s_i2c_master_reg_airo[i2c_port]->INTR_STA;
    log_hal_i2c_info("[i2c%d] intr stat %x", 2, i2c_port, register_value);
    if (register_value == 0x1) {
        return_status = I2C_IRQ_STAT_SUCCESS;
    } else if (register_value & I3C_INTR_MASK_ACKERR_MASK) {
        return_status = I2C_IRQ_STAT_ACK_ERROR;
    } else if (register_value & I3C_INTR_MASK_HS_NACKERR_MASK) {
        return_status = I2C_IRQ_STAT_NACK_ERROR;
    } else {
        return_status = I2C_IRQ_STAT_UNKNOWN_ERROR;
    }
    return return_status;
}


////////////////////////////////////////////////////////////////////////////////////

hal_i2c_status_t i2c_config_speed(hal_i2c_port_t  i2c_port, uint32_t i2c_speed, uint32_t i2c_hs_speed)
{
    hal_i2c_status_t status = HAL_I2C_STATUS_OK;

    if (i2c_port == HAL_I2C_MASTER_AO) {
        i2c_config_speed_v1(i2c_port, i2c_speed);
    } else {
        status = i2c_config_speed_v2(i2c_port, i2c_speed, i2c_hs_speed);
    }
    return status;
}

hal_i2c_status_t     i2c_config_io(hal_i2c_port_t  i2c_port, bool is_opendrain)
{
    hal_i2c_status_t status = HAL_I2C_STATUS_OK;

    if (i2c_port == HAL_I2C_MASTER_AO) {
        i2c_config_io_v1(i2c_port, is_opendrain);
    } else {
        i2c_config_io_v2(i2c_port, is_opendrain);
    }
    return status;
}

hal_i2c_status_t    i2c_config_transfer(hal_i2c_port_t  i2c_port, i2c_transfer_config_t *config)
{
    hal_i2c_status_t status = HAL_I2C_STATUS_OK;

    if (i2c_port == HAL_I2C_MASTER_AO) {
        i2c_config_transfer_v1(i2c_port, config);
    } else {
        i2c_config_transfer_v2(i2c_port, config);
    }
    return status;


}

hal_i2c_status_t    i2c_config_fifo(uint32_t i2c_port, uint8_t op_code, uint8_t *buff, uint32_t buff_sz)
{
    hal_i2c_status_t status = HAL_I2C_STATUS_OK;

    if (i2c_port == HAL_I2C_MASTER_AO) {
        i2c_fifo_control_v1(i2c_port, op_code, buff, buff_sz);
    } else {
        i2c_fifo_control_v2(i2c_port, op_code, buff, buff_sz);
    }
    return status;
}



i2c_irq_status_t    i2c_get_irq_status(uint32_t i2c_port)
{
    i2c_irq_status_t status = I2C_IRQ_STAT_UNKNOWN_ERROR;

    if (i2c_port == HAL_I2C_MASTER_AO) {
        status = i2c_get_irq_status_v1(i2c_port);
    } else {
        status = i2c_get_irq_status_v2(i2c_port);
    }
    return status;
}

int    i2c_op_ioctl(uint32_t i2c_port, uint32_t op_code, uint32_t args)
{
    int result = 0;

    switch (op_code) {
        case I2C_IOCTRL_GET_BUSY_STAT: {
            if (i2c_port == HAL_I2C_MASTER_AO) {
                result = s_i2c_master_reg_ao->START & 0x1;
            } else {
                result = s_i2c_master_reg_airo[i2c_port]->START & 0x1;
            }
        }
        break;
        case I2C_IOCTRL_CLR_IRQ_STAT: {
            if (i2c_port == HAL_I2C_MASTER_AO) {
                s_i2c_master_reg_ao->INTR_STAT = 0x010101;
            } else {
                s_i2c_master_reg_airo[i2c_port]->INTR_STA = 0xFFFF;
            }
        }
        break;
        case I2C_IOCTRL_START: {
            if (i2c_port == HAL_I2C_MASTER_AO) {
                s_i2c_master_reg_ao->START = 0x1;
            } else {
                s_i2c_master_reg_airo[i2c_port]->START = 0x1;
            }
        }
        break;
        case I2C_IOCTRL_GET_IRQ_STAT: {
            if (i2c_port == HAL_I2C_MASTER_AO) {
                result = s_i2c_master_reg_ao->INTR_STAT;
            } else {
                result = s_i2c_master_reg_airo[i2c_port]->INTR_STA;
            }
        }
        break;
#if 0 //def HAL_SLEEP_MANAGER_ENABLED
        case I2C_IOCTRL_LOCK_SLEEP: {
            if (args) {
                hal_sleep_manager_lock_sleep(s_i2c_sleep_handle[i2c_port]);
            } else {
                hal_sleep_manager_unlock_sleep(s_i2c_sleep_handle[i2c_port]);
            }
        }
        break;
#endif
#if 0//def HAL_CLOCK_MODULE_ENABLED

        case I2C_IOCTRL_ENABLE_CLOCK: {
            if (args) {
                hal_clock_enable(s_i2c_clk_pdn_table[i2c_port]);
            } else {
                hal_clock_disable(s_i2c_clk_pdn_table[i2c_port]);
            }
        }
        break;
#endif
        case I2C_IOCTRL_GET_SLV_ADDR: {
            if (i2c_port == HAL_I2C_MASTER_AO) {
                result = ((s_i2c_master_reg_ao->SLAVE_ADDR) >> 1) & 0xFF;
            } else {
                result = ((s_i2c_master_reg_airo[i2c_port]->SLAVE_ADDR) >> 1) & 0xFF;
            }
        }
        break;
        case I2C_IOCTRL_DISABLE_IRQ: {
            static uint32_t save_mask = 0;
            if (args) {
                hal_nvic_save_and_set_interrupt_mask(&save_mask);
            } else {
                hal_nvic_restore_interrupt_mask(save_mask);
            }
        }
        break;
    }
    return result;
}



uint32_t    i2c_speed_enum_to_dec(hal_i2c_frequency_t freq)
{
    uint32_t    freq_dec = 0;
    switch (freq) {
        case HAL_I2C_FREQUENCY_50K:
            freq_dec = 50000;
            break;
        case HAL_I2C_FREQUENCY_100K:
            freq_dec = 100000;
            break;
        case HAL_I2C_FREQUENCY_200K:
            freq_dec = 200000;
            break;
        case HAL_I2C_FREQUENCY_300K:
            freq_dec = 300000;
            break;
        case HAL_I2C_FREQUENCY_400K:
            freq_dec = 400000;
            break;
        case HAL_I2C_FREQUENCY_1M:
            freq_dec = 1000000;
            break;
        //case HAL_I2C_FREQUENCY_2M:      freq_dec = 2000000; break;
        //case HAL_I2C_FREQUENCY_3M:      freq_dec = 3000000; break;
        default:
            freq_dec = 400000;
            break;
    }
    log_hal_i2c_info("[hal][i2c] set frequency to %dHz\r\n", 1, freq_dec);
    return freq_dec;
}

int    i2c_wait_idle_with_timeout(uint32_t i2c_port, uint32_t timeout_us)
{
    uint32_t  tick_pre, tick_cur, tick_dur;
    int       result = 0;
    volatile uint32_t *ptemp = NULL;

    if (i2c_port == HAL_I2C_MASTER_AO) {
        ptemp = &(s_i2c_master_reg_ao->START);
    } else {
        ptemp = &(s_i2c_master_reg_airo[i2c_port]->START);
    }

    hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M, &tick_pre);
    while (1) {
        if (((*ptemp) & 0x1) == 0) {
            result = 0;
            break;
        }
        hal_gpt_get_free_run_count(HAL_GPT_CLOCK_SOURCE_1M, &tick_cur);
        hal_gpt_get_duration_count(tick_pre, tick_cur, &tick_dur);
        if (tick_dur > timeout_us) {
            result = -1;
            break;
        }
    }
    return result;
}


int     i2c_get_port_by_nvic_id(hal_nvic_irq_t irq_num)
{
    int i = 0;
    for (i = 0; i < HAL_I2C_MASTER_MAX; i++) {
        if (irq_num == s_i2c_nvic_irq_num[i]) {
            return i;
        }
    }
    return -1;
}


int     i2c_config_irq(hal_i2c_port_t  i2c_port, hal_nvic_isr_t callback)
{
#ifdef I2C_NVIC_ENABLED

    hal_nvic_register_isr_handler(s_i2c_nvic_irq_num[i2c_port], callback);
    hal_nvic_enable_irq(s_i2c_nvic_irq_num[i2c_port]);
#endif
    return 0;
}


void    i2c_sleep_register_backup(hal_i2c_port_t  i2c_port, uint32_t *buff, uint32_t size)
{
    if (size < 6) {
        return;
    }
    buff[0] = s_i2c_master_reg_airo[i2c_port]->CLOCK_DIV;
    buff[1] = s_i2c_master_reg_airo[i2c_port]->L_TIMING;
    buff[2] = s_i2c_master_reg_airo[i2c_port]->H_TIMING;
    buff[3] = s_i2c_master_reg_airo[i2c_port]->IO_CONFIG;
    buff[4] = s_i2c_master_reg_airo[i2c_port]->CONTROL;
    buff[5] = s_i2c_master_reg_airo[i2c_port]->EXT_CONF;
}

void    i2c_sleep_register_restore(hal_i2c_port_t  i2c_port, uint32_t *buff, uint32_t size)
{
    if (size < 6) {
        return;
    }
    s_i2c_master_reg_airo[i2c_port]->CLOCK_DIV = buff[0];
    s_i2c_master_reg_airo[i2c_port]->L_TIMING  = buff[1];
    s_i2c_master_reg_airo[i2c_port]->H_TIMING  = buff[2];
    s_i2c_master_reg_airo[i2c_port]->IO_CONFIG = buff[3];
    s_i2c_master_reg_airo[i2c_port]->CONTROL   = buff[4];
    s_i2c_master_reg_airo[i2c_port]->EXT_CONF  = buff[5];
}

int i2c_pdma_init(uint32_t i2c_port, i2c_transfer_config_t *config)
{
    pdma_config_t           i2c_dma_config;
    pdma_status_t           dma_statue;
    bool    tx_enable = 0, rx_enable = 0;

    i2c_dma_config.burst_mode = false;
    i2c_dma_config.size       = PDMA_BYTE;
    switch (config->trans_type) {
        case I2C_TRANSFER_TYPE_TX_RX:
            tx_enable = true;
            rx_enable = true;
            break;
        case I2C_TRANSFER_TYPE_TX:
            tx_enable = true;
            rx_enable = false;
            break;
        case I2C_TRANSFER_TYPE_RX:
            tx_enable = false;
            rx_enable = true;
            break;
        default:
            log_hal_i2c_info("[hal][i2c%d] pdma init fail: trans_type err(%d)\r\n", 2, i2c_port, config->trans_type);
            break;
    }
    if (tx_enable) {
        /* config tx pdma */
        dma_statue = pdma_init(s_i2c_pdma_chnl[i2c_port][0]);
        if (dma_statue != PDMA_OK) {
            return -1;
        }
        i2c_dma_config.count        = config->send_size * config->send_pack_size;
        i2c_dma_config.master_type  = PDMA_TX;
        dma_statue = pdma_configure(s_i2c_pdma_chnl[i2c_port][0], &i2c_dma_config);
        if (dma_statue != PDMA_OK) {
            return -2;
        }
        dma_statue = pdma_start_interrupt(s_i2c_pdma_chnl[i2c_port][0], (uint32_t)config->send_buff);
        if (dma_statue != PDMA_OK) {
            return -3;
        }
    }
    if (rx_enable) {
        /* config rx pdma */
        dma_statue = pdma_init(s_i2c_pdma_chnl[i2c_port][1]);
        if (dma_statue != PDMA_OK) {
            return -1;
        }
        i2c_dma_config.master_type  = PDMA_RX;
        i2c_dma_config.count        = config->recv_size * config->recv_pack_size;
        dma_statue = pdma_configure(s_i2c_pdma_chnl[i2c_port][1], &i2c_dma_config);
        if (dma_statue != PDMA_OK) {
            return -2;
        }
        dma_statue = pdma_start_interrupt(s_i2c_pdma_chnl[i2c_port][1], (uint32_t)config->recv_buff);
        if (dma_statue != PDMA_OK) {
            return -3;
        }
    }
    return 0;
}

int    i2c_pdma_deinit(uint32_t i2c_port)
{
    pdma_channel_t          rx_dma_ch, tx_dma_ch;

    tx_dma_ch = s_i2c_pdma_chnl[i2c_port][0];
    rx_dma_ch = s_i2c_pdma_chnl[i2c_port][1];
    pdma_stop(tx_dma_ch);
    pdma_deinit(tx_dma_ch);
    pdma_stop(rx_dma_ch);
    pdma_deinit(rx_dma_ch);

    return PDMA_OK;
}


void    i2c_dump_register(hal_i2c_port_t  i2c_port)
{
    volatile airo_i2c_register_t *i2c_reg = s_i2c_master_reg_airo[i2c_port];

    log_hal_i2c_info("===================== I2C%d Dump(%x) =====================\r\n", 2, i2c_port, i2c_reg);
    log_hal_i2c_info("SLV_ADDR:%x, INTR_MSK:%x, INTR_STA:%x, CONTROL:%x\r\n", 4,
                     i2c_reg->SLAVE_ADDR,   i2c_reg->INTR_MASK,
                     i2c_reg->INTR_STA,     i2c_reg->CONTROL
                    );
    log_hal_i2c_info("TRF_LEN:%x, TRF_AUX:%x, TRC_LEN:%x, DLY_LEN:%x\r\n", 4,
                     i2c_reg->TRANSFER_LEN,     i2c_reg->TRANSFER_LEN_AUX,
                     i2c_reg->TRANSAC_LEN,      i2c_reg->DELAY_LEN
                    );
    log_hal_i2c_info("HTIME:%x, LTIME:%x, HS:%x, START:%x\r\n", 4,
                     i2c_reg->H_TIMING,     i2c_reg->L_TIMING,
                     i2c_reg->HS,           i2c_reg->START
                    );
    log_hal_i2c_info("EXT_CNF:%x, IO_CNF:%x, CLK_DIV:%x, TRAFFIC:%x\r\n", 4,
                     i2c_reg->EXT_CONF,     i2c_reg->IO_CONFIG,
                     i2c_reg->CLOCK_DIV,    i2c_reg->TRAFFIC
                    );
    log_hal_i2c_info("SHAPE:%x, DAT_TM:%x, TMOUT_DIV:%x, DEF_DA:%x\r\n", 4,
                     i2c_reg->SHAPE,        i2c_reg->DATA_TIMING,
                     i2c_reg->TIMEOUT_DIV,  i2c_reg->DEF_DA
                    );
    log_hal_i2c_info("IRQ_INFO:%x, DEBUG_STAT:%x, FIFO_STAT:%x, FIFO_THRESH:%x\r\n", 4,
                     i2c_reg->IRQ_INFO,   i2c_reg->DEBUG_STAT,
                     i2c_reg->FIFO_STAT,  i2c_reg->FIFO_THRESH
                    );
}


void    i2c_pdma_dump_register(hal_i2c_port_t i2c_port)
{
    volatile I2C_PDMA_REGISTER_T     *base_reg = s_i2c_pdma_register[i2c_port];

    log_hal_i2c_info("===================== PDMA Dump (I2C%d) =====================\r\n", 1, i2c_port);
    log_hal_i2c_info("GLB->STA:%x, CPU0_CFG:%x, BUSY:%x, INTR:%x, CLK_CFG:%x\r\n", 5,
                     base_reg->GLB_STA,   base_reg->GLB_CPU0_CFG,
                     base_reg->GLB_BUSY,  base_reg->GLB_INTR,
                     base_reg->GLB_CLK_CFG
                    );
    log_hal_i2c_info("PDMA1-> CON:%x, COUNT:%x, START:%x, INTR_STA:%x, PGMADR:%x, RLCT:%x\r\n", 6,
                     base_reg->PDMA1_CON,    base_reg->PDMA1_COUNT,
                     base_reg->PDMA1_START,  base_reg->PDMA1_INTSTA,
                     base_reg->PDMA1_PGMADDR, base_reg->PDMA1_RLCT
                    );
    log_hal_i2c_info("PDMA2-> CON:%x, COUNT:%x, START:%x, INTR_STA:%x, PGMADR:%x, RLCT:%x\r\n", 6,
                     base_reg->PDMA2_CON,    base_reg->PDMA2_COUNT,
                     base_reg->PDMA2_START,  base_reg->PDMA2_INTSTA,
                     base_reg->PDMA2_PGMADDR, base_reg->PDMA1_RLCT
                    );
}

#endif /* #if defined(HAL_I2C_MASTER_MODULE_ENABLED) */

//////////////////////////////////////////////////////////////////////////////////////////////////

void    queue_init(airo_queue_t  *pfifo)
{
    memset(pfifo, 0, sizeof(airo_queue_t));
}

int     queue_push(airo_queue_t  *pfifo, int  data)
{
    int  temp = 0;

    if (pfifo == NULL) {
        log_hal_msgid_error("[fifo][push] fifo ptr is null\n", 0);
        return I2C_FIFO_STATUS_ERROR_INVALID_PARA;
    }
    temp = (pfifo->tail + 1) % MAX_FIFO_SIZE;
    if (temp == pfifo->head) {
        log_hal_msgid_error("[fifo][push] fifo is full\n", 0);
        return I2C_FIFO_STATUS_ERROR_FULL;
    }
    log_hal_msgid_info("[fifo][push] index %d, value %x\n", 2, temp, data);
    pfifo->tail = temp;
    pfifo->data[temp] = data;
    return temp;
}

int     queue_pop(airo_queue_t  *pfifo,  int *data)
{
    int temp = 0;

    if (pfifo == NULL) {
        log_hal_msgid_error("[fifo][pop] fifo ptr is null\n", 0);
        return I2C_FIFO_STATUS_ERROR_INVALID_PARA;
    }
    if (pfifo->head == pfifo->tail) {
        log_hal_msgid_error("[fifo][pop] fifo is empty\n", 0);
        return I2C_FIFO_STATUS_ERROR_EMPTY;
    }
    temp  = (pfifo->head + 1) % MAX_FIFO_SIZE;
    if (data != NULL) {
        *data  = pfifo->data[temp];
    }
    pfifo->head = temp;
    log_hal_msgid_info("[fifo][pop] index %d, value %x\n", 2, temp, pfifo->data[temp]);
    return temp;
}

int queue_top(airo_queue_t  *pfifo,  int *data)
{
    if (pfifo == NULL) {
        return I2C_FIFO_STATUS_ERROR_INVALID_PARA;
    }
    if (pfifo->head == pfifo->tail) {
        return I2C_FIFO_STATUS_ERROR_EMPTY;
    }
    *data = pfifo->data[pfifo->tail];
    return pfifo->tail;
}


int queue_get_status(airo_queue_t  *pfifo)
{
    int temp;

    if (pfifo == NULL) {
        return I2C_FIFO_STATUS_ERROR_INVALID_PARA;
    }
    if (pfifo->head == pfifo->tail) {
        return I2C_FIFO_STATUS_ERROR_EMPTY;
    }
    temp = (pfifo->tail + 1) % MAX_FIFO_SIZE;
    if (temp == pfifo->head) {
        return I2C_FIFO_STATUS_ERROR_FULL;
    }
    return I2C_FIFO_STATUS_OK;
}

int     queue_get_data_by_index(airo_queue_t  *pfifo, int index)
{
    if (pfifo == NULL || index >= MAX_FIFO_SIZE) {
        return I2C_FIFO_STATUS_ERROR_INVALID_PARA;
    }
    return  pfifo->data[index];
}

int queue_get_empty_index(airo_queue_t  *pfifo)
{
    int temp;
    if (pfifo == NULL) {
        return I2C_FIFO_STATUS_ERROR_INVALID_PARA;
    }
    temp = (pfifo->tail + 1) % MAX_FIFO_SIZE;
    if (temp == pfifo->head) {
        return I2C_FIFO_STATUS_ERROR_FULL;
    }
    return temp;
}


