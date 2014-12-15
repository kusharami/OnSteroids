LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := OnSteroids

LOCAL_SRC_FILES :=	$(subst $(LOCAL_PATH)/./,,$(wildcard $(LOCAL_PATH)/./*.cpp))
										
LOCAL_CFLAGS += -fpermissive
LOCAL_CFLAGS += -fexceptions

LOCAL_LDLIBS := -lGLESv1_CM -landroid -ldl -llog -ljnigraphics

include $(BUILD_SHARED_LIBRARY)
