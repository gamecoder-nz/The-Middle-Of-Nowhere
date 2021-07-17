#ifndef WORLD_SYSTEM_HPP
#define WORLD_SYSTEM_HPP

#include "System.hpp"
#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>

#include <vector>

const int RIGID_BODY_COMPONENT    = 1 << 0;
const int SPRITE_COMPONENT        = 1 << 1;
const int CONTROLLER_COMPONENT    = 1 << 2;
const int ANIMATION_COMPONENT     = 1 << 3;
const int MESH_COMPONENT          = 1 << 4;
const int MAP_COMPONENT           = 1 << 5;
const int PLAYER_COMPONENT        = 1 << 6;

class FWorldSystem : public FSystem
{
public:
	FWorldSystem();

	virtual void ConnectSignals() final;
	virtual void Initialise() final;
	virtual void Update(float dt) final;

	entt::entity CreateEntity(const int flags);

protected:
	void CreateAnimation(entt::entity Entity, std::string AnimationName, sf::Vector2i FrameDimensions, int NumberOfFrames, float TimePerFrame, int YIndex);
	void CreatePlayer();
	void LoadMaps();

	std::vector<entt::entity> Entities;
	entt::entity PlayerEntity;
	std::map<std::string, entt::entity> MapEntities;
private:
};
#endif