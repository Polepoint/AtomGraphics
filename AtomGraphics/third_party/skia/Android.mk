LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE:= skia
LOCAL_SRC_FILES := $(LOCAL_PATH)/libs/$(TARGET_ARCH_ABI)/libskia.a

LOCAL_EXPORT_C_INCLUDES := \
    $(LOCAL_PATH) \
    $(LOCAL_PATH)/skia \
    $(LOCAL_PATH)/skia/android \
    $(LOCAL_PATH)/skia/atlastext \
    $(LOCAL_PATH)/skia/c \
    $(LOCAL_PATH)/skia/codec \
    $(LOCAL_PATH)/skia/config \
    $(LOCAL_PATH)/skia/core \
    $(LOCAL_PATH)/skia/effects \
    $(LOCAL_PATH)/skia/encode \
    $(LOCAL_PATH)/skia/gpu \
    $(LOCAL_PATH)/skia/pathops \
    $(LOCAL_PATH)/skia/ports \
    $(LOCAL_PATH)/skia/private \
    $(LOCAL_PATH)/skia/svg \
    $(LOCAL_PATH)/skia/utils

LOCAL_EXPORT_LDLIBS := \
    -lz \
    -lEGL \
    -lGLESv2

include $(PREBUILT_STATIC_LIBRARY)
