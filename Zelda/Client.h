#pragma once
#include <map>
#include <functional>
#include <string>
#include "ResourcesClass/Utils.h"
#include <vector>
#include <thread>
#include <mutex>
#include "UDP/UdpSocket.h"
#include "Game.h"





class Client
{
public:
	Client(std::string ip);
	~Client();
	void InitClient(std::string ip);
	void ReceiveLoop();
	void SendPacket(sf::Packet p);
	void SendCritickPacket(sf::Packet p, PacketKeys key);

	bool connectSucces;
	void StartGame(Window* window);
	void EraseCritPacket(PacketKeys key);
	ClientData* clientData;
	float startTime;

	int frameRate;
	int startMove;
	bool canDisconnect;
	Game* game;

private:
	std::map<PacketKeys, std::function<void(sf::Packet)>> packets;
	std::map<PacketKeys, sf::Packet> critickPacket;

	sf::Mutex mutex;
	sf::UdpSocket* udpSocket;
	std::string serverIp;
};

