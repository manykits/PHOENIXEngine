LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := PX2Engine

FILE_LIST := $(wildcard $(LOCAL_PATH)/*cpp)
LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)

LOCAL_WHOLE_STATIC_LIBRARIES := Core
LOCAL_WHOLE_STATIC_LIBRARIES += Mathematics
LOCAL_WHOLE_STATIC_LIBRARIES += Graphics
LOCAL_WHOLE_STATIC_LIBRARIES += Unity
LOCAL_WHOLE_STATIC_LIBRARIES += Net
LOCAL_WHOLE_STATIC_LIBRARIES += Effect
LOCAL_WHOLE_STATIC_LIBRARIES += Terrains
LOCAL_WHOLE_STATIC_LIBRARIES += UI
LOCAL_WHOLE_STATIC_LIBRARIES += Physics
LOCAL_WHOLE_STATIC_LIBRARIES += Voxel
LOCAL_WHOLE_STATIC_LIBRARIES += AI
LOCAL_WHOLE_STATIC_LIBRARIES += Logic
LOCAL_WHOLE_STATIC_LIBRARIES += Application

LOCAL_SHARED_LIBRARIES := fmod

LOCAL_C_INCLUDES := $(LOCAL_PATH)/..

include $(BUILD_STATIC_LIBRARY)

$(call import-module, Core)
$(call import-module, Mathematics)
$(call import-module, Graphics)
$(call import-module, Unity)
$(call import-module, Net)
$(call import-module, Effect)
$(call import-module, Terrains)
$(call import-module, UI)
$(call import-module, Physics)
$(call import-module, Voxel)
$(call import-module, AI)
$(call import-module, Logic)
$(call import-module, Application)