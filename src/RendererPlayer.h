#ifndef RENDERERPLAYER_H_INCLUDED
#define RENDERERPLAYER_H_INCLUDED

#include <map>

#include "SFML/Graphics.hpp"

#include "Player.h"

class RendererPlayer {
	public:
		RendererPlayer(sf::RenderWindow& _window);
		void addPlayer(const Player* _player) const;

		void getInput(sf::Event& _event);
		void update();
		void draw() const;

	private:
		void onResize(sf::Vector2f _newSize);
	private:
		sf::RenderWindow& m_window;
		mutable std::vector<const Player*> m_players;
		sf::View m_view;
};



#endif // RENDERERPLAYER_H_INCLUDED
