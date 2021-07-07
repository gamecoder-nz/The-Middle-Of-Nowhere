#include "InputSystem.hpp"
#include "entt/entt.hpp"

extern entt::sigh<void()> QuitGameSignal;

FInputSystem::FInputSystem(sf::RenderWindow* Window)
{
	this->Window = Window;
}

void FInputSystem::Initialise()
{
	
}

void FInputSystem::Update(float dt)
{
	sf::Event event;
	while (Window->pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			QuitGameSignal.publish();
	}
}
