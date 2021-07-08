#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <box2d/b2_body.h>

struct FTransformComponent
{
	sf::Vector2f Position;
};

struct FRigidBodyComponent
{
	sf::Vector2f Velocity;
	sf::Vector2f HitboxOffset;
	sf::FloatRect Hitbox;
	b2Body* Box2dBody;
};
#endif
