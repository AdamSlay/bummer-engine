cmake_minimum_required(VERSION 3.12)

project(BummerTests)


set(TEST_FILES
        Test_AISystem.cpp
        Test_AnimationSystem.cpp
        Test_AttackSystem.cpp
        Test_CollisionSystem.cpp
        Test_CooldownSystem.cpp
        Test_EntityManager.cpp
        Test_EventManager.cpp
        Test_GameEngine.cpp
        Test_InputSystem.cpp
        Test_Menu.cpp
        Test_MovementSystem.cpp
        Test_PhysicsSystem.cpp
        Test_RenderSystem.cpp
        Test_ResourceUtils.cpp
        Test_SceneManager.cpp
        Test_SoundSystem.cpp
        Test_StateMachine.cpp
        Test_Utils.cpp
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