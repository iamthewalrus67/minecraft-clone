//
// Created by adeon on 6/8/23.
//
#include <iostream>

#include "chunk_manager.hpp"

namespace rend {
    Chunk &ChunkManager::addChunk(const glm::ivec3 &chunkPos) {
        auto& chunkRenderer = m_chunkData[chunkPos];
        chunkRenderer.init(chunkPos);

        return chunkRenderer.getChunkRef();
    }

    bool ChunkManager::removeChunk(const glm::ivec3 &chunkPos) {
        auto it = m_chunkData.find(chunkPos);
        if (it == m_chunkData.end()) { return false; }
        m_chunkData.erase(it);
        (*it).second.terminate();
        return true;
    }

    void ChunkManager::reMeshChunks() {
        for (auto& chunkRenderer: m_chunkData) {
            auto& chunk = chunkRenderer.second.getChunkRef();
            if (chunk.waitForReMesh()) {
                std::array<Chunk*, 6> neighborChunks;
                fillNeighborChunksFromPos(&neighborChunks, chunk.getChunkGlobalPos());
                chunkRenderer.second.meshChunk(neighborChunks);
            }
        }
    }

    Chunk *ChunkManager::getChunkRefFromGlobalPos(const glm::vec3 &pos) {
        glm::ivec3 aliquotPos = glm::ivec3{
                static_cast<int>(pos.x) - static_cast<int>(pos.x) % Chunk::WIDTH_X,
                static_cast<int>(pos.y) - static_cast<int>(pos.y) % Chunk::HEIGHT_Y,
                static_cast<int>(pos.z) - static_cast<int>(pos.z) % Chunk::DEPTH_Z,
        };

        try {
            return &(m_chunkData.at(aliquotPos).getChunkRef());
        } catch (...) {
            return nullptr;
        }
    }

    Chunk *ChunkManager::getChunkRefFromAliquotPos(const glm::vec3 &pos) {
        try {
            return &(m_chunkData.at(pos).getChunkRef());
        } catch (...) {
            return nullptr;
        }
    }

    void ChunkManager::fillNeighborChunksFromPos(std::array<Chunk *, 6> *neighbors, const glm::ivec3 &globalChunkPos) {
        // Note: we don't need UP DOWN CHECKS
        // IF YPU WANT TO CHECK UP/DOWN add 5 and 6
        for (uint32_t i = 0; i < 4; ++i) {
            int dim = (i < 2) ? Chunk::DEPTH_Z: Chunk::WIDTH_X;
            util::Direction dir{static_cast<util::Direction::INDEX>(i)};
            (*neighbors)[i] = getChunkRefFromAliquotPos(globalChunkPos + dir.toGlmIVec3() * dim);
        }
        (*neighbors)[4] = nullptr;
        (*neighbors)[5] = nullptr;
    }

    void ChunkManager::terminate() {
        for (auto& cR: m_chunkData) {
            cR.second.terminate();
        }
    }
} // rend