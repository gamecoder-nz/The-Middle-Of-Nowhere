#include "ControllerSystem.hpp"
#include "Components.hpp"
#include "Game.hpp"
#include "Controller.hpp"

FControllerSystem::FControllerSystem()
{
}

void FControllerSystem::ConnectSignals()
{
    FGame::registry.on_destroy<FControllerComponent>().connect<&FControllerSystem::OnDestroy>(this);
}

void FControllerSystem::Initialise()
{
    
}

void FControllerSystem::Update(float dt)
{
    auto View = FGame::registry.view<FControllerComponent>();

    for (auto Entity : View)
    {
        FGame::registry.get<FControllerComponent>(Entity).Controller->Update(dt);
    }
}

void FControllerSystem::OnDestroy(entt::registry& Registry, entt::entity Entity)
{
    FController* Controller = Registry.get<FControllerComponent>(Entity).Controller;

    Controller->Unpossess();
    delete Controller;
}
