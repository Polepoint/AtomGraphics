LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := atomgraphics

# Collect all source files
MY_SRC_LIST := $(wildcard $(LOCAL_PATH)/*.cpp)
MY_SRC_LIST += $(wildcard $(LOCAL_PATH)/android/*.cpp)
MY_SRC_LIST += $(wildcard $(LOCAL_PATH)/android/spi/*.cpp)
MY_SRC_LIST += $(wildcard $(LOCAL_PATH)/base/*.cpp)
MY_SRC_LIST += $(wildcard $(LOCAL_PATH)/base/android/*.cpp)
MY_SRC_LIST += $(wildcard $(LOCAL_PATH)/cache/*.cpp)
MY_SRC_LIST += $(wildcard $(LOCAL_PATH)/cache/android/*.cc)
MY_SRC_LIST += $(wildcard $(LOCAL_PATH)/gpu/*.cpp)
MY_SRC_LIST += $(wildcard $(LOCAL_PATH)/graphics/*.cpp)
MY_SRC_LIST += $(wildcard $(LOCAL_PATH)/graphics/android/*.cpp)
MY_SRC_LIST += $(wildcard $(LOCAL_PATH)/graphics/android/*.cc)
MY_SRC_LIST += $(wildcard $(LOCAL_PATH)/graphics/android/skia/*.cpp)
MY_SRC_LIST += $(wildcard $(LOCAL_PATH)/graphics/font/*.cpp)
MY_SRC_LIST += $(wildcard $(LOCAL_PATH)/graphics/font/android/*.cc)
MY_SRC_LIST += $(wildcard $(LOCAL_PATH)/math/*.cpp)
MY_SRC_LIST += $(wildcard $(LOCAL_PATH)/math/android/*.cc)
MY_SRC_LIST += $(wildcard $(LOCAL_PATH)/memory/*.cpp)
MY_SRC_LIST += $(wildcard $(LOCAL_PATH)/node/*.cpp)
MY_SRC_LIST += $(wildcard $(LOCAL_PATH)/node/canvas/*.cpp)
MY_SRC_LIST += $(wildcard $(LOCAL_PATH)/node/canvas/android/*.cpp)
MY_SRC_LIST += $(wildcard $(LOCAL_PATH)/node/canvas/android/*.cc)
MY_SRC_LIST += $(wildcard $(LOCAL_PATH)/node/canvas/android/skia/*.cpp)
MY_SRC_LIST += $(wildcard $(LOCAL_PATH)/platform/*.cpp)
MY_SRC_LIST += $(wildcard $(LOCAL_PATH)/io/*.cpp)
MY_SRC_LIST += $(wildcard $(LOCAL_PATH)/io/android/*.cc)
MY_SRC_LIST += $(wildcard $(LOCAL_PATH)/javascript/*.cpp)
MY_SRC_LIST += $(wildcard $(LOCAL_PATH)/javascript/android/*.cpp)
MY_SRC_LIST += $(wildcard $(LOCAL_PATH)/javascript/v8/*.cpp)
MY_SRC_LIST += $(wildcard $(LOCAL_PATH)/thread/*.cpp)
MY_SRC_LIST += $(wildcard $(LOCAL_PATH)/thread/android/*.cpp)


# Collect all header files
MY_INCLUDE_LIST := $(wildcard $(LOCAL_PATH)/*.h)
MY_INCLUDE_LIST += $(wildcard $(LOCAL_PATH)/android/*.h)
MY_INCLUDE_LIST += $(wildcard $(LOCAL_PATH)/android/spi/*.h)
MY_INCLUDE_LIST += $(wildcard $(LOCAL_PATH)/base/*.h)
MY_INCLUDE_LIST += $(wildcard $(LOCAL_PATH)/base/android/*.h)
MY_INCLUDE_LIST += $(wildcard $(LOCAL_PATH)/cache/*.h)
MY_INCLUDE_LIST += $(wildcard $(LOCAL_PATH)/cache/android/*.h)
MY_INCLUDE_LIST += $(wildcard $(LOCAL_PATH)/gpu/*.h)
MY_INCLUDE_LIST += $(wildcard $(LOCAL_PATH)/graphics/*.h)
MY_INCLUDE_LIST += $(wildcard $(LOCAL_PATH)/graphics/android/*.h)
MY_INCLUDE_LIST += $(wildcard $(LOCAL_PATH)/graphics/android/skia/*.h)
MY_INCLUDE_LIST += $(wildcard $(LOCAL_PATH)/graphics/font/*.h)
MY_INCLUDE_LIST += $(wildcard $(LOCAL_PATH)/math/*.h)
MY_INCLUDE_LIST += $(wildcard $(LOCAL_PATH)/math/android/*.h)
MY_INCLUDE_LIST += $(wildcard $(LOCAL_PATH)/memory/*.h)
MY_INCLUDE_LIST += $(wildcard $(LOCAL_PATH)/node/*.h)
MY_INCLUDE_LIST += $(wildcard $(LOCAL_PATH)/node/canvas/*.h)
MY_INCLUDE_LIST += $(wildcard $(LOCAL_PATH)/node/canvas/android/*.h)
MY_INCLUDE_LIST += $(wildcard $(LOCAL_PATH)/node/canvas/android/skia/*.h)
MY_INCLUDE_LIST += $(wildcard $(LOCAL_PATH)/platform/*.h)
MY_INCLUDE_LIST += $(wildcard $(LOCAL_PATH)/io/*.h)
MY_INCLUDE_LIST += $(wildcard $(LOCAL_PATH)/io/android/*.h)
MY_INCLUDE_LIST += $(wildcard $(LOCAL_PATH)/javascript/*.h)
MY_INCLUDE_LIST += $(wildcard $(LOCAL_PATH)/javascript/android/*.h)
MY_INCLUDE_LIST += $(wildcard $(LOCAL_PATH)/javascript/v8/*.h)
MY_INCLUDE_LIST += $(wildcard $(LOCAL_PATH)/thread/*.h)
MY_INCLUDE_LIST += $(wildcard $(LOCAL_PATH)/thread/android/*.h)


# Source files to be compiled
LOCAL_SRC_FILES := $(patsubst $(LOCAL_PATH)/%, %, $(MY_SRC_LIST))
# Header files to be included
LOCAL_INCLUDE_FILES := $(patsubst $(LOCAL_PATH)/%, %, $(MY_INCLUDE_LIST))

LOCAL_LDLIBS+= -L$(SYSROOT)/usr/lib -llog -landroid -ljnigraphics

LOCAL_STATIC_LIBRARIES := \
    libv8 \
    libskia

#LOCAL_SHARED_LIBRARIES := \
#    libskia_atg \
#    libjsc

include $(BUILD_SHARED_LIBRARY)

$(call import-add-path, $(LOCAL_PATH))
$(call import-module, third_party/skia)
$(call import-module, third_party/v8)
