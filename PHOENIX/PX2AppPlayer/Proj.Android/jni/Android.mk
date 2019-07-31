LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := AppPlay

FILE_LIST := $(wildcard $(LOCAL_PATH)/*cpp)
LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)

LOCAL_WHOLE_STATIC_LIBRARIES := PX2AppPlayer
LOCAL_WHOLE_STATIC_LIBRARIES += PX2Engine

LOCAL_C_INCLUDES := $(LOCAL_PATH)/.. \
$(PX2_ROOT)/ThirdPartiesLibs/OGLES/ \
$(PX2_ROOT)/PX2AppPlayer/ \
$(PX2_ROOT)/PX2Engine/Unity/AppPlay/ \
$(PX2_ROOT)/PX2SDK/Include/

include $(BUILD_SHARED_LIBRARY)

$(call import-module, PX2Engine)
$(call import-module, PX2AppPlayer)

#ProjHULL

#PROJECT_NAME := General

#include $(CLEAR_VARS)
#LOCAL_MODULE := $(PROJECT_NAME)SO

#LOCAL_WHOLE_STATIC_LIBRARIES := $(PROJECT_NAME)

#include $(BUILD_SHARED_LIBRARY)

#$(call import-module, $(PROJECT_NAME))