#include "Game.hpp"
#include "GameSignals.hpp"
#include "GameConfiguration.hpp"
#include "SystemManager.hpp"
#include "InputSystem.hpp"

FGame::FGame()
{
	SystemManager = new FSystemManager();
}

void FGame::Initialise()
{
	Window = new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), GAME_TITLE, sf::Style::Default);
	SystemManager->AddSystem<FInputSystem>(Window);

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
