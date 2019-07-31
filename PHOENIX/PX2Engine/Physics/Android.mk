LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := Physics

FILE_LIST := $(wildcard $(LOCAL_PATH)/*cpp) \
FILE_LIST += $(wildcard $(LOCAL_PATH)/Bullet/LinearMath/*cpp) \
FILE_LIST += $(wildcard $(LOCAL_PATH)/Bullet/BulletCollision/BroadphaseCollision/*cpp) \
FILE_LIST += $(wildcard $(LOCAL_PATH)/Bullet/BulletCollision/CollisionDispatch/*cpp) \
FILE_LIST += $(wildcard $(LOCAL_PATH)/Bullet/BulletCollision/CollisionShapes/*cpp) \
FILE_LIST += $(wildcard $(LOCAL_PATH)/Bullet/BulletCollision/Gimpact/*cpp) \
FILE_LIST += $(wildcard $(LOCAL_PATH)/Bullet/BulletCollision/NarrowPhaseCollision/*cpp) \
FILE_LIST += $(wildcard $(LOCAL_PATH)/Bullet/BulletDynamics/Character/*cpp) \
FILE_LIST += $(wildcard $(LOCAL_PATH)/Bullet/BulletDynamics/ConstraintSolver/*cpp) \
FILE_LIST += $(wildcard $(LOCAL_PATH)/Bullet/BulletDynamics/Dynamics/*cpp) \
FILE_LIST += $(wildcard $(LOCAL_PATH)/Bullet/BulletDynamics/Vehicle/*cpp) \

LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%) \
Bullet/ \
Bullet/LinearMath/ \
Bullet/BulletCollision/ \
Bullet/BulletDynamics/ \

LOCAL_C_INCLUDES := $(LOCAL_PATH)/.. \
$(LOCAL_PATH)/Bullet/ \
$(PX2_ROOT)/PX2SDK/Include/ \

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/..

LOCAL_CFLAGS := -D__ANDROID__ 
LOCAL_CPPFLAGS := -frtti -std=c++11 -fexceptions

include $(BUILD_STATIC_LIBRARY)
