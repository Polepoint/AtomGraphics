//
// Created by neo on 2018/11/22.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_DEVICE_H
#define ATOMGRAPHICS_DEVICE_H

namespace AtomGraphics {

class Device {

public:
    
    static float DeviceScaleFactor();

protected:
    
    static float _deviceScaleFactor;
};

}


#endif //ATOMGRAPHICS_DEVICE_H
