#include "stdafx.h"
#include "XXXGo.h"

XXXGo::XXXGo(const std::string& name)
	: GameObject(name)
{
}

void XXXGo::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
}

void XXXGo::SetRotation(float angle)
{
	rotation = angle;
}

void XXXGo::SetScale(const sf::Vector2f& s)
{
	scale = s;
}

void XXXGo::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{

	}
}

void XXXGo::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
}

void XXXGo::Init()
{
}

void XXXGo::Release()
{
}

void XXXGo::Reset()
{
}

void XXXGo::Update(float dt)
{
}

void XXXGo::Draw(sf::RenderWindow& window)
{
}
