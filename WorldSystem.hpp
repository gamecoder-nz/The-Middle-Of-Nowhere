#ifndef WORLD_SYSTEM_HPP
#define WORLD_SYSTEM_HPP

#include "System.hpp"
#include <entt/entt.hpp>
#include <vector>

const int RIGID_BODY_COMPONENT = 1 << 0;
const int SPRITE_COMPONENT = 1 << 1;
const int CONTROLLER_COMPONENT = 1 << 2;

class FWorldSystem : public FSystem
{
public:
	FWorldSystem();

	virtual void Initialise() final;
	virtual void Update(float dt) final;

	void CreateEntity(const int flags);

protected:
	std::vector<entt::entity> Entities;
private:
};
#endif