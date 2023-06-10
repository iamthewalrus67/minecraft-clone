//
// Created by iamthewalrus on 6/10/23.
//

#ifndef COOL_GAME_MISC_HPP
#define COOL_GAME_MISC_HPP

#include <glm/glm.hpp>

// find the smallest positive t such that s + t * ds is an integer
inline glm::vec3 intbound(glm::vec3 s, glm::vec3 ds) {
    glm::vec3 res;
    for (size_t i = 0; i < 3; i++) {
        res[i] =
                (ds[i] > 0 ?
                 (glm::ceil(s[i]) - s[i])
                           : (s[i] - glm::floor(s[i])))
                / glm::abs(ds[i]);
    }
    return res;
}

#endif //COOL_GAME_MISC_HPP
