#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Animation.h"
#include "Window.h"

class Bomb :public sf::Sprite
{
public:
	Bomb(Window* window, sf::Vector2f startPosition);
	~Bomb();
	bool checked;

	const sf::Vector2i bombSize = sf::Vector2i(16, 18);
	const sf::Vector2i bombSpriteSize = sf::Vector2i(41, 50);
	Animation* currentAnim;
	Animation* staticBomb = new Animation(this, sf::Vector2i(0, 16), bombSize, 400, 1, 1, 1, false);
	Animation* parpadeo = new Animation(this, sf::Vector2i(385, 16), bombSize, 2000, 10, 10, 1, false);

	Animation* explosion = new Animation(this, sf::Vector2i(32, 0), bombSpriteSize, 1000, 8, 8, 1, false);
private:


	sf::Texture* atlasText;
	static sf::Texture* GetSpriteAtlas();

};

