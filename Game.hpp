#ifndef GAME_HPP
#define GAME_HPP

#include "entt/entt.hpp"
#include <SFML/Graphics.hpp>

class FSystemManager;

class FGame
{
public:
	static entt::registry registry;

	FGame();
	void Initialise();
	void Run();

protected:

private:
	sf::RenderWindow* Window;
	FSystemManager* SystemManager;
	sf::Clock DeltaClock;

	void QuitGame();
};

#endif