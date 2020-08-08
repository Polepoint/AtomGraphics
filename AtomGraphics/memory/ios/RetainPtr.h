//
// Created by neo on 2018/11/13.
// Copyright (c) 2018 neo. All rights reserved.
//

#ifndef ATOMGRAPHICS_RETAINPTR_H
#define ATOMGRAPHICS_RETAINPTR_H


#include <algorithm>
#include <cstddef>
#include <CoreFoundation/CoreFoundation.h>

#ifdef __OBJC__

#import <Foundation/Foundation.h>

#endif

namespace AtomGraphics {

template<typename T>
class RetainPtr;

template<typename T>
RetainPtr<T> adoptCF(T CF_RELEASES_ARGUMENT);

template<typename T>
RetainPtr<T> adoptNS(T NS_RELEASES_ARGUMENT);

template<typename T>
class RetainPtr {
public:
    typedef typename std::remove_pointer<T>::type ValueType;
    typedef ValueType *PtrType;
    typedef CFTypeRef StorageType;

    RetainPtr() : m_ptr(nullptr) {}

    RetainPtr(PtrType ptr) : m_ptr(toStorageType(ptr)) { if (m_ptr) CFRetain(m_ptr); }

    RetainPtr(const RetainPtr &o) : m_ptr(o.m_ptr) { if (StorageType ptr = m_ptr) CFRetain(ptr); }

    RetainPtr(RetainPtr &&o) : m_ptr(toStorageType(o.leakRef())) {}

    template<typename U>
    RetainPtr(RetainPtr<U> &&o) : m_ptr(toStorageType(o.leakRef())) {}

    bool isHashTableDeletedValue() const { return m_ptr == hashTableDeletedValue(); }

    ~RetainPtr();

    template<typename U>
    RetainPtr(const RetainPtr<U> &);

    void clear();

    PtrType leakRef();

    PtrType autorelease();

#ifdef __OBJC__

    id bridgingAutorelease();

#endif

    PtrType get() const { return fromStorageType(m_ptr); }

    PtrType operator->() const { return fromStorageType(m_ptr); }

    explicit operator PtrType() const { return fromStorageType(m_ptr); }

    explicit operator bool() const { return m_ptr; }

    bool operator!() const { return !m_ptr; }

    // This conversion operator allows implicit conversion to bool but not to other integer types.
    typedef StorageType RetainPtr::*UnspecifiedBoolType;

    operator UnspecifiedBoolType() const { return m_ptr ? &RetainPtr::m_ptr : nullptr; }

    RetainPtr &operator=(const RetainPtr &);

    template<typename U>
    RetainPtr &operator=(const RetainPtr<U> &);

    RetainPtr &operator=(PtrType);

    template<typename U>
    RetainPtr &operator=(U *);

    RetainPtr &operator=(RetainPtr &&);

    template<typename U>
    RetainPtr &operator=(RetainPtr<U> &&);

    void swap(RetainPtr &);

    template<typename U>
    friend RetainPtr<U> adoptCF(U CF_RELEASES_ARGUMENT);

    template<typename U>
    friend RetainPtr<U> adoptNS(U NS_RELEASES_ARGUMENT);

private:
    enum AdoptTag {
        Adopt
    };

    RetainPtr(PtrType ptr, AdoptTag) : m_ptr(toStorageType(ptr)) {}

    static PtrType hashTableDeletedValue() { return reinterpret_cast<PtrType>(-1); }

#ifdef __OBJC__

    template<typename U>
    typename std::enable_if<std::is_convertible<U, id>::value, PtrType>::type
    fromStorageTypeHelper(StorageType ptr) const {
        return (__bridge PtrType) const_cast<CF_BRIDGED_TYPE(id) void *>(ptr);
    }

    template<typename U>
    typename std::enable_if<!std::is_convertible<U, id>::value, PtrType>::type
    fromStorageTypeHelper(StorageType ptr) const {
        return (PtrType) const_cast<CF_BRIDGED_TYPE(id) void *>(ptr);
    }

    PtrType fromStorageType(StorageType ptr) const { return fromStorageTypeHelper<PtrType>(ptr); }

    StorageType toStorageType(id ptr) const { return (__bridge StorageType) ptr; }

    StorageType toStorageType(CFTypeRef ptr) const { return (StorageType) ptr; }

#else
    PtrType fromStorageType(StorageType ptr) const
    {
        return (PtrType)const_cast<CF_BRIDGED_TYPE(id) void*>(ptr);
    }
    StorageType toStorageType(PtrType ptr) const { return (StorageType)ptr; }
#endif

#ifdef __OBJC__

    template<typename U>
    std::enable_if_t<std::is_convertible<U, id>::value, PtrType> autoreleaseHelper();

    template<typename U>
    std::enable_if_t<!std::is_convertible<U, id>::value, PtrType> autoreleaseHelper();

#endif

    StorageType m_ptr;
};

// Helper function for creating a RetainPtr using template argument deduction.
template<typename T>
RetainPtr<T> retainPtr(T);

template<typename T>
inline RetainPtr<T>::~RetainPtr() {
    if (StorageType ptr = std::exchange(m_ptr, nullptr))
        CFRelease(ptr);
}

template<typename T>
template<typename U>
inline RetainPtr<T>::RetainPtr(const RetainPtr<U> &o)
        : m_ptr(toStorageType(o.get())) {
    if (StorageType ptr = m_ptr)
        CFRetain(ptr);
}

template<typename T>
inline void RetainPtr<T>::clear() {
    if (StorageType ptr = std::exchange(m_ptr, nullptr))
        CFRelease(ptr);
}

template<typename T>
inline auto RetainPtr<T>::leakRef() -> PtrType {
    return fromStorageType(std::exchange(m_ptr, nullptr));
}

#ifndef __OBJC__

template<typename T> inline auto RetainPtr<T>::autorelease() -> PtrType
{
    if (m_ptr)
        CFAutorelease(m_ptr);
    return leakRef();
}

#else

template<typename T>
template<typename U>
inline auto RetainPtr<T>::autoreleaseHelper() -> std::enable_if_t<std::is_convertible<U, id>::value, PtrType> {
    return CFBridgingRelease(std::exchange(m_ptr, nullptr));
}

template<typename T>
template<typename U>
inline auto RetainPtr<T>::autoreleaseHelper() -> std::enable_if_t<!std::is_convertible<U, id>::value, PtrType> {
    if (m_ptr)
        CFAutorelease(m_ptr);
    return leakRef();
}

template<typename T>
inline auto RetainPtr<T>::autorelease() -> PtrType {
    return autoreleaseHelper<PtrType>();
}

// FIXME: It would be nice if we could base the return type on the type that is toll-free bridged with T rather than using id.
template<typename T>
inline id RetainPtr<T>::bridgingAutorelease() {
    static_assert((!std::is_convertible<PtrType, id>::value), "Don't use bridgingAutorelease for Objective-C pointer types.");
    return CFBridgingRelease(leakRef());
}

#endif

template<typename T>
inline RetainPtr<T> &RetainPtr<T>::operator=(const RetainPtr &o) {
    RetainPtr ptr = o;
    swap(ptr);
    return *this;
}

template<typename T>
template<typename U>
inline RetainPtr<T> &RetainPtr<T>::operator=(const RetainPtr<U> &o) {
    RetainPtr ptr = o;
    swap(ptr);
    return *this;
}

template<typename T>
inline RetainPtr<T> &RetainPtr<T>::operator=(PtrType optr) {
    RetainPtr ptr = optr;
    swap(ptr);
    return *this;
}

template<typename T>
template<typename U>
inline RetainPtr<T> &RetainPtr<T>::operator=(U *optr) {
    RetainPtr ptr = optr;
    swap(ptr);
    return *this;
}

template<typename T>
inline RetainPtr<T> &RetainPtr<T>::operator=(RetainPtr &&o) {
    RetainPtr ptr = std::move(o);
    swap(ptr);
    return *this;
}

template<typename T>
template<typename U>
inline RetainPtr<T> &RetainPtr<T>::operator=(RetainPtr<U> &&o) {
    RetainPtr ptr = std::move(o);
    swap(ptr);
    return *this;
}

template<typename T>
inline void RetainPtr<T>::swap(RetainPtr &o) {
    std::swap(m_ptr, o.m_ptr);
}

template<typename T>
inline void swap(RetainPtr<T> &a, RetainPtr<T> &b) {
    a.swap(b);
}

template<typename T, typename U>
inline bool operator==(const RetainPtr<T> &a, const RetainPtr<U> &b) {
    return a.get() == b.get();
}

template<typename T, typename U>
inline bool operator==(const RetainPtr<T> &a, U *b) {
    return a.get() == b;
}

template<typename T, typename U>
inline bool operator==(T *a, const RetainPtr<U> &b) {
    return a == b.get();
}

template<typename T, typename U>
inline bool operator!=(const RetainPtr<T> &a, const RetainPtr<U> &b) {
    return a.get() != b.get();
}

template<typename T, typename U>
inline bool operator!=(const RetainPtr<T> &a, U *b) {
    return a.get() != b;
}

template<typename T, typename U>
inline bool operator!=(T *a, const RetainPtr<U> &b) {
    return a != b.get();
}

template<typename T>
inline RetainPtr<T> adoptCF(T CF_RELEASES_ARGUMENT ptr) {
#ifdef __OBJC__
    static_assert((!std::is_convertible<T, id>::value), "Don't use adoptCF with Objective-C pointer types, use adoptNS.");
#endif
    return RetainPtr<T>(ptr, RetainPtr<T>::Adopt);
}

#ifdef __OBJC__

template<typename T>
inline RetainPtr<T> adoptNS(T NS_RELEASES_ARGUMENT ptr) {
#if __has_feature(objc_arc)
    return ptr;
#elif defined(OBJC_NO_GC)
    return RetainPtr<T>(ptr, RetainPtr<T>::Adopt);
#else
    RetainPtr<T> result = ptr;
    [ptr release];
    return result;
#endif
}

#endif

template<typename T>
inline RetainPtr<T> retainPtr(T ptr) {
    return ptr;
}
//
//template<typename T>
//struct IsSmartPtr<RetainPtr<T>> {
//    static const bool value = true;
//};
//
//template<typename P>
//struct HashTraits<RetainPtr<P>> : SimpleClassHashTraits<RetainPtr<P>> {
//};
//
//template<typename P>
//struct DefaultHash<RetainPtr<P>> {
//    typedef PtrHash <RetainPtr<P>> Hash;
//};
//
//template<typename P>
//struct RetainPtrObjectHashTraits : SimpleClassHashTraits<RetainPtr<P>> {
//    static const RetainPtr<P> &emptyValue() {
//        static RetainPtr<P> &null = *(new RetainPtr<P>);
//        return null;
//    }
//};

template<typename P>
struct RetainPtrObjectHash {
    static unsigned hash(const RetainPtr<P> &o) {
        return static_cast<unsigned>(CFHash(o.get()));
    }

    static bool equal(const RetainPtr<P> &a, const RetainPtr<P> &b) {
        return CFEqual(a.get(), b.get());
    }

    static const bool safeToCompareToEmptyOrDeleted = false;
};

#ifdef __OBJC__

template<typename T>
T *dynamic_objc_cast(id object) {
    if ([object isKindOfClass:[T class]])
        return (T *) object;

    return nil;
}

#endif

}

#endif //ATOMGRAPHICS_RETAINPTR_H
