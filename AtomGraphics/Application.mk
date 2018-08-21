APP_BUILD_SCRIPT := ./Android.mk

APP_STL := gnustl_shared
APP_ABI := armeabi-v7a x86
APP_PLATFORM := android-9

APP_CFLAGS += -Wall
APP_CPPFLAGS := -frtti -std=c++14 -fsigned-char -Wno-extern-c-compat
APP_LDFLAGS := -latomic

APP_MK_DIR := $(realpath $(dir $(lastword $(MAKEFILE_LIST))))

NDK_MODULE_PATH := $(APP_MK_DIR)/third_party$(HOST_DIRSEP)$(APP_MK_DIR)/../GCanvas

NDK_TOOLCHAIN_VERSION := clang
