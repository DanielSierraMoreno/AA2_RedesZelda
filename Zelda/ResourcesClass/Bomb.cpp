#include "Bomb.h"
#include "ConsoleControl.h"

Bomb::Bomb(Window* window, sf::Vector2f startPosition)
{
	this->setPosition(startPosition.x, startPosition.y);
	this->setTexture(*GetSpriteAtlas());
	this->setScale(sf::Vector2f(1.5f,1.5f));

	currentAnim = staticBomb;
	currentAnim->PlayOnce([&]() {

		currentAnim->Stop();

		});

	window->AddTask([&](Window& w) {
		w.AddDrawable(this);
		});
}

Bomb::~Bomb()
{
	delete staticBomb;
	delete explosion;
}


sf::Texture* Bomb::GetSpriteAtlas()
{
	static sf::Texture* spriteAtlas = nullptr;

	if (spriteAtlas == nullptr)
	{
		spriteAtlas = new sf::Texture();
		if (!spriteAtlas->loadFromFile("Resources/Bomb.png"))
		{
			ConsoleControl::LockMutex();
			std::cout << "Error al cargar el atlas de link";
			ConsoleControl::UnlockMutex();
		}
	}

	return spriteAtlas;
}
