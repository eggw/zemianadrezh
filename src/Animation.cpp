#include "Animation.h"

Animation::Animation(const sf::Texture& _texture)
            : m_textureRects(),
              m_currentIndex{0},
              m_minIndex{0},
              m_maxIndex{0},
              m_texture(_texture),
              m_animationSpeed{1.0f},
              m_animationTimer(),
              m_animationRunning{false} {
	constexpr int SPRITE_WIDTH{32};
	constexpr int SPRITE_HEIGHT{48};

	//LEFT: indeces 0-10
	//idle left
	addFrame({0, 0, SPRITE_WIDTH, SPRITE_HEIGHT});
	//walk left 1
	addFrame({SPRITE_WIDTH, 0, SPRITE_WIDTH, SPRITE_HEIGHT});
	//walk left 2
	addFrame({2 * SPRITE_WIDTH, 0, SPRITE_WIDTH, SPRITE_HEIGHT});
	//walk left 3
	addFrame({3 * SPRITE_WIDTH, 0, SPRITE_WIDTH, SPRITE_HEIGHT});
	//walk left 4
	addFrame({4 * SPRITE_WIDTH, 0, SPRITE_WIDTH, SPRITE_HEIGHT});
	//walk left 5
	addFrame({5 * SPRITE_WIDTH, 0, SPRITE_WIDTH, SPRITE_HEIGHT});
	//walk left 6
	addFrame({6 * SPRITE_WIDTH, 0, SPRITE_WIDTH, SPRITE_HEIGHT});
	//walk left 7
	addFrame({7 * SPRITE_WIDTH, 0, SPRITE_WIDTH, SPRITE_HEIGHT});
	//walk left 8
	addFrame({8 * SPRITE_WIDTH, 0, SPRITE_WIDTH, SPRITE_HEIGHT});
	//walk left 9
	addFrame({9 * SPRITE_WIDTH, 0, SPRITE_WIDTH, SPRITE_HEIGHT});
	//walk left 10
	addFrame({10 * SPRITE_WIDTH, 0, SPRITE_WIDTH, SPRITE_HEIGHT});

	//RIGHT: indeces 11-21
	//idle right
	addFrame({10 * SPRITE_WIDTH, SPRITE_HEIGHT, SPRITE_WIDTH, SPRITE_HEIGHT});
	//walk right 1
	addFrame({9 * SPRITE_WIDTH, SPRITE_HEIGHT, SPRITE_WIDTH, SPRITE_HEIGHT});
	//walk right 2
	addFrame({8 * SPRITE_WIDTH, SPRITE_HEIGHT, SPRITE_WIDTH, SPRITE_HEIGHT});
	//walk right 3
	addFrame({7 * SPRITE_WIDTH, SPRITE_HEIGHT, SPRITE_WIDTH, SPRITE_HEIGHT});
	//walk right 4
	addFrame({6 * SPRITE_WIDTH, SPRITE_HEIGHT, SPRITE_WIDTH, SPRITE_HEIGHT});
	//walk right 5
	addFrame({5 * SPRITE_WIDTH, SPRITE_HEIGHT, SPRITE_WIDTH, SPRITE_HEIGHT});
	//walk right 6
	addFrame({4 * SPRITE_WIDTH, SPRITE_HEIGHT, SPRITE_WIDTH, SPRITE_HEIGHT});
	//walk right 7
	addFrame({3 * SPRITE_WIDTH, SPRITE_HEIGHT, SPRITE_WIDTH, SPRITE_HEIGHT});
	//walk right 8
	addFrame({2 * SPRITE_WIDTH, SPRITE_HEIGHT, SPRITE_WIDTH, SPRITE_HEIGHT});
	//walk right 9
	addFrame({1 * SPRITE_WIDTH, SPRITE_HEIGHT, SPRITE_WIDTH, SPRITE_HEIGHT});
	//walk right 10
	addFrame({SPRITE_WIDTH, SPRITE_HEIGHT, SPRITE_WIDTH, SPRITE_HEIGHT});

	setAnimationRange(11, 21);
}

void Animation::setAnimationRange(int _beginIndex, int _maxIndex) {
	m_minIndex = _beginIndex;
	m_maxIndex = _maxIndex;
	if (m_currentIndex > _maxIndex || m_currentIndex < _beginIndex) {
		m_currentIndex = m_minIndex;
	}
}

void Animation::setAnimationSpeed(float _speed) {
	m_animationSpeed = _speed;
	if (m_animationSpeed == 0) {
		stopAnimation();
	}
	else {
		beginAnimation();
	}
}



void Animation::update() {
	//std::cout << "--------------------------" << std::endl;
	//std::cout << "Run time: " << m_animationTimer.getElapsedTime().asSeconds() << " seconds" << std::endl;
	//std::cout << "m_currentIndex: " << m_currentIndex <<
	//"(" << m_minIndex << ", " << m_maxIndex << ")" << std::endl;
	//std::cout << "m_animationSpeed: " << m_animationSpeed << std::endl;
	//So long as the animation is running
	if (m_animationRunning) {
		//std::cout << "Animation running" << std::endl;
		//And enough time has passed...
		if (m_animationTimer.getElapsedTime().asSeconds() >= m_animationSpeed) {
			//Either increment m_currentIndex, taking us to the next
			//step in the animation, or go back to the first frame.
			if (m_currentIndex < m_maxIndex) {
				m_currentIndex++;
				//std::cout << "setting m_currentIndex to " << m_currentIndex << std::endl;
			}
			else {
				m_currentIndex = m_minIndex;
				//std::cout << "setting m_minIndex to " << m_minIndex << std::endl;
			}

			m_animationTimer.restart();
		}
	}
	else {
		//std::cout << "Animation not running" << std::endl;
	}

	//std::cout << "--------------------------" << std::endl;
}

const sf::Sprite Animation::getCurrentSprite() const {
	return getSprite(m_currentIndex);
}


const sf::Sprite Animation::getSprite(int _index) const {
	sf::Sprite sprite{m_texture, m_textureRects[_index]};
	sprite.setOrigin(sprite.getGlobalBounds().width / 2,
	                 sprite.getGlobalBounds().height);
	return sprite;
}

void Animation::addFrame(sf::IntRect _rect) {
	m_textureRects.push_back(_rect);
}

void Animation::beginAnimation() {
	m_animationRunning = true;
}

void Animation::stopAnimation() {
	m_currentIndex     = m_minIndex;
	m_animationRunning = false;
}
