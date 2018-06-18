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
        void stopAnimation(int _index = 0);

        //without arguments, this returns the sprite
        //corresponding to the textureRect of the current
        //index
        const sf::Sprite getSprite() const;
        const sf::Sprite getSprite(int _index) const;

        void update();
    private:
    	std::vector<sf::IntRect> m_textureRects;
    	int m_currentIndex;
    	int m_minIndex;
    	int m_maxIndex;

    	const sf::Texture& m_texture;

    	float m_animationSpeed;

    	sf::Clock m_animationTimer;
    	bool m_animationRunning;
};

#endif // ANIMATION_H_INCLUDED
