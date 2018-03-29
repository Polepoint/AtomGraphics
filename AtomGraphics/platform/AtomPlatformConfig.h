//
// Created by neo on 2018/3/26.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMPLATFORMCONFIG_H
#define ATOMPLATFORMCONFIG_H

#define ATOM_PLATFORM_IOS                1
#define ATOM_PLATFORM_ANDROID            2

#if defined(__APPLE__) && !defined(ANDROID)

#include <TargetConditionals.h>

#if TARGET_OS_IPHONE // TARGET_OS_IPHONE includes TARGET_OS_IOS TARGET_OS_TV and TARGET_OS_WATCH. see TargetConditionals.h
#undef  ATOM_TARGET_PLATFORM
#define ATOM_TARGET_PLATFORM         ATOM_PLATFORM_IOS
#endif

#endif

#if defined(ANDROID)
#undef  ATOM_TARGET_PLATFORM
#define ATOM_TARGET_PLATFORM         ATOM_PLATFORM_ANDROID
#endif

#endif //ATOMPLATFORMCONFIG_H
