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
