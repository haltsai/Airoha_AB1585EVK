# Copyright Statement:
#
# (C) 2017  Airoha Technology Corp. All rights reserved.
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

#######################################################
# Project vairables assignment

SOURCE_DIR = ../../../../..
SDK_PATH = $(abspath $(PWD)/$(SOURCE_DIR))
PWD= $(shell pwd)
FEATURE ?= feature.mk
DEBUG = 0
FLOAT_TYPE = hard
BUILD_DIR = $(PWD)/build

# Project name
PROJ_NAME = $(shell basename $(dir $(PWD)))
PROJ_PATH = $(abspath $(dir $(PWD)))/GCC
OUTPATH = $(PWD)/build

#######################################################
# Include Module
include $(FEATURE)

#######################################################
# Not changed feature option for this project.

# BT Dual mode
MTK_BT_DUO_ENABLE = y

#BT Profiles
MTK_BT_AVRCP_ENABLE                 = y
MTK_BT_AVRCP_ENH_ENABLE             = y
MTK_BT_A2DP_ENABLE                  = y
MTK_BT_PBAP_ENABLE                  = n
MTK_BT_AVRCP_BROWSE_ENABLE          = y
MTK_BT_A2DP_AAC_ENABLE              = y

#BT external timer
MTK_BT_TIMER_EXTERNAL_ENABLE          = y

MTK_INITIAL_SYNC_BY_SAMPLE_INDEX_SUPPORT = y
# NVDM gain setting table
MTK_AUDIO_GAIN_TABLE_ENABLE         := y
MTK_MUX_BT_ENABLE                   := y
MTK_BLE_GAP_SRV_ENABLE              := y
AIR_BT_ROLE_HANDOVER_SERVICE_ENABLE := y
BT_ROLE_HANDOVER_WITH_SPP_BLE		:= y

# port service
MTK_PORT_SERVICE_ENABLE             = y
MTK_PORT_SERVICE_BT_ENABLE          = y

ifeq ($(AIR_BT_ROLE_HANDOVER_SERVICE_ENABLE), y)
# Do RHO when agent low battery, power off or disable BT
APPS_AUTO_TRIGGER_RHO       		= y
# Do RHO press key on partner
APPS_TRIGGER_RHO_BY_KEY     		= y
endif

# ANC module
ifeq ($(AIR_ANC_ENABLE), y)
ANC_SOURCE_CODE = $(strip $(SOURCE_DIR))/middleware/airoha/audio/anc_control_protected/GCC/module.mk
ifeq ($(ANC_SOURCE_CODE), $(wildcard $(ANC_SOURCE_CODE)))
MTK_ANC_LIB_ENABLE                  := n
else
MTK_ANC_LIB_ENABLE                  := y
endif
endif

ifeq ($(AIR_LE_AUDIO_ENABLE), y)
AIR_BT_BLE_SWB_ENABLE = y
endif

# prompt sound
AIR_PROMPT_SOUND_ENABLE              := y
MTK_CONN_VP_SYNC_ENABLE              := y
# Race relay cmd
MTK_RACE_RELAY_CMD_ENABLE            := y
#AT Command
MTK_AT_COMMAND                       := y
MTK_ATCI_VIA_PORT_SERVICE            := n
MTK_ATCI_VIA_MUX                     := y
MTK_CAPID_IN_NVDM_AT_COMMAND_ENABLE  := y

# HW IO
MTK_AUDIO_SUPPORT_MULTIPLE_MICROPHONE := y
MTK_AUDIO_HW_IO_CONFIG_ENHANCE        := y

MTK_MUX_AWS_MCE_ENABLE = n
# bluetooth connection manager feature support.
MTK_BT_CM_SUPPORT                   = y
MTK_BT_ENABLE                       = y
MTK_BLE_ONLY_ENABLE                 = n
MTK_AWS_MCE_ENABLE                  = y
ifeq ($(MTK_AWS_MCE_ENABLE),y)
MTK_AWS_MCE_ROLE_RECOVERY_ENABLE = y
ifeq ($(AIR_RACE_CMD_ENABLE),y)
AIR_RACE_DUAL_CMD_ENABLE         = y
endif
endif


# avm direct feature
MTK_AVM_DIRECT                        = y

ifeq ($(AIR_BATTERY_MANAGEMENT_STATIC_INIT_ENABLE),y)
CFLAGS += -DAIR_BATTERY_MANAGEMENT_STATIC_INIT_ENABLE
endif

# GSound
GSOUND_CODE = $(strip $(SOURCE_DIR))/middleware/third_party/gsound/module.mk
ifeq ($(GSOUND_CODE), $(wildcard $(GSOUND_CODE)))
else
AIR_GSOUND_ENABLE                   = n
endif

ifeq ($(AIR_GSOUND_ENABLE),y)
MTK_SBC_ENCODER_ENABLE              = y
endif

# AMA
AMA_CODE = $(strip $(SOURCE_DIR))/middleware/airoha/ama/module.mk
ifneq ($(AMA_CODE), $(wildcard $(AMA_CODE)))
AIR_AMA_ENABLE                      = n
endif

# Check xiaowei middleware module.mk exist or not.
XIAOWEI_CODE = $(strip $(SOURCE_DIR))/middleware/airoha/xiaowei/module.mk
ifneq ($(XIAOWEI_CODE), $(wildcard $(XIAOWEI_CODE)))
AIR_XIAOWEI_ENABLE                  = n
endif

# GATT over EDR feature
MTK_GATT_OVER_BREDR_ENABLE = y

# CFU.
MS_CFU_CODE = $(strip $(SOURCE_DIR))/middleware/airoha/ms_cfu/module.mk
ifneq ($(MS_CFU_CODE), $(wildcard $(MS_CFU_CODE)))
AIR_CFU_ENABLE = n
endif

# Teams
MS_TEAMS_CODE = $(strip $(SOURCE_DIR))/middleware/airoha/ms_teams/module.mk
ifneq ($(MS_TEAMS_CODE), $(wildcard $(MS_TEAMS_CODE)))
AIR_MS_TEAMS_ENABLE = n
endif

# share
MCSYNC_SHARE_CODE = $(strip $(SOURCE_DIR))/middleware/airoha/bt_mcsync_share/module.mk
ifneq ($(MCSYNC_SHARE_CODE), $(wildcard $(MCSYNC_SHARE_CODE)))
AIR_MCSYNC_SHARE_ENABLE = n
endif

# Support MIC Loopback AT command
AIR_LINE_IN_ENABLE = y
LINE_IN_PURE_FOR_AMIC_CLASS_G_HQA = y

# ANC User Trigger module
USER_TRIGGER_FF_CODE = $(strip $(SOURCE_DIR))/middleware/airoha/audio/user_trigger_adaptive_ff/readme.txt
ifneq ($(USER_TRIGGER_FF_CODE), $(wildcard $(USER_TRIGGER_FF_CODE)))
AIR_USER_TRIGGER_FF_ENABLE     = n
endif

# ANC User Unaware module
USER_UNAWARE_CODE = $(strip $(SOURCE_DIR))/../dsp/middleware/airoha/dspalg/user_unaware/module.mk
ifneq ($(USER_UNAWARE_CODE), $(wildcard $(USER_UNAWARE_CODE)))
AIR_ANC_USER_UNAWARE_ENABLE     = n
endif

# ANC Environment_Detection module
AIR_ANC_ENVIRONMENT_DETECTION_CODE = $(strip $(SOURCE_DIR))/../dsp/middleware/airoha/dspalg/env_detect/module.mk
ifneq ($(AIR_ANC_ENVIRONMENT_DETECTION_CODE), $(wildcard $(AIR_ANC_ENVIRONMENT_DETECTION_CODE)))
AIR_ANC_ENVIRONMENT_DETECTION_ENABLE     = n
endif

# ANC Wind_Detection module
AIR_ANC_WIND_DETECTION_CODE = $(strip $(SOURCE_DIR))/../dsp/middleware/airoha/dspalg/wind_detection/module.mk
ifneq ($(AIR_ANC_WIND_DETECTION_CODE), $(wildcard $(AIR_ANC_WIND_DETECTION_CODE)))
AIR_ANC_WIND_NOISE_DETECTION_ENABLE     = n
endif

#######################################################

# LD Flags
LDFLAGS = $(ALLFLAGS) $(FPUFLAGS) --specs=nano.specs -lnosys -nostartfiles
LDFLAGS += -Wl,-wrap=malloc -Wl,-wrap=calloc -Wl,-wrap=realloc -Wl,-wrap=free
LDFLAGS += -Wl,--undefined=uxTopUsedPriority

TARGET ?= FLASH
# Select different linker script files by boot method.
# If it is FLASH boot, the script file used must be configured in feature.mk.
ifeq ($(TARGET), SYSRAM)
    MTK_BOOT_TARGET := SYSRAM
    LSCRIPT      := $(IC_CONFIG)_sysram.ld
    CFLAGS += -DBOOT_METHOD=0
else ifeq ($(TARGET), FLASH)
    MTK_BOOT_TARGET := FLASH
    ifdef AIR_MCU_LINKER_SCRIPT_FILE
        LSCRIPT      := $(AIR_MCU_LINKER_SCRIPT_FILE)
    else
        NoLinkerScript = NotSpecifiedLinkerScript
        LSCRIPT      := $(NoLinkerScript)
    endif
    CFLAGS += -DBOOT_METHOD=1
    LINKER_SCRIPT_PATH = $(SOURCE_DIR)/$(APP_PATH)/GCC/$(LSCRIPT)
endif

LDFLAGS += -Wl,-T$(LSCRIPT)
NVDM_BIN_SIZE=$(shell cat $(AIR_MCU_LINKER_SCRIPT_FILE) | grep nvdm.bin | grep -o "[0-9]*" | tail -n 1 )
NVDM_OU_BIN_SIZE=$(shell cat $(AIR_MCU_LINKER_SCRIPT_FILE) | grep nvdm_ou.bin | grep -o "[0-9]*" | tail -n 1 )

# Auto generate flash_download.cfg file
FLASH_DOWNLOAD_CFG_GENERATOR := $(SOURCE_DIR)/tools/scripts/build/auto_download_cfg.sh

ifeq ($(DEBUG_IO), 1)
LDFLAGS += --specs=rdimon.specs -lrdimon
CFLAGS += -DDEBUG_IO
endif

#######################################################
# Gloabl Config
-include $(SOURCE_DIR)/.config
# IC Config
-include $(SOURCE_DIR)/config/chip/$(IC_CONFIG)/chip.mk
# Board Config
#-include $(SOURCE_DIR)/config/board/$(BOARD_CONFIG)/board.mk
# HAL driver files
include $(SOURCE_DIR)/driver/chip/$(IC_CONFIG)/module.mk
# EPT Config
include $(SOURCE_DIR)/driver/board/$(BOARD_CONFIG)/ept/module.mk
# HW Resource Assignment Config
ifeq ($(AIR_BTA_IC_PREMIUM_G2),y)
include $(SOURCE_DIR)/driver/board/$(BOARD_CONFIG)/hw_resource_assignment/module.mk
endif
# RTOS source files
include $(SOURCE_DIR)/kernel/rtos/FreeRTOS/module.mk

# bsp flash source files
include $(SOURCE_DIR)/driver/board/component/bsp_flash/module.mk

# rofs source files
include $(SOURCE_DIR)/middleware/airoha/rofs/module.mk

# kernel service files
include $(SOURCE_DIR)/kernel/service/module.mk
# NVDM files
ifeq ($(AIR_NVDM_ENABLE),y)
include $(SOURCE_DIR)/middleware/airoha/nvdm/module.mk
CFLAGS += -I$(SOURCE_DIR)/prebuilt/middleware/airoha/nvdm/inc
endif
# add USB CLASS files
ifeq ($(AIR_USB_ENABLE), y)
include $(SOURCE_DIR)/middleware/airoha/usb/module.mk
endif

ifeq ($(MTK_PORT_SERVICE_ENABLE), y)
# add PORT SERVICE files
ifeq ($(AIR_IAP2_VIA_MUX_ENABLE),y)
MTK_IAP2_VIA_MUX_ENABLE = y
MTK_IAP2_PROFILE_ENABLE = y
else
MTK_IAP2_VIA_MUX_ENABLE = n
endif
include $(SOURCE_DIR)/middleware/airoha/port_service/module.mk
ifeq ($(MTK_PORT_SERVICE_BT_ENABLE),y)
include $(SOURCE_DIR)/middleware/airoha/bt_air/GCC/module.mk
endif
endif

ULL2_CODE = $(strip $(SOURCE_DIR))/middleware/airoha/bt_ultra_low_latency/module.mk
ifneq ($(ULL2_CODE), $(wildcard $(ULL2_CODE)))
AIR_BLE_ULTRA_LOW_LATENCY_ENABLE     = n
endif
ifeq ($(AIR_BLE_ULTRA_LOW_LATENCY_ENABLE),y)
include $(SOURCE_DIR)/middleware/airoha/bt_ultra_low_latency/module.mk
endif

# Enable Battery management
ifeq ($(AIR_BTA_IC_PREMIUM_G3),y)
MTK_BATTERY_MANAGEMENT_ENABLE       = y
endif
ifeq ($(AIR_BTA_IC_PREMIUM_G2),y)
MTK_BATTERY_MANAGEMENT_ENABLE       = y
endif

ifeq ($(MTK_MUX_BT_ENABLE),y)
CFLAGS += -DMTK_MUX_BT_ENABLE
endif

ifeq ($(AIR_RACE_CMD_ENABLE), y)
ifeq ($(AIR_RACE_FIND_ME_ENABLE), y)
CFLAGS += -DRACE_FIND_ME_ENABLE
endif
include $(SOURCE_DIR)/middleware/airoha/race_cmd/module.mk
endif

ifeq ($(AIR_BT_ROLE_HANDOVER_SERVICE_ENABLE), y)
CFLAGS += -DSUPPORT_ROLE_HANDOVER_SERVICE
# bt bt_role_handover_service
include $(SOURCE_DIR)/middleware/airoha/bt_role_handover/module.mk
endif

ifeq ($(MTK_AWS_MCE_ROLE_RECOVERY_ENABLE), y)
CFLAGS += -DMTK_AWS_MCE_ROLE_RECOVERY_ENABLE
endif

# isink led files
include $(SOURCE_DIR)/driver/board/component/bsp_led/module.mk
CFLAGS += -DLED_ENABLE
CFLAGS += -DAIR_LED_ENABLE

# Bluetooth module
include $(SOURCE_DIR)/middleware/airoha/bluetooth/module.mk

# BT callback manager
include $(SOURCE_DIR)/middleware/airoha/bt_callback_manager/module.mk

# BT connection manager
include $(SOURCE_DIR)/middleware/airoha/bt_connection_manager/module.mk

# bt components le module
include $(SOURCE_DIR)/middleware/airoha/bluetooth_service/module.mk

# ble battery service module
#include $(SOURCE_DIR)/middleware/airoha/ble_bas/module.mk

# IAS service module
ifeq ($(MTK_BLE_IAS), y)
include $(SOURCE_DIR)/middleware/airoha/ble_ias/module.mk
endif

# DIS service module
include $(SOURCE_DIR)/middleware/airoha/ble_dis/module.mk

#AWS MCE Report
ifeq ($(MTK_AWS_MCE_ENABLE), y)
include $(SOURCE_DIR)/middleware/airoha/bt_aws_mce_report/module.mk
endif
# BT Sink service module
include $(SOURCE_DIR)/middleware/airoha/sink/module.mk

# ATCI module
include $(SOURCE_DIR)/middleware/airoha/atci/module.mk

# Audio module
include $(SOURCE_DIR)/middleware/airoha/audio/port/$(IC_CONFIG)/module.mk

# audio middleware files
include $(SOURCE_DIR)/middleware/airoha/audio/module.mk

# include the nanopb module.mk
include $(SOURCE_DIR)/middleware/third_party/nanopb/module.mk

ifeq ($(BOARD_TYPE), ab1588_cell_v4)
CFLAGS += -DAIR_TEMP_FEATURE_ENABLE
endif

# include ANC module
ifeq ($(AIR_ANC_ENABLE), y)
ifeq ($(MTK_ANC_LIB_ENABLE), y)
include $(SOURCE_DIR)/prebuilt/middleware/airoha/audio/anc/module.mk
else
include $(SOURCE_DIR)/middleware/airoha/audio/anc_control_protected/GCC/module.mk
endif
endif


ifeq ($(AIR_FOTA_ENABLE), y)
include $(SOURCE_DIR)/middleware/airoha/fota/module.mk
endif

include $(SOURCE_DIR)/middleware/third_party/mbedtls/module.mk

# CFLAGS += -DMTK_AUDIO_AT_COMMAND_DISABLE #Audio need AT CMD

ifeq ($(MTK_BATTERY_MANAGEMENT_ENABLE), y)
include $(SOURCE_DIR)/middleware/airoha/battery_management/module.mk
endif

ifeq ($(AIR_AIRO_KEY_ENABLE), y)
ifeq ($(AIR_EINT_KEY_ENABLE), y)
include $(SOURCE_DIR)/driver/board/component/bsp_eint_key/module.mk
endif
include $(SOURCE_DIR)/middleware/airoha/key/module.mk
endif

ifeq ($(MTK_BATTERY_EOC_ENTER_RTC),y)
CFLAGS += -DMTK_BATTERY_EOC_ENTER_RTC
endif

# Smart Charger 1wire
ifeq ($(AIR_1WIRE_ENABLE), y)
include $(SOURCE_DIR)/middleware/airoha/smart_charger/module.mk
endif


ifeq ($(MTK_HCI_LOG_MIX_WITH_SYSLOG), y)
CFLAGS += -DHCI_LOG_MIX_WITH_SYSLOG
endif

ifeq ($(MTK_BT_ENABLE),y)
include $(SOURCE_DIR)/middleware/third_party/micro_ecc/module.mk
endif

ifeq ($(AIR_BT_FAST_PAIR_ENABLE),y)
ifneq ($(wildcard $(strip $(SOURCE_DIR))/middleware/airoha/bt_fast_pair/),)
include $(SOURCE_DIR)/middleware/airoha/bt_fast_pair/module.mk
endif
endif

# ui shell
include $(SOURCE_DIR)/middleware/airoha/ui_shell/module.mk

# GSound
ifeq ($(AIR_GSOUND_ENABLE),y)
include $(SOURCE_DIR)/middleware/third_party/gsound/module.mk
endif

# AMA
ifeq ($(AIR_AMA_ENABLE),y)
include $(SOURCE_DIR)/middleware/airoha/ama/module.mk
endif

# Xiaowei middleware module.mk
ifeq ($(AIR_XIAOWEI_ENABLE),y)
CFLAGS += -DAIR_XIAOWEI_ENABLE
include $(SOURCE_DIR)/middleware/airoha/xiaowei/module.mk
endif # End xiaowei

# isink led files
#include $(SOURCE_DIR)/driver/board/$(BOARD_CONFIG)/led/module.mk
#CFLAGS += -DAIR_LED_ENABLE


# LE Audio
ifeq ($(AIR_LE_AUDIO_ENABLE), y)
LE_AUDIO_LIB = $(strip $(SOURCE_DIR))/middleware/airoha/bt_le_audio/module.mk
ifeq  ($(LE_AUDIO_LIB), $(wildcard $(LE_AUDIO_LIB)))
CFLAGS += -DBT_LE_AUDIO_ENABLE
CFLAGS += -DBT_EATT_ENABLE
CFLAGS += -DAIR_LE_AUDIO_ENABLE
CFLAGS += -DAIR_LE_AUDIO_CIS_ENABLE
CFLAGS += -DAIR_BT_CODEC_BLE_ENABLED
CFLAGS += -DAPPS_MULTI_INSTANCE_SERVICE_DISCOVERY_ENABLE

include $(SOURCE_DIR)/middleware/airoha/bt_le_audio/module.mk

ifeq ($(AIR_MULTI_POINT_ENABLE), y)
CFLAGS += -DAIR_LE_AUDIO_MULTIPOINT_ENABLE
endif
ifeq ($(AIR_LE_AUDIO_BIS_ENABLE), y)
CFLAGS += -DAIR_LE_AUDIO_BIS_ENABLE
endif
ifneq ($(AIR_XIAOAI_ENABLE), y)
CFLAGS += -DAIR_LE_AUDIO_MTK_HFP_AT_CMD
endif
# BLE audio dongle
ifeq ($(AIR_LE_AUDIO_DONGLE_ENABLE), y)
CFLAGS += -DAIR_BLE_AUDIO_DONGLE_ENABLE
endif
endif
endif

#quickly reconnecting feature
ifeq ($(AIR_LE_AUDIO_ENABLE),y)
ifeq ($(AIR_LE_AUDIO_QUICK_RECONN_ENABLE),y)
CFLAGS += -DAIR_LE_AUDIO_QUICK_RECONN_ENABLE
endif
endif

ifeq ($(MTK_GATT_OVER_BREDR_ENABLE),y)
CFLAGS += -DMTK_GATT_OVER_BREDR_ENABLE
endif

ifeq ($(AIR_CFU_ENABLE), y)
include $(SOURCE_DIR)/middleware/airoha/ms_cfu/module.mk
endif

ifeq ($(AIR_MS_TEAMS_ENABLE), y)
include $(SOURCE_DIR)/middleware/airoha/ms_teams/module.mk
endif

ifeq ($(AIR_MCSYNC_SHARE_ENABLE), y)
include $(SOURCE_DIR)/middleware/airoha/bt_mcsync_share/module.mk
endif

#######################################################
# Main APP files
APP_PATH        := $(patsubst $(SDK_PATH)/%,%,$(abspath $(dir $(PWD))))
APP_PATH_SRC    := $(APP_PATH)/src

include $(SOURCE_DIR)/$(APP_PATH_SRC)/apps/module.mk

# Config bin path
CONFIG_BIN_PATH := $(SOURCE_DIR)/$(APP_PATH)/config_bin/$(BOARD_TYPE)


##############################################################################
#
# SDK source files
#
##############################################################################
APP_FILES      += $(APP_PATH_SRC)/main.c
APP_FILES      += $(APP_PATH)/GCC/syscalls.c
APP_FILES      += $(APP_PATH_SRC)/regions_init.c
APP_FILES      += $(APP_PATH_SRC)/sys_init.c
APP_FILES      += $(APP_PATH_SRC)/boards/$(BOARD_TYPE)/ept_gpio_var.c
APP_FILES      += $(APP_PATH_SRC)/boards/$(BOARD_TYPE)/ept_eint_var.c
APP_FILES      += $(APP_PATH_SRC)/task_def.c
APP_FILES      += $(APP_PATH_SRC)/bt_init.c
APP_FILES      += $(APP_PATH_SRC)/bt_app_utility/hci_log.c
APP_FILES      += $(APP_PATH_SRC)/dcxo_capid.c
APP_FILES      += $(APP_PATH_SRC)/nvdm_config.c
APP_FILES      += $(APP_PATH_SRC)/nvdm_config_factory_reset.c
APP_FILES      += $(APP_PATH_SRC)/bt_app_utility/bt_power_on_config.c

APP_FILES      += $(APP_PATH_SRC)/bt_app_utility/gatt_service.c \
                  $(APP_PATH_SRC)/bt_app_utility/bt_gattc_discovery.c \
                  $(APP_PATH_SRC)/bt_app_utility/bt_app_common.c \
                  $(APP_PATH_SRC)/bt_app_utility/bt_app_common_at_cmd.c \
                  $(APP_PATH_SRC)/bt_app_utility/bt_app_dis.c \
                  $(APP_PATH_SRC)/bt_app_utility/bt_customer_config.c

ifeq ($(AIR_RACE_CMD_ENABLE), y)
APP_FILES      += $(APP_PATH_SRC)/race/race_app_aws_event_hdl.c \
                  $(APP_PATH_SRC)/race/race_app_race_event_hdl.c \
                  $(APP_PATH_SRC)/race/race_app_bt_event_hdl.c
endif

SYS_FILES      += $(APP_PATH_SRC)/system_cmsis.c

C_FILES        += $(APP_FILES) $(SYS_FILES)
S_FILES        += $(APP_PATH)/GCC/startup_$(IC_CONFIG).s

APP_FILES      += $(APP_PATH_SRC)/bt_app_utility/low_power_demo/bt_at_cmd.c

ifeq ($(BT_ROLE_HANDOVER_WITH_SPP_BLE), y)
CFLAGS += -DBT_ROLE_HANDOVER_WITH_SPP_BLE
endif

# Add IA2P profile support
ifeq ($(AIR_IAP2_VIA_MUX_ENABLE),y)
APP_FILES      += $(APP_PATH_SRC)/iap2/iAP2_config.c
IAP2_CODE = $(strip $(SOURCE_DIR))/middleware/airoha/iap2/GCC/module.mk
ifeq ($(IAP2_CODE), $(wildcard $(IAP2_CODE)))
CFLAGS += -DMTK_IAP2_PROFILE_ENABLE
CFLAGS += -DMTK_IAP2_VIA_MUX_ENABLE
include $(SOURCE_DIR)/middleware/airoha/iap2/GCC/module.mk
include $(SOURCE_DIR)/middleware/airoha/mfi_coprocessor/module.mk
endif
endif

ifeq ($(AIR_MULTI_POINT_ENABLE),y)
CFLAGS += -DAIR_MULTI_POINT_ENABLE
endif


#C_FILES += $(APP_PATH_SRC)/ble_bas_app/ble_bas_app.c
#C_FILES += $(APP_PATH_SRC)/ble_find_me_server/ble_find_me_server.c


##############################################################################
#
# SDK object files
#
##############################################################################


C_OBJS          = $(C_FILES:%.c=$(BUILD_DIR)/%.o)
CXX_OBJS        = $(CXX_FILES:%.cpp=$(BUILD_DIR)/%.o)
S_OBJS          = $(S_FILES:%.s=$(BUILD_DIR)/%.o)
CXX_OBJS        = $(CXX_FILES:%.cpp=$(BUILD_DIR)/%.o)

# TODO: need to include secure world function *.o
ifneq ($(AIR_LIMIT_TZ_ENABLE),n)
    ifeq ($(TARGET), SYSRAM)
    C_OBJS     += $(SOURCE_DIR)/prebuilt/middleware/airoha/security_fw/veneer_table.o
    else ifeq ($(TARGET), FLASH)
    C_OBJS     += $(SOURCE_DIR)/prebuilt/middleware/airoha/security_fw_rom/veneer_table.o
    endif
endif

# security compile option
ifeq ($(AIR_BTA_IC_PREMIUM_G3),y)
ifeq ($(AIR_LIMIT_TZ_ENABLE),n)
CFLAGS += -mcmse
endif
endif
#######################################################
# Include path

CFLAGS += -I$(SOURCE_DIR)/$(APP_PATH)/inc
CFLAGS += -I$(SOURCE_DIR)/driver/CMSIS/Include
CFLAGS += -I$(SOURCE_DIR)/driver/CMSIS/Device/airoha/$(IC_CONFIG)/Include
CFLAGS += -I$(SOURCE_DIR)/$(APP_PATH)/inc/bt_app_utility
#CFLAGS += -I$(SOURCE_DIR)/$(APP_PATH)/inc/ble_find_me_server
#CFLAGS += -I$(SOURCE_DIR)/$(APP_PATH)/inc/ble_bas_app
ifeq ($(AIR_RACE_CMD_ENABLE), y)
CFLAGS += -I$(SOURCE_DIR)/$(APP_PATH)/inc/race
endif
ifeq ($(AIR_PROMPT_SOUND_ENABLE), y)
CFLAGS += -I$(SOURCE_DIR)/$(APP_PATH)/inc/voice_tone
endif
CFLAGS += -I$(SOURCE_DIR)/$(APP_PATH)/inc/boards/$(BOARD_TYPE)
CFLAGS += -I$(SOURCE_DIR)/$(APP_PATH)/src/boards/$(BOARD_TYPE)

ifeq ($(AIR_IAP2_VIA_MUX_ENABLE), y)
CFLAGS += -I$(SOURCE_DIR)/$(APP_PATH)/inc/iap2
endif

######################################################
# Transfer the feature option to the LM generator
# Check AMA and AMA Hotword enabled or not
AMA_HOTWORD_ENABLED = n
GVA_HOTWORD_ENABLED = n
ifeq ($(AIR_AMA_ENABLE)_$(AIR_AMA_HOTWORD_ENABLE),y_y)
AMA_HOTWORD_ENABLED = y
endif
# Check GVA and GVA Hotword enabled or not
ifeq ($(AIR_GSOUND_ENABLE)_$(AIR_GSOUND_HOTWORD_ENABLE),y_y)
GVA_HOTWORD_ENABLED = y
endif

ifeq ($(findstring y,$(GVA_HOTWORD_ENABLED) $(AMA_HOTWORD_ENABLED)),y)
    # Check the VA model manager folder exist or not
    VA_MODEL_MANAGER_CODE = $(strip $(SOURCE_DIR))/middleware/airoha/va_model_manager/module.mk

    ifeq ($(VA_MODEL_MANAGER_CODE), $(wildcard $(VA_MODEL_MANAGER_CODE)))
        # Exist the va model manager folder, enable the feature option and include the module.mk
        AIR_VA_MODEL_MANAGER_ENABLE = y
        CFLAGS += -DAIR_VA_MODEL_MANAGER_ENABLE
        CFLAGS += -DMTK_VA_MODEL_MANAGER_ENABLE
        include $(SOURCE_DIR)/middleware/airoha/va_model_manager/module.mk
    else
        # Do not exist the va model manager folder, pop up a build error
        $(warning "Error: [Addon feature option fail] The va_model_manager addon package do not exist, please contact the owner to fix it.")
        @echo "[Addon feature option fail] The va_model_manager addon package do not exist, please contact the owner to fix it.";
        @set -e; exit 2;
    endif
endif

#######################################################
# Rules

.PHONY: proj clean $(MODULE_PATH)

all: cleanlog checkoption proj
	@mkdir -p $(BUILD_DIR)
	@$(SIZE) $(OUTPATH)/$(PROJ_NAME).elf
	@if [ -e $(CONFIG_BIN_PATH)/nvdm.bin ]; then \
		cp -f $(CONFIG_BIN_PATH)/nvdm.bin $(OUTPATH)/; \
	else \
		if [ "$(AIR_BTA_IC_PREMIUM_G2)" = "y" ]; then \
			$(SOURCE_DIR)/$(POSTBUILD) -f nv2bin -size $(NVDM_BIN_SIZE) -o $(OUTPATH)/ -s "$(SOURCE_DIR)/config/chip/$(IC_CONFIG)/nvkey.xml,$(CONFIG_BIN_PATH)/nvkey.xml" -mode "3" >&2; \
			if [ "$$?" != "0" ]; then \
				echo; echo "error: Failed to generate nvdm binary file in mode 3"; echo; \
				set -e; exit 1; \
			else \
			   echo; echo "Succeeded to generate nvdm binary file in mode 3"; echo; \
			fi; \
		else \
			$(SOURCE_DIR)/$(POSTBUILD) -f nv2bin -size $(NVDM_OU_BIN_SIZE) -o $(OUTPATH)/ -s "$(SOURCE_DIR)/config/chip/$(IC_CONFIG)/nvkey.xml,$(CONFIG_BIN_PATH)/nvkey.xml" -mode "1" >&2; \
			if [ "$$?" != "0" ]; then \
				echo; echo "error: Failed to generate nvdm binary file in mode 1"; echo; \
				set -e; exit 1; \
			else \
			   echo; echo "Succeeded to generate nvdm binary file in mode 1"; echo; \
			fi; \
			$(SOURCE_DIR)/$(POSTBUILD) -f nv2bin -size $(NVDM_BIN_SIZE) -o $(OUTPATH)/ -s "$(SOURCE_DIR)/config/chip/$(IC_CONFIG)/nvkey.xml,$(CONFIG_BIN_PATH)/nvkey.xml" -mode "2" >&2; \
			if [ "$$?" != "0" ]; then \
				echo; echo "error: Failed to generate nvdm binary file in mode 2"; echo; \
				set -e; exit 1; \
			else \
			   echo "Succeeded to generate nvdm binary file in mode 2"; echo; \
			fi; \
		fi \
	fi
	@cp -f $(SOURCE_DIR)/config/chip/$(IC_CONFIG)/nvkey.xml $(OUTPATH)/nvkey_chip.xml
	@cp -f $(CONFIG_BIN_PATH)/nvkey.xml $(OUTPATH)/

	# For generate the language model binary file and copy the binary file to the output path.
	@if [ "$(AIR_VA_MODEL_MANAGER_ENABLE)" = "y" ]; then \
		echo; \
		echo "==============================================================="; \
		echo "= Start to generate language model binary"; \
		echo "==============================================================="; \
		$(SOURCE_DIR)/$(LM_BIN_GENERATOR) -i $(CONFIG_BIN_PATH)/language_model/config.xml -o $(OUTPATH) -l $(AIR_MCU_LINKER_SCRIPT_FILE) -a $(AMA_HOTWORD_ENABLED) -g $(GVA_HOTWORD_ENABLED); \
		if [ "$$?" != "0" ]; then \
			echo; echo "error: Failed to generate language model binary" >&2; echo; \
			set -e; exit 1; \
		fi; \
		echo "==============================================================="; \
		echo "= Generate language model binary finished"; \
		echo "==============================================================="; \
		echo; \
	fi

	@cp -f $(CONFIG_BIN_PATH)/filesystem.bin $(OUTPATH)/
	@if [ "$(TARGET)" = "FLASH" ]; then $(FLASH_DOWNLOAD_CFG_GENERATOR) $(LINKER_SCRIPT_PATH) $(OUTPATH) $(IC_CONFIG) MCU_FW; fi
	@$(SOURCE_DIR)/tools/scripts/build/copy_firmware.sh $(SOURCE_DIR) $(OUTPATH) $(IC_CONFIG) $(BOARD_CONFIG) $(PROJ_NAME).bin $(PWD)

MOD_EXTRA = BUILD_DIR=$(BUILD_DIR) OUTPATH=$(OUTPATH) PROJ_PATH=$(PROJ_PATH) IC_CONFIG=$(IC_CONFIG)

checkoption:
# fota related files
ifeq ($(AIR_FOTA_VIA_RACE_CMD_ENABLE),y)
ifneq ($(AIR_FOTA_ENABLE),y)
	$(warning "Error: [Conflict feature option] To enable AIR_FOTA_VIA_RACE_CMD_ENABLE must support AIR_FOTA_ENABLE")
	@echo "Error: [Conflict feature option] To enable AIR_FOTA_VIA_RACE_CMD_ENABLE must support AIR_FOTA_ENABLE";
	@set -e; exit 2;
endif
endif

ifeq ($(AIR_ANC_FIT_DETECTION_ENABLE),y)
ifneq ($(AIR_ANC_ENABLE)_$(AIR_RECORD_ENABLE)_$(AIR_RACE_CMD_ENABLE),y_y_y)
	$(warning "Error: [Conflict feature option] To enable AIR_ANC_FIT_DETECTION_ENABLE must support AIR_RECORD_ENABLE/AIR_ANC_ENABLE/AIR_RACE_CMD_ENABLE")
	@echo "Error: [Conflict feature option] To enable AIR_ANC_FIT_DETECTION_ENABLE must support AIR_RECORD_ENABLE/AIR_ANC_ENABLE/AIR_RACE_CMD_ENABLE";
	@set -e; exit 2;
endif
endif

ifeq ($(AIR_SMART_CHARGER_ENABLE),y)
ifneq ($(AIR_1WIRE_ENABLE),y)
	$(warning "Error: [Conflict feature option] To enable AIR_SMART_CHARGER_ENABLE must support AIR_1WIRE_ENABLE")
	@echo "Error: [Conflict feature option] To enable AIR_SMART_CHARGER_ENABLE must support AIR_1WIRE_ENABLE";
	@set -e; exit 2;
endif
endif

ifeq ($(AIR_AMA_ENABLE),y)
ifneq ($(AIR_AUDIO_CODEC_MANAGER_ENABLE)_$(AIR_OPUS_ENCODER_ENABLE)_$(AIR_RECORD_ENABLE),y_y_y)
	$(warning "Error: [Conflict feature option] To enable AIR_AMA_ENABLE must support AIR_AUDIO_CODEC_MANAGER_ENABLE/AIR_OPUS_ENCODER_ENABLE/AIR_RECORD_ENABLE")
	@echo "Error: [Conflict feature option] To enable AIR_AMA_ENABLE must support AIR_AUDIO_CODEC_MANAGER_ENABLE/AIR_OPUS_ENCODER_ENABLE/AIR_RECORD_ENABLE";
	@set -e; exit 2;
endif
endif

ifeq ($(AIR_GSOUND_ENABLE),y)
ifneq ($(AIR_RECORD_ENABLE),y)
	$(warning "Error: [Conflict feature option] To enable AIR_GSOUND_ENABLE must support AIR_RECORD_ENABLE")
	@echo "Error: [Conflict feature option] To enable AIR_GSOUND_ENABLE must support AIR_RECORD_ENABLE";
	@set -e; exit 2;
endif
endif

ifeq ($(AIR_XIAOWEI_ENABLE),y)
ifneq ($(AIR_AUDIO_CODEC_MANAGER_ENABLE)_$(AIR_OPUS_ENCODER_ENABLE)_$(AIR_RECORD_ENABLE),y_y_y)
	$(warning "Error: [Conflict feature option] To enable AIR_XIAOWEI_ENABLE must support AIR_AUDIO_CODEC_MANAGER_ENABLE/AIR_OPUS_ENCODER_ENABLE/AIR_RECORD_ENABLE")
	@echo "Error: [Conflict feature option] To enable AIR_XIAOWEI_ENABLE must support AIR_AUDIO_CODEC_MANAGER_ENABLE/AIR_OPUS_ENCODER_ENABLE/AIR_RECORD_ENABLE";
	@set -e; exit 2;
endif
endif

ifeq ($(AIR_BT_FAST_PAIR_SASS_ENABLE)_$(AIR_MULTI_POINT_ENABLE),y_y)
ifneq ($(AIR_BT_SINK_SRV_STATE_MANAGER_ENABLE),y)
	$(warning "Error: [Conflict feature option] To enable AIR_BT_FAST_PAIR_SASS_ENABLE and AIR_MULTI_POINT_ENABLE must support AIR_BT_SINK_SRV_STATE_MANAGER_ENABLE")
	@echo "Error: [Conflict feature option] To enable AIR_BT_FAST_PAIR_SASS_ENABLE and AIR_MULTI_POINT_ENABLE must support AIR_BT_SINK_SRV_STATE_MANAGER_ENABLE";
	@set -e; exit 2;
endif
endif

# Add make error for AMA and AMA Hotword feature enabled, but do not exist language model.
ifeq ($(AIR_AMA_ENABLE)_$(AIR_AMA_HOTWORD_ENABLE),y_y)
ifeq ($(wildcard $(SOURCE_DIR)/prebuilt/middleware/third_party/language_model/ama),)
	$(warning "Error: [Addon feature option fail] The AMA language model for wake word detection is not exist, please contact Amazon to get the language model. Then put into the folder prebuilt/middleware/third_party/language_model/ama")
	@echo "[Addon feature option fail] The AMA language model for wake word detection is not exist, please contact Amazon to get the language model. Then put into the folder prebuilt/middleware/third_party/language_model/ama";
	@set -e; exit 2;
endif
endif

# Add make error for Secure Boot enabled, but do not exist Secure Boot prebuilt library.
ifeq ($(AIR_SECURITY_SECURE_BOOT_ENABLE),y)
ifeq ($(wildcard $(strip $(SOURCE_DIR))/prebuilt/middleware/airoha/secure_boot/),)
	$(warning "Error: [Addon feature option fail] The secure boot addon package doesn't exist. Please contact Airoha to get this addon package. ")
	@echo "[Addon feature option fail] The secure boot addon package doesn't exist. Please contact Airoha to get this addon package.";
	@set -e; exit 2;
endif
endif

ifeq ($(findstring y,$(AIR_LE_AUDIO_ENABLE) $(AIR_LE_AUDIO_DONGLE_ENABLE)),y)
ifeq ($(wildcard $(strip $(SOURCE_DIR))/middleware/airoha/bt_le_audio/),)
	$(warning "Error: [Addon feature option fail] The bt_le_audio addon package do not exist, please contact Airoha PLM to request the LE Audio addon package.")
	@echo "[Addon feature option fail] The bt_le_audio addon package do not exist, please contact Airoha PLM to request the LE Audio addon package.";
	@set -e; exit 2;
endif
endif

ifneq ($(findstring 3RD_PARTY_AI_NR, $(AIR_ECNR_CONFIG_TYPE)),)
ifeq ($(wildcard $(strip $(SOURCE_DIR))/../dsp/prebuilt/middleware/third_party/dspalg/igo_nr/),)
	$(warning "Error: [Addon feature option fail] The AI NR addon package do not exist, please contact Airoha PLM to request the AI NR addon package.")
	@echo "[Addon feature option fail] The AI NR addon package do not exist, please contact Airoha PLM to request the AI NR addon package.";
	@set -e; exit 2;
endif
endif

# Add make error for Ultra Low Latency enabled.
ifeq ($(AIR_BT_ULTRA_LOW_LATENCY_ENABLE),y)
	$(warning "Error: [Conflict feature option] the BT_ULTRA_LOW_LATENCY feature is not support ")
	@echo "Error: [Conflict feature option] the BT_ULTRA_LOW_LATENCY feature is not support";
	@set -e; exit 2;
endif

# 3rd party BLE advertising module
ifeq ($(AIR_TILE_ENABLE), y)
ifneq ($(AIR_APPS_POWER_SAVE_ENABLE), y)
	$(warning "Error: [Conflict feature option] To enable AIR_TILE_ENABLE must support AIR_APPS_POWER_SAVE_ENABLE")
	@echo "Error: [Conflict feature option] To enable AIR_TILE_ENABLE must support AIR_APPS_POWER_SAVE_ENABLE";
	@set -e; exit 2;
endif
endif

# ANC User Trigger module
ifeq ($(AIR_USER_TRIGGER_FF_ENABLE), y)
ifneq ($(AIR_ANC_ENABLE),y)
	$(warning "Error: [Conflict feature option] To enable AIR_USER_TRIGGER_FF_ENABLE must support AIR_ANC_ENABLE")
	@echo "Error: [Conflict feature option] To enable AIR_USER_TRIGGER_FF_ENABLE must support AIR_ANC_ENABLE";
	@set -e; exit 2;
endif
endif

# ANC User Unaware module
ifeq ($(AIR_ANC_USER_UNAWARE_ENABLE), y)
ifneq ($(AIR_ANC_ENABLE),y)
	$(warning "Error: [Conflict feature option] To enable AIR_ANC_USER_UNAWARE_ENABLE must support AIR_ANC_ENABLE")
	@echo "Error: [Conflict feature option] To enable AIR_ANC_USER_UNAWARE_ENABLE must support AIR_ANC_ENABLE";
	@set -e; exit 2;
endif
endif

ifeq ($(AIR_ANC_ENVIRONMENT_DETECTION_ENABLE), y)
ifneq ($(AIR_ANC_ENABLE),y)
	$(warning "Error: [Conflict feature option] To enable AIR_ANC_ENVIRONMENT_DETECTION_ENABLE must support AIR_ANC_ENABLE")
	@echo "Error: [Conflict feature option] To enable AIR_ANC_ENVIRONMENT_DETECTION_ENABLE must support AIR_ANC_ENABLE";
	@set -e; exit 2;
endif
endif

ifeq ($(AIR_ANC_WIND_NOISE_DETECTION_ENABLE), y)
ifneq ($(AIR_ANC_ENABLE),y)
	$(warning "Error: [Conflict feature option] To enable AIR_ANC_WIND_NOISE_DETECTION_ENABLE must support AIR_ANC_ENABLE")
	@echo "Error: [Conflict feature option] To enable AIR_ANC_WIND_NOISE_DETECTION_ENABLE must support AIR_ANC_ENABLE";
	@set -e; exit 2;
endif
endif

ifeq ($(AIR_ADAPTIVE_EQ_ENABLE), y)
ifneq ($(AIR_PEQ_ENABLE), y)
	$(warning "Error: [Conflict feature option] To enable AIR_ADAPTIVE_EQ_ENABLE must support AIR_PEQ_ENABLE")
	@echo "Error: [Conflict feature option] To enable AIR_ADAPTIVE_EQ_ENABLE must support AIR_PEQ_ENABLE";
	@set -e; exit 2;
endif
endif

ifeq ($(AIR_BT_FAST_PAIR_ENABLE),y)
ifeq ($(wildcard $(strip $(SOURCE_DIR))/middleware/airoha/bt_fast_pair/),)
	$(warning "Error: [Addon feature option fail] The fastpair addon package do not exist, please contact Airoha PLM to request the fastpair_addon_sla addon package.")
	@echo "[Addon feature option fail] The fastpair addon package do not exist, please contact Airoha PLM to request the fastpair_addon_sla addon package.";
	@set -e; exit 2;
endif
endif

$(LIBS): $(MODULE_PATH)

$(MODULE_PATH):
	@+make -C $@ $(MOD_EXTRA) $($@_EXTRA)

proj: $(OUTPATH)/$(PROJ_NAME).elf

$(OUTPATH)/$(PROJ_NAME).elf: $(C_OBJS) $(CXX_OBJS) $(S_OBJS) $(LIBS)
	@if [ "$(LSCRIPT)" = "$(NoLinkerScript)" ]; then \
		echo >&2 ; \
		echo "=========================================================================" >&2 ; \
		echo "In flash booting mode, the linker script must be specified in feature.mk." >&2 ; \
		echo "=========================================================================" >&2 ; \
		echo >&2 ; \
		exit 68; \
	fi
	@echo Linking...
	@cp $(LSCRIPT) $(OUTPATH)
	@cp $(FEATURE) $(OUTPATH)
	@if [ -e "$@" ]; then rm -f "$@"; fi
	@if [ -e "$(OUTPATH)/$(PROJ_NAME).map" ]; then rm -f "$(OUTPATH)/$(PROJ_NAME).map"; fi
	@if [ -e "$(OUTPATH)/$(PROJ_NAME).bin" ]; then rm -f "$(OUTPATH)/$(PROJ_NAME).bin"; fi
	@echo $(patsubst $(SDK_PATH)/%,$(SOURCE_DIR)/%, $(LDFLAGS) -Wl,--start-group -Wl,--whole-archive $^ -Wl,--no-whole-archive -Wl,--end-group -Wl,-Map=$(patsubst $(SDK_PATH)/%,$(SOURCE_DIR)/%,$(OUTPATH)/$(PROJ_NAME).map) -lm -o $@) > $(OUTPATH)/link_option.tmp
	@$(CC) @$(OUTPATH)/link_option.tmp 2>>$(ERR_LOG)
	@rm -f $(OUTPATH)/link_option.tmp
	@$(OBJCOPY) -O binary --only-section=.log_str $(OUTPATH)/$(PROJ_NAME).elf $(OUTPATH)/mcu_log_str.bin
	@$(OBJCOPY) -O binary --only-section=.partition_table $(OUTPATH)/$(PROJ_NAME).elf $(OUTPATH)/partition_table.bin
	@$(OBJCOPY) -O binary --remove-section=.log_str --remove-section=.partition_table $(OUTPATH)/$(PROJ_NAME).elf $(OUTPATH)/$(PROJ_NAME).bin
	@echo Done


ifneq ($(filter MINGW% MSYS%,$(OS_VERSION)),)
  POSTBUILD := tools/scripts/build/PostBuildTool/windows/nvdm_post_build.exe
  LM_BIN_GENERATOR := tools/scripts/build/lm_bin_generator/windows/gen_lm_tool.exe
else ifeq ($(OS_VERSION), Darwin)
  POSTBUILD := tools/scripts/build/PostBuildTool/mac/nvdm_post_build
  LM_BIN_GENERATOR := tools/scripts/build/lm_bin_generator/mac/gen_lm_tool
else
  POSTBUILD := tools/scripts/build/PostBuildTool/linux/nvdm_post_build
  LM_BIN_GENERATOR := tools/scripts/build/lm_bin_generator/linux/gen_lm_tool
endif

include $(SOURCE_DIR)/.rule.mk

clean:
	rm -rf $(OUTPATH)

.PHONY: options

options:
	@for opt in `sed 's/[()]/\n/g' $(SOURCE_DIR)/config/chip/$(IC_CONFIG)/chip.mk|sed 's/-D/ /'|sort|uniq|grep ^MTK_|grep _ENABLE$$`; do echo $$opt; done
print-%  : ; @echo $* = $($*)
