//
// Created by neo on 2018/10/23.
//

#ifndef ATOMGRAPHICS_V8PLATFORM_H
#define ATOMGRAPHICS_V8PLATFORM_H

#include <v8/v8.h>

namespace AtomGraphics {

namespace JS {

class V8Platform {

public:
    static V8Platform *SharedPlatform();

    V8Platform();

    v8::Isolate *getIsolate() const;

private:

    v8::Isolate *m_isolate;
    std::unique_ptr<v8::Platform> m_platform;

    uint64_t AmountOfPhysicalMemory();

    uint64_t AmountOfVirtualMemory();

};

} // namespace JS

} // namespace GraphicsEnvironment


#endif //ATOMGRAPHICS_V8PLATFORM_H
