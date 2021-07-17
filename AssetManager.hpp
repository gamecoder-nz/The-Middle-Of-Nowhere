#ifndef ASSET_MANAGER_HPP
#define ASSET_MANAGER_HPP
#include <SFML/Graphics.hpp>

class FAssetManager
{
public:
	FAssetManager();
	void LoadAssets();

	static FAssetManager& GetInstance();

	sf::Texture& GetPlayerTexture();

private:
	void LoadTextures();

	sf::Texture PlayerTexture;
};

#endif