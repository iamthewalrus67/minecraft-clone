
#ifndef COOL_GAME_CHUNK_HPP
#define COOL_GAME_CHUNK_HPP

#include <array>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <bgfx/bgfx.h>

#include "blocks.hpp"

/*  CHUNK
 *  1-------2
 *  |5------+6
 *  ||      ||
 *  ||      ||
 *  0+------3|
 *   4-------7
 *   The chunk position is the point "0" in this "image"
 *   The block position inside the chunk is basically the same.
 *   So the point "0" has the lowest position in terms of coordinate values
 */

namespace rend {
    class Chunk {
        using BlockID = uint16_t;
    public:
        static constexpr uint32_t WIDTH_X = 32;
        static constexpr uint32_t HEIGHT_Y = 64;
        static constexpr uint32_t DEPTH_Z = 32;
        static constexpr float BLOCK_SIZE = 2.0f;

        Chunk(const glm::vec3& pos): m_positionBL{pos} {}

        //! Get the BlockID ref at the positiov provided by the ivec
        BlockID& operator[](const glm::ivec3& pos);
        //! Get the actual position of a singular block based on chunk position
        void positionOf(glm::vec3* posToFill, const glm::ivec3& posInChunk);
    private:
        std::array<BlockID, WIDTH_X * HEIGHT_Y * DEPTH_Z> m_data;
        glm::vec3 m_positionBL;
    };


    //! Just a test method for initing a test chunk
    inline void initTestChunk(Chunk* chunkPtr) {
        for (uint32_t w = 0; w < Chunk::WIDTH_X; ++w) {
            for (uint32_t h = 0; h < Chunk::HEIGHT_Y; ++h) {
                for (uint32_t d = 0; d < Chunk::DEPTH_Z; ++d) {
                    (*chunkPtr)[glm::vec3{w, h, d}] = (h < Chunk::HEIGHT_Y / 2) ? (BLOCKS::GRASS): (BLOCKS::AIR);
                }
            }
        }
    }
} // rend

#endif //COOL_GAME_CHUNK_HPP
