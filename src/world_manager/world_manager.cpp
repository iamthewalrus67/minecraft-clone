#include "world_manager/world_manager.hpp"


world::WorldManager::WorldManager(siv::PerlinNoise::seed_type init_seed,
                                  double rend_dist):
                                  m_heightSeed(init_seed),
                                  m_snowSeed(init_seed+1),
                                  m_tempSeed(init_seed+2),
                                  m_treesSeed(init_seed+3),
                                  m_ironSeed(init_seed+4),
                                  m_coalSeed(init_seed+5),
                                  m_heightNoise(m_heightSeed),
                                  m_snowNoise(m_snowSeed),
                                  m_tempNoise(m_tempSeed),
                                  m_treesNoise(m_treesSeed),
                                  m_ironNoise(m_ironSeed),
                                  m_coalNoise(m_coalSeed),
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
            if(calculate_dist(cameraPos, glm::ivec3{x, 0, z}) <= m_renderDistance * m_chunkDimensions.x and
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

            double temperature = m_tempNoise.octave2D_01(remapedCoords.x / temp_freq,
                                                         remapedCoords.z / temp_freq, TEMPOCTAVES, tempPersistnce);

            double h_persistance = (temperature < mountainsThreshold) ? 0.9 - (temperature * 4): defaultPersistance;
            h_persistance = (temperature <= 0.8 and temperature >= 0.1) ? 0.1: h_persistance;

            auto height = static_cast<uint32_t>(m_heightNoise.octave2D_01(remapedCoords.x / frequency,
                                                                          remapedCoords.z / frequency,
                                                                          OCTAVES,
                                                                          h_persistance)
                                                * m_chunkDimensions.y / heightScalar
                                                + m_chunkDimensions.y / heightAdditionScalar);

            auto h = m_snowNoise.noise2D_01(remapedCoords.x / snow_freq,
                                            remapedCoords.z / snow_freq);
            auto sand_height = static_cast<uint32_t>(h * sandScalar);
            auto snow_height = static_cast<uint32_t>(h * (m_chunkDimensions.y / snowScalar));


            for (uint32_t y = 0; y < rend::Chunk::HEIGHT_Y; ++y) {
                if(y <= height - 3){
                    handleOres(glm::ivec3{x, y, z}, remapedCoords, height, newChunk);
                }
                else if(temperature < 0.9){
                    handlePlains(glm::ivec3{x, y, z}, remapedCoords, height, sand_height, snow_height, h_persistance, newChunk);
                }
                else if(temperature >= 0.9){
                    handleDesert(glm::ivec3{x, y, z}, height, newChunk);
                }

            }
        }
    }
}

void world::WorldManager::handleOres(glm::ivec3 pos, glm::ivec3 remapedCoords, uint32_t height, rend::Chunk& newChunk){
    uint32_t x = pos.x;
    uint32_t y = pos.y;
    uint32_t z = pos.z;
    if(m_ironNoise.octave3D_01(remapedCoords.x / iron_freq, y / iron_freq, remapedCoords.z / iron_freq, 5, 0.8) >= 0.995){
        newChunk.setBlock(glm::vec3{x, y, z}, rend::BLOCKS::IRON);
    }
    else if(y >= m_chunkDimensions.y * coalLowerBound and m_coalNoise.octave3D_01(remapedCoords.x / coal_freq, y / coal_freq, remapedCoords.z / coal_freq, 5, 0.85) >= 0.975){
        newChunk.setBlock(glm::vec3{x, y, z}, rend::BLOCKS::COAL);
    }
    else{
        newChunk.setBlock(glm::vec3{x, y, z}, rend::BLOCKS::STONE);
    }
}


void world::WorldManager::handlePlains(glm::ivec3 pos, glm::ivec3 remapedCoords, uint32_t height, uint32_t sand_height, uint32_t snow_height, double h_persistance, rend::Chunk& newChunk){
    uint32_t x = pos.x;
    uint32_t y = pos.y;
    uint32_t z = pos.z;
    if(y == height and ((static_cast<uint32_t>((m_chunkDimensions.y * waterLevel))) <= height and height <= (static_cast<uint32_t>((m_chunkDimensions.y * waterLevel)) + sand_height))){
        newChunk.setBlock(glm::vec3{x, y, z}, rend::BLOCKS::SAND);
    }
    else if(y < height and height <= m_chunkDimensions.y - snow_height){
        newChunk.setBlock(glm::vec3{x, y, z}, rend::BLOCKS::DIRT);
    }
    else if(y == height and
            height <= m_chunkDimensions.y - snow_height){
        newChunk.setBlock(glm::vec3{x, y, z}, rend::BLOCKS::GRASS);
    }
    else if(y < height) {
        newChunk.setBlock(glm::vec3{x, y, z}, rend::BLOCKS::STONE);
    }
    else if(y == height){
        newChunk.setBlock(glm::vec3{x, y, z}, rend::BLOCKS::SNOW_POWDER);
    }
    else if(y < (m_chunkDimensions.y * waterLevel)){
        newChunk.setBlock(glm::vec3{x, y, z}, rend::BLOCKS::WATER);
    }
    else if(y >= height + 1 and y <= height + 6
            and m_treesNoise.octave2D_01(remapedCoords.x / world::trees_freq, remapedCoords.z / world::trees_freq, world::OCTAVES, 0.8) > 0.95
            and height < m_chunkDimensions.y - snow_height * 1.5
                         and height > (m_chunkDimensions.y * waterLevel) + 2){
        newChunk.setBlock(glm::vec3{x, y, z}, rend::BLOCKS::OAK_WOOD);
    }
    else{
        if(height < m_chunkDimensions.y - snow_height * 1.5 + 7
                    and height > (m_chunkDimensions.y / 10. * 7.) + 2
           and handleTrees(glm::ivec3{remapedCoords.x, y, remapedCoords.z}, h_persistance, height)){
            newChunk.setBlock(glm::vec3{x, y, z}, rend::BLOCKS::OAK_LEAVES);
        }
        else{
            newChunk.setBlock(glm::vec3{x, y, z}, rend::BLOCKS::AIR);
        }
    }
}


void world::WorldManager::handleDesert(glm::ivec3 pos, uint32_t height, rend::Chunk& newChunk){
    uint32_t x = pos.x;
    uint32_t y = pos.y;
    uint32_t z = pos.z;
    if(y <= height - 3){
        newChunk.setBlock(glm::vec3{x, y, z}, rend::BLOCKS::STONE);
    }
    else if(y < height){
        newChunk.setBlock(glm::vec3{x, y, z}, rend::BLOCKS::SAND);
    }
    else if(y <= (m_chunkDimensions.y * waterLevel)){
        newChunk.setBlock(glm::vec3{x, y, z}, rend::BLOCKS::WATER);
    }
    else{
        newChunk.setBlock(glm::vec3{x, y, z}, rend::BLOCKS::AIR);
    }
}

bool world::WorldManager::handleTrees(glm::ivec3 coords, double persistance, int32_t h){
    if(coords.y <= h or coords.y > h+7) return false;
    for(int i=-2; i <=2; ++i){
        for(int j=-2; j<=2; ++j){
            auto height = static_cast<uint32_t>(m_heightNoise.octave2D_01((coords.x + i) / world::frequency,
                                                                          (coords.z + j) / world::frequency,
                                                                          world::OCTAVES,
                                                                          persistance)
                                                * m_chunkDimensions.y / 1.8
                                                + m_chunkDimensions.y / 2.3);

            if(coords.y < height+4 or coords.y > (height+8+ (int)((-std::abs(i) - std::abs(j))/2.))) continue;

            if(m_treesNoise.octave2D_01((coords.x + i) / world::trees_freq, (coords.z + j) / world::trees_freq, world::OCTAVES, 0.8) > 0.95) return true;
        }
    }
    return false;
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
