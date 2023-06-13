//
// Created by iamthewalrus on 6/11/23.
//

#ifndef COOL_GAME_AABB_HPP
#define COOL_GAME_AABB_HPP

#include <glm/glm.hpp>
#include <glm/fwd.hpp>

namespace math {

template<typename T>
class AABB {
public:
    using V = glm::vec<3, T, glm::defaultp>;
    using M = glm::mat<4, 4, T, glm::defaultp>;

    V min, max;

    AABB() = default;
    AABB(V max): min(0), max(max) {}
    AABB(V min, V max): min(min), max(max) {}

    static AABB unit() {
        return AABB{V{1}};
    }

    AABB transform(const M& m) const {
        return AABB<T>{m * min, m * max};
    }

    AABB translate(const V& v) const {
        return AABB<T>{min + v, max + v};
    }

    AABB scale(const V& v) const {
        const auto d = max - min;
        return AABB<T>{min, d * v};
    }

    AABB scale(const T& s) const {
        return scale(V{s});
    }

    AABB<T> scaleCenter(const V& v) const {
        const auto
                c = (min + max) / static_cast<T>(2),
                d = max - min,
                h = d / static_cast<T>(2),
                e = h * v;
        return AABB<T>{c - e, c + e};
    }

    AABB scaleCenter(const T& s) const {
        return scaleCenter(V(s));
    }

    AABB centerOn(const V& v, glm::bvec3 axes = glm::bvec3{1}) const {
        const auto
                d = max - min,
                h = d / static_cast<T>(2);

        return AABB<T>{
                glm::mix(min, v - h, glm::vec3(axes)),
                glm::mix(max, v + h, glm::vec3(axes))};
    }

    V center() const {
        return (min + max) / static_cast<T>(2);
    }

    V size() const {
        return max - min;
    }

    V depth(const AABB& other) const {
        const auto &a = *this, &b = other;
        V res, c_a = a.center(), c_b = b.center();

        for (size_t i = 0; i < 3; ++i) {
            res[i] = c_a[i] < c_b[i] ?
                    a.max[i] - b.min[i]
                    : b.max[i] - a.min[i];
        }

        return res;
    }

    bool collides(const AABB& other) const {
        return  min.x <= other.max.x &&
                max.x >= other.min.x &&
                min.y <= other.max.y &&
                max.y >= other.min.y &&
                min.z <= other.max.z &&
                max.z >= other.min.z;
    }

    bool contains(const V& point) const {
        return  point.x >= min.x &&
                point.x <= max.x &&
                point.y >= min.y &&
                point.y <= max.y &&
                point.z >= min.z &&
                point.z <= max.z;
    }

}; // AABB

using AABBf = AABB<float>;
using AABBi = AABB<int>;

} // math

#endif //COOL_GAME_AABB_HPP
