#pragma once

#include <iostream>
#include <SFML/Network.hpp>
#include <thread>
#include "ResourcesClass/Player.h"
#include "ResourcesClass/Window.h"

class Client;

class Game
{
public:
    Game(Window* window, Client* client);

    ~Game();


    void Run();
    void DeletePlayer(int id);
    void DeleteBomb(int id);

    void AddPlayer(Client* client, ClientData* data, bool player);
    std::map<int, Player*> players;
    std::map<int, Bomb*> bombs;
    void AddBomb(BombData* data);
    void UpdateBomb(BombData* data);
    void EndGame();
private:
    Window* _window;
    bool end;
    int checkConnection;
    int chechActivity;
    Client* client;
};