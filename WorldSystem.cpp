#include "WorldSystem.hpp"
#include "Game.hpp"
#include "Components.hpp"
#include "GameSignals.hpp"
#include "PlayerController.hpp"

FWorldSystem::FWorldSystem()
{
}

void FWorldSystem::Initialise()
{
	CreateEntitySignalSink.connect<&FWorldSystem::CreateEntity>(this);

	CreateEntity(RIGID_BODY_COMPONENT | SPRITE_COMPONENT | CONTROLLER_COMPONENT);

	sf::Sprite Sprite;
	sf::Texture Texture;
	Texture.loadFromFile("Player.png");

	Sprite.setTexture(Texture);
	
	auto& RenderableComponent = FGame::registry.get<FSpriteComponent>(Entities[0]);
	auto& ControllerComponent = FGame::registry.get<FControllerComponent>(Entities[0]);
	
	FPlayerController* PlayerController = new FPlayerController();
	PlayerController->Possess(Entities[0]);
	ControllerComponent.Controller = PlayerController;

	RenderableComponent.Sprite = Sprite;
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
		FGame::registry.emplace<FRigidBodyComponent>(NewEntity, sf::Vector2f(0.0f, 0.0f), sf::FloatRect(0.0f, 16.0f, 16.0f, 16.0f), nullptr);
	}

	if ((flags & SPRITE_COMPONENT) != 0)
	{
		FGame::registry.emplace<FSpriteComponent>(NewEntity);
	}

	if ((flags & CONTROLLER_COMPONENT) != 0)
	{
		FGame::registry.emplace<FControllerComponent>(NewEntity, nullptr);
	}

	Entities.push_back(NewEntity);
}