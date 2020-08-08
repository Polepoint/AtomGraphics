//
// Created by neo on 2018/11/22.
// Copyright (c) 2018 neo. All rights reserved.
//

#include "Device.h"

namespace AtomGraphics {

float Device::_deviceScaleFactor = 1;

float Device::DeviceScaleFactor() {
    return _deviceScaleFactor;
}

}