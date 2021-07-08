#ifndef GAME_SIGNALS_HPP
#define GAME_SIGNALS_HPP

#include "entt/entt.hpp"

extern entt::sigh<void()> QuitGameSignal;
extern entt::sink<void()> QuitGameSignalSink;

extern entt::sigh<void(int)> CreateEntitySignal;
extern entt::sink<void(int)> CreateEntitySignalSink;

#endif