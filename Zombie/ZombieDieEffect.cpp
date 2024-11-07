#include "stdafx.h"
#include "ZombieDieEffect.h"

ZombieDieEffect::ZombieDieEffect()
	:SpriteGo("graphics/blood.png")
{
	sortingLayer = SortingLayers::Foreground;
	sortingOrder = -10;
}


void ZombieDieEffect::Update(float dt)
{
	timer -= dt;
	if (timer < 0)
	{
		SetActive(false);
	}
	if (timer < 5);
	{
		fadeoutTimer = timer;
	}

	sf::Uint8 alpha = (int)(255 - 255 * (10-timer) / 10);
	sprite.setColor({ sprite.getColor().r, sprite.getColor().g, sprite.getColor().b, alpha });
}
