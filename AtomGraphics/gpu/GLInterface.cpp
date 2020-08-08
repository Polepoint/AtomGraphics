//
// Created by neo on 2019/5/25.
//

#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include "base/AtomLog.h"
#include "GLInterface.h"


namespace AtomGraphics {

sk_sp<const GrGLInterface> GLInterface::MakeDebugInterface(bool useGLES2) {
    auto itf = GrGLMakeNativeInterface();
    GrGLInterface *glInterface = const_cast<GrGLInterface *>(itf.get());

    // Depending on the advertised version and extensions, skia checks for
    // existence of entrypoints. However some of those we don't yet handle in
    // gl_bindings, so we need to fake the version to the maximum fully supported
    // by the bindings (GL 4.1 or ES 3.0), and blacklist extensions that skia
    // handles but bindings don't.
    if (useGLES2) {
        const char *fake_version = "OpenGL ES 2.0";
        glInterface->fFunctions.fGetString = [fake_version](GLenum name) {
            if (name == GL_VERSION) {
                return reinterpret_cast<const GLubyte *>(fake_version);
            }
            return glGetString(name);
        };
    }

    return itf;
}

}