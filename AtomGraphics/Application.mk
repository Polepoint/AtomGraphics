APP_BUILD_SCRIPT := ./Android.mk

APP_STL := c++_static
APP_ABI := armeabi-v7a arm64-v8a x86
APP_PLATFORM := android-19

APP_CFLAGS += -Wall
APP_CPPFLAGS := -frtti -std=c++14 -fsigned-char -Wno-extern-c-compat

APP_MK_DIR := $(realpath $(dir $(lastword $(MAKEFILE_LIST))))

NDK_MODULE_PATH := $(APP_MK_DIR)/third_party

NDK_TOOLCHAIN_VERSION := clang
