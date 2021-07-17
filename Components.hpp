#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

#include <SFML/Graphics.hpp>
#include <box2d/b2_body.h>

class FController;

struct FTransformComponent
{
	sf::Vector2f Position;
	int Rotation;
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

struct FMeshComponent
{
	sf::VertexArray VertexArray;
	sf::Texture Texture;
};

struct FMapComponent
{
	std::vector<sf::VertexArray> MapLayers;
	sf::Texture Texture;
	std::vector<bool> WalkableTiles;
	sf::Vector2u MapDimensions;
};

struct FPlayerComponent
{

};
#endif
