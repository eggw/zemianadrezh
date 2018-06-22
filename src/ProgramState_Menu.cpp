#include "ProgramState_Menu.h"

#include "FontManager.h"

#include <iostream>
ProgramState_Menu::ProgramState_Menu(Program& _program)
    :ProgramState(_program),
     m_view(m_program.m_window->getDefaultView()) {

    m_program.m_window->setView(m_view);

}

ProgramState_Menu::~ProgramState_Menu() {

}


/*
When a left click is registered, this function checks if any
menu items are selected (via their boolean value). It then
calls the function pointer, if it's not null.
*/
void ProgramState_Menu::getInput(sf::Event& _event) {
    ProgramState::getInput(_event);

    if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        for(auto& menuItem : m_menuItems) {
            if(isMousedOver(menuItem) && !isFunctionNull(menuItem)) {
                (m_program.*std::get<1>(menuItem))();
            }
        }
    }
}

bool ProgramState_Menu::isMousedOver(ProgramState_Menu::MenuItem _menuItem) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(*m_program.m_window);
    sf::Vector2f pixelPos{m_program.m_window->mapPixelToCoords(mousePos)};

    return std::get<2>(_menuItem).getGlobalBounds().intersects({pixelPos.x,
																pixelPos.y,
																1,
																1});
}

bool ProgramState_Menu::isFunctionNull(ProgramState_Menu::MenuItem _menuItem) {
    return std::get<1>(_menuItem) == nullptr;
}

void ProgramState_Menu::update() {
    for(auto& menuItem : m_menuItems) {
        if(isMousedOver(menuItem) && !isFunctionNull(menuItem)) {
            std::get<0>(menuItem) = true;
            std::get<2>(menuItem).setFillColor(sf::Color::Yellow);
            std::get<2>(menuItem).setScale({1.2, 1.2});
        }
        else {
            std::get<0>(menuItem) = false;
            std::get<2>(menuItem).setFillColor(sf::Color::White);
            std::get<2>(menuItem).setScale({1.0, 1.0});
        }
    }
}

void ProgramState_Menu::draw() {
    m_program.m_window->setView(m_view);
    for(auto& menuItem : m_menuItems) {
        m_program.m_window->draw(std::get<2>(menuItem));
    }
}

void ProgramState_Menu::addMenuItem(const std::string _string, void(Program::*f)()) {
    sf::Text menuItem;
    float verticalSpacing{40.0f};

    menuItem.setFont(FontManager::get_instance().getFont(FontManager::Type::ANDY));
    menuItem.setString(_string);
    menuItem.setOrigin(menuItem.getGlobalBounds().width / 2,
                       menuItem.getGlobalBounds().height / 2);

    m_menuItems.push_back(std::make_tuple(false, f, menuItem));

    int index = floor(m_menuItems.size() / 2);

    auto it = m_menuItems.begin();
    it += index;

    //Set the element in the center to be in the middle of the
    //screen

    std::get<2>(*it).setPosition(m_program.m_window->getSize().x / 2,
                                 m_program.m_window->getSize().y / 2);

    //Then, set the position of all elements above the center
    while(it != m_menuItems.begin()) {
        sf::Vector2f lastPosition = std::get<2>(*it).getPosition();
        it--;
        std::get<2>(*it).setPosition(lastPosition.x, lastPosition.y - verticalSpacing);
    }

    it = m_menuItems.begin();
    it += index;

    //Finally, set the positions for all elements below the center
    while(it != m_menuItems.end() - 1) {
        sf::Vector2f lastPosition = std::get<2>(*it).getPosition();
        it++;
        std::get<2>(*it).setPosition(lastPosition.x, lastPosition.y + verticalSpacing);
    }

}

void ProgramState_Menu::onResize(sf::Vector2u _newSize) {
    ProgramState::onResize(_newSize);
    sf::Vector2f newSize{float(_newSize.x), float(_newSize.y)};
    m_view.setSize(newSize);
}
