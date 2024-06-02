#include "Game.h"
#include "Client.h"

Game::Game(Window* window, Client* client)
{
    _window = window;
    this->client = client;
    end = false;
}

Game::~Game()
{
}

void Game::Run()
{
    while (_window->IsOpen()|| !end)
    {
        auto currentTime = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration<float, std::milli>(currentTime.time_since_epoch());

        float currentTimeMillis = duration.count();

        if (currentTimeMillis - client->startTime > ((float)1000 / (float)client->frameRate))
        {
            client->startTime = currentTimeMillis;
            client->startMove++;

            for (auto it = players.begin(); it != players.end(); ++it)
            {

                Player* playerPtr = it->second;

                playerPtr->Update();


            }

        }

    }
    sf::Packet p;
    p << static_cast<int>(PacketKeys::DisconnectClientToServer) << *client->clientData;



    client->SendCritickPacket(p, PacketKeys::DisconnectConfirmation);


}

void Game::DeletePlayer(int id)
{

    _window->EraseDrawable(players[id]->playerLink);
    _window->EraseDrawable(players[id]->name);
    _window->EraseDrawable(players[id]->sword);



    players.erase(id);
}

void Game::DeleteBomb(int id)
{
    _window->EraseDrawable(bombs[id]);


    bombs.erase(id);

}

void Game::AddPlayer(Client * client,ClientData* data, bool player)
{
    Player* _player = new Player(_window, player, client, data);
    _player->setPos(data->posX, data->posY);
    players[data->clientID] = _player;
}

void Game::AddBomb(BombData* data)
{
    Bomb* bomb = new Bomb(_window,sf::Vector2f((float)data->posX, (float)data->posY));


    bombs[data->bombID] = bomb;
}

void Game::UpdateBomb(BombData* data)
{
    if (bombs.find(data->bombID) != bombs.end())
    {

        if (data->clientID != -1)
        {
            if (players.find(data->clientID) != players.end())
            {
                bombs.find(data->bombID)->second->setPosition(sf::Vector2f((float)players.find(data->clientID)->second->data->posX+5, (float)players.find(data->clientID)->second->data->posY-10));

            }

        }
        else
        {
            bombs.find(data->bombID)->second->setPosition(data->posX, data->posY);

            if (bombs.find(data->bombID)->second->currentAnim == bombs.find(data->bombID)->second->staticBomb && data->action == EXPLOTE)
            {

                bombs.find(data->bombID)->second->currentAnim = bombs.find(data->bombID)->second->explosion;
                bombs.find(data->bombID)->second->currentAnim->PlayOnce([&]() {



                    });
            }





        }



    }
}

void Game::EndGame()
{
    _window->CloseWindow();
    end = true;

}
