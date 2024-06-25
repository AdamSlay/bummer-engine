#include <gtest/gtest.h>
#include "../src/Systems/CooldownSystem.h"


TEST(CooldownSystemTest, TestDecrementDashCooldown) {
    CooldownSystem cooldownSystem;
    Entity testEntity(1);
    testEntity.addComponent<Dash>({1, true, 2.0, 6.0});
    testEntity.getComponent<Dash>().currentCooldown = 2.0;

    cooldownSystem.decrementDashCooldown(testEntity, 1.0);

    EXPECT_EQ(testEntity.getComponent<Dash>().currentCooldown, 1.0);
}