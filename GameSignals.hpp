#ifndef GAME_SIGNALS_HPP
#define GAME_SIGNALS_HPP

#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>
#include <tmxlite/Map.hpp>

class b2World;

extern entt::sigh<void()> QuitGameSignal;
extern entt::sink<void()> QuitGameSignalSink;

extern entt::sigh<void(int)> CreateEntitySignal;
extern entt::sink<void(int)> CreateEntitySignalSink;

extern entt::sigh<void(sf::Keyboard::Key)> KeyDownSignal;
extern entt::sink<void(sf::Keyboard::Key)> KeyDownSignalSink;

extern entt::sigh<void(sf::Keyboard::Key)> KeyUpSignal;
extern entt::sink<void(sf::Keyboard::Key)> KeyUpSignalSink;

extern entt::sigh<void(entt::entity, std::string)> PlayAnimationSignal;
extern entt::sink<void(entt::entity, std::string)> PlayAnimationSignalSink;

extern entt::sigh<void(entt::entity, sf::Vector2f)> ChangeMapSignal;
extern entt::sink<void(entt::entity, sf::Vector2f)> ChangeMapSignalSink;

extern entt::sigh<void(entt::entity, tmx::Map &TmxMap)> CreateMapLayersSignal;
extern entt::sink<void(entt::entity, tmx::Map &TmxMap)> CreateMapLayersSignalSink;

#ifdef _DEBUG
extern entt::sigh<b2World*()> GetBox2dWorldSignal;
extern entt::sink<b2World*()> GetBox2dWorldSignalSink;
#endif

#endif