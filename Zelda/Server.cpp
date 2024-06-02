#include "Server.h"
#include <iostream>
#include <random>
#include <set>
#include <SFML/Graphics.hpp>


Server::Server()
{
    checkAFKS = 0;
    frameRate = 30;
    InitServer();
    currentClientID = 0;
    currentBombID = 0;
    auto currentTime = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration<float, std::milli>(currentTime.time_since_epoch());

    startTime = duration.count();
    startTimeSpawnBomb = duration.count();
    packets.insert({ PacketKeys::LoginServer, [this](sf::Packet packet, ClientData* player) {
     
        int posX = 0;
        int posY = 0;
        bool loop = true;
        do
        {
            loop = true;
            posX = getRandomInt(WindowWidth());
            posY = getRandomInt(WindowHeight());

            for (auto it = clients.begin(); it != clients.end(); it++) {

                if (it->first != player->clientID)
                {
                    if (CheckBounds(posX, posY, it->second->posX, it->second->posY,2))
                    {
                        loop = false;
                    }
                }
  
            }

        } while (!loop);

        clients[player->clientID]->posX = posX;
        clients[player->clientID]->posY = posY;

        sf::Packet p;
        p << static_cast<int>(PacketKeys::LoginConfirmClient) << *clients[player->clientID];

        SendPacket(clients[player->clientID], p);
    } 
    });

    packets.insert({ PacketKeys::UpdateClient, [this](sf::Packet packet, ClientData* player) {

        mutex2.lock();
        if (clients.find(player->clientID) == clients.end())
        {

            sf::Packet p;
            p << static_cast<int>(PacketKeys::DisconnectClient);
            SendPacket(player, p);
        }
        else
        {

            if (clients[player->clientID]->action == ATTACK)
            {
                for (auto it = clients.begin(); it != clients.end(); it++) {

                    if (it->first != player->clientID)
                    {
                        switch (clients[player->clientID]->orientation)
                        {
                        case UP:
                            if (CheckBounds(clients[player->clientID]->posX, clients[player->clientID]->posY - 15, it->second->posX, it->second->posY, 50))
                            {
                                it->second->lives--;

                                if (it->second->lives <= 0)
                                {
                                    sf::Packet p;
                                    p << static_cast<int>(PacketKeys::DisconnectClient);
                                    SendPacket(it->second, p);
                                    eraseValues.push_back(it->first);

                                }
                                else
                                {
                                    it->second->posY -= 35;

                                    if (it->second->posY <= 30)
                                        it->second->posY = 30;

                                }


                            }

                            break;
                        case DOWN:

                            if (CheckBounds(clients[player->clientID]->posX, clients[player->clientID]->posY + 15, it->second->posX, it->second->posY, 50))
                            {
                                it->second->lives--;

                                if (it->second->lives <= 0)
                                {
                                    sf::Packet p;
                                    p << static_cast<int>(PacketKeys::DisconnectClient);
                                    SendPacket(it->second, p);
                                    eraseValues.push_back(it->first);
                                }
                                else
                                {
                                    it->second->posY += 35;

                                    if (it->second->posY >= WindowHeight() - 30)
                                        it->second->posY = WindowHeight() - 30;

                                }
                            }
                            break;
                        case RIGHT:
                            if (CheckBounds(clients[player->clientID]->posX + 15, clients[player->clientID]->posY, it->second->posX, it->second->posY, 50))
                            {
                                it->second->lives--;

                                if (it->second->lives <= 0)
                                {
                                    sf::Packet p;
                                    p << static_cast<int>(PacketKeys::DisconnectClient);
                                    SendPacket(it->second, p);
                                    eraseValues.push_back(it->first);
                                }
                                else
                                {
                                    it->second->posX += 35;

                                    if (it->second->posX >= WindowWidth() - 30)
                                        it->second->posX = WindowWidth() - 30;

                                }
                            }

                            break;
                        case LEFT:

                            if (CheckBounds(clients[player->clientID]->posX - 15, clients[player->clientID]->posY, it->second->posX, it->second->posY, 50))
                            {
                                it->second->lives--;

                                if (it->second->lives <= 0)
                                {
                                    sf::Packet p;
                                    p << static_cast<int>(PacketKeys::DisconnectClient);
                                    SendPacket(it->second, p);
                                    eraseValues.push_back(it->first);
                                }
                                else
                                {
                                    it->second->posX -= 35;

                                    if (it->second->posX <= 30)
                                        it->second->posX = 30;

                                }
                            }

                            break;
                        default:
                            break;
                        }
                    }




                }


            }
            else  if (clients[player->clientID]->action == GRAB )
            {

                        switch (clients[player->clientID]->orientation)
                        {
                        case UP:
                            if (!clients[player->clientID]->hasBomb)
                            {

                                for (auto it2 = bombs.begin(); it2 != bombs.end(); it2++) {
                                    if (CheckBounds(clients[player->clientID]->posX, clients[player->clientID]->posY - 15, it2->second->posX, it2->second->posY, 50) && it2->second->action == IDLEBOMB)
                                    {
                                        clients[player->clientID]->hasBomb = true;
                                        clients[player->clientID]->bombID = it2->first;

                                        it2->second->clientID = player->clientID;
                                        it2 = bombs.end();
                                        it2--;
                                    }
                                }

                                if (!clients[player->clientID]->hasBomb)
                                {
                                    clients[player->clientID]->action == IDLE;
                                }
                            }
                            else
                            {
                                clients[player->clientID]->hasBomb = false;



                                bombs[clients[player->clientID]->bombID]->clientID = -1;
                                bombs[clients[player->clientID]->bombID]->action = STARTEXPLOSION;
                                bombs[clients[player->clientID]->bombID]->posX = clients[player->clientID]->posX+5;
                                bombs[clients[player->clientID]->bombID]->posY = clients[player->clientID]->posY-25;


                                auto currentTime = std::chrono::high_resolution_clock::now();

                                auto duration = std::chrono::duration<float, std::milli>(currentTime.time_since_epoch());

                                float currentTimeMillis = duration.count();
                                bombs[clients[player->clientID]->bombID]->timeToExplote = currentTimeMillis;

                                clients[player->clientID]->bombID = -1;
                            }


                            break;
                        case DOWN:

                            if (!clients[player->clientID]->hasBomb)
                            {

                                for (auto it2 = bombs.begin(); it2 != bombs.end(); it2++) {
                                    if (CheckBounds(clients[player->clientID]->posX, clients[player->clientID]->posY + 15, it2->second->posX, it2->second->posY, 50) && it2->second->action == IDLEBOMB)
                                    {
                                        clients[player->clientID]->hasBomb = true;
                                        clients[player->clientID]->bombID = it2->first;

                                        it2->second->clientID = player->clientID;
                                        it2 = bombs.end();
                                        it2--;
                                    }
                                }

                                if (!clients[player->clientID]->hasBomb)
                                {
                                    clients[player->clientID]->action == IDLE;

                                }
                            }
                            else
                            {
                                clients[player->clientID]->hasBomb = false;



                                bombs[clients[player->clientID]->bombID]->clientID = -1;
                                bombs[clients[player->clientID]->bombID]->action = STARTEXPLOSION;
                                bombs[clients[player->clientID]->bombID]->posX = clients[player->clientID]->posX+5;
                                bombs[clients[player->clientID]->bombID]->posY = clients[player->clientID]->posY+20;


                                auto currentTime = std::chrono::high_resolution_clock::now();

                                auto duration = std::chrono::duration<float, std::milli>(currentTime.time_since_epoch());

                                float currentTimeMillis = duration.count();
                                bombs[clients[player->clientID]->bombID]->timeToExplote = currentTimeMillis;

                                clients[player->clientID]->bombID = -1;
                            }
                            break;
                        case RIGHT:
                            if (!clients[player->clientID]->hasBomb)
                            {

                                for (auto it2 = bombs.begin(); it2 != bombs.end(); it2++) {
                                    if (CheckBounds(clients[player->clientID]->posX+15, clients[player->clientID]->posY, it2->second->posX, it2->second->posY, 50) && it2->second->action == IDLEBOMB)
                                    {
                                        clients[player->clientID]->hasBomb = true;
                                        clients[player->clientID]->bombID = it2->first;

                                        it2->second->clientID = player->clientID;
                                        it2 = bombs.end();
                                        it2--;
                                    }
                                }
                                if (!clients[player->clientID]->hasBomb)
                                {
                                    clients[player->clientID]->action == IDLE;

                                }
                            }
                            else
                            {
                                clients[player->clientID]->hasBomb = false;



                                bombs[clients[player->clientID]->bombID]->clientID = -1;
                                bombs[clients[player->clientID]->bombID]->action = STARTEXPLOSION;
                                bombs[clients[player->clientID]->bombID]->posX = clients[player->clientID]->posX+30;
                                bombs[clients[player->clientID]->bombID]->posY = clients[player->clientID]->posY+10;


                                auto currentTime = std::chrono::high_resolution_clock::now();

                                auto duration = std::chrono::duration<float, std::milli>(currentTime.time_since_epoch());

                                float currentTimeMillis = duration.count();
                                bombs[clients[player->clientID]->bombID]->timeToExplote = currentTimeMillis;

                                clients[player->clientID]->bombID = -1;
                            }

                            break;
                        case LEFT:
                            if (!clients[player->clientID]->hasBomb)
                            {

                                for (auto it2 = bombs.begin(); it2 != bombs.end(); it2++) {
                                    if (CheckBounds(clients[player->clientID]->posX-15, clients[player->clientID]->posY, it2->second->posX, it2->second->posY, 50) && it2->second->action == IDLEBOMB)
                                    {
                                        clients[player->clientID]->hasBomb = true;
                                        clients[player->clientID]->bombID = it2->first;

                                        it2->second->clientID = player->clientID;
                                        it2 = bombs.end();
                                        it2--;
                                    }
                                }

                                if (!clients[player->clientID]->hasBomb)
                                {
                                    clients[player->clientID]->action == IDLE;

                                }
                            }
                            else
                            {
                                clients[player->clientID]->hasBomb = false;



                                bombs[clients[player->clientID]->bombID]->clientID = -1;
                                bombs[clients[player->clientID]->bombID]->action = STARTEXPLOSION;

                                bombs[clients[player->clientID]->bombID]->posX = clients[player->clientID]->posX-20;
                                bombs[clients[player->clientID]->bombID]->posY = clients[player->clientID]->posY+10;


                                auto currentTime = std::chrono::high_resolution_clock::now();

                                auto duration = std::chrono::duration<float, std::milli>(currentTime.time_since_epoch());

                                float currentTimeMillis = duration.count();
                                bombs[clients[player->clientID]->bombID]->timeToExplote = currentTimeMillis;

                                clients[player->clientID]->bombID = -1;
                            }

                            break;
                        default:
                            break;
                        }
                    






                }
            clients[player->clientID]->checkAFK = 0;





            //std::cout << "Update client" << std::endl;
        }
        mutex2.unlock();





    }
    });

    packets.insert({ PacketKeys::DisconnectClientToServer, [this](sf::Packet packet, ClientData* player) {

    mutex2.lock();


    sf::Packet packet;

    packet << static_cast<int>(PacketKeys::DisconnectConfirmation);

    SendPacket(player, packet);


    if (clients.find(player->clientID) != clients.end())
    {
        eraseValues.push_back(player->clientID);
    }


    mutex2.unlock();

    }
    });
}

Server::~Server()
{

}

void Server::InitServer()
{
    unsigned short a = PORT;
    udpSocket = new sf::UdpSocket();


    if (udpSocket->bind(a) != sf::Socket::Done) { //Puerto on escoltar
        std::cout << "Error listening on port 5000" << std::endl;
        return;
    }
    std::cout << "Server is listening on port 5000 and ip " << sf::IpAddress::getLocalAddress() << std::endl;

    std::thread* loop = new std::thread(&Server::ReceiveLoop, this); //Thread al bucle per rebre paquetes
    loop->detach();


}

void Server::ReceiveLoop()
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

            if (key == PacketKeys::LoginServer)
            {
                std::string name;
                *p >> name;

                bool exist = false;
                ClientData* data = new ClientData();
                int id = 0;
                for (auto it2 = clients.begin(); it2 != clients.end(); it2++) {

                    if (it2->second->ip == ipAdr->toString() && it2->second->port == *port)
                    {
                        exist = true;
                        id = it2->second->clientID;
                    }
                }


                if (!exist)
                {
                        data->ip = ipAdr->toString();
                        data->port = *port;
                        data->name = name;
                        data->clientID = currentClientID;

                        data->checkAFK = 0;
                        data->posX = getRandomInt(WindowWidth());
                        data->posY = getRandomInt(WindowHeight());
                        clients[currentClientID] = data;
                        currentClientID++;

                }
                else
                {
                    data = clients[id];
                }


                auto it = packets.find(key);
                if (it != packets.end()) {
                    it->second(*p, data);
                }

            }
            else
            {
                ClientData* data = new ClientData();
                *p >> *data;
                if (clients.find(data->clientID) != clients.end())
                {


                clients[data->clientID] = data;

                auto currentTime = std::chrono::high_resolution_clock::now();

                auto duration = std::chrono::duration<float, std::milli>(currentTime.time_since_epoch());

                float currentTimeMillis = duration.count();

                clients[data->clientID]->actionStartTime = currentTimeMillis;


                auto it = packets.find(key);
                if (it != packets.end()) {
                    it->second(*p, data);
                }
                }

            }

        }
    }
}

void Server::UpdateAllClients()
{
    for (auto it2 = clients.begin(); it2 != clients.end(); it2++) {

        if (it2->second->action == MOVE)
        {
            switch (it2->second->orientation)
            {
            case UP:
                it2->second->posY -= SPEED;
                if (it2->second->posY < 30)
                    it2->second->posY = 30;
                break;
            case DOWN:
                it2->second->posY += SPEED;
                if (it2->second->posY > WindowHeight() - 30)
                    it2->second->posY = WindowHeight() - 30;
                break;
            case LEFT:
                it2->second->posX -= SPEED;
                if(it2->second->posX < 30)
                    it2->second->posX = 30;

                break;
            case RIGHT:
                it2->second->posX += SPEED;
                if (it2->second->posX > WindowWidth() - 30)
                    it2->second->posX = WindowWidth() - 30;
                break;
            default:
                break;
            }

        }
        

    }

}

void Server::GameLoop()
{
    while (true) {
        auto currentTime = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration<float, std::milli>(currentTime.time_since_epoch());

        float currentTimeMillis = duration.count();

        if (currentTimeMillis-startTime > ((float)1000 / (float)frameRate))
        {
            UpdateAllClients();
            checkAFKS++;
            startTime = currentTimeMillis;

            if (currentTimeMillis - startTimeSpawnBomb > 10000)
            {
                startTimeSpawnBomb = currentTimeMillis;

                BombData* newBomb = new BombData();
                newBomb->bombID = currentBombID;

                newBomb->posX = getRandomInt(WindowWidth());
                newBomb->posY = getRandomInt(WindowHeight());

                bombs[currentBombID] = newBomb;

                currentBombID++;
            }


            std::vector<int> eraseBomb;
            for (auto it = bombs.begin(); it != bombs.end(); it++) {
                if ((currentTimeMillis - it->second->timeToExplote) >= 3000 && (it->second->action == STARTEXPLOSION))
                {
                    it->second->action = EXPLOTE;
                    it->second->posX -= 15;
                    it->second->posY -= 20;

                }

                if ((currentTimeMillis - it->second->timeToExplote) >= 3500 && (it->second->action == EXPLOTE))
                {

                    for (auto it2 = clients.begin(); it2 != clients.end(); it2++) {

                        if (CheckBounds(it->second->posX, it->second->posY, it2->second->posX, it2->second->posY, 50))
                        {
                            it2->second->lives -= 3;

                            if (it2->second->lives <= 0)
                            {
                                sf::Packet p;
                                p << static_cast<int>(PacketKeys::DisconnectClient);
                                SendPacket(it2->second, p);
                                eraseValues.push_back(it2->first);
                            }
   
                        }
                    }
                    eraseBomb.push_back(it->first);

                }
            }



            for (int i = 0; i < eraseBomb.size(); i++)
            {
                if (bombs.find(eraseBomb[i]) != bombs.end())
                    bombs.erase(eraseBomb[i]);

            }

            for (auto it = clients.begin(); it != clients.end(); it++) {

                if ((currentTimeMillis - it->second->actionStartTime) >= 1000 && (it->second->action != MOVE && it->second->action != IDLE))
                {
                    it->second->action = IDLE;
                }


                sf::Packet packet;

                CPVector<ClientData> allData;
                for (auto it2 = clients.begin(); it2 != clients.end(); it2++) {
                    allData.push_back(it2->second);

                }
   
                packet << static_cast<int>(PacketKeys::UpdateRoom) << allData;

                sf::Packet packet2;

                CPVector<BombData> allBombs;

                for (auto it3 = bombs.begin(); it3 != bombs.end(); it3++) {
                    allBombs.push_back(it3->second);

                }
                packet2 << static_cast<int>(PacketKeys::UpdateBombs) << allBombs;


                for (auto it2 = clients.begin(); it2 != clients.end(); it2++) {

                    SendPacket(it2->second, packet);
                    SendPacket(it2->second, packet2);

                }




            }


            //2 veces por segundo le suma a cada cliente +1 a checkAFK
            //Si el cliente no envia un PacketKeys::UpdateClient checkAFK sigue sumando, si si que lo envia se reinicia
            //Si  checkAFK llega a 30 == 15 segundos sin enviar info, lo desconecta

            if (checkAFKS % (frameRate / 2) == 0)
            {
                checkAFKS = 0;
                for (auto it2 = clients.begin(); it2 != clients.end(); it2++) {

                    if (it2->second->checkAFK >= 30)
                    {
                        std::cout << "AFK" << std::endl;

                        sf::Packet p;
                        p << static_cast<int>(PacketKeys::DisconnectClient);
                        SendPacket(it2->second, p);
                        eraseValues.push_back(it2->first);

                    }
                    else
                    {
                        it2->second->checkAFK++;

                    }

                }

      
            }
            for (int i = 0; i < eraseValues.size(); i++)
            {
                if (clients.find(eraseValues[i]) != clients.end())
                {
                    if (clients.find(eraseValues[i])->second->hasBomb)
                    {
                        if (bombs.find(clients.find(eraseValues[i])->second->bombID) != bombs.end())
                        {
                            bombs.find(clients.find(eraseValues[i])->second->bombID)->second->action = IDLEBOMB;
                            bombs.find(clients.find(eraseValues[i])->second->bombID)->second->clientID = -1;
                            bombs.find(clients.find(eraseValues[i])->second->bombID)->second->posX = clients.find(eraseValues[i])->second->posX;

                            bombs.find(clients.find(eraseValues[i])->second->bombID)->second->posY = clients.find(eraseValues[i])->second->posY+25;

                        }
                    }



                    clients.erase(eraseValues[i]);


                }

            }
            eraseValues.clear();
        }


    }
}

void Server::SendPacket(ClientData* data, sf::Packet packet)
{
    mutex.lock();
    udpSocket->send(packet, data->ip, data->port);
    mutex.unlock();
}
