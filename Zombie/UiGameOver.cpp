#include "stdafx.h"
#include "UiGameOver.h"

UiGameOver::UiGameOver(const std::string& name)
	: GameObject(name)
{
}

void UiGameOver::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
}

void UiGameOver::SetRotation(float angle)
{
	rotation = angle;
}

void UiGameOver::SetScale(const sf::Vector2f& scale)
{
	this->scale = scale;
}

void UiGameOver::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{

	}
}

void UiGameOver::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
}

void UiGameOver::Init()
{
	sortingLayer = SortingLayers::UI;
	sortingOrder = 7;
}

void UiGameOver::Release()
{
}

void UiGameOver::Reset()
{
	float textSize = 200.f;
	sf::Font& font = FONT_MGR.Get("fonts/zombiecontrol.ttf");
	textGameOver.setFont(font);
	textGameOver.setCharacterSize(textSize);
	textGameOver.setFillColor(sf::Color::White);
	Utils::SetOrigin(textGameOver, Origins::ML);
	textGameOver.setPosition({ 400.f, 300.f });
	textGameOver.setString("PRESS ENTER\nTO CONTINUE");
}

void UiGameOver::Update(float dt)
{

}

void UiGameOver::Draw(sf::RenderWindow& window)
{
	window.draw(textGameOver);
}
