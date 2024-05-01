cmake_minimum_required(VERSION 3.12)

project(BummerTests)


set(TEST_FILES
    test_engine.cpp
    test_resources.cpp
    test_ui.cpp
)

set(GTEST_COLOR 1)

# Add test cpp file
add_executable(${PROJECT_NAME} ${TEST_FILES})

target_link_libraries(BummerTests 
        BummerLib
        SDL2::SDL2
        $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>
        $<IF:$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>,SDL2_ttf::SDL2_ttf,SDL2_ttf::SDL2_ttf-static>
        nlohmann_json::nlohmann_json
        GTest::gtest
        GTest::gtest_main
)

# Add test to testing
add_test(NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME})