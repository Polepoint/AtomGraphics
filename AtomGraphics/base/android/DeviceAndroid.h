//
// Created by neo on 2019/3/13.
//

#ifndef ATOMGRAPHICS_DEVICEANDROID_H
#define ATOMGRAPHICS_DEVICEANDROID_H

#include "base/Device.h"

namespace AtomGraphics {

class DeviceAndroid : public Device {

public:

    static void InitializePlatform();

private:

    static void SetDeviceScaleFactor(float deviceScaleFactor);

    friend class ScreenDisplayManager;
};

}


#endif //ATOMGRAPHICS_DEVICEANDROID_H
