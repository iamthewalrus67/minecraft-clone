#include "world_manager/world_manager.hpp"


world::WorldManager::WorldManager(siv::PerlinNoise::seed_type height_seed,
                                  siv::PerlinNoise::seed_type temp_seed):
                                  m_heightSeed(height_seed),
                                  m_tempSeed(temp_seed),
                                  m_heightNoise(m_heightSeed),
                                  m_tempNoise(m_tempSeed){
}

void world::WorldManager::deleteCreateChunks(rend::ChunkManager &chunkManager,
                                             control::FlyingCameraController &camera) {
    glm::vec3 cameraPos = camera.getPosition();
    deleteChunks(cameraPos);
}

void world::WorldManager::deleteChunks(const glm::vec3 &camera_pos) {

}



void world::WorldManager::printNoisesSamples() {
    for (int y = 0; y < 5; ++y){
        for (int x = 0; x < 5; ++x){
            const double val = m_heightNoise.noise2D_01((x * 0.01), (y * 0.01));

            std::cout << val << '\t';
        }

        std::cout << '\n';
    }


    for (int y = 0; y < 5; ++y){
        for (int x = 0; x < 5; ++x){
            const double val = m_tempNoise.noise2D_01((x * 0.01), (y * 0.01));

            std::cout << val << '\t';
        }

        std::cout << '\n';
    }
}