#include <iostream>

#include "bgfx/bgfx.h"
#include "shader_loading.hpp"

/*
 * Get shader handle form compile shader
 */
bgfx::ShaderHandle loadShader(const char *name) {
    char *data = new char[2048];
    std::ifstream file{};
    size_t fileSize = 0;

    file.open(name);
    if (file.is_open()) {
        file.seekg(0, std::ios::end);
        fileSize = file.tellg();
        file.seekg(0, std::ios::beg);
        file.read(data, fileSize);
        file.close();
    }

    const bgfx::Memory *mem = bgfx::copy(data, fileSize + 1);
    mem->data[mem->size - 1] = '\0';
    bgfx::ShaderHandle handle = bgfx::createShader(mem);
    bgfx::setName(handle, name);

    return handle;
}
