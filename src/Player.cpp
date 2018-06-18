#include "Player.h"

#include "TextureManager.h"

#include "Keybinds.h"


Player::Player()
    :m_sprite(),
     m_animationSheet(TextureManager::get_instance().getTexture(TextureManager::Type::PLAYER)) {

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

void Player::getInput() {
    if(sf::Keyboard::isKeyPressed(Key::LEFT_KEY)) {
        m_facingLeft = true;
        m_animationSheet.setAnimationRange(0, 10);
        m_speed = -3;
    }

    else if(sf::Keyboard::isKeyPressed(Key::RIGHT_KEY)) {
        m_facingLeft = false;
        m_animationSheet.setAnimationRange(11, 21);
        m_speed = 3;

    }
    else{
        m_speed = 0;
    }
}

void Player::update() {
    m_animationSheet.setAnimationSpeed(abs(m_speed) * 0.01);
    m_positionX += m_speed;
    m_animationSheet.update();

    m_sprite = m_animationSheet.getSprite();
    m_sprite.setPosition(getPosition());
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(m_sprite, states);
}

