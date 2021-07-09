#ifndef ANIMATION_SYSTEM_HPP
#define ANIMATION_SYSTEM_HPP

#include "System.hpp"
#include <string>
#include <entt/entt.hpp>

class FAnimationSystem :
    public FSystem
{
public:
	FAnimationSystem();

	virtual void Initialise() final;
	virtual void Update(float dt) final;

protected:
	void PlayAnimation(entt::entity Entity, std::string AnimationName);
private:
};

#endif
