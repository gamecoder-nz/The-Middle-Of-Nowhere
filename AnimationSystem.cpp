#include "AnimationSystem.hpp"
#include "Components.hpp"
#include "Game.hpp"
#include "GameSignals.hpp"

FAnimationSystem::FAnimationSystem()
{
}

void FAnimationSystem::ConnectSignals()
{
	PlayAnimationSignalSink.connect<&FAnimationSystem::PlayAnimation>(this);
}

void FAnimationSystem::Initialise()
{
	
}

void FAnimationSystem::Update(float dt)
{
	auto View = FGame::registry.view<FAnimationComponent>();

	for (auto Entity : View)
	{
		FAnimationComponent& AnimationComponent = FGame::registry.get<FAnimationComponent>(Entity);
		FSpriteComponent& SpriteComponent = FGame::registry.get<FSpriteComponent>(Entity);

		AnimationComponent.CurrentFrameTime += dt;
		FAnimation CurrentAnimation = AnimationComponent.CurrentAnimation;
		sf::Vector2i FrameDimensions = CurrentAnimation.FrameDimensions;

		if (AnimationComponent.CurrentFrameTime >= CurrentAnimation.TimePerFrame)
		{
			AnimationComponent.CurrentFrame++;
			AnimationComponent.CurrentFrameTime = 0.0f;

			if (AnimationComponent.CurrentFrame == CurrentAnimation.NumberOfFrames)
			{
				AnimationComponent.CurrentFrame = 0;
			}

			SpriteComponent.Sprite.setTextureRect(sf::IntRect(AnimationComponent.CurrentFrame* FrameDimensions.x, FrameDimensions.y* CurrentAnimation.YIndex, FrameDimensions.x, FrameDimensions.y));
		}
	}
}

void FAnimationSystem::PlayAnimation(entt::entity Entity, std::string AnimationName)
{
	FAnimationComponent& AnimationComponent = FGame::registry.get<FAnimationComponent>(Entity);
	FSpriteComponent& SpriteComponent = FGame::registry.get<FSpriteComponent>(Entity);

	AnimationComponent.CurrentFrame = 0;
	AnimationComponent.CurrentFrameTime = 0.0f;
	AnimationComponent.CurrentAnimation = AnimationComponent.Animations[AnimationName];

	sf::Vector2i FrameDimensions = AnimationComponent.CurrentAnimation.FrameDimensions;

	SpriteComponent.Sprite.setTextureRect(sf::IntRect(0, FrameDimensions.y * AnimationComponent.CurrentAnimation.YIndex, FrameDimensions.x, FrameDimensions.y));
}
