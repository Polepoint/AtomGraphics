//
// Created by neo on 2019/4/18.
//

#ifndef ATOMGRAPHICS_WRAPPERCACHE_H
#define ATOMGRAPHICS_WRAPPERCACHE_H

#include <map>
#include <v8/v8.h>
#include "WrapperTypeInfo.h"

namespace AtomGraphics {

class WrapperCache {

public:

    v8::Local<v8::Object> get(v8::Isolate *isolate, const WrapperTypeInfo *wrapperTypeInfo);

    void set(v8::Isolate *isolate, const WrapperTypeInfo *wrapperTypeInfo, v8::Local<v8::Object> instance_template);

private:

    struct GlobalValueReference {
        v8::Global<v8::Object> value_;
    };

    std::map<const WrapperTypeInfo *, GlobalValueReference *> wrapper_map_cache_;

};


}


#endif //ATOMGRAPHICS_WRAPPERCACHE_H
