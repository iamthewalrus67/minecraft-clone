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

    void ChunkManager::terminate() {
        for (auto& cR: m_chunkData) {
            cR.second.terminate();
        }
    }
} // rend