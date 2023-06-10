UNAME_S = $(shell uname -s)

CC = clang++

INCFLAGS  = -Isubmods/bgfx/include
INCFLAGS += -Isubmods/bx/include
INCFLAGS += -Isubmods/bimg/include
INCFLAGS += -Isubmods/glfw/include
INCFLAGS += -Isubmods/bgfx/3rdparty/fcpp
INCFLAGS += -Isubmods/spdlog/include
INCFLAGS += -Isubmods/glm
INCFLAGS += -Isrc

$(info INCLFAGS: $(INCFLAGS))

CCFLAGS = -std=c++20 -O2 -g -Wall -Wextra -Wpedantic -Wno-unused-parameter
CCFLAGS += $(INCFLAGS)




ifeq ($(UNAME_S), Darwin)
	CCFLAGS += -DDarvin=1
	FRAMEWORKS	= -framework QuartzCore
    FRAMEWORKS += -framework Cocoa
    FRAMEWORKS += -framework Carbon
    FRAMEWORKS += -framework Metal
    FRAMEWORKS += -framework CoreFoundation
    FRAMEWORKS += -framework IOKit

	LDFLAGS += $(FRAMEWORKS)
	# TODO: select based on ($ arch)
	BGFX_TARGET=osx-arm64
	BGFX_DEPS_TARGET=osx-arm64
endif


ifeq ($(UNAME_S), Linux)
	BGFX_TARGET = linux
	BGFX_DEPS_TARGET = linux64_gcc
endif

SRC = $(shell find src -name "*.cpp")
INCL = $(shell find src -name "*.hpp")
OBJ = $(SRC:%.cpp=$(OBJ_DIR)/%.o)
OBJ_DIR = obj
BIN = bin
BIN_NAME = game

BGFX_BIN = submods/bgfx/.build/$(BGFX_DEPS_TARGET)/bin
BGFX_CONFIG = Debug
BGFX_COMPILE_FLAGS = BGFX_CONFIG=RENDERER_OPENGL=45

ifeq ($(UNAME_S), Darwin)
	LDFLAGS += -lstdc++
	LDFLAGS += submods/glfw/src/libglfw3.a
	LDFLAGS += submods/spdlog/libspdlog.a
    LDFLAGS += $(BGFX_BIN)/libbgfx$(BGFX_CONFIG).a
    LDFLAGS += $(BGFX_BIN)/libbimg$(BGFX_CONFIG).a
    LDFLAGS += $(BGFX_BIN)/libbx$(BGFX_CONFIG).a
    LDFLAGS += $(BGFX_BIN)/libfcpp$(BGFX_CONFIG).a
endif

ifeq ($(UNAME_S), Linux)
	LDFLAGS += submods/glfw/src/libglfw3.a
	LDFLAGS += -lGL
	LDFLAGS += -lX11
	LDFLAGS += $(BGFX_BIN)/libbgfx$(BGFX_CONFIG).a
	LDFLAGS += $(BGFX_BIN)/libbimg$(BGFX_CONFIG).a
	LDFLAGS += $(BGFX_BIN)/libbx$(BGFX_CONFIG).a
	LDFLAGS += $(BGFX_BIN)/libfcpp$(BGFX_CONFIG).a
	LDFLAGS += submods/spdlog/libspdlog.a
	LDFLAGS += $(INCFLAGS)
endif

SHADERS_PATH = res/shaders
SHADERS = $(shell find $(SHADERS_PATH)/* | grep -E ".*/(vs|fs).*.sc")
SHADERS_OUT = $(SHADERS:.sc=.bin)
SHADERC = submods/bgfx/.build/$(BGFX_DEPS_TARGET)/bin/shaderc$(BGFX_CONFIG)

ifeq ($(UNAME_S), Darwin)
	SHADER_TARGET	= metal
    SHADER_PLATFORM = osx
endif

ifeq ($(UNAME_S), Linux)
	SHADER_TARGET = 150
	SHADER_PLATFORM = linux
endif


CCFLAGS += -DSHARED_TARGET_$(SHADER_TARGET) \
		   -DSHADER_PLATFORM_$(SHADER_PLATFORM) \
		   -DBX_CONFIG_DEBUG \
		   -DSPDLOG_COMPILED_LIB

.PHONY: all clean

all: dirs libs shaders build

ifeq ($(UNAME_S), Linux)
libs:
	cd submods/bgfx && make $(BGFX_TARGET) -j8 $(BGFX_COMPILE_FLAGS)
	cd submods/glfw && cmake . && make
	cd submods/spdlog && cmake . && make
endif

ifeq ($(UNAME_S), Darwin)
libs:
	export LD_PATH="$(FRAMEWORKS)"
	cd submods/bx && make $(BGFX_DEPS_TARGET) -j8
	cd submods/bimg && make $(BGFX_DEPS_TARGET) -j8
	cd submods/bgfx && make $(BGFX_TARGET) -j8
	cd submods/glfw && cmake . && make -j8
	cd submods/spdlog && cmake . && make
	export LD_PATH=""
endif

dirs:
	mkdir -p ./$(BIN)

# shader -> bin
%.bin: %.sc
	$(SHADERC)	--type $(shell echo $(notdir $@) | cut -c 1)						\
						  -i submods/bgfx/src										\
							--platform $(SHADER_PLATFORM)							\
							-p $(SHADER_TARGET) 									\
							--varyingdef $(dir $@)varying.def.sc					\
							-f $<													\
							-o $@

shaders: $(SHADERS_OUT)

run: build
	$(BIN)/$(BIN_NAME)

build: dirs shaders $(OBJ)
	$(CC) -o $(BIN)/$(BIN_NAME) $(filter %.o,$^) $(LDFLAGS)

$(OBJ): $(OBJ_DIR)/%.o: %.cpp $(INCL)
	mkdir -p $(@D)
	$(CC) -o $@ -c $< $(CCFLAGS)

clean:
	rm -rf $(shell find res/shaders -name "*.bin")
	rm -rf $(BIN) $(OBJ)
	rm -rf submods/glfw/CMakeCache.txt
