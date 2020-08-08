//
// Created by neo on 2019/4/16.
//

#ifndef ATOMGRAPHICS_SCRIPTWRAPPER_H
#define ATOMGRAPHICS_SCRIPTWRAPPER_H

#include <v8/v8.h>
#include <map>
#include "WrapperCache.h"
#include "WrapperTypeInfo.h"
#include "V8GCMarkingVisitor.h"

namespace AtomGraphics {

constexpr uint32_t kV8WrapperTraceMarkBitMask = 1 << 0;
constexpr uint32_t kWrappableTraceMarkBitMask = 1 << 1;

class ScriptWrappable {

public:

    ScriptWrappable();

    virtual ~ScriptWrappable() {
        wrapper_.Reset();
    };

    v8::Local<v8::Object> Wrap(v8::Isolate *isolate, v8::Local<v8::Context> context);

    v8::Local<v8::Object> CreateWrapperFromCache(v8::Isolate *isolate,
                                                 v8::Local<v8::Context> context,
                                                 const WrapperTypeInfo *wrapperTypeInfo);

    virtual const WrapperTypeInfo *getWrapperTypeInfo() const = 0;

    virtual v8::Local<v8::Object> AssociateWithWrapper(
            v8::Isolate *,
            const WrapperTypeInfo *,
            v8::Local<v8::Object> wrapper);

    void SetNativeInfo(v8::Local<v8::Object> &wrapper, const WrapperTypeInfo *wrapper_type_info);

    template<typename T>
    static bool CheckWrapperInfoMatch(v8::Local<v8::Object> object) {
        return ToScriptWrappable(object)->getWrapperTypeInfo()->interface_name ==
                T::wrapperTypeInfo.interface_name;
    }

    template<typename T>
    T *ToImpl() {
        return static_cast<T *>(this);
    }

    virtual void Trace(V8GCMarkingVisitor *) = 0;

    bool isV8WrapperMarked() {
        return m_markFlags & kV8WrapperTraceMarkBitMask;
    }

    void markV8Wrapper() {
        m_markFlags |= kV8WrapperTraceMarkBitMask;
    }

    void unmarkV8Wrapper() {
        m_markFlags &= ~kV8WrapperTraceMarkBitMask;
    }

    bool isWrappableMarked() {
        return m_markFlags & kWrappableTraceMarkBitMask;
    }

    void markWrappable() {
        m_markFlags |= kWrappableTraceMarkBitMask;
    }

    void unmarkWrappble() {
        m_markFlags &= ~kWrappableTraceMarkBitMask;
    }

    const v8::Persistent<v8::Object> &wrapper() const { return wrapper_; }

protected:

    static bool HasInstance(v8::Local<v8::Value> value, v8::Isolate *isolate,
                            const WrapperTypeInfo *wrapperTypeInfo);

private:

    v8::Persistent<v8::Object> wrapper_;
    uint32_t m_markFlags{0};

    bool SetWrapper(v8::Isolate *isolate, v8::Local<v8::Object> &wrapper,
                    const WrapperTypeInfo *wrapper_type_info);

    static WrapperCache *SharedWrapperCache();

    static v8::Local<v8::Function> ConstructorForType(v8::Isolate *isolate,
                                                      v8::Local<v8::Context> context,
                                                      const WrapperTypeInfo *wrapperTypeInfo);
};

}


#endif //ATOMGRAPHICS_SCRIPTWRAPPER_H
