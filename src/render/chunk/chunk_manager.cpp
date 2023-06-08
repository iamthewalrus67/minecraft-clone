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
        m_chunkData[chunkPos].terminate();
        std::cout << m_chunkData.erase(chunkPos) << std::endl;
    }

    void ChunkManager::terminate() {
        for (auto& cR: m_chunkData) {
            cR.second.terminate();
        }
    }
} // rend