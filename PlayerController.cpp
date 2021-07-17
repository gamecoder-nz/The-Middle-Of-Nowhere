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
	sf::Vector2f &Velocity = FGame::registry.get<FRigidBodyComponent>(Entity).Velocity;

	if (MoveUp == true && Velocity.y > -0.001f)
	{
		UpdateVelocity = true;
		Velocity.y = -1.0f;
	}
	if (MoveDown == true && Velocity.y < 0.001f)
	{
		UpdateVelocity = true;
		Velocity.y = 1.0f;
	}
	if (MoveUp == false && Velocity.y < -0.001f)
	{
		UpdateVelocity = true;
		Velocity.y = 0.0f;
	}
	if (MoveDown == false && Velocity.y > 0.001f)
	{
		UpdateVelocity = true;
		Velocity.y = 0.0f;
	}

	if (MoveLeft == true && Velocity.x > -0.001f)
	{
		UpdateVelocity = true;
		Velocity.x = -1.0f;
	}
	if (MoveRight == true && Velocity.x < 0.001f)
	{
		UpdateVelocity = true;
		Velocity.x = 1.0f;
	}
	if (MoveLeft == false && Velocity.x < -0.001f)
	{
		UpdateVelocity = true;
		Velocity.x = 0.0f;
	}
	if (MoveRight == false && Velocity.x > 0.001f)
	{
		UpdateVelocity = true;
		Velocity.x = 0.0f;
	}

	if (UpdateVelocity == true)
	{
		if (MoveRight == true)
		{
			PlayAnimationSignal.publish(Entity, "WALK_RIGHT");
		}
		else if (MoveLeft == true)
		{
			PlayAnimationSignal.publish(Entity, "WALK_LEFT");
		}
		else if (MoveUp == true)
		{
			PlayAnimationSignal.publish(Entity, "WALK_UP");
		}
		else if (MoveDown == true)
		{
			PlayAnimationSignal.publish(Entity, "WALK_DOWN");
		}
		else
		{
			int Rotation = FGame::registry.get<FTransformComponent>(Entity).Rotation;

			if (Rotation == 0)
			{
				PlayAnimationSignal.publish(Entity, "IDLE_UP");
			}
			else if (Rotation == 90)
			{
				PlayAnimationSignal.publish(Entity, "IDLE_RIGHT");
			}
			else if (Rotation == 180)
			{
				PlayAnimationSignal.publish(Entity, "IDLE_DOWN");
			}
			else if (Rotation == 270)
			{
				PlayAnimationSignal.publish(Entity, "IDLE_LEFT");
			}
		}
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
