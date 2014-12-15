LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := NativeLibrary

LOCAL_ARM_MODE	:= thumb

LOCAL_SRC_FILES :=	$(subst $(LOCAL_PATH)/./,,$(wildcard $(LOCAL_PATH)/./engine/*.cpp))
LOCAL_SRC_FILES +=	$(subst $(LOCAL_PATH)/./,,$(wildcard $(LOCAL_PATH)/./objects/*.cpp))
LOCAL_SRC_FILES +=	$(subst $(LOCAL_PATH)/./,,$(wildcard $(LOCAL_PATH)/./game/*.cpp))
LOCAL_SRC_FILES +=	$(subst $(LOCAL_PATH)/./,,$(wildcard $(LOCAL_PATH)/./*.cpp))
										
LOCAL_CFLAGS += -fpermissive -fexceptions -g -ggdb -std=c++11

#LOCAL_LDLIBS := -lGLESv1_CM -landroid -ldl -llog -ljnigraphics
LOCAL_LDLIBS :=  -lstdc++ -lc -lm -llog -landroid -ldl -ljnigraphics -lGLESv1_CM -lGLESv2 -lEGL

include $(BUILD_SHARED_LIBRARY)
