#include "RenderSystem.hpp"
#include "Game.hpp"
#include "Components.hpp"
#include "GameSignals.hpp"
#include "GameConfiguration.hpp"
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/Map.hpp>

FRenderSystem::FRenderSystem(sf::RenderWindow* Window)
{
	this->Window = Window;
    MapEntity = entt::null;
}

void FRenderSystem::ConnectSignals()
{
    FGame::registry.on_construct<FSpriteComponent>().connect<&FRenderSystem::OnConstruct>(this);
    FGame::registry.on_construct<FPlayerComponent>().connect<&FRenderSystem::OnPlayerConstruct>(this);
    ChangeMapSignalSink.connect<&FRenderSystem::OnChangeMap>(this);
    CreateMapLayersSignalSink.connect<&FRenderSystem::OnCreateMapLayers>(this);
}

void FRenderSystem::Initialise()
{
#ifdef _DEBUG
    Box2dDraw = new FBox2dDraw(Window);
    Box2dDraw->SetFlags(b2Draw::e_shapeBit);
    GetBox2dWorldSignal.collect([&](b2World* Box2dWorld) { this->Box2dWorld = Box2dWorld; });
    Box2dWorld->SetDebugDraw(this->Box2dDraw);
#endif
}

void FRenderSystem::Update(float dt)
{
	auto View = FGame::registry.view<FSpriteComponent>();
    Window->clear();

    if (MapEntity != entt::null)
    {
        FMapComponent MapComponent = FGame::registry.get<FMapComponent>(MapEntity);

        for (sf::VertexArray MapLayer : MapComponent.MapLayers)
        {
            Window->draw(MapLayer, &MapComponent.Texture);
        }
        
    }

    for (auto Entity : View)
    {
        FSpriteComponent SpriteComponent = FGame::registry.get<FSpriteComponent>(Entity);
        FTransformComponent TransformComponent = FGame::registry.get<FTransformComponent>(Entity);
        SpriteComponent.Sprite.setPosition(sf::Vector2f(std::floor(TransformComponent.Position.x), std::floor(TransformComponent.Position.y)));
        Window->draw(SpriteComponent.Sprite);

        sf::Vector2f SpritePosition = SpriteComponent.Sprite.getPosition();
        sf::IntRect TextureRect = SpriteComponent.Sprite.getTextureRect();
        sf::VertexArray VertexArray;
        VertexArray.setPrimitiveType(sf::LineStrip);
        VertexArray.append(sf::Vertex(SpritePosition, sf::Color::White));
        VertexArray.append(sf::Vertex(SpritePosition + sf::Vector2f(TextureRect.width, 0.0f), sf::Color::White));
        VertexArray.append(sf::Vertex(SpritePosition + sf::Vector2f(TextureRect.width, TextureRect.height), sf::Color::White));
        VertexArray.append(sf::Vertex(SpritePosition + sf::Vector2f(0.0f, TextureRect.height), sf::Color::White));
        VertexArray.append(sf::Vertex(SpritePosition, sf::Color::White));
        Window->draw(VertexArray);
    }

#ifdef _DEBUG
    Box2dWorld->DebugDraw();
#endif
    Window->display();
}

void FRenderSystem::OnConstruct(entt::registry& Registry, entt::entity Entity)
{
    
}

void FRenderSystem::OnPlayerConstruct(entt::registry& Registry, entt::entity Entity)
{
    PlayerEntity = Entity;
}

void FRenderSystem::OnChangeMap(entt::entity MapEntity, sf::Vector2f NewPosition)
{
    if (FGame::registry.any_of<FMapComponent>(MapEntity) == true)
    {
        this->MapEntity = MapEntity;
    }
}

void FRenderSystem::OnCreateMapLayers(entt::entity MapEntity, tmx::Map& TmxMap)
{
    FMapComponent& MapComponent = FGame::registry.get<FMapComponent>(MapEntity);
    tmx::Tileset Tileset = TmxMap.getTilesets()[0];
    MapComponent.Texture.loadFromFile(Tileset.getImagePath());

    const std::vector<tmx::Layer::Ptr> &Layers = TmxMap.getLayers();

    for (int Index = 0; Index < Layers.size(); Index++)
    {
        if (TmxMap.getLayers()[Index]->getName().find("Ground") != std::string::npos)
        {
            tmx::TileLayer TileLayer = TmxMap.getLayers()[Index]->getLayerAs<tmx::TileLayer>();
            
            sf::VertexArray MapLayer;
            MapLayer.setPrimitiveType(sf::PrimitiveType::Quads);

            for (unsigned int Y = 0; Y < MapComponent.MapDimensions.y; Y++)
            {
                for (unsigned int X = 0; X < MapComponent.MapDimensions.x; X++)
                {
                    sf::Vector2f TilePosition(X * TILE_SIZE, Y * TILE_SIZE);
                    tmx::TileLayer::Tile Tile = TileLayer.getTiles()[Y * MapComponent.MapDimensions.x + X];
                    unsigned int TileId = Tile.ID - Tileset.getFirstGID();

                    sf::Vector2f TextureCoordinates = sf::Vector2f(TILE_SIZE * (TileId % Tileset.getColumnCount()), TILE_SIZE * std::floor(TileId / Tileset.getColumnCount()));

                    MapLayer.append(sf::Vertex(TilePosition, TextureCoordinates));
                    MapLayer.append(sf::Vertex(TilePosition + sf::Vector2f(TILE_SIZE, 0.0f), TextureCoordinates + sf::Vector2f(TILE_SIZE, 0.0f)));
                    MapLayer.append(sf::Vertex(TilePosition + sf::Vector2f(TILE_SIZE, TILE_SIZE), TextureCoordinates + sf::Vector2f(TILE_SIZE, TILE_SIZE)));
                    MapLayer.append(sf::Vertex(TilePosition + sf::Vector2f(0.0f, TILE_SIZE), TextureCoordinates + sf::Vector2f(0.0f, TILE_SIZE)));
                }
            }

            MapComponent.MapLayers.push_back(MapLayer);
        }
    }
}

#ifdef _DEBUG
FBox2dDraw::FBox2dDraw(sf::RenderWindow* Window)
{
    this->Window = Window;
}

void FBox2dDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
    
}

void FBox2dDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
    sf::VertexArray VertexArray;
    VertexArray.setPrimitiveType(sf::LineStrip);

    for (int I = 0; I < vertexCount; I++)
    {
        VertexArray.append(sf::Vertex(sf::Vector2f(vertices[I].x * BOX2D_SCALE_FACTOR, vertices[I].y * BOX2D_SCALE_FACTOR), sf::Color::White));
    }

    VertexArray.append(sf::Vertex(sf::Vector2f(vertices[0].x * BOX2D_SCALE_FACTOR, vertices[0].y * BOX2D_SCALE_FACTOR), sf::Color::White));

    Window->draw(VertexArray);
}

void FBox2dDraw::DrawCircle(const b2Vec2& center, float radius, const b2Color& color)
{
    
}

void FBox2dDraw::DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color)
{
}

void FBox2dDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
    sf::Vertex line[] =
    {
        sf::Vertex(sf::Vector2f(p1.x * BOX2D_SCALE_FACTOR, p1.y * BOX2D_SCALE_FACTOR), sf::Color::White),
        sf::Vertex(sf::Vector2f(p2.x * BOX2D_SCALE_FACTOR, p2.y * BOX2D_SCALE_FACTOR), sf::Color::White)
    };

    Window->draw(line, 2, sf::Lines);
}

void FBox2dDraw::DrawTransform(const b2Transform& xf)
{
}

void FBox2dDraw::DrawPoint(const b2Vec2& p, float size, const b2Color& color)
{
}
#endif