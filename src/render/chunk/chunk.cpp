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

    const Chunk::BlockID &Chunk::operator[](const glm::ivec3 &pos) const {
        // Yes, I too have no fucking clue how to math here
        return m_data[pos.x + pos.y * WIDTH_X + pos.z * WIDTH_X * HEIGHT_Y];
    }

    void Chunk::positionOf(glm::vec3 *posToFill, const glm::ivec3 &posInChunk) {
        (*posToFill).x = m_positionBL.x + BLOCK_SIZE * posInChunk.x;
        (*posToFill).y = m_positionBL.y + BLOCK_SIZE * posInChunk.y;
        (*posToFill).z = m_positionBL.z + BLOCK_SIZE * posInChunk.z;
    }

    bool Chunk::isBlockAir(const glm::ivec3 &posInChunk) const {
        if (posInChunk.x < 0 || posInChunk.y < 0 || posInChunk.z < 0 ||
            posInChunk.x >= Chunk::WIDTH_X || posInChunk.y >= Chunk::HEIGHT_Y || posInChunk.z >= Chunk::DEPTH_Z) {
            return true;
        }

        return operator[](posInChunk) == BLOCKS::AIR;
    }
} // rend