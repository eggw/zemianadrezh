#ifndef ANIMATION_H_INCLUDED
#define ANIMATION_H_INCLUDED

#include <SFML/Graphics.hpp>

class Animation {
  public:
	Animation(const sf::Texture& _texture);

	void addFrame(sf::IntRect _rect);

	void setAnimationRange(int _beginIndex, int _maxIndex);
	void setAnimationSpeed(float _speed);

	void beginAnimation();
	void stopAnimation();

	const sf::Sprite getCurrentSprite() const;

	//This function deals with managing the current
	//frame of the sprite.
	void update();

  private:
	const sf::Sprite getSprite(int _index) const;

	std::vector<sf::IntRect> m_textureRects;
	int                      m_currentIndex;
	int                      m_minIndex;
	int                      m_maxIndex;

	const sf::Texture& m_texture;
	sf::Sprite         m_sprite;

	float m_animationSpeed;

	sf::Clock m_animationTimer;
	bool      m_animationRunning;
};

#endif // ANIMATION_H_INCLUDED
