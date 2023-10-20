#pragma once

#include <iostream>
#include <vector>
#include "../ECS/Systems/Systems.hpp"

using entity_t = std::size_t;

namespace game {
    class EntityManager {
        public:
            EntityManager();
            ~EntityManager();

        protected:
            entity_t getPlayerEntityFromId(unsigned int id);
            entity_t getMissileEntityFromId(unsigned int id);
            entity_t getEnnemiEntityFromId(unsigned int id);

            std::vector<std::pair<size_t, entity_t>> _players;
            std::vector<std::pair<size_t, entity_t>> _missiles;
            std::vector<std::pair<size_t, entity_t>> _ennemies;
            std::vector<ECS::systems::MovableSystem::EntityPos> _entityPositions;
            std::vector<ECS::systems::ControllableSystem::EntityEvent> _entityEvents;
            std::vector<entity_t> _parallax;
            std::vector<entity_t> _buttons;
            std::vector<entity_t> _looser;
            entity_t _menuEntity;
            entity_t _playerEntity;
            entity_t _playerLife;
    };
}
