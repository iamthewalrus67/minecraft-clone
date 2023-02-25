#ifndef SHADER_LOADING_H
#define SHADER_LOADING_H

#include <fstream>

#include "bgfx/bgfx.h"

bgfx::ShaderHandle loadShader(const char *name);

#endif // !SHADER_LOADING_H
