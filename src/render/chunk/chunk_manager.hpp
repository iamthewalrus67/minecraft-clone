//
// Created by adeon on 6/8/23.
//

#ifndef COOL_GAME_CHUNK_MANAGER_HPP
#define COOL_GAME_CHUNK_MANAGER_HPP

#include <unordered_map>
#include <optional>
#include <span>

#include "chunk_renderer.hpp"

#include "util/util.hpp"
#include "math/aabb.hpp"

namespace rend {
    //! A class that manages all of the chunks
    class ChunkManager {
        using ChunkRendererMap = std::unordered_map<glm::ivec3, ChunkRenderer, ul::fuckingGlmHashFunctionIvec>;
    public:
        //! Add chunk by position, to control this chunk, capture the reference into a variable
        [[nodiscard]] Chunk& addChunk(const glm::ivec3& chunkPos);

        //! Get the chunk position from ARBITRARY GLOBAL POSITION
        [[nodiscard]] Chunk * getChunkRefFromGlobalPos(const glm::vec3& pos);
        //! Get the chunk position from ARBITRARY GLOBAL POSITION
        [[nodiscard]] Chunk * getChunkRefFromAliquotPos(const glm::vec3& pos);
        //! remove the chunk by position, if position is invalid, returns false
        bool removeChunk(const glm::ivec3& chunkPos);

        //! Remesh chunks if needed
        void reMeshChunks();

        //! Fill the neighbor chunks with pointers based on the given chunk position
        void fillNeighborChunksFromPos(std::array<Chunk*, 6>* neighbors, const glm::ivec3& globalChunkPos);

        [[nodiscard]] Chunk* getNeighboorByDir(util::Direction dir, const glm::ivec3& globalChunkPos);

        //! Get all the chunk renderers, done so that the renderer can pull them
        [[nodiscard]] ChunkRendererMap& getChunkRenderers() { return m_chunkData; }

        //! Get colliders for blocks in the chunk
        size_t getColliders(const std::span<math::AABBf>& dest, const math::AABBi& area);

        //! Terminate all renderers
        void terminate();
    private:
        // A map of chunkPos - ChunkRenderer that contains a chunk
        //! WARNING: POSITION HAS TO HAVE x % Chunk::WIDTH == 0 and y % Chunk::HEIGHT == 0 and z % Chunk::DEPTH == 0
        ChunkRendererMap m_chunkData;
    };
}

#endif //COOL_GAME_CHUNK_MANAGER_HPP
