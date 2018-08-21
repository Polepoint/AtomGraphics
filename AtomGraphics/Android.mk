LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := atomgraphics

LOCAL_MODULE_FILENAME := libatomgraphics

# Collect all source files
MY_SRC_LIST := $(wildcard $(LOCAL_PATH)/*.cpp)
MY_SRC_LIST += $(wildcard $(LOCAL_PATH)/base/*.cpp)
MY_SRC_LIST += $(wildcard $(LOCAL_PATH)/graphics/*.cpp)
MY_SRC_LIST += $(wildcard $(LOCAL_PATH)/graphics/android/*.cpp)
MY_SRC_LIST += $(wildcard $(LOCAL_PATH)/math/*.cpp)
MY_SRC_LIST += $(wildcard $(LOCAL_PATH)/node/*.cpp)
MY_SRC_LIST += $(wildcard $(LOCAL_PATH)/node/android/*.cpp)
MY_SRC_LIST += $(wildcard $(LOCAL_PATH)/node/canvas/*.cpp)
MY_SRC_LIST += $(wildcard $(LOCAL_PATH)/node/canvas/android/*.cpp)
MY_SRC_LIST += $(wildcard $(LOCAL_PATH)/platform/*.cpp)
MY_SRC_LIST += $(wildcard $(LOCAL_PATH)/android/*.cpp)
MY_SRC_LIST += $(wildcard $(LOCAL_PATH)/javascript/*.cpp)
MY_SRC_LIST += $(wildcard $(LOCAL_PATH)/javascript/android/*.cpp)
MY_SRC_LIST += $(wildcard $(LOCAL_PATH)/jsbridge/*.cpp)
MY_SRC_LIST += $(wildcard $(LOCAL_PATH)/thread/*.cpp)
MY_SRC_LIST += $(wildcard $(LOCAL_PATH)/thread/android/*.cpp)


# Collect all header files
MY_INCLUDE_LIST := $(wildcard $(LOCAL_PATH)/*.h)
MY_INCLUDE_LIST += $(wildcard $(LOCAL_PATH)/base/*.h)
MY_INCLUDE_LIST += $(wildcard $(LOCAL_PATH)/graphics/*.h)
MY_INCLUDE_LIST += $(wildcard $(LOCAL_PATH)/graphics/android/*.h)
MY_INCLUDE_LIST += $(wildcard $(LOCAL_PATH)/math/*.h)
MY_INCLUDE_LIST += $(wildcard $(LOCAL_PATH)/node/*.h)
MY_INCLUDE_LIST += $(wildcard $(LOCAL_PATH)/node/android/*.h)
MY_INCLUDE_LIST += $(wildcard $(LOCAL_PATH)/node/canvas/*.h)
MY_INCLUDE_LIST += $(wildcard $(LOCAL_PATH)/node/canvas/android/*.h)
MY_INCLUDE_LIST += $(wildcard $(LOCAL_PATH)/platform/*.h)
MY_INCLUDE_LIST += $(wildcard $(LOCAL_PATH)/android/*.h)
MY_INCLUDE_LIST += $(wildcard $(LOCAL_PATH)/javascript/*.h)
MY_INCLUDE_LIST += $(wildcard $(LOCAL_PATH)/javascript/android/*.h)
MY_INCLUDE_LIST += $(wildcard $(LOCAL_PATH)/jsbridge/*.h)
MY_INCLUDE_LIST += $(wildcard $(LOCAL_PATH)/thread/*.h)
MY_INCLUDE_LIST += $(wildcard $(LOCAL_PATH)/thread/android/*.h)


# Source files to be compiled
LOCAL_SRC_FILES := $(patsubst $(LOCAL_PATH)/%, %, $(MY_SRC_LIST))
# Header files to be included
LOCAL_INCLUDE_FILES := $(patsubst $(LOCAL_PATH)/%, %, $(MY_INCLUDE_LIST))

LOCAL_LDLIBS+= -L$(SYSROOT)/usr/lib -llog -landroid

LOCAL_SHARED_LIBRARIES := libgcanvas libjsc

include $(BUILD_SHARED_LIBRARY)

$(call import-add-path, $(LOCAL_PATH))
$(call import-module, third_party/jsc)
$(call import-module, ../GCanvas/GCanvas/core/src)

#cmd-strip :=
