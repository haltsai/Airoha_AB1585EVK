/*
 * Generated by Airoha Easy PinMux Tool Version 2.7.5 for AB1588. Copyright Airoha Inc. (C) 2015.
 * 2021-11-29 15:36:14:0542
 * Do Not Modify the File.
 */

/*****************************************************************************
*
* Filename:
* ---------
*    ***.*
*
* Project:
* --------
*
* Description:
* ------------
*
* Author:
* -------
*
*============================================================================
****************************************************************************/

#ifndef _EPT_GPIO_DRV_H
#define _EPT_GPIO_DRV_H

#define MODE_0  0
#define MODE_1  1
#define MODE_2  2
#define MODE_3  3
#define MODE_4  4
#define MODE_5  5
#define MODE_6  6
#define MODE_7  7
#define MODE_8  8
#define MODE_9  9
#define MODE_NC  0

#define PULL_ENABLE  1
#define PULL_DISABLE  0
#define DIR_INPUT  0
#define DIR_OUTPUT  1
#define GPIO_PORTNULL_MODE  0
#define GPIO_PORTNULL_DIR   0
#define GPIO_PORTNULL_OUTPUT_LEVEL  0
#define GPIO_PORTNULL_PU        0
#define GPIO_PORTNULL_PD        0
#define GPIO_PORTNULL_R0    0
#define GPIO_PORTNULL_R1    0
#define GPIO_PORTNULL_PUPD  0
#define GPIO_PORTNULL_DOUT  0
#define GPIO_PORTNULL_RSEL  0
#define GPIO_PORT0_MODE   MODE_1 // 1:AP_JTMS : Used for AP_JTMS
#define GPIO_PORT1_MODE   MODE_1 // 1:AP_JTCK : Used for AP_JTCK
#define GPIO_PORT2_MODE   MODE_1 // 1:DSP_JTMS : Used for DSP_JTMS
#define GPIO_PORT3_MODE   MODE_1 // 1:DSP_JTCK : Used for DSP_JTCK
#define GPIO_PORT4_MODE   MODE_1 // 1:DSP_JRST : Used for DSP_JRST
#define GPIO_PORT5_MODE   MODE_1 // 1:DSP_JTDI : Used for DSP_JTDI
#define GPIO_PORT6_MODE   MODE_1 // 1:DSP_JTDO : Used for DSP_JTDO
#define GPIO_PORT7_MODE   MODE_1 // 1:UART2_TXD : Used for UART2_TXD
#define GPIO_PORT8_MODE   MODE_1 // 1:UART2_RXD : Used for UART2_RXD
#define GPIO_PORT9_MODE   MODE_1 // 1:UART1_TXD : Used for UART1_TXD
#define GPIO_PORT10_MODE   MODE_1 // 1:UART1_RXD : Used for UART1_RXD
#define GPIO_PORT11_MODE   MODE_1 // 1:UART0_TXD : Used for UART0_TXD
#define GPIO_PORT12_MODE   MODE_1 // 1:UART0_RXD : Used for UART0_RXD
#define GPIO_PORT13_MODE   MODE_9 // 9:EINT13 : Used for EINT13
#define GPIO_PORT14_MODE   MODE_9 // 9:EINT14 : Used for EINT14
#define GPIO_PORT15_MODE   MODE_9 // 9:EINT15 : Used for EINT15
#define GPIO_PORT16_MODE   MODE_9 // 9:EINT16 : Used for EINT16
#define GPIO_PORT17_MODE   MODE_1 // 1:I2C2_SCL : Used for I2C2_SCL
#define GPIO_PORT18_MODE   MODE_1 // 1:I2C2_SDA : Used for I2C2_SDA
#define GPIO_PORT19_MODE   MODE_6 // 6:MSDC0_CLK0 : Used for MSDC0_CLK0
#define GPIO_PORT20_MODE   MODE_6 // 6:MSDC0_CMD : Used for MSDC0_CMD
#define GPIO_PORT21_MODE   MODE_6 // 6:MSDC0_DAT0 : Used for MSDC0_DAT0
#define GPIO_PORT22_MODE   MODE_6 // 6:MSDC0_DAT1 : Used for MSDC0_DAT1
#define GPIO_PORT23_MODE   MODE_6 // 6:MSDC0_DAT2 : Used for MSDC0_DAT2
#define GPIO_PORT24_MODE   MODE_6 // 6:MSDC0_DAT3 : Used for MSDC0_DAT3
#define GPIO_PORT25_MODE   MODE_6 // 6:MSDC0_RST : Used for MSDC0_RST
#define GPIO_PORT26_MODE   MODE_9 // 9:EINT26 : Used for EINT26
#define GPIO_PORT27_MODE   MODE_4 // 4:SPI_MST1_SCK : Used for SPI_MST1_SCK
#define GPIO_PORT28_MODE   MODE_4 // 4:SPI_MST1_MOSI : Used for SPI_MST1_MOSI
#define GPIO_PORT29_MODE   MODE_4 // 4:SPI_MST1_MISO : Used for SPI_MST1_MISO
#define GPIO_PORT30_MODE   MODE_4 // 4:SPI_MST1_CS0 : Used for SPI_MST1_CS0
#define GPIO_PORT31_MODE   MODE_4 // 4:SPI_MST1_SIO2 : Used for SPI_MST1_SIO2
#define GPIO_PORT32_MODE   MODE_4 // 4:SPI_MST1_SIO3 : Used for SPI_MST1_SIO3
#define GPIO_PORT33_MODE   MODE_NC
#define GPIO_PORT34_MODE   MODE_NC
#define GPIO_PORT35_MODE   MODE_NC
#define GPIO_PORT36_MODE   MODE_NC
#define GPIO_PORT37_MODE   MODE_NC
#define GPIO_PORT38_MODE   MODE_NC
#define GPIO_PORT39_MODE   MODE_NC
#define GPIO_PORT40_MODE   MODE_NC
#define GPIO_PORT41_MODE   MODE_NC
#define GPIO_PORT42_MODE   MODE_NC
#define GPIO_PORT43_MODE   MODE_NC
#define GPIO_PORT44_MODE   MODE_NC
#define GPIO_PORT45_MODE   MODE_NC
#define GPIO_PORT46_MODE   MODE_0 // 0:GPIO46 : No Pin
#define GPIO_PORT47_MODE   MODE_0 // 0:GPIO47 : No Pin
#define GPIO_PORT48_MODE   MODE_0 // 0:GPIO48 : No Pin
#define GPIO_PORT49_MODE   MODE_0 // 0:GPIO49 : No Pin
#define GPIO_PORT50_MODE   MODE_0 // 0:GPIO50 : No Pin
#define GPIO_PORT51_MODE   MODE_0 // 0:GPIO51 : No Pin
#define GPIO_PORT52_MODE   MODE_0 // 0:GPIO52 : No Pin
#define GPIO_PORT53_MODE   MODE_0 // 0:GPIO53 : No Pin
#define GPIO_PORT54_MODE   MODE_0 // 0:GPIO54 : No Pin
#define GPIO_PORT55_MODE   MODE_0 // 0:GPIO55 : No Pin
#define GPIO_PORT56_MODE   MODE_0 // 0:GPIO56 : No Pin
#define GPIO_PORT57_MODE   MODE_0 // 0:GPIO57 : No Pin
#define RTC_GPIO_PORT0_MODE   MODE_2 // 2:CTP0 : Used for CTP0
#define RTC_GPIO_PORT1_MODE   MODE_NC
#define RTC_GPIO_PORT2_MODE   MODE_NC
#define RTC_GPIO_PORT3_MODE   MODE_NC


#define GPIO_PORT0_DIR   DIR_INPUT
#define GPIO_PORT1_DIR   DIR_INPUT
#define GPIO_PORT2_DIR   DIR_INPUT
#define GPIO_PORT3_DIR   DIR_INPUT
#define GPIO_PORT4_DIR   DIR_INPUT
#define GPIO_PORT5_DIR   DIR_INPUT
#define GPIO_PORT6_DIR   DIR_INPUT
#define GPIO_PORT7_DIR   DIR_INPUT
#define GPIO_PORT8_DIR   DIR_INPUT
#define GPIO_PORT9_DIR   DIR_INPUT
#define GPIO_PORT10_DIR   DIR_INPUT
#define GPIO_PORT11_DIR   DIR_INPUT
#define GPIO_PORT12_DIR   DIR_INPUT
#define GPIO_PORT13_DIR   DIR_INPUT
#define GPIO_PORT14_DIR   DIR_INPUT
#define GPIO_PORT15_DIR   DIR_INPUT
#define GPIO_PORT16_DIR   DIR_INPUT
#define GPIO_PORT17_DIR   DIR_INPUT
#define GPIO_PORT18_DIR   DIR_INPUT
#define GPIO_PORT19_DIR   DIR_INPUT
#define GPIO_PORT20_DIR   DIR_INPUT
#define GPIO_PORT21_DIR   DIR_INPUT
#define GPIO_PORT22_DIR   DIR_INPUT
#define GPIO_PORT23_DIR   DIR_INPUT
#define GPIO_PORT24_DIR   DIR_INPUT
#define GPIO_PORT25_DIR   DIR_INPUT
#define GPIO_PORT26_DIR   DIR_INPUT
#define GPIO_PORT27_DIR   DIR_INPUT
#define GPIO_PORT28_DIR   DIR_INPUT
#define GPIO_PORT29_DIR   DIR_INPUT
#define GPIO_PORT30_DIR   DIR_INPUT
#define GPIO_PORT31_DIR   DIR_INPUT
#define GPIO_PORT32_DIR   DIR_INPUT
#define GPIO_PORT33_DIR   DIR_INPUT
#define GPIO_PORT34_DIR   DIR_INPUT
#define GPIO_PORT35_DIR   DIR_INPUT
#define GPIO_PORT36_DIR   DIR_INPUT
#define GPIO_PORT37_DIR   DIR_INPUT
#define GPIO_PORT38_DIR   DIR_INPUT
#define GPIO_PORT39_DIR   DIR_INPUT
#define GPIO_PORT40_DIR   DIR_INPUT
#define GPIO_PORT41_DIR   DIR_INPUT
#define GPIO_PORT42_DIR   DIR_INPUT
#define GPIO_PORT43_DIR   DIR_INPUT
#define GPIO_PORT44_DIR   DIR_INPUT
#define GPIO_PORT45_DIR   DIR_INPUT
#define GPIO_PORT46_DIR   DIR_INPUT
#define GPIO_PORT47_DIR   DIR_INPUT
#define GPIO_PORT48_DIR   DIR_INPUT
#define GPIO_PORT49_DIR   DIR_INPUT
#define GPIO_PORT50_DIR   DIR_INPUT
#define GPIO_PORT51_DIR   DIR_INPUT
#define GPIO_PORT52_DIR   DIR_INPUT
#define GPIO_PORT53_DIR   DIR_INPUT
#define GPIO_PORT54_DIR   DIR_INPUT
#define GPIO_PORT55_DIR   DIR_INPUT
#define GPIO_PORT56_DIR   DIR_INPUT
#define GPIO_PORT57_DIR   DIR_INPUT
#define RTC_GPIO_PORT0_DIR   DIR_INPUT
#define RTC_GPIO_PORT1_DIR   DIR_INPUT
#define RTC_GPIO_PORT2_DIR   DIR_INPUT
#define RTC_GPIO_PORT3_DIR   DIR_INPUT


#define GPIO_PORT0_OUTPUT_LEVEL   0
#define GPIO_PORT1_OUTPUT_LEVEL   0
#define GPIO_PORT2_OUTPUT_LEVEL   0
#define GPIO_PORT3_OUTPUT_LEVEL   0
#define GPIO_PORT4_OUTPUT_LEVEL   0
#define GPIO_PORT5_OUTPUT_LEVEL   0
#define GPIO_PORT6_OUTPUT_LEVEL   0
#define GPIO_PORT7_OUTPUT_LEVEL   0
#define GPIO_PORT8_OUTPUT_LEVEL   0
#define GPIO_PORT9_OUTPUT_LEVEL   0
#define GPIO_PORT10_OUTPUT_LEVEL   0
#define GPIO_PORT11_OUTPUT_LEVEL   0
#define GPIO_PORT12_OUTPUT_LEVEL   0
#define GPIO_PORT13_OUTPUT_LEVEL   0
#define GPIO_PORT14_OUTPUT_LEVEL   0
#define GPIO_PORT15_OUTPUT_LEVEL   0
#define GPIO_PORT16_OUTPUT_LEVEL   0
#define GPIO_PORT17_OUTPUT_LEVEL   0
#define GPIO_PORT18_OUTPUT_LEVEL   0
#define GPIO_PORT19_OUTPUT_LEVEL   0
#define GPIO_PORT20_OUTPUT_LEVEL   0
#define GPIO_PORT21_OUTPUT_LEVEL   0
#define GPIO_PORT22_OUTPUT_LEVEL   0
#define GPIO_PORT23_OUTPUT_LEVEL   0
#define GPIO_PORT24_OUTPUT_LEVEL   0
#define GPIO_PORT25_OUTPUT_LEVEL   0
#define GPIO_PORT26_OUTPUT_LEVEL   0
#define GPIO_PORT27_OUTPUT_LEVEL   0
#define GPIO_PORT28_OUTPUT_LEVEL   0
#define GPIO_PORT29_OUTPUT_LEVEL   0
#define GPIO_PORT30_OUTPUT_LEVEL   0
#define GPIO_PORT31_OUTPUT_LEVEL   0
#define GPIO_PORT32_OUTPUT_LEVEL   0
#define GPIO_PORT33_OUTPUT_LEVEL   0
#define GPIO_PORT34_OUTPUT_LEVEL   0
#define GPIO_PORT35_OUTPUT_LEVEL   0
#define GPIO_PORT36_OUTPUT_LEVEL   0
#define GPIO_PORT37_OUTPUT_LEVEL   0
#define GPIO_PORT38_OUTPUT_LEVEL   0
#define GPIO_PORT39_OUTPUT_LEVEL   0
#define GPIO_PORT40_OUTPUT_LEVEL   0
#define GPIO_PORT41_OUTPUT_LEVEL   0
#define GPIO_PORT42_OUTPUT_LEVEL   0
#define GPIO_PORT43_OUTPUT_LEVEL   0
#define GPIO_PORT44_OUTPUT_LEVEL   0
#define GPIO_PORT45_OUTPUT_LEVEL   0
#define GPIO_PORT46_OUTPUT_LEVEL   0
#define GPIO_PORT47_OUTPUT_LEVEL   0
#define GPIO_PORT48_OUTPUT_LEVEL   0
#define GPIO_PORT49_OUTPUT_LEVEL   0
#define GPIO_PORT50_OUTPUT_LEVEL   0
#define GPIO_PORT51_OUTPUT_LEVEL   0
#define GPIO_PORT52_OUTPUT_LEVEL   0
#define GPIO_PORT53_OUTPUT_LEVEL   0
#define GPIO_PORT54_OUTPUT_LEVEL   0
#define GPIO_PORT55_OUTPUT_LEVEL   0
#define GPIO_PORT56_OUTPUT_LEVEL   0
#define GPIO_PORT57_OUTPUT_LEVEL   0
#define RTC_GPIO_PORT0_OUTPUT_LEVEL   0
#define RTC_GPIO_PORT1_OUTPUT_LEVEL   0
#define RTC_GPIO_PORT2_OUTPUT_LEVEL   0
#define RTC_GPIO_PORT3_OUTPUT_LEVEL   0


#define GPIO_PORT0_PU   1
#define GPIO_PORT1_PU   1
#define GPIO_PORT2_PU   1
#define GPIO_PORT3_PU   1
#define GPIO_PORT4_PU   0
#define GPIO_PORT5_PU   1
#define GPIO_PORT6_PU   0
#define GPIO_PORT7_PU   0
#define GPIO_PORT8_PU   1
#define GPIO_PORT9_PU   0
#define GPIO_PORT10_PU   1
#define GPIO_PORT11_PU   0
#define GPIO_PORT12_PU   1
#define GPIO_PORT13_PU   1
#define GPIO_PORT14_PU   1
#define GPIO_PORT15_PU   1
#define GPIO_PORT16_PU   1
#define GPIO_PORT17_PU   1
#define GPIO_PORT18_PU   1
#define GPIO_PORT19_PU   0
#define GPIO_PORT20_PU   0
#define GPIO_PORT21_PU   0
#define GPIO_PORT22_PU   0
#define GPIO_PORT23_PU   0
#define GPIO_PORT24_PU   0
#define GPIO_PORT25_PU   0
#define GPIO_PORT26_PU   1
#define GPIO_PORT27_PU   0
#define GPIO_PORT28_PU   0
#define GPIO_PORT29_PU   0
#define GPIO_PORT30_PU   0
#define GPIO_PORT31_PU   0
#define GPIO_PORT32_PU   0
#define GPIO_PORT33_PU   0
#define GPIO_PORT34_PU   0
#define GPIO_PORT35_PU   0
#define GPIO_PORT36_PU   0
#define GPIO_PORT37_PU   0
#define GPIO_PORT38_PU   0
#define GPIO_PORT39_PU   0
#define GPIO_PORT40_PU   0
#define GPIO_PORT41_PU   0
#define GPIO_PORT42_PU   0
#define GPIO_PORT43_PU   0
#define GPIO_PORT44_PU   0
#define GPIO_PORT45_PU   0
#define GPIO_PORT46_PU   0
#define GPIO_PORT47_PU   0
#define GPIO_PORT48_PU   0
#define GPIO_PORT49_PU   0
#define GPIO_PORT50_PU   0
#define GPIO_PORT51_PU   0
#define GPIO_PORT52_PU   0
#define GPIO_PORT53_PU   0
#define GPIO_PORT54_PU   0
#define GPIO_PORT55_PU   0
#define GPIO_PORT56_PU   0
#define GPIO_PORT57_PU   0
#define RTC_GPIO_PORT0_PU   0
#define RTC_GPIO_PORT1_PU   0
#define RTC_GPIO_PORT2_PU   0
#define RTC_GPIO_PORT3_PU   0


#define GPIO_PORT0_PD   0
#define GPIO_PORT1_PD   0
#define GPIO_PORT2_PD   0
#define GPIO_PORT3_PD   0
#define GPIO_PORT4_PD   1
#define GPIO_PORT5_PD   0
#define GPIO_PORT6_PD   0
#define GPIO_PORT7_PD   0
#define GPIO_PORT8_PD   0
#define GPIO_PORT9_PD   0
#define GPIO_PORT10_PD   0
#define GPIO_PORT11_PD   0
#define GPIO_PORT12_PD   0
#define GPIO_PORT13_PD   0
#define GPIO_PORT14_PD   0
#define GPIO_PORT15_PD   0
#define GPIO_PORT16_PD   0
#define GPIO_PORT17_PD   0
#define GPIO_PORT18_PD   0
#define GPIO_PORT19_PD   0
#define GPIO_PORT20_PD   0
#define GPIO_PORT21_PD   0
#define GPIO_PORT22_PD   0
#define GPIO_PORT23_PD   0
#define GPIO_PORT24_PD   0
#define GPIO_PORT25_PD   0
#define GPIO_PORT26_PD   0
#define GPIO_PORT27_PD   0
#define GPIO_PORT28_PD   0
#define GPIO_PORT29_PD   1
#define GPIO_PORT30_PD   1
#define GPIO_PORT31_PD   0
#define GPIO_PORT32_PD   0
#define GPIO_PORT33_PD   1
#define GPIO_PORT34_PD   1
#define GPIO_PORT35_PD   1
#define GPIO_PORT36_PD   1
#define GPIO_PORT37_PD   1
#define GPIO_PORT38_PD   1
#define GPIO_PORT39_PD   1
#define GPIO_PORT40_PD   1
#define GPIO_PORT41_PD   1
#define GPIO_PORT42_PD   1
#define GPIO_PORT43_PD   1
#define GPIO_PORT44_PD   1
#define GPIO_PORT45_PD   1
#define GPIO_PORT46_PD   1
#define GPIO_PORT47_PD   1
#define GPIO_PORT48_PD   1
#define GPIO_PORT49_PD   1
#define GPIO_PORT50_PD   1
#define GPIO_PORT51_PD   1
#define GPIO_PORT52_PD   1
#define GPIO_PORT53_PD   1
#define GPIO_PORT54_PD   1
#define GPIO_PORT55_PD   1
#define GPIO_PORT56_PD   1
#define GPIO_PORT57_PD   1
#define RTC_GPIO_PORT0_PD   0
#define RTC_GPIO_PORT1_PD   1
#define RTC_GPIO_PORT2_PD   1
#define RTC_GPIO_PORT3_PD   1


// PUPDR  PU-75K_ohms
#define GPIO_PORT13_RSEL  0
// PUPDR  PU-75K_ohms
#define GPIO_PORT14_RSEL  0
// PUPDR  PU-75K_ohms
#define GPIO_PORT15_RSEL  0
// PUPDR  PU-75K_ohms
#define GPIO_PORT16_RSEL  0
// PUPDR  PU-75K_ohms
#define GPIO_PORT17_RSEL  0
// PUPDR  PU-75K_ohms
#define GPIO_PORT18_RSEL  0
// PUPD1  PD-50K_ohms
#define GPIO_PORT19_PUPD  1
#define GPIO_PORT19_R1  1
#define GPIO_PORT19_R0  0
// PUPD1  PU-10K_ohms
#define GPIO_PORT20_PUPD  1
#define GPIO_PORT20_R1  0
#define GPIO_PORT20_R0  1
// PUPD1  PU-10K_ohms
#define GPIO_PORT21_PUPD  1
#define GPIO_PORT21_R1  0
#define GPIO_PORT21_R0  1
// PUPD1  PU-10K_ohms
#define GPIO_PORT22_PUPD  1
#define GPIO_PORT22_R1  0
#define GPIO_PORT22_R0  1
// PUPD1  PU-10K_ohms
#define GPIO_PORT23_PUPD  1
#define GPIO_PORT23_R1  0
#define GPIO_PORT23_R0  1
// PUPD1  PU-10K_ohms
#define GPIO_PORT24_PUPD  1
#define GPIO_PORT24_R1  0
#define GPIO_PORT24_R0  1
// PUPD1  Disable_pull
#define GPIO_PORT25_PUPD  0
#define GPIO_PORT25_R1  0
#define GPIO_PORT25_R0  0
// PUPDR  PD-75K_ohms
#define GPIO_PORT47_RSEL  2
// PUPDR  PD-75K_ohms
#define GPIO_PORT48_RSEL  2
// PUPDR  PD-75K_ohms
#define GPIO_PORT49_RSEL  2
// PUPDR  PD-75K_ohms
#define GPIO_PORT50_RSEL  2


#define EPT_GPIO_PIN_MASK_1   0x00003FFF
#define EPT_GPIO_PIN_MASK_2   0x00000000
#define EPT_GPIO_PIN_MASK_0   0xFFFFFFFF


#define GPIO_VOLTAGE_TYPE_0 0x00000000
#define GPIO_VOLTAGE_TYPE_1 0x00000000
#define GPIO_VOLTAGE_TYPE_2 0x00000000
#define GPIO_VOLTAGE_TYPE_3 0x00000000
#define GPIO_VOLTAGE_TYPE_4 0x00000000
#define GPIO_VOLTAGE_TYPE_5 0x00000000
#define GPIO_VOLTAGE_TYPE_6 0x00000000
#define GPIO_VOLTAGE_TYPE_7 0x00000000

#endif /* _EPT_GPIO_DRV_H */
