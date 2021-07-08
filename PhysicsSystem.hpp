#ifndef PHYSICS_SYSTEM_HPP
#define PHYSICS_SYSTEM_HPP

#include "System.hpp"
#include "entt/entt.hpp"
#include <box2d/box2d.h>

class FPhysicsSystem :
    public FSystem, public b2ContactListener
{
public:
	FPhysicsSystem();

	virtual void Initialise() final;
	virtual void Update(float dt) final;

protected:
	b2World Box2dWorld;

	void OnConstruct(entt::registry&, entt::entity);
	void BeginContact(b2Contact* contact);

private:
};

#endif