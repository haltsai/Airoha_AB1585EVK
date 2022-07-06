################################################################################
# Compiler Toolchain Settings
################################################################################
#Xtensa tool chain path & license setting,
#These settings can be configured either in a project's Makefile for the specific
#project or setting here for all the projets.

IOT_SDK_XTENSA_VERSION = 8013
include $(ROOTDIR)/.config.mk
include $(ROOTDIR)/.rule.mk
XTENSA_CORE = Hifi5_MT2833_FDI_FINAL
