#ifndef RENDER_SYSTEM_HPP
#define RENDER_SYSTEM_HPP

#include "System.hpp"
#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/Map.hpp>

#ifdef _DEBUG
#include <box2d/box2d.h>

class FBox2dDraw : public b2Draw
{
public:
	FBox2dDraw(sf::RenderWindow* Window);
	void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);

	void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);

	void DrawCircle(const b2Vec2& center, float radius, const b2Color& color);

	void DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color);

	void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);

	void DrawTransform(const b2Transform& xf);

	void DrawPoint(const b2Vec2& p, float size, const b2Color& color);

private:
	sf::RenderWindow* Window;
};
#endif

class FRenderSystem :
    public FSystem
{
public:
	FRenderSystem(sf::RenderWindow* Window);

	virtual void ConnectSignals() final;
	virtual void Initialise() final;
	virtual void Update(float dt) final;

protected:
	void OnConstruct(entt::registry&, entt::entity);
	void OnPlayerConstruct(entt::registry& Registry, entt::entity Entity);
	void OnChangeMap(entt::entity MapEntity, sf::Vector2f NewPosition);
	void OnCreateMapLayers(entt::entity MapEntity, tmx::Map& TmxMap);

private:
	sf::RenderWindow* Window;
	entt::entity MapEntity;
	entt::entity PlayerEntity;

#ifdef _DEBUG
	b2World* Box2dWorld;
	FBox2dDraw* Box2dDraw;
#endif
};

#endif