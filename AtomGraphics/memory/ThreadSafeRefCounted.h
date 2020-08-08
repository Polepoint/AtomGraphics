//
// Created by neo on 2018/12/12.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_THREADSAFEREFCOUNTEDBASE_H
#define ATOMGRAPHICS_THREADSAFEREFCOUNTEDBASE_H

#include "AtomMacros.h"
#include "FastMalloc.h"
#include "thread/MainThread.h"
#include <atomic>

//namespace GraphicsEnvironment {
//
//class ThreadSafeRefCountedBase {
//AT_MAKE_NONCOPYABLE(ThreadSafeRefCountedBase);
//AT_MAKE_FAST_ALLOCATED;
//public:
//    ThreadSafeRefCountedBase() = default;
//
//    void ref() const {
//        ++m_refCount;
//    }
//
//    bool hasOneRef() const {
//        return refCount() == 1;
//    }
//
//    unsigned refCount() const {
//        return m_refCount;
//    }
//
//protected:
//    // Returns whether the pointer should be freed or not.
//    bool derefBase() const {
//        return !--m_refCount;
//    }
//
//private:
//    mutable std::atomic<unsigned> m_refCount{1};
//};
//
//enum class DestructionThread {
//    Any, Main
//};
//
//template<class T, DestructionThread destructionThread = DestructionThread::Any>
//class ThreadSafeRefCounted : public ThreadSafeRefCountedBase {
//public:
//    void deref() const {
//        if (!derefBase())
//            return;
//        if (destructionThread == DestructionThread::Any || isMainThread()) {
//            delete static_cast<const T *>(this);
//            return;
//        }
//        callOnMainThread([this] {
//            delete static_cast<const T *>(this);
//        });
//    }
//
//protected:
//    ThreadSafeRefCounted() = default;
//};
//
//} // namespace WTF
//
//using GraphicsEnvironment::ThreadSafeRefCounted;

#endif //ATOMGRAPHICS_THREADSAFEREFCOUNTEDBASE_H
