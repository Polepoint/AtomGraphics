//
// Created by neo on 2018/11/23.
// Copyright (c) 2018 neo. All rights reserved.
//


#include "DeviceIOS.h"
#include <UIKit/UIScreen.h>

namespace AtomGraphics {

void DeviceIOS::InitializePlatform() {
    static bool initialized;
    if (initialized) {
        return;
    }
    
    initialized = true;
    
    _deviceScaleFactor = [UIScreen mainScreen].scale;
}

}
