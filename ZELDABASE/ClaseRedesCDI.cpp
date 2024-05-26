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


void RunWindows(Window* window)
{
    window->RunWindowsLoop(); // Run the window loop
}

int main()
{
    Window* _window = new Window();
    //_window->Init();

    std::thread* _windowsThread = new std::thread(&Window::Init, _window);
    _windowsThread->detach();


    Game* _game = new Game(_window);
    _game->Run(); // Run game loop in main thread

    delete _game;
    delete _window;
    return 0;
}

