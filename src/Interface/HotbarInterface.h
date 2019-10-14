#ifndef HOTBARINTERFACE_H_INCLUDED
#define HOTBARINTERFACE_H_INCLUDED

#include <SFML/Graphics.hpp>
#include "FontManager.h"

class HotbarInterface : public sf::Drawable {
  public:
	HotbarInterface();

	void getInput(sf::Event& _event);
	void update(int _timeslice);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	void onResize(sf::Vector2u _newSize);

  private:
	struct HotbarRect {
		HotbarRect(sf::RectangleShape _rectangle, int _number)
		            : m_rectangle{_rectangle}, m_numberText{} {
			m_numberText.setFont(
			  *FontManager::get_instance().get(FONT::ANDY));
			m_numberText.setCharacterSize(18);
			m_numberText.setString(std::to_string(_number));

			float numberTextX{m_rectangle.getPosition().x};
			float numberTextY{m_rectangle.getPosition().y};

			numberTextX += 0.05 * (m_rectangle.getLocalBounds().width);
			numberTextY += 0.4 * (m_rectangle.getLocalBounds().height);

			m_numberText.setPosition(numberTextX, numberTextY);
		}
		sf::RectangleShape m_rectangle;
		sf::Text           m_numberText;
	};

	//Functions -----------------------------------
	void setActiveHotbarSlot(int _i);
	//---------------------------------------------

	//Data members --------------------------------
	mutable sf::RenderTarget* m_target;
	sf::Vector2u              m_lastTargetSize;

	std::vector<HotbarRect> m_hotbar;
	sf::View                m_hotbarView;

	HotbarRect* m_activeHotbarSlot;
	//---------------------------------------------
};

#endif // HOTBARINTERFACE_H_INCLUDED
