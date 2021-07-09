#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <entt/entt.hpp>

class FController
{
public:
	FController();
	
	virtual void Possess(entt::entity Entity) = 0;
	virtual void Unpossess() = 0;
	virtual void Update(float dt) = 0;

protected:
	entt::entity Entity;
};

#endif