# Copyright Statement:
#
# (C) 2020  Airoha Technology Corp. All rights reserved.
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

#------------------------------------------------------------------------------
# See README.TXT for more information.
#------------------------------------------------------------------------------

################################################################################
# Project Configuration
################################################################################
PWD          :=  $(shell pwd)
ROOTDIR      :=  ../../../../..
PROJ_PATH    :=  $(patsubst $(abspath $(PWD)/$(ROOTDIR))/%,%,$(abspath $(dir $(PWD))))
FEATURE      ?=  feature.mk
PROJ_NAME    :=  $(notdir $(shell cd ../ ; pwd))
BOARD        :=  $(firstword $(subst /, ,$(patsubst $(abspath $(ROOTDIR))/project/%,%,$(PWD))))
FEATURE_BASENAME      :=  $(basename $(FEATURE))
DEFAULT_BIN_PATH      :=  $(abspath $(strip $(ROOTDIR))/prebuilt/dsp0_bin/$(strip $(BOARD))/$(strip $(PROJ_NAME))/$(strip $(FEATURE_BASENAME)))

OUTDIR       :=  $(abspath $(strip $(ROOTDIR))/out/$(strip $(BOARD))/$(strip $(PROJ_NAME)))
LIBDIR       :=  $(strip $(ROOTDIR))/prebuilt/dsp0/standard/ab156x
LIBDIR2      :=  $(strip $(ROOTDIR))/prebuilt/middleware/airoha/dspalg
LIBDIR3      :=  $(strip $(ROOTDIR))/prebuilt/middleware/third_party/dspalg
BUILD_LIBDIR :=  $(strip $(OUTDIR))/lib
LOGDIR       :=  $(strip $(OUTDIR))/log
ERR_LOG      :=  $(strip $(LOGDIR))/err.log
BUILD_LOG    :=  $(strip $(LOGDIR))/build.log

IMAGE        :=  $(strip $(OUTDIR))/$(strip $(PROJ_NAME)).out
BINARY       :=  $(strip $(OUTDIR))/$(strip $(PROJ_NAME)).bin
HEX          :=  $(strip $(OUTDIR))/$(strip $(PROJ_NAME)).hex
DEASM        :=  $(strip $(OUTDIR))/$(strip $(PROJ_NAME)).asm
MAP          :=  $(strip $(OUTDIR))/$(strip $(PROJ_NAME)).map

OBJ_ADD_ON :=


################################################################################
# Include Compiler and Commands Configuration
################################################################################
LDFLAG      +=  -wrap=malloc
LDFLAG      +=  -wrap=free
LDFLAG      +=  -wrap=calloc
LDFLAG      +=  -wrap=realloc
LDFLAG      +=  -wrap=_Assert
LDFLAG      +=  -wrap=_exit

include $(FEATURE)
# Not changed feature option for this project.
FPGA_ENV                              := n
CCCI_ENABLE                           := y
PRELOADER_ENABLE                      := y
# DSP Memory Management enable
DSP_MEMORY_MANAGEMENT_EBANLE          ?= n
# heap dump
MTK_SUPPORT_HEAP_DEBUG                := y
# heap peak profiling
## N/A
MTK_HEAP_SIZE_GUARD_ENABLE            := n
# SWLA
MTK_SWLA_ENABLE                       := y
# 2a2d, AFE driver
MTK_AUDIO_SUPPORT_MULTIPLE_MICROPHONE := y
LINE_IN_PURE_FOR_AMIC_CLASS_G_HQA     := y
# HFP
MTK_BT_HFP_FORWARDER_ENABLE           := y
AIR_BT_HFP_CVSD_ENABLE                := y
AIR_BT_A2DP_CVSD_USE_PIC_ENABLE       := y
# DRC V2 module
MTK_BT_HFP_SPE_ALG_V2                 := y
# Voice Prompt module
MTK_PROMPT_SOUND_ENABLE               ?= y
MTK_DSP_AUDIO_MESSAGE_ENABLE          := y
MTK_BT_AVM_SHARE_BUF                  := y
# Mute smoother module
AIR_MUTE_SMOOTHER_ENABLE              := y
# Voice PLC module
AIR_VOICE_PLC_ENABLE                  := y
MTK_PLC_USE_PIC                       := y
ifeq ($(AIR_BT_A2DP_SBC_ENABLE), y)
MTK_BT_A2DP_SBC_USE_PIC               := y
endif
ifeq ($(AIR_BT_A2DP_AAC_ENABLE), y)
MTK_BT_A2DP_AAC_USE_PIC               := y
endif
MTK_BT_A2DP_MSBC_USE_PIC              ?= y
# Voice DRC module
AIR_VOICE_DRC_ENABLE                  := y
AIR_FIXED_RATIO_SRC                   ?= y
AIR_ADAPITVE_EQ_DETECTION_USE_PIC_ENABLE               := n
# Sidetone
AIR_SIDETONE_ENABLE                   ?= y
ENABLE_SIDETONE_RAMP_TIMER            ?= y
# Low power level
MTK_LOWPOWER_LEVEL                    := 2

ifeq ($(AIR_VOICE_AGC_ENABLE), y)
MTK_BT_AGC_USE_PIC := y
else
MTK_BT_AGC_USE_PIC := n
endif

ifeq ($(AIR_ANC_ENABLE), y)
ANC_SOURCE_CODE = $(strip $(ROOTDIR))/middleware/airoha/dspfw/anc_protected/port/chip/$(IC_CONFIG)/module.mk
ifeq ($(ANC_SOURCE_CODE), $(wildcard $(ANC_SOURCE_CODE)))
MTK_ANC_LIB_ENABLE                    := n
else
MTK_ANC_LIB_ENABLE                    := y
endif
AIR_ANC_DEBUG_PATH_EANBLE             := y
endif

# Support MIC Loopback AT command
AIR_LINE_IN_ENABLE = y
LINE_IN_PURE_FOR_AMIC_CLASS_G_HQA = y

ifeq ($(AIR_FULL_ADAPTIVE_ANC_ENABLE), y)
FULL_ADAPTIVE_ANC_SOURCE_CODE = $(strip $(ROOTDIR))/middleware/airoha/dspalg/full_adaptive_anc_protected/module.mk
ifeq ($(FULL_ADAPTIVE_ANC_SOURCE_CODE), $(wildcard $(FULL_ADAPTIVE_ANC_SOURCE_CODE)))
AIR_FULL_ADAPT_ANC_LIB_ENABLE         := n
else
AIR_FULL_ADAPT_ANC_LIB_ENABLE         := y
endif
else
ifeq ($(AIR_ANC_ENABLE), y)
AIR_FULL_ADAPT_ANC_LIB_ENABLE         := y
endif
endif

ifeq ($(AIR_FULL_ADAPTIVE_ANC_ENABLE),y)
FULL_ADAPTIVE_ANC_LIB = $(strip $(ROOTDIR))/prebuilt/middleware/airoha/dspalg/full_adaptive_anc/$(IC_CONFIG)/
ifeq ($(FULL_ADAPTIVE_ANC_LIB), $(wildcard $(FULL_ADAPTIVE_ANC_LIB)))
ifeq ($(AIR_FULL_ADAPT_ANC_LIB_ENABLE), y)
include $(ROOTDIR)/prebuilt/middleware/airoha/dspalg/full_adaptive_anc/$(IC_CONFIG)/module.mk
else
include $(ROOTDIR)/middleware/airoha/dspalg/full_adaptive_anc_protected/module.mk
endif
else
#$(error "Error: [Addon error] No corresponding lib for Full Adaptive ANC, please check!")
#@echo "Error: [Addon error] No corresponding lib for Full Adaptive ANC, please check!";
#@set -e; exit 2;
    AIR_FULL_ADAPTIVE_ANC_ENABLE := n
endif
endif

# ANC User Trigger module
ifeq ($(AIR_USER_TRIGGER_FF_ENABLE), y)
ifneq ($(AIR_ANC_ENABLE),y)
    $(error "Error: [Conflict feature option] To enable AIR_USER_TRIGGER_FF_ENABLE must support AIR_ANC_ENABLE")
else
USER_TRIGGER_FF_CODE = $(strip $(ROOTDIR))/middleware/airoha/dspalg/user_trigger_ff/user_trigger_adaptive_ff_v2/module.mk
ifeq ($(USER_TRIGGER_FF_CODE), $(wildcard $(USER_TRIGGER_FF_CODE)))
else
AIR_USER_TRIGGER_FF_ENABLE     = n
endif
endif
endif

# ANC User Unaware module
ifeq ($(AIR_ANC_USER_UNAWARE_ENABLE), y)
ifneq ($(AIR_ANC_ENABLE),y)
    $(error "Error: [Conflict feature option] To enable AIR_ANC_USER_UNAWARE_ENABLE must support AIR_ANC_ENABLE")
else
USER_UNAWARE_CODE = $(strip $(ROOTDIR))/middleware/airoha/dspalg/user_unaware/module.mk
ifeq ($(USER_UNAWARE_CODE), $(wildcard $(USER_UNAWARE_CODE)))
else
AIR_ANC_USER_UNAWARE_ENABLE     = n
endif
endif
endif

# ANC Environment_Detection module
ifeq ($(AIR_ANC_ENVIRONMENT_DETECTION_ENABLE), y)
ifneq ($(AIR_ANC_ENABLE),y)
    $(error "Error: [Conflict feature option] To enable AIR_ANC_ENVIRONMENT_DETECTION_ENABLE must support AIR_ANC_ENABLE")
else
AIR_ANC_ENVIRONMENT_DETECTION_CODE = $(strip $(ROOTDIR))/middleware/airoha/dspalg/env_detect/module.mk
ifeq ($(AIR_ANC_ENVIRONMENT_DETECTION_CODE), $(wildcard $(AIR_ANC_ENVIRONMENT_DETECTION_CODE)))
else
AIR_ANC_ENVIRONMENT_DETECTION_ENABLE     := n
endif
endif
endif

# Clk Skew
AIR_BT_CLK_SKEW_ENABLE = y
ifeq ($(IC_CONFIG),ab156x)
MTK_BT_CLK_SKEW_USE_PIC = y
endif
# Load Clk Skew Lib From Source Code or Object File
CLKSKEW_LIB = $(strip $(ROOTDIR))/middleware/airoha/dspalg/clk_skew_protected/module.mk
ifeq ($(CLKSKEW_LIB), $(wildcard $(CLKSKEW_LIB)))
MTK_BT_CLK_SKEW_LOAD_ENABLE           := y
else
MTK_BT_CLK_SKEW_LOAD_ENABLE           := n
endif

# LE audio dongle
ifeq ($(AIR_LE_AUDIO_DONGLE_ENABLE),y)
ifeq ($(AIR_LE_AUDIO_ENABLE),y)
AIR_LE_AUDIO_ENABLE                   := n
endif
AIR_BLE_AUDIO_DONGLE_ENABLE           := y
AIR_AUDIO_TRANSMITTER_ENABLE          := y
AIR_AUDIO_BT_COMMON_ENABLE            := y
AIR_SOFTWARE_BUFFER_ENABLE            := y
AIR_SOFTWARE_CLK_SKEW_ENABLE          := y
AIR_SOFTWARE_GAIN_ENABLE              := y
AIR_SOFTWARE_MIXER_ENABLE             := y
AIR_SOFTWARE_SRC_ENABLE               := y
LC3_CODEC_LIB = $(strip $(ROOTDIR))/middleware/airoha/dspalg/lc3_codec/module.mk
ifeq ($(LC3_CODEC_LIB), $(wildcard $(LC3_CODEC_LIB)))
AIR_BT_CODEC_BLE_V2_ENABLED           := y
MTK_BT_A2DP_LC3_USE_PIC               := y
MTK_BT_A2DP_LC3_USE_LIGHT_PIC         := n
else
AIR_LE_AUDIO_DONGLE_ENABLE            := n
AIR_BT_CODEC_BLE_V2_ENABLED           := n
MTK_BT_A2DP_LC3_USE_PIC               := n
MTK_BT_A2DP_LC3_USE_LIGHT_PIC         := n
endif
endif

# LC3
ifeq ($(AIR_LE_AUDIO_ENABLE),y)
LC3_CODEC_LIB = $(strip $(ROOTDIR))/middleware/airoha/dspalg/lc3_codec/module.mk
ifeq ($(LC3_CODEC_LIB), $(wildcard $(LC3_CODEC_LIB)))
AIR_BT_CODEC_BLE_ENABLED              := y
AIR_BT_LE_LC3_ENABLE                  := y
MTK_BT_A2DP_LC3_USE_PIC               := y
MTK_BT_A2DP_LC3_USE_LIGHT_PIC         := n
ifeq ($(IC_CONFIG),ab156x)
AIR_BT_BLE_SWB_ENABLE                 := n
else
AIR_BT_BLE_SWB_ENABLE                 := y
endif
else
AIR_LE_AUDIO_ENABLE                   := n
AIR_BT_CODEC_BLE_ENABLED              := n
AIR_BT_LE_LC3_ENABLE                  := n
MTK_BT_A2DP_LC3_USE_PIC               := n
MTK_BT_A2DP_LC3_USE_LIGHT_PIC         := n
AIR_BT_BLE_SWB_ENABLE                 := n
endif
endif



LC3PLUS_LIB = $(strip $(ROOTDIR))/middleware/airoha/dspalg/lc3plus_codec/module.mk
ifeq ($(LC3PLUS_LIB), $(wildcard $(LC3PLUS_LIB)))
else
AIR_ULL_BLE_HEADSET_ENABLE = n
AIR_ULL_AUDIO_V2_DONGLE_ENABLE = n
endif

# ULL 2.0 headset
ifeq ($(AIR_ULL_BLE_HEADSET_ENABLE),y)
AIR_BT_CODEC_BLE_ENABLED              := y
ifeq ($(IC_CONFIG),ab156x)
AIR_BT_BLE_SWB_ENABLE                 := n
else
AIR_BT_BLE_SWB_ENABLE                 := y
endif
AIR_BT_LE_LC3PLUS_ENABLE              := y
AIR_BT_LE_LC3PLUS_USE_PIC             := y
endif



# ULL dongle
ifeq ($(AIR_ULL_GAMING_DONGLE_ENABLE),y)
AIR_GAMING_MODE_DONGLE_ENABLE         := y
AIR_AUDIO_TRANSMITTER_ENABLE          := y
AIR_AUDIO_BT_COMMON_ENABLE            := y
AIR_SOFTWARE_BUFFER_ENABLE            := y
AIR_SOFTWARE_CLK_SKEW_ENABLE          := y
AIR_SOFTWARE_GAIN_ENABLE              := y
AIR_SOFTWARE_MIXER_ENABLE             := y
AIR_SOFTWARE_SRC_ENABLE               := y
endif

# ULL 2.0 dongle
ifeq ($(AIR_ULL_AUDIO_V2_DONGLE_ENABLE),y)
AIR_AUDIO_TRANSMITTER_ENABLE          := y
AIR_AUDIO_BT_COMMON_ENABLE            := y
AIR_SOFTWARE_BUFFER_ENABLE            := y
AIR_SOFTWARE_CLK_SKEW_ENABLE          := y
AIR_SOFTWARE_GAIN_ENABLE              := y
AIR_SOFTWARE_MIXER_ENABLE             := y
AIR_SOFTWARE_SRC_ENABLE               := y
AIR_BT_LE_LC3PLUS_ENABLE              := y
AIR_BT_LE_LC3PLUS_USE_PIC             := y
AIR_FIXED_RATIO_SRC_USE_PIC           := y
endif


##$(error "Error: $(AIR_LE_AUDIO_ENABLE), $(AIR_BT_CODEC_BLE_V2_ENABLED), $(AIR_BT_CODEC_BLE_ENABLED),")

# ADAPTIVE EQ
ifeq ($(AIR_ADAPTIVE_EQ_ENABLE),y)
AUDIO_AEQ_LIB = $(strip $(ROOTDIR))/middleware/airoha/dspalg/adaptive_eq_detection/module.mk
ifeq ($(AUDIO_AEQ_LIB), $(wildcard $(AUDIO_AEQ_LIB)))
include $(ROOTDIR)/middleware/airoha/dspalg/adaptive_eq_detection/module.mk
else
AIR_ADAPTIVE_EQ_ENABLE = n
endif
endif

include $(ROOTDIR)/.config.mk
include $(ROOTDIR)/config/chip/$(IC_CONFIG)/chip.mk

# Select different linker script files by boot method.
# If it is FLASH boot, the script file used must be configured in feature.mk.
TARGET ?= FLASH
ifeq ($(TARGET), IRAM)
ifeq ($(AIR_BTA_IC_PREMIUM_G3),y)
    LSCRIPT      := ab158x_dsp0_iram.lcf
else
    LSCRIPT      := ab156x_dsp0_iram.lcf
endif
else ifeq ($(TARGET), FLASH)
    ifdef AIR_DSP_LINKER_SCRIPT_FILE
        LSCRIPT      := $(AIR_DSP_LINKER_SCRIPT_FILE)
    else
        NoLinkerScript = NotSpecifiedLinkerScript
        LSCRIPT      := $(NoLinkerScript)
    endif
endif

################################################################################
# Include Module Configuration
################################################################################
include $(ROOTDIR)/driver/chip/$(IC_CONFIG)/module.mk
include $(ROOTDIR)/kernel/service/exception_handler/module.mk
include $(ROOTDIR)/kernel/service/mux/module.mk
include $(ROOTDIR)/kernel/service/syslog/module.mk
include $(ROOTDIR)/kernel/service/pre_libloader/dsp0/module.mk
include $(ROOTDIR)/kernel/service/ccci/module.mk
include $(ROOTDIR)/kernel/service/platform/module.mk
include $(ROOTDIR)/kernel/service/heapsize_guard/module.mk


ifeq ($(AIR_BTA_IC_PREMIUM_G3),y)
include $(ROOTDIR)/kernel/service/context_switch/bta_ic_premium_g3/module.mk
else
include $(ROOTDIR)/kernel/service/context_switch/$(IC_CONFIG)/module.mk
endif

include $(ROOTDIR)/kernel/service/swla/module.mk
include $(ROOTDIR)/kernel/service/airo_clib/module.mk
include $(ROOTDIR)/kernel/rtos/FreeRTOS/Source/module.mk

#include $(ROOTDIR)/kernel/rtos/FreeRTOS_test/module.mk
include $(ROOTDIR)/middleware/airoha/dspfw/port/chip/$(IC_CONFIG)/module.mk
include $(ROOTDIR)/middleware/airoha/stream/module.mk
include $(ROOTDIR)/middleware/airoha/dspalg/src/module.mk
include $(ROOTDIR)/middleware/airoha/dspalg/aac_decoder/module.mk
include $(ROOTDIR)/middleware/airoha/dspalg/sbc_decoder/module.mk
include $(ROOTDIR)/middleware/airoha/dspalg/cvsd_decoder/module.mk
include $(ROOTDIR)/middleware/airoha/dspalg/msbc_decoder/module.mk
include $(ROOTDIR)/middleware/airoha/dspalg/compander/module.mk
include $(ROOTDIR)/middleware/airoha/dspalg/agc/module.mk
include $(ROOTDIR)/middleware/airoha/dspalg/ec_nr/module.mk
include $(ROOTDIR)/middleware/airoha/dspalg/clk_skew/module.mk
ifeq ($(AIR_ANC_WIND_NOISE_DETECTION_ENABLE),y)
include $(ROOTDIR)/middleware/airoha/dspalg/wind_detection/module.mk
endif

ifeq ($(AIR_ANC_USER_UNAWARE_ENABLE),y)
include $(ROOTDIR)/middleware/airoha/dspalg/user_unaware/module.mk
endif
ifeq ($(AIR_ANC_ENVIRONMENT_DETECTION_ENABLE),y)
include $(ROOTDIR)/middleware/airoha/dspalg/env_detect/module.mk
endif
ifeq ($(AIR_USER_TRIGGER_FF_ENABLE),y)
include $(ROOTDIR)/middleware/airoha/dspalg/user_trigger_ff/user_trigger_adaptive_ff_v2/module.mk
endif

ifeq ($(AIR_VOLUME_ESTIMATOR_ENABLE),y)
include $(ROOTDIR)/middleware/airoha/dspalg/volume_estimator/module.mk
endif

include $(ROOTDIR)/middleware/airoha/dspalg/ins/module.mk

ifeq ($(MTK_BT_A2DP_AIRO_CELT_ENABLE),y)
include $(ROOTDIR)/middleware/third_party/dspalg/celt_decoder/module.mk
endif

ifeq ($(AIR_CELT_ENC_ENABLE),y)
include $(ROOTDIR)/middleware/third_party/dspalg/celt_encoder/module.mk
endif

ifeq ($(AIR_FIXED_RATIO_SRC),y)
include $(ROOTDIR)/middleware/airoha/dspalg/src_fixed_ratio/module.mk
endif

ifeq ($(AIR_SOFTWARE_SRC_ENABLE),y)
include $(ROOTDIR)/middleware/airoha/dspalg/sw_src/module.mk
endif

ifeq ($(AIR_SOFTWARE_GAIN_ENABLE),y)
include $(ROOTDIR)/middleware/airoha/dspalg/sw_gain/module.mk
endif

ifeq ($(AIR_SOFTWARE_MIXER_ENABLE),y)
include $(ROOTDIR)/middleware/airoha/dspalg/sw_mixer/module.mk
endif

ifeq ($(AIR_SOFTWARE_BUFFER_ENABLE),y)
include $(ROOTDIR)/middleware/airoha/dspalg/sw_buffer/module.mk
endif

ifeq ($(AIR_BT_LE_LC3_ENABLE), y)
include $(ROOTDIR)/middleware/airoha/dspalg/lc3_codec/module.mk
endif
ifeq ($(AIR_BT_CODEC_BLE_V2_ENABLED), y)
include $(ROOTDIR)/middleware/airoha/dspalg/lc3_codec/module.mk
endif


ifeq ($(AIR_BT_A2DP_VENDOR_ENABLE), y)
include $(ROOTDIR)/middleware/third_party/dspalg/vendor_decoder/module.mk
endif

ifeq ($(MTK_BT_CLK_SKEW_LOAD_ENABLE), y)
include $(ROOTDIR)/middleware/airoha/dspalg/clk_skew_protected/module.mk
else
include $(ROOTDIR)/prebuilt/middleware/airoha/dspalg/clk_skew/$(IC_CONFIG)/module.mk
endif

ifeq ($(AIR_AUDIO_PLC_ENABLE),y)
AUDIO_PLC_LIB = $(strip $(ROOTDIR))/middleware/third_party/dspalg/audio_plc/module.mk
ifeq ($(AUDIO_PLC_LIB), $(wildcard $(AUDIO_PLC_LIB)))
include $(ROOTDIR)/middleware/third_party/dspalg/audio_plc/module.mk
else
AIR_AUDIO_PLC_ENABLE = n
endif
endif

ifeq ($(AIR_BTA_IC_PREMIUM_G3),y)
CHIP_NAME = bta_ic_premium_g3
else
CHIP_NAME = $(IC_CONFIG)
endif

ifeq ($(AIR_AMA_HOTWORD_ENABLE),y)
include $(ROOTDIR)/middleware/airoha/dspalg/wwe/module.mk
endif

include $(ROOTDIR)/middleware/airoha/dspalg/voice_plc/module.mk
include $(ROOTDIR)/middleware/airoha/dspalg/peq/module.mk

ifeq ($(AIR_ANC_ENABLE), y)
ifeq ($(MTK_ANC_LIB_ENABLE), y)
include $(ROOTDIR)/prebuilt/middleware/airoha/dspfw/anc/$(IC_CONFIG)/module.mk
else
include $(ROOTDIR)/middleware/airoha/dspfw/anc_protected/port/chip/$(IC_CONFIG)/module.mk
endif
ifeq ($(AIR_ANC_DEBUG_PATH_EANBLE), y)
ANC_DEBUG_PATH_SOURCE_CODE = $(strip $(ROOTDIR))/middleware/airoha/dspfw/anc_protected/debug_path/port/chip/$(IC_CONFIG)/module.mk
ifeq ($(ANC_DEBUG_PATH_SOURCE_CODE), $(wildcard $(ANC_DEBUG_PATH_SOURCE_CODE)))
include $(ROOTDIR)/middleware/airoha/dspfw/anc_protected/debug_path/port/chip/$(IC_CONFIG)/module.mk
endif
endif
endif

ifeq ($(MTK_LEAKAGE_DETECTION_ENABLE), y)
include $(ROOTDIR)/middleware/airoha/dspalg/leakage_compensation/module.mk
endif

ifeq ($(MTK_AUDIO_LOOPBACK_TEST_ENABLE), y)
include $(ROOTDIR)/middleware/airoha/dspalg/audio_loopback_test/module.mk
endif

ifeq ($(MTK_USER_TRIGGER_FF_ENABLE),y)
include $(ROOTDIR)/middleware/airoha/dspalg/user_trigger_ff/user_trigger_adaptive_ff_v2/module.mk
endif

ifeq ($(AIR_ADVANCED_PASSTHROUGH_ENABLE),y)
ifeq ($(AIR_ADVANCED_PASSTHROUGH_LIB_ENABLE),y)
include $(ROOTDIR)/prebuilt/middleware/airoha/dspfw/psap/$(IC_CONFIG)/module.mk
else
include $(ROOTDIR)/middleware/airoha/psap_protected/port/chip/$(IC_CONFIG)/module.mk
endif
endif

ifeq ($(AIR_AFC_ENABLE),y)
ifeq ($(AIR_AFC_LIB_ENABLE),y)
include $(ROOTDIR)/prebuilt/middleware/airoha/dspalg/afc/$(IC_CONFIG)/module.mk
else
include $(ROOTDIR)/middleware/airoha/dspalg/afc_protected/module.mk
endif
endif

ifeq ($(AIR_LD_NR_ENABLE),y)
ifeq ($(AIR_LD_NR_LIB_ENABLE),y)
include $(ROOTDIR)/prebuilt/middleware/airoha/dspalg/ld_nr/$(IC_CONFIG)/module.mk
else
include $(ROOTDIR)/middleware/airoha/dspalg/ld_nr_protected/module.mk
endif
endif

ifeq ($(AIR_AT_AGC_ENABLE),y)
ifeq ($(AIR_AT_AGC_LIB_ENABLE),y)
include $(ROOTDIR)/prebuilt/middleware/airoha/dspalg/at_agc/$(IC_CONFIG)/module.mk
else
include $(ROOTDIR)/middleware/airoha/dspalg/at_agc_protected/module.mk
endif
endif

ifeq ($(AIR_BT_LE_LC3PLUS_ENABLE), y)
include $(ROOTDIR)/middleware/airoha/dspalg/lc3plus_codec/module.mk
endif

################################################################################
# Project Sources/Define/Include Rule List
################################################################################
PROJ_C_SRC   +=  $(strip $(PROJ_PATH))/src/main.c
PROJ_C_SRC   +=  $(strip $(PROJ_PATH))/src/regions_init.c
PROJ_C_SRC   +=  $(strip $(PROJ_PATH))/src/sys_init.c
PROJ_C_SRC   +=  $(strip $(PROJ_PATH))/src/syscalls.c
PROJ_C_SRC   +=  $(strip $(PROJ_PATH))/src/preloader_pisplit_configure.c
ifeq ($(IC_CONFIG),ab156x)
PROJ_ASM_SRC +=  $(strip $(PROJ_PATH))/src/ab156x_STARTUP.S
else
PROJ_ASM_SRC +=  $(strip $(PROJ_PATH))/src/ab158x_STARTUP.S
endif
PROJ_INC     +=  $(strip $(PROJ_PATH))/inc/

CXX_FILES    +=

ASM_SRC      +=  $(PROJ_ASM_SRC)
C_SRC        +=  $(PROJ_C_SRC)
DEFINE       +=  $(PROJ_DEFINE)
INC          +=  $(PROJ_INC)

# .o objects & .d dependency list
OBJ  =  $(ASM_SRC:%.S=$(OUTDIR)/%.o) $(C_SRC:%.c=$(OUTDIR)/%.o)
DEP  =  $(ASM_SRC:%.S=$(OUTDIR)/%.d) $(C_SRC:%.c=$(OUTDIR)/%.d)
CXX_OBJS    = $(CXX_FILES:%.cpp=$(OUTDIR)/%.o)

################################################################################
# PIC lib, also can defined in module's module.mk
################################################################################
#PIC     += $(ROOTDIR)/project/mt2811a_evb/templates/dsp_pisplit_library_build_demo/dsp0/Build/lib/dsp0_pisplit_library_build_demo.o
#PIC     += $(ROOTDIR)/project/mt2811a_evb/templates/dsp_pisplit_library_build_demo/dsp1/Build/lib/dsp1_pisplit_library_build_demo.o
################################################################################
# Libraries Configuration
################################################################################

# Use the library files in the Xtensa toolchain instead of prebuilt.
# LIBS  +=  $(strip $(LIBDIR))/libhal.a
# LIBS  +=  $(strip $(LIBDIR))/libc.a
# LIBS  +=  $(strip $(LIBDIR))/libgcc.a
# LIBS  +=  $(strip $(LIBDIR))/libhandlers-board.a
# LIBS  +=  $(strip $(LIBDIR))/libgloss.a

LIBS_BUILD  +=  $(strip $(BUILD_LIBDIR))/libctxsw.a



################################################################################
# Generate feature option list
################################################################################
$(shell (test -d $(strip $(OUTDIR)) || mkdir -p $(strip $(OUTDIR))))
MTK_AIR_MAKE_VARS := $(shell echo '$(.VARIABLES)' | awk -v RS=' ' '/^[a-zA-Z0-9]+[a-zA-Z0-9_]*[\r\n]*$$/' | grep  "^MTK_\|AIR_")
$(foreach v,$(MTK_AIR_MAKE_VARS), $(shell echo $(v)='$($(v))' >> $(strip $(OUTDIR))/feature_opts_list_checkoption.log))


################################################################################
# Generate predefined files requires for dynamic memory management.
################################################################################
ifeq ($(DSP_MEMORY_MANAGEMENT_EBANLE),y)
DMM_SCRIPT     = $(strip $(ROOTDIR))/kernel/service/DSP_Memory_Managementv1/memory_predefine/dmm_script/dmm.py
INPUT_CSV      = $(strip $(ROOTDIR))/tools/memory/pre_define.csv
INPUT_FEATURE  = $(strip $(OUTDIR))/feature_opts_list_checkoption.log
$(info DMM_SCRIPT: $(DMM_SCRIPT))
$(info INPUT_CSV: $(INPUT_CSV))
$(info INPUT_FEATURE: $(INPUT_FEATURE))
# The output of the script must be imported into the file, otherwise
# according to the rules of the Makefile, the output of the script will be
# replaced here, and the output of the script does not conform to the
# rules of the Makefile, so a build ERROR will be encountered !!!
$(shell (test -d $(strip $(OUTDIR)/dmm_gen/) || mkdir -p $(strip $(OUTDIR)/dmm_gen/)))
$(shell python3 $(DMM_SCRIPT) -f $(INPUT_CSV) -d $(INPUT_FEATURE) -o $(OUTDIR)/dmm_gen \
    > $(OUTDIR)/dmm_gen/build.log 2> $(OUTDIR)/dmm_gen/err.log;)

endif
################################################################################
# Make Target
################################################################################
.PHONY: all clean
all : clean_log checkoption $(IMAGE)

checkoption:
#
ifeq ($(AIR_AMA_HOTWORD_USE_PIC_ENABLE),y)
ifneq ($(AIR_AMA_HOTWORD_ENABLE),y)
	$(warning "Error: [Conflict feature option] To enable AIR_AMA_HOTWORD_USE_PIC_ENABLE must support AIR_AMA_HOTWORD_ENABLE")
	@echo "Error: [Conflict feature option] To enable AIR_AMA_HOTWORD_USE_PIC_ENABLE must support AIR_AMA_HOTWORD_ENABLE";
	@set -e; exit 2;
endif
endif

#
ifeq ($(AIR_GSOUND_HOTWORD_USE_PIC_ENABLE),y)
ifneq ($(AIR_GSOUND_HOTWORD_ENABLE),y)
	$(warning "Error: [Conflict feature option] To enable AIR_GSOUND_HOTWORD_USE_PIC_ENABLE must support AIR_GSOUND_HOTWORD_ENABLE")
	@echo "Error: [Conflict feature option] To enable AIR_GSOUND_HOTWORD_USE_PIC_ENABLE must support AIR_GSOUND_HOTWORD_ENABLE";
	@set -e; exit 2;
endif
endif

#
ifeq ($(AIR_HWSRC_ON_MAIN_STREAM_ENABLE),y)
ifeq ($(AIR_HWSRC_IN_STREAM_ENABLE),y)
	$(warning "Error: [Conflict feature option] To enable AIR_HWSRC_ON_MAIN_STREAM_ENABLE must not support AIR_HWSRC_IN_STREAM_ENABLE")
	@echo "Error: [Conflict feature option] To enable AIR_HWSRC_ON_MAIN_STREAM_ENABLE must not support AIR_HWSRC_IN_STREAM_ENABLE";
	@set -e; exit 2;
endif
endif

#
ifeq ($(AIR_HWSRC_ON_MAIN_STREAM_ENABLE),y)
ifneq ($(AIR_HWSRC_CLKSKEW_ENABLE),y)
	$(warning "Error: [Conflict feature option] To enable AIR_HWSRC_ON_MAIN_STREAM_ENABLE must support AIR_HWSRC_CLKSKEW_ENABLE")
	@echo "Error: [Conflict feature option] To enable AIR_HWSRC_ON_MAIN_STREAM_ENABLE must support AIR_HWSRC_CLKSKEW_ENABLE";
	@set -e; exit 2;
endif
endif

#
ifeq ($(AIR_HWSRC_IN_STREAM_ENABLE),y)
ifneq ($(AIR_HWSRC_CLKSKEW_ENABLE),y)
	$(warning "Error: [Conflict feature option] To enable AIR_HWSRC_IN_STREAM_ENABLE must support AIR_HWSRC_CLKSKEW_ENABLE")
	@echo "Error: [Conflict feature option] To enable AIR_HWSRC_IN_STREAM_ENABLE must support AIR_HWSRC_CLKSKEW_ENABLE";
	@set -e; exit 2;
endif
endif

#
ifeq ($(AIR_HWSRC_CLKSKEW_ENABLE),y)
ifneq ($(AIR_HWSRC_ON_MAIN_STREAM_ENABLE),y)
ifneq ($(AIR_HWSRC_IN_STREAM_ENABLE),y)
	$(warning "Error: [Conflict feature option] To enable AIR_HWSRC_CLKSKEW_ENABLE must support AIR_HWSRC_ON_MAIN_STREAM_ENABLE or AIR_HWSRC_IN_STREAM_ENABLE")
	@echo "Error: [Conflict feature option] To enable AIR_HWSRC_CLKSKEW_ENABLE must support AIR_HWSRC_ON_MAIN_STREAM_ENABLE or AIR_HWSRC_IN_STREAM_ENABLE";
	@set -e; exit 2;
endif
endif
endif

#
ifeq ($(AIR_DOWNLINK_RATE),48k)
ifneq ($(AIR_HWSRC_ON_MAIN_STREAM_ENABLE),y)
ifneq ($(AIR_HWSRC_IN_STREAM_ENABLE),y)
	$(warning "Error: [Conflict feature option] To enable AIR_DOWNLINK_RATE = 48k must support AIR_HWSRC_ON_MAIN_STREAM_ENABLE")
	@echo "Error: [Conflict feature option] To enable AIR_DOWNLINK_RATE = 48k must support AIR_HWSRC_ON_MAIN_STREAM_ENABLE";
	@set -e; exit 2;
endif
endif
endif

#
ifeq ($(AIR_DOWNLINK_RATE),96k)
ifneq ($(AIR_HWSRC_ON_MAIN_STREAM_ENABLE),y)
ifneq ($(AIR_HWSRC_IN_STREAM_ENABLE),y)
	$(warning "Error: [Conflict feature option] To enable AIR_DOWNLINK_RATE = 96k must support AIR_HWSRC_ON_MAIN_STREAM_ENABLE")
	@echo "Error: [Conflict feature option] To enable AIR_DOWNLINK_RATE = 96k must support AIR_HWSRC_ON_MAIN_STREAM_ENABLE";
	@set -e; exit 2;
endif
endif
endif

#
ifeq ($(AIR_FULL_ADAPTIVE_ANC_ENABLE),y)
ifneq ($(AIR_ANC_ENABLE),y)
	$(warning "Error: [Conflict feature option] To enable AIR_FULL_ADAPTIVE_ANC_ENABLE must support AIR_ANC_ENABLE")
	@echo "Error: [Conflict feature option] To enable AIR_FULL_ADAPTIVE_ANC_ENABLE must support AIR_ANC_ENABLE";
	@set -e; exit 2;
endif
endif

#
ifeq ($(AIR_ANC_FIT_DETECTION_ENABLE),y)
ifneq ($(AIR_ANC_ENABLE)_$(AIR_MIC_RECORD_ENABLE),y_y)
	$(warning "Error: [Conflict feature option] To enable AIR_ANC_FIT_DETECTION_ENABLE must support AIR_ANC_ENABLE/AIR_MIC_RECORD_ENABLE")
	@echo "Error: [Conflict feature option] To enable AIR_ANC_FIT_DETECTION_ENABLE must support AIR_ANC_ENABLE/AIR_MIC_RECORD_ENABLE";
	@set -e; exit 2;
endif
endif

#
ifeq ($(AIR_ANC_WIND_NOISE_DETECTION_ENABLE),y)
ifneq ($(AIR_ANC_ENABLE),y)
	$(warning "Error: [Conflict feature option] To enable AIR_ANC_WIND_NOISE_DETECTION_ENABLE must support AIR_ANC_ENABLE")
	@echo "Error: [Conflict feature option] To enable AIR_ANC_WIND_NOISE_DETECTION_ENABLE must support AIR_ANC_ENABLE";
	@set -e; exit 2;
endif
endif

#
ifeq ($(AIR_AMA_HOTWORD_ENABLE),y)
ifeq ($(wildcard $(ROOTDIR)/prebuilt/middleware/airoha/dspalg/wwe/ama),)
	$(warning "Error: [Addon error] No corresponding lib for AMA HOTWORD, please contact Amazon to get the lib. Then put into the folder prebuilt/middleware/airoha/dspalg/wwe/ama")
	@echo "Error: [Addon error] No corresponding lib for AMA HOTWORD, please contact Amazon to get the lib. Then put into the folder prebuilt/middleware/airoha/dspalg/wwe/ama";
	@set -e; exit 2;
endif
endif

ifneq ($(findstring 3RD_PARTY_AI_NR, $(AIR_ECNR_CONFIG_TYPE)),)
ifeq ($(wildcard $(strip $(ROOTDIR))/prebuilt/middleware/third_party/dspalg/igo_nr/),)
	$(warning "Error: [Addon feature option fail] The AI NR addon package do not exist, please contact Airoha PLM to request the AI NR addon package.")
	@echo "[Addon feature option fail] The AI NR addon package do not exist, please contact Airoha PLM to request the AI NR addon package.";
	@set -e; exit 2;
endif
endif

################################################################################
# Compile ComponentPreDefine.o after addon check.
################################################################################
ifeq ($(DSP_MEMORY_MANAGEMENT_EBANLE),y)
include $(ROOTDIR)/kernel/service/DSP_Memory_Managementv1/module.mk
endif

dbg :
	make all CCFLAG='-g -W -O0' LDFLAG='--gc-sections' DEFINE='BUILD_DEBUG BUILD_IODBG BUILD_CATCH BUILD_UARTDUMP BUILD_USR_ALLOC CONFIG_OS_FREERTOS'

clean :
	rm -rf $(OUTDIR)

symtab :
	$(NM)  -gn > null
	perl symtabgen.pl null > $(SYMBOLTAB)

prebuilt_install:
	mkdir -p $(DEFAULT_BIN_PATH)/
	cp -rf $(BINARY) $(DEFAULT_BIN_PATH)/
	cp -rf $(IMAGE) $(DEFAULT_BIN_PATH)/
	cp -rf $(HEX) $(DEFAULT_BIN_PATH)/
	cp -rf $(MAP) $(DEFAULT_BIN_PATH)/
	cp -rf $(DEASM) $(DEFAULT_BIN_PATH)/
	cp -rf $(strip $(OUTDIR))/dsp*_log_str.bin $(DEFAULT_BIN_PATH)/

MO_EXTRA = OUTDIR="$(OUTDIR)" PROJ_CCFLAG="$(CCFLAG)" PROJ_ASFLAG="$(ASFLAG)" PROJ_INC="$(INC)"

$(LIBS_BUILD) :
	make -C $(ROOTDIR)/kernel/service/context_switch/$(CHIP_NAME)/ $(MO_EXTRA)
$(IMAGE) : $(OBJ) $(LIBS) $(LIBS_BUILD)
	@if [ "$(LSCRIPT)" == "$(NoLinkerScript)" ]; then \
		echo >&2 ; \
		echo "=========================================================================" >&2 ; \
		echo "In flash booting mode, the linker script must be specified in feature.mk." >&2 ; \
		echo "=========================================================================" >&2 ; \
		echo >&2 ; \
		exit 68; \
	fi
	@echo Linking... $@ with $(LSCRIPT)
	@cp $(LSCRIPT) $(OUTDIR)
	@cp $(FEATURE) $(OUTDIR)
	@echo Libraries in link stage: $(LIBS) >> $(BUILD_LOG)

# Detect multiple definition:
# @$(LD) -o $@ $(OBJ) $(PIC) -Map $(MAP) -T $(LSCRIPT) $(LDFLAG) --start-group --whole-archive $(LIBS_BUILD) `echo $(LIBS) | sed 's/ /\n/g' | sort -u` $(XTENSA_STANDARD_LIBS) --no-whole-archive --end-group  2>> $(ERR_LOG)

	@echo $(LD) -o $@ $(OBJ) $(OUTDIR)/middleware/airoha/verno/verno.o\
		$(OBJ_ADD_ON) $(PIC) \
		-Map $(MAP) -T $(LSCRIPT) $(LDFLAG) --start-group $(LIBS_BUILD) \
		--whole-archive `echo $(LIBS) | sed 's/ /\n/g' | sort -u` --no-whole-archive \
		$(XTENSA_STANDARD_LIBS) --end-group 2>> $(ERR_LOG)

	@$(LD) -o $@ $(OBJ) $(OUTDIR)/middleware/airoha/verno/verno.o\
		$(OBJ_ADD_ON) $(PIC) \
		-Map $(MAP) -T $(LSCRIPT) $(LDFLAG) --start-group $(LIBS_BUILD) \
		--whole-archive `echo $(LIBS) | sed 's/ /\n/g' | sort -u` --no-whole-archive \
		$(XTENSA_STANDARD_LIBS) --end-group  2>> $(ERR_LOG)

	@$(OBJDUMP) -d $@ > $(DEASM)
	@$(OBJCOPY) -O ihex $@ $(HEX)
	@$(OBJCOPY) -O binary --remove-section=.log_str $@ $(BINARY)
	@$(OBJCOPY) -O binary --only-section=.log_str $@ $(OUTDIR)/dsp0_log_str.bin
	@echo Building Completed

################################################################################
# Include Rules
################################################################################
#Settings for the xtensa tool chain path & license for the project,
#the settings are used by $(ROOTDIR)/rule.mk

#XTENSA_ROOT =
#XTENSA_VER =
#XTENSA_LICENSE_FILE =
include $(ROOTDIR)/.rule.mk
-include $(DEP)
