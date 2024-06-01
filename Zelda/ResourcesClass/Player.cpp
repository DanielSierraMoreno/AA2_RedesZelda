#include "Player.h"
#include "..//Client.h"

Player::Player(Window* window, bool player, Client* client, ClientData* data) //: GameObject()
{
	_window = window;
	lastFrameData = data;



	sword = new sf::Sprite;
	sf::Texture* spriteAtlas = nullptr;

	if (spriteAtlas == nullptr)
	{
		spriteAtlas = new sf::Texture();
		if (!spriteAtlas->loadFromFile("Resources/Link.png"))
		{
			ConsoleControl::LockMutex();
			std::cout << "Error al cargar el atlas de link";
			ConsoleControl::UnlockMutex();
		}
		else
		{
			sword->setTexture(*spriteAtlas);
			sword->setTextureRect(sf::IntRect(sf::Vector2i(32, 32*9), sf::Vector2i(32, 32)));
			sword->setPosition(-15,-15);
			_window->AddTask([&](Window& w) {
				w.AddDrawable(sword);
				});
		}
	}


	sf::Font* font = new sf::Font();
	if (!font->loadFromFile("Resources/Minecraft.ttf")) {
		// Maneja el error
		std::cout << "Error al cargar la fuente" << std::endl;

	}
	else
	{
		name = new sf::Text();
		name->setFont(*font); 
		name->setString(data->name);
		name->setCharacterSize(15); 
		name->setFillColor(sf::Color::White); 

		_window->AddTask([&](Window& w) {
			w.AddDrawable(name);
			});


	}
	

	this->data = data;
	this->client = client;
	std::cout << "Create player" << std::endl;
	playerLink = new LinkCharacter({ 15, 15 }, "Player");
	_position = { (float)client->clientData->posX, (float)client->clientData->posY };
	_orientation = DOWN;
	_currentAnim = new Animation();
	_currentAnim = playerLink->idle;

	_currentAnim->PlayOnce([&]() {
		_currentAnim->Stop();
		_currentAnim = nullptr;
		});

	IsPlayer = player;
	_window->AddTask([&](Window& w) {
		w.AddDrawable(playerLink);
		});

	if (!player)
	{
		playerLink->setColor(sf::Color(155,0,0));
		sword->setColor(sf::Color(155, 0, 0));

		return;
	}
	_window->SubscribeKeyPressed(sf::Keyboard::W, [&]() {
		MoveUp();
		});

	_window->SubscribeKeyPressed(sf::Keyboard::S, [&]() {
		MoveDown();
		});

	_window->SubscribeKeyPressed(sf::Keyboard::A, [&]() {
		MoveLeft();
		});

	_window->SubscribeKeyPressed(sf::Keyboard::D, [&]() {
		MoveRight();
		});
	_window->SubscribeKeyPressed(sf::Keyboard::Space, [&]() {
		Attack();
		});
	_window->SubscribeKeyPressed(sf::Keyboard::E, [&]() {
		Grab();
		});
	//_window->SubscribeKeyPressed(sf::Keyboard::Space, [&]() {
	//	Attack();
	//	});

	//_window->SubscribeKeyPressed(sf::Keyboard::E, [&]() {
	//	InteractWithBomb();
	//	});
}


Player::~Player()
{
	delete playerLink;
}

void Player::Update()
{
	if (IsPlayer)
	{


		//switch (ConsoleControl::ReadNextKey())
		//{
		//case KB_UP:
		//{
		//	//If the animation has not ended yet
		//	if (_currentAnim != nullptr)
		//		_currentAnim->Stop();

		//	_currentAnim = playerLink->moveUpAnim;


		//	_position.y -= POSITION_INCREMENT;
		//	_orientation = UP;

		//	_currentAnim->PlayOnce([&]() {
		//		_currentAnim->Stop();

		//		});

		//	break;
		//}
		//case KB_DOWN:
		//{
		//	if (_currentAnim != nullptr)
		//		_currentAnim->Stop();

		//	_currentAnim = playerLink->moveDownAnim;

		//	_position.y += POSITION_INCREMENT;
		//	_orientation = DOWN;

		//	_currentAnim->PlayOnce([&]() {
		//		_currentAnim->Stop();

		//		});

		//	break;
		//}
		//case KB_LEFT:
		//{
		//	if (_currentAnim != nullptr)
		//		_currentAnim->Stop();

		//	_currentAnim = playerLink->moveLeftAnim;

		//	_position.x -= POSITION_INCREMENT;
		//	_orientation = LEFT;

		//	_currentAnim->PlayOnce([&]() {
		//		_currentAnim->Stop();
		//		_currentAnim = nullptr;
		//		});

		//	break;
		//}
		//case KB_RIGHT:
		//{
		//	if (_currentAnim != nullptr)
		//		_currentAnim->Stop();

		//	_currentAnim = playerLink->moveRightAnim;

		//	_position.x += POSITION_INCREMENT;
		//	_orientation = RIGHT;

		//	_currentAnim->PlayOnce([&]() {
		//		_currentAnim->Stop();
		//		_currentAnim = nullptr;
		//		});

		//	break;
		//}
		//}
	}

}

void Player::UpdateValues(ClientData* data)
{




	this->data = data;
	name->setPosition(data->posX,data->posY);
	if (this->data->action == MOVE)
	{


		switch (this->data->orientation)
		{
		case UP:

			if(!data->hasBomb)
				_currentAnim = playerLink->moveUpAnim;
			else
				_currentAnim = playerLink->chargeUpAnim;

			if (!_currentAnim->IsPlaying())
			{
				_currentAnim->PlayOnce([&]() {
					_currentAnim->Stop();

					});
			}

			break;
		case DOWN:


			if (!data->hasBomb)
				_currentAnim = playerLink->moveDownAnim;
			else
				_currentAnim = playerLink->chargeDownAnim;

			if (!_currentAnim->IsPlaying())
			{
				_currentAnim->PlayOnce([&]() {
					_currentAnim->Stop();

					});
			}
			break;
		case RIGHT:


			if (!data->hasBomb)
				_currentAnim = playerLink->moveRightAnim;
			else
				_currentAnim = playerLink->chargeRightAnim;

			if (!_currentAnim->IsPlaying())
			{
				_currentAnim->PlayOnce([&]() {
					_currentAnim->Stop();

					});
			}
			break;
		case LEFT:


			if (!data->hasBomb)
				_currentAnim = playerLink->moveLeftAnim;
			else
				_currentAnim = playerLink->chargeLeftAnim;

			if (!_currentAnim->IsPlaying())
			{

				_currentAnim->PlayOnce([&]() {
					_currentAnim->Stop();

					});
			}
			break;
		default:
			break;
		}
	}

	if (this->data->action != lastFrameData->action)
	{
		if (this->data->action != ATTACK)
		{
			sword->setPosition(-15, -15);

		}

		if (this->data->action == ATTACK)
		{
			switch (this->data->orientation)
			{
			case UP:
				sword->setPosition(this->data->posX-5, this->data->posY -7);
				sword->setRotation(0);
				_currentAnim->Stop();

				_currentAnim = playerLink->atackUpAnim;


		
					_currentAnim->PlayOnce([&]() {
						_currentAnim->Stop();

						});
				

				break;
			case DOWN:
				sword->setPosition(this->data->posX+40, this->data->posY + 45);
				sword->setRotation(180);
				_currentAnim->Stop();

				_currentAnim = playerLink->atackDownAnim;


		
					_currentAnim->PlayOnce([&]() {
						_currentAnim->Stop();

						});
				
				break;
			case RIGHT:
				sword->setPosition(this->data->posX+20 +25, this->data->posY);
				sword->setRotation(90);
				_currentAnim->Stop();

				_currentAnim = playerLink->atackRightAnim;


	
					_currentAnim->PlayOnce([&]() {
						_currentAnim->Stop();

						});
				
				break;
			case LEFT:
				sword->setPosition(this->data->posX+15 -25, this->data->posY+45);
				sword->setRotation(270);
				_currentAnim->Stop();

				_currentAnim = playerLink->atackLeftAnim;


		

					_currentAnim->PlayOnce([&]() {
						_currentAnim->Stop();

						});
				
				break;
			default:
				break;
			}
		}
		else if(this->data->action == GRAB)
		{
			switch (this->data->orientation)
			{
			case UP:


				if (!data->hasBomb)
					_currentAnim = playerLink->grabUpAnim;
				else
					_currentAnim = playerLink->throwUpAnim;


				_currentAnim->PlayOnce([&]() {
					_currentAnim->Stop();

					});


				break;
			case DOWN:
				if (!data->hasBomb)
					_currentAnim = playerLink->grabDownAnim;
				else
					_currentAnim = playerLink->throwDownAnim;



				_currentAnim->PlayOnce([&]() {
					_currentAnim->Stop();

					});

				break;
			case RIGHT:
				if (!data->hasBomb)
					_currentAnim = playerLink->grabRightAnim;
				else
					_currentAnim = playerLink->throwRightAnim;



				_currentAnim->PlayOnce([&]() {
					_currentAnim->Stop();

					});

				break;
			case LEFT:
				if (!data->hasBomb)
					_currentAnim = playerLink->grabLeftAnim;
				else
					_currentAnim = playerLink->throwLeftAnim;




				_currentAnim->PlayOnce([&]() {
					_currentAnim->Stop();

					});

				break;
			default:
				break;
			}

		}
	}




	playerLink->setPosition({ (float)this->data->posX, (float)this->data->posY });
	if(name != nullptr)
		name->setPosition({ (float)this->data->posX, (float)this->data->posY-10 });

	if(client->startMove >= 15 && client->clientData->action == MOVE)
		Stop();




}

void Player::MoveUp()
{
	if (client->clientData->action == ATTACK || client->clientData->action == GRAB)
		return;
	//If the animation has not ended yet

	ClientData data = *client->clientData;


	client->startMove = 0;
	data.orientation = UP;
	data.action = MOVE;

	sf::Packet p;
	p << static_cast<int>(PacketKeys::UpdateClient) << data;
	client->SendPacket(p);
}

void Player::MoveDown()
{
	if (client->clientData->action == ATTACK || client->clientData->action == GRAB)
		return;


	ClientData data = *client->clientData;


	client->startMove = 0;
	data.orientation = DOWN;
	data.action = MOVE;

	sf::Packet p;
	p << static_cast<int>(PacketKeys::UpdateClient) << data;
	client->SendPacket(p);
}

void Player::MoveLeft()
{
	if (client->clientData->action == ATTACK || client->clientData->action == GRAB)
		return;

	ClientData data = *client->clientData;


	client->startMove = 0;
	data.orientation = LEFT;
	data.action = MOVE;

	sf::Packet p;
	p << static_cast<int>(PacketKeys::UpdateClient) << data;
	client->SendPacket(p);
}

void Player::MoveRight()
{
	if (client->clientData->action == ATTACK|| client->clientData->action == GRAB)
		return;

	ClientData data = *client->clientData;


	client->startMove = 0;
	data.orientation = RIGHT;
	data.action = MOVE;
	sf::Packet p;
	p << static_cast<int>(PacketKeys::UpdateClient) << data;
	client->SendPacket(p);
}

void Player::Attack()
{
	if (client->clientData->action != ATTACK || client->clientData->action != GRAB)
	{
		ClientData data = *client->clientData;



		data.action = ATTACK;


		sf::Packet p;
		p << static_cast<int>(PacketKeys::UpdateClient) << data;
		client->SendPacket(p);
	}

}

void Player::Grab()
{
	if (client->clientData->action != ATTACK || client->clientData->action != GRAB)
	{
		ClientData data = *client->clientData;



		data.action = GRAB;

		sf::Packet p;
		p << static_cast<int>(PacketKeys::UpdateClient) << data;
		client->SendPacket(p);
	}
}

void Player::setPos(float x, float y)
{
	_position.x = x;
	_position.y = y;
}

void Player::Stop()
{
	client->startMove = 0;

	ClientData data = *client->clientData;



	data.action = IDLE;


	sf::Packet p;
	p << static_cast<int>(PacketKeys::UpdateClient) << data;
	client->SendPacket(p);

}
