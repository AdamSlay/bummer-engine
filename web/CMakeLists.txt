cmake_minimum_required(VERSION 3.22)
project(BummerEngine)

set(CMAKE_CXX_STANDARD 17)

# Check if we are using Emscripten
 if(DEFINED EMSCRIPTEN_VERSION OR CMAKE_SYSTEM_NAME STREQUAL "Emscripten")
    set(EMSCRIPTEN TRUE)
    # Add the include path for nlohmann-json
    set(INCLUDE_PATH "/opt/homebrew/opt/nlohmann-json/include")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${INCLUDE_PATH}")
 else()
    set(EMSCRIPTEN FALSE)
    # Include the vcpkg toolchain file for non-Emscripten builds
    set(CMAKE_TOOLCHAIN_FILE "./vcpkg/scripts/buildsystems/vcpkg.cmake"
            CACHE STRING "Vcpkg toolchain file")
endif()

# Conditionally set compiler flags for Emscripten
if(EMSCRIPTEN)
    set(USE_FLAGS "-s USE_SDL=2 -s USE_SDL_IMAGE=2 -s USE_SDL_TTF=2 -s USE_SDL_MIXER=2")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${USE_FLAGS}")
endif()

# Conditionally find packages
if(NOT EMSCRIPTEN)
    find_package(SDL2 REQUIRED)
    find_package(SDL2_image CONFIG REQUIRED)
    find_package(SDL2_ttf CONFIG REQUIRED)
    find_package(SDL2_mixer CONFIG REQUIRED)
    find_package(nlohmann_json 3.2.0 REQUIRED)
endif()

# Set the source files for your project
set(SOURCE_FILES
        ../src/Resources/ResourceUtils.cpp
        ../src/Resources/ResourceUtils.h
        ../src/GameEngine/GameEngine.cpp
        ../src/GameEngine/GameEngine.h
        ../src/ECS/Components.h
        ../src/ECS/EntityManager.cpp
        ../src/ECS/EntityManager.h
        ../src/UI/Menu.cpp
        ../src/UI/Menu.h
        ../src/Config.cpp
        ../src/Config.h
        ../src/Systems/RenderSystem.cpp
        ../src/Systems/RenderSystem.h
        ../src/Resources/TextureManager.cpp
        ../src/Resources/TextureManager.h
        ../src/Systems/MovementSystem.cpp
        ../src/Systems/MovementSystem.h
        ../src/Systems/CollisionSystem.cpp
        ../src/Systems/CollisionSystem.h
        ../src/Systems/AnimationSystem.cpp
        ../src/Systems/AnimationSystem.h
        ../src/Utils.cpp
        ../src/Systems/InputSystem.cpp
        ../src/Systems/InputSystem.h
        ../src/Utils.h
        ../src/Systems/PhysicsSystem.cpp
        ../src/Systems/PhysicsSystem.h
        ../src/Systems/SoundSystem.cpp
        ../src/Systems/SoundSystem.h
        ../src/ECS/EventManager.cpp
        ../src/ECS/EventManager.h
        ../src/ECS/SceneManager.cpp
        ../src/ECS/SceneManager.h
        ../src/Systems/AttackSystem.cpp
        ../src/Systems/AttackSystem.h
        ../src/Systems/AISystem.cpp
        ../src/Systems/AISystem.h
        ../src/ECS/StateMachine.cpp
        ../src/ECS/StateMachine.h
        ../src/Systems/CooldownSystem.cpp
        ../src/Systems/CooldownSystem.h
)

# Add the executable
add_executable(${PROJECT_NAME} ../src/main.cpp ${SOURCE_FILES})

# Conditionally link libraries
if(NOT EMSCRIPTEN)
    target_link_libraries(${PROJECT_NAME} PRIVATE
            SDL2::SDL2
            SDL2_image::SDL2_image
            SDL2_ttf::SDL2_ttf
            SDL2_mixer::SDL2_mixer
            nlohmann_json::nlohmann_json
    )
endif()