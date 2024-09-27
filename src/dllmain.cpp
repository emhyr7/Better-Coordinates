#include "dllmain.hpp"

#include <print>

#include <amethyst/runtime/events/InputEvents.hpp>
#include <minecraft/src/common/world/level/storage/GameRules.hpp>
#include <minecraft/src/common/world/level/Level.hpp>
#include <minecraft/src/common/Minecraft.hpp>

using namespace Amethyst;

bool checking_started = false;

AmethystContext *context; 

unsigned char prior_gamerule_state[sizeof(GameRule)];
bool gamerule_state_difference[sizeof(GameRule)];

bool made_input_handlers = false;

void handle_on_start_join_game(OnStartJoinGameEvent &event)
{
  if (!made_input_handlers)
  {
    context->mInputManager->AddButtonDownHandler(
      "show_coordinates",
      [](FocusImpact, ClientInstance &client)
      {
        checking_started = true;
        Level *level = client.minecraft->getLevel();
        GameRules &gamerules = level->getGameRules();
        unsigned char *show_choordinates_p = ((unsigned char *)&gamerules.mGamerules[GameRulesIndex::ShowCoordinates]) + 4;
        bool show_choordinates = *show_choordinates_p;
        show_choordinates ^= true;
        *show_choordinates_p = show_choordinates;
        log_info("showing_coordinates: {}", show_choordinates);
      },
      false);
    made_input_handlers = true;
  }
}

void handle_register_inputs(RegisterInputsEvent &event)
{
  event.inputManager.RegisterNewInput("show_coordinates", {VK_F3});
}

void handle_update(UpdateEvent &)
{
#if 0
  if (!checking_started) return;
  Level *level = GetLevel();
  GameRules &gamerules = level->getGameRules();
  unsigned char *bytes = (unsigned char *)&gamerules.mGamerules[GameRulesIndex::ShowCoordinates];
  bool differed_ever = false;
  for (int i = 0; i < sizeof(GameRule) - 1; ++i)
  {
    unsigned char from = prior_gamerule_state[i];
    unsigned char to = bytes[i];
    bool differed = from != to;
    gamerule_state_difference[i] = differed;
    if (differed)
    {
      differed_ever = true;
      std::print("{}:{}:{} ", i, from, to);
    }
  }
  if (differed_ever) std::print("\n---------------");
  memcpy(prior_gamerule_state, bytes, sizeof(GameRule));
#endif
}

EXPORT void Initialize(AmethystContext &_context)
{
  context = &_context;
  
  InitializeAmethystMod(*context);
  context->mFeatures->enableInputSystem = true;

  auto &events = GetEventBus();
  events.AddListener<UpdateEvent>(&handle_update);
  events.AddListener<RegisterInputsEvent>(&handle_register_inputs);
  events.AddListener<OnStartJoinGameEvent>(&handle_on_start_join_game);
  log_info("Initialized successfully.");
}

BOOL APIENTRY DllMain(HMODULE, DWORD, LPVOID)
{
  return TRUE;
}
