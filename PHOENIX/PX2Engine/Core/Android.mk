LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := Core

FILE_LIST := $(wildcard $(LOCAL_PATH)/*cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/RapidXML/*cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/RapidJSON/*cpp)
LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)

LOCAL_C_INCLUDES := $(LOCAL_PATH)/.. \
$(LOCAL_PATH)/RapidXML/ \
$(LOCAL_PATH)/RapidJSON/

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/..

LOCAL_CFLAGS := -D__ANDROID__
LOCAL_CPPFLAGS := -frtti -std=c++11 -fexceptions -LUAPLUS_EXCEPTIONS

LOCAL_LDLIBS := -llog -pthread
LOCAL_EXPORT_LDLIBS := -llog

include $(BUILD_STATIC_LIBRARY)
