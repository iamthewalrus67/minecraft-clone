#ifndef SHADER_LOADING_HPP
#define SHADER_LOADING_HPP

#include <fstream>

#include <bgfx/bgfx.h>

bgfx::ShaderHandle loadShader(const char *name);

#endif // SHADER_LOADING_HPP
