#include <gtest/gtest.h>
#include "../src/ECS/EntityManager.h"

TEST(EntityManagerTest, TestAddComponentPlayer) {
    // Arrange
    TextureManager textureManager;
    SDL_Renderer* renderer = SDL_CreateRenderer(SDL_CreateWindow("", 0, 0, 0, 0, 0), -1, 0);
    EntityManager entityManager(&textureManager, renderer);
    Entity& entity = entityManager.createEntity();
    nlohmann::ordered_json componentJson;
    componentJson["playerNum"] = 1;

    // Act
    entityManager.addComponentPlayer(entity, componentJson);

    // Assert
    ASSERT_TRUE(entity.hasComponent<Player>());
    ASSERT_EQ(entity.getComponent<Player>().playerNumber, 1);

    // Cleanup
    SDL_DestroyRenderer(renderer);
}

TEST(EntityManagerTest, TestAddComponentIntent) {
    // Arrange
    TextureManager textureManager;
    SDL_Renderer* renderer = SDL_CreateRenderer(SDL_CreateWindow("", 0, 0, 0, 0, 0), -1, 0);
    EntityManager entityManager(&textureManager, renderer);
    Entity& entity = entityManager.createEntity();
    nlohmann::ordered_json componentJson;
    componentJson["action"] = "MOVE_RIGHT";
    componentJson["direction"] = "RIGHT";

    // Act
    entityManager.addComponentIntent(entity, componentJson);

    // Assert
    ASSERT_TRUE(entity.hasComponent<Intent>());
    ASSERT_EQ(entity.getComponent<Intent>().action, Action::MOVE_RIGHT);
    ASSERT_EQ(entity.getComponent<Intent>().direction, Direction::RIGHT);

    // Cleanup
    SDL_DestroyRenderer(renderer);
}

TEST(EntityManagerTest, TestAddComponentNpc) {
    // Arrange
    TextureManager textureManager;
    SDL_Renderer* renderer = SDL_CreateRenderer(SDL_CreateWindow("", 0, 0, 0, 0, 0), -1, 0);
    EntityManager entityManager(&textureManager, renderer);
    Entity& entity = entityManager.createEntity();
    nlohmann::ordered_json componentJson;
    componentJson["type"] = "Alien";

    // Act
    entityManager.addComponentNpc(entity, componentJson);

    // Assert
    ASSERT_TRUE(entity.hasComponent<Npc>());
    ASSERT_EQ(entity.getComponent<Npc>().type, "Alien");

    // Cleanup
    SDL_DestroyRenderer(renderer);
}