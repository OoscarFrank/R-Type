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
        /**
         * @brief Construct a new Menu Manager object
         * 
         * @param ecs 
         */
            MenuManager(ECS::Registry &ecs);
        /**
         * @brief Destroy the Menu Manager object
         * 
         */
            ~MenuManager();
        /**
         * @brief Enum of the button type
         * 
         */
            enum BUTTON_TYPE {
                NO_BUTTON = -1,
                CREATE_GAME,
                JOIN_GAME,
                EXIT_SYSTEM,
                LEAVE_GAME,
                ROOM_LIST_0,
                ROOM_LIST_1,
                ROOM_LIST_2,
                ROOM_LIST_3,
                ROOM_LIST_4,
                ROOM_LIST_5,
            };
        /**
         * @brief Enum of the menu type
         * 
         */
            enum MENU_TYPE {
                MAIN_MENU,
                LOOSER_MENU,
                COUNT
            };
        /**
         * @brief Set the Selected Button object
         * 
         * @param type 
         */
            void setSelectedButton(BUTTON_TYPE type);
        /**
         * @brief Create a Menu object
         * 
         * @param type 
         * @param entity 
         * @param isDisplay 
         * @param buttons 
         */
            void createMenu(MENU_TYPE type, entity_t entity, bool isDisplay, std::vector<BUTTON_TYPE> buttons);
        /**
         * @brief Remove a Menu object
         * 
         * @param type 
         */
            void removeMenu(MENU_TYPE type);
        /**
         * @brief Create a Button object
         * 
         * @param type 
         * @param entity 
         */
            void createButton(BUTTON_TYPE type, entity_t entity);
        /**
         * @brief Get the Menu Entity object
         * 
         * @param type 
         * @return entity_t 
         */
            entity_t getMenuEntity(MENU_TYPE type);
        /**
         * @brief Change the Menu Entity object
         * 
         * @param type 
         * @param entity 
         */
            void changeMenuEntity(MENU_TYPE type, entity_t entity);
        /**
         * @brief Get the menu state
         * 
         * @param type 
         * @return true 
         * @return false 
         */
            bool menuState(MENU_TYPE type);
        /**
         * @brief Enable a menu
         * 
         * @param type 
         */
            void enableMenu(MENU_TYPE type);
        /**
         * @brief Disable a menu
         * 
         * @param type 
         */
            void disableMenu(MENU_TYPE type);
        /**
         * @brief Disable all menus
         * 
         */
            void disableAllmenu();
        /**
         * @brief Get the next button in menu
         * 
         * @param type 
         */
            void nextButtonInMenu(MENU_TYPE type);
        /**
         * @brief Get the previous button in menu
         * 
         * @param type 
         */
            void previousButtonInMenu(MENU_TYPE type);
        /**
         * @brief Execute the button in menu
         * 
         * @param ecs 
         * @return true 
         * @return false 
         */
            bool executeButtonInMenu(ECS::Registry &ecs);
        private:
        /**
         * @brief Store the buttons
         * 
         */
            std::unordered_map<BUTTON_TYPE, entity_t> _buttons;
        /**
         * @brief Store the menus
         * 
         */
            std::unordered_map<MENU_TYPE, std::pair<std::pair<entity_t, bool>, std::vector<BUTTON_TYPE>>> _menu; // MENU_TYPE, menu entity, isDisplayed, buttons
        /**
         * @brief Is button selected
         * 
         */
            BUTTON_TYPE _selectedButton;
        /**
         * @brief Instance of the ECS
         * 
         */
            ECS::Registry &_ecs;
        /**
         * @brief Check last button input
         * 
         * @return true 
         * @return false 
         */
            bool checkLastButtonInput();
        /**
         * @brief Store the last button input
         * 
         */
            long _lastButtonInput;

    };
}
