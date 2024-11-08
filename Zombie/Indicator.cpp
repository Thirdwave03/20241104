#include "stdafx.h"
#include "Indicator.h"

Indicator::Indicator()
{
	sortingLayer = SortingLayers::Foreground;
	sortingOrder = 1;
}

Indicator::~Indicator()
{
}


void Indicator::Init()
{
}

void Indicator::Release()
{
}

void Indicator::Reset()
{
}

void Indicator::Update(float dt)
{
	timer -= dt;
	if (timer < 0)
	{
		SetActive(false);
	}

	body.setPosition(body.getPosition() + speed * dt);
	Utils::SetOrigin(body, Origins::MC);
}

void Indicator::Draw(sf::RenderWindow& window)
{
	window.draw(body);
}

void Indicator::SetTimer()
{
	timer = 2.5f;
}

void Indicator::SetIndicator(int info, sf::Sprite sp, sf::Color color)
{
	SetTimer();
	body.setFont(FONT_MGR.Get("fonts/zombiecontrol.ttf"));
	body.setCharacterSize(30.f);
	body.setFillColor(color);
	if (color == sf::Color::Yellow)
		body.setCharacterSize(45.f);
	string = std::to_string(info);
	body.setString(string);
	Utils::SetOrigin(body, Origins::MC);
	body.setPosition({ sp.getGlobalBounds().left + 40.f + Utils::RandomRange(-15,15), sp.getGlobalBounds().top + 10.f});
	SetActive(true);
}
