#pragma once
#include <SFML/Graphics/Sprite.hpp>

#include "LinkCharacter.h"
#include "ConsoleControl.h"
#include "Window.h"

#define POSITION_INCREMENT 5
enum Orientation { UP = 0, DOWN = 1, LEFT = 2, RIGHT = 3 };

class Player
{
public:
	Player(Window* window);
	~Player();

	void Update();

	LinkCharacter* playerLink;

	void MoveUp();
	void MoveDown();
	void MoveLeft();
	void MoveRight();

private:

	Orientation _orientation;
	Window* _window;
	Animation* _currentAnim;
	sf::Vector2f _position;
};