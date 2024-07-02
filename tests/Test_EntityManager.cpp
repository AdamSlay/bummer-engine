#include <gtest/gtest.h>
#include "../src/ECS/EntityManager.h"


TEST(EntityManagerTest, TestCreateEntity) {
    // Arrange
    TextureManager textureManager;
    SDL_Renderer* renderer = SDL_CreateRenderer(SDL_CreateWindow("", 0, 0, 0, 0, 0), -1, 0);
    EntityManager entityManager(&textureManager, renderer);

    // Act
    Entity entity = entityManager.createEntity();
    Entity entity1 = entityManager.createEntity();

    // Assert
    ASSERT_TRUE(entityManager.getEntities().size() == 2);
    ASSERT_EQ(entityManager.getEntities()[0].getID(), 0);
    ASSERT_EQ(entity.getID(), 0);
    ASSERT_EQ(entityManager.getEntities()[1].getID(), 1);
    ASSERT_EQ(entity1.getID(), 1);

    // Cleanup
    SDL_DestroyRenderer(renderer);
}

TEST(EntityManagerTest, TestRemoveEntity) {
    // Arrange
    TextureManager textureManager;
    SDL_Renderer* renderer = SDL_CreateRenderer(SDL_CreateWindow("", 0, 0, 0, 0, 0), -1, 0);
    EntityManager entityManager(&textureManager, renderer);

    // Create a few entities
    Entity entity1 = entityManager.createEntity();
    Entity entity2 = entityManager.createEntity();
    Entity entity3 = entityManager.createEntity();


    ASSERT_EQ(entity1.getID(), 0);
    ASSERT_EQ(entity2.getID(), 1);
    ASSERT_EQ(entity3.getID(), 2);

    // Act
    entityManager.removeEntity(entity2.getID());

    // Assert
    auto& entities = entityManager.getEntities();
    ASSERT_EQ(entities.size(), 2);
    ASSERT_EQ(entities[0].getID(), entity1.getID());
    ASSERT_EQ(entities[1].getID(), entity3.getID());

    // Cleanup
    SDL_DestroyRenderer(renderer);
}

TEST(EntityManagerTest, TestGetEntities) {
    // Arrange
    TextureManager textureManager;
    SDL_Renderer* renderer = SDL_CreateRenderer(SDL_CreateWindow("", 0, 0, 0, 0, 0), -1, 0);
    EntityManager entityManager(&textureManager, renderer);

    // Create some entities
    Entity entity1 = entityManager.createEntity();
    Entity entity2 = entityManager.createEntity();
    Entity entity3 = entityManager.createEntity();

    // Act
    std::vector<Entity>& entities = entityManager.getEntities();

    // Assert
    ASSERT_EQ(entities.size(), 3);
    ASSERT_EQ(entities[0].getID(), entity1.getID());
    ASSERT_EQ(entities[1].getID(), entity2.getID());
    ASSERT_EQ(entities[2].getID(), entity3.getID());

    // Cleanup
    SDL_DestroyRenderer(renderer);
}

TEST(EntityManagerTest, TestGetEntityById) {
    // Arrange
    TextureManager textureManager;
    SDL_Renderer* renderer = SDL_CreateRenderer(SDL_CreateWindow("", 0, 0, 0, 0, 0), -1, 0);
    EntityManager entityManager(&textureManager, renderer);

    // Create some entities
    Entity entity1 = entityManager.createEntity();
    Entity entity2 = entityManager.createEntity();
    Entity entity3 = entityManager.createEntity();

    // Act
    Entity retrievedEntity1 = entityManager.getEntityById(entity1.getID());
    Entity retrievedEntity2 = entityManager.getEntityById(entity2.getID());
    Entity retrievedEntity3 = entityManager.getEntityById(entity3.getID());

    // Assert
    ASSERT_EQ(retrievedEntity1.getID(), entity1.getID());
    ASSERT_EQ(retrievedEntity2.getID(), entity2.getID());
    ASSERT_EQ(retrievedEntity3.getID(), entity3.getID());

    // Cleanup
    SDL_DestroyRenderer(renderer);
}

TEST(EntityManagerTest, TestGetPlayer) {
    // Arrange
    TextureManager textureManager;
    SDL_Renderer* renderer = SDL_CreateRenderer(SDL_CreateWindow("", 0, 0, 0, 0, 0), -1, 0);
    EntityManager entityManager(&textureManager, renderer);

    // Create a player entity
    Entity& playerEntity = entityManager.createEntity();
    playerEntity.addComponent<Player>({1});

    // Act
    Entity& retrievedPlayerEntity = entityManager.getPlayer();

    // Assert
    ASSERT_EQ(retrievedPlayerEntity.getID(), playerEntity.getID());

    // Cleanup
    SDL_DestroyRenderer(renderer);
}

TEST(EntityManagerTest, TestClearEntities) {
    // Arrange
    TextureManager textureManager;
    SDL_Renderer* renderer = SDL_CreateRenderer(SDL_CreateWindow("", 0, 0, 0, 0, 0), -1, 0);
    EntityManager entityManager(&textureManager, renderer);

    // Create a few entities
    entityManager.createEntity();
    entityManager.createEntity();
    entityManager.createEntity();

    // Act
    entityManager.clearEntities();

    // Assert
    ASSERT_TRUE(entityManager.getEntities().empty());

    // Cleanup
    SDL_DestroyRenderer(renderer);
}

TEST(EntityManagerTest, TestGetCollidableEntities) {
    // Arrange
    TextureManager textureManager;
    SDL_Renderer* renderer = SDL_CreateRenderer(SDL_CreateWindow("", 0, 0, 0, 0, 0), -1, 0);
    EntityManager entityManager(&textureManager, renderer);

    // Create a few entities with Collider components
    Entity& entity1 = entityManager.createEntity();
    entity1.addComponent<Collider>({0,0,10,10});

    Entity& entity2 = entityManager.createEntity();
    entity2.addComponent<Collider>({10, 10, 10, 10});

    // Create a few entities without Collider components
    entityManager.createEntity();
    entityManager.createEntity();

    // Act
    std::vector<Entity>& collidableEntities = entityManager.getCollidableEntities();

    // Assert
    ASSERT_EQ(collidableEntities.size(), 2);

    // Cleanup
    SDL_DestroyRenderer(renderer);
}

TEST(EntityManagerTest, TestGetMovableCollidableEntities) {
    // Arrange
    TextureManager textureManager;
    SDL_Renderer* renderer = SDL_CreateRenderer(SDL_CreateWindow("", 0, 0, 0, 0, 0), -1, 0);
    EntityManager entityManager(&textureManager, renderer);

    // Create a few entities with Collider components
    Entity& entity1 = entityManager.createEntity();
    entity1.addComponent<Collider>({0,0,10,10});
    entity1.addComponent<Transform>({0, 0, 1.0});
    entity1.addComponent<Velocity>({1, 1, 1, 1});


    Entity& entity2 = entityManager.createEntity();
    entity2.addComponent<Collider>({10, 10, 10, 10});
    entity2.addComponent<Transform>({10, 10, 1.0});
    entity2.addComponent<Velocity>({1, 1, 1, 1});

    // Create a few entities without Collider components
    entityManager.createEntity();
    entityManager.createEntity();

    // Act
    std::vector<Entity>& movableCollidableEntities = entityManager.getMovableCollidableEntities();

    // Assert
    ASSERT_EQ(movableCollidableEntities.size(), 2);

    // Cleanup
    SDL_DestroyRenderer(renderer);
}

TEST(EntityManagerTest, TestCreateEntityFromTemplate) {
    // Arrange
    TextureManager textureManager;
    SDL_Renderer* renderer = SDL_CreateRenderer(SDL_CreateWindow("", 0, 0, 0, 0, 0), -1, 0);
    EntityManager entityManager(&textureManager, renderer);
    std::string templatePath = "../../tests/data/test_template.json";  // Replace with the path to your test template

    // Act
    Entity& entity = entityManager.createEntityFromTemplate(templatePath);

    // Assert
    ASSERT_TRUE(entity.hasComponent<Transform>());
    ASSERT_EQ(entity.getComponent<Transform>().x, 590);
    ASSERT_EQ(entity.getComponent<Transform>().y, 50);
    ASSERT_EQ(entity.getComponent<Transform>().scale, 2);

    ASSERT_TRUE(entity.hasComponent<Collider>());
    ASSERT_EQ(entity.getComponent<Collider>().offsetX, 24);
    ASSERT_EQ(entity.getComponent<Collider>().offsetY, 55);
    ASSERT_EQ(entity.getComponent<Collider>().width, 16);
    ASSERT_EQ(entity.getComponent<Collider>().height, 40);

    // Cleanup
    SDL_DestroyRenderer(renderer);
}

TEST(EntityManagerTest, TestLoadTemplateFile) {
    // Arrange
    TextureManager textureManager;
    SDL_Renderer* renderer = SDL_CreateRenderer(SDL_CreateWindow("", 0, 0, 0, 0, 0), -1, 0);
    EntityManager entityManager(&textureManager, renderer);
    std::string templatePath = "../../tests/data/test_template.json";  // Replace with the path to your test template

    // Act
    nlohmann::ordered_json templateJson = entityManager.loadTemplateFile(templatePath);

    // Assert
    ASSERT_TRUE(templateJson.contains("components"));
    ASSERT_TRUE(templateJson["components"].contains("Transform"));
    ASSERT_EQ(templateJson["components"]["Transform"]["x"], 590);
    ASSERT_EQ(templateJson["components"]["Transform"]["y"], 50);
    ASSERT_EQ(templateJson["components"]["Transform"]["scale"], 2);

    // Cleanup
    SDL_DestroyRenderer(renderer);
}

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
    componentJson["texturePath"] = "../../tests/data/test_sprite.png";
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

TEST(EntityManagerTest, TestAddComponentJumps) {
    // Arrange
    TextureManager textureManager;
    SDL_Renderer* renderer = SDL_CreateRenderer(SDL_CreateWindow("", 0, 0, 0, 0, 0), -1, 0);
    EntityManager entityManager(&textureManager, renderer);
    Entity& entity = entityManager.createEntity();
    nlohmann::ordered_json componentJson;
    componentJson["jumps"] = 1;
    componentJson["maxJumps"] = 2;
    componentJson["jumpVelocity"] = 3;

    // Act
    entityManager.addComponentJumps(entity, componentJson);

    // Assert
    ASSERT_TRUE(entity.hasComponent<Jumps>());
    ASSERT_EQ(entity.getComponent<Jumps>().jumps, 1);
    ASSERT_EQ(entity.getComponent<Jumps>().maxJumps, 2);
    ASSERT_EQ(entity.getComponent<Jumps>().jumpVelocity, 3);

    // Cleanup
    SDL_DestroyRenderer(renderer);
}

TEST(EntityManagerTest, TestAddComponentDash) {
    // Arrange
    TextureManager textureManager;
    SDL_Renderer* renderer = SDL_CreateRenderer(SDL_CreateWindow("", 0, 0, 0, 0, 0), -1, 0);
    EntityManager entityManager(&textureManager, renderer);
    Entity& entity = entityManager.createEntity();
    nlohmann::ordered_json componentJson;
    componentJson["speed"] = 10;
    componentJson["isDashing"] = false;
    componentJson["initCooldown"] = 1.0f;
    componentJson["initDuration"] = 2.0f;

    // Act
    entityManager.addComponentDash(entity, componentJson);

    // Assert
    ASSERT_TRUE(entity.hasComponent<Dash>());
    ASSERT_EQ(entity.getComponent<Dash>().speed, 10);
    ASSERT_EQ(entity.getComponent<Dash>().isDashing, false);
    ASSERT_EQ(entity.getComponent<Dash>().initCooldown, 1.0f);
    ASSERT_EQ(entity.getComponent<Dash>().initDuration, 2.0f);
    ASSERT_EQ(entity.getComponent<Dash>().currentDuration, 2.0f);
    ASSERT_EQ(entity.getComponent<Dash>().currentCooldown, 0.0f);

    // Cleanup
    SDL_DestroyRenderer(renderer);
}

TEST(EntityManagerTest, TestAddComponentHealth) {
    // Arrange
    TextureManager textureManager;
    SDL_Renderer* renderer = SDL_CreateRenderer(SDL_CreateWindow("", 0, 0, 0, 0, 0), -1, 0);
    EntityManager entityManager(&textureManager, renderer);
    Entity& entity = entityManager.createEntity();
    nlohmann::ordered_json componentJson;
    componentJson["maxHealth"] = 100;
    componentJson["currentHealth"] = 80;
    componentJson["invincibilityFrames"] = 10;

    // Act
    entityManager.addComponentHealth(entity, componentJson);

    // Assert
    ASSERT_TRUE(entity.hasComponent<Health>());
    ASSERT_EQ(entity.getComponent<Health>().maxHealth, 100);
    ASSERT_EQ(entity.getComponent<Health>().currentHealth, 80);
    ASSERT_EQ(entity.getComponent<Health>().invincibilityFrames, 10);

    // Cleanup
    SDL_DestroyRenderer(renderer);
}

TEST(EntityManagerTest, TestAddComponentState) {
    // Arrange
    TextureManager textureManager;
    SDL_Renderer* renderer = SDL_CreateRenderer(SDL_CreateWindow("", 0, 0, 0, 0, 0), -1, 0);
    EntityManager entityManager(&textureManager, renderer);
    Entity& entity = entityManager.createEntity();
    nlohmann::ordered_json componentJson;
    componentJson["state"] = "IDLE";

    // Act
    entityManager.addComponentState(entity, componentJson);

    // Assert
    ASSERT_TRUE(entity.hasComponent<State>());
    ASSERT_EQ(entity.getComponent<State>().state, EntityManager::playerStatesMap["IDLE"]);

    // Cleanup
    SDL_DestroyRenderer(renderer);
}

TEST(EntityManagerTest, TestAddComponentAI) {
    // Arrange
    TextureManager textureManager;
    SDL_Renderer* renderer = SDL_CreateRenderer(SDL_CreateWindow("", 0, 0, 0, 0, 0), -1, 0);
    EntityManager entityManager(&textureManager, renderer);
    Entity& entity = entityManager.createEntity();

    // Add a Transform component to the entity
    nlohmann::ordered_json transformJson;
    transformJson["x"] = 0;
    transformJson["y"] = 0;
    transformJson["scale"] = 1.0;
    entityManager.addComponentTransform(entity, transformJson);

    nlohmann::ordered_json componentJson;
    componentJson["state"] = "IDLE";
    componentJson["patrolRange"] = 100.0;
    componentJson["attackRange"] = 50.0;
    componentJson["pursuitRange"] = 150.0;

    // Act
    entityManager.addComponentAI(entity, componentJson);

    // Assert
    ASSERT_TRUE(entity.hasComponent<AI>());
    ASSERT_EQ(entity.getComponent<AI>().state, "IDLE");
    ASSERT_EQ(entity.getComponent<AI>().patrolRange, 100.0);
    ASSERT_EQ(entity.getComponent<AI>().attackRange, 50.0);
    ASSERT_EQ(entity.getComponent<AI>().pursuitRange, 150.0);

    // Cleanup
    SDL_DestroyRenderer(renderer);
}

TEST(EntityManagerTest, TestAddComponentAttackMap) {
    // Arrange
    TextureManager textureManager;
    SDL_Renderer* renderer = SDL_CreateRenderer(SDL_CreateWindow("", 0, 0, 0, 0, 0), -1, 0);
    EntityManager entityManager(&textureManager, renderer);
    Entity& entity = entityManager.createEntity();
    nlohmann::ordered_json componentJson;
    componentJson["attack1"] = "../../etc/templates/attacks/alien_basic.json";
    componentJson["attack2"] = "../../etc/templates/attacks/basic.json";

    // Act
    entityManager.addComponentAttackMap(entity, componentJson);

    // Assert
    ASSERT_TRUE(entity.hasComponent<AttackMap>());
    auto& attackMap = entity.getComponent<AttackMap>().attacks;
    ASSERT_TRUE(attackMap.find("attack1") != attackMap.end());
    ASSERT_TRUE(attackMap.find("attack2") != attackMap.end());

    // Cleanup
    SDL_DestroyRenderer(renderer);
}

TEST(EntityManagerTest, TestAddComponentAnimator) {
    // Arrange
    TextureManager textureManager;
    SDL_Renderer* renderer = SDL_CreateRenderer(SDL_CreateWindow("", 0, 0, 0, 0, 0), -1, 0);
    EntityManager entityManager(&textureManager, renderer);
    Entity& entity = entityManager.createEntity();
    nlohmann::ordered_json componentJson;
    componentJson["animatorPath"] = "../../tests/data/test_anim.json";

    // Act
    entityManager.addComponentAnimator(entity, componentJson);

    // Assert
    ASSERT_TRUE(entity.hasComponent<Animator>());
    auto& animations = entity.getComponent<Animator>().animations;
    ASSERT_TRUE(animations.find(EntityManager::playerStatesMap["IDLE"]) != animations.end());

    // Cleanup
    SDL_DestroyRenderer(renderer);
}