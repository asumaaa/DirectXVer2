#include "FireParticle.h"
#include "stdlib.h"
#define PI 3.141592653589793238462643

void FireParticle::Initialize()
{
	//要素数
	positions.resize(spriteNum);
	scales.resize(spriteNum);
	rotations.resize(spriteNum);
	velocitys.resize(spriteNum);
	colors.resize(spriteNum);

	for (int i = 0; i < spriteNum; i++)
	{
		std::unique_ptr<Sprite>newSprite = std::make_unique<Sprite>();
		newSprite->Initialize();
		//ランダムで違うテクスチャをセット
		if (static_cast<float>(rand() % 2) == 0)
		{
			newSprite->SetTextureNum(textureNum0);
		}
		else
		{
			newSprite->SetTextureNum(textureNum1);
		}
		sprite.push_back(std::move(newSprite));
	}
	/*sprite = new Sprite;
	sprite->Initialize();
	sprite->LoadFile(L"Resources/toriko2.png");*/

	for (int i = 0; i < spriteNum; i++)
	{
		velocitys[i].x = 0.0f;
		velocitys[i].y = 0.0f;
		colors[i].x = 1.0f;
		colors[i].y = 1.0f;
		colors[i].z = 1.0f;
	}
}

void FireParticle::Update()
{
	timer += 0.5;
	if (timer > time)
	{
		timer = 0;
	}

	for (int i = 0; i < spriteNum; i++)
	{
		//タイマーで位置リセット
		if (i == timer)
		{
			scales[i].x = scale.x;
			scales[i].y = scale.y;
			positions[i].x = position.x;
			positions[i].y = position.y;
			velocitys[i].x = static_cast<float>(rand() % 4 - 1);
			velocitys[i].y = static_cast<float>(rand() % 4 - 4);
			colors[i].x = 1.0f;
			colors[i].y = 1.0f;
			colors[i].z = 1.0f;
		}
		scales[i].x -= 4;
		scales[i].y -= 4;
		positions[i].x += velocitys[i].x;
		positions[i].y += velocitys[i].y;
		colors[i].x -= 0.04f;
		colors[i].y -= 0.04f;
		colors[i].z -= 0.04f;
	}

	int i = 0;
	for (std::unique_ptr<Sprite>& sprite : sprite)
	{
		sprite->SetAlpha(1.0f);
		sprite->SetColor(colors[i]);
		sprite->SetScale({ scales[i].x,scales[i].y });
		sprite->SetPosition({ positions[i].x,positions[i].y });
		sprite->Update();

		i++;
	}
}

void FireParticle::Draw(ID3D12GraphicsCommandList* cmdList)
{
	for(std::unique_ptr<Sprite>& sprite : sprite)
	{
		sprite->Draw(cmdList);
	}
}
