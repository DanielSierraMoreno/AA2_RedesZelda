#include "Client.h"
#include <iostream>
#include <string>
#include <limits>
#include <random>





Client::Client(std::string ip)
{
	canDisconnect = false;
	frameRate = 100;
	startMove = 0;
	connectSucces = false;
	clientData = new ClientData();


	udpSocket = new sf::UdpSocket();

	std::thread* loop = new std::thread(&Client::ReceiveLoop, this);
	loop->detach();



	packets.insert({ PacketKeys::DisconnectClient, [this](sf::Packet packet) {

		mutex.lock();
		std::cout << "Disconnect";

		game->EndGame();

		mutex.unlock();

	}
	});
	packets.insert({ PacketKeys::DisconnectConfirmation, [this](sf::Packet packet) {
				mutex.lock();

		canDisconnect = true;
		EraseCritPacket(PacketKeys::DisconnectConfirmation);
		mutex.unlock();

	}
	});

	packets.insert({ PacketKeys::LoginConfirmClient, [this](sf::Packet packet) {

		packet >> *clientData;

		EraseCritPacket(PacketKeys::LoginConfirmClient);
		connectSucces = true;
		std::cout << "Client confirmed";
		auto currentTime = std::chrono::high_resolution_clock::now();

		auto duration = std::chrono::duration<float, std::milli>(currentTime.time_since_epoch());

		float currentTimeMillis = duration.count();

		startTime = currentTimeMillis;

	}
	});
	packets.insert({ PacketKeys::UpdateBombs, [this](sf::Packet packet) {

		CPVector<BombData> allBombs;

		packet >> allBombs;
		mutex.lock();
		if (game != nullptr)
		{
			for (auto it = game->bombs.begin(); it != game->bombs.end(); it++) {

				it->second->checked = false;
			}

			for (int i = 0; i < allBombs.size(); i++) {

				if (!game->bombs.empty())
				{
					if (game->bombs.find(allBombs[i]->bombID) != game->bombs.end())
					{

						game->UpdateBomb(allBombs[i]);
						game->bombs[allBombs[i]->bombID]->checked = true;

					}
					else
					{

						game->AddBomb(allBombs[i]);
						game->bombs[allBombs[i]->bombID]->checked = true;

					}
				}
				else
				{
					game->AddBomb(allBombs[i]);
					game->bombs[allBombs[i]->bombID]->checked = true;

				}


			}

			std::vector<int> toDelete;
			for (auto it = game->bombs.begin(); it != game->bombs.end(); it++) {

				if (!it->second->checked)
				{
					toDelete.push_back(it->first);
				}

			}

			for (int i = 0; i < toDelete.size(); i++)
			{
				game->DeleteBomb(toDelete[i]);
			}


		}
		mutex.unlock();

	}
	});


	packets.insert({ PacketKeys::UpdateRoom, [this](sf::Packet packet) {

		CPVector<ClientData> allData;

		packet >> allData;

		mutex.lock();
		if (game != nullptr)
		{
			for (auto it = game->players.begin(); it != game->players.end(); it++) {

				it->second->checked = false;
			}
			for (int i = 0; i < allData.size(); i++) {

				if (game->players.size() >= 1)
				{
					if (game->players.find(allData[i]->clientID) != game->players.end())
					{
						//std::cout << "Update";
						game->players.find(allData[i]->clientID)->second->lastFrameData = game->players.find(allData[i]->clientID)->second->data;
						if(clientData->clientID == allData[i]->clientID)
							clientData = allData[i];

						game->players.find(allData[i]->clientID)->second->UpdateValues(allData[i]);
						game->players.find(allData[i]->clientID)->second->checked = true;
					}
					else
					{
						std::cout << "Add enemy";

						game->AddPlayer(this,allData[i], false);
						game->players.find(allData[i]->clientID)->second->checked = true;

					}
				}
			}
			std::vector<int> toDelete;
			for (auto it = game->players.begin(); it != game->players.end(); it++) {

				if (!it->second->checked)
				{
					toDelete.push_back(it->first);
				}

			}

			for (int i = 0; i < toDelete.size(); i++)
			{
					game->DeletePlayer(toDelete[i]);

			}





		}
		
		mutex.unlock();



	}
	});


}

Client::~Client()
{
}

void Client::InitClient(std::string ip)
{
	serverIp = ip;
	std::string name;

	std::cout << "Enter name:\n";

	std::cin >> name;


	sf::Packet p;
	p << static_cast<int>(PacketKeys::LoginServer) << name;



	SendCritickPacket(p, PacketKeys::LoginConfirmClient);



}

void Client::ReceiveLoop()
{
	sf::Packet* p = new sf::Packet();
	sf::IpAddress* ipAdr = new sf::IpAddress();
	unsigned short* port = new unsigned short;

	while (true) {

		if (udpSocket->receive(*p, *ipAdr, *port) == sf::Socket::Done)
		{
			int keyInt; 
			*p >> keyInt;
			PacketKeys key = static_cast<PacketKeys>(keyInt);

			auto it = packets.find(key);
			if (it != packets.end()) {
				it->second(*p);
			}
		}

		for (auto it = critickPacket.begin(); it != critickPacket.end(); it++) {

			SendPacket(it->second);
			std::cout << "Resend crit paquet\n";

		}
	}
}

void Client::SendPacket(sf::Packet p)
{
	unsigned short a = PORT;

	if (udpSocket->send(p, serverIp, a) != sf::Socket::Done)
	{
		std::cout << "Error to send packet\n";


	}
	else
	{
		//std::cout << "Packet send\n";

	}
}

void Client::SendCritickPacket(sf::Packet p, PacketKeys key)
{
	unsigned short a = PORT;

	if (udpSocket->send(p, serverIp, a) != sf::Socket::Done)
	{
		std::cout << "Error to send packet\n";
		connectSucces = false;

	}
	else
	{
		//std::cout << "Packet send\n";

		critickPacket[key] = p;


	}

}

void Client::StartGame(Window* window)
{
	game = new Game(window, this);

	game->AddPlayer(this,clientData, true);
	game->Run();


}

void Client::EraseCritPacket(PacketKeys key)
{
	if (critickPacket.find(key) != critickPacket.end())
	{
		critickPacket.erase(key);
	}
}
