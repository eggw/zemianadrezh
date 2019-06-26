#ifndef ANIMATIONMOVEMENT_H_INCLUDED
#define ANIMATIONMOVEMENT_H_INCLUDED

#include <SFML/Graphics.hpp>

//The texture for a movement animation should contain 11 sprites for
//each direction (left, right) with the first row being the left-facing
//sprite and the second being the right-facing sprite.
//
//The dimensions of the player sprite are specified in AnimationMovement.cpp

class AnimationMovement {
  public:
	AnimationMovement(const sf::Texture& _texture);

	//Move assignment overload
	AnimationMovement& operator=(const AnimationMovement&) noexcept {
		return *this;
	};

	void             setFacingLeft();
	void             setFacingRight();
	void             setAnimationSpeed(float _speed);
	const sf::Sprite getCurrentSprite() const;
	void             update();

  private:
	//Functions -----------------------------------
	void             setAnimationRange(int _beginIndex, int _maxIndex);
	const sf::Sprite getSprite(int _index) const;
	void             addFrame(sf::IntRect _rect);
	void             beginAnimation();
	void             stopAnimation();
	//---------------------------------------------

	//Data members --------------------------------
	std::vector<sf::IntRect> m_textureRects;
	int                      m_currentIndex;
	int                      m_minIndex;
	int                      m_maxIndex;

	const sf::Texture& m_texture;
	sf::Sprite         m_sprite;

	float m_animationSpeed;

	sf::Clock m_animationTimer;
	bool      m_animationRunning;
	//---------------------------------------------
};

#endif // ANIMATIONMOVEMENT_H_INCLUDED
