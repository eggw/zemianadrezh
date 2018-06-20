#include "ProgramState_Menu.h"

#include "FontManager.h"

#include <iostream>
ProgramState_Menu::ProgramState_Menu(Program& _program)
    :ProgramState(_program) {

}

ProgramState_Menu::~ProgramState_Menu() {

}


/*
When a left click is registered, this function checks if any
menu items are selected (via their boolean value). It then
calls the function pointer, if it's not null.
*/
void ProgramState_Menu::getInput() {
    if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        for(auto& menuItem : m_menuItems) {
            if(std::get<0>(menuItem) && std::get<1>(menuItem) != nullptr) {
                (m_program.*std::get<1>(menuItem))();
            }
        }
    }
}

void ProgramState_Menu::update() {
    m_program.m_window->setView(m_program.m_window->getDefaultView());
    sf::Vector2i mousePos = sf::Mouse::getPosition(*m_program.m_window);
    for(auto& menuItem : m_menuItems) {
        if(std::get<2>(menuItem).getGlobalBounds().intersects({float(mousePos.x), float(mousePos.y), 1, 1})) {
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
