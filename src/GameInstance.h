#ifndef GAMEINSTANCE_H_INCLUDED
#define GAMEINSTANCE_H_INCLUDED

#include <memory>

#include "World.h"
#include "Player.h"

class GameInstance {
    public:
        GameInstance();

        const World& getWorld() const;
        const std::vector<std::unique_ptr<Player>>& getOtherPlayers() const;

        const World::EncodedWorldData encodeWorldData() const;
        void parseWorldData(World::EncodedWorldData& _data);

        //Server and Client both have different implementations of this.
        //The server either adds or updates the player data, depending on whether
        //it is already present in m_otherPlayers or not.

        //Client does the same thing, but also does an extra check: if the player
        //being added has the same name as theirs (i.e. it is theirs), it is not
        //appended to m_otherPlayers.
        virtual void updateOtherPlayers(Player::EncodedPlayerData _data) = 0;

    protected:
        World m_world;
        std::vector<std::unique_ptr<Player>> m_otherPlayers;
};

#endif // GAMEINSTANCE_H_INCLUDED
