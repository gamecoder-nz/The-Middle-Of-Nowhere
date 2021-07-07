#include "EntityFactory.hpp"
#include "Game.hpp"
#include "Components.hpp"

static entt::entity CreateEntity(const int flags)
{
	entt::entity NewEntity = FGame::registry.create();
	FGame::registry.emplace<TransformComponent>(NewEntity, sf::Vector2f(0.0f, 0.0f));
}