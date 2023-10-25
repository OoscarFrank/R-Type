#include "MenuManager.hpp"

using namespace game;

MenuManager::MenuManager()
{
}

MenuManager::~MenuManager()
{
}

void MenuManager::addMenu(MENU_TYPE type, entity_t entity, bool isDisplay, std::vector<BUTTON_TYPE> buttons)
{
    if (this->_menu.find(type) != this->_menu.end())
        return;
    this->_menu[type] = std::make_pair(std::make_pair(entity, isDisplay), buttons);
}

void MenuManager::removeMenu(MENU_TYPE type)
{
    if (this->_menu.find(type) == this->_menu.end())
        return;
    this->_menu.erase(type);
}

bool MenuManager::isMenuExist(MENU_TYPE type)
{
    return this->_menu.find(type) != this->_menu.end();
}

entity_t MenuManager::getMenuEntity(MENU_TYPE type)
{
    if (this->_menu.find(type) == this->_menu.end())
        return -1;
    return this->_menu[type].first.first;
}

void MenuManager::changeMenuEntity(MENU_TYPE type, entity_t entity)
{
    if (this->_menu.find(type) == this->_menu.end())
        return;
    this->_menu[type].first.first = entity;
}

void MenuManager::menuIsDisplayed(MENU_TYPE type, bool isDisplayed)
{
    if (this->_menu.find(type) == this->_menu.end())
        return;
    this->_menu[type].first.second = isDisplayed;
}

void MenuManager::enableMenu(ECS::Registry &ecs, MENU_TYPE type)
{
    if (this->_menu.find(type) == this->_menu.end())
        return;
    auto &menu = this->_menu[type];
    menu.first.second = true;
    ecs.enableEntity(menu.first.first);
    for (auto &button : menu.second)
        ecs.enableEntity(this->_buttons[button]);
}

void MenuManager::disableMenu(ECS::Registry &ecs, MENU_TYPE type)
{
    if (this->_menu.find(type) == this->_menu.end())
        return;
    auto &menu = this->_menu[type];
    menu.first.second = false;
    ecs.disableEntity(menu.first.first);
    for (auto &button : menu.second)
        ecs.disableEntity(this->_buttons[button]);
}


void MenuManager::addButton(BUTTON_TYPE type, entity_t entity)
{
    if (this->_buttons.find(type) != this->_buttons.end())
        return;
    this->_buttons[type] = entity;
}

void MenuManager::removeButton(BUTTON_TYPE type)
{
    if (this->_buttons.find(type) == this->_buttons.end())
        return;
    this->_buttons.erase(type);
}

void MenuManager::addButtonToMenu(MENU_TYPE type, BUTTON_TYPE button)
{
    if (this->_menu.find(type) == this->_menu.end())
        return;
    this->_menu[type].second.push_back(button);
}

void MenuManager::removeButtonInMenu(MENU_TYPE type, BUTTON_TYPE button)
{
    if (this->_menu.find(type) == this->_menu.end())
        return;
    auto it = std::find(this->_menu[type].second.begin(), this->_menu[type].second.end(), button);
    if (it != this->_menu[type].second.end())
        this->_menu[type].second.erase(it);
}

std::vector<MenuManager::BUTTON_TYPE> MenuManager::getMenuButtons(MENU_TYPE type)
{
    if (this->_menu.find(type) == this->_menu.end())
        return std::vector<BUTTON_TYPE>();
    return this->_menu[type].second;
}
