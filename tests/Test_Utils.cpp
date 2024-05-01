#include <gtest/gtest.h>
#include "../src/Utils.h"
#include "../src/ECS/Components.h"


TEST(UtilsTest, Test_stringToAction) {
    Action jumpAction = Utils::stringToAction("JUMP");
    EXPECT_EQ(jumpAction, Action::JUMP);

    Action attackAction = Utils::stringToAction("ATTACK");
    EXPECT_EQ(attackAction, Action::ATTACK);
}

TEST(UtilsTest, Test_actionToString) {
    std::string jumpAction = Utils::actionToString(Action::JUMP);
    EXPECT_EQ(jumpAction, "JUMP");

    std::string waitAction = Utils::actionToString(Action::WAIT);
    EXPECT_EQ(waitAction, "WAIT");
}

TEST(UtilsTest, Test_stringToDirection) {
    Direction leftDirection = Utils::stringToDirection("LEFT");
    EXPECT_EQ(leftDirection, Direction::LEFT);

    Direction invalidDirection = Utils::stringToDirection("INVALID");
    EXPECT_EQ(invalidDirection, Direction::STILL);
}
