#include "Player.h"

#include "TextureManager.h"

#include "Keybinds.h"

#include "InputLocker.h"

Player::Player(std::string _name)
    :m_sprite(),
     m_animationSheet(TextureManager::get_instance().getTexture(TextureManager::Type::PLAYER)),
     m_playerName{_name} {

    constexpr int SPRITE_WIDTH{32};
    constexpr int SPRITE_HEIGHT{48};

    //LEFT: indeces 0-10
    //idle left
    m_animationSheet.addFrame({0,0,SPRITE_WIDTH,SPRITE_HEIGHT});
    //walk left 1
    m_animationSheet.addFrame({SPRITE_WIDTH,0,SPRITE_WIDTH,SPRITE_HEIGHT});
    //walk left 2
    m_animationSheet.addFrame({2*SPRITE_WIDTH,0,SPRITE_WIDTH,SPRITE_HEIGHT});
    //walk left 3
    m_animationSheet.addFrame({3*SPRITE_WIDTH,0,SPRITE_WIDTH,SPRITE_HEIGHT});
    //walk left 4
    m_animationSheet.addFrame({4*SPRITE_WIDTH,0,SPRITE_WIDTH,SPRITE_HEIGHT});
    //walk left 5
    m_animationSheet.addFrame({5*SPRITE_WIDTH,0,SPRITE_WIDTH,SPRITE_HEIGHT});
    //walk left 6
    m_animationSheet.addFrame({6*SPRITE_WIDTH,0,SPRITE_WIDTH,SPRITE_HEIGHT});
    //walk left 7
    m_animationSheet.addFrame({7*SPRITE_WIDTH,0,SPRITE_WIDTH,SPRITE_HEIGHT});
    //walk left 8
    m_animationSheet.addFrame({8*SPRITE_WIDTH,0,SPRITE_WIDTH,SPRITE_HEIGHT});
    //walk left 9
    m_animationSheet.addFrame({9*SPRITE_WIDTH,0,SPRITE_WIDTH,SPRITE_HEIGHT});
    //walk left 10
    m_animationSheet.addFrame({10*SPRITE_WIDTH,0,SPRITE_WIDTH,SPRITE_HEIGHT});


    //RIGHT: indeces 11-21
    //idle right
    m_animationSheet.addFrame({10*SPRITE_WIDTH,SPRITE_HEIGHT,SPRITE_WIDTH,SPRITE_HEIGHT});
    //walk right 1
    m_animationSheet.addFrame({9*SPRITE_WIDTH,SPRITE_HEIGHT,SPRITE_WIDTH,SPRITE_HEIGHT});
    //walk right 2
    m_animationSheet.addFrame({8*SPRITE_WIDTH,SPRITE_HEIGHT,SPRITE_WIDTH,SPRITE_HEIGHT});
    //walk right 3
    m_animationSheet.addFrame({7*SPRITE_WIDTH,SPRITE_HEIGHT,SPRITE_WIDTH,SPRITE_HEIGHT});
    //walk right 4
    m_animationSheet.addFrame({6*SPRITE_WIDTH,SPRITE_HEIGHT,SPRITE_WIDTH,SPRITE_HEIGHT});
    //walk right 5
    m_animationSheet.addFrame({5*SPRITE_WIDTH,SPRITE_HEIGHT,SPRITE_WIDTH,SPRITE_HEIGHT});
    //walk right 6
    m_animationSheet.addFrame({4*SPRITE_WIDTH,SPRITE_HEIGHT,SPRITE_WIDTH,SPRITE_HEIGHT});
    //walk right 7
    m_animationSheet.addFrame({3*SPRITE_WIDTH,SPRITE_HEIGHT,SPRITE_WIDTH,SPRITE_HEIGHT});
    //walk right 8
    m_animationSheet.addFrame({2*SPRITE_WIDTH,SPRITE_HEIGHT,SPRITE_WIDTH,SPRITE_HEIGHT});
    //walk right 9
    m_animationSheet.addFrame({1*SPRITE_WIDTH,SPRITE_HEIGHT,SPRITE_WIDTH,SPRITE_HEIGHT});
    //walk right 10
    m_animationSheet.addFrame({SPRITE_WIDTH,SPRITE_HEIGHT,SPRITE_WIDTH,SPRITE_HEIGHT});


    m_animationSheet.setAnimationRange(11, 21);
}

const std::string& Player::getName() const {
    return m_playerName;
}

void Player::getInput() {
    if(InputLocker::get_instance().isLocked()) {
        return;
    }

    if(sf::Keyboard::isKeyPressed(Key::LEFT_KEY)) {
        m_facingLeft = true;
        m_animationSheet.setAnimationRange(0, 10);
        m_horizontalSpeed = -3;
    }

    else if(sf::Keyboard::isKeyPressed(Key::RIGHT_KEY)) {
        m_facingLeft = false;
        m_animationSheet.setAnimationRange(11, 21);
        m_horizontalSpeed = 3;

    }
    else {
        m_horizontalSpeed = 0;
    }
}

void Player::update() {
    m_animationSheet.setAnimationSpeed(abs(m_horizontalSpeed) * 0.01);
    m_position = {m_position.x + m_horizontalSpeed, m_position.y};
    m_animationSheet.update();

    m_sprite = m_animationSheet.getSprite();
    m_sprite.setPosition(getPosition());
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(m_sprite, states);
}

const Player::EncodedPlayerData Player::encodeData() const {
    EncodedPlayerData data;

    data.playerName = getName();
    data.facingLeft = m_facingLeft;
    data.speed = m_horizontalSpeed;
    data.positionX = m_position.x;
    data.positionY = m_position.y;

    return data;
}

void Player::parseData(Player::EncodedPlayerData& _data) {
    m_playerName = _data.playerName;
    m_facingLeft = _data.facingLeft;
    m_horizontalSpeed = _data.speed;
    m_position.x = _data.positionX;
    m_position.y = _data.positionY;
}

