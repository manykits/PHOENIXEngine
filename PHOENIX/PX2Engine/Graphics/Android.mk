LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := Graphics

FILE_LIST := $(wildcard $(LOCAL_PATH)/*cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/Renderers/OpenGLESRenderer/*cpp)
LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)

LOCAL_C_INCLUDES := $(LOCAL_PATH)/.. \
$(LOCAL_PATH)/Renderers/OpenGLESRenderer/ \
$(PX2_ROOT)/PX2SDK/Include/ \
$(PX2_ROOT)/ThirdPartiesLibs/OGLES/

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/..

LOCAL_CFLAGS := -D__ANDROID__ -DPX2_USE_OPENGLES
LOCAL_CPPFLAGS := -frtti -std=c++11 -fexceptions -LUAPLUS_EXCEPTIONS

LOCAL_LDLIBS := -lGLESv2 -lEGL -llog 
LOCAL_EXPORT_LDLIBS := -llog -lGLESv2

include $(BUILD_STATIC_LIBRARY)
