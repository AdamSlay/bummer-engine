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

TEST(EntityManagerTest, TestAddComponentTransform) {
    // Arrange
    TextureManager textureManager;
    SDL_Renderer* renderer = SDL_CreateRenderer(SDL_CreateWindow("", 0, 0, 0, 0, 0), -1, 0);
    EntityManager entityManager(&textureManager, renderer);
    Entity& entity = entityManager.createEntity();
    nlohmann::ordered_json componentJson;
    componentJson["x"] = 100;
    componentJson["y"] = 200;
    componentJson["scale"] = 1.0;

    // Act
    entityManager.addComponentTransform(entity, componentJson);

    // Assert
    ASSERT_TRUE(entity.hasComponent<Transform>());
    ASSERT_EQ(entity.getComponent<Transform>().x, 100);
    ASSERT_EQ(entity.getComponent<Transform>().y, 200);
    ASSERT_EQ(entity.getComponent<Transform>().scale, 1.0);

    // Cleanup
    SDL_DestroyRenderer(renderer);
}

TEST(EntityManagerTest, TestAddComponentCollider) {
    // Arrange
    TextureManager textureManager;
    SDL_Renderer* renderer = SDL_CreateRenderer(SDL_CreateWindow("", 0, 0, 0, 0, 0), -1, 0);
    EntityManager entityManager(&textureManager, renderer);
    Entity& entity = entityManager.createEntity();
    nlohmann::ordered_json componentJson;
    componentJson["offsetX"] = 10;
    componentJson["offsetY"] = 20;
    componentJson["width"] = 30;
    componentJson["height"] = 40;

    // Act
    entityManager.addComponentCollider(entity, componentJson);

    // Assert
    ASSERT_TRUE(entity.hasComponent<Collider>());
    ASSERT_EQ(entity.getComponent<Collider>().offsetX, 10);
    ASSERT_EQ(entity.getComponent<Collider>().offsetY, 20);
    ASSERT_EQ(entity.getComponent<Collider>().width, 30);
    ASSERT_EQ(entity.getComponent<Collider>().height, 40);

    // Cleanup
    SDL_DestroyRenderer(renderer);
}

TEST(EntityManagerTest, TestAddComponentSprite) {
    // Arrange
    TextureManager textureManager;
    SDL_Renderer* renderer = SDL_CreateRenderer(SDL_CreateWindow("", 0, 0, 0, 0, 0), -1, 0);
    EntityManager entityManager(&textureManager, renderer);
    Entity& entity = entityManager.createEntity();
    nlohmann::ordered_json componentJson;
    componentJson["texturePath"] = "path/to/texture.png";
    componentJson["srcRect"]["x"] = 0;
    componentJson["srcRect"]["y"] = 0;
    componentJson["srcRect"]["w"] = 32;
    componentJson["srcRect"]["h"] = 32;

    // Act
    entityManager.addComponentSprite(entity, componentJson);

    // Assert
    ASSERT_TRUE(entity.hasComponent<Sprite>());
    ASSERT_EQ(entity.getComponent<Sprite>().srcRect.x, 0);
    ASSERT_EQ(entity.getComponent<Sprite>().srcRect.y, 0);
    ASSERT_EQ(entity.getComponent<Sprite>().srcRect.w, 32);
    ASSERT_EQ(entity.getComponent<Sprite>().srcRect.h, 32);

    // Cleanup
    SDL_DestroyRenderer(renderer);
}

TEST(EntityManagerTest, TestAddComponentVelocity) {
    // Arrange
    TextureManager textureManager;
    SDL_Renderer* renderer = SDL_CreateRenderer(SDL_CreateWindow("", 0, 0, 0, 0, 0), -1, 0);
    EntityManager entityManager(&textureManager, renderer);
    Entity& entity = entityManager.createEntity();
    nlohmann::ordered_json componentJson;
    componentJson["dx"] = 1.0;
    componentJson["dy"] = 2.0;
    componentJson["direction"] = 1;
    componentJson["speed"] = 3.0;

    // Act
    entityManager.addComponentVelocity(entity, componentJson);

    // Assert
    ASSERT_TRUE(entity.hasComponent<Velocity>());
    ASSERT_EQ(entity.getComponent<Velocity>().dx, 1.0);
    ASSERT_EQ(entity.getComponent<Velocity>().dy, 2.0);
    ASSERT_EQ(entity.getComponent<Velocity>().direction, 1);
    ASSERT_EQ(entity.getComponent<Velocity>().speed, 3.0);

    // Cleanup
    SDL_DestroyRenderer(renderer);
}

TEST(EntityManagerTest, TestAddComponentInput) {
    // Arrange
    TextureManager textureManager;
    SDL_Renderer* renderer = SDL_CreateRenderer(SDL_CreateWindow("", 0, 0, 0, 0, 0), -1, 0);
    EntityManager entityManager(&textureManager, renderer);
    Entity& entity = entityManager.createEntity();
    nlohmann::ordered_json componentJson;

    // Act
    entityManager.addComponentInput(entity, componentJson);

    // Assert
    ASSERT_TRUE(entity.hasComponent<Input>());

    // Cleanup
    SDL_DestroyRenderer(renderer);
}

TEST(EntityManagerTest, TestAddComponentGravity) {
    // Arrange
    TextureManager textureManager;
    SDL_Renderer* renderer = SDL_CreateRenderer(SDL_CreateWindow("", 0, 0, 0, 0, 0), -1, 0);
    EntityManager entityManager(&textureManager, renderer);
    Entity& entity = entityManager.createEntity();
    nlohmann::ordered_json componentJson;
    componentJson["baseGravity"] = 1.0;
    componentJson["gravity"] = 2.0;
    componentJson["ascendFactor"] = 3.0;
    componentJson["descendFactor"] = 4.0;

    // Act
    entityManager.addComponentGravity(entity, componentJson);

    // Assert
    ASSERT_TRUE(entity.hasComponent<Gravity>());
    ASSERT_EQ(entity.getComponent<Gravity>().baseGravity, 1.0);
    ASSERT_EQ(entity.getComponent<Gravity>().gravity, 2.0);
    ASSERT_EQ(entity.getComponent<Gravity>().ascendFactor, 3.0);
    ASSERT_EQ(entity.getComponent<Gravity>().descendFactor, 4.0);

    // Cleanup
    SDL_DestroyRenderer(renderer);
}