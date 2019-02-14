#include "ProgramState_Menu.h"

#include <cmath>
#include <random>
#include <chrono>

#include "FontManager.h"

const std::string	TITLE_TEXT{"zemianadrezh"};

sf::Text ProgramState_Menu::m_titleText;

//The safe way to compare floats
bool almostEqual(float _a, float _b, float _magnitude = 0.001) {
	return (fabs(_a - _b) < _a * _magnitude);
}

ProgramState_Menu::ProgramState_Menu(Program& _program, bool _visibleOverPreviousState)
	:ProgramState(_program, _visibleOverPreviousState),
	 m_view{sf::FloatRect(0, 0,
							float(m_program.m_window->getSize().x),
							float(m_program.m_window->getSize().y))} {

	m_program.m_window->setView(m_view);

	m_titleText.setFont(FontManager::get_instance().getFont(FontManager::Type::ANDY));
	m_titleText.setCharacterSize(64);
	m_titleText.setString(TITLE_TEXT);
	m_titleText.setOutlineThickness(1);
	m_titleText.setOrigin(m_titleText.getLocalBounds().width / 2,
						  m_titleText.getLocalBounds().height / 2);
	
	onResize(m_program.m_window->getSize());
}

ProgramState_Menu::~ProgramState_Menu() {

}

bool lmbPressed(sf::Event& _event) {
	if(_event.type == sf::Event::MouseButtonPressed
			&&
			_event.mouseButton.button == sf::Mouse::Left) {
		return true;
	}
	else {
		return false;
	}
}

/*
When a left click is registered, this function checks if any
menu items are selected (via their boolean value). It then
calls the function pointer, if it's not null.

Also checks if a MenuItem's keyboard shortcut was pressed,
and calls its function pointer if so.
*/
void ProgramState_Menu::getInput(sf::Event& _event) {
	ProgramState::getInput(_event);

	if(lmbPressed(_event)) {
		for(const auto& menuItem : m_menuItems) {
			if(isMousedOver(menuItem) && !isFunctionNull(menuItem)) {
				(m_program.*std::get<1>(menuItem))();
				return;
			}
		}
	}

	if(_event.type == sf::Event::KeyPressed) {
		for(const auto& menuItem : m_menuItems) {
			int key { std::get<3>(menuItem) };
			if(key == sf::Keyboard::Unknown) {
				continue;
			}

			if(_event.key.code == key && !isFunctionNull(menuItem)) {
				(m_program.*std::get<1>(menuItem))();
				return;
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
			1
															   });
}

bool ProgramState_Menu::isFunctionNull(ProgramState_Menu::MenuItem _menuItem) {
	return std::get<1>(_menuItem) == nullptr;
}

void ProgramState_Menu::update(int _timeslice) {
	detectMouseClicks();
	updateTitleText(_timeslice);
}

void ProgramState_Menu::draw() {
	m_program.m_window->setView(m_view);
	for(const auto& menuItem : m_menuItems) {
		m_program.m_window->draw(std::get<2>(menuItem));
	}

	m_program.m_window->draw(m_titleText);
}

void ProgramState_Menu::addMenuItem(const std::string& _string,
									void(Program::*f)(),
									int _keyCode) {
	sf::Text menuItem;
	float verticalSpacing{45.0f};

	menuItem.setFont(FontManager::get_instance().getFont(FontManager::Type::ANDY));
	menuItem.setOutlineThickness(1);
	menuItem.setCharacterSize(34);
	menuItem.setString(_string);
	menuItem.setOrigin(menuItem.getGlobalBounds().width / 2,
					   menuItem.getGlobalBounds().height / 2);

	m_menuItems.push_back(std::make_tuple(false, f, menuItem, _keyCode));

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

void ProgramState_Menu::addGap() {
	addMenuItem("");
}

void ProgramState_Menu::onResize(sf::Vector2u _newSize) {
	ProgramState::onResize(_newSize);
	m_view.setSize({float(_newSize.x),
                    float(_newSize.y)});

	float newTitleY{ m_view.getCenter().y - m_program.m_window->getSize().y / 3};
	m_titleText.setPosition({m_view.getCenter().x,  newTitleY});
}

void ProgramState_Menu::detectMouseClicks() {
	for(auto& menuItem : m_menuItems) {
		if(isMousedOver(menuItem) && !isFunctionNull(menuItem)) {
			std::get<0>(menuItem) = true;
			std::get<2>(menuItem).setFillColor(sf::Color::Yellow);
			std::get<2>(menuItem).setScale({1.2f, 1.2f});
		}
		else {
			std::get<0>(menuItem) = false;
			if(std::get<1>(menuItem) == nullptr) {
				std::get<2>(menuItem).setFillColor(sf::Color(255,255,255,100));
				std::get<2>(menuItem).setOutlineColor(sf::Color(0,0,0,100));
			}
			else{
				std::get<2>(menuItem).setFillColor(sf::Color::White);
			}
			std::get<2>(menuItem).setScale({1.0f, 1.0f});
		}
	}
}

void ProgramState_Menu::updateTitleText(int _timeslice) {
	float f_timeslice {float(_timeslice)};

	//-------------------------------------------------
	//randomize fill color every half second
	static sf::Clock clock;

	if(clock.getElapsedTime().asMilliseconds() >= _timeslice * 30) {
		auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
		static std::mt19937 mt_rand(seed);
		std::uniform_int_distribution<int> uniform_dist(0, 255);
		int r = uniform_dist(mt_rand);
		int g = uniform_dist(mt_rand);
		int b = uniform_dist(mt_rand);

		int rx = uniform_dist(mt_rand);
		int gx = uniform_dist(mt_rand);
		int bx = uniform_dist(mt_rand);

		m_titleText.setFillColor(sf::Color(r,g,b));
		m_titleText.setOutlineColor(sf::Color(rx,gx,bx));

		clock.restart();
	}
	//-------------------------------------------------
	//Rotate the title text

	static float 	rotationDegrees	{10};
	static bool 	rotatingLeft	{false};
	float 			deltaRotation	{0};
	const float 	curRotation		{m_titleText.getRotation()};

	if(almostEqual(curRotation, rotationDegrees)) {
		rotatingLeft = true;
	}
	else if(almostEqual(curRotation, (360 - rotationDegrees))){
		rotatingLeft = false;
	}

	if(rotatingLeft) {
		deltaRotation = -1 * (f_timeslice / 1000.f);
	}
	else {
		deltaRotation = (f_timeslice / 1000.f);
	}
	m_titleText.rotate(deltaRotation);

	//-------------------------------------------------
	//Scale the title text

	static sf::Vector2f	 	minScale	{0.75, 0.75};
	static sf::Vector2f 	maxScale	{1.25, 1.25};
	static bool 			scalingDown {false};
	static sf::Vector2f 	deltaScale	{1,1};

    if(maxScale.x <= deltaScale.x) {
		scalingDown = true;
    }
    else if(minScale.x >= deltaScale.x) {
		scalingDown = false;
    }

    if(scalingDown) {
		deltaScale.x -= f_timeslice / 10000.f;
		deltaScale.y -= f_timeslice / 10000.f;
	}
	else {
		deltaScale.x += f_timeslice / 10000.f;
		deltaScale.y += f_timeslice / 10000.f;
	}
	m_titleText.setScale(deltaScale);

	//std::cout << "maxScale.x = " << maxScale.x << ", " << "deltaScale.x = " << deltaScale.x << std::endl;
	//-------------------------------------------------
}
