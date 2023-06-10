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
                chunkRenderer.second.meshChunk();
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

    void ChunkManager::terminate() {
        for (auto& cR: m_chunkData) {
            cR.second.terminate();
        }
    }
} // rend