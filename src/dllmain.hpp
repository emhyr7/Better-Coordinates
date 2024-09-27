#pragma once

#include <Windows.h>

#include <amethyst/runtime/ModContext.hpp>
#include <amethyst/runtime/events/GameEvents.hpp>

#define log_info(...) Log::Info("[Better-Coordinates] " __VA_ARGS__)

#define EXPORT extern "C" __declspec(dllexport)

void OnStartJoinGame(OnStartJoinGameEvent& event);
