#pragma once
#include <map>
#include <functional>
#include <string>
#include "ResourcesClass/Utils.h"
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>





class Server
{
public:
	Server();
	~Server();
	void InitServer();

	void ReceiveLoop();

	void UpdateAllClients();
	void GameLoop();
	void SendPacket(ClientData* data, sf::Packet packet);


private:
	sf::UdpSocket* udpSocket; 
	std::map<PacketKeys, std::function<void(sf::Packet, ClientData*)>> packets;
	std::map<int, ClientData*> clients;
	std::map<int, BombData*> bombs;

	std::mutex mutex;
	std::mutex mutex2;
	std::vector<int> eraseValues;
	int currentClientID;
	int currentBombID;

	float startTime;

	float startTimeSpawnBomb;

	int checkAFKS;
	int frameRate;
};

