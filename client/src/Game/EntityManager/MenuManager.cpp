#include "MenuManager.hpp"

using namespace game;

MenuManager::MenuManager(ECS::Registry &ecs): _ecs(ecs)
{
    this->_lastButtonInput = NOW;
    this->_selectedButton = NO_BUTTON;
}

MenuManager::~MenuManager()
{
}

void MenuManager::initFirstButton(BUTTON_TYPE type)
{
    this->_selectedButton = type;
    this->_ecs.modify_component<ECS::components::TextureRectComponent>(this->_buttons[this->_selectedButton], [](ECS::components::TextureRectComponent &comp) {
        comp.setFrameOnTexture(1);
    });

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

void MenuManager::createButton(BUTTON_TYPE type, entity_t entity)
{
    if (this->_buttons.find(type) != this->_buttons.end())
        return;
    this->_buttons.emplace(type, entity);
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

void MenuManager::enableMenu(MENU_TYPE type)
{
    if (this->_menu.find(type) == this->_menu.end())
        return;
    auto &menu = this->_menu[type];
    menu.first.second = true;
    this->_ecs.enableEntity(menu.first.first);
    for (auto &button : menu.second)
        this->_ecs.enableEntity(this->_buttons[button]);
}

void MenuManager::disableMenu(MENU_TYPE type)
{
    if (this->_menu.find(type) == this->_menu.end())
        return;
    auto &menu = this->_menu[type];
    menu.first.second = false;
    this->_ecs.disableEntity(menu.first.first);
    for (auto &button : menu.second)
        this->_ecs.disableEntity(this->_buttons[button]);
}

bool MenuManager::checkLastButtonInput()
{
    auto now = NOW;
    if (now - this->_lastButtonInput < 150)
        return false;
    this->_lastButtonInput = now;
    return true;
}

void MenuManager::nextButtonInMenu(MENU_TYPE type)
{
    if (this->checkLastButtonInput() == false)
        return;
    if (this->_menu.find(type) == this->_menu.end())
        return;

    auto &menu = this->_menu[type];
    auto &buttons = menu.second;
    if (buttons.size() == 0)
        return;
    auto it = std::find(buttons.begin(), buttons.end(), this->_selectedButton);
    if (it == buttons.end())
        return;
    this->_ecs.modify_component<ECS::components::TextureRectComponent>(this->_buttons[this->_selectedButton], [](ECS::components::TextureRectComponent &comp) {
        comp.setFrameOnTexture(0);
    });
    if (it + 1 == buttons.end())
        this->_selectedButton = buttons[0];
    else
        this->_selectedButton = *(it + 1);
    this->_ecs.modify_component<ECS::components::TextureRectComponent>(this->_buttons[this->_selectedButton], [](ECS::components::TextureRectComponent &comp) {
        comp.setFrameOnTexture(1);
    });
}

void MenuManager::previousButtonInMenu(MENU_TYPE type)
{
    if (this->checkLastButtonInput() == false)
        return;
    if (this->_menu.find(type) == this->_menu.end())
        return;

    auto &menu = this->_menu[type];
    auto &buttons = menu.second;
    if (buttons.size() == 0)
        return;
    auto it = std::find(buttons.begin(), buttons.end(), this->_selectedButton);
    if (it == buttons.end())
        return;
    this->_ecs.modify_component<ECS::components::TextureRectComponent>(this->_buttons[this->_selectedButton], [](ECS::components::TextureRectComponent &comp) {
        comp.setFrameOnTexture(0);
    });
    if (it == buttons.begin())
        this->_selectedButton = buttons[buttons.size() - 1];
    else
        this->_selectedButton = *(it - 1);
    this->_ecs.modify_component<ECS::components::TextureRectComponent>(this->_buttons[this->_selectedButton], [](ECS::components::TextureRectComponent &comp) {
        comp.setFrameOnTexture(1);
    });
}

void MenuManager::executeButtonInMenu(ECS::Registry &ecs)
{
    if (this->checkLastButtonInput() == false)
        return;
    if (this->_buttons.find(this->_selectedButton) == this->_buttons.end())
        return;
    auto &buttonComponents = ecs.get_components<ECS::components::ButtonComponent>();
    auto entity = this->_buttons[this->_selectedButton];
    auto buttonComponent = buttonComponents[entity];
    buttonComponent->executeAction();
}
