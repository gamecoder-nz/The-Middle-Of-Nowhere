#ifndef INPUT_SYSTEM_HPP
#define INPUT_SYSTEM_HPP

#include "System.hpp"
#include "GameSignals.hpp"
#include <SFML/Graphics.hpp>

class FInputSystem : public FSystem
{
public:
	FInputSystem(sf::RenderWindow* Window);

	virtual void ConnectSignals() final;
	virtual void Initialise() final;
	virtual void Update(float dt) final;

protected:

private:
	sf::RenderWindow* Window;
};

#endif