#pragma once
#include <SFML/Graphics/Sprite.hpp>

#include "LinkCharacter.h"
#include "ConsoleControl.h"
#include "Window.h"
#include "Utils.h"

#define POSITION_INCREMENT 5

class Client;

class Player
{
public:
	Player(Window* window, bool player, Client* client, ClientData* data);
	~Player();

	void Update();

	void UpdateValues(ClientData* data);
	LinkCharacter* playerLink;

	void MoveUp();
	void MoveDown();
	void MoveLeft();
	void MoveRight();
	void Attack();
	void Grab();
	void setPos(float x, float y);
	void Stop();
	bool checked;
	ClientData* lastFrameData;
	ClientData* data;
	sf::Text* name;
	sf::Sprite* sword;
	bool CheckUpdate;

private:
	Client* client;

	Orientation _orientation;
	Window* _window;
	Animation* _currentAnim;
	sf::Vector2f _position;
	bool IsPlayer;

};