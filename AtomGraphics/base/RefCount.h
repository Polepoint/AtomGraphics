#ifndef REFCOUNT_H_
#define REFCOUNT_H_

namespace gcanvas
{
/// <description>
/// RefCount
/// </description>
class RefCount
{
public:
    /// <description>
    /// TODO
    /// </description>
    RefCount() : mRefCount(0) {}

    /// <description>
    /// TODO
    /// </description>
    void AddRef() { ++mRefCount; }

    /// <description>
    /// TODO
    /// </description>
    /// <remarks>
    /// TODO
    /// </remarks>
    void ReleaseRef()
    {
        if (--mRefCount <= 0) delete this;
    }

protected:
    /// <description>
    /// TODO
    /// </description>
    /// <remarks>
    /// TODO
    /// </remarks>
    virtual ~RefCount() {}

protected:
    unsigned int mRefCount;
};
}

#endif
