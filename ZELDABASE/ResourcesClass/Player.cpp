#include "Player.h"

Player::Player(Window* window) //: GameObject()
{
	std::cout << "Create player" << std::endl;
	playerLink = new LinkCharacter({ 15, 15 }, "Player");
	_position = { 15, 15 };
	_orientation = DOWN;
	_currentAnim = new Animation();
	_currentAnim = nullptr;

	_window = window;

	_window->AddTask([&](Window& w) {
		w.AddDrawable(playerLink);
		});

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
	switch (ConsoleControl::ReadNextKey())
	{
	case KB_UP:
	{
		//If the animation has not ended yet
		if (_currentAnim != nullptr)
			_currentAnim->Stop();

		_currentAnim = playerLink->moveUpAnim;


		_position.y -= POSITION_INCREMENT;
		_orientation = UP;

		_currentAnim->PlayOnce([&]() {
			_currentAnim->Stop();
			_currentAnim = nullptr;
			});

		break;
	}
	case KB_DOWN:
	{
		if (_currentAnim != nullptr)
			_currentAnim->Stop();

		_currentAnim = playerLink->moveDownAnim;

		_position.y += POSITION_INCREMENT;
		_orientation = DOWN;

		_currentAnim->PlayOnce([&]() {
			_currentAnim->Stop();
			_currentAnim = nullptr;
			});

		break;
	}
	case KB_LEFT:
	{
		if (_currentAnim != nullptr)
			_currentAnim->Stop();

		_currentAnim = playerLink->moveLeftAnim;

		_position.x -= POSITION_INCREMENT;
		_orientation = LEFT;

		_currentAnim->PlayOnce([&]() {
			_currentAnim->Stop();
			_currentAnim = nullptr;
			});

		break;
	}
	case KB_RIGHT:
	{
		if (_currentAnim != nullptr)
			_currentAnim->Stop();

		_currentAnim = playerLink->moveRightAnim;

		_position.x += POSITION_INCREMENT;
		_orientation = RIGHT;

		_currentAnim->PlayOnce([&]() {
			_currentAnim->Stop();
			_currentAnim = nullptr;
			});

		break;
	}
	}

	playerLink->setPosition({ _position.x, _position.y });

}

void Player::MoveUp()
{
	//If the animation has not ended yet
	if (_currentAnim != nullptr)
		_currentAnim->Stop();

	_currentAnim = playerLink->moveUpAnim;


	_position.y -= POSITION_INCREMENT;
	_orientation = UP;

	_currentAnim->PlayOnce([&]() {
		_currentAnim->Stop();
		_currentAnim = nullptr;
		});
}

void Player::MoveDown()
{
	if (_currentAnim != nullptr)
		_currentAnim->Stop();

	_currentAnim = playerLink->moveDownAnim;

	_position.y += POSITION_INCREMENT;
	_orientation = DOWN;

	_currentAnim->PlayOnce([&]() {
		_currentAnim->Stop();
		_currentAnim = nullptr;
		});
}

void Player::MoveLeft()
{
	if (_currentAnim != nullptr)
		_currentAnim->Stop();

	_currentAnim = playerLink->moveLeftAnim;

	_position.x -= POSITION_INCREMENT;
	_orientation = LEFT;

	_currentAnim->PlayOnce([&]() {
		_currentAnim->Stop();
		_currentAnim = nullptr;
		});
}

void Player::MoveRight()
{
	if (_currentAnim != nullptr)
		_currentAnim->Stop();

	_currentAnim = playerLink->moveRightAnim;

	_position.x += POSITION_INCREMENT;
	_orientation = RIGHT;

	_currentAnim->PlayOnce([&]() {
		_currentAnim->Stop();
		_currentAnim = nullptr;
		});
}
