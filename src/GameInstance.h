#ifndef GAMEINSTANCE_H_INCLUDED
#define GAMEINSTANCE_H_INCLUDED

#include "World.h"

class GameInstance {
    public:
        GameInstance();

        const World& getWorld() const;

        const World::EncodedWorldData encodeWorldData() const;
        void parseWorldData(World::EncodedWorldData& _data);

    protected:
        World m_world;
};

#endif // GAMEINSTANCE_H_INCLUDED
