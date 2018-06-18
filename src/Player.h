#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include "EntityMoving.h"
#include "Animation.h"

class Player : public EntityMoving, public sf::Drawable {
    public:
        struct EncodedPlayerData {
            std::string playerName;
            float speed;
            float positionX;
            float positionY;
        };

        Player();

        const std::string& getName() const;

        void getInput();
        void update();
        void draw(sf::RenderTarget& target, sf::RenderStates states) const;

        const EncodedPlayerData encodeData() const;
        void parseData(EncodedPlayerData& _data);
    private:
        sf::Sprite m_sprite;
        Animation m_animationSheet;
        std::string m_playerName;
};


#endif // PLAYER_H_INCLUDED
