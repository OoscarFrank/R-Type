#pragma once

#include <iostream>
#include <vector>
#include "../../ECS/Systems/Systems.hpp"

using entity_t = std::size_t;

namespace game {
    class MenuManager {
        public:
            MenuManager();
            ~MenuManager();

            enum BUTTON_TYPE {
                CREATE_GAME,
                JOIN_GAME,
                EXIT_SYSTEM
            };

            enum MENU_TYPE {
                MAIN_MENU,
                PAUSE_MENU,
                LOOSER_MENU
            };

        protected:
            void addMenu(MENU_TYPE type, entity_t entity, bool isDisplay, std::vector<BUTTON_TYPE> buttons);
            void removeMenu(MENU_TYPE type);
            bool isMenuExist(MENU_TYPE type);
            entity_t getMenuEntity(MENU_TYPE type);
            void changeMenuEntity(MENU_TYPE type, entity_t entity);
            void menuIsDisplayed(MENU_TYPE type, bool isDisplayed);
            void enableMenu(ECS::Registry &ecs, MENU_TYPE type);
            void disableMenu(ECS::Registry &ecs, MENU_TYPE type);

            void addButton(BUTTON_TYPE type, entity_t entity);
            void removeButton(BUTTON_TYPE type);
            void addButtonToMenu(MENU_TYPE type, BUTTON_TYPE button);
            void removeButtonInMenu(MENU_TYPE type, BUTTON_TYPE button);


            std::vector<BUTTON_TYPE> getMenuButtons(MENU_TYPE type);


            std::unordered_map<BUTTON_TYPE, entity_t> _buttons;
            std::unordered_map<MENU_TYPE, std::pair<std::pair<entity_t, bool>, std::vector<BUTTON_TYPE>>> _menu; // MENU_TYPE, menu entity, isDisplayed, buttons
    };
}
