#include <iostream>
#include <SFML/Network.hpp>
#include <thread>
#include "ResourcesClass/Player.h"
#include "ResourcesClass/Window.h"


class Game
{
public:
    Game(Window* window)
    {
        _window = window;
        _player = new Player(window);
    }
    ~Game()
    {
        delete _player;
    }

    void Run()
    {
        while (_window->IsOpen()) // Continue loop while window is open
        {
            _player->Update();
        }
    }


private:
    Window* _window;
    Player* _player;
};