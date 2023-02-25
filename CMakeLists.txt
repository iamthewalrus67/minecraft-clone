cmake_minimum_required(VERSION 3.15)

set(PROJECT_NAME cool_game)
project(${PROJECT_NAME} C CXX)

set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME} src/main.cpp)
target_compile_definitions(${PROJECT_NAME} PRIVATE BX_CONFIG_DEBUG)

# Check if submodules exist
if(NOT EXISTS "${PROJECT_SOURCE_DIR}/submods/bx/makefile")
	message(FATAL_ERROR "The bx submodules was not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
endif()

if(NOT EXISTS "${PROJECT_SOURCE_DIR}/submods/bimg/makefile")
	message(FATAL_ERROR "The bimg submodules was not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
endif()

if(NOT EXISTS "${PROJECT_SOURCE_DIR}/submods/bgfx/makefile")
	message(FATAL_ERROR "The bgfx submodules was not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
endif()

if(NOT EXISTS "${PROJECT_SOURCE_DIR}/submods/bnet/makefile")
	message(FATAL_ERROR "The bnet submodules was not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
endif()


if(UNIX)
  #include dirs
  target_include_directories(${PROJECT_NAME} 
	PRIVATE
		submods/bgfx/include
		submods/bx/include
		submods/bimg/include
  )

  target_include_directories(${PROJECT_NAME} 
	PRIVATE 
		submods/bgfx/3rdparty/
		submods/bgfx/examples/common
	)

  target_include_directories(${PROJECT_NAME}
		  PRIVATE
		  	submods/glfw/include
		  )

  #LINK LIBS
  target_link_libraries(${PROJECT_NAME} glfw3 GL X11 ${CMAKE_DL_LIBS})

  target_link_directories(${PROJECT_NAME} PRIVATE submods/glfw/src)

  #BGFX LINKS LOGIC
  target_link_directories(${PROJECT_NAME} PRIVATE ./submods/bgfx/.build/linux64_gcc/bin/)

  if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    if(NOT EXISTS "${PROJECT_SOURCE_DIR}/submods/bgfx/.build/linux64_gcc/bin/libbgfxDebug.a")
      message(FATAL_ERROR "The bgfx submodule was not built as debug! Go to bgfx folder and run: make linux-debug64")
    endif()
	target_link_libraries(${PROJECT_NAME}
		example-glueDebug
		example-commonDebug
		bimg_decodeDebug
		bgfxDebug
		bimgDebug
		bxDebug
	)
  elseif("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    if(NOT EXISTS "${PROJECT_SOURCE_DIR}/submods/bgfx/.build/linux64_gcc/bin/libbgfxRelease.a")
      message(FATAL_ERROR "The bgfx submodule was not built as release! Go to bgfx folder and run: make linux-release64")
    endif()
	target_link_libraries(${PROJECT_NAME}
		example-glueRelease
		example-commonRelease
		bimg_decodeRelease
		bgfxRelease
		bimgRelease 
		bxRelease
	)
  endif()
else()
	message(FATAL_ERROR "This is for linux only right now, what's on your Hard Drive?")
endif()
