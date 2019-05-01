#include "GameInstance.h"

#include "TextureManager.h"

#include "Tags/PlayerTag.h"
#include "Components/ComponentAnimation.h"
#include "Components/ComponentDirection.h"
#include "Components/ComponentName.h"
#include "Components/ComponentPhysics.h"
#include "Components/ComponentPosition.h"

GameInstance::GameInstance() : m_world() {
}

const World& GameInstance::getWorld() const {
	return m_world;
}

/*
const std::shared_ptr<std::vector<std::shared_ptr<Player>>>
GameInstance::getPlayers() const {
    return m_players;
}
*/

const encodedChunks GameInstance::encodeWorldChunks() const {
	return m_world.encodeChunks();
}

void GameInstance::parseWorldChunk(const WorldChunk::EncodedChunkData& _data) {
	m_world.parseChunk(_data);
}

void GameInstance::addPlayer(const ComponentsPlayer& _data) {
	entt::entity e{m_registry.create()};
	m_registry.assign<ComponentAnimation>(
	  e,
	  TextureManager::get_instance().getTexture(TextureManager::Type::PLAYER));

	m_registry.assign<PlayerTag>(e);
	m_registry.assign<ComponentName>(e, _data.m_name);
	m_registry.assign<ComponentPhysics>(e, _data.m_vel);
	m_registry.assign<ComponentDirection>(e, _data.m_dir);
	m_registry.assign<ComponentPosition>(e, _data.m_pos);
}

void GameInstance::removePlayer(entt::entity _e) {
	m_registry.remove<PlayerTag>(_e);
	m_registry.remove<ComponentAnimation>(_e);
	m_registry.remove<ComponentName>(_e);
	m_registry.remove<ComponentPhysics>(_e);
	m_registry.remove<ComponentDirection>(_e);
	m_registry.remove<ComponentPosition>(_e);
}

void GameInstance::removePlayer(std::string& _name) {
	auto view = m_registry.view<PlayerTag, ComponentName>();
	for (auto& entity : view) {
		auto name = view.get<ComponentName>(entity).m_name;
		if (name == _name) {
			m_registry.remove<PlayerTag>(entity);
			m_registry.remove<ComponentAnimation>(entity);
			m_registry.remove<ComponentName>(entity);
			m_registry.remove<ComponentPhysics>(entity);
			m_registry.remove<ComponentDirection>(entity);
			m_registry.remove<ComponentPosition>(entity);
		}
	}
}
