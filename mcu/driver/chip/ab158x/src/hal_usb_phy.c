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


#include "hal_usb.h"
#include "hal_usb_internal.h"

#if defined (HAL_USB_MODULE_ENABLED) || defined (HAL_USB_HOST_MODULE_ENABLED)

#include "hal_eint.h"
#include "hal_gpt.h"
#include "hal_log.h"
#include "hal_pmu.h"

#include <stdio.h>

void hal_usbphy_poweron_initialize(void)
{
    pmu_enable_usb_power(PMU_ON);
    uint32_t data;

    /* switch to USB function */
    USB_DRV_ClearBits32(U2PHYDTM0, U2PHYDTM0_FORCE_UART_EN);
    USB_DRV_ClearBits32(U2PHYDTM1, U2PHYDTM1_RG_UART_EN);

    /* force suspend till PHY set complete */
    USB_DRV_SetBits32(U2PHYDTM0, U2PHYDTM0_RG_SUSPENDM);
    USB_DRV_SetBits32(U2PHYDTM0, U2PHYDTM0_FORCE_SUSPENDM);

    /* DP/DM mode */
    USB_DRV_ClearBits32(U2PHYACR4, U2PHYACR4_USB20_GPIO_MODE);
    USB_DRV_ClearBits32(U2PHYACR4, U2PHYACR4_RG_USB20_GPIO_CTL);
    /* dp_100k & dm_100k disable */
    USB_DRV_ClearBits32(U2PHYACR4, U2PHYACR4_RG_USB20_DP_100K_EN);
    USB_DRV_ClearBits32(U2PHYACR4, U2PHYACR4_RG_USB20_DM_100K_EN);

    /* RG_USB20_INTR_CAL would be written in eFUSE 0x420C0370[4:0] */
    data = DRV_Reg32(0x420C0370) & 0b11111;
    printf("0x420C0370 & 0b11111 data = 0x%0X", (unsigned int)data);
    if (data == 0) {
        printf("not to set USBPHYACR1 RG_USB20_INTR_CAL");
    } else {
        USB_DRV_SetData32(USBPHYACR1, 0b11111 << 19, data << 19);
    }

    /* RG_HALF_PMOS_EN on */
    USB_DRV_SetData32(USBPHYACR2, 1 << 4, 1 << 4);

    /* Slew rate control */
    USB_DRV_ClearBits32(USBPHYACR5, USBPHYACR5_RG_USB20_HSTX_SRCTRL_MSK);
    USB_DRV_SetData32(USBPHYACR5, USBPHYACR5_RG_USB20_HSTX_SRCTRL_MSK, 0b000 << 12);

    /* DP/DM BC1.1 path Disable */
    USB_DRV_ClearBits32(USBPHYACR6, USBPHYACR6_RG_USB20_BC11_SW_EN);
    /* OTG Disable */
    USB_DRV_ClearBits32(USBPHYACR6, USBPHYACR6_RG_USB20_OTG_VBUSCMP_EN);
    /* pre-emphasis level */
    USB_DRV_ClearBits32(USBPHYACR6, 0b11 << 30);
    USB_DRV_SetData32(USBPHYACR6, 0b11 << 30, 0b10 << 30);

    /* Release force suspendm */
    USB_DRV_ClearBits32(U2PHYDTM0, U2PHYDTM0_FORCE_SUSPENDM);

    /* Force to device mode */
    USB_DRV_SetBits32(U2PHYDTM1, U2PHYDTM1_RG_IDDIG);
    USB_DRV_SetBits32(U2PHYDTM1, U2PHYDTM1_force_iddig);
    /* USB PHY's hw of vbusvalid, bvalid, avalid is useless. Set force to bypass it. */
    USB_DRV_SetBits32(U2PHYDTM1, U2PHYDTM1_RG_VBUSVALID | U2PHYDTM1_RG_BVALID | U2PHYDTM1_RG_AVALID);
    USB_DRV_SetBits32(U2PHYDTM1, U2PHYDTM1_force_vbusvalid | U2PHYDTM1_force_bvalid | U2PHYDTM1_force_avalid);

    hal_gpt_delay_us(2000);

    USB_DRV_SetBits32(U2PHYDTM0, U2PHYDTM0_RG_SUSPENDM);
}

void hal_usbphy_deinit_case(void)
{
    DRV_WriteReg32(0xA2090070, 0x01220121);
    hal_gpt_delay_ms(1);
    DRV_WriteReg32(0xA2090070, 0x00220021);
    USB_DRV_SetBits32((USB_PHY_MSTAR + 0x86C), (1 << 3));
}

void hal_usbphy_enter_suspend(void)
{
    USB_DRV_SetBits32(USBPHYACR0, USBPHYACR0_RG_USB20_BGR_EN);
    USB_DRV_SetBits32(USBPHYACR0, USBPHYACR0_RG_USB20_REF_EN);

    USB_DRV_SetBits32(U2PHYACR3, U2PHYACR3_RG_USB20_TMODE_FS_LS_RCV_EN);

    USB_DRV_ClearBits32(U2PHYACR4, U2PHYACR4_RG_USB20_HS_RCV_EN_MODE_MSK);
    USB_DRV_SetBits32(U2PHYACR4, 0b01 << 0);

    USB_DRV_SetBits32(U2PHYDMON0, U2PHYDMON0_RG_USB20_BGLPF_FORCE_ON);
    USB_DRV_SetBits32(U2PHYDMON0, U2PHYDMON0_RG_USB20_BGLPF_FORCE_OFF);
}

void hal_usbphy_leave_suspend()
{
    USB_DRV_ClearBits32(USBPHYACR0, USBPHYACR0_RG_USB20_BGR_EN);
    USB_DRV_ClearBits32(USBPHYACR0, USBPHYACR0_RG_USB20_REF_EN);

    USB_DRV_ClearBits32(U2PHYACR3, U2PHYACR3_RG_USB20_TMODE_FS_LS_RCV_EN);

    USB_DRV_ClearBits32(U2PHYACR4, U2PHYACR4_RG_USB20_HS_RCV_EN_MODE_MSK);
    USB_DRV_SetBits32(U2PHYACR4, 0b10 << 0);

    USB_DRV_ClearBits32(U2PHYDMON0, U2PHYDMON0_RG_USB20_BGLPF_FORCE_ON);
    USB_DRV_ClearBits32(U2PHYDMON0, U2PHYDMON0_RG_USB20_BGLPF_FORCE_OFF);
}

#if 0 /* Dummy code section*/

// USB_FPGA_DVT : inclde file
/* SCCB v2 */
#include "sccb_v2.h"
/* I2C */
#include "hal_i2c_master.h"

#if 1 // USB_FPGA_DVT : 0 - Real Chip  / 1 - FGPA

static volatile USBPHY_REGISTER_T *musb_phy = (USBPHY_REGISTER_T *)USB_SIFSLV_BASE;

#define U3_PHY_PAGE 0xff

void _u3_write_bank(uint32_t value)
{
    i2C_contorl_write(0x60, (uint8_t)U3_PHY_PAGE, (uint8_t)value);
}

uint32_t _u3_read_reg(uint32_t address)
{
    uint8_t databuffer = 0;

    databuffer = i2C_contorl_read(0x60, (uint8_t)address);

    return databuffer;
}

void _u3_write_reg(uint32_t address, uint32_t value)
{
    i2C_contorl_write(0x60, (uint8_t)address, (uint8_t)value);
}

uint32_t u3_phy_read_reg32(uint32_t addr)
{
    uint32_t bank;
    uint32_t addr8;
    uint32_t data;

    bank = (addr >> 16) & 0xff;
    addr8 = addr & 0xff;

    _u3_write_bank(bank);
    data = _u3_read_reg(addr8);
    data |= (_u3_read_reg(addr8 + 1) << 8);
    data |= (_u3_read_reg(addr8 + 2) << 16);
    data |= (_u3_read_reg(addr8 + 3) << 24);
    return data;
}

uint32_t u3_phy_write_reg32(uint32_t addr, uint32_t data)
{
    uint32_t bank;
    uint32_t addr8;
    uint32_t data_0, data_1, data_2, data_3;

    bank = (addr >> 16) & 0xff;
    addr8 = addr & 0xff;
    data_0 = data & 0xff;
    data_1 = (data >> 8) & 0xff;
    data_2 = (data >> 16) & 0xff;
    data_3 = (data >> 24) & 0xff;

    _u3_write_bank(bank);
    _u3_write_reg(addr8, data_0);
    _u3_write_reg(addr8 + 1, data_1);
    _u3_write_reg(addr8 + 2, data_2);
    _u3_write_reg(addr8 + 3, data_3);

    return 0;
}

volatile uint32_t Edw_read;
volatile uint32_t Edw_write;
volatile uint32_t Edw_addr;
volatile uint32_t Edw_offset;
volatile uint32_t Edw_mask;
volatile uint32_t Edw_value;
volatile uint32_t Edw_read2;

void u3_phy_write_field32(uint32_t addr, uint32_t offset, uint32_t mask, uint32_t value)
{
    uint32_t cur_value;
    uint32_t new_value;

    cur_value = u3_phy_read_reg32(addr);
    new_value = (cur_value & (~mask)) | ((value << offset) & mask);

    Edw_read = cur_value;
    Edw_write = new_value;
    Edw_addr = addr;
    Edw_offset = offset;
    Edw_mask = mask;
    Edw_value = value;

    u3_phy_write_reg32(addr, new_value);
    Edw_read2 = u3_phy_read_reg32(addr);
}

uint32_t u3_phy_write_reg8(uint32_t addr, uint8_t data)
{
    uint32_t bank;
    uint32_t addr8;

    bank = (addr >> 16) & 0xff;
    addr8 = addr & 0xff;
    _u3_write_bank(bank);
    _u3_write_reg(addr8, data);

    return 0;
}

void USB_EnableI2C(void)
{
    uint32_t index;
//I2C ================
    SCCB_TRANSACTION_RESULT  i2c_result;
    sccb_config_struct config;
    uint8_t  address = 0xC0;   // KS address setting ??
    uint8_t  data0[2] = {0x02, 0x44};
    static bool    bIsInit = FALSE;

    // only init once!
    if (bIsInit) {
        return;
    }

    hal_i2c_config_t i2c_config;
    i2c_config.frequency = HAL_I2C_FREQUENCY_400K;
    hal_i2c_master_init(HAL_I2C_MASTER_1, &i2c_config);

#if 1
    /* A60931_FPGA_Init_patch_20190219.txt - KS
       USB2.0
           1.lower u2 rx sensitivity for protocol analyzer
           2.phy bandgap force mode
           3.tx slew rate fixed when no calibration flow in software
    */

    //U2PHY initial
    //I2C  0x60  0xFC[31:24] 0x00  RW                                    //Change bank address to 0x00
    u3_phy_write_field32(0x0000FC, 24, (0xFF << 24), 0x00);
    //I2C  0x60  0x00[00:00] 0x01  RW  RG_SIFSLV_BGR_EN                   //USB20_BGR_EN
    u3_phy_write_field32(0x000000, 0, (0x1 << 0), 0x01);
    //I2C  0x60  0x00[05:05] 0x01  RW  RG_USB20_INTR_EN                   //RG_USB20_INTR_EN
    u3_phy_write_field32(0x000000, 5, (0x1 << 5), 0x01);
    //I2C  0x60  0x04[23:19] 0x10  RW  RG_USB20_INTR_CAL                   //RG_USB20_INTR_CAL
    u3_phy_write_field32(0x000004, 19, (0x1F << 19), 0x10);
    //I2C  0x60  0x18[23:23] 0x00  RW  RG_USB20_BC11_SW_EN               //RG_USB20_BC11_SW_EN, Disable BC 1.1
    u3_phy_write_field32(0x000018, 23, (0x1 << 23), 0x00);
    //I2C  0x60  0x68[18:18] 0x00  RW  force_suspendm                    //force_suspendm = 0
    u3_phy_write_field32(0x000068, 18, (0x1 << 18), 0x00);
    //I2C  0x60  0x68[03:03] 0x01  RW  RG_SUSPENDM                       //RG_SUSPENDM when force_suspendm = 1
    u3_phy_write_field32(0x000068, 3, (0x1 << 3), 0x01);

    //U2PHY patch
    //I2C  0x60  0x14[14:12] 0x04  RW  RG_USB20_HSTX_SRCTRL
    u3_phy_write_field32(0x000014, 12, (0x7 << 12), 0x04);
    //I2C  0x60  0x18[03:00] 0x03  RW  RG_USB20_SQTH
    u3_phy_write_field32(0x000018, 0, (0xF << 0), 0x03);
#endif

#if 0
    //USB 3.0
    //TPHY Init
    //I2C  0x60  0xFC[31:24] 0x30  RW                                    //Change bank address to 0x30
    u3_phy_write_field32(0x3000FC, 24, (0xFF << 24), 0x30);
    //I2C  0x60  0x18[24:24] 0x01  RW  RG_SSUSB_LN0_CDR_RESERVE           //AVDD10 source select
    u3_phy_write_field32(0x300018, 24, (0x1 << 24), 0x01);

    //I2C  0x60  0xFC[31:24] 0x40  RW                                    //Change bank address to 0x40
    u3_phy_write_field32(0x3000FC, 24, (0xFF << 24), 0x40);
    //I2C  0x60  0x60[13:12] 0x02  RW  RG_SSUSB_LFPS_DEGLITCH_U3           //
    u3_phy_write_field32(0x300060, 12, (0x03 << 12), 0x02);

    //I2C  0x60  0xFC[31:24] 0x30  RW                                    //Change bank address to 0x30
    u3_phy_write_field32(0x3000FC, 24, (0xFF << 24), 0x30);
    //I2C  0x60  0x04[29:29] 0x01  RW  RG_VUSB10_ON                       //SSUSB 1.0V power ON
    u3_phy_write_field32(0x300004, 29, (0x01 << 29), 0x01);
    //I2C  0x60  0x04[25:21] 0x11  RW  RG_SSUSB_XTAL_TOP_RESERVE           //RG_SSUSB_XTAL_TOP_RESERVE<15:11> =10001
    u3_phy_write_field32(0x300004, 21, (0x1F << 21), 0x11);

    //I2C  0x60  0xFC[31:24] 0x40  RW                                    //Change bank address to 0x40
    u3_phy_write_field32(0x4000FC, 24, (0xFF << 24), 0x40);
    //I2C  0x60  0x38[15:00] 0x47  RW  DA_SSUSB_PLL_SSC_DELTA1           //fine tune SSC delta1 to let SSC min average ~0ppm
    u3_phy_write_field32(0x400038, 0, (0xFFFF << 0), 0x47);
    //I2C  0x60  0x40[31:16] 0x44  RW  DA_SSUSB_PLL_SSC_DELTA            //fine tune SSC delta to let SSC min average ~0ppm
    u3_phy_write_field32(0x400040, 16, (0xFFFF << 16), 0x44);

    //I2C  0x60  0xFC[31:24] 0x30  RW                                    //Change bank address to 0x30
    u3_phy_write_field32(0x4000FC, 24, (0xFF << 24), 0x30);
    //I2C  0x60  0x14[15:00] 0x190  RW  RG_SSUSB_PLL_SSC_PRD               //fine tune SSC PRD to let SSC freq average 31.5KHz
    u3_phy_write_field32(0x400014, 0, (0xFFFF << 24), 0x190);

    ////I2C  0x60  0xFC[31:24]  0x10     RW                                     //Change bank address to 0x10
    ////I2C  0x60  0x08[22:22]  0x00     RW  rg_ssusb_p3_entry                    //disable ssusb_p3_entry to work around resume from P3
    ////I2C  0x60  0x08[23:23]  0x01     RW  rg_ssusb_p3_entry_sel                //force disable ssusb_p3_entry to work around resume from P3
    ////I2C  0x60  0xFC[31:24]  0x60     RW                                     //Change bank address to 0x60
    ////I2C  0x60  0x14[24:24]  0x00     RW  rg_ssusb_p3_bias_pwd                //disable ssusb_p3_bias_pwd to work around resume from P3

    //Set INTR & TX/RX Impedance
    //I2C  0x60  0xFC[31:24] 0x30  RW                                     //Change bank address to 0x30
    u3_phy_write_field32(0x3000FC, 24, (0xFF << 24), 0x30);
    //I2C  0x60  0x00[26:26] 0x01  RW  RG_SSUSB_INTR_EN                    //INTR_EN
    u3_phy_write_field32(0x300000, 26, (0x1 << 26), 0x01);
    //I2C  0x60  0x00[15:10] 0x28  RW  RG_SSUSB_IEXT_INTR_CTRL            //Set Iext R selection
    u3_phy_write_field32(0x300000, 10, (0x3F << 10), 0x28);

    //I2C  0x60  0xFC[31:24] 0x10  RW                                     //Change bank address to 0x10
    u3_phy_write_field32(0x1000FC, 24, (0xFF << 24), 0x10);
    //I2C  0x60  0x10[31:31] 0x01  RW  rg_ssusb_force_tx_impsel            //Force da_ssusb_tx_impsel enable
    u3_phy_write_field32(0x100010, 24, (0x1 << 31), 0x01);
    //I2C  0x60  0x10[28:24] 0x11  RW  rg_ssusb_tx_impsel                 //Set TX Impedance
    u3_phy_write_field32(0x100010, 24, (0x1F << 24), 0x11);
    //I2C  0x60  0x14[31:31] 0x01  RW  rg_ssusb_force_rx_impsel            //Force da_ssusb_rx_impsel enable
    u3_phy_write_field32(0x100014, 31, (0x1 << 31), 0x01);
    //I2C  0x60  0x14[28:24] 0x10  RW  rg_ssusb_rx_impsel                 //Set RX Impedance
    u3_phy_write_field32(0x100014, 24, (0x1F << 24), 0x10);

    //I2C  0x60  0xFC[31:24] 0x00  RW                                     //Change bank address to 0x00
    u3_phy_write_field32(0x0000FC, 24, (0xFF << 24), 0x00);
    //I2C  0x60  0x00[05:05] 0x01  RW  RG_USB20_INTR_EN                    //U2 INTR_EN
    u3_phy_write_field32(0x000000, 5, (0x1 << 5), 0x01);
    //I2C  0x60  0x04[23:19] 0x13  RW  RG_USB20_INTR_CAL                    //Set Iext R selection
    u3_phy_write_field32(0x0000FC, 19, (0x1F << 19), 0x13);

    //RX_DET timing patch
    //I2C  0x60  0xFC[31:24] 0x20  RW
    u3_phy_write_field32(0x2000FC, 24, (0xFF << 24), 0x20);
    //I2C  0x60  0x28[08:00] 0x50  RW  rg_ssusb_rxdet_stb1_set
    u3_phy_write_field32(0x200028, 0, (0x1FF << 0), 0x50);
    //I2C  0x60  0x28[17:09] 0x10  RW  rg_ssusb_rxdet_stb2_set
    u3_phy_write_field32(0x2000FC, 9, (0x1FF << 9), 0x10);
    //I2C  0x60  0x2C[08:00] 0x10  RW  rg_ssusb_rxdet_stb2_set_p3
    u3_phy_write_field32(0x2000FC, 0, (0x1FF << 0), 0x10);
    //I2C  0x60  0xFC[31:24] 0x30  RW
    u3_phy_write_field32(0x3000FC, 24, (0xFF << 24), 0x30);
    //I2C  0x60  0x1C[11:10] 0x02  RW  RG_SSUSB_RXDET_VTHSEL_L
    u3_phy_write_field32(0x30001C, 10, (0xFF << 10), 0x02);

    //Adjust pipe_clk phase for FPGA
    //I2C  0x70  0xFC[31:24] 0x70  RW                                     //Change bank address to 0x70
    u3_phy_write_field32(0x7000FC, 24, (0xFF << 24), 0x70);
    //I2C  0x70  0x88[03:02] 0x01  RW                                     //Pipe reset, clk driving current
    u3_phy_write_field32(0x700088, 2, (0x03 << 2), 0x01);
    //I2C  0x70  0x88[05:04] 0x01  RW                                     //Data lane 0 driving current
    u3_phy_write_field32(0x700088, 4, (0x03 << 4), 0x01);
    //I2C  0x70  0x88[07:06] 0x01  RW                                     //Data lane 1 driving current
    u3_phy_write_field32(0x7000FC, 2, (0x03 << 6), 0x01);
    //I2C  0x70  0x88[09:08] 0x01  RW                                     //Data lane 2 driving current
    u3_phy_write_field32(0x700088, 8, (0x03 << 8), 0x01);
    //I2C  0x70  0x88[11:10] 0x01  RW                                     //Data lane 3 driving current
    u3_phy_write_field32(0x700088, 10, (0x03 << 10), 0x01);
    //I2C  0x70  0x9C[04:00] 0x03  RW  rg_ssusb_ckphase                    //PCLK phase 0x00~0x1F <-- adjust timing
    u3_phy_write_field32(0x70009C, 24, (0x1F << 0), 0x03);
#endif

    for (index = 0; index < 50000; index++);

    bIsInit = TRUE;
}



void hal_usbphy_save_current(void)
{
    //switch to USB function. (system register, force ip into usb mode)
    USB_DRV_ClearBits32(&musb_phy->u2phydtm0, U2PHYDTM0_force_uart_en);
    USB_DRV_ClearBits32(&musb_phy->u2phydtm1, U2PHYDTM1_RG_UART_EN);
    USB_DRV_ClearBits32(&musb_phy->u2phyacr4, U2PHYACR4_rg_usb20_gpio_ctl);
    USB_DRV_ClearBits32(&musb_phy->u2phyacr4, U2PHYACR4_usb20_gpio_mode);
    //(let suspendm=1, enable usb 480MHz pll)
    USB_DRV_SetBits32(&musb_phy->u2phydtm0, U2PHYDTM0_RG_SUSPENDM);
    //(force_suspendm=1)
    USB_DRV_SetBits32(&musb_phy->u2phydtm0, U2PHYDTM0_force_suspendm);
    //Wait 2 ms. Wait USBPLL stable.
    hal_gpt_delay_ms(2);
    //save current
    USB_DRV_SetBits32(&musb_phy->u2phydtm0, U2PHYDTM0_RG_DPPULLDOWN);
    USB_DRV_SetBits32(&musb_phy->u2phydtm0, U2PHYDTM0_RG_DMPULLDOWN);
    USB_DRV_WriteReg32(&musb_phy->u2phydtm0, (USB_DRV_Reg32(&musb_phy->u2phydtm0) & (~(0x3 << U2PHYDTM0_RG_XCVRSEL_1_0_OFST))) | U2PHYDTM0_RG_XCVRSEL_1_0);
    USB_DRV_SetBits32(&musb_phy->u2phydtm0, U2PHYDTM0_RG_TERMSEL);
    USB_DRV_WriteReg32(&musb_phy->u2phydtm0, USB_DRV_Reg32(&musb_phy->u2phydtm0) & (~(0xF << U2PHYDTM0_RG_DATAIN_3_0_OFST)));
    USB_DRV_SetBits32(&musb_phy->u2phydtm0, U2PHYDTM0_force_dp_pulldown);
    USB_DRV_SetBits32(&musb_phy->u2phydtm0, U2PHYDTM0_force_dm_pulldown);
    USB_DRV_SetBits32(&musb_phy->u2phydtm0, U2PHYDTM0_force_xcvrsel);
    USB_DRV_SetBits32(&musb_phy->u2phydtm0, U2PHYDTM0_force_termsel);
    USB_DRV_SetBits32(&musb_phy->u2phydtm0, U2PHYDTM0_force_datain);
    //DP/DM BC1.1 path Disable
    USB_DRV_ClearBits32(&musb_phy->u2phyacr3, U2PHYACR3_RG_USB20_PHY_REV_7);
    //OTG Disable
    USB_DRV_ClearBits32(&musb_phy->u2phyacr2, U2PHYACR2_RG_USB20_OTG_VBUSCMP_EN);
    //wait 800us
    hal_gpt_delay_ms(1);
    //(let suspendm=0, set utmi into analog power down )
    USB_DRV_ClearBits32(&musb_phy->u2phydtm0, U2PHYDTM0_RG_SUSPENDM);
    //wait 1us
    hal_gpt_delay_ms(1);
    //Turn off internal 48Mhz PLL if there is no other hardware module is using  the 48Mhz clock -the control register is in clock document
    //@@@@@@@@@@@
    //A2D LVSH ENABLE
    USB_DRV_ClearBits32(&musb_phy->u2phyac0, U2PHYAC0_RG_USB_LVSH_EN);
    //Turn off AVDD33_USB(PHY 3.3v power) ?the control register is in PMIC document
    //@@@@@@@@@@@

}

void hal_usbphy_recover(void)
{
    //A2D LVSH ENABLE
    USB_DRV_SetBits32(&musb_phy->u2phyac0, U2PHYAC0_RG_USB_LVSH_EN);
    //switch to USB function. (system register, force ip into usb mode)
    USB_DRV_ClearBits32(&musb_phy->u2phydtm0, U2PHYDTM0_force_uart_en);
    USB_DRV_ClearBits32(&musb_phy->u2phydtm1, U2PHYDTM1_RG_UART_EN);
    USB_DRV_ClearBits32(&musb_phy->u2phyacr4, U2PHYACR4_rg_usb20_gpio_ctl);
    USB_DRV_ClearBits32(&musb_phy->u2phyacr4, U2PHYACR4_usb20_gpio_mode);
    //Release force suspendm. ? (force_suspendm=0) (let suspendm=1, enable usb 480MHz pll)
    USB_DRV_ClearBits32(&musb_phy->u2phydtm0, U2PHYDTM0_force_suspendm);
    //recover
    USB_DRV_ClearBits32(&musb_phy->u2phydtm0, U2PHYDTM0_RG_DPPULLDOWN);
    USB_DRV_ClearBits32(&musb_phy->u2phydtm0, U2PHYDTM0_RG_DMPULLDOWN);
    USB_DRV_WriteReg32(&musb_phy->u2phydtm0, USB_DRV_Reg32(&musb_phy->u2phydtm0) & (~(0x3 << U2PHYDTM0_RG_XCVRSEL_1_0_OFST)));
    USB_DRV_ClearBits32(&musb_phy->u2phydtm0, U2PHYDTM0_RG_TERMSEL);
    USB_DRV_WriteReg32(&musb_phy->u2phydtm0, USB_DRV_Reg32(&musb_phy->u2phydtm0) & (~(0xF << U2PHYDTM0_RG_DATAIN_3_0_OFST)));
    USB_DRV_ClearBits32(&musb_phy->u2phydtm0, U2PHYDTM0_force_dp_pulldown);
    USB_DRV_ClearBits32(&musb_phy->u2phydtm0, U2PHYDTM0_force_dm_pulldown);
    USB_DRV_ClearBits32(&musb_phy->u2phydtm0, U2PHYDTM0_force_xcvrsel);
    USB_DRV_ClearBits32(&musb_phy->u2phydtm0, U2PHYDTM0_force_termsel);
    USB_DRV_ClearBits32(&musb_phy->u2phydtm0, U2PHYDTM0_force_datain);
    //DP/DM BC1.1 path Disable
    USB_DRV_ClearBits32(&musb_phy->u2phyacr3, U2PHYACR3_RG_USB20_PHY_REV_7);
    //OTG Enable
    USB_DRV_SetBits32(&musb_phy->u2phyacr2, U2PHYACR2_RG_USB20_OTG_VBUSCMP_EN);
    //Wait 800 usec
    hal_gpt_delay_ms(1);
    USB_DRV_WriteReg32(&musb_phy->u2phydtm1, 0x00003E2E);
}

#define USB_HS_SLEW_RATE_CAL_TIME_WINDOW 0x400
#define USB_HS_SLEW_RATE_CAL_A 32
#define USB_HS_SLEW_RATE_CAL_FRA (1000)

void hal_usbphy_slew_rate_calibration(void)
{
    double freq, x;
    uint32_t data = 0;
    uint32_t timeout = 0;
    uint32_t b = 0;

    /*printf("\r\n\r\n\r\n\r\n\r\n");*/
    /*printf("hal_usbphy_slew_rate_calibration - before 0x%x = 0x%.8x\r\n", &musb_phy->u2phyacr0, USB_DRV_Reg32(&musb_phy->u2phyacr0));*/

    //enable USB ring oscillator
    USB_DRV_SetBits32(&musb_phy->u2phyacr0, U2PHYACR0_RG_USB20_HSTX_SRCAL_EN);
    hal_gpt_delay_ms(1);
    //Enable free run clock
    USB_DRV_SetBits32(&musb_phy->fmmonr1, USB_FMMONR1_RG_FRCK_EN);
    //RG_MONCLK_SEL = 00, 60MHz clock
    USB_DRV_WriteReg32(&musb_phy->fmcr0, (1 << 24) | (USB_HS_SLEW_RATE_CAL_TIME_WINDOW));

    //check USB_FM_VLD
    //while ((USB_DRV_Reg32(&musb_phy->fmmonr1) & 0x1) != 0x1) {
    hal_gpt_delay_ms(5);
    while ((USB_DRV_Reg8(&musb_phy->fmmonr1)) != 0x1) {
        timeout++;
        if (timeout == 500) {
            log_hal_error("USB slew rate calibration timeout\n");
            break;
        }
    }

    data = USB_DRV_Reg32(&musb_phy->fmmonr0);
    freq = (double)((48 * USB_HS_SLEW_RATE_CAL_TIME_WINDOW) / (double)data);
    x = USB_HS_SLEW_RATE_CAL_A * freq / USB_HS_SLEW_RATE_CAL_FRA;
    b = (int)(x + (double)0.5);

    /*printf("hal_usbphy_slew_rate_calibration - data 0x%x\r\n", data);*/
    /*printf("hal_usbphy_slew_rate_calibration - 48* (%d/%d) * 0.0%d = %d\r\n",
                            USB_HS_SLEW_RATE_CAL_TIME_WINDOW,
                            data,
                            USB_HS_SLEW_RATE_CAL_A,
                            b);*/
    if (b > 0x7) {
        b = 0x07;
        log_hal_error("USB slew rate calibration overflow - fix value to 0x07\r\n");
    }

    //Disable free run clock
    USB_DRV_WriteReg32(&musb_phy->fmcr0, USB_DRV_Reg32(&musb_phy->fmcr0) & 0xFEFFFFFF);
    USB_DRV_ClearBits32(&musb_phy->fmmonr1, USB_FMMONR1_RG_FRCK_EN);
    USB_DRV_WriteReg32(&musb_phy->u2phyacr0, (USB_DRV_Reg32(&musb_phy->u2phyacr0) & ~U2PHYACR0_RG_USB20_HSTX_SRCTRL_CLR) | ((b & 0x7) << 16));
    //Disable USB ring oscillator
    USB_DRV_ClearBits32(&musb_phy->u2phyacr0, U2PHYACR0_RG_USB20_HSTX_SRCAL_EN);
    /*printf("hal_usbphy_slew_rate_calibration - after 0x%x = 0x%.8x\r\n", &musb_phy->u2phyacr0, USB_DRV_Reg32(&musb_phy->u2phyacr0));*/
    /*printf("\r\n\r\n\r\n\r\n\r\n");*/
}

#else // USB_FPGA_DVT : Real Chip

void hal_usbphy_poweron_initialize(void)
{
    /* Setting from SE:Dora */
    uint32_t    *reg ;

    reg = (volatile uint32_t *)0x42040B80;
    *reg = 0x00100000;
    reg = (volatile uint32_t *)0x42050200;
    *reg = 0x01010101;
    reg = (volatile uint32_t *)0x42050208;
    *reg = 0x000E0101;
    reg = (volatile uint32_t *)0x42050250;
    *reg = 0x01010101;
    reg = (volatile uint32_t *)0x42050258;
    *reg = 0x000E0102;
    hal_gpt_delay_ms(10);

    reg = (volatile uint32_t *)0x42050008;
    *reg = 0x01000101;
    reg = (volatile uint32_t *)0x4205000C;
    *reg = 0x01000101;
    reg = (volatile uint32_t *)0x4205010C;
    *reg = 0x00000000;
    reg = (volatile uint32_t *)0x42050120;
    *reg = 0x00000101;
    hal_gpt_delay_ms(5);

    reg = (volatile uint32_t *)0x4205011C;
    *reg = 0x00060001;
    reg = (volatile uint32_t *)0x42050124;
    *reg = 0x00000100;
    reg = (volatile uint32_t *)0x42050100;
    *reg = 0x00010000;
    reg = (volatile uint32_t *)0x42050104;
    *reg = 0x01010018;
    reg = (volatile uint32_t *)0x42050100;
    *reg = 0x00010001;
    hal_gpt_delay_ms(5);
    reg = (volatile uint32_t *)0x42050004;
    *reg = 0x01000101;
    hal_gpt_delay_ms(10);

    //UPLL into USB Module
    reg = (volatile uint32_t *)0x42080080;
    *reg = 0x00000100;
    hal_gpt_delay_ms(350);
    *reg = 0x00000110;
    hal_gpt_delay_ms(200);
    *reg = 0x00000111;

    //PMU USB control2     USB VBUS
    reg = (volatile uint32_t *)0x41030868;
    *reg = 0x00040000;
    reg = (volatile uint32_t *)0x41030818;
    //*reg = 0x00000482;
    //*reg = 0x000C0482;
    *reg = 0x001F0482;
    reg = (volatile uint32_t *)0x41030868;
    *reg = 0x00040094;
    reg = (volatile uint32_t *)0x41030868;
    *reg = 0x00FF0094;
    hal_gpt_delay_ms(5);

    //Check 30MHz PHY Clock at GPIO22
    reg = (volatile uint32_t *)0x41030860;
    *reg = 0x00000002;
    reg = (volatile uint32_t *)0x41030864;
    *reg = 0x17040001;
    reg = (volatile uint32_t *)0x4103086C;
    *reg = 0x00000000;
    reg = (volatile uint32_t *)0x42010008;
    *reg = 0x0000000E;
    reg = (volatile uint32_t *)0x41030868;
    //*reg = 0x00FF001C;
    *reg = 0x0000001C; // no force
    reg = (volatile uint32_t *)0x41030860;
    *reg = 0x02000002;

    // Refine Eye
    reg = (volatile uint32_t *)0x41030814;
    *reg |= 0x00007000;

    /*  */
    DRV_WriteReg32(0xA2090070, 0x01220121);
    hal_gpt_delay_ms(1);
    DRV_WriteReg32(0xA2090070, 0x00220021);

    DRV_WriteReg32((USB_PHY_MSTAR + 0x868), 0x1420A620); //U2PLL ebable
    DRV_WriteReg32((USB_PHY_MSTAR + 0x808), 0x80404084); //utmi
    DRV_WriteReg32((USB_PHY_MSTAR + 0x81C), 0x008880A1); //utmi
    DRV_WriteReg32((USB_PHY_MSTAR + 0x800), 0x94006BC3); //utmi
    hal_gpt_delay_us(2000);
    DRV_WriteReg32((USB_PHY_MSTAR + 0x800), 0x940069C3); //utmi
    hal_gpt_delay_us(2000);
    DRV_WriteReg32((USB_PHY_MSTAR + 0x800), 0x94000001); //utmi
    hal_gpt_delay_us(2000);
    DRV_WriteReg32((USB_PHY_MSTAR + 0x810), 0x01FF0040); //utmi
    DRV_WriteReg32((USB_PHY_MSTAR + 0x810), 0x00FF0040); //utmi
    DRV_WriteReg32((USB_PHY_MSTAR + 0x808), 0x80404004); //utmi
    hal_gpt_delay_us(7000);
    DRV_WriteReg32((USB_PHY_MSTAR + 0x838), 0x00000001); //utmi
    DRV_WriteReg32((USB_PHY_MSTAR + 0x838), 0x00000000); //utmi

    DRV_WriteReg32((USB_PHY_MSTAR + 0x800), 0x28840001); //utmi
    DRV_WriteReg32((USB_PHY_MSTAR + 0x804), 0x00403064); //utmi
    DRV_WriteReg32((USB_PHY_MSTAR + 0x808), 0x8040D507); //utmi
    DRV_WriteReg32((USB_PHY_MSTAR + 0x804), 0x00433064); //utmi
    DRV_WriteReg32((USB_PHY_MSTAR + 0x804), 0x00403064); //utmi
    DRV_WriteReg32((USB_PHY_MSTAR + 0x814), 0x0000000B); //utmi
    USB_DRV_SetBits32((USB_PHY_MSTAR + 0x828), (1 << 9));
    USB_DRV_SetBits32((USB_PHY_MSTAR + 0x828), (1 << 7));
}

void hal_usbphy_save_current(void)
{

}

void hal_usbphy_save_current_for_charge(void)
{

}

void hal_usbphy_recover(void)
{

}

#define USB_HS_SLEW_RATE_CAL_TIME_WINDOW 0x400
#define USB_HS_SLEW_RATE_CAL_A 32
#define USB_HS_SLEW_RATE_CAL_FRA (1000)

void hal_usbphy_slew_rate_calibration(void)
{

}
#endif /* USB_FPGA_DVT */
#endif /* Dummy code section */

#endif /* HAL_USB_MODULE_ENABLED or HAL_USB_HOST_MODULE_ENABLED */
