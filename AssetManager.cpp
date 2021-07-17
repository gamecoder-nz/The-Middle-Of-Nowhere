#include "AssetManager.hpp"

FAssetManager::FAssetManager()
{
}

void FAssetManager::LoadAssets()
{
	LoadTextures();
}

FAssetManager& FAssetManager::GetInstance()
{
	static FAssetManager AssetManager;
	return AssetManager;
}

sf::Texture& FAssetManager::GetPlayerTexture()
{
	return PlayerTexture;
}

void FAssetManager::LoadTextures()
{
	PlayerTexture.loadFromFile("Resources/Textures/PlayerSheet.png");
}
