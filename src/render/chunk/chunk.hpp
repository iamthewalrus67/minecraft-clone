
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
        //! A local to chunk block struct
        struct Block {
            glm::ivec3 localChunkPos;
            BLOCKS blockID;
        };

        static constexpr uint32_t WIDTH_X = 16;
        static constexpr uint32_t HEIGHT_Y = 128;
        static constexpr uint32_t DEPTH_Z = 16;
        static constexpr float BLOCK_SIZE = 1.0f;

        Chunk(): m_initialized{false}, m_toBeMeshed{false}, m_toBeMeshedByPlayer{false} {}

        //! Initialize the chunk
        void init(const glm::vec3& pos);

        //! Get the BlockID ref at the positiov provided by the ivec
        [[nodiscard]] BLOCKS operator[](const glm::ivec3 &pos) const;
        //! Set the block at the blockID(so the chunk can log that is was changed), player has a separate function
        void setBlock(const glm::ivec3 &pos, BlockID blockId);

        //! The same as set block but logs the past changed block potion so that we can reload the needed neighbor chunk
        //! If change is on the edge
        void setBlockByPlayer(const glm::ivec3 &pos, BlockID blockId);
        //! Get the actual position of a singular block based on chunk position
        void positionOf(glm::vec3* posToFill, const glm::ivec3& posInChunk);

        //! If waitForReMeshByPlayer is true, return the direction of the neighbor chunk to change
        //! The maximum of 2 dirs can be filled
        uint32_t getDirOfNeighborToBeChanged(std::array<util::Direction, 2>* dirs);

        //! Get the block at the specified globalPosition
        [[nodiscard]] Block getBlockDataFromGlobalPos(const glm::vec3& globalPos);

        //! True if block has transparency
        [[nodiscard]] bool isBlockTransparent(const glm::ivec3& posInChunk) const;
        [[nodiscard]] bool isBlockAir(const glm::ivec3& posInChunk) const;
        //! Returns true if the block does not take up entire block size, LIKE WATER LAVA, NOT AIR!!!!!
        [[nodiscard]] bool isBlockNotFull(const glm::ivec3& posInChunk) const;
        [[nodiscard]] bool isOutOfBounds(const glm::ivec3& posInChunk) const;
        [[nodiscard]] bool isInitialized() const { return m_initialized; }
        //! Check if chunk was changed so we now whether it should be remeshed
        [[nodiscard]] bool waitForReMesh() const { return m_toBeMeshed; }
        //! If the last remesh change was by player, this will return true, so we can call getDirOfNeighborToBeChanged
        //! And change the needed neighbor chunk
        [[nodiscard]] bool waitForReMeshByPlayer() const {
            return m_toBeMeshedByPlayer;
        }
        //! Gets the position of the chunk
        [[nodiscard]] glm::vec3 getChunkGlobalPos() { return m_positionBL; }
        //! Tell the chunk that ts was remeshed
        void logReMesh() {
            m_toBeMeshed = false;
            m_toBeMeshedByPlayer = false;
        }
        void setToReMesh() { m_toBeMeshed = true; }
    private:
        std::array<BlockID, WIDTH_X * HEIGHT_Y * DEPTH_Z> m_data{};
        glm::vec3 m_positionBL;
        // The position that the player changed last in chunk
        glm::ivec3 m_lastPlayerChangedPosition;
        bool m_initialized;
        bool m_toBeMeshed;
        bool m_toBeMeshedByPlayer;
    };

    //! Just a test method for initing a test chunk
    inline void initTestChunk(Chunk* chunkPtr) {
        for (uint32_t w = 0; w < Chunk::WIDTH_X; ++w) {
            for (uint32_t h = 0; h < Chunk::HEIGHT_Y; ++h) {
                for (uint32_t d = 0; d < Chunk::DEPTH_Z; ++d) {
                    (*chunkPtr).setBlock(glm::vec3{w, h, d}, (h < Chunk::HEIGHT_Y / 2) ? (w + h + d) % 3 + 1: 0);
                }
            }
        }
    }
} // rend

#endif //COOL_GAME_CHUNK_HPP
