cmake_minimum_required(VERSION 3.22)
project(BummerEngine)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_TOOLCHAIN_FILE "./vcpkg/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING "Vcpkg toolchain file")

find_package(SDL2 REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
find_package(SDL2_ttf CONFIG REQUIRED)
find_package(SDL2_mixer CONFIG REQUIRED)
find_package(nlohmann_json 3.2.0 REQUIRED)
find_package(GTest CONFIG REQUIRED)

set(SOURCE_FILES
        src/Config.cpp
        src/Config.h
        src/ECS/Components.h
        src/ECS/Entity.cpp
        src/ECS/Entity.h
        src/ECS/EntityManager.cpp
        src/ECS/EntityManager.h
        src/ECS/EventManager.cpp
        src/ECS/EventManager.h
        src/ECS/SceneManager.cpp
        src/ECS/SceneManager.h
        src/ECS/StateMachine.cpp
        src/ECS/StateMachine.h
        src/GameEngine/GameEngine.cpp
        src/GameEngine/GameEngine.h
        src/Resources/ResourceUtils.cpp
        src/Resources/ResourceUtils.h
        src/Resources/TextureManager.cpp
        src/Resources/TextureManager.h
        src/Systems/AISystem.cpp
        src/Systems/AISystem.h
        src/Systems/AnimationSystem.cpp
        src/Systems/AnimationSystem.h
        src/Systems/AttackSystem.cpp
        src/Systems/AttackSystem.h
        src/Systems/CollisionSystem.cpp
        src/Systems/CollisionSystem.h
        src/Systems/CooldownSystem.cpp
        src/Systems/CooldownSystem.h
        src/Systems/InputSystem.cpp
        src/Systems/InputSystem.h
        src/Systems/MovementSystem.cpp
        src/Systems/MovementSystem.h
        src/Systems/PhysicsSystem.cpp
        src/Systems/PhysicsSystem.h
        src/Systems/RenderSystem.h
        src/Systems/RenderSystem.cpp
        src/Systems/SoundSystem.cpp
        src/Systems/SoundSystem.h
        src/UI/Menu.cpp
        src/UI/Menu.h
        src/Utils.cpp
        src/Utils.h
)

add_library(BummerLib ${SOURCE_FILES})

add_executable(${PROJECT_NAME} src/main.cpp)

target_link_libraries(BummerLib PUBLIC
        SDL2::SDL2
        $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>
        $<IF:$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>,SDL2_ttf::SDL2_ttf,SDL2_ttf::SDL2_ttf-static>
        $<IF:$<TARGET_EXISTS:SDL2_mixer::SDL2_mixer>,SDL2_mixer::SDL2_mixer,SDL2_mixer::SDL2_mixer-static>
        nlohmann_json::nlohmann_json
        GTest::gtest
        GTest::gtest_main
)

target_link_libraries(${PROJECT_NAME} PRIVATE BummerLib)

enable_testing()

add_subdirectory(tests)