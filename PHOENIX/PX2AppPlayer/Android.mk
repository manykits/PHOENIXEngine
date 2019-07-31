LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := PX2AppPlayer

FILE_LIST := $(wildcard $(LOCAL_PATH)/*cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/AppFrame/*cpp)
LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)

LOCAL_C_INCLUDES := $(LOCAL_PATH)/..\
$(LOCAL_PATH)/AppFrame/ \
$(PX2_ROOT)/PX2SDK/Include/

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)

LOCAL_CFLAGS := -D__ANDROID__
LOCAL_CPPFLAGS := -frtti -std=c++11 -fexceptions -LUAPLUS_EXCEPTIONS
            
include $(BUILD_STATIC_LIBRARY)