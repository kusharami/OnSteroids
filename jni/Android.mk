LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := OnSteroids

LOCAL_ARM_MODE := arm

LOCAL_SRC_FILES :=	$(subst $(LOCAL_PATH)/./,,$(wildcard $(LOCAL_PATH)/./engine/*.cpp))
LOCAL_SRC_FILES +=	$(subst $(LOCAL_PATH)/./,,$(wildcard $(LOCAL_PATH)/./objects/*.cpp))
LOCAL_SRC_FILES +=	$(subst $(LOCAL_PATH)/./,,$(wildcard $(LOCAL_PATH)/./game/*.cpp))
LOCAL_SRC_FILES +=	$(subst $(LOCAL_PATH)/./,,$(wildcard $(LOCAL_PATH)/./*.cpp))
						
LOCAL_CPP_FEATURES := rtti exceptions				
LOCAL_CFLAGS += -fpermissive -frtti -fexceptions -g -ggdb -std=c++11

LOCAL_CPPFLAGS += -ffunction-sections -fdata-sections
LOCAL_CFLAGS += -ffunction-sections -fdata-sections 
LOCAL_LDFLAGS += -Wl,--gc-sections


#LOCAL_LDLIBS := -lGLESv1_CM -landroid -ldl -llog -ljnigraphics
LOCAL_LDLIBS :=  -lstdc++ -lc -lm -llog -landroid -ldl -ljnigraphics -lGLESv1_CM -lGLESv2 -lEGL

include $(BUILD_SHARED_LIBRARY)
