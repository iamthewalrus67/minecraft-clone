#include "world_manager/world_manager.hpp"


world::WorldManager::WorldManager(siv::PerlinNoise::seed_type init_seed,
                                  double rend_dist):
                                  m_heightSeed(init_seed),
                                  m_snowSeed(init_seed+1),
                                  m_tempSeed(init_seed+2),
                                  m_heightNoise(m_heightSeed),
                                  m_snowNoise(m_snowSeed),
                                  m_tempNoise(m_tempSeed),
                                  m_chunkDimensions{rend::Chunk::WIDTH_X, rend::Chunk::HEIGHT_Y, rend::Chunk::DEPTH_Z},
                                  m_renderDistance(rend_dist){
}

void world::WorldManager::deleteCreateChunks(rend::ChunkManager &chunkManager,
                                             std::shared_ptr<control::FlyingCameraController> camera) {
    glm::vec3 cameraPos = camera->getPosition();
    deleteChunks(chunkManager, cameraPos);
    createChunks(chunkManager, cameraPos);
}

void world::WorldManager::createChunks(rend::ChunkManager &chunkManager, const glm::vec3 &cameraPos){
    auto currChunkPos = chunkGlobalPositionFromPosition(cameraPos);
    int xPos = currChunkPos.x;
    int zPos = currChunkPos.z;
    for(int x = xPos - m_chunkDimensions.x * m_renderDistance; x <= xPos + m_chunkDimensions.x * m_renderDistance; x += m_chunkDimensions.x){
        for(int z = zPos - m_chunkDimensions.z * m_renderDistance; z <= zPos + m_chunkDimensions.z * m_renderDistance; z += m_chunkDimensions.z){
            if(calculate_dist(cameraPos, glm::ivec3{x, 0, z}) <= m_renderDistance * m_chunkDimensions.x &&
                std::count(m_renderedChunksPositions.begin(), m_renderedChunksPositions.end(), glm::vec3{x, 0, z}) == 0){
                auto chunkPos = glm::ivec3{x, 0, z};
                auto& newChunk = chunkManager.addChunk(chunkPos);
                m_renderedChunksPositions.emplace_back(chunkPos);
                fillChunk(newChunk, chunkPos);
            }
        }
    }
}

void world::WorldManager::fillChunk(rend::Chunk& newChunk, glm::ivec3 &chunkPos){
    for (uint32_t x = 0; x < rend::Chunk::WIDTH_X; ++x) {
        for (uint32_t z = 0; z < rend::Chunk::DEPTH_Z; ++z) {
            auto remapedCoords = glm::ivec3{chunkPos.x + x - m_chunkDimensions.x/2, 0, chunkPos.z + z - m_chunkDimensions.z};

            double temperature = m_tempNoise.octave2D_01(remapedCoords.x / world::temp_freq,
                                                         remapedCoords.z / world::temp_freq, 10, 0.65);

            double h_persistance = (temperature < 0.1) ? 1. - (temperature * 4): 0.3;
            auto height = static_cast<uint32_t>(m_heightNoise.octave2D_01(remapedCoords.x / world::frequency,
                                                                          remapedCoords.z / world::frequency,
                                                                          world::OCTAVES,
                                                                          h_persistance)
                                                * m_chunkDimensions.y / 1.8
                                                + m_chunkDimensions.y / 2.3);

            auto h = m_snowNoise.noise2D_01(remapedCoords.x / world::snow_freq,
                                            remapedCoords.z / world::snow_freq);
            auto sand_height = static_cast<uint32_t>(h * 2);
            auto snow_height = static_cast<uint32_t>(h * (m_chunkDimensions.y / 5.));

//            std::cout<<temperature<<std::endl;

            for (uint32_t y = 0; y < rend::Chunk::HEIGHT_Y; ++y) {
                if(y <= height - 3){
                    newChunk.setBlock(glm::vec3{x, y, z}, rend::BLOCKS::STONE);
                }
                else if(temperature < 0.9){
                    if(y == height && ((static_cast<uint32_t>((m_chunkDimensions.y / 10. * 6.))) <= height && height <= (static_cast<uint32_t>((m_chunkDimensions.y / 10. * 7.)) + sand_height))){
                        newChunk.setBlock(glm::vec3{x, y, z}, rend::BLOCKS::SAND);
                    }
                    else if(y < height && height <= m_chunkDimensions.y - snow_height){
                        newChunk.setBlock(glm::vec3{x, y, z}, rend::BLOCKS::DIRT);
                    }
                    else if(y == height &&
                            height <= m_chunkDimensions.y - snow_height){
                        newChunk.setBlock(glm::vec3{x, y, z}, rend::BLOCKS::GRASS);
                    }
                    else if(y < height) {
                        newChunk.setBlock(glm::vec3{x, y, z}, rend::BLOCKS::STONE);
                    }
                    else if(y == height){
                        newChunk.setBlock(glm::vec3{x, y, z}, rend::BLOCKS::SNOW_POWDER);
                    }
                    else if(y < (m_chunkDimensions.y / 10. * 7.)){
                        newChunk.setBlock(glm::vec3{x, y, z}, rend::BLOCKS::WATER);
                    }

                    else{
                        newChunk.setBlock(glm::vec3{x, y, z}, rend::BLOCKS::AIR);
                    }
                }
                else if(temperature >= 0.9){
                    if(y <= height - 3){
                        newChunk.setBlock(glm::vec3{x, y, z}, rend::BLOCKS::STONE);
                    }
                    else if(y < height){
                        newChunk.setBlock(glm::vec3{x, y, z}, rend::BLOCKS::SAND);
                    }
                    else if(y <= (m_chunkDimensions.y / 10. * 7.)){
                        newChunk.setBlock(glm::vec3{x, y, z}, rend::BLOCKS::WATER);
                    }
                    else{
                        newChunk.setBlock(glm::vec3{x, y, z}, rend::BLOCKS::AIR);
                    }
                }

            }
        }
    }
}


void world::WorldManager::deleteChunks(rend::ChunkManager &chunkManager, const glm::vec3 &cameraPos) {
    auto it = m_renderedChunksPositions.begin();
    for (; it != m_renderedChunksPositions.end();) {
        if (calculate_dist(*it, cameraPos) >= m_renderDistance * m_chunkDimensions.x) {
            chunkManager.removeChunk(*it);
            m_renderedChunksPositions.erase(it);
        } else {
            it++;
        }
    }
}

glm::ivec3 world::WorldManager::chunkGlobalPositionFromPosition(glm::vec3 globPos) {
    auto pos = glm::ivec3(globPos);
    int x_offset = (int) pos.x % m_chunkDimensions.x;
    int z_offset = (int) pos.z % m_chunkDimensions.z;
    int x_coord = (x_offset < m_chunkDimensions.x / 2) ? pos.x - x_offset :pos.x + (m_chunkDimensions.x - x_offset);
    int z_coord = (z_offset < m_chunkDimensions.z / 2) ? pos.z - z_offset :pos.z + (m_chunkDimensions.z - z_offset);
    return glm::ivec3{x_coord, pos.y, z_coord};
}

double world::WorldManager::calculate_dist(const glm::vec3 cameraPos, const glm::ivec3 chunkPos){
    return (std::sqrt(std::pow(cameraPos.x - chunkPos.x, 2) + std::pow(cameraPos.z - chunkPos.z, 2)));
}
