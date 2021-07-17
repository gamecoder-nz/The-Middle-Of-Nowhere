#include "PhysicsSystem.hpp"
#include "Game.hpp"
#include "Components.hpp"
#include "GameConfiguration.hpp"
#include "GameSignals.hpp"

#include <bitset>

FPhysicsSystem::FPhysicsSystem() : Box2dWorld({ 0.0f, 0.0f })
{
    PlayerEntity = entt::null;
}

void FPhysicsSystem::ConnectSignals()
{
    FGame::registry.on_construct<FRigidBodyComponent>().connect<&FPhysicsSystem::OnConstruct>(this);
    FGame::registry.on_construct<FPlayerComponent>().connect<&FPhysicsSystem::OnPlayerConstruct>(this);
    FGame::registry.on_update<FRigidBodyComponent>().connect<&FPhysicsSystem::OnUpdate>(this);
    FGame::registry.on_update<FTransformComponent>().connect<&FPhysicsSystem::OnUpdate>(this);
    ChangeMapSignalSink.connect<&FPhysicsSystem::OnChangeMap>(this);

#ifdef _DEBUG
    GetBox2dWorldSignalSink.connect<&FPhysicsSystem::GetBox2dWorld>(this);
#endif
}

void FPhysicsSystem::Initialise()
{
	Box2dWorld.SetContactListener(this);
}

void FPhysicsSystem::Update(float dt)
{
    auto View = FGame::registry.view<FRigidBodyComponent>(); 
    for (auto Entity : View)
    {
        FRigidBodyComponent RigidBodyComponent = FGame::registry.get<FRigidBodyComponent>(Entity);
        FTransformComponent& TransformComponent = FGame::registry.get<FTransformComponent>(Entity);
        b2Vec2 NewVelocity({ RigidBodyComponent.Velocity.x, RigidBodyComponent.Velocity.y });

        if (NewVelocity.x > 0.001f)
        {
            TransformComponent.Rotation = 90;
        }
        else if (NewVelocity.x < -0.001f)
        {
            TransformComponent.Rotation = 270;
        }
        else if (NewVelocity.y > 0.001f)
        {
            TransformComponent.Rotation = 180;
        }
        else if (NewVelocity.y < -0.001f)
        {
            TransformComponent.Rotation = 0;
        }

        NewVelocity.Normalize();

        RigidBodyComponent.Box2dBody->SetLinearVelocity(NewVelocity);
    }

    while (BodiesToRemove.size() > 0)
    {
        Box2dWorld.DestroyBody(BodiesToRemove[0]);
        BodiesToRemove.erase(BodiesToRemove.begin());
    }

    Box2dWorld.Step(1.0f / 60.0f, 6, 2);

    for (auto Entity : View)
    {
        FRigidBodyComponent RigidBodyComponent = FGame::registry.get<FRigidBodyComponent>(Entity);
        FTransformComponent &TransformComponent = FGame::registry.get<FTransformComponent>(Entity);
        b2Vec2 Box2DPosition = RigidBodyComponent.Box2dBody->GetTransform().p;
        Box2DPosition *= BOX2D_SCALE_FACTOR;

        sf::Vector2f NewPosition(Box2DPosition.x, Box2DPosition.y);
        NewPosition -= sf::Vector2f(RigidBodyComponent.Hitbox.left + RigidBodyComponent.Hitbox.width / 2.0f, RigidBodyComponent.Hitbox.top + RigidBodyComponent.Hitbox.height / 2.0f);

        TransformComponent.Position = NewPosition;
    }
}

void FPhysicsSystem::OnConstruct(entt::registry& Registry, entt::entity Entity)
{
    FTransformComponent TransformComponent = Registry.get<FTransformComponent>(Entity);
    FRigidBodyComponent &RigidBodyComponent = Registry.get<FRigidBodyComponent>(Entity);
    sf::FloatRect Hitbox = RigidBodyComponent.Hitbox;

    Hitbox.left = TransformComponent.Position.x + Hitbox.left + Hitbox.width / 2.0f;
    Hitbox.top = TransformComponent.Position.y + Hitbox.top + Hitbox.height / 2.0f;

    b2BodyDef BodyDef;
    BodyDef.type = b2_dynamicBody;
    BodyDef.position.Set(Hitbox.left / BOX2D_SCALE_FACTOR, Hitbox.top / BOX2D_SCALE_FACTOR);
    BodyDef.userData.pointer = reinterpret_cast<uintptr_t>(&Entity);
    BodyDef.fixedRotation = true;
    b2Body* Body = Box2dWorld.CreateBody(&BodyDef);

    RigidBodyComponent.Box2dBody = Body;

    b2PolygonShape DynamicBox;
    DynamicBox.SetAsBox(Hitbox.width / BOX2D_SCALE_FACTOR / 2.0f, Hitbox.height / BOX2D_SCALE_FACTOR / 2.0f);

    b2FixtureDef FixtureDef;
    FixtureDef.shape = &DynamicBox;
    FixtureDef.density = 1.0f;
    FixtureDef.friction = 0.3f;

    if (Registry.any_of<FPlayerComponent>(Entity) == true)
    {
        FixtureDef.filter.categoryBits = PLAYER;
        FixtureDef.filter.maskBits = WALL | ENTITY | OBJECT;
    }

    Body->CreateFixture(&FixtureDef);
}

void FPhysicsSystem::OnPlayerConstruct(entt::registry& Registry, entt::entity Entity)
{
    PlayerEntity = Entity;
}

void FPhysicsSystem::MoveCollisionBoxes()
{
    FTransformComponent TransformComponent = FGame::registry.get<FTransformComponent>(PlayerEntity);
    FRigidBodyComponent RigidBodyComponent = FGame::registry.get<FRigidBodyComponent>(PlayerEntity);
    FMapComponent MapComponent = FGame::registry.get<FMapComponent>(MapEntity);

    RigidBodyComponent.Hitbox.left = TransformComponent.Position.x + RigidBodyComponent.Hitbox.left;
    RigidBodyComponent.Hitbox.top = TransformComponent.Position.y + RigidBodyComponent.Hitbox.top;

    std::vector<std::vector<sf::Vector2u>> BoxGroups;
    std::vector<bool> ClosedSet;
    ClosedSet.resize(MapComponent.MapDimensions.x * MapComponent.MapDimensions.y);

    for (int Y = 0; Y < MapComponent.MapDimensions.y; Y++)
    {
        for (int X = 0; X < MapComponent.MapDimensions.x; X++)
        {
            int Index = Y * MapComponent.MapDimensions.x + X;

            if (MapComponent.WalkableTiles[Index] == true || ClosedSet[Index] == true)
            {
                continue;
            }

            std::vector<sf::Vector2u> OpenSet({ sf::Vector2u(X, Y) });
            std::vector<sf::Vector2u> BoxGroup;

            while (OpenSet.size() > 0)
            {
                sf::Vector2u CurrentVertex = OpenSet[0];
                int CurrentVertexIndex = CurrentVertex.y * MapComponent.MapDimensions.x + CurrentVertex.x;
                OpenSet.erase(OpenSet.begin());

                if (ClosedSet[CurrentVertexIndex] == true)
                {
                    continue;
                }

                ClosedSet[CurrentVertexIndex] = true;

                BoxGroup.push_back(CurrentVertex);

                if (CurrentVertex.x > 0 && MapComponent.WalkableTiles[CurrentVertexIndex - 1] == false && ClosedSet[CurrentVertexIndex - 1] == false)
                {
                    OpenSet.push_back(CurrentVertex - sf::Vector2u(1, 0));
                }

                if (CurrentVertex.y > 0 && MapComponent.WalkableTiles[CurrentVertexIndex - MapComponent.MapDimensions.x] == false && ClosedSet[CurrentVertexIndex - MapComponent.MapDimensions.x] == false)
                {
                    OpenSet.push_back(CurrentVertex - sf::Vector2u(0, 1));
                }

                if (CurrentVertex.x < 49 && MapComponent.WalkableTiles[CurrentVertexIndex + 1] == false && ClosedSet[CurrentVertexIndex + 1] == false)
                {
                    OpenSet.push_back(CurrentVertex + sf::Vector2u(1, 0));
                }

                if (CurrentVertex.y < 49 && MapComponent.WalkableTiles[CurrentVertexIndex + MapComponent.MapDimensions.x] == false && ClosedSet[CurrentVertexIndex + MapComponent.MapDimensions.x] == false)
                {
                    OpenSet.push_back(CurrentVertex + sf::Vector2u(0, 1));
                }
            }

            BoxGroups.push_back(BoxGroup);
        }
    }

    for (std::vector<sf::Vector2u> BoxGroup : BoxGroups)
    {
        std::vector<sf::Vector2u> Vertices;
        std::vector<sf::Vector2u> UnfilteredVertices;

        std::bitset<50 * 50> BoxMap;

        for (sf::Vector2u Box : BoxGroup)
        {
            BoxMap[Box.y * 50 + Box.x] = true;

            UnfilteredVertices.push_back(Box * TILE_SIZE);
            UnfilteredVertices.push_back((Box + sf::Vector2u(1, 0)) * TILE_SIZE);
            UnfilteredVertices.push_back((Box + sf::Vector2u(1, 1)) * TILE_SIZE);
            UnfilteredVertices.push_back((Box + sf::Vector2u(0, 1)) * TILE_SIZE);
        }

        for (sf::Vector2u Vertex : UnfilteredVertices)
        {
            if (std::count(UnfilteredVertices.begin(), UnfilteredVertices.end(), Vertex) % 2 == 1 && std::find(Vertices.begin(), Vertices.end(), Vertex) == Vertices.end())
            {
                Vertices.push_back(Vertex);
            }
        }

        sf::Vector2u CurrentVertex = Vertices[0];
        Vertices.erase(Vertices.begin());

        std::vector<b2Vec2> OrderedVertices;

        while (Vertices.size() > 0)
        {
            int CornerType = 0;
            sf::Vector2u TileCoordinates = CurrentVertex / TILE_SIZE;

            if (BoxMap[TileCoordinates.y * 50 + TileCoordinates.x] == true)
            {
                CornerType += 2;
            }

            if (BoxMap[TileCoordinates.y * 50 + TileCoordinates.x - 1] == true)
            {
                CornerType += 1;
            }

            if (BoxMap[(TileCoordinates.y - 1) * 50 + TileCoordinates.x] == true)
            {
                CornerType += 4;
            }

            if (BoxMap[(TileCoordinates.y - 1) * 50 + TileCoordinates.x - 1] == true)
            {
                CornerType += 8;
            }

            OrderedVertices.push_back(b2Vec2(float(CurrentVertex.x) / BOX2D_SCALE_FACTOR, float(CurrentVertex.y) / BOX2D_SCALE_FACTOR));
            auto It = Vertices.begin();
            std::vector<sf::Vector2u> NextVertices;


            switch (CornerType)
            {
            case 1:
            case 13:

                while ((It = std::find_if(It, Vertices.end(), [&](sf::Vector2u ThisVertex) {
                    return ThisVertex.x == CurrentVertex.x && ThisVertex.y > CurrentVertex.y;
                    })) != Vertices.end())
                {
                    NextVertices.push_back(*It);
                    It++;
                }

                CurrentVertex = *std::min_element(NextVertices.begin(), NextVertices.end(), [](sf::Vector2u Left, sf::Vector2u Right) {
                    return Left.y < Right.y;
                    });
                break;

            case 2:
            case 11:
                while ((It = std::find_if(It, Vertices.end(), [&](sf::Vector2u ThisVertex) {
                    return ThisVertex.x > CurrentVertex.x && ThisVertex.y == CurrentVertex.y;
                })) != Vertices.end())
                {
                    NextVertices.push_back(*It);
                    It++;
                }

                CurrentVertex = *std::min_element(NextVertices.begin(), NextVertices.end(), [](sf::Vector2u Left, sf::Vector2u Right) {
                    return Left.x < Right.x;
                    });
                break;

            case 4:
            case 7:

                while ((It = std::find_if(It, Vertices.end(), [&](sf::Vector2u ThisVertex) {
                    return ThisVertex.x == CurrentVertex.x && ThisVertex.y < CurrentVertex.y;
                    })) != Vertices.end())
                {
                    NextVertices.push_back(*It);
                    It++;
                }

                CurrentVertex = *std::min_element(NextVertices.begin(), NextVertices.end(), [](sf::Vector2u Left, sf::Vector2u Right) {
                    return Left.y > Right.y;
                    });
                break;

            case 8:
            case 14:

                while ((It = std::find_if(It, Vertices.end(), [&](sf::Vector2u ThisVertex) {
                    return ThisVertex.x < CurrentVertex.x&& ThisVertex.y == CurrentVertex.y;
                    })) != Vertices.end())
                {
                    NextVertices.push_back(*It);
                    It++;
                }

                CurrentVertex = *std::min_element(NextVertices.begin(), NextVertices.end(), [](sf::Vector2u Left, sf::Vector2u Right) {
                    return Left.x > Right.x;
                    });
                break;
            }

            Vertices.erase(std::find(Vertices.begin(), Vertices.end(), CurrentVertex));

            if (Vertices.size() == 0)
            {
                OrderedVertices.push_back(b2Vec2(float(CurrentVertex.x) / BOX2D_SCALE_FACTOR, float(CurrentVertex.y) / BOX2D_SCALE_FACTOR));
            }
        }

        b2BodyDef ChainBodyDef;
        ChainBodyDef.position.Set(0.0f, 0.0f);
        b2Body* ChainBody = Box2dWorld.CreateBody(&ChainBodyDef);

        b2ChainShape ChainShape;
        ChainShape.CreateLoop(&OrderedVertices[0], OrderedVertices.size());

        b2FixtureDef* FixtureDef = new b2FixtureDef();
        FixtureDef->shape = &ChainShape;
        FixtureDef->filter.categoryBits = WALL;
        FixtureDef->filter.maskBits = ENTITY;
        
        ChainBody->CreateFixture(FixtureDef);
    }
}

void FPhysicsSystem::OnUpdate(entt::registry& Registry, entt::entity Entity)
{
    FTransformComponent TransformComponent = Registry.get<FTransformComponent>(Entity);
    FRigidBodyComponent RigidBodyComponent = Registry.get<FRigidBodyComponent>(Entity);
    RigidBodyComponent.Hitbox.left = TransformComponent.Position.x + RigidBodyComponent.Hitbox.left + RigidBodyComponent.Hitbox.width / 2.0f;
    RigidBodyComponent.Hitbox.top = TransformComponent.Position.y + RigidBodyComponent.Hitbox.top + RigidBodyComponent.Hitbox.height / 2.0f;

    RigidBodyComponent.Box2dBody->SetTransform(b2Vec2(RigidBodyComponent.Hitbox.left / BOX2D_SCALE_FACTOR, RigidBodyComponent.Hitbox.top / BOX2D_SCALE_FACTOR), 0.0f);
}

void FPhysicsSystem::OnDestroy(entt::registry& Registry, entt::entity Entity)
{
    FRigidBodyComponent RigidBodyComponent = Registry.get<FRigidBodyComponent>(Entity);
    BodiesToRemove.push_back(RigidBodyComponent.Box2dBody);
}

void FPhysicsSystem::OnChangeMap(entt::entity MapEntity, sf::Vector2f NewPosition)
{
    FTransformComponent& TransformComponent = FGame::registry.get<FTransformComponent>(PlayerEntity);
    FRigidBodyComponent RigidBodyComponent = FGame::registry.get<FRigidBodyComponent>(PlayerEntity);

    TransformComponent.Position = sf::Vector2f(NewPosition);

    this->MapEntity = MapEntity;

    MoveCollisionBoxes();
}

#ifdef _DEBUG
b2World* FPhysicsSystem::GetBox2dWorld()
{
    return &Box2dWorld;
}
#endif


void FPhysicsSystem::BeginContact(b2Contact* contact)
{
}

void FPhysicsSystem::EndContact(b2Contact* contact)
{
}