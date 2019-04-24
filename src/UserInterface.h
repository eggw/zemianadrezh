#ifndef USERINTERFACE_H_INCLUDED
#define USERINTERFACE_H_INCLUDED

#include <SFML/Graphics.hpp>
#include "FontManager.h"

class UserInterface : public sf::Drawable {

public:
	UserInterface(sf::RenderWindow& _window);

	void getInput(sf::Event& _event);
	void update(int _timeslice);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	void onResize(sf::Vector2u _newSize);


private:
	void setActiveHotbarSlot(int _i);

	struct HotbarRect {
		HotbarRect(	sf::RectangleShape _rectangle, int _number)
				:	m_rectangle{_rectangle},
					m_numberText{} {

			m_numberText.setFont(FontManager::get_instance().getFont(FontManager::Type::ANDY));
			m_numberText.setCharacterSize(18);
			m_numberText.setString(std::to_string(_number));

			float numberTextX {m_rectangle.getPosition().x};
			float numberTextY {m_rectangle.getPosition().y};

			numberTextX += 0.05 * (m_rectangle.getLocalBounds().width);
			numberTextY += 0.4 * (m_rectangle.getLocalBounds().height);

			m_numberText.setPosition(numberTextX,
									 numberTextY);
		}
		sf::RectangleShape m_rectangle;
		sf::Text m_numberText;
	};

	sf::RenderWindow& m_window;
	std::vector<HotbarRect> m_hotbar;
	sf::View m_hotbarView;

	HotbarRect* m_activeHotbarSlot;
};

#endif // USERINTERFACE_H_INCLUDED
