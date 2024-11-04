#include <gtest/gtest.h>
#include "../src/Systems/CollisionSystem.h"

TEST(CollisionSystemTest, CheckCollision) {
    CollisionSystem collisionSystem;

    Entity entityA(1);
    entityA.addComponent<Transform>({0, 0, 1});
    entityA.addComponent<Collider>({0, 0, 50, 50});

    Entity entityB(2);
    entityB.addComponent<Transform>({25, 25, 1});
    entityB.addComponent<Collider>({0, 0, 50, 50});

    EXPECT_TRUE(collisionSystem.checkCollision(entityA, entityB));

    entityB.getComponent<Transform>().x = 100;
    EXPECT_FALSE(collisionSystem.checkCollision(entityA, entityB));
}

TEST(CollisionSystemTest, HandleCollisionXLeft) {
    CollisionSystem collisionSystem;

    Entity entityA(1);
    entityA.addComponent<Transform>({100, 50, 1});
    entityA.addComponent<Collider>({0, 0, 50, 50});
    entityA.addComponent<Velocity>({5, 0, 1, 0});

    Entity entityB(2);
    entityB.addComponent<Transform>({140, 50, 1});
    entityB.addComponent<Collider>({0, 0, 50, 50});

    collisionSystem.handleCollisionX(entityA, entityB);

    EXPECT_EQ(entityA.getComponent<Velocity>().dx, 0);
    EXPECT_EQ(entityA.getComponent<Transform>().x, 89);
}

TEST(CollisionSystemTest, HandleCollisionXRight) {
    CollisionSystem collisionSystem;

    Entity entityA(1);
    entityA.addComponent<Transform>({100, 50, 1});
    entityA.addComponent<Collider>({0, 0, 50, 50});
    entityA.addComponent<Velocity>({-5, 0, 1, 0});

    Entity entityB(2);
    entityB.addComponent<Transform>({40, 50, 1});
    entityB.addComponent<Collider>({0, 0, 50, 50});

    collisionSystem.handleCollisionX(entityA, entityB);

    EXPECT_EQ(entityA.getComponent<Velocity>().dx, 0);
    EXPECT_EQ(entityA.getComponent<Transform>().x, 91);
}

TEST(CollisionSystemTest, HandleCollisionYAbove) {
    CollisionSystem collisionSystem;

    Entity entityA(1);
    entityA.addComponent<Transform>({50, 100, 1});
    entityA.addComponent<Collider>({0, 0, 50, 50});
    entityA.addComponent<Velocity>({0, 5, 0, 1});
    entityA.addComponent<Gravity>({1.0, 2.0, 0.5, 1.5, 0.65, 3.0});

    Entity entityB(2);
    entityB.addComponent<Transform>({50, 140, 1});
    entityB.addComponent<Collider>({0, 0, 50, 50});

    collisionSystem.handleCollisionY(entityA, entityB);

    EXPECT_EQ(entityA.getComponent<Velocity>().dy, 0);
    EXPECT_EQ(entityA.getComponent<Transform>().y, 89);
    EXPECT_EQ(entityA.getComponent<Gravity>().gravity, entityA.getComponent<Gravity>().baseGravity);
}

TEST(CollisionSystemTest, HandleCollisionYBelow) {
    CollisionSystem collisionSystem;

    Entity entityA(1);
    entityA.addComponent<Transform>({50, 100, 1});
    entityA.addComponent<Collider>({0, 0, 50, 50});
    entityA.addComponent<Velocity>({0, -5, 0, 1});

    Entity entityB(2);
    entityB.addComponent<Transform>({50, 40, 1});
    entityB.addComponent<Collider>({0, 0, 50, 50});

    collisionSystem.handleCollisionY(entityA, entityB);

    EXPECT_EQ(entityA.getComponent<Velocity>().dy, 0);
    EXPECT_EQ(entityA.getComponent<Transform>().y, 91);
}

TEST(CollisionSystemTest, CheckCollisionX) {
    CollisionSystem collisionSystem;

    Entity entityA(1);
    entityA.addComponent<Transform>({0, 0, 1});
    entityA.addComponent<Collider>({0, 0, 50, 50});

    Entity entityB(2);
    entityB.addComponent<Transform>({60, 0, 1});
    entityB.addComponent<Collider>({0, 0, 50, 50});

    EXPECT_FALSE(collisionSystem.checkCollisionX(entityA, entityB));

    entityB.getComponent<Transform>().x = 40;
    EXPECT_TRUE(collisionSystem.checkCollisionX(entityA, entityB));
}

TEST(CollisionSystemTest, isTouchingXaxis) {
    CollisionSystem collisionSystem;

    SDL_Rect rectA = {0, 0, 50, 50};
    SDL_Rect rectB = {60, 0, 50, 50};
    EXPECT_FALSE(collisionSystem.isTouchingXaxis(rectA, rectB));

    rectB.x = 40;
    EXPECT_TRUE(collisionSystem.isTouchingXaxis(rectA, rectB));
}

TEST(CollisionSystemTest, isLeftOf) {
    CollisionSystem collisionSystem;

    SDL_Rect rectA = {0, 0, 50, 50};
    SDL_Rect rectB = {60, 0, 50, 50};
    EXPECT_TRUE(collisionSystem.isLeftOf(rectA, rectB));

    rectB.x = 40;
    EXPECT_FALSE(collisionSystem.isLeftOf(rectA, rectB));
}

TEST(CollisionSystemTest, isRightOf) {
    CollisionSystem collisionSystem;

    SDL_Rect rectA = {60, 0, 50, 50};
    SDL_Rect rectB = {0, 0, 50, 50};
    EXPECT_TRUE(collisionSystem.isRightOf(rectA, rectB));

    rectA.x = 40;
    EXPECT_FALSE(collisionSystem.isRightOf(rectA, rectB));
}

TEST(CollisionSystemTest, CheckCollisionY) {
    CollisionSystem collisionSystem;

    Entity entityA(1);
    entityA.addComponent<Transform>({0, 0, 1});
    entityA.addComponent<Collider>({0, 0, 50, 50});

    Entity entityB(2);
    entityB.addComponent<Transform>({0, 60, 1});
    entityB.addComponent<Collider>({0, 0, 50, 50});

    EXPECT_FALSE(collisionSystem.checkCollisionY(entityA, entityB));

    entityB.getComponent<Transform>().y = 40;
    EXPECT_TRUE(collisionSystem.checkCollisionY(entityA, entityB));
}

TEST(CollisionSystemTest, isTouchingYaxis) {
    CollisionSystem collisionSystem;

    SDL_Rect rectA = {0, 0, 50, 50};
    SDL_Rect rectB = {0, 60, 50, 50};
    EXPECT_FALSE(collisionSystem.isTouchingYaxis(rectA, rectB));

    rectB.y = 40;
    EXPECT_TRUE(collisionSystem.isTouchingYaxis(rectA, rectB));
}

TEST(CollisionSystemTest, isAbove) {
    CollisionSystem collisionSystem;

    SDL_Rect rectA = {0, 0, 50, 50};
    SDL_Rect rectB = {0, 60, 50, 50};
    EXPECT_TRUE(collisionSystem.isAbove(rectA, rectB));

    rectB.y = 40;
    EXPECT_FALSE(collisionSystem.isAbove(rectA, rectB));
}

TEST(CollisionSystemTest, isBelow) {
    CollisionSystem collisionSystem;

    SDL_Rect rectA = {0, 60, 50, 50};
    SDL_Rect rectB = {0, 0, 50, 50};
    EXPECT_TRUE(collisionSystem.isBelow(rectA, rectB));

    rectA.y = 40;
    EXPECT_FALSE(collisionSystem.isBelow(rectA, rectB));
}

TEST(CollisionSystemTest, StopAndRepositionToLeft) {
    CollisionSystem collisionSystem;

    Entity entityA(1);
    entityA.addComponent<Transform>({100, 50, 1});
    entityA.addComponent<Collider>({0, 0, 50, 50});
    entityA.addComponent<Velocity>({5, 0, 1, 0});

    Entity entityB(2);
    entityB.addComponent<Transform>({140, 50, 1});
    entityB.addComponent<Collider>({0, 0, 50, 50});

    collisionSystem.stopAndRepositionToLeft(entityA, entityA.getColliderRect(), entityB.getColliderRect());

    EXPECT_EQ(entityA.getComponent<Velocity>().dx, 0);
    EXPECT_EQ(entityA.getComponent<Transform>().x, 89);
}

TEST(CollisionSystemTest, StopAndRepositionToRight) {
    CollisionSystem collisionSystem;

    Entity entityA(1);
    entityA.addComponent<Transform>({100, 50, 1});
    entityA.addComponent<Collider>({0, 0, 50, 50});
    entityA.addComponent<Velocity>({-5, 0, 1, 0});

    Entity entityB(2);
    entityB.addComponent<Transform>({40, 50, 1});
    entityB.addComponent<Collider>({0, 0, 50, 50});

    collisionSystem.stopAndRepositionToRight(entityA, entityB.getColliderRect());

    EXPECT_EQ(entityA.getComponent<Velocity>().dx, 0);
    EXPECT_EQ(entityA.getComponent<Transform>().x, 91);
}

TEST(CollisionSystemTest, StopAndRepositionAbove) {
    CollisionSystem collisionSystem;

    Entity entityA(1);
    entityA.addComponent<Transform>({50, 100, 1});
    entityA.addComponent<Collider>({0, 0, 50, 50});
    entityA.addComponent<Velocity>({0, 5, 0, 1});
    entityA.addComponent<Gravity>({1.0, 2.0, 0.5, 1.5, 0.65, 3.0});

    Entity entityB(2);
    entityB.addComponent<Transform>({50, 140, 1});
    entityB.addComponent<Collider>({0, 0, 50, 50});

    collisionSystem.stopAndRepositionAbove(entityA, entityA.getColliderRect(), entityB.getColliderRect());

    EXPECT_EQ(entityA.getComponent<Velocity>().dy, 0);
    EXPECT_EQ(entityA.getComponent<Transform>().y, 89);
    EXPECT_EQ(entityA.getComponent<Gravity>().gravity, entityA.getComponent<Gravity>().baseGravity);
}

TEST(CollisionSystemTest, StopAndRepositionBelow) {
    CollisionSystem collisionSystem;

    Entity entityA(1);
    entityA.addComponent<Transform>({50, 100, 1});
    entityA.addComponent<Collider>({0, 0, 50, 50});
    entityA.addComponent<Velocity>({0, -5, 0, 1});

    Entity entityB(2);
    entityB.addComponent<Transform>({50, 40, 1});
    entityB.addComponent<Collider>({0, 0, 50, 50});

    collisionSystem.stopAndRepositionBelow(entityA, entityB.getColliderRect());

    EXPECT_EQ(entityA.getComponent<Velocity>().dy, 0);
    EXPECT_EQ(entityA.getComponent<Transform>().y, 91);
}