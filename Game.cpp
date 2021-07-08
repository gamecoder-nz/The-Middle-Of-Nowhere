#include "Game.hpp"
#include "GameSignals.hpp"
#include "GameConfiguration.hpp"
#include "SystemManager.hpp"
#include "InputSystem.hpp"
#include "WorldSystem.hpp"
#include "PhysicsSystem.hpp"

entt::sigh<void()> QuitGameSignal;
entt::sink<void()> QuitGameSignalSink{ QuitGameSignal };

FGame::FGame()
{
	SystemManager = new FSystemManager();
}

void FGame::Initialise()
{
	Window = new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), GAME_TITLE, sf::Style::Default);
	SystemManager->AddSystem<FInputSystem>(Window);
	SystemManager->AddSystem<FPhysicsSystem>();
	SystemManager->AddSystem<FWorldSystem>();

	QuitGameSignalSink.connect<&FGame::QuitGame>(this);
}

void FGame::Run()
{
	while (Window->isOpen() == true)
	{
		float dt = DeltaClock.restart().asSeconds();

		SystemManager->Update(dt);

		Window->display();
	}
}

void FGame::QuitGame()
{
	Window->close();
}
