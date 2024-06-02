#pragma once
#include "../ICodable.h"
#include "Bomb.h"
#include <random>


enum class PacketKeys { LoginServer, UpdateRoom, LoginConfirmClient, UpdateClient, DisconnectClient, DisconnectClientToServer, DisconnectConfirmation, UpdateBombs};
enum Orientation { UP = 0, DOWN = 1, LEFT = 2, RIGHT = 3 };
enum Action { IDLE = 0, MOVE = 1, ATTACK = 2, GRAB = 3 };
enum ActionBomb { IDLEBOMB = 0, EXPLOTE = 1, STARTEXPLOSION = 2, RELEASE = 3 };

#define PORT 500;


#define KNOCKBACK 10;
#define WINDOWWIDTH 800;
#define WINDOWHEIGHT 600;
#define SPEED 3;

bool CheckBounds(int firstX, int firstY, int secondX, int secondY, float distance);



unsigned int WindowWidth();
unsigned int WindowHeight();
int getRandomInt(int X);

class BombData : public ICodable {
public:
	BombData() = default;

	int clientID = -1;
	int bombID;
	float timeToExplote;
	ActionBomb action = IDLEBOMB;

	float posX, posY;

private:


	//bool explosionStarted;
	// Heredado vía ICodable
	void Code(sf::Packet& packet) override {
		packet << bombID << clientID << posX << posY << (int)action << timeToExplote;

	}
	void Decode(sf::Packet& packet) override {
		int act;
		packet >> bombID >> clientID >> posX >> posY >> act >> timeToExplote;
		action = static_cast<ActionBomb>(act);
	}
};

class ClientData : public ICodable {
public:
	ClientData() = default;

	int checkAFK = 0;

	float actionStartTime;
	std::string ip;
	unsigned short port;
	std::string name;
	int clientID;
	int bombID;

	float posX, posY;


	int lives = 3;
	Action action;
	Orientation orientation;
	bool hasBomb = false;

	bool IsMoving = false;

private:



	void Code(sf::Packet& packet) override {
		
		packet << clientID << ip << port << name << posX << posY << lives << (int)orientation << hasBomb << IsMoving << (int)action << checkAFK << bombID;


	}
	void Decode(sf::Packet& packet) override {
		int orient, act;
		packet >> clientID >> ip >> port >> name >> posX >> posY >> lives >> orient >> hasBomb >> IsMoving >> act >> checkAFK >> bombID;
		orientation = static_cast<Orientation>(orient);
		action = static_cast<Action>(act);


	}
};

