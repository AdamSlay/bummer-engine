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
find_package(nlohmann_json 3.2.0 REQUIRED)

# Set the source files for your project
set(SOURCE_FILES
        src/main.cpp
        src/Resources/ResourceUtils.cpp
        src/Resources/ResourceUtils.h
        src/GameEngine/GameEngine.cpp
        src/GameEngine/GameEngine.h
        src/ECS/EntityManager.cpp
        src/ECS/EntityManager.h
)

# Create the executable
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# Link the SDL2 framework
target_link_libraries(BummerEngine PRIVATE
        SDL2::SDL2
        $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>
        $<IF:$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>,SDL2_ttf::SDL2_ttf,SDL2_ttf::SDL2_ttf-static>
        nlohmann_json::nlohmann_json
)