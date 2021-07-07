#ifndef GAME_SIGNALS_HPP
#define GAME_SIGNALS_HPP

#include "entt/entt.hpp"

entt::sigh<void()> QuitGameSignal;
entt::sink QuitGameSignalSink{ QuitGameSignal };

#endif