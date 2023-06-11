//
// Created by adeon on 6/7/23.
//
#include <iostream>

#include "chunk.hpp"

namespace rend {
    // TODO: Meshed at init
    void Chunk::init(const glm::vec3 &pos) {
        m_positionBL = pos;
        m_initialized = true;
        m_toBeMeshed = false;
    }

    void Chunk::setBlock(const glm::ivec3 &pos, rend::Chunk::BlockID blockId) {
        m_data[pos.x + pos.y * WIDTH_X + pos.z * WIDTH_X * HEIGHT_Y] = blockId;
        m_toBeMeshed = true;
    }

    void Chunk::setBlockByPlayer(const glm::ivec3 &pos, rend::Chunk::BlockID blockId) {
        m_data[pos.x + pos.y * WIDTH_X + pos.z * WIDTH_X * HEIGHT_Y] = blockId;
        m_toBeMeshed = true;
        if (pos.x == 0 || pos.x == WIDTH_X - 1 ||
                pos.z == 0 || pos.z == DEPTH_Z - 1) {
            m_toBeMeshedByPlayer = true;
            m_lastPlayerChangedPosition = pos;
        }
    }

    BLOCKS Chunk::operator[](const glm::ivec3 &pos) const {
        // Yes, I too have no fucking clue how to math here
        return static_cast<BLOCKS>(m_data[pos.x + pos.y * WIDTH_X + pos.z * WIDTH_X * HEIGHT_Y]);
    }

    void Chunk::positionOf(glm::vec3 *posToFill, const glm::ivec3 &posInChunk) {
        (*posToFill).x = m_positionBL.x + BLOCK_SIZE * posInChunk.x;
        (*posToFill).y = m_positionBL.y + BLOCK_SIZE * posInChunk.y;
        (*posToFill).z = m_positionBL.z + BLOCK_SIZE * posInChunk.z;
    }

    uint32_t Chunk::getDirOfNeighborToBeChanged(std::array<util::Direction, 2>* dirs) {
        uint32_t dirsToChange = 0;
        if (m_lastPlayerChangedPosition.x == WIDTH_X - 1) {
            (*dirs)[0] = util::Direction{util::Direction::RIGHT};
            ++dirsToChange;
        } else if (m_lastPlayerChangedPosition.x == 0) {
            (*dirs)[0] = util::Direction{util::Direction::LEFT};
            ++dirsToChange;
        }

        if (m_lastPlayerChangedPosition.z == 0) {
            (*dirs)[1] = util::Direction{util::Direction::BACK};
            ++dirsToChange;
        } else if (m_lastPlayerChangedPosition.z == DEPTH_Z - 1) {
            (*dirs)[1] = util::Direction{util::Direction::FRONT};
            ++dirsToChange;
        }

        return dirsToChange;
    }

    bool Chunk::isBlockAir(const glm::ivec3 &posInChunk) const {
        if (isOutOfBounds(posInChunk)) { return true; }
        return operator[](posInChunk) == BLOCKS::AIR;
    }

    bool Chunk::isBlockNotFull(const glm::ivec3& posInChunk) const {
        BLOCKS b = operator[](posInChunk);
        return (b == BLOCKS::WATER || b == BLOCKS::LAVA || b == BLOCKS::SNOW_POWDER);
    }

    bool Chunk::isOutOfBounds(const glm::ivec3 &posInChunk) const {
        if (posInChunk.x < 0 || posInChunk.y < 0 || posInChunk.z < 0 ||
            posInChunk.x >= Chunk::WIDTH_X || posInChunk.y >= Chunk::HEIGHT_Y || posInChunk.z >= Chunk::DEPTH_Z) {
            return true;
        }
        return false;
    }

    Chunk::Block Chunk::getBlockDataFromGlobalPos(const glm::vec3 &globalPos) {
        Block out;

        glm::ivec3 localBlockPos = glm::floor((globalPos - m_positionBL) / BLOCK_SIZE);

        out.localChunkPos = localBlockPos;
        out.blockID = operator[](localBlockPos);

        return out;
    }
} // rend