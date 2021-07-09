#ifndef RENDER_SYSTEM_HPP
#define RENDER_SYSTEM_HPP

#include "System.hpp"
#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>

class FRenderSystem :
    public FSystem
{
public:
	FRenderSystem(sf::RenderWindow* Window);

	virtual void Initialise() final;
	virtual void Update(float dt) final;

protected:
	void OnConstruct(entt::registry&, entt::entity);

private:
	sf::RenderWindow* Window;
};

#endif