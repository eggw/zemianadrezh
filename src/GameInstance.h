#ifndef GAMEINSTANCE_H_INCLUDED
#define GAMEINSTANCE_H_INCLUDED

#include "World.h"

class GameInstance {
  public:
    GameInstance();

    const World& getWorld() const;

  protected:
    World m_world;
};

#endif // GAMEINSTANCE_H_INCLUDED
