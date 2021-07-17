#include "WorldSystem.hpp"
#include "Game.hpp"
#include "Components.hpp"
#include "GameSignals.hpp"
#include "PlayerController.hpp"
#include "AssetManager.hpp"
#include "GameConfiguration.hpp"

#include <SFML/Graphics/Vertex.hpp>
#include <tmxlite/TileLayer.hpp>
#include <box2d/box2d.h>

FWorldSystem::FWorldSystem()
{
}

void FWorldSystem::ConnectSignals()
{
	CreateEntitySignalSink.connect<&FWorldSystem::CreateEntity>(this);
}

void FWorldSystem::Initialise()
{	
	CreatePlayer();
	LoadMaps();

	ChangeMapSignal.publish(MapEntities["Farm House"], sf::Vector2f(0, -10));
}

void FWorldSystem::Update(float dt)
{
}

entt::entity FWorldSystem::CreateEntity(const int flags)
{
	entt::entity NewEntity = FGame::registry.create();
	FGame::registry.emplace<FTransformComponent>(NewEntity, sf::Vector2f(0.0f, 0.0f), 0);

	if ((flags & RIGID_BODY_COMPONENT) != 0)
	{
		FGame::registry.emplace<FRigidBodyComponent>(NewEntity, sf::Vector2f(0.0f, 0.0f), sf::FloatRect(16.0f, 32.0f, 32.0f, 32.0f), nullptr);
	}

	if ((flags & SPRITE_COMPONENT) != 0)
	{
		FGame::registry.emplace<FSpriteComponent>(NewEntity);
	}

	if ((flags & CONTROLLER_COMPONENT) != 0)
	{
		FGame::registry.emplace<FControllerComponent>(NewEntity, nullptr);
	}

	if ((flags & ANIMATION_COMPONENT) != 0)
	{
		FGame::registry.emplace<FAnimationComponent>(NewEntity);
	}

	if ((flags & MESH_COMPONENT) != 0)
	{
		FGame::registry.emplace<FMeshComponent>(NewEntity);
	}

	if ((flags & MAP_COMPONENT) != 0)
	{
		FGame::registry.emplace<FMapComponent>(NewEntity);
	}

	if ((flags & PLAYER_COMPONENT) != 0)
	{
		FGame::registry.emplace<FPlayerComponent>(NewEntity);
	}

	Entities.push_back(NewEntity);

	return NewEntity;
}

void FWorldSystem::CreateAnimation(entt::entity Entity, std::string AnimationName, sf::Vector2i FrameDimensions, int NumberOfFrames, float TimePerFrame, int YIndex)
{
	auto& AnimationComponent = FGame::registry.get<FAnimationComponent>(Entity);

	FAnimation Animation;
	Animation.FrameDimensions = FrameDimensions;
	Animation.NumberOfFrames = NumberOfFrames;
	Animation.TimePerFrame = TimePerFrame;
	Animation.YIndex = YIndex;

	AnimationComponent.Animations[AnimationName] = Animation;
}

void FWorldSystem::CreatePlayer()
{
	PlayerEntity = CreateEntity(RIGID_BODY_COMPONENT | SPRITE_COMPONENT | CONTROLLER_COMPONENT | ANIMATION_COMPONENT | PLAYER_COMPONENT);

	auto& ControllerComponent = FGame::registry.get<FControllerComponent>(PlayerEntity);
	FPlayerController* PlayerController = new FPlayerController();
	ControllerComponent.Controller = PlayerController;
	PlayerController->Possess(PlayerEntity);

	auto& SpriteComponent = FGame::registry.get<FSpriteComponent>(PlayerEntity);
	SpriteComponent.Sprite.setTexture(FAssetManager::GetInstance().GetPlayerTexture());

	CreateAnimation(PlayerEntity, "IDLE_DOWN",  sf::Vector2i(64, 64), 9, 1.0f / 4.0f, 0);
	CreateAnimation(PlayerEntity, "WALK_DOWN",  sf::Vector2i(64, 64), 4, 1.0f / 4.0f, 1);
	CreateAnimation(PlayerEntity, "WALK_LEFT",  sf::Vector2i(64, 64), 4, 1.0f / 4.0f, 2);
	CreateAnimation(PlayerEntity, "WALK_RIGHT", sf::Vector2i(64, 64), 4, 1.0f / 4.0f, 3);
	CreateAnimation(PlayerEntity, "WALK_UP",    sf::Vector2i(64, 64), 4, 1.0f / 4.0f, 4);
	CreateAnimation(PlayerEntity, "IDLE_LEFT",  sf::Vector2i(64, 64), 1, 1.0f / 4.0f, 5);
	CreateAnimation(PlayerEntity, "IDLE_RIGHT", sf::Vector2i(64, 64), 1, 1.0f / 4.0f, 6);
	CreateAnimation(PlayerEntity, "IDLE_UP",    sf::Vector2i(64, 64), 1, 1.0f / 4.0f, 7);

	PlayAnimationSignal.publish(PlayerEntity, "IDLE_DOWN");
}

void FWorldSystem::LoadMaps()
{
	tmx::Map TmxMap;
	TmxMap.load("Resources/Maps/Farm House.tmx");

	tmx::TileLayer TileLayer = TmxMap.getLayers()[0]->getLayerAs<tmx::TileLayer>();
	tmx::Tileset Tileset = TmxMap.getTilesets()[0];
	tmx::Vector2u TileCount = TmxMap.getTileCount();

	entt::entity MapEntity = CreateEntity(MAP_COMPONENT);

	FMapComponent& MapComponent = FGame::registry.get<FMapComponent>(MapEntity);
	MapComponent.WalkableTiles.resize(TileCount.x * TileCount.y);
	MapComponent.MapDimensions = sf::Vector2u(TmxMap.getTileCount().x, TmxMap.getTileCount().y);

	CreateMapLayersSignal.publish(MapEntity, TmxMap);

	TileLayer = TmxMap.getLayers()[3]->getLayerAs<tmx::TileLayer>();
	for (unsigned int Y = 0; Y < TileCount.y; Y++)
	{
		for (unsigned int X = 0; X < TileCount.x; X++)
		{
			tmx::TileLayer::Tile Tile = TileLayer.getTiles()[Y * TileCount.x + X];

			if (Tile.ID == 0)
			{
				MapComponent.WalkableTiles[Y * TileCount.x + X] = true;
			}
			else
			{
				MapComponent.WalkableTiles[Y * TileCount.x + X] = false;
			}
		}
	}

	MapEntities["Farm House"] = MapEntity;
}
