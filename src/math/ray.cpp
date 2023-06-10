//
// Created by iamthewalrus on 6/10/23.
//

#include "math/ray.hpp"
#include <glm/gtx/norm.hpp>

namespace math {

std::optional <Ray::Intersection>
Ray::intersectBlock(std::function<bool(glm::ivec3)> isBlockFound, float maxDistance) {
    glm::ivec3 d{0};
    glm::ivec3 p, step;
    glm::vec3 tMax, tDelta;
    float radius;

    p = glm::floor(m_origin);
    step = glm::sign(m_direction);
    tMax = intbound(m_origin, m_direction);
    tDelta = glm::vec3(step) / m_direction;
    radius = maxDistance / glm::l2Norm(m_direction);

    for (;;) {
        if (isBlockFound(p)) {
            return std::make_optional<Intersection>(p, d);
        }

        if (tMax.x < tMax.y) {
            if (tMax.x < tMax.z) {
                if (tMax.x > radius) {
                    break;
                }

                p.x += step.x;
                tMax.x += tDelta.x;
                d = glm::ivec3(-step.x, 0, 0);
            } else {
                if (tMax.z > radius) {
                    break;
                }

                p.z += step.z;
                tMax.z += tDelta.z;
                d = glm::ivec3(0, 0, -step.z);
            }
        } else {
            if (tMax.y < tMax.z) {
                if (tMax.y > radius) {
                    break;
                }

                p.y += step.y;
                tMax.y += tDelta.y;
                d = glm::ivec3(0, -step.y, 0);
            } else {
                if (tMax.z > radius) {
                    break;
                }

                p.z += step.z;
                tMax.z += tDelta.z;
                d = glm::ivec3(0, 0, -step.z);
            }
        }
    }

    return std::nullopt;
}

} // math