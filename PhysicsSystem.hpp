#ifndef PHYSICS_SYSTEM_HPP
#define PHYSICS_SYSTEM_HPP

#include "System.hpp"
#include <entt/entt.hpp>
#include <box2d/box2d.h>
#include <SFML/Graphics.hpp>

const uint16 NOTHING         = 0;
const uint16 PLAYER          = 1 << 0;
const uint16 ENTITY          = 1 << 1;
const uint16 WALL            = 1 << 2;
const uint16 OBJECT          = 1 << 3;


class FPhysicsSystem :
    public FSystem, public b2ContactListener
{
public:
	FPhysicsSystem();

	virtual void ConnectSignals() final;
	virtual void Initialise() final;
	virtual void Update(float dt) final;

protected:
	b2World Box2dWorld;
	std::vector<b2Body*> BodiesToRemove;
	std::vector<b2Body*> MapCollisionBodies;
	entt::entity PlayerEntity;
	sf::Vector2u PlayerHitboxStartingTileCoordinates;
	entt::entity MapEntity;

	void MoveCollisionBoxes();

	void OnConstruct(entt::registry& Registry, entt::entity Entity);
	void OnPlayerConstruct(entt::registry& Registry, entt::entity Entity);
	void OnUpdate(entt::registry& Registry, entt::entity Entity);
	void OnDestroy(entt::registry& Registry, entt::entity Entity);
	void OnChangeMap(entt::entity MapEntity, sf::Vector2f NewPosition);
#ifdef _DEBUG
	b2World* GetBox2dWorld();
#endif
	void BeginContact(b2Contact* contact);
	void EndContact(b2Contact* contact);

private:
};

#endif