#pragma once

#include <iostream>
#include <vector>
#include "../../Utils.hpp"
#include "../../ECS/Registry.hpp"
#include <chrono>

using entity_t = std::size_t;

namespace game {
    class MenuManager {
        public:
            MenuManager(ECS::Registry &ecs);
            ~MenuManager();

            enum BUTTON_TYPE {
                NO_BUTTON = -1,
                CREATE_GAME,
                JOIN_GAME,
                EXIT_SYSTEM
            };

            enum MENU_TYPE {
                MAIN_MENU,
                PAUSE_MENU,
                LOOSER_MENU
            };


            void initFirstButton(BUTTON_TYPE type);
            void createMenu(MENU_TYPE type, entity_t entity, bool isDisplay, std::vector<BUTTON_TYPE> buttons);
            void removeMenu(MENU_TYPE type);

            void createButton(BUTTON_TYPE type, entity_t entity);

            entity_t getMenuEntity(MENU_TYPE type);
            void changeMenuEntity(MENU_TYPE type, entity_t entity);
            bool menuState(MENU_TYPE type);
            void enableMenu(MENU_TYPE type);
            void disableMenu(MENU_TYPE type);

            void nextButtonInMenu(MENU_TYPE type);
            void previousButtonInMenu(MENU_TYPE type);
            void executeButtonInMenu(ECS::Registry &ecs);


        private:
            std::unordered_map<BUTTON_TYPE, entity_t> _buttons;
            std::unordered_map<MENU_TYPE, std::pair<std::pair<entity_t, bool>, std::vector<BUTTON_TYPE>>> _menu; // MENU_TYPE, menu entity, isDisplayed, buttons
            BUTTON_TYPE _selectedButton;
            ECS::Registry &_ecs;
            bool checkLastButtonInput();
            long _lastButtonInput;

    };
}
