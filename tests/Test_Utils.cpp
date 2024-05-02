#include <gtest/gtest.h>
#include "../src/Utils.h"


TEST(UtilsTest, Test_stringToAction) {
    Action jumpAction = Utils::stringToAction("JUMP");
    EXPECT_EQ(jumpAction, Action::JUMP);

    Action attackAction = Utils::stringToAction("ATTACK");
    EXPECT_EQ(attackAction, Action::ATTACK);

    Action invalidAction = Utils::stringToAction("invalid");
    EXPECT_EQ(invalidAction, Action::WAIT);
}

TEST(UtilsTest, Test_stringToDirection) {
    Direction leftDirection = Utils::stringToDirection("LEFT");
    EXPECT_EQ(leftDirection, Direction::LEFT);

    Direction joyRightDirection = Utils::stringToDirection("JOY_RIGHT");
    EXPECT_EQ(joyRightDirection, Direction::JOY_RIGHT);

    Direction invalidDirection = Utils::stringToDirection("invalid");
    EXPECT_EQ(invalidDirection, Direction::STILL);
}

TEST(UtilsTest, Test_actionToString) {
    std::string jumpAction = Utils::actionToString(Action::JUMP);
    EXPECT_EQ(jumpAction, "JUMP");

    std::string waitAction = Utils::actionToString(Action::WAIT);
    EXPECT_EQ(waitAction, "WAIT");
}

TEST(UtilsTest, Test_playerStateToString) {
    std::string idleState = Utils::playerStateToString(playerStates::IDLE);
    EXPECT_EQ(idleState, "IDLE");

    std::string runState = Utils::playerStateToString(playerStates::RUN);
    EXPECT_EQ(runState, "RUN");
}

TEST(UtilsTest, Test_calculateColliderRect) {
    Transform transformA = {100, 100, 2};
    Collider colliderA = {0, 0, 50, 50};
    SDL_Rect rectA = Utils::calculateColliderRect(transformA, colliderA);
    EXPECT_EQ(rectA.x, 100);
    EXPECT_EQ(rectA.y, 100);
    EXPECT_EQ(rectA.w, 100);
    EXPECT_EQ(rectA.h, 100);

    Transform transformB = {0, 0, 1.5};
    Collider colliderB = {100, 100, 100, 100};
    SDL_Rect rectB = Utils::calculateColliderRect(transformB, colliderB);
    EXPECT_EQ(rectB.x, 150);
    EXPECT_EQ(rectB.y, 150);
    EXPECT_EQ(rectB.w, 150);
    EXPECT_EQ(rectB.h, 150);

    Transform transformC = {0, 0, 1};
    Collider colliderC = {0, 0, 0, 0};
    SDL_Rect rectC = Utils::calculateColliderRect(transformC, colliderC);
    EXPECT_EQ(rectC.x, 0);
    EXPECT_EQ(rectC.y, 0);
    EXPECT_EQ(rectC.w, 0);
    EXPECT_EQ(rectC.h, 0);
}

TEST(UtilsTest, Test_SetTransformX) {
    // Case A
    Entity entityA;
    entityA.addComponent<Transform>({10, 10, 1});
    entityA.addComponent<Collider>({10, 10, 10, 10});

    Utils::setTransformX(entityA, 100);

    auto& transformA = entityA.getComponent<Transform>();
    EXPECT_EQ(transformA.x, 90);

    // Case B
    Entity entityB;
    entityB.addComponent<Transform>({10, 10, 2});
    entityB.addComponent<Collider>({10, 10, 10, 10});

    Utils::setTransformX(entityB, 200);

    auto& transformB = entityB.getComponent<Transform>();
    EXPECT_EQ(transformB.x, 180);
}

TEST(UtilsTest, Test_SetTransformY) {
    // Case A
    Entity entityA;
    entityA.addComponent<Transform>({10, 10, 1});
    entityA.addComponent<Collider>({10, 10, 10, 10});

    Utils::setTransformY(entityA, 200);

    auto& transformA = entityA.getComponent<Transform>();
    EXPECT_EQ(transformA.y, 190);

    // Case B
    Entity entityB;
    entityB.addComponent<Transform>({10, 10, 2});
    entityB.addComponent<Collider>({10, 10, 10, 10});

    Utils::setTransformY(entityB, 200);

    auto& transformB = entityB.getComponent<Transform>();
    EXPECT_EQ(transformB.y, 180);
}