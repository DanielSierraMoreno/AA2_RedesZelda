#include <iostream>
#include <SFML/Network.hpp>
#include <thread>
#include "ResourcesClass/Player.h"
#include "ResourcesClass/Window.h"
#include "Server.h"
#include "Client.h"



void RunWindows(Window* window)
{
    window->RunWindowsLoop(); // Run the window loop
}

int main()
{
    char mode;
    std::string ip;

    do
    {
        std::cout << "Enter mode (S for Server, C for Client): ";
        mode = ConsoleControl::WaitForReadNextChar();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 

    } while (mode != 's' && mode != 'S' && mode != 'C' && mode != 'c');


    if (mode == 'S' || mode == 's') {
        ConsoleControl::Clear();

        Server* server = new Server();


        server->GameLoop();
    }
    else {
        ConsoleControl::Clear();

        Client* client = new Client(ip);

        do
        {        
            std::cout << "Enter server ip:\n";

            std::cin >> ip;
            ConsoleControl::Clear();

            client->InitClient(ip);
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));

            client->EraseCritPacket(PacketKeys::LoginConfirmClient);

            ConsoleControl::Clear();


        } while (!client->connectSucces);


        Window* _window = new Window(WindowWidth(), WindowHeight(), "GAME", client);
        //_window->Init();

        std::thread* _windowsThread = new std::thread(&Window::Init, _window);
        _windowsThread->detach();


        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        client->StartGame(_window);
      
        while (!client->canDisconnect)
        {

        }
            std::cout << "Disconnected\n";




        delete _window;
    }
   



    while (true)
    {

    }


   


    return 0;
}

