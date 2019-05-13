#include "GameInstance.h"

#include "TextureManager.h"

#include "Tags/PlayerTag.h"
#include "Components/ComponentAnimation.h"
#include "Components/ComponentDirection.h"
#include "Components/ComponentName.h"
#include "Components/ComponentPhysics.h"
#include "Components/ComponentPosition.h"

GameInstance::GameInstance(GameInstance* _g) : m_world{nullptr}, m_registry{nullptr} {
	if(_g == nullptr) {
		initialise();
	}
	else {
		initialise(_g);
	}
}

const World& GameInstance::getWorld() const {
	return *m_world;
}

/*
const std::shared_ptr<std::vector<std::shared_ptr<Player>>>
GameInstance::getPlayers() const {
    return m_players;
}
*/

const EncodedChunks GameInstance::encodeWorldChunks() const {
	return m_world->encodeChunks();
}

void GameInstance::parseWorldChunk(const WorldChunk::EncodedChunkData& _data) {
	m_world->parseChunk(_data);
}

void GameInstance::updatePlayer(const ComponentsPlayer& _data) {
	auto view{m_registry->view<PlayerTag>()};
	for (auto& entity : view) {
		auto name{m_registry->get<ComponentName>(entity).m_name};
		if (name == _data.compName.m_name) {
			m_registry->replace<ComponentPhysics>(entity, _data.compVel);
			m_registry->replace<ComponentDirection>(entity, _data.compDir);
			m_registry->replace<ComponentPosition>(entity, _data.compPos);
			return;
		}
	}

	//If we got this far, that means no player with a matching name
	//was found. Let's create it.
	addPlayer(_data);
}

void GameInstance::removePlayer(entt::entity _e) {
	m_registry->destroy(_e);
}

void GameInstance::removePlayer(std::string& _name) {
	auto view = m_registry->view<PlayerTag>();
	for (auto& entity : view) {
		const auto name = m_registry->get<ComponentName>(entity).m_name;
		if (name == _name) {
			removePlayer(entity);
			return;
		}
	}
}

void GameInstance::addPlayer(const ComponentsPlayer& _data) {
	entt::entity e{m_registry->create()};
	addPlayer(_data, e);
}

void GameInstance::addPlayer(const ComponentsPlayer& _data, entt::entity _e) {
	m_registry->assign<ComponentAnimation>(
	  _e,
	  TextureManager::get_instance().getTexture(TextureManager::Type::PLAYER));

	m_registry->assign<PlayerTag>(_e);
	m_registry->assign<ComponentName>(_e, _data.compName);
	m_registry->assign<ComponentPhysics>(_e, _data.compVel);
	m_registry->assign<ComponentDirection>(_e, _data.compDir);
	m_registry->assign<ComponentPosition>(_e, _data.compPos);
}

void GameInstance::initialise(GameInstance* _g) {
	m_world    = _g->m_world;
	m_registry = _g->m_registry;
}

void GameInstance::initialise() {
	m_world    = std::make_shared<World>();
	m_registry = std::make_shared<entt::registry>();
}
