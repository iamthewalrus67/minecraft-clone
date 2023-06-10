#ifndef MINECRAFT_CLONE_WORLD_MANAGER_HPP
#define MINECRAFT_CLONE_WORLD_MANAGER_HPP

#include "ext/PerlinNoise.hpp"
#include "iostream"

namespace world{
    constexpr siv::PerlinNoise::seed_type DEF_SEED = 123456u;

    class WorldManager{
    public:
        WorldManager(siv::PerlinNoise::seed_type height_seed,
                     siv::PerlinNoise::seed_type temp_seed);

        WorldManager(siv::PerlinNoise::seed_type init_seed): WorldManager(init_seed, init_seed+1) {};

        WorldManager(): WorldManager(DEF_SEED, DEF_SEED+1){};
        ~WorldManager() = default;

        void printNoisesSamples();

    private:
        siv::PerlinNoise::seed_type m_heightSeed;
        siv::PerlinNoise::seed_type m_tempSeed;

        siv::PerlinNoise m_heightNoise;
        siv::PerlinNoise m_tempNoise;

    };
}


#endif //MINECRAFT_CLONE_WORLD_MANAGER_HPP
