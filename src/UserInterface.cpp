#include "UserInterface.h"

#include "Keybinds.h"

UserInterface::UserInterface(sf::RenderWindow& _window)
	:m_window{_window},
	m_hotbarView(),
	m_activeHotbarSlot{nullptr}
{
	int hotbarRectWidth{40};
	int hotbarRectHeight{40};

	for(int i{0}; i < 10; i++) {
		sf::RectangleShape shape;
		shape.setSize({	float(hotbarRectWidth),
						float(hotbarRectHeight)});

		shape.setPosition(((i * hotbarRectWidth) + hotbarRectWidth / 6) * 1.2, 10);
		shape.setOutlineThickness(1);
		shape.setFillColor(sf::Color(255,255,255,20));
		shape.setOutlineColor(sf::Color::Black);

		//This is just to ensure that the hotbar goes from 1-9 first, then 0
		if(i < 9){
			m_hotbar.push_back({shape,i+1});
		}
		else{
			m_hotbar.push_back({shape,0});
		}
	}

	setActiveHotbarSlot(1);

	onResize(m_window.getSize());
}

void UserInterface::getInput(sf::Event& _event) {
	switch(_event.type) {

	case sf::Event::KeyPressed: {

		if(_event.key.code == Key::HOTBAR_1) {
			setActiveHotbarSlot(1);
			return;
		}
		else if(_event.key.code == Key::HOTBAR_2) {
			setActiveHotbarSlot(2);
			return;
		}
		else if(_event.key.code == Key::HOTBAR_3) {
			setActiveHotbarSlot(3);
			return;
		}
		else if(_event.key.code == Key::HOTBAR_4) {
			setActiveHotbarSlot(4);
			return;
		}
		else if(_event.key.code == Key::HOTBAR_5) {
			setActiveHotbarSlot(5);
			return;
		}
		else if(_event.key.code == Key::HOTBAR_6) {
			setActiveHotbarSlot(6);
			return;
		}
		else if(_event.key.code == Key::HOTBAR_7) {
			setActiveHotbarSlot(7);
			return;
		}
		else if(_event.key.code == Key::HOTBAR_8) {
			setActiveHotbarSlot(8);
			return;
		}
		else if(_event.key.code == Key::HOTBAR_9) {
			setActiveHotbarSlot(9);
			return;
		}
		else if(_event.key.code == Key::HOTBAR_0) {
			setActiveHotbarSlot(0);
			return;
		}

		else{
			return;
		}
	}

	case sf::Event::Resized: {
			sf::Vector2u newSize{_event.size.width, _event.size.height};
			onResize(newSize);
			break;
		}
	default:
		break;
	}

}

void UserInterface::update(int _timeslice) {

}

void UserInterface::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	sf::View previousView = target.getView();

	target.setView(m_hotbarView);

	for(auto& rect : m_hotbar) {
		target.draw(rect.m_rectangle);
		target.draw(rect.m_numberText);
	}

	target.setView(previousView);
}

void UserInterface::onResize(sf::Vector2u _newSize) {
    m_hotbarView = sf::View({float(_newSize.x/2), float(_newSize.y/2)},
							{float(_newSize.x), float(_newSize.y)});
}

void UserInterface::setActiveHotbarSlot(int _i) {
	if(m_activeHotbarSlot != nullptr) {
		m_activeHotbarSlot->m_rectangle.setOutlineColor(sf::Color::Black);
	}

    if(_i == 0) {
		m_activeHotbarSlot = &m_hotbar[9];
    }
    else if(_i > 0 && _i <= 9) {
		m_activeHotbarSlot = &m_hotbar[_i - 1];
    }

    m_activeHotbarSlot->m_rectangle.setOutlineColor(sf::Color::Yellow);
}
