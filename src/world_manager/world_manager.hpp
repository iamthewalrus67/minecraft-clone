#ifndef MINECRAFT_CLONE_WORLD_MANAGER_HPP
#define MINECRAFT_CLONE_WORLD_MANAGER_HPP

#include <vector>
#include "ext/PerlinNoise.hpp"
#include "iostream"
#include "controllers/camera/flying_camera_controller.hpp"
#include "render/chunk/chunk.hpp"
#include "render/chunk/chunk_manager.hpp"

namespace world{
    // default seed
    constexpr siv::PerlinNoise::seed_type DEF_SEED = 123456u;
    // in chunks
    constexpr double DEF_RENDDIST = 8;

    class WorldManager{
    public:
        // Main constructor which takes N seeds for each noise
        [[maybe_unused]] WorldManager(siv::PerlinNoise::seed_type height_seed,
                     siv::PerlinNoise::seed_type temp_seed,
                     double rend_dist=DEF_RENDDIST);

        // Constructor for one specified seed, each next seed would be taken as next int number after previous
        [[maybe_unused]] explicit WorldManager(siv::PerlinNoise::seed_type init_seed, double rend_dist=DEF_RENDDIST): WorldManager(init_seed, init_seed+1) {};

        // Constructor that uses defined default seed
        [[maybe_unused]] explicit WorldManager(double rend_dist=DEF_RENDDIST): WorldManager(DEF_SEED, DEF_SEED+1){};
        ~WorldManager() = default;

        // test function to print noises samples
        void printNoisesSamples();

        void deleteCreateChunks(rend::ChunkManager &chunkManager,
                                std::unique_ptr<control::FlyingCameraController>& camera);

    private:
        siv::PerlinNoise::seed_type m_heightSeed;
        siv::PerlinNoise::seed_type m_tempSeed;

        siv::PerlinNoise m_heightNoise;
        siv::PerlinNoise m_tempNoise;

        std::vector<glm::vec3> m_renderedChunksPositions{};

        glm::ivec3 m_chunkDimensions;

        double m_renderDistance;

    private:
        glm::ivec3 chunkGlobalPositionFromPosition(glm::vec3 globPos);
        void deleteChunks(rend::ChunkManager &chunkManager, const glm::vec3 &cameraPos);
        void createChunks(rend::ChunkManager &chunkManager, const glm::vec3 &cameraPos);
        void fillChunk(rend::Chunk& newChunk, glm::ivec3 &chunkPos);

        static double calculate_dist(glm::vec3 cameraPos, glm::ivec3 chunkPos);
    };

}


#endif //MINECRAFT_CLONE_WORLD_MANAGER_HPP
