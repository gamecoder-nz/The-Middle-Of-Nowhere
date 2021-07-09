#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <box2d/b2_body.h>

class FController;

struct FTransformComponent
{
	sf::Vector2f Position;
};

struct FRigidBodyComponent
{
	sf::Vector2f Velocity;
	sf::FloatRect Hitbox;
	b2Body* Box2dBody;
};

struct FSpriteComponent
{
	sf::Sprite Sprite;
};

struct FControllerComponent
{
	FController* Controller;
};

struct FAnimation
{
	sf::Vector2i FrameDimensions;
	int YIndex;
	int NumberOfFrames;
	float TimePerFrame;
};

struct FAnimationComponent
{
	std::map<std::string, FAnimation> Animations;
	FAnimation CurrentAnimation;
	float CurrentFrameTime;
	int CurrentFrame;
};
#endif
