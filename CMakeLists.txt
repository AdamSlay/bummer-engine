cmake_minimum_required(VERSION 3.22)
project(BummerEngine)

set(CMAKE_CXX_STANDARD 17)

# Include the vcpkg toolchain file
set(CMAKE_TOOLCHAIN_FILE "./vcpkg/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING "Vcpkg toolchain file")

# Find the SDL2 package
find_package(SDL2 REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
find_package(SDL2_ttf CONFIG REQUIRED)
find_package(SDL2_mixer CONFIG REQUIRED)
find_package(nlohmann_json 3.2.0 REQUIRED)

# Set the source files for your project
set(SOURCE_FILES
        src/Resources/ResourceUtils.cpp
        src/Resources/ResourceUtils.h
        src/GameEngine/GameEngine.cpp
        src/GameEngine/GameEngine.h
        src/ECS/Components.h
        src/ECS/EntityManager.cpp
        src/ECS/EntityManager.h
        src/UI/Menu.cpp
        src/UI/Menu.h
        src/Config.cpp
        src/Config.h
        src/Systems/RenderSystem.cpp
        src/Systems/RenderSystem.h
        src/Resources/TextureManager.cpp
        src/Resources/TextureManager.h
        src/Systems/MovementSystem.cpp
        src/Systems/MovementSystem.h
        src/Systems/CollisionSystem.cpp
        src/Systems/CollisionSystem.h
        src/Systems/AnimationSystem.cpp
        src/Systems/AnimationSystem.h
        src/Utils.cpp
        src/Systems/InputSystem.cpp
        src/Systems/InputSystem.h
        src/Utils.h
        src/Systems/PhysicsSystem.cpp
        src/Systems/PhysicsSystem.h
        src/Systems/SoundSystem.cpp
        src/Systems/SoundSystem.h
        src/ECS/EventManager.cpp
        src/ECS/EventManager.h
        src/ECS/SceneManager.cpp
        src/ECS/SceneManager.h
        src/Systems/AttackSystem.cpp
        src/Systems/AttackSystem.h
        src/Systems/AISystem.cpp
        src/Systems/AISystem.h
)

add_library(BummerLib ${SOURCE_FILES})

# Create the executable
add_executable(${PROJECT_NAME} src/main.cpp)

# Link the SDL2 framework
target_link_libraries(BummerLib PUBLIC
        SDL2::SDL2
        $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>
        $<IF:$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>,SDL2_ttf::SDL2_ttf,SDL2_ttf::SDL2_ttf-static>
        $<IF:$<TARGET_EXISTS:SDL2_mixer::SDL2_mixer>,SDL2_mixer::SDL2_mixer,SDL2_mixer::SDL2_mixer-static>
        nlohmann_json::nlohmann_json
)

target_link_libraries(${PROJECT_NAME} PRIVATE BummerLib)

# Enable testing
enable_testing()

# Add your test subdirectory
add_subdirectory(tests)