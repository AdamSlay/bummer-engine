#ifndef BUMMERENGINE_UTILS_H
#define BUMMERENGINE_UTILS_H

#include <utility>

#include <SDL2/SDL.h>

#include "ECS/Components.h"
#include "ECS/EntityManager.h"

namespace Utils {
    /**
     * Utility functions
     */

    std::pair<int, int> getColliderPos(const Entity& entity) {
        /**
         * Get the position of the collider
         *
         * @param entity: The entity
         * @return: A pair of integers representing the x and y position of the collider
         */
        const Transform& transform = entity.getComponent<Transform>();
        const Collider& collider = entity.getComponent<Collider>();
        int x = transform.x + (collider.offsetX * transform.scale);
        int y = transform.y + (collider.offsetY * transform.scale);
        return {x, y};
    }
}

#endif //BUMMERENGINE_UTILS_H
