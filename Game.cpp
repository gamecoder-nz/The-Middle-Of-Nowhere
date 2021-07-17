#include "Game.hpp"
#include "GameSignals.hpp"
#include "GameConfiguration.hpp"
#include "SystemManager.hpp"
#include "InputSystem.hpp"
#include "WorldSystem.hpp"
#include "PhysicsSystem.hpp"
#include "RenderSystem.hpp"
#include "ControllerSystem.hpp"
#include "AnimationSystem.hpp"
#include "AssetManager.hpp"

entt::sigh<void()> QuitGameSignal;
entt::sink<void()> QuitGameSignalSink{ QuitGameSignal };

entt::sigh<void(int)> CreateEntitySignal;
entt::sink<void(int)> CreateEntitySignalSink{ CreateEntitySignal };

entt::sigh<void(sf::Keyboard::Key)> KeyDownSignal;
entt::sink<void(sf::Keyboard::Key)> KeyDownSignalSink{ KeyDownSignal };

entt::sigh<void(sf::Keyboard::Key)> KeyUpSignal;
entt::sink<void(sf::Keyboard::Key)> KeyUpSignalSink{ KeyUpSignal };

entt::sigh<void(entt::entity, std::string)> PlayAnimationSignal;
entt::sink<void(entt::entity, std::string)> PlayAnimationSignalSink{ PlayAnimationSignal };

entt::sigh<void(entt::entity, sf::Vector2f)> ChangeMapSignal;
entt::sink<void(entt::entity, sf::Vector2f)> ChangeMapSignalSink{ ChangeMapSignal };

entt::sigh<void(entt::entity, tmx::Map& TmxMap)> CreateMapLayersSignal;
entt::sink<void(entt::entity, tmx::Map& TmxMap)> CreateMapLayersSignalSink{ CreateMapLayersSignal };

#ifdef _DEBUG
entt::sigh<b2World* ()> GetBox2dWorldSignal;
entt::sink<b2World* ()> GetBox2dWorldSignalSink{ GetBox2dWorldSignal };
#endif

FGame::FGame()
{
	SystemManager = new FSystemManager();
}

void FGame::Initialise()
{
	Window = new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), GAME_TITLE, sf::Style::Default);
	Window->setFramerateLimit(60);

	SystemManager->AddSystem<FInputSystem>(Window);
	SystemManager->AddSystem<FControllerSystem>();
	SystemManager->AddSystem<FAnimationSystem>();
	SystemManager->AddSystem<FPhysicsSystem>();
	SystemManager->AddSystem<FWorldSystem>();
	SystemManager->AddSystem<FRenderSystem>(Window);
	SystemManager->Initialise();

	QuitGameSignalSink.connect<&FGame::QuitGame>(this);
	Frames = 0;
	FPSClock = 0.0f;

	FAssetManager::GetInstance().LoadAssets();
}

void FGame::Run()
{
	while (Window->isOpen() == true)
	{
		float dt = DeltaClock.restart().asSeconds();

		SystemManager->Update(dt);

		Frames++;
		FPSClock += dt;

		if (FPSClock > 0.1f)
		{
			//printf("FPS: %d\n", Frames*10);
			Frames = 0;
			FPSClock = 0.0f;
		}
	}
}

void FGame::QuitGame()
{
	Window->close();
}
