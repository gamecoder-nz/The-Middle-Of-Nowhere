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
    FGame::registry.on_update<FRigidBodyComponent>().connect<&FPhysicsSystem::OnUpdate>(this);

	Box2dWorld.SetContactListener(this);
}

void FPhysicsSystem::Update(float dt)
{
    while (BodiesToRemove.size() > 0)
    {
        Box2dWorld.DestroyBody(BodiesToRemove[0]);
        BodiesToRemove.erase(BodiesToRemove.begin());
    }

    Box2dWorld.Step(dt, 6, 2);

    auto View = FGame::registry.view<FRigidBodyComponent>();

    for (auto Entity : View)
    {
        FRigidBodyComponent RigidBodyComponent = FGame::registry.get<FRigidBodyComponent>(Entity);
        FTransformComponent TransformComponent = FGame::registry.get<FTransformComponent>(Entity);
        b2Vec2 Box2DPosition = RigidBodyComponent.Box2dBody->GetPosition();
        Box2DPosition *= BOX2D_SCALE_FACTOR;

        sf::Vector2f NewPosition(Box2DPosition.x, Box2DPosition.y);
        NewPosition -= sf::Vector2f(RigidBodyComponent.Hitbox.left, RigidBodyComponent.Hitbox.top);

        FGame::registry.patch<FTransformComponent>(Entity, [&](FTransformComponent& TransformComponent) { TransformComponent.Position = NewPosition; });
    }
}

void FPhysicsSystem::OnConstruct(entt::registry& Registry, entt::entity Entity)
{
    FTransformComponent TransformComponent = Registry.get<FTransformComponent>(Entity);
    FRigidBodyComponent RigidBodyComponent = Registry.get<FRigidBodyComponent>(Entity);
    RigidBodyComponent.Hitbox.left = TransformComponent.Position.x + RigidBodyComponent.Hitbox.left;
    RigidBodyComponent.Hitbox.top = TransformComponent.Position.y + RigidBodyComponent.Hitbox.top;

    b2BodyDef BodyDef;
    BodyDef.type = b2_dynamicBody;
    BodyDef.position.Set(RigidBodyComponent.Hitbox.left / BOX2D_SCALE_FACTOR, RigidBodyComponent.Hitbox.top / BOX2D_SCALE_FACTOR);
    BodyDef.userData.pointer = reinterpret_cast<uintptr_t>(&Entity);
    b2Body* Body = Box2dWorld.CreateBody(&BodyDef);

    Registry.patch<FRigidBodyComponent>(Entity, [&](FRigidBodyComponent& RigidBodyComponent) { RigidBodyComponent.Box2dBody = Body; });

    b2PolygonShape DynamicBox;
    DynamicBox.SetAsBox(RigidBodyComponent.Hitbox.width / BOX2D_SCALE_FACTOR, RigidBodyComponent.Hitbox.height / BOX2D_SCALE_FACTOR);

    b2FixtureDef FixtureDef;
    FixtureDef.shape = &DynamicBox;
    FixtureDef.density = 1.0f;
    FixtureDef.friction = 0.3f;

    Body->CreateFixture(&FixtureDef);
}

void FPhysicsSystem::OnUpdate(entt::registry& Registry, entt::entity Entity)
{
    FRigidBodyComponent RigidBodyComponent = Registry.get<FRigidBodyComponent>(Entity);
    b2Vec2 NewVelocity({ RigidBodyComponent.Velocity.x, RigidBodyComponent.Velocity.y });
    
    float Speed = NewVelocity.Normalize();
    NewVelocity *= Speed;

    RigidBodyComponent.Box2dBody->SetLinearVelocity(NewVelocity);
}

void FPhysicsSystem::OnDestroy(entt::registry& Registry, entt::entity Entity)
{
    FRigidBodyComponent RigidBodyComponent = Registry.get<FRigidBodyComponent>(Entity);
    BodiesToRemove.push_back(RigidBodyComponent.Box2dBody);
}

void FPhysicsSystem::BeginContact(b2Contact* contact)
{
}
