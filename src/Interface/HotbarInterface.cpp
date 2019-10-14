#include "Interface/HotbarInterface.h"

#include "InputLocker.h"
#include "Keybinds.h"

HotbarInterface::HotbarInterface()
            : m_target{nullptr},
              m_lastTargetSize{},
              m_hotbarView{},
              m_activeHotbarSlot{nullptr} {
	int hotbarRectWidth{40};
	int hotbarRectHeight{40};

	for (int i{0}; i < 10; i++) {
		sf::RectangleShape shape;
		shape.setSize({float(hotbarRectWidth), float(hotbarRectHeight)});

		shape.setPosition(((i * hotbarRectWidth) + hotbarRectWidth / 6) * 1.2,
		                  10);
		shape.setOutlineThickness(1);
		shape.setFillColor(sf::Color(255, 255, 255, 20));
		shape.setOutlineColor(sf::Color::Black);

		//This is just to ensure that the hotbar goes from 1-9 first, then 0
		if (i < 9) {
			m_hotbar.push_back({shape, i + 1});
		}
		else {
			m_hotbar.push_back({shape, 0});
		}
	}

	setActiveHotbarSlot(1);
}

void HotbarInterface::getInput(sf::Event& _event) {
	if (InputLocker::get_instance().isLocked()) {
		return;
	}

	switch (_event.type) {
	case sf::Event::KeyPressed: {
		if (_event.key.code == Key::HOTBAR_1) {
			setActiveHotbarSlot(1);
			return;
		}
		else if (_event.key.code == Key::HOTBAR_2) {
			setActiveHotbarSlot(2);
			return;
		}
		else if (_event.key.code == Key::HOTBAR_3) {
			setActiveHotbarSlot(3);
			return;
		}
		else if (_event.key.code == Key::HOTBAR_4) {
			setActiveHotbarSlot(4);
			return;
		}
		else if (_event.key.code == Key::HOTBAR_5) {
			setActiveHotbarSlot(5);
			return;
		}
		else if (_event.key.code == Key::HOTBAR_6) {
			setActiveHotbarSlot(6);
			return;
		}
		else if (_event.key.code == Key::HOTBAR_7) {
			setActiveHotbarSlot(7);
			return;
		}
		else if (_event.key.code == Key::HOTBAR_8) {
			setActiveHotbarSlot(8);
			return;
		}
		else if (_event.key.code == Key::HOTBAR_9) {
			setActiveHotbarSlot(9);
			return;
		}
		else if (_event.key.code == Key::HOTBAR_0) {
			setActiveHotbarSlot(0);
			return;
		}
	}

	default:
		break;
	}
}

void HotbarInterface::update(int _timeslice) {
	//onResize() will need to be called at least once after
	//m_target is set, so this code will be needed here.
	//As a result, we also won't really need to detect
	//a Resized event in getInput(), since this part
	//will handle that as well.
	if (m_target != nullptr) {
		if (m_target->getSize() != m_lastTargetSize) {
			m_lastTargetSize = m_target->getSize();
			onResize(m_lastTargetSize);
		}
	}
}

void HotbarInterface::draw(sf::RenderTarget& target,
                           sf::RenderStates  states) const {
	m_target = &target;

	sf::View previousView = m_target->getView();

	m_target->setView(m_hotbarView);

	for (auto& rect : m_hotbar) {
		m_target->draw(rect.m_rectangle);
		m_target->draw(rect.m_numberText);
	}

	m_target->setView(previousView);
}

void HotbarInterface::onResize(sf::Vector2u _newSize) {
	m_hotbarView = sf::View({float(_newSize.x / 2), float(_newSize.y / 2)},
	                        {float(_newSize.x), float(_newSize.y)});
}

void HotbarInterface::setActiveHotbarSlot(int _i) {
	if (m_activeHotbarSlot != nullptr) {
		m_activeHotbarSlot->m_rectangle.setOutlineColor(sf::Color::Black);
	}

	if (_i == 0) {
		m_activeHotbarSlot = &m_hotbar[9];
	}
	else if (_i > 0 && _i <= 9) {
		m_activeHotbarSlot = &m_hotbar[_i - 1];
	}

	m_activeHotbarSlot->m_rectangle.setOutlineColor(sf::Color::Yellow);
}
