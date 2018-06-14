#include "GameInstance.h"

GameInstance::GameInstance()
    :m_world() {

}

const World& GameInstance::getWorld() const {
    return m_world;
}
