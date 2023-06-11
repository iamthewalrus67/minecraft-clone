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
    constexpr double DEF_RENDDIST = 20;
    // the bigger it is the smoother the noise is
    constexpr double frequency = 100.;
    constexpr double snow_freq = 25.;
    constexpr double temp_freq = 150.;
    // the bigger it is the more details there are
    constexpr int OCTAVES = 4;

    class WorldManager{
    public:
        // Constructor for one specified seed, each next seed would be taken as next int number after previous
        [[maybe_unused]] explicit WorldManager(siv::PerlinNoise::seed_type init_seed, double rend_dist=DEF_RENDDIST);

        // Constructor that uses defined default seed
        [[maybe_unused]] explicit WorldManager(double rend_dist=DEF_RENDDIST): WorldManager(DEF_SEED){};
        ~WorldManager() = default;

        void deleteCreateChunks(rend::ChunkManager &chunkManager,
                                std::shared_ptr<control::FlyingCameraController> camera);

    private:
        siv::PerlinNoise::seed_type m_heightSeed;
        siv::PerlinNoise::seed_type m_snowSeed;
        siv::PerlinNoise::seed_type m_tempSeed;

        siv::PerlinNoise m_heightNoise;
        siv::PerlinNoise m_snowNoise;
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
