#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/Rect.hpp>

struct TransformComponent
{
	sf::Vector2f Position;
};

struct RigidBodyComponent
{
	sf::Vector2f Velocity;
	sf::Vector2f HitboxOffset;
	sf::FloatRect Hitbox;
};
#endif
