//
// Created by neo on 2019/3/13.
//

#include "android/ScreenDisplayManager.h"
#include "DeviceAndroid.h"

namespace AtomGraphics {

void DeviceAndroid::InitializePlatform() {
    static bool initialized;
    if (initialized) {
        return;
    }

    initialized = true;

    _deviceScaleFactor = ScreenDisplayManager::Singleton()->getPrimaryDisplay().getDeviceScaleFactor();
}

void DeviceAndroid::SetDeviceScaleFactor(float deviceScaleFactor) {
    _deviceScaleFactor = deviceScaleFactor;
}

}