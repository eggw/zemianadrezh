#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include "EntityMoving.h"
#include "Animation.h"

class Player : public EntityMoving, public sf::Drawable {
    public:
    	Player();
    	void getInput();
    	void update();
    	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    private:
    	sf::Sprite m_sprite;
    	Animation m_animationSheet;
};


#endif // PLAYER_H_INCLUDED
