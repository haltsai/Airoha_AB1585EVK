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
IC_TYPE                               = ab1585
BOARD_TYPE                            = ab1585_evk_dchs
BL_FEATURE                            = feature_85_evb.mk


##############################################
#           Custom feature option            #
##############################################

## part1: configure y/n

###### Audio/Voice effects ######

# This option is to enable/disable Audio PLC.
# It must be turned on/off for both DSP and MCU, otherwise, it will not work.
AIR_AUDIO_PLC_ENABLE = n

# This option is to enable/disable Personal Sound Amplification Products(advanced passthrough) main function
# It must be turned on/off for both DSP and MCU, otherwise, it will not work.
AIR_ADVANCED_PASSTHROUGH_ENABLE = n

# This option is to enable/disable Active Noise Cancellation (ANC) main funtion.
# It must be turned on/off for both DSP and MCU, otherwise, it will not work.
AIR_ANC_ENABLE = y

# This option is to enable PEQ feature.
# It must be turned on/off for both DSP and MCU, otherwise, it will not work.
AIR_PEQ_ENABLE = y

# This option is used to enable line in.
# It must be turned on/off for both DSP and MCU, otherwise, it will not work. No support for earbuds project.
AIR_LINE_IN_ENABLE = y

# This option is to enable LINEIN PEQ feature.
# It must be turned on/off for both DSP and MCU, otherwise, it will not work.
AIR_LINE_IN_PEQ_ENABLE = y

# This option is to enable/disable LINE IN INS feature.
# It must be turned on/off for both DSP and MCU, otherwise, it will not work. No support for earbuds project.
AIR_LINE_IN_INS_ENABLE = y

# This option is used to enable usb audio in.
# It must be turned on/off for both DSP and MCU, otherwise, it will not work. No support for earbuds project.
AIR_USB_AUDIO_IN_ENABLE = n

# This option is used to enable usb audio in(wired audio).
# It must be turned on/off for both DSP and MCU, otherwise, it will not work. No support for earbuds project.
AIR_USB_AUDIO_IN_MIX_ENABLE = n

# This option is used to enable usb audio out(wired audio).
# It must be turned on/off for both DSP and MCU, otherwise, it will not work. No support for earbuds project.
AIR_USB_AUDIO_OUT_ENABLE = n

# This option is to enable/disable fit detection
# It must be turned on/off for both DSP and MCU, otherwise, it will not work.
# Dependency: AIR_ANC_ENABLE must be enabled when this option is set to y.
AIR_ANC_FIT_DETECTION_ENABLE = n

# This option is used to enable/disable User triggered adaptive ANC.
# It must be turned on/off for both DSP and MCU, otherwise, it will not work.
# Dependency: AIR_ANC_ENABLE and AIR_AUDIO_TRANSMITTER_ENABLE must be enabled when this option is set to y.
AIR_ANC_USER_TRIGGER_ENABLE = n

# This option is used to enable/disable wind noise detection adaptive ANC.
# It must be turned on/off for both DSP and MCU, otherwise, it will not work.
# Dependency: AIR_ANC_ENABLE must be enabled when this option is set to y.
AIR_ANC_WIND_NOISE_DETECTION_ENABLE = n

# This option is used to enable/disable Environment Detection adaptive ANC.
# It must be turned on/off for both DSP and MCU, otherwise, it will not work.
# Dependency: AIR_ANC_ENABLE must be enabled when this option is set to y.
AIR_ANC_ENVIRONMENT_DETECTION_ENABLE = n

# This is the option of audio transmitter. Users who use audio transmitter need to open this option.
# It must be turned on/off for both DSP and MCU, otherwise, it will not work.
AIR_AUDIO_TRANSMITTER_ENABLE = y

# This option is uesd for enhanced gain setting.
AIR_AUDIO_GAIN_SETTING_ENHANCE_ENABLE = y

# This option is used to enable/disable the AT CMD trigging voice prompt.
AIR_AUDIO_AT_CMD_PROMPT_SOUND_ENABLE = n

# This option is used to enable/disable amp dc compensation.
AIR_AMP_DC_COMPENSATION_ENABLE = y

# This option is used to enable/disable SBC encoder.
AIR_SBC_ENCODER_ENABLE = n

# This option is used to support stereo MP3 voice prompt.
AIR_MP3_STEREO_SUPPORT_ENABLE = y

# This option is used to enable/disable HWSRC CLK SKEW.
# It must be turned on/off for both DSP and MCU, otherwise, it will not work.
AIR_HWSRC_CLKSKEW_ENABLE = y

# This option is used to support HWSRC for DL1.
AIR_HWSRC_ON_MAIN_STREAM_ENABLE = n

# This option is for speaker project hwsrc clk skew. It should not be enabled by default.
AIR_HWSRC_IN_STREAM_ENABLE = y

###### Scenario ######

# This option is used to enable record. It must be enabled when using voice assistant.
AIR_RECORD_ENABLE = y

# This option is used to enable/disable HFP scenario
# It must be turned on/off for both DSP and MCU, otherwise, it will not work.
AIR_BT_HFP_ENABLE = y

# This option is used to enable/disable BT sink state manager.
AIR_BT_SINK_SRV_STATE_MANAGER_ENABLE  = n

######  Codec ######

# This option is used to enable vendor codec.
# It must be turned on/off for both DSP and MCU, otherwise, it will not work.
AIR_BT_A2DP_VENDOR_ENABLE = n

# This option is to enable Codec Manager.
AIR_AUDIO_CODEC_MANAGER_ENABLE = y

# This option is used to add opus encode support.
AIR_OPUS_ENCODER_ENABLE = n

######  Voice assistant/VAD ######

# This option is used to enable AMA feature.
AIR_AMA_ENABLE = n

# This option is used to enable AMA HOT word feature.
# It must be turned on/off for both DSP and MCU, otherwise, it will not work.
# Dependency: AIR_AMA_ENABLE must be enabled when this option is set to y.
AIR_AMA_HOTWORD_ENABLE = n

# This option is used to enable AMA IAP2 .
# Dependency: AIR_AMA_ENABLE must be enabled when this option is set to y.
AIR_AMA_IAP2_APP_RELAY_ENABLE = n

# This option is used to enable AMA control sidetone.
# Dependency: AIR_AMA_ENABLE must be enabled when this option is set to y.
AIR_AMA_SIDETONE_ENABLE = n

# This option is used to enable AMA WWE during calling feature
# Dependency: AIR_AMA_ENABLE and AIR_AMA_HOTWORD_ENABLE must be enabled when this option is set to y.
AIR_AMA_HOTWORD_DURING_CALL_ENABLE = n

# This option is used to enable AMA ADV before EDR connected feature
# Dependency: AIR_AMA_ENABLE must be enabled when this option is set to y.
AIR_AMA_ADV_ENABLE_BEFORE_EDR_CONNECT_ENABLE = n

# This option is used to enable GSound.
# Dependency: AIR_RECORD_ENABLE and AIR_SBC_ENCODER_ENABLE must be enabled when this option is set to y.
AIR_GSOUND_ENABLE = n

# This option is used to enable GSound Hotword.
# It must be turned on/off for both DSP and MCU, otherwise, it will not work.
# Dependency: AIR_GSOUND_ENABLE must be enabled when this option is set to y.
# AIR_GSOUND_HOTWORD_ENABLE = n

# This option is used to enable multi va support competition.
AIR_MULTI_VA_SUPPORT_COMPETITION_ENABLE = y

######  BT/MCSync ######

# This option is used to enable/disable BT related AT command.
AIR_BT_AT_COMMAND_ENABLE = y

# This option is used to enable some speaker-specific functions
AIR_BT_SPEAKER_ENABLE = n

# This option is used to enable multi link at the same time.
AIR_MULTI_POINT_ENABLE = y

# This option is to enable/disable LE Audio.
# It must be turned on/off for both DSP and MCU, otherwise, it will not work.
AIR_LE_AUDIO_ENABLE = n

# This option is used to support LE Audio broadcast.
# Dependency: AIR_LE_AUDIO_ENABLE must be enabled when this option is set to y.
AIR_LE_AUDIO_BIS_ENABLE = n

# This option is to enable LE Audio dongle scenario.
# Dependency: AIR_LE_AUDIO_ENABLE must be enabled when this option is set to y.
# It must be turned on/off for both DSP and MCU, otherwise, it will not work.
AIR_LE_AUDIO_DONGLE_ENABLE = n

# This option is used to enable the feature that iap2 data is transmitted through MUX
AIR_IAP2_VIA_MUX_ENABLE = y

# This option is used to enable teams.
# Dependency: AIR_USB_AUDIO_HID_ENABLE must be enabled when this option is set to y.
AIR_MS_TEAMS_ENABLE = n

# This option is used to enable ull2.0.
AIR_BLE_ULTRA_LOW_LATENCY_ENABLE = y

######  System ######

# This option is used to enable fast pair.
AIR_BT_FAST_PAIR_ENABLE = y

# This option is used to enable fast pair SASS.
# Dependency: AIR_BT_FAST_PAIR_ENABLE must be enabled when this option is set to y.
AIR_BT_FAST_PAIR_SASS_ENABLE = n

# This option is used to enable fast pair spot.
# Dependency: AIR_BT_FAST_PAIR_ENABLE must be enabled when this option is set to y.
AIR_SPOT_ENABLE = y

# This option is used to enable FOTA basic function.
AIR_FOTA_ENABLE = y

# This option is to enable FOTA via race cmd.
# Dependency: AIR_FOTA_ENABLE, AIR_RACE_CMD_ENABLE and AIR_RACE_DUAL_CMD_ENABLE (if it is an earbuds project) must be enabled when this option is set to y.
AIR_FOTA_VIA_RACE_CMD_ENABLE = y

# This option is used to enable wearing detection.
AIR_WEARING_DETECTION_ENABLE = n

# This option is used to enable component firmware update module.
# Dependency: AIR_USB_AUDIO_HID_ENABLE, AIR_LE_AUDIO_ENABLE, AIR_RACE_CMD_ENABLE, AIR_FOTA_ENABLE must enable, and AIR_USB_AUDIO_VERSION cannot be none.
AIR_CFU_ENABLE = n

# This option is to enable USB module.
AIR_USB_ENABLE = y

# This option is to use USB TX as usage of microphone.
# Dependency: AIR_USB_ENABLE must be enabled, and AIR_USB_AUDIO_VERSION cannot be none when this option is set to y.
AIR_USB_AUDIO_MICROPHONE_ENABLE = n

# This option is to add more one USB RX to enable second speaker.
# Dependency: AIR_USB_ENABLE must be enabled, and AIR_USB_AUDIO_VERSION cannot be none when this option is set to y.
AIR_USB_AUDIO_2ND_SPEAKER_ENABLE = n

# This option is used to support HID (Human Interface Device).
# Dependency: AIR_USB_ENABLE must be enabled when this option is set to y.
AIR_USB_AUDIO_HID_ENABLE = n

# This option is to enable MS XBOX GIP feature.
# Dependency: AIR_BT_ULTRA_LOW_LATENCY_ENABLE must be enabled when this option is set to y.
AIR_MS_GIP_ENABLE = n

# This option is to enable Device Mass Storage feature.
# Dependency: AIR_USB_ENABLE must be enabled when this option is set to y.
AIR_USB_MSC_DEV_ENABLE = n

# This option is used to support RACE CMD trigger Find me.
# Dependency: AIR_RACE_CMD_ENABLE must be enabled when this option is set to y.
AIR_RACE_FIND_ME_ENABLE = y

# This option is used to enable race cmd.
AIR_RACE_CMD_ENABLE = y

# This option is used to enable 3rd party BLE advertising
AIR_TILE_ENABLE = n

# This option is to enable minidump feature.
AIR_MINIDUMP_ENABLE = n

# This option is to enable audio dump for CM4 side debug use.
# It must be turned on/off for both DSP and MCU, otherwise, it will not work.
AIR_AUDIO_DUMP_ENABLE = y

# This option is to support air dump for ecnr.
# It must be turned on/off for both DSP and MCU, otherwise, it will not work.
AIR_AIRDUMP_ENABLE = n

# This option is to enable and disable CPU utilization function.
AIR_OS_CPU_UTILIZATION_ENABLE = y

# This option is used to enable/disable the power saving in APP.
AIR_APPS_POWER_SAVE_ENABLE = y

# This option is used to enable/disable key driver.
AIR_AIRO_KEY_ENABLE = y

# This option is used to enable/disable eint key module.
AIR_EINT_KEY_ENABLE = y

# This option is used to enable/disable secure boot module.
# NOTICE:
#   - This option can only be enabled if the secure-boot-addon-repo exists.
#   - secure-boot-addon-repo path: mcu\prebuilt\middleware\airoha\secure_boot
#   - If this option enabled without secure boot addon repo, the code will build fail.
AIR_SECURITY_SECURE_BOOT_ENABLE = n

# This option is to enable/disable Microsoft Swift Pair.
AIR_SWIFT_PAIR_ENABLE = n

# This option is used to enable/disable low lantency mux.
AIR_LOW_LANTENCY_MUX_ENABLE = y

## part2: need to set specified value

###### Voice assistant/VAD ######

# This option is to configure key trigger mode of AMA.
# mode:
#     NONE : The button trigger mode is disabled
#     TTT : Tap-To-Talk trigger mode.
#     PTT : Push-To-Talk trigger mode.
# Dependency: AIR_AMA_ENABLE must be enabled when this option is not set to NONE.
AIR_AMA_BUTTON_TRIGGER_MODE = TTT

######  System ######

# Using specific linker script during linking process.
AIR_MCU_LINKER_SCRIPT_FILE = ab158x_flash_8m.ld


# This option is to configure mbedTLS features.
AIR_MBEDTLS_CONFIG_FILE = config-vendor-fota-race-cmd.h

# This option is to configure system log debug level.
# debug level: none, error, warning, info and debug
#              empty   : All debug logs are compiled.
#              error   : Only error logs are compiled.
#              warning : Only warning and error logs are compiled.
#              info    : Only info, warning, and error logs are compiled.
#              debug   : All debug logs are compiled.
#              none    : All debugs are disabled.
AIR_DEBUG_LEVEL = info

# Choose the version of UAC (USB Audio Class). Default setting is version 1.
# Dependency: AIR_USB_ENABLE must be enabled when this option is set to v1.
# USB Audio Version : v1, none
#                     none  : means disable USB Audio.
AIR_USB_AUDIO_VERSION = v1

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
AIR_DOWNLINK_RATE = 96k

# This option is to choose the type of ECNR. Default setting is none.
# It must be set to the same value for both DSP and MCU, otherwise, it will not work.
#                  none             : disable ECNR
#                  ECNR_1_OR_2_MIC  : Inhouse ECNR to support 1 or 2 MIC.
#                  ECNR_1MIC_INEAR  : Inhouse ECNR to support 1 + 1 MIC.
#                  ECNR_2MIC_INEAR  : Inhouse ECNR to support 1 + 2 MIC.
#                  3RD_PARTY_AI_NR  : 3rd party AINR to support 1/2 MIC.
#                  3RD_PARTY_AI_NR_OFFLOAD_PREV  : 3rd party AINR to support 1 MIC with offload.
#                  3RD_PARTY_AI_NR_OFFLOAD_POST  : 3rd party AINR to support 1 MIC with offload(dongle).
#                  3RD_PARTY_AI_NR_INEAR : 3rd party AINR to support 1/2 + 1 MIC.
#                  3RD_PARTY_CUSTOMIZED : Customized NR.
AIR_ECNR_CONFIG_TYPE = 3RD_PARTY_AI_NR_INEAR

######  Audio peripheral ######

# This option is to configure voice prompt decoder codec type.
# codec type: none, mp3, wav, opus (specific), all
#             all  : Support all decoder codec types, mp3/pcm/wav/opus.
#             mp3  : Support MP3 voice prompt files, default selection.
#             wav  : Support WAV voice prompt files, a-law/u-law/ms-adpcm/ima-adpcm/pure pcm.
#             opus : Support OPUS voice prompt files, 1 channel, 48 KHz, 20 ms frame size and CELT elementary stream.
#             none : Unused codec. It can still be used to play pure pcm.
AIR_PROMPT_CODEC_TYPE = mp3


# This option is to define product category.
AIR_PRODUCT_CATEGORY = AB1585_Headset

# This option is to choose the type of dual chip mixing mode.
# Dual chip mixing mode : master, slave, none
#                         master : The side which have HW key and control the partner. It supports ULL connection.
#                         slave  : The side which is controlled. It doesn't support ULL connection.
#                         none   : Not a master or salve.
# It must be set to the same value for both DSP and MCU, otherwise, it will not work.
AIR_DUAL_CHIP_MIXING_MODE = slave

