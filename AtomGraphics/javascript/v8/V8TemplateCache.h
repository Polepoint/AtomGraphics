//
// Created by neo on 2019/4/16.
//

#ifndef ATOMGRAPHICS_V8TEMPLATECACHE_H
#define ATOMGRAPHICS_V8TEMPLATECACHE_H

#include <v8/v8.h>
#include <map>
#include "WrapperTypeInfo.h"

namespace AtomGraphics {

using InstallTemplateFunction =
void (*)(v8::Isolate *isolate,
         v8::Local<v8::Context> context,
         v8::Local<v8::FunctionTemplate> interface_template);

class V8TemplateCache {

public:
    static v8::Local<v8::FunctionTemplate> ClassTemplate(v8::Isolate *isolate,
                                                         v8::Local<v8::Context> context,
                                                         const WrapperTypeInfo *wrapper_type_info,
                                                         InstallTemplateFunction configure_dom_class_template);

    static v8::Local<v8::FunctionTemplate> FindInterfaceTemplate(v8::Isolate *isolate,
                                                                 const WrapperTypeInfo *typeInfo);

private:

    static V8TemplateCache *SharedCache();

    static void CacheInterfaceTemplate(v8::Isolate *isolate, const WrapperTypeInfo *typeInfo,
                                       v8::Local<v8::FunctionTemplate> value);

    std::map<const WrapperTypeInfo *, v8::Eternal<v8::FunctionTemplate>> m_templateMap;
};

}


#endif //ATOMGRAPHICS_V8TEMPLATECACHE_H
