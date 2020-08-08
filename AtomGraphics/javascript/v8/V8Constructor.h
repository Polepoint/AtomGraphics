//
// Created by neo on 2019/4/17.
//

#ifndef ATOMGRAPHICS_V8CONSTRUCTOR_H
#define ATOMGRAPHICS_V8CONSTRUCTOR_H

#include <v8/v8.h>

namespace AtomGraphics {

class ConstructorMode {

public:
    enum Mode {
        kWrapExistingObject, kCreateNewObject
    };

    ConstructorMode(v8::Isolate *isolate);

    ~ConstructorMode();

    static bool Current(v8::Isolate *isolate);

private:
    v8::Isolate *isolate_;
    ConstructorMode::Mode previous_;
};

}

#endif //ATOMGRAPHICS_V8CONSTRUCTOR_H
