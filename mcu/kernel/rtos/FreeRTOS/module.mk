
###################################################
# Sources
RTOS_SRC = kernel/rtos/FreeRTOS/Source

RTOS_FILES =	$(RTOS_SRC)/tasks.c \
		$(RTOS_SRC)/list.c \
		$(RTOS_SRC)/queue.c \
		$(RTOS_SRC)/timers.c \
		$(RTOS_SRC)/event_groups.c \
		$(RTOS_SRC)/stream_buffer.c \
		$(RTOS_SRC)/portable/MemMang/heap_4.c

ifeq ($(MTK_CPU_TYPE),ARM_CM33)
ifeq ($(AIROHA_TFM_ENABLE),y)
#for cm33 + TFM
PORTABLE = ARM_CM33_TFM
RTOS_FILES +=	$(RTOS_SRC)/portable/GCC/$(PORTABLE)/port.c \
				$(RTOS_SRC)/portable/GCC/$(PORTABLE)/portasm.c \
				$(RTOS_SRC)/portable/GCC/$(PORTABLE)/os_wrapper_freertos.c \
				$(RTOS_SRC)/portable/GCC/$(PORTABLE)/tz_context_mgmt.c
CFLAGS     += -I$(SOURCE_DIR)/$(RTOS_SRC)/portable/GCC/$(PORTABLE)
else
#for cm33 w/o TFM + w/ or w/o TrustZone feature
#ifeq ($(AIR_LIMIT_TZ_ENABLE),y)
#PORTABLE = ARM_CM33
#CFLAGS     += -I$(SOURCE_DIR)/$(RTOS_SRC)/portable/GCC/$(PORTABLE)/secure
#else
PORTABLE = ARM_CM33_NTZ
#endif
RTOS_FILES +=	$(RTOS_SRC)/portable/GCC/$(PORTABLE)/non_secure/port.c \
				$(RTOS_SRC)/portable/GCC/$(PORTABLE)/non_secure/portasm.c
CFLAGS     += -I$(SOURCE_DIR)/$(RTOS_SRC)/portable/GCC/$(PORTABLE)/non_secure
endif
else
#for CM4
RTOS_FILES +=	$(RTOS_SRC)/portable/GCC/ARM_CM4F/port.c
CFLAGS     += -I$(SOURCE_DIR)/$(RTOS_SRC)/portable/GCC/ARM_CM4F
endif

ifeq ($(PRODUCT_VERSION),1552)
RTOS_FILES += 	$(RTOS_SRC)/portable/GCC/ab155x/port_tick.c 
CFLAGS     += -I$(SOURCE_DIR)/$(RTOS_SRC)/portable/GCC/ab155x
endif

ifeq ($(IC_CONFIG),mt2822)
RTOS_FILES += 	$(RTOS_SRC)/portable/GCC/mt2822/port_tick.c 
CFLAGS     += -I$(SOURCE_DIR)/$(RTOS_SRC)/portable/GCC/mt2822
endif

ifeq ($(IC_CONFIG),ab156x)
RTOS_FILES += 	$(RTOS_SRC)/portable/GCC/ab156x/port_tick.c 
CFLAGS     += -I$(SOURCE_DIR)/$(RTOS_SRC)/portable/GCC/ab156x
endif

ifeq ($(PRODUCT_VERSION),7698)
RTOS_FILES += 	$(RTOS_SRC)/portable/GCC/aw7698/port_tick.c
CFLAGS     += -I$(SOURCE_DIR)/$(RTOS_SRC)/portable/GCC/aw7698
endif

ifeq ($(PRODUCT_VERSION),3335)
RTOS_FILES += 	$(RTOS_SRC)/portable/GCC/ag3335/port_tick.c
CFLAGS     += -I$(SOURCE_DIR)/$(RTOS_SRC)/portable/GCC/ag3335
endif

ifeq ($(AIR_BTA_IC_PREMIUM_G3),y)
RTOS_FILES += 	$(RTOS_SRC)/portable/GCC/bta_ic_premium_g3/port_tick.c
CFLAGS     += -I$(SOURCE_DIR)/$(RTOS_SRC)/portable/GCC/bta_ic_premium_g3
endif

C_FILES += $(RTOS_FILES)

###################################################
# include path
CFLAGS += -I$(SOURCE_DIR)/$(RTOS_SRC)/include
CFLAGS += -I$(SOURCE_DIR)/$(RTOS_SRC)/portable/GCC/ARM_CM4F

CFLAGS += -I$(SOURCE_DIR)/kernel/service/inc

###################################################
#Enable the feature by configuring
FREERTOS_VERSION = V10_AND_LATER
CFLAGS += -DFREERTOS_ENABLE
CFLAGS += -DFREERTOS_VERSION=$(FREERTOS_VERSION)