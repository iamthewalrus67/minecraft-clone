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
    constexpr double DEF_RENDDIST = 4;
    // the bigger it is the smoother the noise is
    constexpr double frequency = 100.;
    constexpr double snow_freq = 300.;
    constexpr double temp_freq = 150.;
    constexpr double trees_freq = 0.25;
    constexpr double iron_freq = 10.;
    constexpr double coal_freq = 8.;
    // the bigger it is the more details there are
    constexpr int OCTAVES = 6;
    constexpr int TEMPOCTAVES = 10;
    
    constexpr double defaultPersistance = 0.5;
    constexpr double tempPersistnce = 0.65;
    constexpr double mountainsThreshold = 0.1;

    constexpr double heightScalar = 1.;
    constexpr double heightAdditionScalar = 10000.;

    constexpr double sandScalar = 2;
    constexpr double snowScalar = 2.65;

    constexpr double waterLevel = 5. / 10.;
    constexpr double ironUpperBound = 5. / 10.;
    constexpr double coalLowerBound = 1. / 10.;

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
        siv::PerlinNoise::seed_type m_treesSeed;
        siv::PerlinNoise::seed_type m_ironSeed;
        siv::PerlinNoise::seed_type m_coalSeed;

        siv::PerlinNoise m_heightNoise;
        siv::PerlinNoise m_snowNoise;
        siv::PerlinNoise m_tempNoise;
        siv::PerlinNoise m_treesNoise;
        siv::PerlinNoise m_ironNoise;
        siv::PerlinNoise m_coalNoise;


        std::vector<glm::vec3> m_renderedChunksPositions{};
        glm::ivec3 m_chunkDimensions;

        double m_renderDistance;

    private:
        glm::ivec3 chunkGlobalPositionFromPosition(glm::vec3 globPos);
        void deleteChunks(rend::ChunkManager &chunkManager, const glm::vec3 &cameraPos);
        void createChunks(rend::ChunkManager &chunkManager, const glm::vec3 &cameraPos);
        void fillChunk(rend::Chunk& newChunk, glm::ivec3 &chunkPos);
        bool handleTrees(glm::ivec3 coords,  double persistance, int32_t h);
        void handlePlains(glm::ivec3 pos, glm::ivec3 remapedCoords, uint32_t height, uint32_t sand_height, uint32_t snow_height, double h_persistance, rend::Chunk& newChunk);
        void handleDesert(glm::ivec3 pos, uint32_t height, rend::Chunk& newChunk);
        void handleOresCaves(glm::ivec3 pos, glm::ivec3 remapedCoords, uint32_t height, rend::Chunk& newChunk);

        static double calculate_dist(glm::vec3 cameraPos, glm::ivec3 chunkPos);
    };

}


#endif //MINECRAFT_CLONE_WORLD_MANAGER_HPP
