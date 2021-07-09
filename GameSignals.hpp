#ifndef GAME_SIGNALS_HPP
#define GAME_SIGNALS_HPP

#include <entt/entt.hpp>
#include <SFML/Window/Keyboard.hpp>

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

#endif