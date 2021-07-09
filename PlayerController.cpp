#include "PlayerController.hpp"
#include "GameSignals.hpp"
#include "Game.hpp"
#include "Components.hpp"

FPlayerController::FPlayerController()
{
	MoveUp = false;
	MoveDown = false;
	MoveLeft = false;
	MoveRight = false;
}

void FPlayerController::Possess(entt::entity Entity)
{
	this->Entity = Entity;

	KeyDownSignalSink.connect<&FPlayerController::OnKeyDown>(this);
	KeyUpSignalSink.connect<&FPlayerController::OnKeyUp>(this);
}

void FPlayerController::Unpossess()
{
	Entity = entt::null;

	KeyDownSignalSink.disconnect<&FPlayerController::OnKeyDown>(this);
	KeyUpSignalSink.disconnect<&FPlayerController::OnKeyUp>(this);
}

void FPlayerController::Update(float dt)
{
	bool UpdateVelocity = false;
	sf::Vector2f Velocity = FGame::registry.get<FRigidBodyComponent>(Entity).Velocity;

	if (MoveUp == true && Velocity.y > -0.001f)
	{
		UpdateVelocity = true;
		Velocity.y = -1.0f;
	}
	else if (MoveDown == true && Velocity.y < 0.001f)
	{
		UpdateVelocity = true;
		Velocity.y = 1.0f;
	}
	else if (MoveUp == false && Velocity.y < -0.001f)
	{
		UpdateVelocity = true;
		Velocity.y = 0.0f;
	}
	else if (MoveDown == false && Velocity.y > 0.001f)
	{
		UpdateVelocity = true;
		Velocity.y = 0.0f;
	}

	if (MoveLeft == true && Velocity.x > -0.001f)
	{
		UpdateVelocity = true;
		Velocity.x = -1.0f;
	}
	else if (MoveRight == true && Velocity.x < 0.001f)
	{
		UpdateVelocity = true;
		Velocity.x = 1.0f;
	}
	else if (MoveLeft == false && Velocity.x < -0.001f)
	{
		UpdateVelocity = true;
		Velocity.x = 0.0f;
	}
	else if (MoveRight == false && Velocity.x > 0.001f)
	{
		UpdateVelocity = true;
		Velocity.x = 0.0f;
	}

	if (UpdateVelocity == true)
	{
		FGame::registry.patch<FRigidBodyComponent>(Entity, [&](auto& RigidBodyComponent) { RigidBodyComponent.Velocity = Velocity; });
	}
}

void FPlayerController::OnKeyDown(sf::Keyboard::Key Key)
{
	if (Key == sf::Keyboard::W)
	{
		MoveUp = true;
	}
	else if (Key == sf::Keyboard::S)
	{
		MoveDown = true;
	}
	else if (Key == sf::Keyboard::A)
	{
		MoveLeft = true;
	}
	else if (Key == sf::Keyboard::D)
	{
		MoveRight = true;
	}
}

void FPlayerController::OnKeyUp(sf::Keyboard::Key Key)
{
	if (Key == sf::Keyboard::W)
	{
		MoveUp = false;
	}
	else if (Key == sf::Keyboard::S)
	{
		MoveDown = false;
	}
	else if (Key == sf::Keyboard::A)
	{
		MoveLeft = false;
	}
	else if (Key == sf::Keyboard::D)
	{
		MoveRight = false;
	}
}
