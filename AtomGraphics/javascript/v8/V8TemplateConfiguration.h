//
// Created by neo on 2019/4/17.
//

#ifndef ATOMGRAPHICS_V8DOMTEMPLATECONFIGURATION_H
#define ATOMGRAPHICS_V8DOMTEMPLATECONFIGURATION_H

#include <v8/v8.h>

namespace AtomGraphics {

class V8TemplateConfiguration {

public:
    static void InitializeInterfaceTemplate(v8::Isolate *isolate,
                                            v8::Local<v8::FunctionTemplate> interface_template,
                                            const char *interface_name,
                                            uint32_t v8_internal_field_count);

private:
    static void SetClassString(v8::Isolate *, v8::Local<v8::ObjectTemplate>, const char *class_string);
};

}


#endif //ATOMGRAPHICS_V8DOMTEMPLATECONFIGURATION_H
