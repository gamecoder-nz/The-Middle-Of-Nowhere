#ifndef PLAYER_CONTROLLER_HPP
#define PLAYER_CONTROLLER_HPP

#include "Controller.hpp"
#include <SFML/Window/Keyboard.hpp>

class FPlayerController :
    public FController
{
public:
	FPlayerController();

	void Possess(entt::entity Entity) final;
	void Unpossess() final;
	void Update(float dt) final;

protected:
	void OnKeyDown(sf::Keyboard::Key Key);
	void OnKeyUp(sf::Keyboard::Key Key);

	bool MoveUp;
	bool MoveDown;
	bool MoveLeft;
	bool MoveRight;
};

#endif