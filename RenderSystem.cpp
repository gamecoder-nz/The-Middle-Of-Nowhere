#include "RenderSystem.hpp"
#include "Game.hpp"
#include "Components.hpp"

FRenderSystem::FRenderSystem(sf::RenderWindow* Window)
{
	this->Window = Window;
}

void FRenderSystem::Initialise()
{
	FGame::registry.on_construct<FSpriteComponent>().connect<&FRenderSystem::OnConstruct>(this);
}

void FRenderSystem::Update(float dt)
{
	auto View = FGame::registry.view<FSpriteComponent>();
    Window->clear();

    for (auto Entity : View)
    {
        FSpriteComponent SpriteComponent = FGame::registry.get<FSpriteComponent>(Entity);
        FTransformComponent TransformComponent = FGame::registry.get<FTransformComponent>(Entity);
        SpriteComponent.Sprite.move(TransformComponent.Position);
        Window->draw(SpriteComponent.Sprite);
    }

    Window->display();
}

void FRenderSystem::OnConstruct(entt::registry&, entt::entity)
{
}
