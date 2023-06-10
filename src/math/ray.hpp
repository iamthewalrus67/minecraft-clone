//
// Created by iamthewalrus on 6/10/23.
//

#ifndef COOL_GAME_RAY_HPP
#define COOL_GAME_RAY_HPP

#include <glm/glm.hpp>
#include <optional>
#include <functional>
#include "misc.hpp"

namespace math {

class Ray {
private:
    glm::vec3 m_origin;
    glm::vec3 m_direction;
public:
    struct Intersection {
        glm::vec3 position;
        glm::vec3 direction;
    };

    Ray() = default;
    Ray(const glm::vec3& origin, const glm::vec3& direction): m_origin{origin}, m_direction{direction} {}

    std::optional<Intersection> intersectBlock(std::function<bool(glm::ivec3)> isBlockFound, float maxDistance);

};

} // math

#endif //COOL_GAME_RAY_HPP
