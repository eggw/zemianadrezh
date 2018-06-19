#ifndef GAMEINSTANCE_H_INCLUDED
#define GAMEINSTANCE_H_INCLUDED

#include <memory>

#include "World.h"
#include "Player.h"

class GameInstance {
    public:
        GameInstance();

        const World& getWorld() const;

        const World::EncodedWorldData encodeWorldData() const;
        void parseWorldData(World::EncodedWorldData& _data);

        void updateOtherPlayers(Player::EncodedPlayerData _data);

    protected:
        World m_world;
        std::vector<std::unique_ptr<Player>> m_otherPlayers;
};

#endif // GAMEINSTANCE_H_INCLUDED
