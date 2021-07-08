#include "WorldSystem.hpp"
#include "Game.hpp"
#include "Components.hpp"

entt::sigh<void(int)> CreateEntitySignal;
entt::sink CreateEntitySignalSink{ CreateEntitySignal };

FWorldSystem::FWorldSystem()
{
}

void FWorldSystem::Initialise()
{
	CreateEntitySignalSink.connect<&FWorldSystem::CreateEntity>(this);

	CreateEntity(RIGID_BODY_COMPONENT);
}

void FWorldSystem::Update(float dt)
{
}

void FWorldSystem::CreateEntity(const int flags)
{
	entt::entity NewEntity = FGame::registry.create();
	FGame::registry.emplace<FTransformComponent>(NewEntity, sf::Vector2f(0.0f, 0.0f));

	if ((flags & RIGID_BODY_COMPONENT) != 0)
	{
		FGame::registry.emplace<FRigidBodyComponent>(NewEntity, sf::Vector2f(0.0f, 0.0f), sf::Vector2f(0.0f, 16.0f), sf::FloatRect(0.0f, 0.0f, 16.0f, 16.0f), nullptr);
	}

	Entities.push_back(NewEntity);
}