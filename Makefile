UNAME_S = $(shell uname -s)

CC = g++

INCFLAGS  = -Isubmods/bgfx/include
INCFLAGS += -Isubmods/bx/include
INCFLAGS += -Isubmods/bimg/include
INCFLAGS += -Isubmods/glfw/include
INCFLAGS += -Isubmods/bgfx/3rdparty/fcpp
INCFLAGS += -Isrc

$(info INCLFAGS: $(INCFLAGS))

CCFLAGS = -std=c++20 -O2 -g -Wall -Wextra -Wpedantic -Wno-unused-parameter
CCFLAGS += $(INCFLAGS)

BGFX_TARGET = linux
BGFX_DEPS_TARGET = linux64_gcc

SRC = $(shell find src -name "*.cpp")
OBJ = $(SRC:%.cpp=$(OBJ_DIR)/%.o)
OBJ_DIR = obj
BIN = bin

BGFX_BIN = submods/bgfx/.build/$(BGFX_DEPS_TARGET)/bin
BGFX_CONFIG = Debug

LDFLAGS += -lX11
LDFLAGS += -lGL
LDFLAGS += $(BGFX_BIN)/libbgfx$(BGFX_CONFIG).a
LDFLAGS += $(BGFX_BIN)/libbimg$(BGFX_CONFIG).a
LDFLAGS += $(BGFX_BIN)/libbx$(BGFX_CONFIG).a
LDFLAGS += $(BGFX_BIN)/libfcpp$(BGFX_CONFIG).a
LDFLAGS += submods/glfw/src/libglfw3.a
LDFLAGS += $(INCFLAGS)

SHADERS_PATH = res/shaders
SHADERS	= $(shell find $(SHADERS_PATH)/* | grep -E ".*/(vs|fs).*.sc")
SHADERS_OUT	= $(SHADERS:.sc=.bin)
SHADERC	= submods/bgfx/.build/$(BGFX_DEPS_TARGET)/bin/shaderc$(BGFX_CONFIG)
SHADER_TARGET = 330
SHADER_PLATFORM = linux

CCFLAGS += -DSHARED_TARGET_$(SHADER_TARGET) \
		   -DSHADER_PLATFORM_$(SHADER_PLATFORM) \
		   -DBX_CONFIG_DEBUG

.PHONY: all clean

all: dirs libs shaders build

libs:
	cd submods/bgfx && make $(BGFX_TARGET)
	cd submods/glfw && cmake . && make

dirs:
	mkdir -p ./$(BIN)

# shader -> bin
%.bin: %.sc
	$(SHADERC)	--type $(shell echo $(notdir $@) | cut -c 1)						\
						  -i submods/bgfx/src										\
							--platform $(SHADER_PLATFORM)							\
							--varyingdef $(dir $@)varying.def.sc					\
							-f $<													\
							-o $@

shaders: $(SHADERS_OUT)

run: build
	$(BIN)/game

build: dirs shaders $(OBJ)
	$(CC) -o $(BIN)/game $(filter %.o,$^) $(LDFLAGS)

$(OBJ): $(OBJ_DIR)/%.o: %.cpp
	mkdir -p $(@D)
	$(CC) -o $@ -c $< $(CCFLAGS)

clean:
	rm -rf $(shell find res/shaders -name "*.bin")
	rm -rf $(BIN) $(OBJ)
	rm -rf submods/glfw/CMakeCache.txt
