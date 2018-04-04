#include "math/Quaternion.h"

namespace AtomGraphics {

    inline Quaternion Quaternion::operator*(const Quaternion &q) const {
        Quaternion result(*this);
        result.multiply(q);
        return result;
    }

    inline Quaternion &Quaternion::operator*=(const Quaternion &q) {
        multiply(q);
        return *this;
    }

    inline Vec3 Quaternion::operator*(const Vec3 &v) const {
        Vec3 uv, uuv;
        Vec3 qvec(x, y, z);
        Vec3::cross(qvec, v, &uv);
        Vec3::cross(qvec, uv, &uuv);

        uv *= (2.0f * w);
        uuv *= 2.0f;

        return v + uv + uuv;
    }

}