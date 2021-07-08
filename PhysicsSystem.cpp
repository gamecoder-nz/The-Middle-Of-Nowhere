#include "PhysicsSystem.hpp"
#include "Game.hpp"
#include "Components.hpp"
#include "GameConfiguration.hpp"

FPhysicsSystem::FPhysicsSystem() : Box2dWorld({ 0.0f, 0.0f })
{
}

void FPhysicsSystem::Initialise()
{
	FGame::registry.on_construct<FRigidBodyComponent>().connect<&FPhysicsSystem::OnConstruct>(this);
	Box2dWorld.SetContactListener(this);
}

void FPhysicsSystem::Update(float dt)
{
    Box2dWorld.Step(dt, 6, 2);
}

void FPhysicsSystem::OnConstruct(entt::registry& Registry, entt::entity Entity)
{
    FTransformComponent TransformComponent = Registry.get<FTransformComponent>(Entity);
    FRigidBodyComponent RigidBodyComponent = Registry.get<FRigidBodyComponent>(Entity);
    RigidBodyComponent.Hitbox.left = TransformComponent.Position.x + RigidBodyComponent.HitboxOffset.x;
    RigidBodyComponent.Hitbox.top = TransformComponent.Position.y + RigidBodyComponent.HitboxOffset.y;

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(RigidBodyComponent.Hitbox.left / BOX2D_SCALE_FACTOR, RigidBodyComponent.Hitbox.top / BOX2D_SCALE_FACTOR);
    bodyDef.userData.pointer = reinterpret_cast<uintptr_t>(&Entity);
    b2Body* body = Box2dWorld.CreateBody(&bodyDef);

    Registry.patch<FRigidBodyComponent>(Entity, [&](FRigidBodyComponent& RigidBodyComponent) { RigidBodyComponent.Box2dBody = body; });

    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(0.1f, 0.1f);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;

    body->CreateFixture(&fixtureDef);
}

void FPhysicsSystem::BeginContact(b2Contact* contact)
{
}
