LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := Application

FILE_LIST := $(wildcard $(LOCAL_PATH)/*cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/AppPlay/*cpp)
LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%) \
../ToScript/PX2ToLua.cpp

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/.. \
$(LOCAL_PATH)/ToScript/ \

LOCAL_C_INCLUDES := $(LOCAL_PATH)/.. \
$(LOCAL_PATH)/AppPlay/ \
$(PX2_ROOT)/PX2Engine/Unity/Lua/ \
$(PX2_ROOT)/PX2Engine/Unity/ToLua/ \
$(PX2_ROOT)/PX2Engine/Unity/LuaPlus/ \
$(PX2_ROOT)/PX2Engine/ToScript/ \
$(PX2_ROOT)/PX2Engine/Unity/AppPlay/ \
$(PX2_ROOT)/PX2Engine/AI/OpenSteer/include/ \
$(PX2_ROOT)/PX2SDK/Include/

LOCAL_CFLAGS := -D__ANDROID__
LOCAL_CPPFLAGS := -frtti -std=c++11 -fexceptions -LUAPLUS_EXCEPTIONS

include $(BUILD_STATIC_LIBRARY)
