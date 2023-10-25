#include "MenuManager.hpp"

using namespace game;

MenuManager::MenuManager()
{
    this->_lastButtonInput = NOW;
    this->_selectedButton = CREATE_GAME;
}

MenuManager::~MenuManager()
{
}

void MenuManager::createMenu(MENU_TYPE type, entity_t entity, bool isDisplay, std::vector<BUTTON_TYPE> buttons)
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

bool MenuManager::menuState(MENU_TYPE type)
{
    if (this->_menu.find(type) == this->_menu.end())
        return false;
    return this->_menu[type].first.second;
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

void MenuManager::nextButtonInMenu(ECS::Registry &ecs, MENU_TYPE type)
{
    auto now = NOW;
    if (now - this->_lastButtonInput < 150)
        return;
    this->_lastButtonInput = now;

    if (this->_menu.find(type) == this->_menu.end())
        return;
    auto &menu = this->_menu[type];
    auto &buttons = menu.second;
    if (buttons.size() == 0)
        return;
    auto it = std::find(buttons.begin(), buttons.end(), this->_selectedButton);
    if (it == buttons.end())
        return;
    ecs.modify_component<ECS::components::TextureRectComponent>(this->_buttons[this->_selectedButton], [](ECS::components::TextureRectComponent &comp) {
        comp.setFrameOnTexture(0);
    });
    if (it + 1 == buttons.end())
        this->_selectedButton = buttons[0];
    else
        this->_selectedButton = *(it + 1);
    ecs.modify_component<ECS::components::TextureRectComponent>(this->_buttons[this->_selectedButton], [](ECS::components::TextureRectComponent &comp) {
        comp.setFrameOnTexture(1);
    });
}
