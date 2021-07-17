#ifndef CONTROLLER_SYSTEM_HPP
#define CONTROLELR_SYSTEM_HPP

#include "System.hpp"
#include <entt/entt.hpp>

class FControllerSystem :
    public FSystem
{
public:
	FControllerSystem();

	virtual void ConnectSignals() final;
	virtual void Initialise() final;
	virtual void Update(float dt) final;

protected:
	void OnDestroy(entt::registry& Registry, entt::entity Entity);
private:
};

#endif