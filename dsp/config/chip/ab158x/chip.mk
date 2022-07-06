XTENSA_CORE               = Hifi5_MT2833_FDI_FINAL
MIDDLEWARE_PROPRIETARY    = middleware/airoha
MTK_SYSLOG_VERSION_2     ?= y
MTK_SYSLOG_SUB_FEATURE_STRING_LOG_SUPPORT = y
MTK_SYSLOG_SUB_FEATURE_BINARY_LOG_SUPPORT = y
MTK_SYSLOG_SUB_FEATURE_MSGID_TO_STRING_LOG_SUPPORT = n
MTK_SYSLOG_SUB_FEATURE_USB_ACTIVE_MODE = y
MTK_SYSLOG_SUB_FEATURE_OFFLINE_DUMP_ACTIVE_MODE = y
MTK_DEBUG_PLAIN_LOG_ENABLE            = n
MTK_SAVE_LOG_AND_CONTEXT_DUMP_ENABLE  ?= n
MTK_CPU_NUMBER_1                      ?= y
MTK_MUX_ENABLE                        ?= y
FPGA_ENV						      ?= n
IOT_SDK_XTENSA_VERSION                 := 9018
AIR_BT_A2DP_ENABLE ?= y
#MTK_BT_A2DP_AAC_ENABLE ?= y
AIR_BT_A2DP_SBC_ENABLE ?= y
AIR_BT_HFP_MSBC_ENABLE ?= y

ifeq ($(AIR_ECNR_CONFIG_TYPE),ECNR_1_OR_2_MIC)
AIR_VOICE_NR_ENABLE        	:= y
AIR_VOICE_NR_USE_PIC_ENABLE	:= y
AIR_ECNR_1_OR_2_MIC_ENABLE 	:= y
endif

ifeq ($(AIR_ECNR_CONFIG_TYPE),ECNR_1MIC_INEAR)
AIR_VOICE_NR_ENABLE        	:= y
AIR_VOICE_NR_USE_PIC_ENABLE	:= y
AIR_ECNR_1MIC_INEAR_ENABLE 	:= y
endif

ifeq ($(AIR_ECNR_CONFIG_TYPE),ECNR_2MIC_INEAR)
AIR_VOICE_NR_ENABLE        	:= y
AIR_VOICE_NR_USE_PIC_ENABLE	:= y
AIR_ECNR_2MIC_INEAR_ENABLE 	:= y
endif

ifeq ($(AIR_ECNR_CONFIG_TYPE),3RD_PARTY_AI_NR)
AIR_VOICE_NR_ENABLE        	:= y
AIR_VOICE_NR_USE_PIC_ENABLE	:= y
AIR_3RD_PARTY_NR_ENABLE		:= y
ifneq ($(wildcard $(strip $(ROOTDIR))/prebuilt/middleware/third_party/dspalg/igo_nr/),)
AIR_AI_NR_PREMIUM_ENABLE 	:= y
endif
endif

ifeq ($(AIR_ECNR_CONFIG_TYPE),3RD_PARTY_AI_NR_INEAR)
AIR_VOICE_NR_ENABLE        	:= y
AIR_VOICE_NR_USE_PIC_ENABLE	:= y
AIR_3RD_PARTY_NR_ENABLE 	:= y
ifneq ($(wildcard $(strip $(ROOTDIR))/prebuilt/middleware/third_party/dspalg/igo_nr/),)
AIR_AI_NR_PREMIUM_INEAR_ENABLE := y
AIR_AI_NR_PREMIUM_INEAR_270K_VARIANT_1_ENABLE := y
endif
endif

ifeq ($(AIR_ECNR_CONFIG_TYPE),3RD_PARTY_CUSTOMIZED)
AIR_VOICE_NR_ENABLE        	:= y
AIR_VOICE_NR_USE_PIC_ENABLE	:= y
AIR_3RD_PARTY_NR_ENABLE 	:= y
endif

AIR_AUDIO_HARDWARE_ENABLE ?= y
AIR_AUDIO_SUPPORT_MULTIPLE_MICROPHONE_ENABLE ?= y
AIR_UPLINK_RATE                       ?= none
AIR_UPLINK_RESOLUTION                 ?= none
AIR_DOWNLINK_RATE                     ?= none
# code run in S or NS world #s or ns
# This feature must be set for both DSP and MCU, otherwise, it will not work.
AIR_CPU_SECURITY_MODE                 ?= s

CCFLAG += -DAIR_BTA_IC_PREMIUM_G3
AIR_BTA_IC_PREMIUM_G3 = y

ifeq ($(IC_TYPE),ab1588)
CCFLAG += -DAIR_BTA_IC_PREMIUM_G3_TYPE_S
endif
ifeq ($(IC_TYPE),ab1585)
CCFLAG += -DAIR_BTA_IC_PREMIUM_G3_TYPE_P
endif
PRODUCT_VERSION           = 0
CCFLAG += -DPRODUCT_VERSION=$(PRODUCT_VERSION)
CCFLAG += -DCORE_DSP0
ASFLAG += -DCORE_DSP0
CCFLAG += -D$(TARGET)_BOOTING
ifeq ($(FPGA_ENV),y)
CCFLAG += -DFPGA_ENV
endif

# Link and Compile Configuration

#CCFLAG      :=  
CCFLAG      +=  -g
CCFLAG      +=  -W
CCFLAG      +=  -Wall
CCFLAG      +=  -Os
CCFLAG      +=  -INLINE:requested
CCFLAG      +=  -mlongcalls
ifeq ($(IOT_SDK_XTENSA_VERSION),9018)
CCFLAG      +=  -std=c11
CCFLAG      +=  -fgnu-keywords
else ifeq ($(IOT_SDK_XTENSA_VERSION),8013)
CCFLAG      +=  -std=gnu99
endif
CCFLAG      +=  -ffunction-sections -fdata-sections -mtext-section-literals
CCFLAG      +=  -OPT:space_flix=1

#ASFLAG      :=  
ASFLAG      +=  -W
ASFLAG      +=  -Wall
ASFLAG      +=  -g
ASFLAG      +=  -mlongcalls --text-section-literals

#LDFLAG      :=  
LDFLAG      +=  --gc-sections
LDFLAG      +=  --no-relax
LDFLAG      +=  -wrap=printf
LDFLAG      +=  -wrap=puts
LDFLAG	    += -u _UserExceptionVector
LDFLAG	    += -u _KernelExceptionVector
LDFLAG	    += -u _DoubleExceptionVector

##
## MTK_DEBUG_PLAIN_LOG_ENABLE
## Brief:       This option is used to force log display with plain style.
## Usage:       Enable the feature by configuring it as y.
##              y : log display with plain style
##              n : log display with race style, need pc logging tool support
## Path:        kernel/service/syslog
## Dependency:  None
## Notice:      None
## Related doc :None
##
ifeq ($(MTK_DEBUG_PLAIN_LOG_ENABLE),y)
CCFLAG += -DMTK_DEBUG_PLAIN_LOG_ENABLE
endif

##
## AIR_DEBUG_LEVEL
## Brief:       This option is to configure system log debug level.
## Usage:       The valid values are empty, error, warning, info, debug, and none.
##              The setting will determine whether a debug log will be compiled.
##              However, the setting has no effect on the prebuilt library.
##              empty   : All debug logs are compiled.
##              error   : Only error logs are compiled.
##              warning : Only warning and error logs are compiled.
##              info    : Only info, warning, and error logs are compiled.
##              debug   : All debug logs are compiled.
##              none    : All debugs are disabled.
## Path:        kernel/service
## Dependency:  None
## Notice:      None
## Realted doc: Please refer to doc/LinkIt_for_RTOS_System_Log_Developers_Guide.pdf
##
AIR_DEBUG_LEVEL ?= $(MTK_DEBUG_LEVEL)

ifeq ($(AIR_DEBUG_LEVEL),)
CCFLAG += -DMTK_DEBUG_LEVEL_DEBUG
CCFLAG += -DMTK_DEBUG_LEVEL_INFO
CCFLAG += -DMTK_DEBUG_LEVEL_WARNING
CCFLAG += -DMTK_DEBUG_LEVEL_ERROR
endif

ifeq ($(AIR_DEBUG_LEVEL),error)
CCFLAG += -DMTK_DEBUG_LEVEL_ERROR
endif

ifeq ($(AIR_DEBUG_LEVEL),warning)
CCFLAG += -DMTK_DEBUG_LEVEL_WARNING
CCFLAG += -DMTK_DEBUG_LEVEL_ERROR
endif

ifeq ($(AIR_DEBUG_LEVEL),info)
CCFLAG += -DMTK_DEBUG_LEVEL_INFO
CCFLAG += -DMTK_DEBUG_LEVEL_WARNING
CCFLAG += -DMTK_DEBUG_LEVEL_ERROR
endif

ifeq ($(AIR_DEBUG_LEVEL),debug)
CCFLAG += -DMTK_DEBUG_LEVEL_DEBUG
CCFLAG += -DMTK_DEBUG_LEVEL_INFO
CCFLAG += -DMTK_DEBUG_LEVEL_WARNING
CCFLAG += -DMTK_DEBUG_LEVEL_ERROR
endif

ifeq ($(AIR_DEBUG_LEVEL),printf)
CCFLAG += -DMTK_DEBUG_LEVEL_PRINTF
endif
ifeq ($(AIR_DEBUG_LEVEL),none)
CCFLAG += -DMTK_DEBUG_LEVEL_NONE
endif

##
## MTK_SAVE_LOG_AND_CONTEXT_DUMP_ENABLE
## Brief:       This option is to enable runtime log and crash context save in flash feature.
## Usage:       Enable the feature by configuring it as y.
##              y : save runtime logging registers and all memory in coredump format
##              n   : no effect
## Path:        kernel/service/src_core
## Dependency:  flash driver
## Notice:      Reserve flash blocks to store runtime log and dumped data
## Related doc :None
##
ifeq ($(MTK_SAVE_LOG_AND_CONTEXT_DUMP_ENABLE),y)
CCFLAG += -DMTK_SAVE_LOG_AND_CONTEXT_DUMP_ENABLE
endif
##

##
## MTK_USB_DEMO_ENABLED
## Brief:       This option is to enable USB device feature.
## Usage:       Enable the feature by configuring it as y.
## Path:        middleware/airoha/usb/
## Dependency:  None
## Notice:      None
## Related doc :None
##
ifeq ($(MTK_USB_DEMO_ENABLED),y)
CCFLAG += -DMTK_USB_DEMO_ENABLED
endif

##
## MTK_PROMPT_SOUND_ENABLE
## Brief:       This option is to enable prompt sound feature.
## Usage:       Enable enable prompt sound feature by configuring it as y.
##              y : enable prompt sound feature.
##              n : not enable prompt sound feature.
## Path:        middleware\airoha\stream
## Dependency:  None.
## Notice:      None.
## Realted doc: None.
##
ifeq ($(MTK_PROMPT_SOUND_ENABLE),y)
CCFLAG += -DMTK_PROMPT_SOUND_ENABLE
endif

##
## MTK_I2S_SLAVE_ENABLE
## Brief:       This option is to open i2s slave driver.
## Usage:       Enable the i2s slave driver by configuring it as y.
##              y : open i2s slave driver.
##              n : not open i2s slave driver.
## Path:        middleware/airoha/dspfw/port/chip/mt2822/src/dsp_lower_layer/dsp_drv
## Dependency:  None
## Notice:      None
## Realted doc: None
##
ifeq ($(MTK_I2S_SLAVE_ENABLE),y)
CCFLAG += -DMTK_I2S_SLAVE_ENABLE
endif




##
## DSP_MEMORY_MANAGEMENT_EBANLE
## Brief:       This option is to open dsp memory management feature.
## Usage:       Enable the dsp memory management by configuring it as y.
##              y : open dsp memory management feature.
##              n : not open dsp memory management feature.
## Path:        kernel/service/DSP_Memory_Management
## Dependency:  None
## Notice:      None
## Realted doc: None
##
ifeq ($(DSP_MEMORY_MANAGEMENT_EBANLE),y)
CCFLAG += -DDSP_MEMORY_MANAGEMENT_EBANLE
CCFLAG += -DAIR_DSP_MEMORY_REGION_ENABLE
endif



##
## PRELOADER_ENABLE
## Brief:       This option is to enable and disable preload pisplit features(dynamic to load PIC libraries)
## Usage:       If the value is "y", the PRELOADER_ENABLE compile option will be defined. You must also include the kernel/service/pre_libloader/dsp0/module.mk in your Makefile before setting the option to "y".
## Path:        kernel/service/pre_libloader
## Dependency:  None
## Notice:      None
## Relative doc:None
##
ifeq ($(PRELOADER_ENABLE),y)
CCFLAG += -DPRELOADER_ENABLE
ASFLAG += -DPRELOADER_ENABLE

##
## DSP0_PISPLIT_DEMO_LIBRARY
## Brief:       This option is to enable and disable the demo of DSP0 PIC library
## Usage:       If the value is "y", the DSP0_PISPLIT_DEMO_LIBRARY compile option will be defined. This is a sub-feature option of PRELOADER_ENABLE.
## Path:        kernel/service/pre_libloader/dsp0/dsp0_pic_demo_portable
## Dependency:  PRELOADER_ENABLE
## Notice:      None
## Relative doc:None
##
ifeq ($(DSP0_PISPLIT_DEMO_LIBRARY),y)
CCFLAG += -DDSP0_PISPLIT_DEMO_LIBRARY
endif
endif

##
## CCCI_ENABLE
## Brief:       This option is to enable and disable CCCI(Cross Core communication Interface)
## Usage:       If the value is "y", the CCCI_ENABLE compile option will be defined. 
## Path:        kernel/service/ccci
## Dependency:  None
## Notice:      None
## Relative doc:None
##
ifeq ($(CCCI_ENABLE),y)
CCFLAG += -DCCCI_ENABLE
endif


##
## LINE_IN_PURE_FOR_AMIC_CLASS_G_HQA
## Brief:       This option is to enable for Audio HQA verification.
## Usage:       If the value is "y",  the LINE_IN_PURE_FOR_AMIC_CLASS_G_HQA option will be defined.
## Path:        middleware/airoha/audio/
## Dependency:  None
## Notice:      None
## Relative doc:None
##
ifeq ($(LINE_IN_PURE_FOR_AMIC_CLASS_G_HQA),y)
CCFLAG += -DLINE_IN_PURE_FOR_AMIC_CLASS_G_HQA
endif

##
## ANALOG_OUTPUT_CLASSD_ENABLE
## Brief:       This option is to enable for default setting to class-d.
## Usage:       If the value is "y",  the ANALOG_OUTPUT_CLASSD_ENABLE option will be defined.
## Path:        middleware/airoha/audio/
## Dependency:  None
## Notice:      None
## Relative doc:None
##
ifeq ($(ANALOG_OUTPUT_CLASSD_ENABLE),y)
CCFLAG += -DANALOG_OUTPUT_CLASSD_ENABLE
endif

##
## AIR_VOICE_DRC_ENABLE
## Brief:       This option is to enable voice drc.
## Usage:       If the value is "y",  the AIR_VOICE_DRC_ENABLE option will be defined.
## Path:        middleware/airoha/dspfw/
## Dependency:  None
## Notice:      None
## Relative doc:None
##
ifeq ($(AIR_VOICE_DRC_ENABLE),y)
AIR_BT_A2DP_CPD_USE_PIC_ENABLE 	:= y
CCFLAG += -DMTK_BT_A2DP_CPD_USE_PIC
CCFLAG += -DAIR_VOICE_DRC_ENABLE
endif

##
## AIR_ANC_ENABLE
## Brief:       This option is to enable Active Noise Cancellation (ANC) main function.
## Usage:       Enable ANC feature by configuring it as y.
##              y : enable ANC feature.
##              n : not enable ANC feature.
## Path:        dsp/prebuilt/middleware/airoha/dspfw/anc/$(IC_TYPE)
## Dependency:  None.
## Notice:      None.
## Realted doc: None.
##
ifeq ($(AIR_ANC_ENABLE),y)
CCFLAG += -DMTK_ANC_ENABLE
CCFLAG += -DMTK_ANC_V2
CCFLAG += -DHAL_AUDIO_ANC_ENABLE
ifeq ($(AIR_BTA_IC_PREMIUM_G3),y)
CCFLAG += -DAIR_ANC_V3
endif
endif

##
## AIR_FULL_ADAPTIVE_ANC_ENABLE
## Brief:       This option is to enable full adaptive Active Noise Cancellation (ANC) main function.
## Usage:       Enable full adaptive ANC feature by configuring it as y.
##              y : enable full adaptive ANC feature.
##              n : not enable full adaptive ANC feature.
## Path:        dsp/prebuilt/middleware/airoha/dspalg/full_adaptive_anc/$(IC_TYPE)
## Dependency:  None.
## Notice:      None.
## Realted doc: None.
##
ifeq ($(AIR_FULL_ADAPTIVE_ANC_ENABLE),y)
CCFLAG += -DAIR_FULL_ADAPTIVE_ANC_ENABLE
CCFLAG += -DMTK_AUDIO_DUMP_SPDIF_TRUNCATE_24BIT
endif

##
## AIR_UPLINK_RATE
## Brief:       This option is to choose the uplink rate.
## Usage:       The valid values are none, 48k.
##              The setting will determine which rate of uplink rate will be used.
##              none    : uplink rate will be handled by scenario itself.
##              48k     : uplink rate will be fixed in 48k Hz.
## Path:        dsp/driver/chip/$(IC_TYPE)
## Dependency:  None
## Notice:      None
##
ifeq ($(AIR_UPLINK_RATE),48k)
CCFLAG += -DAIR_UL_FIX_SAMPLING_RATE_48K
CCFLAG += -DAIR_FIXED_RATIO_SRC
endif

##
## AIR_UPLINK_RESOLUTION
## Brief:       This option is to fix the uplink DMA resolution.
## Usage:       The valid values are none, 32bit.
##              The setting will determine which resolution of uplink DMA will be used.
##              none    : uplink resolution will be handled by scenario itself.
##              32bit   : uplink resolution will be fixed at 32-bit.
## Path:        dsp/driver/chip/$(IC_TYPE)
## Dependency:  None
## Notice:      None
##
ifeq ($(AIR_UPLINK_RESOLUTION),32bit)
CCFLAG += -DAIR_UL_FIX_RESOLUTION_32BIT
endif

##
## AIR_DOWNLINK_RATE
## Brief:       This option is to choose the downlink rate.
## Usage:       The valid values are none, 48k, 96k.
##              The setting will determine which rate of downlink rate will be used.
##              none    : downlink rate will be handled by scenario itself.
##              48k     : downlink rate will be fixed in 48k Hz.
##              96k     : downlink rate will be fixed in 96k Hz.
## Path:        dsp/driver/chip/$(IC_TYPE)
## Dependency:  None
## Notice:      None
##
ifeq ($(AIR_DOWNLINK_RATE),48k)
CCFLAG += -DFIXED_SAMPLING_RATE_TO_48KHZ
endif

ifeq ($(AIR_DOWNLINK_RATE),96k)
CCFLAG += -DAIR_FIXED_DL_SAMPLING_RATE_TO_96KHZ
endif

##
## AIR_PEQ_ENABLE
## Brief:       This option is to enable PEQ feature.
## Usage:       If the value is "y",  the AIR_PEQ_ENABLE option will be defined.
## Path:        middleware/airoha/dspalg/
## Dependency:  AIR_BT_PEQ_USE_PIC_ENABLE
## Notice:      None
## Relative doc:None
##
AIR_PEQ_ENABLE ?= $(MTK_PEQ_ENABLE)
ifeq ($(AIR_PEQ_ENABLE),y)
AIR_BT_PEQ_USE_PIC_ENABLE := y
CCFLAG += -DMTK_PEQ_ENABLE
CCFLAG += -DMTK_BT_PEQ_USE_PIC
endif

##
## AIR_ADAPTIVE_EQ_ENABLE
## Brief:       This option is to enable adaptive eq feature.
## Usage:       If the value is "y",  the AIR_ADAPTIVE_EQ_ENABLE option will be defined.
## Path:        middleware/airoha/dspalg/
## Dependency:  AIR_BT_PEQ_USE_PIC_ENABLE
## Notice:      None
## Relative doc:None
##
ifeq ($(AIR_ADAPTIVE_EQ_ENABLE),y)
CCFLAG += -DAIR_ADAPTIVE_EQ_ENABLE
endif

##
## AIR_DRC_ENABLE
## Brief:       This option is to enable DRC feature.
## Usage:       If the value is "y",  the AIR_DRC_ENABLE option will be defined.
## Path:        middleware/airoha/dspalg/
## Dependency:  None
## Notice:      None
## Relative doc:None
##
ifeq ($(AIR_DRC_ENABLE),y)
AIR_BT_A2DP_CPD_USE_PIC_ENABLE 	:= y
CCFLAG += -DMTK_BT_A2DP_CPD_USE_PIC
CCFLAG += -DAIR_DRC_ENABLE
endif

##
## AIR_LINE_IN_PEQ_ENABLE
## Brief:       This option is to enable LINE_IN PEQ feature.
## Usage:       If the value is "y",  the AIR_LINE_IN_PEQ_ENABLE option will be defined.
## Path:        middleware/airoha/dspalg/
## Dependency:  AIR_BT_PEQ_USE_PIC_ENABLE
## Notice:      None
## Relative doc:None
##
AIR_LINE_IN_PEQ_ENABLE ?= $(MTK_LINEIN_PEQ_ENABLE)
ifeq ($(AIR_LINE_IN_PEQ_ENABLE),y)
AIR_BT_PEQ_USE_PIC_ENABLE := y
CCFLAG += -DMTK_LINEIN_PEQ_ENABLE
CCFLAG += -DMTK_BT_PEQ_USE_PIC
endif

##
## AIR_LINE_IN_INS_ENABLE
## Brief:       This option is to enable LINE_IN INS feature.
## Usage:       If the value is "y",  the AIR_LINE_IN_INS_ENABLE option will be defined.
## Path:        middleware/airoha/dspalg/
## Dependency:  None
## Notice:      None
## Relative doc:None
##
AIR_LINE_IN_INS_ENABLE ?= $(MTK_LINEIN_INS_ENABLE)
ifeq ($(AIR_LINE_IN_INS_ENABLE),y)
CCFLAG += -DMTK_LINEIN_INS_ENABLE
endif

##
## AIR_LINE_IN_MIX_ENABLE
## Brief:       Internal use.
## Notice:      AIR_LINE_IN_MIX_ENABLE is a option to support line-in audio mix with A2DP/HFP.
##
ifeq ($(AIR_LINE_IN_MIX_ENABLE),y)
CCFLAG += -DAIR_LINE_IN_MIX_ENABLE
AIR_AUDIO_TRANSMITTER_ENABLE = y
AIR_WIRED_AUDIO_ENABLE = y
AIR_SOFTWARE_GAIN_ENABLE = y
AIR_SOFTWARE_BUFFER_ENABLE = y
AIR_SOFTWARE_MIXER_ENABLE = y
AIR_SOFTWARE_SRC_ENABLE = y
endif

##
## AIR_USB_AUDIO_IN_MIX_ENABLE
## Brief:       Internal use.
## Notice:      AIR_USB_AUDIO_IN_MIX_ENABLE is a option to support usb-in audio mix with A2DP/HFP.
##
ifeq ($(AIR_USB_AUDIO_IN_MIX_ENABLE),y)
CCFLAG += -DAIR_USB_AUDIO_IN_MIX_ENABLE
AIR_AUDIO_TRANSMITTER_ENABLE = y
AIR_WIRED_AUDIO_ENABLE = y
AIR_SOFTWARE_GAIN_ENABLE = y
AIR_SOFTWARE_BUFFER_ENABLE = y
AIR_SOFTWARE_MIXER_ENABLE = y
AIR_SOFTWARE_SRC_ENABLE = y
endif

##
## AIR_WIRED_AUDIO_ENABLE
## Brief:       Internal use.
## Notice:      AIR_WIRED_AUDIO_ENABLE is a option to enable wired audio,inlucde line-in/line-out and usb-in/usb-out.
##
ifeq ($(AIR_WIRED_AUDIO_ENABLE),y)
CCFLAG += -DAIR_WIRED_AUDIO_ENABLE
endif
###############################################################################
##
## The following makefile options are not configurable or only for internal user. They may be removed in the future. 

##
## MTK_CPU_NUMBER_1
## Brief:       Internal use.
##
ifeq ($(MTK_CPU_NUMBER_1),y)
CCFLAG += -DMTK_CPU_NUMBER_1
CCFLAG += -DMTK_MAX_CPU_NUMBER_2
endif


##
## AIR_AMA_HOTWORD_ENABLE
## Brief:       Internal use.
## Notice:      AIR_AMA_HOTWORD_ENABLE is a option to support AMA HOTWORD function.
##
ifeq ($(AIR_AMA_HOTWORD_ENABLE),y)
CCFLAG += -DMTK_WWE_ENABLE
CCFLAG += -DMTK_WWE_AMA_ENABLE
ifeq ($(AIR_AMA_HOTWORD_USE_PIC_ENABLE),y)
CCFLAG += -DMTK_WWE_USE_PIC
CCFLAG += -DMTK_WWE_AMA_USE_PIC
endif
endif

##
## AIR_GSOUND_HOTWORD_ENABLE
## Brief:       Internal use.
## Notice:      MTK_WWE_GSOUND_ENABLE is a option to support Gsound WWE function.
##
ifeq ($(AIR_GSOUND_HOTWORD_ENABLE),y)
CCFLAG += -DMTK_WWE_ENABLE
CCFLAG += -DMTK_WWE_GSOUND_ENABLE
ifeq ($(AIR_GSOUND_HOTWORD_ENABLE),y)
CCFLAG += -DMTK_WWE_USE_PIC
CCFLAG += -DMTK_WWE_GSOUND_USE_PIC
endif
endif

##
## MTK_SENSOR_SOURCE_ENABLE
## Brief:       Internal use.
## Notice:      MTK_SENSOR_SOURCE_ENABLE is a option to support Sensor Source.
##
ifeq ($(MTK_SENSOR_SOURCE_ENABLE),y)
CCFLAG += -DMTK_SENSOR_SOURCE_ENABLE
endif

##
## MTK_SUPPORT_HEAP_DEBUG
## Brief:       Internal use.
## MTK_SUPPORT_HEAP_DEBUG is a option to show heap status (alocatted or free),
## It's for RD internal development and debug. Default should be disabled.
##
ifeq ($(MTK_SUPPORT_HEAP_DEBUG),y)
CCFLAG += -DMTK_SUPPORT_HEAP_DEBUG
endif

##
## MTK_HEAP_SIZE_GUARD_ENABLE
## Brief:       Internal use.
## MTK_HEAP_SIZE_GUARD_ENABLE is a option to profiling heap usage,
## It's for RD internal development and debug. Default should be disabled.
##
ifeq ($(MTK_HEAP_SIZE_GUARD_ENABLE),y)
LDFLAG  += -wrap=pvPortMalloc -wrap=vPortFree
CCFLAG  += -DMTK_HEAP_SIZE_GUARD_ENABLE
endif

##
## MTK_SWLA_ENABLE
## Brief:       Internal use. This option is to enable and disable the Software Logical Analyzer service, Each event(task/isr activity) is recorded while CPU context switching, also support customization tag
## Usage:       If the value is "y", the MTK_SWLA_ENABLE compile option will be defined. You must also include the gva\kernel\service\module.mk in your Makefile before setting the option to "y".
## Path:        kernel/service
## Dependency:  None
## Notice:      None
## Relative doc:None
##
ifeq ($(MTK_SWLA_ENABLE),y)
CCFLAG += -DMTK_SWLA_ENABLE
CCFLAG += -DPRODUCT_VERSION_STR=\"$(PRODUCT_VERSION)\"
endif
##

##
## AIR_AUDIO_SUPPORT_MULTIPLE_MICROPHONE_ENABLE
## Brief:       Internal use.
## Notice:      AIR_AUDIO_SUPPORT_MULTIPLE_MICROPHONE_ENABLE is a option to to support multiple microphone.
##
ifeq ($(AIR_AUDIO_SUPPORT_MULTIPLE_MICROPHONE_ENABLE),y)
CCFLAG += -DHAL_AUDIO_SUPPORT_MULTIPLE_MICROPHONE
CCFLAG += -DAIR_AUDIO_SUPPORT_MULTIPLE_MICROPHONE
endif

##
## AIR_FIXED_RATIO_SRC
## Brief:       Internal use. This option is to enable fixed ratio SRC.
## Usage:       If the value is "y",  the AIR_FIXED_RATIO_SRC option will be defined.
## Path:        driver/chip/
## Dependency:  None
## Notice:      None
## Relative doc:None
##
ifeq ($(AIR_FIXED_RATIO_SRC),y)
CCFLAG += -DAIR_FIXED_RATIO_SRC
ifeq ($(AIR_FIXED_RATIO_SRC_USE_PIC),y)
CCFLAG += -DAIR_FIXED_RATIO_SRC_USE_PIC
endif
endif

##
## AIR_AUDIO_DUMP_ENABLE
## Brief:       Internal use.
## Notice:      AIR_AUDIO_DUMP_ENABLE is a option to choose that whether the dump path will be selected by Config Tool(y) or by Coding(n). Default should be Config Tool(y).
##
ifeq ($(AIR_AUDIO_DUMP_ENABLE),y)
CCFLAG += -DAIR_AUDIO_DUMP_ENABLE
endif

##
## MTK_BT_A2DP_MSBC_USE_PIC
## Brief:       Internal use.
## Notice:      MTK_BT_A2DP_MSBC_USE_PIC is a option to use mSBC PIC. Default should be enabled.
##
ifeq ($(MTK_BT_A2DP_MSBC_USE_PIC),y)
CCFLAG += -DMTK_BT_A2DP_MSBC_USE_PIC
endif

##
## MTK_BT_A2DP_CVSD_USE_PIC
## Brief:       Internal use.
## Notice:      MTK_BT_A2DP_CVSD_USE_PIC is a option to use CVSD PIC. Default should be enabled.
##
ifeq ($(MTK_BT_A2DP_CVSD_USE_PIC),y)
CCFLAG += -DMTK_BT_A2DP_CVSD_USE_PIC
endif

##
## MTK_BT_CLK_SKEW_USE_PIC
## Brief:       Internal use.
## Notice:      MTK_BT_CLK_SKEW_USE_PIC is a option to use clock skew PIC. Default should be enabled.
##
ifeq ($(MTK_BT_CLK_SKEW_USE_PIC),y)
CCFLAG += -DMTK_BT_CLK_SKEW_USE_PIC
endif

##
## MTK_VOICE_AGC_ENABLE
## Brief:       Internal use.
## Notice:      MTK_VOICE_AGC_ENABLE is a option to enable Voice AGC. Default should be enabled.
##
AIR_VOICE_AGC_ENABLE ?= $(MTK_VOICE_AGC_ENABLE)
ifeq ($(AIR_VOICE_AGC_ENABLE),y)
CCFLAG += -DMTK_VOICE_AGC_ENABLE
endif


##
## MTK_BT_AGC_USE_PIC
## Brief:       Internal use.
## Notice:      MTK_BT_AGC_USE_PIC is a option to use AGC PIC. If MTK_VOICE_AGC_ENABLE is enabled, this compile option will be enabled.
##
ifeq ($(MTK_BT_AGC_USE_PIC),y)
CCFLAG += -DMTK_BT_AGC_USE_PIC
endif



##
## MTK_PLC_USE_PIC
## Brief:       Internal use.
## Notice:      MTK_PLC_USE_PIC is a option to use Packet Lost Compensation(PLC) PIC. Default should be enabled.
##
ifeq ($(MTK_PLC_USE_PIC),y)
CCFLAG += -DMTK_PLC_USE_PIC
endif

##
## MTK_BT_HFP_SPE_ALG_V2
## Brief:       Internal use.
## Notice:      MTK_BT_HFP_SPE_ALG_V2 is a option for mt2822 which use different algorithm interface (CPD, clk skew). Default should be enabled.
##
ifeq ($(MTK_BT_HFP_SPE_ALG_V2),y)
CCFLAG += -DMTK_BT_HFP_SPE_ALG_V2
endif

##
## AIR_HWSRC_ON_MAIN_STREAM_ENABLE
## Brief:       Internal use.
## Notice:      AIR_HWSRC_ON_MAIN_STREAM_ENABLE is a option support HWSRC dor DL1.
##
AIR_HWSRC_ON_MAIN_STREAM_ENABLE ?= $(ENABLE_HWSRC_ON_MAIN_STREAM)
ifeq ($(AIR_HWSRC_ON_MAIN_STREAM_ENABLE),y)
CCFLAG += -DENABLE_HWSRC_ON_MAIN_STREAM
CCFLAG += -DENABLE_HWSRC_CLKSKEW
endif

##
## AIR_HWSRC_IN_STREAM_ENABLE
## Brief:       Internal use.
## Notice:      AIR_HWSRC_IN_STREAM_ENABLE is a option for speaker project hwsrc clk skew. Default should not be enabled.
##
AIR_HWSRC_IN_STREAM_ENABLE ?= $(MTK_HWSRC_IN_STREAM)
ifeq ($(AIR_HWSRC_IN_STREAM_ENABLE),y)
CCFLAG += -DMTK_HWSRC_IN_STREAM
CCFLAG += -DENABLE_HWSRC_CLKSKEW
endif

##
## AIR_BT_HFP_ENABLE
## Brief:       Internal use.
## Notice:      AIR_BT_HFP_ENABLE is a option for HFP feature. Default should be enabled.
##
ifeq ($(AIR_BT_HFP_ENABLE),y)
CCFLAG += -DAIR_BT_HFP_ENABLE
endif

##
## MTK_BT_HFP_FORWARDER_ENABLE
## Brief:       Internal use.
## Notice:      MTK_BT_HFP_FORWARDER_ENABLE is a option for mt2822 HFP which have audio forwarder. Default should be enabled.
##
ifeq ($(MTK_BT_HFP_FORWARDER_ENABLE),y)
CCFLAG += -DMTK_BT_HFP_FORWARDER_ENABLE
endif

##
## AIR_LE_AUDIO_ENABLE
## Brief:       Internal use.
## Notice:      AIR_LE_AUDIO_ENABLE is a option for BLE feature. Default should be enabled.
##
ifeq ($(AIR_LE_AUDIO_ENABLE),y)
CCFLAG += -DAIR_LE_AUDIO_ENABLE
endif

##
## AIR_LE_AUDIO_DONGLE_ENABLE
## Brief:       Internal use.
## Notice:      AIR_LE_AUDIO_DONGLE_ENABLE is a option for BLE dongle feature. Default should be enabled.
##
ifeq ($(AIR_LE_AUDIO_DONGLE_ENABLE),y)
CCFLAG += -DAIR_LE_AUDIO_DONGLE_ENABLE
CCFLAG += -DAIR_BLE_AUDIO_DONGLE_ENABLE
endif

##
## AIR_BT_CODEC_BLE_ENABLED
## Brief:       Internal use.
## Notice:      AIR_BT_CODEC_BLE_ENABLED is a option for BLE feature. Default should be enabled.
##
ifeq ($(AIR_BT_CODEC_BLE_ENABLED),y)
CCFLAG += -DAIR_BT_CODEC_BLE_ENABLED
endif



##
## AIR_BT_LE_LC3_ENABLE
## Brief:       Internal use.
## Notice:      AIR_BT_LE_LC3_ENABLE is a option for LC3 feature. Default should be enabled.
##
ifeq ($(AIR_BT_LE_LC3_ENABLE),y)
CCFLAG += -DAIR_BT_LE_LC3_ENABLE
endif

##
## AIR_BT_CODEC_BLE_V2_ENABLED
## Brief:       Internal use.
## Notice:      AIR_BT_CODEC_BLE_V2_ENABLED is a option for BLE feature. Default should be enabled.
##
ifeq ($(AIR_BT_CODEC_BLE_V2_ENABLED),y)
CCFLAG += -DAIR_BT_CODEC_BLE_V2_ENABLED
endif

##
## AIR_BT_A2DP_LC3_USE_PIC
## Brief:       Internal use.
## Notice:      AIR_BT_A2DP_LC3_USE_PIC is a option to use LC3 PIC. Default should be enabled.
##
ifeq ($(MTK_BT_A2DP_LC3_USE_PIC),y)
CCFLAG += -DMTK_BT_A2DP_LC3_USE_PIC
endif

##
## AIR_BT_A2DP_LC3_USE_LIGHT_PIC
## Brief:       Internal use.
## Notice:      AIR_BT_A2DP_LC3_USE_LIGHT_PIC is a option to use LC3 light PIC. Default should be enabled.
##
ifeq ($(MTK_BT_A2DP_LC3_USE_LIGHT_PIC),y)
CCFLAG += -DMTK_BT_A2DP_LC3_USE_LIGHT_PIC
endif

##
## AIR_BT_BLE_SWB_ENABLE
## Brief:       Internal use.
## Notice:      AIR_BT_BLE_SWB_ENABLE is a option to use SWB SPE. Default should be enabled.
##
ifeq ($(AIR_BT_BLE_SWB_ENABLE),y)
CCFLAG += -DAIR_BT_BLE_SWB_ENABLE
endif

##
## MTK_DSP_AUDIO_MESSAGE_ENABLE
## Brief:       Internal use.
## Notice:      MTK_DSP_AUDIO_MESSAGE_ENABLE is a option to enable audio message ISR handler. Default must be enabled.
##
ifeq ($(MTK_DSP_AUDIO_MESSAGE_ENABLE),y)
CCFLAG += -DMTK_DSP_AUDIO_MESSAGE_ENABLE
endif

##
## AIR_USB_AUDIO_IN_ENABLE
## Brief:       Internal use.
## Notice:      AIR_USB_AUDIO_IN_ENABLE is a option to support usb-in audio.
##
ifeq ($(AIR_USB_AUDIO_IN_ENABLE),y)
CCFLAG += -DAIR_USB_AUDIO_IN_ENABLE
MTK_CM4_PLAYBACK_ENABLE = y
endif
##
## MTK_CM4_PLAYBACK_ENABLE
## Brief:       Internal use.
## Notice:      MTK_CM4_PLAYBACK_ENABLE is a option to support CM4 playback function.
##
ifeq ($(MTK_CM4_PLAYBACK_ENABLE),y)
CCFLAG += -DMTK_CM4_PLAYBACK_ENABLE
endif

##
## AIR_MIC_RECORD_ENABLE
## Brief:       Internal use.
## Notice:      AIR_MIC_RECORD_ENABLE is a option to support CM4 record function.
##
AIR_MIC_RECORD_ENABLE ?= $(MTK_CM4_RECORD_ENABLE)
ifeq ($(AIR_MIC_RECORD_ENABLE),y)
CCFLAG += -DMTK_CM4_RECORD_ENABLE
endif

##
## AIR_ANC_WIND_NOISE_DETECTION_ENABLE
## Brief:       Internal use.
## Notice:      AIR_ANC_WIND_NOISE_DETECTION_ENABLE is a option to support wind detection to suppress noise when ANC on.
##
ifeq ($(AIR_ANC_ENABLE),y)
ifeq ($(AIR_ANC_WIND_NOISE_DETECTION_ENABLE),y)
ifeq ($(AIR_WIND_DETECTION_USE_PIC),y)
CCFLAG += -DAIR_WIND_DETECTION_USE_PIC
endif
MTK_ANC_SURROUND_MONITOR_ENABLE = y
CCFLAG += -DAIR_ANC_WIND_DETECTION_ENABLE
CCFLAG += -DMTK_ANC_SURROUND_MONITOR_ENABLE
CCFLAG += -DAIR_AUDIO_TRANSMITTER_ENABLE
endif
endif

## AIR_ANC_ENVIRONMENT_DETECTION_ENABLE
## Brief:       Internal use.
## Notice:      AIR_ANC_ENVIRONMENT_DETECTION_ENABLE
##
ifeq ($(AIR_ANC_ENABLE),y)
ifeq ($(AIR_ANC_ENVIRONMENT_DETECTION_ENABLE),y)
MTK_ANC_SURROUND_MONITOR_ENABLE = y
CCFLAG += -DAIR_ANC_ENVIRONMENT_DETECTION_ENABLE
CCFLAG += -DMTK_ANC_SURROUND_MONITOR_ENABLE
CCFLAG += -DAIR_AUDIO_TRANSMITTER_ENABLE
endif
endif

## ANC_DEBUG_PATH_EANBLE
## Brief:       Internal use.
## Notice:      ANC_DEBUG_PATH_EANBLE
##
ifeq ($(AIR_ANC_ENABLE),y)
ifeq ($(AIR_ANC_DEBUG_PATH_EANBLE),y)
CCFLAG += -DANC_DEBUG_PATH_EANBLE
endif
endif

##
## AIR_SIDETONE_ENABLE
## Brief:       Internal use.
## Notice:      AIR_SIDETONE_ENABLE is a option to support sidetone function.
##
ifeq ($(AIR_SIDETONE_ENABLE),y)
CCFLAG += -DAIR_SIDETONE_ENABLE
endif

##
## AIR_BT_A2DP_ENABLE
## Brief:       Internal use.
## Notice:      AIR_BT_A2DP_ENABLE is a option to support AUDIO LOOPBACK TEST function.
##
AIR_BT_A2DP_ENABLE ?= $(MTK_BT_A2DP_ENABLE)
ifeq ($(AIR_BT_A2DP_ENABLE),y)
CCFLAG += -DMTK_BT_A2DP_ENABLE
endif

##
## MTK_BT_A2DP_AAC_ENABLE
## Brief:       Internal use.
## Notice:      MTK_BT_A2DP_AAC_ENABLE is a option to use AAC codec.
##
ifeq ($(AIR_BT_A2DP_AAC_ENABLE),y)
CCFLAG += -DMTK_BT_A2DP_AAC_ENABLE
endif

##
## AIR_BT_A2DP_SBC_ENABLE
## Brief:       Internal use.
## Notice:      AIR_BT_A2DP_SBC_ENABLE is a option to use SBC codec.
##
AIR_BT_A2DP_SBC_ENABLE ?= $(MTK_BT_A2DP_SBC_ENABLE)
ifeq ($(AIR_BT_A2DP_SBC_ENABLE),y)
CCFLAG += -DMTK_BT_A2DP_SBC_ENABLE
endif

##
## MTK_BT_A2DP_AAC_USE_PIC
## Brief:       Internal use.
## Notice:      MTK_BT_A2DP_AAC_USE_PIC is a option to use AAC PIC. If MTK_BT_A2DP_AAC_ENABLE is enabled, this one should be enabled too.
##
ifeq ($(MTK_BT_A2DP_AAC_USE_PIC),y)
CCFLAG += -DMTK_BT_A2DP_AAC_USE_PIC 
endif

##
## MTK_BT_A2DP_SBC_USE_PIC
## Brief:       Internal use.
## Notice:      MTK_BT_A2DP_SBC_USE_PIC is a option to use SBC PIC. Default should be enabled.
##
ifeq ($(MTK_BT_A2DP_SBC_USE_PIC),y)
CCFLAG += -DMTK_BT_A2DP_SBC_USE_PIC
endif

##
## AIR_ADAPITVE_EQ_DETECTION_USE_PIC_ENABLE
## Brief:       Internal use.
## Notice:      AIR_ADAPITVE_EQ_DETECTION_USE_PIC_ENABLE is a option to use adaptive eq PIC. Default should be enabled.
##
ifeq ($(AIR_ADAPITVE_EQ_DETECTION_USE_PIC_ENABLE),y)
CCFLAG += -DAIR_ADAPITVE_EQ_DETECTION_USE_PIC_ENABLE
endif

##
## MTK_BT_A2DP_VENDOR_ENABLE
## Brief:       Internal use.
## Notice:      MTK_BT_A2DP_VENDOR_ENABLE is an option to use Vendor codec & Vendor codec PIC. Default should be enabled.
##
VENDOR_LIB = $(strip $(ROOTDIR))/middleware/third_party/dspalg/vendor_decoder/module.mk
ifeq ($(VENDOR_LIB), $(wildcard $(VENDOR_LIB)))
ifeq ($(AIR_BT_A2DP_VENDOR_ENABLE),y)
CCFLAG += -DMTK_BT_A2DP_VENDOR_ENABLE
##
## MTK_BT_A2DP_VENDOR_USE_PIC
## Brief:       Internal use.
## Notice:      MTK_BT_A2DP_VENDOR_USE_PIC is an option to use Vendor codec PIC. Default should be enabled.
##
ifeq ($(MTK_BT_A2DP_VENDOR_USE_PIC),y)
CCFLAG += -DMTK_BT_A2DP_VENDOR_USE_PIC
endif
##
## MTK_BT_A2DP_VENDOR_BC_ENABLE
## Brief:       Internal use.
## Notice:      MTK_BT_A2DP_VENDOR_BC_ENABLE is an option to apply buffer control to the Vendor codec. Default should be enabled.
##
ifeq ($(MTK_BT_A2DP_VENDOR_BC_ENABLE),y)
CCFLAG += -DMTK_BT_A2DP_VENDOR_BC_ENABLE
endif
endif
else
AIR_BT_A2DP_VENDOR_ENABLE = n
endif

##
## AIR_MUTE_SMOOTHER_ENABLE
## Brief:       Internal use.
## Notice:      AIR_MUTE_SMOOTHER_ENABLE is a option to global turn on/off mute smoother function.
##
ifeq ($(AIR_MUTE_SMOOTHER_ENABLE),y)
CCFLAG += -DAIR_MUTE_SMOOTHER_ENABLE
endif

##
## AIR_VOICE_PLC_ENABLE
## Brief:       Internal use.
## Notice:      AIR_VOICE_PLC_ENABLE is a option to global turn on/off voice PLC function.
##
ifeq ($(AIR_VOICE_PLC_ENABLE),y)
CCFLAG += -DAIR_VOICE_PLC_ENABLE
endif

##
## AIR_BT_HFP_CVSD_ENABLE
## Brief:       Internal use.
## Notice:      AIR_BT_HFP_CVSD_ENABLE is a option to global turn on/off CVSD encode/decode function.
##
ifeq ($(AIR_BT_HFP_CVSD_ENABLE),y)
CCFLAG += -DAIR_BT_HFP_CVSD_ENABLE
endif

##
## AIR_BT_A2DP_CVSD_USE_PIC_ENABLE
## Brief:       Internal use.
## Notice:      AIR_BT_A2DP_CVSD_USE_PIC_ENABLE is a option to global turn on/off CVSD encode/decode function.
##
ifeq ($(AIR_BT_A2DP_CVSD_USE_PIC_ENABLE),y)
CCFLAG += -DAIR_BT_A2DP_CVSD_USE_PIC_ENABLE
endif

##
## AIR_BT_HFP_MSBC_ENABLE
## Brief:       Internal use.
## Notice:      AIR_BT_HFP_MSBC_ENABLE is a option to global turn on/off mSBC encode/decode function.
##
ifeq ($(AIR_BT_HFP_MSBC_ENABLE),y)
CCFLAG += -DAIR_BT_HFP_MSBC_ENABLE
endif

##
## AIR_VOICE_NR_ENABLE
## Brief:       Internal use.
## Notice:      AIR_VOICE_NR_ENABLE is a option to global turn on/off voice NR function.
##
ifeq ($(AIR_VOICE_NR_ENABLE),y)
CCFLAG += -DAIR_VOICE_NR_ENABLE
endif

##
## AIR_VOICE_NR_USE_PIC_ENABLE
## Brief:       Internal use.
## Notice:      AIR_VOICE_NR_USE_PIC_ENABLE is a option to use ECNR(Echo Cancellation / Noice Reduction) PIC. Default should be enabled.
##
ifeq ($(AIR_VOICE_NR_USE_PIC_ENABLE),y)
CCFLAG += -DAIR_VOICE_NR_USE_PIC_ENABLE
endif

##
## AIR_ECNR_1_OR_2_MIC_ENABLE
## Brief:       Internal use.
## Notice:      AIR_ECNR_1_OR_2_MIC_ENABLE is a option for 1 or 2. Default should not be enabled.
##
ifeq ($(AIR_ECNR_1_OR_2_MIC_ENABLE),y)
CCFLAG += -DAIR_ECNR_1_OR_2_MIC_ENABLE
endif

##
## AIR_ECNR_1MIC_INEAR_ENABLE
## Brief:       Internal use.
## Notice:      AIR_ECNR_1MIC_INEAR_ENABLE is a option to support INEAR function.
##
ifeq ($(AIR_ECNR_1MIC_INEAR_ENABLE),y)
CCFLAG += -DAIR_ECNR_1MIC_INEAR_ENABLE
endif

##
## AIR_ECNR_2MIC_INEAR_ENABLE
## Brief:       Internal use.
## Notice:      AIR_ECNR_2MIC_INEAR_ENABLE is a option to support 2+1NR function.
##
ifeq ($(AIR_ECNR_2MIC_INEAR_ENABLE),y)
CCFLAG += -DAIR_ECNR_2MIC_INEAR_ENABLE
endif

##
## AIR_AI_NR_PREMIUM_ENABLE
## Brief:       Internal use.
## Notice:      AIR_AI_NR_PREMIUM_ENABLE is a option to support AINR premium function.
##
ifeq ($(AIR_AI_NR_PREMIUM_ENABLE),y)
CCFLAG += -DAIR_AI_NR_PREMIUM_ENABLE
endif

##
## AIR_AI_NR_PREMIUM_INEAR_ENABLE
## Brief:       Internal use.
## Notice:      AIR_AI_NR_PREMIUM_INEAR_ENABLE is a option to support AINR premium with FB function.
##
ifeq ($(AIR_AI_NR_PREMIUM_INEAR_ENABLE),y)
CCFLAG += -DAIR_AI_NR_PREMIUM_INEAR_ENABLE
endif

##
## AIR_AI_NR_PREMIUM_INEAR_270K_VARIANT_1_ENABLE
## Brief:       Internal use.
## Notice:      AIR_AI_NR_PREMIUM_INEAR_270K_VARIANT_1_ENABLE is a option to support AINR premium with FB function.
##
ifeq ($(AIR_AI_NR_PREMIUM_INEAR_270K_VARIANT_1_ENABLE),y)
CCFLAG += -DAIR_AI_NR_PREMIUM_INEAR_270K_VARIANT_1_ENABLE
endif

##
## AIR_3RD_PARTY_NR_ENABLE
## Brief:       Internal use.
## Notice:      AIR_3RD_PARTY_NR_ENABLE is a option to support 3rd party ECNR function.
##
ifeq ($(AIR_3RD_PARTY_NR_ENABLE),y)
CCFLAG += -DAIR_3RD_PARTY_NR_ENABLE
endif

## AIR_MULTI_MIC_STREAM_ENABLE
## Brief:       Internal use.
## Notice:      AIR_MULTI_MIC_STREAM_ENABLE is a option to concurrently use AFE source. 
##
ifeq ($(AIR_MULTI_MIC_STREAM_ENABLE),y)
CCFLAG += -DAIR_MULTI_MIC_STREAM_ENABLE
endif

##
## AIR_AIRDUMP_ENABLE
## Brief:       Internal use.
## Notice:      AIR_AIRDUMP_ENABLE is a option to to support AirDump function.
ifeq ($(AIR_AIRDUMP_ENABLE),y)
CCFLAG += -DAIR_AIRDUMP_ENABLE
endif

##
## AIR_AUDIO_DUMP_BY_SPDIF_ENABLE
## Brief:       Internal use.
## Notice:      AIR_AUDIO_DUMP_BY_SPDIF_ENABLE is an option to support audio dump over SPDIF interface.
ifeq ($(AIR_AUDIO_DUMP_BY_SPDIF_ENABLE),y)
CCFLAG += -DAIR_AUDIO_DUMP_BY_SPDIF_ENABLE
endif

##
## AIR_AUDIO_TRANSMITTER_ENABLE
## Brief:       Internal use.
## Notice:      AIR_AUDIO_TRANSMITTER_ENABLE is a option for audio transmitter
ifeq ($(AIR_AUDIO_TRANSMITTER_ENABLE),y)
CCFLAG += -DAIR_AUDIO_TRANSMITTER_ENABLE
endif

##
## AIR_AUDIO_BT_COMMON_ENABLE
## Brief:       Internal use.
## Notice:      AIR_AUDIO_BT_COMMON_ENABLE is a option to to support audio bt common architecture.
ifeq ($(AIR_AUDIO_BT_COMMON_ENABLE), y)
CCFLAG += -DAIR_AUDIO_BT_COMMON_ENABLE
endif

##
## AIR_CPU_SECURITY_MODE
## Brief:       Internal use. This option is determine build in security world or none security world.
## Usage:       The valid values are s,ns.
## Dependency:  None
## Notice:      None
##
ifeq ($(AIR_CPU_SECURITY_MODE),s)
CCFLAG += -DAIR_CPU_IN_SECURITY_MODE
else
#CCFLAG += -DAIR_CPU_IN_NON_SECURITY_MODE
endif

##
## AIR_AUDIO_HARDWARE_ENABLE
## Brief:       Internal use.
## Notice:      AIR_AUDIO_HARDWARE_ENABLE is a option to to enable audio hardware.
ifeq ($(AIR_AUDIO_HARDWARE_ENABLE), y)
CCFLAG += -DAIR_AUDIO_HARDWARE_ENABLE
endif

##
## AIR_HWSRC_CLKSKEW_ENABLE
## Brief:       Internal use.
## Notice:      AIR_HWSRC_CLKSKEW_ENABLE is a option to to enable HWSRC CLK SKEW.
ifeq ($(AIR_HWSRC_CLKSKEW_ENABLE), y)
CCFLAG += -DENABLE_HWSRC_CLKSKEW
endif

##
## AIR_BT_CLK_SKEW_ENABLE
## Brief:       Internal use.
## Notice:      AIR_HWSRC_CLKSKEW_ENABLE is a option to to enable Airoha CLK Skew.
ifeq ($(AIR_BT_CLK_SKEW_ENABLE), y)
CCFLAG += -DAIR_BT_CLK_SKEW_ENABLE
endif

##
## AIR_SOFTWARE_SRC_ENABLE
## Brief:       Internal use.
## Notice:      AIR_SOFTWARE_SRC_ENABLE is a option to enable software src.
##
ifeq ($(AIR_SOFTWARE_SRC_ENABLE),y)
CCFLAG += -DAIR_SOFTWARE_SRC_ENABLE
endif

##
## AIR_SOFTWARE_CLK_SKEW_ENABLE
## Brief:       Internal use.
## Notice:      AIR_SOFTWARE_CLK_SKEW_ENABLE is a option to enable software clk skew.
##
ifeq ($(AIR_SOFTWARE_CLK_SKEW_ENABLE),y)
CCFLAG += -DAIR_SOFTWARE_CLK_SKEW_ENABLE
endif

##
## AIR_SOFTWARE_GAIN_ENABLE
## Brief:       Internal use.
## Notice:      AIR_SOFTWARE_GAIN_ENABLE is a option to enable software gain.
##
ifeq ($(AIR_SOFTWARE_GAIN_ENABLE),y)
CCFLAG += -DAIR_SOFTWARE_GAIN_ENABLE
endif

##
## AIR_DUAL_CHIP_MIXING_MODE
## Brief:       This option is used to set the type of dual chip mixing mode.
## Usage:       If this value is "master", the AIR_DUAL_CHIP_MIXING_MODE_ROLE_MASTER_ENABLE will be defiend. If the value is "slave", the AIR_DUAL_CHIP_MIXING_MODE_ROLE_SLAVE_ENABLE will be defiend
## Path:        None
## Dependency:  None
## Notice:      None
## Relative doc:None
##
ifeq ($(AIR_DUAL_CHIP_MIXING_MODE),master)
CCFLAG  += -DAIR_DUAL_CHIP_MIXING_MODE_ROLE_MASTER_ENABLE
AIR_SOFTWARE_MIXER_ENABLE = y
AIR_SOFTWARE_BUFFER_ENABLE = n
AIR_LOW_LANTENCY_MUX_ENABLE = y
endif
ifeq ($(AIR_DUAL_CHIP_MIXING_MODE),slave)
CCFLAG  += -DAIR_DUAL_CHIP_MIXING_MODE_ROLE_SLAVE_ENABLE
AIR_SOFTWARE_MIXER_ENABLE = y
AIR_SOFTWARE_BUFFER_ENABLE = n
AIR_LOW_LANTENCY_MUX_ENABLE = y
endif

##
## AIR_SOFTWARE_MIXER_ENABLE
## Brief:       Internal use.
## Notice:      AIR_SOFTWARE_MIXER_ENABLE is a option to enable software mixer.
##
ifeq ($(AIR_SOFTWARE_MIXER_ENABLE),y)
CCFLAG += -DAIR_SOFTWARE_MIXER_ENABLE
endif

##
## AIR_SOFTWARE_BUFFER_ENABLE
## Brief:       Internal use.
## Notice:      AIR_SOFTWARE_BUFFER_ENABLE is a option to enable software buffer.
##
ifeq ($(AIR_SOFTWARE_BUFFER_ENABLE),y)
CCFLAG += -DAIR_SOFTWARE_BUFFER_ENABLE
endif

##
## AIR_ANC_FIT_DETECTION_ENABLE
## Brief:       Internal use.
## Notice:      AIR_ANC_FIT_DETECTION_ENABLE is a option to enable fit detection.
##
MTK_LEAKAGE_DETECTION_ENABLE ?= $(AIR_ANC_FIT_DETECTION_ENABLE)
ifeq ($(MTK_LEAKAGE_DETECTION_ENABLE),y)
CCFLAG += -DMTK_LEAKAGE_DETECTION_ENABLE
endif

##
## AIR_AUDIO_NONREALTIME_RX_ENABLE
## Brief:       Internal use.
## Notice:      AIR_AUDIO_NONREALTIME_RX_ENABLE is a option to enable low priority RX task mechanism.
ifeq ($(AIR_AUDIO_NONREALTIME_RX_ENABLE), y)
CCFLAG += -DAIR_AUDIO_NONREALTIME_RX_ENABLE
#CCFLAG += -DAIR_AUDIO_IRQ_LEVEL_RX_ENABLE
endif

##
## AIR_ULL_AUDIO_V2_DONGLE_ENABLE
## Brief:       Internal use.
## Notice:      AIR_ULL_AUDIO_V2_DONGLE_ENABLE is a option for ULL2.0 dongle feature.
##
ifeq ($(AIR_ULL_AUDIO_V2_DONGLE_ENABLE),y)
CCFLAG += -DAIR_ULL_AUDIO_V2_DONGLE_ENABLE
endif

##
## AIR_ULL_BLE_HEADSET_ENABLE
## Brief:       Internal use.
## Notice:      AIR_ULL_BLE_HEADSET_ENABLE is a option for ULL2.0 headset feature.
##
ifeq ($(AIR_ULL_BLE_HEADSET_ENABLE),y)
CCFLAG += -DAIR_ULL_BLE_HEADSET_ENABLE
endif

##
## AIR_BT_LE_LC3PLUS_ENABLE
## Brief:       Internal use.
## Notice:      AIR_BT_LE_LC3PLUS_ENABLE is a option for LC3plus codec.
##
ifeq ($(AIR_BT_LE_LC3PLUS_ENABLE),y)
CCFLAG += -DAIR_BT_LE_LC3PLUS_ENABLE
endif

##
## AIR_BT_A2DP_LC3PLUS_USE_PIC
## Brief:       Internal use.
## Notice:      AIRBT_A2DP_LC3PLUS_USE_PIC is a option to use LC3 PIC. Default should be enabled.
##
ifeq ($(AIR_BT_LE_LC3PLUS_USE_PIC),y)
CCFLAG += -DAIR_BT_LE_LC3PLUS_USE_PIC
endif
##

##
## AIR_WIRELESS_MIC_ENABLE
## Brief:       Internal use.
## Notice:      This option is for wireless mic.
##
ifeq ($(AIR_WIRELESS_MIC_ENABLE),y)
CCFLAG += -DAIR_WIRELESS_MIC_ENABLE
endif

##
## AIR_VOLUME_CONTROL_ON_DRC_ENABLE
## Brief:       Internal use.
## Notice:      AIR_VOLUME_CONTROL_ON_DRC_ENABLE is a option to enable volume control on DRC(Dynamic Range Compression)
##
ifeq ($(AIR_VOLUME_CONTROL_ON_DRC_ENABLE),y)
CCFLAG += -DAIR_VOLUME_CONTROL_ON_DRC_ENABLE
endif

###############################################################################


ifneq ($(MTK_LOWPOWER_LEVEL),)
CCFLAG += -DMTK_LOWPOWER_LEVEL=$(MTK_LOWPOWER_LEVEL)
endif

ifeq ($(ENABLE_SIDETONE_RAMP_TIMER),y)
CCFLAG += -DENABLE_SIDETONE_RAMP_TIMER
endif

ifeq ($(ENABLE_FRAMEWORK_MULTIPLE_CHANNEL),y)
CCFLAG += -DENABLE_FRAMEWORK_MULTIPLE_CHANNEL
endif

ifeq ($(ENABLE_AUDIO_WITH_JOINT_MIC),y)
CCFLAG += -DENABLE_AUDIO_WITH_JOINT_MIC
endif

ifeq ($(MTK_DSP1_DRAM_FOR_DSP0_POOL_ENABLE),y)
CCFLAG += -DMTK_DSP1_DRAM_FOR_DSP0_POOL_ENABLE
endif

ifeq ($(AB1568_BRING_UP_DSP_DEFAULT_HW_LOOPBACK),y)
CCFLAG += -DAB1568_BRING_UP_DSP_DEFAULT_HW_LOOPBACK
endif

ifeq ($(MTK_BT_AVM_SHARE_BUF),y)
CCFLAG += -DMTK_BT_AVM_SHARE_BUF
endif

ifeq ($(AIR_AUDIO_PLC_ENABLE),y)
AUDIO_PLC_LIB = $(strip $(ROOTDIR))/middleware/third_party/dspalg/audio_plc/module.mk
ifeq ($(AUDIO_PLC_LIB), $(wildcard $(AUDIO_PLC_LIB)))
CCFLAG += -DMTK_AUDIO_PLC_ENABLE
else
AIR_AUDIO_PLC_ENABLE = n
endif
endif

ifeq ($(MTK_SLT_AUDIO_HW),y)
CCFLAG += -DMTK_SLT_AUDIO_HW
endif

##
## AIR_A2DP_NLE_ENABLE
## Brief:       Internal use.
## Notice:      AIR_A2DP_NLE_ENABLE is a option to enabled A2DP NLE feature.
##
ifeq ($(AIR_A2DP_NLE_ENABLE),y)
CCFLAG += -DAIR_A2DP_NLE_ENABLE
endif

##
## AIR_RESET_SDM_ENABLE
## Brief:       Internal use.
## Notice:      AIR_RESET_SDM_ENABLE is a option to enabled RESET SDM feature.
##
ifeq ($(AIR_RESET_SDM_ENABLE),y)
CCFLAG += -DAIR_RESET_SDM_ENABLE
endif

##
## AIR_MITO_AUDIO_PATH_ENABLE
## Brief:       Internal use.
## Notice:      AIR_MITO_AUDIO_PATH_ENABLE is a option to enabled MITO project audio path.
##
ifeq ($(AIR_MITO_AUDIO_PATH_ENABLE),y)
CCFLAG += -DAIR_MITO_AUDIO_PATH_ENABLE
endif

# Use the library files attached to the Xtensa tool chain to participate in the link.
XTENSA_STANDARD_LIBS  +=  $(strip $(XTENSA_LIB_PATH))/arch/lib/libhal.a
XTENSA_STANDARD_LIBS  +=  $(strip $(XTENSA_LIB_PATH))/lib/libc.a
XTENSA_STANDARD_LIBS  +=  $(strip $(XTENSA_LIB_PATH))/lib/xcc/libgcc.a
XTENSA_STANDARD_LIBS  +=  $(strip $(XTENSA_LIB_PATH))/arch/lib/libhandlers-board.a
XTENSA_STANDARD_LIBS  +=  $(strip $(XTENSA_LIB_PATH))/lib/libgloss.a
XTENSA_STANDARD_LIBS  +=  $(strip $(XTENSA_LIB_PATH))/lib/libm.a
