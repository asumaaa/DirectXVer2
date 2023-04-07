#include "FireParticle.h"

void FireParticle::SetTexture(const wchar_t* fileName)
{
	for (int i = 0; i < textureNum; i++)
	{
		std::unique_ptr<Sprite>newSprite = std::make_unique<Sprite>();
		newSprite->Initialize();
		/*sprite.push_back(std::move(newSprite));*/
	}
	/*sprite = new Sprite;
	sprite->Initialize();
	sprite->LoadFile(L"Resources/toriko2.png");*/
}

void FireParticle::SetSprite(Sprite* sprite_)
{
	/*for (int i = 0; i < textureNum; i++)
	{
		std::unique_ptr<Sprite>newSprite = std::make_unique<Sprite>();
		sprite.push_back(std::move(newSprite));
	}*/
}
