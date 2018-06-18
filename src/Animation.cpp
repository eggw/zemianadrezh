#include "Animation.h"

Animation::Animation(const sf::Texture& _texture)
    :m_textureRects(),
     m_currentIndex{0},
     m_minIndex{0},
     m_maxIndex{0},
     m_texture(_texture),
     m_animationSpeed{1.0f},
     m_animationTimer(),
     m_animationRunning{false} {

}

void Animation::addFrame(sf::IntRect _rect) {
    m_textureRects.push_back(_rect);
}

void Animation::setAnimationRange(int _beginIndex, int _maxIndex) {
	m_minIndex = _beginIndex;
	m_maxIndex = _maxIndex;
	m_currentIndex = m_minIndex;
}

void Animation::setAnimationSpeed(float _speed) {
	m_animationSpeed = _speed;
	if(m_animationSpeed == 0){
		stopAnimation();
	}
	else{
		beginAnimation();
	}
}

void Animation::beginAnimation() {
    m_animationTimer.restart();
    m_animationRunning = true;
    m_currentIndex = m_minIndex;
}

void Animation::stopAnimation(int _index) {
	m_currentIndex = m_minIndex;
	m_animationRunning = false;
}

void Animation::update() {
	//So long as the animation is running
    if(m_animationRunning) {
		//And enough time has passed...
		if(m_animationTimer.getElapsedTime().asSeconds() >= m_animationSpeed){
			//Either increment m_currentIndex, taking us to the next
			//step in the animation, or go back to the first frame.
			if(m_currentIndex < m_maxIndex) {
				m_currentIndex++;
			}
			else{
				m_currentIndex = m_minIndex;
			}
		}
    }
}

const sf::Sprite Animation::getSprite() const {
	return getSprite(m_currentIndex);
}

const sf::Sprite Animation::getSprite(int _index) const {
	sf::Sprite sprite{m_texture, m_textureRects[_index]};
	sprite.setOrigin(sprite.getGlobalBounds().width / 2, sprite.getGlobalBounds().height);
	return sprite;
}
