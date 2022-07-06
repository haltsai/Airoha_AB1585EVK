/* Copyright Statement:
 *
 * (C) 2021  Airoha Technology Corp. All rights reserved.
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

#include "hal_gpio.h"

#ifdef HAL_GPIO_MODULE_ENABLED
#include "hal_gpio_internal.h"
#include "hal_log.h"

const hal_gpio_cfg_reg_t gpio_cfg_table[] = {
//DRV_REG                 shift IES_REG                 shift PD_REG                  PU_REG                  shift PUPD_REG                R0_REG                  R1_REG                  shift RDSEL_REG               shift RSEL                    shift EH_REG                  shift TDSEL_REG               shift SMT_REG                 SR_REG                  shift G_REG                   shift
    { IO_CFG_0_BASE + 0x0020, 0, IO_CFG_0_BASE + 0x0050, 0, IO_CFG_0_BASE + 0x0060, IO_CFG_0_BASE + 0x0070, 0, 0XFFFFFFFF, 0XFFFFFFFF, 0XFFFFFFFF, 0XFF, IO_CFG_0_BASE + 0x0080, 0, 0XFFFFFFFF, 0XFF, 0XFFFFFFFF, 0XFF, IO_CFG_0_BASE + 0x00B0, 0, IO_CFG_0_BASE + 0x0090, IO_CFG_0_BASE + 0x00A0, 0, IO_CFG_0_BASE + 0x0040, 0    },                                                                               //HAL_GPIO_0
    { IO_CFG_0_BASE + 0x0020, 3, IO_CFG_0_BASE + 0x0050, 1, IO_CFG_0_BASE + 0x0060, IO_CFG_0_BASE + 0x0070, 1, 0XFFFFFFFF, 0XFFFFFFFF, 0XFFFFFFFF, 0XFF, IO_CFG_0_BASE + 0x0080, 2, 0XFFFFFFFF, 0XFF, 0XFFFFFFFF, 0XFF, IO_CFG_0_BASE + 0x00B0, 4, IO_CFG_0_BASE + 0x0090, IO_CFG_0_BASE + 0x00A0, 1, IO_CFG_0_BASE + 0x0040, 1    },                                                                               //HAL_GPIO_1
    { IO_CFG_0_BASE + 0x0020, 6, IO_CFG_0_BASE + 0x0050, 2, IO_CFG_0_BASE + 0x0060, IO_CFG_0_BASE + 0x0070, 2, 0XFFFFFFFF, 0XFFFFFFFF, 0XFFFFFFFF, 0XFF, IO_CFG_0_BASE + 0x0080, 4, 0XFFFFFFFF, 0XFF, 0XFFFFFFFF, 0XFF, IO_CFG_0_BASE + 0x00B0, 8, IO_CFG_0_BASE + 0x0090, IO_CFG_0_BASE + 0x00A0, 2, IO_CFG_0_BASE + 0x0040, 2    },                                                                               //HAL_GPIO_2
    { IO_CFG_0_BASE + 0x0020, 9, IO_CFG_0_BASE + 0x0050, 3, IO_CFG_0_BASE + 0x0060, IO_CFG_0_BASE + 0x0070, 3, 0XFFFFFFFF, 0XFFFFFFFF, 0XFFFFFFFF, 0XFF, IO_CFG_0_BASE + 0x0080, 6, 0XFFFFFFFF, 0XFF, 0XFFFFFFFF, 0XFF, IO_CFG_0_BASE + 0x00B0, 12, IO_CFG_0_BASE + 0x0090, IO_CFG_0_BASE + 0x00A0, 3, IO_CFG_0_BASE + 0x0040, 3    },                                                                              //HAL_GPIO_3
    { IO_CFG_0_BASE + 0x0020, 12, IO_CFG_0_BASE + 0x0050, 4, IO_CFG_0_BASE + 0x0060, IO_CFG_0_BASE + 0x0070, 4, 0XFFFFFFFF, 0XFFFFFFFF, 0XFFFFFFFF, 0XFF, IO_CFG_0_BASE + 0x0080, 8, 0XFFFFFFFF, 0XFF, 0XFFFFFFFF, 0XFF, IO_CFG_0_BASE + 0x00B0, 16, IO_CFG_0_BASE + 0x0090, IO_CFG_0_BASE + 0x00A0, 4, IO_CFG_0_BASE + 0x0040, 4    },                                                                             //HAL_GPIO_4
    { IO_CFG_0_BASE + 0x0020, 15, IO_CFG_0_BASE + 0x0050, 5, IO_CFG_0_BASE + 0x0060, IO_CFG_0_BASE + 0x0070, 5, 0XFFFFFFFF, 0XFFFFFFFF, 0XFFFFFFFF, 0XFF, IO_CFG_0_BASE + 0x0080, 10, 0XFFFFFFFF, 0XFF, 0XFFFFFFFF, 0XFF, IO_CFG_0_BASE + 0x00B0, 20, IO_CFG_0_BASE + 0x0090, IO_CFG_0_BASE + 0x00A0, 5, IO_CFG_0_BASE + 0x0040, 5    },                                                                            //HAL_GPIO_5
    { IO_CFG_0_BASE + 0x0020, 18, IO_CFG_0_BASE + 0x0050, 6, IO_CFG_0_BASE + 0x0060, IO_CFG_0_BASE + 0x0070, 6, 0XFFFFFFFF, 0XFFFFFFFF, 0XFFFFFFFF, 0XFF, IO_CFG_0_BASE + 0x0080, 12, 0XFFFFFFFF, 0XFF, 0XFFFFFFFF, 0XFF, IO_CFG_0_BASE + 0x00B0, 24, IO_CFG_0_BASE + 0x0090, IO_CFG_0_BASE + 0x00A0, 6, IO_CFG_0_BASE + 0x0040, 6    },                                                                            //HAL_GPIO_6
    { IO_CFG_0_BASE + 0x0020, 21, IO_CFG_0_BASE + 0x0050, 7, IO_CFG_0_BASE + 0x0060, IO_CFG_0_BASE + 0x0070, 7, 0XFFFFFFFF, 0XFFFFFFFF, 0XFFFFFFFF, 0XFF, IO_CFG_0_BASE + 0x0080, 14, 0XFFFFFFFF, 0XFF, 0XFFFFFFFF, 0XFF, IO_CFG_0_BASE + 0x00B0, 28, IO_CFG_0_BASE + 0x0090, IO_CFG_0_BASE + 0x00A0, 7, IO_CFG_0_BASE + 0x0040, 7    },                                                                            //HAL_GPIO_7
    { IO_CFG_0_BASE + 0x0020, 24, IO_CFG_0_BASE + 0x0050, 8, IO_CFG_0_BASE + 0x0060, IO_CFG_0_BASE + 0x0070, 8, 0XFFFFFFFF, 0XFFFFFFFF, 0XFFFFFFFF, 0XFF, IO_CFG_0_BASE + 0x0080, 16, 0XFFFFFFFF, 0XFF, 0XFFFFFFFF, 0XFF, IO_CFG_0_BASE + 0x00C0, 0, IO_CFG_0_BASE + 0x0090, IO_CFG_0_BASE + 0x00A0, 8, IO_CFG_0_BASE + 0x0040, 8    },                                                                             //HAL_GPIO_8
    { IO_CFG_0_BASE + 0x0020, 27, IO_CFG_0_BASE + 0x0050, 9, IO_CFG_0_BASE + 0x0060, IO_CFG_0_BASE + 0x0070, 9, 0XFFFFFFFF, 0XFFFFFFFF, 0XFFFFFFFF, 0XFF, IO_CFG_0_BASE + 0x0080, 18, 0XFFFFFFFF, 0XFF, 0XFFFFFFFF, 0XFF, IO_CFG_0_BASE + 0x00C0, 4, IO_CFG_0_BASE + 0x0090, IO_CFG_0_BASE + 0x00A0, 9, IO_CFG_0_BASE + 0x0040, 9    },                                                                             //HAL_GPIO_9
    { IO_CFG_1_BASE + 0x0020, 0, IO_CFG_1_BASE + 0x00B0, 0, IO_CFG_1_BASE + 0x00D0, IO_CFG_1_BASE + 0x0100, 0, 0XFFFFFFFF, 0XFFFFFFFF, 0XFFFFFFFF, 0XFF, IO_CFG_1_BASE + 0x0140, 0, 0XFFFFFFFF, 0XFF, 0XFFFFFFFF, 0XFF, IO_CFG_1_BASE + 0x01E0, 0, IO_CFG_1_BASE + 0x01A0, IO_CFG_1_BASE + 0x01C0, 0, IO_CFG_1_BASE + 0x0090, 0    },                                                                               //HAL_GPIO_10
    { IO_CFG_1_BASE + 0x0020, 3, IO_CFG_1_BASE + 0x00B0, 1, IO_CFG_1_BASE + 0x00D0, IO_CFG_1_BASE + 0x0100, 1, 0XFFFFFFFF, 0XFFFFFFFF, 0XFFFFFFFF, 0XFF, IO_CFG_1_BASE + 0x0140, 2, 0XFFFFFFFF, 0XFF, 0XFFFFFFFF, 0XFF, IO_CFG_1_BASE + 0x01E0, 4, IO_CFG_1_BASE + 0x01A0, IO_CFG_1_BASE + 0x01C0, 1, IO_CFG_1_BASE + 0x0090, 1    },                                                                               //HAL_GPIO_11
    { IO_CFG_1_BASE + 0x0020, 6, IO_CFG_1_BASE + 0x00B0, 2, IO_CFG_1_BASE + 0x00D0, IO_CFG_1_BASE + 0x0100, 2, 0XFFFFFFFF, 0XFFFFFFFF, 0XFFFFFFFF, 0XFF, IO_CFG_1_BASE + 0x0140, 4, 0XFFFFFFFF, 0XFF, 0XFFFFFFFF, 0XFF, IO_CFG_1_BASE + 0x01E0, 8, IO_CFG_1_BASE + 0x01A0, IO_CFG_1_BASE + 0x01C0, 2, IO_CFG_1_BASE + 0x0090, 2    },                                                                               //HAL_GPIO_12
    { IO_CFG_1_BASE + 0x0020, 9, IO_CFG_1_BASE + 0x00B0, 3, IO_CFG_1_BASE + 0x00D0, IO_CFG_1_BASE + 0x0100, 3, 0XFFFFFFFF, 0XFFFFFFFF, 0XFFFFFFFF, 0XFF, IO_CFG_1_BASE + 0x0140, 6, IO_CFG_1_BASE + 0x0190,    0, IO_CFG_1_BASE + 0x0080, 0, IO_CFG_1_BASE + 0x01E0, 12, IO_CFG_1_BASE + 0x01A0, IO_CFG_1_BASE + 0x01C0, 3, 0XFFFFFFFF, 0XFF },                                                                     //HAL_GPIO_13
    { IO_CFG_1_BASE + 0x0020, 12, IO_CFG_1_BASE + 0x00B0, 4, IO_CFG_1_BASE + 0x00D0, IO_CFG_1_BASE + 0x0100, 4, 0XFFFFFFFF, 0XFFFFFFFF, 0XFFFFFFFF, 0XFF, IO_CFG_1_BASE + 0x0140, 8, IO_CFG_1_BASE + 0x0190,    2, IO_CFG_1_BASE + 0x0080, 3, IO_CFG_1_BASE + 0x01E0, 16, IO_CFG_1_BASE + 0x01A0, IO_CFG_1_BASE + 0x01C0, 4, 0XFFFFFFFF, 0XFF },                                                                    //HAL_GPIO_14
    { IO_CFG_1_BASE + 0x0020, 15, IO_CFG_1_BASE + 0x00B0, 5, IO_CFG_1_BASE + 0x00D0, IO_CFG_1_BASE + 0x0100, 5, 0XFFFFFFFF, 0XFFFFFFFF, 0XFFFFFFFF, 0XFF, IO_CFG_1_BASE + 0x0140, 10, IO_CFG_1_BASE + 0x0190,    4, IO_CFG_1_BASE + 0x0080, 6, IO_CFG_1_BASE + 0x01E0, 20, IO_CFG_1_BASE + 0x01A0, IO_CFG_1_BASE + 0x01C0, 5, 0XFFFFFFFF, 0XFF },                                                                   //HAL_GPIO_15
    { IO_CFG_1_BASE + 0x0020, 18, IO_CFG_1_BASE + 0x00B0, 6, IO_CFG_1_BASE + 0x00D0, IO_CFG_1_BASE + 0x0100, 6, 0XFFFFFFFF, 0XFFFFFFFF, 0XFFFFFFFF, 0XFF, IO_CFG_1_BASE + 0x0140, 12, IO_CFG_1_BASE + 0x0190,    6, IO_CFG_1_BASE + 0x0080, 9, IO_CFG_1_BASE + 0x01E0, 24, IO_CFG_1_BASE + 0x01A0, IO_CFG_1_BASE + 0x01C0, 6, 0XFFFFFFFF, 0XFF },                                                                   //HAL_GPIO_16
    { IO_CFG_1_BASE + 0x0020, 21, IO_CFG_1_BASE + 0x00B0, 7, IO_CFG_1_BASE + 0x00D0, IO_CFG_1_BASE + 0x0100, 7, 0XFFFFFFFF, 0XFFFFFFFF, 0XFFFFFFFF, 0XFF, IO_CFG_1_BASE + 0x0140, 14, IO_CFG_1_BASE + 0x0190,    8, IO_CFG_1_BASE + 0x0080, 12, IO_CFG_1_BASE + 0x01E0, 28, IO_CFG_1_BASE + 0x01A0, IO_CFG_1_BASE + 0x01C0, 7, 0XFFFFFFFF, 0XFF },                                                                  //HAL_GPIO_17
    { IO_CFG_1_BASE + 0x0020, 24, IO_CFG_1_BASE + 0x00B0, 8, IO_CFG_1_BASE + 0x00D0, IO_CFG_1_BASE + 0x0100, 8, 0XFFFFFFFF, 0XFFFFFFFF, 0XFFFFFFFF, 0XFF, IO_CFG_1_BASE + 0x0140, 16, IO_CFG_1_BASE + 0x0190,   10, IO_CFG_1_BASE + 0x0080, 15, IO_CFG_1_BASE + 0x01F0, 0, IO_CFG_1_BASE + 0x01A0, IO_CFG_1_BASE + 0x01C0, 8, 0XFFFFFFFF, 0XFF },                                                                   //HAL_GPIO_18
    { IO_CFG_1_BASE + 0x0020, 27, IO_CFG_1_BASE + 0x00B0, 9, 0XFFFFFFFF, 0XFFFFFFFF, 0XFF, IO_CFG_1_BASE + 0x00F0, IO_CFG_1_BASE + 0x0120, IO_CFG_1_BASE + 0x0130, 0, IO_CFG_1_BASE + 0x0140, 18, 0XFFFFFFFF, 0XFF, 0XFFFFFFFF, 0XFF, IO_CFG_1_BASE + 0x01F0, 4, IO_CFG_1_BASE + 0x01A0, IO_CFG_1_BASE + 0x01C0, 9, 0XFFFFFFFF, 0XFF },                                                                             //HAL_GPIO_19
    { IO_CFG_1_BASE + 0x0030, 0, IO_CFG_1_BASE + 0x00B0, 10, 0XFFFFFFFF, 0XFFFFFFFF, 0XFF, IO_CFG_1_BASE + 0x00F0, IO_CFG_1_BASE + 0x0120, IO_CFG_1_BASE + 0x0130, 1, IO_CFG_1_BASE + 0x0140, 24, 0XFFFFFFFF, 0XFF, 0XFFFFFFFF, 0XFF, IO_CFG_1_BASE + 0x01F0, 8, IO_CFG_1_BASE + 0x01A0, IO_CFG_1_BASE + 0x01C0, 10, 0XFFFFFFFF, 0XFF },                                                                            //HAL_GPIO_20
    { IO_CFG_1_BASE + 0x0030, 3, IO_CFG_1_BASE + 0x00B0, 11, 0XFFFFFFFF, 0XFFFFFFFF, 0XFF, IO_CFG_1_BASE + 0x00F0, IO_CFG_1_BASE + 0x0120, IO_CFG_1_BASE + 0x0130, 2, IO_CFG_1_BASE + 0x0150, 0, 0XFFFFFFFF, 0XFF, 0XFFFFFFFF, 0XFF, IO_CFG_1_BASE + 0x01F0, 12, IO_CFG_1_BASE + 0x01A0, IO_CFG_1_BASE + 0x01C0, 11, 0XFFFFFFFF, 0XFF },                                                                            //HAL_GPIO_21
    { IO_CFG_1_BASE + 0x0030, 6, IO_CFG_1_BASE + 0x00B0, 12, 0XFFFFFFFF, 0XFFFFFFFF, 0XFF, IO_CFG_1_BASE + 0x00F0, IO_CFG_1_BASE + 0x0120, IO_CFG_1_BASE + 0x0130, 3, IO_CFG_1_BASE + 0x0150, 6, 0XFFFFFFFF, 0XFF, 0XFFFFFFFF, 0XFF, IO_CFG_1_BASE + 0x01F0, 16, IO_CFG_1_BASE + 0x01A0, IO_CFG_1_BASE + 0x01C0, 12, 0XFFFFFFFF, 0XFF },                                                                            //HAL_GPIO_22
    { IO_CFG_1_BASE + 0x0030, 9, IO_CFG_1_BASE + 0x00B0, 13, 0XFFFFFFFF, 0XFFFFFFFF, 0XFF, IO_CFG_1_BASE + 0x00F0, IO_CFG_1_BASE + 0x0120, IO_CFG_1_BASE + 0x0130, 4, IO_CFG_1_BASE + 0x0150, 12, 0XFFFFFFFF, 0XFF, 0XFFFFFFFF, 0XFF, IO_CFG_1_BASE + 0x01F0, 20, IO_CFG_1_BASE + 0x01A0, IO_CFG_1_BASE + 0x01C0, 13, 0XFFFFFFFF, 0XFF },                                                                           //HAL_GPIO_23
    { IO_CFG_1_BASE + 0x0030, 12, IO_CFG_1_BASE + 0x00B0, 14, 0XFFFFFFFF, 0XFFFFFFFF, 0XFF, IO_CFG_1_BASE + 0x00F0, IO_CFG_1_BASE + 0x0120, IO_CFG_1_BASE + 0x0130, 5, IO_CFG_1_BASE + 0x0150, 18, 0XFFFFFFFF, 0XFF, 0XFFFFFFFF, 0XFF, IO_CFG_1_BASE + 0x01F0, 24, IO_CFG_1_BASE + 0x01A0, IO_CFG_1_BASE + 0x01C0, 14, 0XFFFFFFFF, 0XFF },                                                                          //HAL_GPIO_24
    { IO_CFG_1_BASE + 0x0030, 15, IO_CFG_1_BASE + 0x00B0, 15, 0XFFFFFFFF, 0XFFFFFFFF, 0XFF, IO_CFG_1_BASE + 0x00F0, IO_CFG_1_BASE + 0x0120, IO_CFG_1_BASE + 0x0130, 6, IO_CFG_1_BASE + 0x0150, 24, 0XFFFFFFFF, 0XFF, 0XFFFFFFFF, 0XFF, IO_CFG_1_BASE + 0x01F0, 28, IO_CFG_1_BASE + 0x01A0, IO_CFG_1_BASE + 0x01C0, 15, 0XFFFFFFFF, 0XFF },                                                                          //HAL_GPIO_25
    { IO_CFG_1_BASE + 0x0030, 18, IO_CFG_1_BASE + 0x00B0, 16, IO_CFG_1_BASE + 0x00D0, IO_CFG_1_BASE + 0x0100, 9, 0XFFFFFFFF, 0XFFFFFFFF, 0XFFFFFFFF, 0XFF, IO_CFG_1_BASE + 0x0150, 30, 0XFFFFFFFF, 0XFF, 0XFFFFFFFF, 0XFF, IO_CFG_1_BASE + 0x0200, 0, IO_CFG_1_BASE + 0x01A0, IO_CFG_1_BASE + 0x01C0, 16, IO_CFG_1_BASE + 0x0090, 3    },                                                                           //HAL_GPIO_26
    { IO_CFG_1_BASE + 0x0030, 21, IO_CFG_1_BASE + 0x00B0, 17, IO_CFG_1_BASE + 0x00D0, IO_CFG_1_BASE + 0x0100, 10, 0XFFFFFFFF, 0XFFFFFFFF, 0XFFFFFFFF, 0XFF, IO_CFG_1_BASE + 0x0160, 0, 0XFFFFFFFF, 0XFF, 0XFFFFFFFF, 0XFF, IO_CFG_1_BASE + 0x0200, 4, IO_CFG_1_BASE + 0x01A0, IO_CFG_1_BASE + 0x01C0, 17, IO_CFG_1_BASE + 0x0090, 4    },                                                                           //HAL_GPIO_27
    { IO_CFG_1_BASE + 0x0030, 24, IO_CFG_1_BASE + 0x00B0, 18, IO_CFG_1_BASE + 0x00D0, IO_CFG_1_BASE + 0x0100, 11, 0XFFFFFFFF, 0XFFFFFFFF, 0XFFFFFFFF, 0XFF, IO_CFG_1_BASE + 0x0160, 2, 0XFFFFFFFF, 0XFF, 0XFFFFFFFF, 0XFF, IO_CFG_1_BASE + 0x0200, 8, IO_CFG_1_BASE + 0x01A0, IO_CFG_1_BASE + 0x01C0, 18, IO_CFG_1_BASE + 0x0090, 5    },                                                                           //HAL_GPIO_28
    { IO_CFG_1_BASE + 0x0030, 27, IO_CFG_1_BASE + 0x00B0, 19, IO_CFG_1_BASE + 0x00D0, IO_CFG_1_BASE + 0x0100, 12, 0XFFFFFFFF, 0XFFFFFFFF, 0XFFFFFFFF, 0XFF, IO_CFG_1_BASE + 0x0160, 4, 0XFFFFFFFF, 0XFF, 0XFFFFFFFF, 0XFF, IO_CFG_1_BASE + 0x0200, 12, IO_CFG_1_BASE + 0x01A0, IO_CFG_1_BASE + 0x01C0, 19, IO_CFG_1_BASE + 0x0090, 6    },                                                                          //HAL_GPIO_29
    { IO_CFG_1_BASE + 0x0040, 0, IO_CFG_1_BASE + 0x00B0, 20, IO_CFG_1_BASE + 0x00D0, IO_CFG_1_BASE + 0x0100, 13, 0XFFFFFFFF, 0XFFFFFFFF, 0XFFFFFFFF, 0XFF, IO_CFG_1_BASE + 0x0160, 6, 0XFFFFFFFF, 0XFF, 0XFFFFFFFF, 0XFF, IO_CFG_1_BASE + 0x0200, 16, IO_CFG_1_BASE + 0x01A0, IO_CFG_1_BASE + 0x01C0, 20, IO_CFG_1_BASE + 0x0090, 7    },                                                                           //HAL_GPIO_30
    { IO_CFG_1_BASE + 0x0040, 3, IO_CFG_1_BASE + 0x00B0, 21, IO_CFG_1_BASE + 0x00D0, IO_CFG_1_BASE + 0x0100, 14, 0XFFFFFFFF, 0XFFFFFFFF, 0XFFFFFFFF, 0XFF, IO_CFG_1_BASE + 0x0160, 8, 0XFFFFFFFF, 0XFF, 0XFFFFFFFF, 0XFF, IO_CFG_1_BASE + 0x0200, 20, IO_CFG_1_BASE + 0x01A0, IO_CFG_1_BASE + 0x01C0, 21, IO_CFG_1_BASE + 0x0090, 8    },                                                                           //HAL_GPIO_31
    { IO_CFG_1_BASE + 0x0040, 6, IO_CFG_1_BASE + 0x00B0, 22, IO_CFG_1_BASE + 0x00D0, IO_CFG_1_BASE + 0x0100, 15, 0XFFFFFFFF, 0XFFFFFFFF, 0XFFFFFFFF, 0XFF, IO_CFG_1_BASE + 0x0160, 10, 0XFFFFFFFF, 0XFF, 0XFFFFFFFF, 0XFF, IO_CFG_1_BASE + 0x0200, 24, IO_CFG_1_BASE + 0x01A0, IO_CFG_1_BASE + 0x01C0, 22, IO_CFG_1_BASE + 0x0090, 9    },                                                                          //HAL_GPIO_32
    { IO_CFG_1_BASE + 0x0040, 9, IO_CFG_1_BASE + 0x00B0, 23, IO_CFG_1_BASE + 0x00D0, IO_CFG_1_BASE + 0x0100, 16, 0XFFFFFFFF, 0XFFFFFFFF, 0XFFFFFFFF, 0XFF, IO_CFG_1_BASE + 0x0160, 12, 0XFFFFFFFF, 0XFF, 0XFFFFFFFF, 0XFF, IO_CFG_1_BASE + 0x0200, 28, IO_CFG_1_BASE + 0x01A0, IO_CFG_1_BASE + 0x01C0, 23, IO_CFG_1_BASE + 0x0090, 10   },                                                                          //HAL_GPIO_33
    { IO_CFG_1_BASE + 0x0040, 12, IO_CFG_1_BASE + 0x00B0, 24, IO_CFG_1_BASE + 0x00D0, IO_CFG_1_BASE + 0x0100, 17, 0XFFFFFFFF, 0XFFFFFFFF, 0XFFFFFFFF, 0XFF, IO_CFG_1_BASE + 0x0160, 14, 0XFFFFFFFF, 0XFF, 0XFFFFFFFF, 0XFF, IO_CFG_1_BASE + 0x0210, 0, IO_CFG_1_BASE + 0x01A0, IO_CFG_1_BASE + 0x01C0, 24, IO_CFG_1_BASE + 0x0090, 11   },                                                                          //HAL_GPIO_34
    { IO_CFG_1_BASE + 0x0040, 15, IO_CFG_1_BASE + 0x00B0, 25, IO_CFG_1_BASE + 0x00D0, IO_CFG_1_BASE + 0x0100, 18, 0XFFFFFFFF, 0XFFFFFFFF, 0XFFFFFFFF, 0XFF, IO_CFG_1_BASE + 0x0160, 16, 0XFFFFFFFF, 0XFF, 0XFFFFFFFF, 0XFF, IO_CFG_1_BASE + 0x0210, 4, IO_CFG_1_BASE + 0x01A0, IO_CFG_1_BASE + 0x01C0, 25, IO_CFG_1_BASE + 0x0090, 12   },                                                                          //HAL_GPIO_35
    { IO_CFG_1_BASE + 0x0040, 18, IO_CFG_1_BASE + 0x00B0, 26, IO_CFG_1_BASE + 0x00D0, IO_CFG_1_BASE + 0x0100, 19, 0XFFFFFFFF, 0XFFFFFFFF, 0XFFFFFFFF, 0XFF, IO_CFG_1_BASE + 0x0160, 18, 0XFFFFFFFF, 0XFF, 0XFFFFFFFF, 0XFF, IO_CFG_1_BASE + 0x0210, 8, IO_CFG_1_BASE + 0x01A0, IO_CFG_1_BASE + 0x01C0, 26, IO_CFG_1_BASE + 0x0090, 13   },                                                                          //HAL_GPIO_36
    { IO_CFG_1_BASE + 0x0040, 21, IO_CFG_1_BASE + 0x00B0, 27, IO_CFG_1_BASE + 0x00D0, IO_CFG_1_BASE + 0x0100, 20, 0XFFFFFFFF, 0XFFFFFFFF, 0XFFFFFFFF, 0XFF, IO_CFG_1_BASE + 0x0160, 20, 0XFFFFFFFF, 0XFF, 0XFFFFFFFF, 0XFF, IO_CFG_1_BASE + 0x0210, 12, IO_CFG_1_BASE + 0x01A0, IO_CFG_1_BASE + 0x01C0, 27, IO_CFG_1_BASE + 0x0090, 14   },                                                                         //HAL_GPIO_37
    { IO_CFG_1_BASE + 0x0040, 24, IO_CFG_1_BASE + 0x00B0, 28, IO_CFG_1_BASE + 0x00D0, IO_CFG_1_BASE + 0x0100, 21, 0XFFFFFFFF, 0XFFFFFFFF, 0XFFFFFFFF, 0XFF, IO_CFG_1_BASE + 0x0160, 22, 0XFFFFFFFF, 0XFF, 0XFFFFFFFF, 0XFF, IO_CFG_1_BASE + 0x0210, 16, IO_CFG_1_BASE + 0x01A0, IO_CFG_1_BASE + 0x01C0, 28, IO_CFG_1_BASE + 0x0090, 15   },                                                                         //HAL_GPIO_38
    { IO_CFG_1_BASE + 0x0040, 27, IO_CFG_1_BASE + 0x00B0, 29, IO_CFG_1_BASE + 0x00D0, IO_CFG_1_BASE + 0x0100, 22, 0XFFFFFFFF, 0XFFFFFFFF, 0XFFFFFFFF, 0XFF, IO_CFG_1_BASE + 0x0160, 24, 0XFFFFFFFF, 0XFF, 0XFFFFFFFF, 0XFF, IO_CFG_1_BASE + 0x0210, 20, IO_CFG_1_BASE + 0x01A0, IO_CFG_1_BASE + 0x01C0, 29, IO_CFG_1_BASE + 0x0090, 16   },                                                                         //HAL_GPIO_39
    { IO_CFG_1_BASE + 0x0050, 0, IO_CFG_1_BASE + 0x00B0, 30, IO_CFG_1_BASE + 0x00D0, IO_CFG_1_BASE + 0x0100, 23, 0XFFFFFFFF, 0XFFFFFFFF, 0XFFFFFFFF, 0XFF, IO_CFG_1_BASE + 0x0160, 26, 0XFFFFFFFF, 0XFF, 0XFFFFFFFF, 0XFF, IO_CFG_1_BASE + 0x0210, 24, IO_CFG_1_BASE + 0x01A0, IO_CFG_1_BASE + 0x01C0, 30, IO_CFG_1_BASE + 0x0090, 17   },                                                                          //HAL_GPIO_40
    { IO_CFG_1_BASE + 0x0050, 3, IO_CFG_1_BASE + 0x00B0, 31, IO_CFG_1_BASE + 0x00D0, IO_CFG_1_BASE + 0x0100, 24, 0XFFFFFFFF, 0XFFFFFFFF, 0XFFFFFFFF, 0XFF, IO_CFG_1_BASE + 0x0160, 28, 0XFFFFFFFF, 0XFF, 0XFFFFFFFF, 0XFF, IO_CFG_1_BASE + 0x0210, 28, IO_CFG_1_BASE + 0x01A0, IO_CFG_1_BASE + 0x01C0, 31, IO_CFG_1_BASE + 0x0090, 18   },                                                                          //HAL_GPIO_41
    { IO_CFG_1_BASE + 0x0050, 6, IO_CFG_1_BASE + 0x00C0, 0, IO_CFG_1_BASE + 0x00D0, IO_CFG_1_BASE + 0x0100, 25, 0XFFFFFFFF, 0XFFFFFFFF, 0XFFFFFFFF, 0XFF, IO_CFG_1_BASE + 0x0160, 30, 0XFFFFFFFF, 0XFF, 0XFFFFFFFF, 0XFF, IO_CFG_1_BASE + 0x0220, 0, IO_CFG_1_BASE + 0x01B0, IO_CFG_1_BASE + 0x01D0, 0, IO_CFG_1_BASE + 0x0090, 19   },                                                                             //HAL_GPIO_42
    { IO_CFG_1_BASE + 0x0050, 9, IO_CFG_1_BASE + 0x00C0, 1, IO_CFG_1_BASE + 0x00D0, IO_CFG_1_BASE + 0x0100, 26, 0XFFFFFFFF, 0XFFFFFFFF, 0XFFFFFFFF, 0XFF, IO_CFG_1_BASE + 0x0170, 0, 0XFFFFFFFF, 0XFF, 0XFFFFFFFF, 0XFF, IO_CFG_1_BASE + 0x0220, 4, IO_CFG_1_BASE + 0x01B0, IO_CFG_1_BASE + 0x01D0, 1, IO_CFG_1_BASE + 0x0090, 20   },                                                                              //HAL_GPIO_43
    { IO_CFG_1_BASE + 0x0050, 12, IO_CFG_1_BASE + 0x00C0, 2, IO_CFG_1_BASE + 0x00D0, IO_CFG_1_BASE + 0x0100, 27, 0XFFFFFFFF, 0XFFFFFFFF, 0XFFFFFFFF, 0XFF, IO_CFG_1_BASE + 0x0170, 2, 0XFFFFFFFF, 0XFF, 0XFFFFFFFF, 0XFF, IO_CFG_1_BASE + 0x0220, 8, IO_CFG_1_BASE + 0x01B0, IO_CFG_1_BASE + 0x01D0, 2, IO_CFG_1_BASE + 0x0090, 21   },                                                                             //HAL_GPIO_44
    { IO_CFG_1_BASE + 0x0050, 15, IO_CFG_1_BASE + 0x00C0, 3, IO_CFG_1_BASE + 0x00D0, IO_CFG_1_BASE + 0x0100, 28, 0XFFFFFFFF, 0XFFFFFFFF, 0XFFFFFFFF, 0XFF, IO_CFG_1_BASE + 0x0170, 4, 0XFFFFFFFF, 0XFF, 0XFFFFFFFF, 0XFF, IO_CFG_1_BASE + 0x0220, 12, IO_CFG_1_BASE + 0x01B0, IO_CFG_1_BASE + 0x01D0, 3, IO_CFG_1_BASE + 0x0090, 22   },                                                                            //HAL_GPIO_45
    { IO_CFG_1_BASE + 0x0050, 18, IO_CFG_1_BASE + 0x00C0, 4, IO_CFG_1_BASE + 0x00D0, IO_CFG_1_BASE + 0x0100, 29, 0XFFFFFFFF, 0XFFFFFFFF, 0XFFFFFFFF, 0XFF, IO_CFG_1_BASE + 0x0170, 6, 0XFFFFFFFF, 0XFF, 0XFFFFFFFF, 0XFF, IO_CFG_1_BASE + 0x0220, 16, IO_CFG_1_BASE + 0x01B0, IO_CFG_1_BASE + 0x01D0, 4, IO_CFG_1_BASE + 0x0090, 23   },                                                                            //HAL_GPIO_46
    { IO_CFG_1_BASE + 0x0050, 21, IO_CFG_1_BASE + 0x00C0, 5, IO_CFG_1_BASE + 0x00D0, IO_CFG_1_BASE + 0x0100, 30, 0XFFFFFFFF, 0XFFFFFFFF, 0XFFFFFFFF, 0XFF, IO_CFG_1_BASE + 0x0170, 8, IO_CFG_1_BASE + 0x0190,   12, IO_CFG_1_BASE + 0x0080, 18, IO_CFG_1_BASE + 0x0220, 20, IO_CFG_1_BASE + 0x01B0, IO_CFG_1_BASE + 0x01D0, 5, 0XFFFFFFFF, 0XFF },                                                                  //HAL_GPIO_47
    { IO_CFG_1_BASE + 0x0050, 24, IO_CFG_1_BASE + 0x00C0, 6, IO_CFG_1_BASE + 0x00D0, IO_CFG_1_BASE + 0x0100, 31, 0XFFFFFFFF, 0XFFFFFFFF, 0XFFFFFFFF, 0XFF, IO_CFG_1_BASE + 0x0170, 10, IO_CFG_1_BASE + 0x0190,   14, IO_CFG_1_BASE + 0x0080, 21, IO_CFG_1_BASE + 0x0220, 24, IO_CFG_1_BASE + 0x01B0, IO_CFG_1_BASE + 0x01D0, 6, 0XFFFFFFFF, 0XFF },                                                                 //HAL_GPIO_48
    { IO_CFG_1_BASE + 0x0050, 27, IO_CFG_1_BASE + 0x00C0, 7, IO_CFG_1_BASE + 0x00E0, IO_CFG_1_BASE + 0x0110, 0, 0XFFFFFFFF, 0XFFFFFFFF, 0XFFFFFFFF, 0XFF, IO_CFG_1_BASE + 0x0170, 12, IO_CFG_1_BASE + 0x0190,   16, IO_CFG_1_BASE + 0x0080, 24, IO_CFG_1_BASE + 0x0220, 28, IO_CFG_1_BASE + 0x01B0, IO_CFG_1_BASE + 0x01D0, 7, 0XFFFFFFFF, 0XFF },                                                                  //HAL_GPIO_49
    { IO_CFG_1_BASE + 0x0060, 0, IO_CFG_1_BASE + 0x00C0, 8, IO_CFG_1_BASE + 0x00E0, IO_CFG_1_BASE + 0x0110, 1, 0XFFFFFFFF, 0XFFFFFFFF, 0XFFFFFFFF, 0XFF, IO_CFG_1_BASE + 0x0170, 14, IO_CFG_1_BASE + 0x0190,   18, IO_CFG_1_BASE + 0x0080, 27, IO_CFG_1_BASE + 0x0230, 0, IO_CFG_1_BASE + 0x01B0, IO_CFG_1_BASE + 0x01D0, 8, 0XFFFFFFFF, 0XFF },                                                                    //HAL_GPIO_50
    { IO_CFG_1_BASE + 0x0060, 3, IO_CFG_1_BASE + 0x00C0, 9, IO_CFG_1_BASE + 0x00E0, IO_CFG_1_BASE + 0x0110, 2, 0XFFFFFFFF, 0XFFFFFFFF, 0XFFFFFFFF, 0XFF, IO_CFG_1_BASE + 0x0170, 16, 0XFFFFFFFF, 0XFF, 0XFFFFFFFF, 0XFF, IO_CFG_1_BASE + 0x0230, 4, IO_CFG_1_BASE + 0x01B0, IO_CFG_1_BASE + 0x01D0, 9, IO_CFG_1_BASE + 0x0090, 24   },                                                                              //HAL_GPIO_51
    { IO_CFG_1_BASE + 0x0060, 6, IO_CFG_1_BASE + 0x00C0, 10, IO_CFG_1_BASE + 0x00E0, IO_CFG_1_BASE + 0x0110, 3, 0XFFFFFFFF, 0XFFFFFFFF, 0XFFFFFFFF, 0XFF, IO_CFG_1_BASE + 0x0170, 18, 0XFFFFFFFF, 0XFF, 0XFFFFFFFF, 0XFF, IO_CFG_1_BASE + 0x0230, 8, IO_CFG_1_BASE + 0x01B0, IO_CFG_1_BASE + 0x01D0, 10, IO_CFG_1_BASE + 0x0090, 25   },                                                                            //HAL_GPIO_52
    { IO_CFG_1_BASE + 0x0060, 9, IO_CFG_1_BASE + 0x00C0, 11, IO_CFG_1_BASE + 0x00E0, IO_CFG_1_BASE + 0x0110, 4, 0XFFFFFFFF, 0XFFFFFFFF, 0XFFFFFFFF, 0XFF, IO_CFG_1_BASE + 0x0170, 20, 0XFFFFFFFF, 0XFF, 0XFFFFFFFF, 0XFF, IO_CFG_1_BASE + 0x0230, 12, IO_CFG_1_BASE + 0x01B0, IO_CFG_1_BASE + 0x01D0, 11, IO_CFG_1_BASE + 0x0090, 26   },                                                                           //HAL_GPIO_53
    { IO_CFG_1_BASE + 0x0060, 12, IO_CFG_1_BASE + 0x00C0, 12, IO_CFG_1_BASE + 0x00E0, IO_CFG_1_BASE + 0x0110, 5, 0XFFFFFFFF, 0XFFFFFFFF, 0XFFFFFFFF, 0XFF, IO_CFG_1_BASE + 0x0170, 22, 0XFFFFFFFF, 0XFF, 0XFFFFFFFF, 0XFF, IO_CFG_1_BASE + 0x0230, 16, IO_CFG_1_BASE + 0x01B0, IO_CFG_1_BASE + 0x01D0, 12, IO_CFG_1_BASE + 0x0090, 27   },                                                                          //HAL_GPIO_54
    { IO_CFG_1_BASE + 0x0060, 15, IO_CFG_1_BASE + 0x00C0, 13, IO_CFG_1_BASE + 0x00E0, IO_CFG_1_BASE + 0x0110, 6, 0XFFFFFFFF, 0XFFFFFFFF, 0XFFFFFFFF, 0XFF, IO_CFG_1_BASE + 0x0170, 24, 0XFFFFFFFF, 0XFF, 0XFFFFFFFF, 0XFF, IO_CFG_1_BASE + 0x0230, 20, IO_CFG_1_BASE + 0x01B0, IO_CFG_1_BASE + 0x01D0, 13, IO_CFG_1_BASE + 0x0090, 28   },                                                                          //HAL_GPIO_55
    { IO_CFG_0_BASE + 0x0030, 0, IO_CFG_0_BASE + 0x0050, 10, IO_CFG_0_BASE + 0x0060, IO_CFG_0_BASE + 0x0070, 10, 0XFFFFFFFF, 0XFFFFFFFF, 0XFFFFFFFF, 0XFF, IO_CFG_0_BASE + 0x0080, 20, 0XFFFFFFFF, 0XFF, 0XFFFFFFFF, 0XFF, IO_CFG_0_BASE + 0x00C0, 8, IO_CFG_0_BASE + 0x0090, IO_CFG_0_BASE + 0x00A0, 10, IO_CFG_0_BASE + 0x0040, 10   },                                                                           //HAL_GPIO_56
    { IO_CFG_0_BASE + 0x0030, 3, IO_CFG_0_BASE + 0x0050, 11, IO_CFG_0_BASE + 0x0060, IO_CFG_0_BASE + 0x0070, 11, 0XFFFFFFFF, 0XFFFFFFFF, 0XFFFFFFFF, 0XFF, IO_CFG_0_BASE + 0x0080, 22, 0XFFFFFFFF, 0XFF, 0XFFFFFFFF, 0XFF, IO_CFG_0_BASE + 0x00C0, 12, IO_CFG_0_BASE + 0x0090, IO_CFG_0_BASE + 0x00A0, 11, IO_CFG_0_BASE + 0x0040, 11   },                                                                          //HAL_GPIO_57
};


void gpio_get_state(hal_gpio_pin_t gpio_pin, gpio_state_t *gpio_state)
{

    uint32_t mode;
    uint32_t dir;
    uint32_t din;
    uint32_t dout;
    uint32_t pu;
    uint32_t pd;
    uint32_t pupd;
    uint32_t r0;
    uint32_t r1;
    uint32_t rsel;

    gpio_pull_type_t pull_type;
    uint32_t temp;
    uint32_t shift;
    uint32_t reg_index;
    uint32_t bit_index;
    hal_gpio_driving_current_t driving_value;

    //const char *direct[2] = {"input", "output"};
    //const char *pull_state[10] = {"disable_pull", "PU_R", "PD_R", "PU_R0","PD_R0", "PU_R0_R1", "PD_R0_R1", "PUPD_Error","PU_R1","PD_R1"};


    reg_index = gpio_pin / 8;
    bit_index = (gpio_pin % 8) * 4;
    mode = (gpio_base->GPIO_MODE.RW[reg_index] >> (bit_index) & 0xf);

    reg_index = gpio_pin / 32;
    bit_index = gpio_pin % 32;
    dir  = (gpio_base->GPIO_DIR.RW[reg_index] >> (bit_index) & 0x1);
    din  = (gpio_base->GPIO_DIN.R[reg_index] >> (bit_index) & 0x1);
    dout = (gpio_base->GPIO_DOUT.RW[reg_index] >> (bit_index) & 0x1);

    pu = 0xf;
    pd = 0xf;
    pupd = 0xf;
    r0   = 0xf;
    r1   = 0xf;

    shift = 0xff;
    pull_type = GPIO_PUPD_ERR;

    if (gpio_cfg_table[gpio_pin].pupd_shift != 0xff) {
        shift = gpio_cfg_table[gpio_pin].pupd_shift;
        pu = (GPIO_REG32(gpio_cfg_table[gpio_pin].pu_reg) >> shift) & 0x01;
        pd = (GPIO_REG32(gpio_cfg_table[gpio_pin].pd_reg) >> shift) & 0x01;

        temp = (pu << 4) + pd;
        if (gpio_cfg_table[gpio_pin].rsel_shift != 0xff) {
            shift = gpio_cfg_table[gpio_pin].rsel_shift;
            rsel = (GPIO_REG32(gpio_cfg_table[gpio_pin].rsel_reg) >> shift) & 0x3;
            temp |= ((rsel & 0x1) << 8 | ((rsel >> 1) & 0x1) << 12);
        }
        //log_hal_msgid_info("pu=%d pd=%d, temp=%.3x\r\n", 3, pu,pd,temp);

        if (temp == 0x00) {
            pull_type = GPIO_NO_PULL;
        } else if (temp == 0x10) {
            pull_type = GPIO_PU_R;
        } else if (temp == 0x01) {
            pull_type = GPIO_PD_R;
        } else if (temp == 0x101) {
            pull_type = GPIO_PD_R0;
        } else if (temp == 0x1001) {
            pull_type = GPIO_PD_R1;
        } else if (temp == 0x1101) {
            pull_type = GPIO_PD_R0_R1;
        } else if (temp == 0x110) {
            pull_type = GPIO_PU_R0;
        } else if (temp == 0x1010) {
            pull_type = GPIO_PU_R1;
        } else if (temp == 0x1110) {
            pull_type = GPIO_PU_R0_R1;
        } else if ((temp & 0xFF) == 0x11) {
            pull_type = GPIO_PUPD_ERR;
            log_hal_msgid_info("error pu = %x, pd= %x\r\n", 2, pu, pd);
        }
    } else if (gpio_cfg_table[gpio_pin].pupd_r0_r1_shift != 0xff) {
        shift = gpio_cfg_table[gpio_pin].pupd_r0_r1_shift;
        pupd = (GPIO_REG32(gpio_cfg_table[gpio_pin].pupd_reg) >> shift) & 0x01;
        r0 = (GPIO_REG32(gpio_cfg_table[gpio_pin].r0_reg) >> shift) & 0x01;
        r1 = (GPIO_REG32(gpio_cfg_table[gpio_pin].r1_reg) >> shift) & 0x01;

        temp = (pupd << 8) + (r0 << 4) + r1;
        //log_hal_msgid_info("pupd=%d r0=%d, r1=%d, temp=%.3x\r\n", 4, pupd,r0,r1,temp);

        if (temp == 0x010) {
            pull_type = GPIO_PU_R0;
        } else if (temp == 0x001) {
            pull_type = GPIO_PU_R1;
        } else if (temp == 0x110) {
            pull_type = GPIO_PD_R0;
        } else if (temp == 0x101) {
            pull_type = GPIO_PD_R1;
        } else if (temp == 0x011) {
            pull_type = GPIO_PU_R0_R1;
        } else if (temp == 0x111) {
            pull_type = GPIO_PD_R0_R1;
        } else if ((temp == 0x100) || (temp == 0x000)) {
            pull_type = GPIO_NO_PULL;
        } else {
            pull_type = GPIO_PUPD_ERR;
            log_hal_msgid_info("error pupd-r0-r1 = %x\r\n", 1, temp);
        }
    }


    hal_gpio_get_driving_current((hal_gpio_pin_t)gpio_pin, &driving_value);

    gpio_state->mode = mode;
    gpio_state->dir  = dir;
    gpio_state->din  = din;
    gpio_state->dout = dout;
    gpio_state->pull_type = pull_type;
    gpio_state->current_type = (uint8_t)driving_value;

    //log_hal_msgid_info("GPIO%d, mode=%d, %s, din=%d, dout=%d, %s\r\n",6, gpio_pin, mode, direct[dir], din,dout,pull_state[pull_type]);
}

#endif

