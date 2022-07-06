# Copyright Statement:
#
# (C) 2021  Airoha Technology Corp. All rights reserved.
#
# This software/firmware and related documentation ("Airoha Software") are
# protected under relevant copyright laws. The information contained herein
# is confidential and proprietary to Airoha Technology Corp. ("Airoha") and/or its licensors.
# Without the prior written permission of Airoha and/or its licensors,
# any reproduction, modification, use or disclosure of Airoha Software,
# and information contained herein, in whole or in part, shall be strictly prohibited.
# You may only use, reproduce, modify, or distribute (as applicable) Airoha Software
# if you have agreed to and been bound by the applicable license agreement with
# Airoha ("License Agreement") and been granted explicit permission to do so within
# the License Agreement ("Permitted User").  If you are not a Permitted User,
# please cease any access or use of Airoha Software immediately.
# BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
# THAT AIROHA SOFTWARE RECEIVED FROM AIROHA AND/OR ITS REPRESENTATIVES
# ARE PROVIDED TO RECEIVER ON AN "AS-IS" BASIS ONLY. AIROHA EXPRESSLY DISCLAIMS ANY AND ALL
# WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
# NEITHER DOES AIROHA PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
# SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
# SUPPLIED WITH AIROHA SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
# THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
# THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
# CONTAINED IN AIROHA SOFTWARE. AIROHA SHALL ALSO NOT BE RESPONSIBLE FOR ANY AIROHA
# SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
# STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND AIROHA'S ENTIRE AND
# CUMULATIVE LIABILITY WITH RESPECT TO AIROHA SOFTWARE RELEASED HEREUNDER WILL BE,
# AT AIROHA'S OPTION, TO REVISE OR REPLACE AIROHA SOFTWARE AT ISSUE,
# OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
# AIROHA FOR SUCH AIROHA SOFTWARE AT ISSUE.
#

IC_CONFIG                             = ab158x
BOARD_CONFIG                          = ab158x_evb

##############################################
#           Custom feature option            #
##############################################

## part1: configure y/n

###### Audio/Voice effects ######

# This option is to enable/disable Audio PLC.
# It must be turned on/off for both DSP and MCU, otherwise, it will not work.
AIR_AUDIO_PLC_ENABLE = n

# This option is to enable/disable Personal Sound Amplification Products (advanced passthrough) main function.
# It must be turned on/off for both DSP and MCU, otherwise, it will not work.
AIR_ADVANCED_PASSTHROUGH_ENABLE = n

# This option is to enable/disable Active Noise Cancellation (ANC) main function.
# It must be turned on/off for both DSP and MCU, otherwise, it will not work.
AIR_ANC_ENABLE = y

# This option is used to enable/disable full adaptive ANC.
# It must be turned on/off for both DSP and MCU, otherwise, it will not work. No support for headset project..
# Dependency: AIR_ANC_ENABLE must be enabled when this option is set to y.
AIR_FULL_ADAPTIVE_ANC_ENABLE = n

# This option is to enable PEQ feature.
# It must be turned on/off for both DSP and MCU, otherwise, it will not work.
# Dependency: AIR_BT_PEQ_USE_PIC_ENABLE must be enabled when this option is set to y.
AIR_PEQ_ENABLE = y

# This option is to enable adaptive eq feature.
# It must be turned on/off for both DSP and MCU, otherwise, it will not work.
# Dependency: AIR_PEQ_ENABLE must be enabled when this option is set to y.
AIR_ADAPTIVE_EQ_ENABLE = n

# This option is to enable Dynamic range compression feature.
AIR_DRC_ENABLE = y

# This option is to enable/disable fit detection.
# It must be turned on/off for both DSP and MCU, otherwise, it will not work.
# Dependency: AIR_ANC_ENABLE must be enabled when this option is set to y.
AIR_ANC_FIT_DETECTION_ENABLE = n

# This option is used to enable/disable wind noise detection adaptive ANC.
# It must be turned on/off for both DSP and MCU, otherwise, it will not work.
# Dependency: AIR_ANC_ENABLE must be enabled when this option is set to y.
AIR_ANC_WIND_NOISE_DETECTION_ENABLE = n

# This option is used to enable/disable Environment Detection adaptive ANC.
# It must be turned on/off for both DSP and MCU, otherwise, it will not work.
# Dependency: AIR_ANC_ENABLE and AIR_AUDIO_TRANSMITTER_ENABLE must be enabled when this option is set to y.
AIR_ANC_ENVIRONMENT_DETECTION_ENABLE = n

# This is the option of audio transmitter. Users who use audio transmitter need to open this option.
# It must be turned on/off for both DSP and MCU, otherwise, it will not work.
AIR_AUDIO_TRANSMITTER_ENABLE = y

# This option is used to support HWSRC for DL1.
AIR_HWSRC_ON_MAIN_STREAM_ENABLE = y

# This option is for speaker project hwsrc clk skew. It should not be enabled by default.
AIR_HWSRC_IN_STREAM_ENABLE = n

# This option is used to enable/disable HWSRC CLK SKEW.
# It must be turned on/off for both DSP and MCU, otherwise, it will not work.
# Dependency: AIR_HWSRC_ON_MAIN_STREAM_ENABLE must be enabled when this option is set to y.
AIR_HWSRC_CLKSKEW_ENABLE = y

# This option is used to enable/disable reset SDM while silence. It should be enabled by default.
AIR_RESET_SDM_ENABLE = n

###### Audio peripheral ######

# This option is used to enable line in mix with A2DP/HFP.
# It must be turned on/off for both DSP and MCU, otherwise, it will not work. No support for earbuds project..
AIR_LINE_IN_MIX_ENABLE = n

# This option is used to enable line in.
# It must be turned on/off for both DSP and MCU, otherwise, it will not work. No support for earbuds project.
AIR_LINE_IN_ENABLE = y

# This option is to enable/disable LINE IN PEQ feature.
# It must be turned on/off for both DSP and MCU, otherwise, it will not work. No support for earbuds project.
# Dependency: AIR_DRC_ENABLE and AIR_LINE_IN_ENABLE must be enabled when this option is set to y.
AIR_LINE_IN_PEQ_ENABLE = y

# This option is to enable/disable LINE IN INS feature.
# It must be turned on/off for both DSP and MCU, otherwise, it will not work. No support for earbuds project.
AIR_LINE_IN_INS_ENABLE = y

# This option is used to enable line out.
# It must be turned on/off for both DSP and MCU, otherwise, it will not work. No support for earbuds project.
AIR_LINE_OUT_ENABLE = n

# This option is used to enable wired USB audio playback.
# It must be turned on/off for both DSP and MCU, otherwise, it will not work. No support for earbuds project.
AIR_USB_AUDIO_IN_ENABLE = y

# This option is used to enable wired USB audio out.
# It must be turned on/off for both DSP and MCU, otherwise, it will not work. No support for earbuds project.
AIR_USB_AUDIO_OUT_ENABLE = n

# This option is used to enable wired USB audio in mix with A2DP/HFP.
# It must be turned on/off for both DSP and MCU, otherwise, it will not work. No support for earbuds project.
AIR_USB_AUDIO_IN_MIX_ENABLE = n

###### Scenario ######

# This option is used to enable/disable HFP scenario.
# It must be turned on/off for both DSP and MCU, otherwise, it will not work.
AIR_BT_HFP_ENABLE = y

# This option is used to enable/disable record mic input scenario.
AIR_MIC_RECORD_ENABLE = y

######  Codec ######

# This option is used to enable vendor codec.
# It must be turned on/off for both DSP and MCU, otherwise, it will not work.
AIR_BT_A2DP_VENDOR_ENABLE = n

# This option is used to enable AAC codec.
# It must be turned on/off for both DSP and MCU, otherwise, it will not work.
AIR_BT_A2DP_AAC_ENABLE = y

# This option is used to enable SBC codec.
# It must be turned on/off for both DSP and MCU, otherwise, it will not work.
AIR_BT_A2DP_SBC_ENABLE = y

######  Voice assistant/VAD ######

# This option is used to enable AMA HOT word feature.
# It must be turned on/off for both DSP and MCU, otherwise, it will not work.
AIR_AMA_HOTWORD_ENABLE = n

# This option is used to enable AMA hotword PIC version.
# Dependency: AIR_AMA_HOTWORD_ENABLE must be enabled when this option is set to y.
AIR_AMA_HOTWORD_USE_PIC_ENABLE = n

######  BT/MCSync ######

# This option is to enable/disable LE Audio.
# It must be turned on/off for both DSP and MCU, otherwise, it will not work.
AIR_LE_AUDIO_ENABLE = y

# This option is to enable LE Audio dongle scenario.
# It must be turned on/off for both DSP and MCU, otherwise, it will not work.
AIR_LE_AUDIO_DONGLE_ENABLE = n

# This option is to enable ULL dongle scenario.
# It must be turned on/off for both DSP and MCU side AIR_BT_ULTRA_LOW_LATENCY_ENABLE option, otherwise, it will not work.
AIR_ULL_GAMING_DONGLE_ENABLE = n

# This option is to enable ULL headset scenario.
# It must be turned on/off for both DSP and MCU side AIR_BT_ULTRA_LOW_LATENCY_ENABLE option, otherwise, it will not work.
AIR_ULL_GAMING_HEADSET_ENABLE = n

######  System ######

# This option is to enable audio dump for CM4 side debug use.
# It must be turned on/off for both DSP and MCU, otherwise, it will not work.
AIR_AUDIO_DUMP_ENABLE = y

# This option is to support air dump for ecnr.
# It must be turned on/off for both DSP and MCU, otherwise, it will not work.
AIR_AIRDUMP_ENABLE = n

# This option is used to enable/disable low lantency mux.
AIR_LOW_LANTENCY_MUX_ENABLE = n

## part2: need to set specified value


######  System ######

# Using specific linker script during linking process.
AIR_DSP_LINKER_SCRIPT_FILE = ab158x_dsp0_flash_8m.lcf

# This option is to configure system log debug level.
# debug level: none, error, warning, info and debug
#              empty   : All debug logs are compiled.
#              error   : Only error logs are compiled.
#              warning : Only warning and error logs are compiled.
#              info    : Only info, warning, and error logs are compiled.
#              debug   : All debug logs are compiled.
#              none    : All debugs are disabled.
AIR_DEBUG_LEVEL = info

######  Audio ######

# This option is to choose the uplink rate. Default setting is none.
# It must be set to the same value for both DSP and MCU, otherwise, it will not work.
# Up Link Rate : none, 48k
#                none : uplink rate will be handled by scenario itself.
#                48k  : uplink rate will be fixed in 48k Hz.
AIR_UPLINK_RATE   = none

# This option is to fix the uplink DMA resolution. Default setting is none.
# It must be set to the same value for both DSP and MCU, otherwise, it will not work.
# Up Link Resolution : none, 32bit
#                      none : uplink resolution will be handled by scenario itself.
#                      32bit: uplink resolution will be fixed at 32-bit.
AIR_UPLINK_RESOLUTION   = none

# This option is to choose the downlink rate. Default setting is none.
# It must be set to the same value for both DSP and MCU, otherwise, it will not work.
# Down Link Rate : none, 48k, 96k
#                  none  : downlink rate will be handled by scenario itself.
#                  48k   : downlink rate will be fixed in 48k Hz.
#                  96k   : downlink rate will be fixed in 96k Hz.
AIR_DOWNLINK_RATE = 48k

# This option is to choose the type of ECNR. Default setting is none.
# It must be set to the same value for both DSP and MCU, otherwise, it will not work.
#                  none             : disable ECNR
#                  ECNR_1_OR_2_MIC  : Inhouse ECNR to support 1 or 2 MIC.
#                  ECNR_1MIC_INEAR  : Inhouse ECNR to support 1 + 1 MIC.
#                  ECNR_2MIC_INEAR  : Inhouse ECNR to support 1 + 2 MIC.
#                  3RD_PARTY_AI_NR  : 3rd party AINR to support 1/2 MIC.
#                  3RD_PARTY_AI_NR_INEAR : 3rd party AINR to support 1/2 + 1 MIC.
#                  3RD_PARTY_CUSTOMIZED : Customized NR.
AIR_ECNR_CONFIG_TYPE = 3RD_PARTY_AI_NR_INEAR

######  Audio peripheral ######

# This option is to choose the type of dual chip mixing mode.
# Dual chip mixing mode : master, slave, none
#                         master : The side which have HW key and control the partner. It supports ULL connection.
#                         slave  : The side which is controlled. It doesn't support ULL connection.
#                         none   : Not a master or salve.
# It must be set to the same value for both DSP and MCU, otherwise, it will not work.
AIR_DUAL_CHIP_MIXING_MODE = none

# This option is used to  concurrently use AFE source.
AIR_MULTI_MIC_STREAM_ENABLE = y

############################################
#               Internal use               #
############################################