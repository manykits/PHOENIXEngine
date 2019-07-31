LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := Logic

FILE_LIST := $(wildcard $(LOCAL_PATH)/*cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/BlueBlock/*cpp)
LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)

LOCAL_C_INCLUDES := $(LOCAL_PATH)/.. \
$(PX2_ROOT)/PX2SDK/Include/ \
$(PX2_ROOT)/PX2Engine/Unity/Lua/ \
$(PX2_ROOT)/PX2Engine/Unity/ToLua/ \
$(PX2_ROOT)/PX2Engine/Unity/LuaPlus/ \
LOCAL_C_INCLUDES += $(LOCAL_PATH)/BlueBlock/ \


LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/..

LOCAL_CFLAGS := -D__ANDROID__
LOCAL_CPPFLAGS := -frtti -std=c++11 -fexceptions -LUAPLUS_EXCEPTIONS

include $(BUILD_STATIC_LIBRARY)
