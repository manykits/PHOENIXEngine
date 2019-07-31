LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := Mathematics

FILE_LIST := $(wildcard $(LOCAL_PATH)/*cpp)
LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/..

LOCAL_C_INCLUDES := $(LOCAL_PATH)/.. \
$(PX2_ROOT)/PX2SDK/Include/

include $(BUILD_STATIC_LIBRARY)
