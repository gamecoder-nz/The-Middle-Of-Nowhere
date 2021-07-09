#include "InputSystem.hpp"
#include <entt/entt.hpp>

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
		{
			QuitGameSignal.publish();
		}
		else if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Escape)
			{
				QuitGameSignal.publish();
			}
			else
			{
				KeyDownSignal.publish(event.key.code);
			}
		}
		else if (event.type == sf::Event::KeyReleased)
		{
			KeyUpSignal.publish(event.key.code);
		}
	}
}
