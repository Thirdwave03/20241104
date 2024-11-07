#include "stdafx.h"
#include "UiNotification.h"

UiNotification::UiNotification(const std::string& name)
	: GameObject(name)
{
}

void UiNotification::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
}

void UiNotification::SetRotation(float angle)
{
	rotation = angle;
}

void UiNotification::SetScale(const sf::Vector2f& scale)
{
	this->scale = scale;
}

void UiNotification::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{

	}
}

void UiNotification::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
}

void UiNotification::Init()
{
	sortingLayer = SortingLayers::UI;
	sortingOrder = 7;
}

void UiNotification::Release()
{
}

void UiNotification::Reset()
{
	opacityController = 0;
	opacityDuration = 2.f;
	float textSize = 150.f;
	sf::Font& font = FONT_MGR.Get("fonts/zombiecontrol.ttf");
	textCenterMsg.setFont(font);
	textCenterMsg.setCharacterSize(textSize);
	textCenterMsg.setFillColor(sf::Color::White);
	Utils::SetOrigin(textCenterMsg, Origins::MC);
	auto wSize = FRAMEWORK.GetWindowSizeF();
	textCenterMsg.setPosition({wSize.x, wSize.y});
	textCenterMsg.setString("DEFAULT");
}

void UiNotification::Update(float dt)
{
	displayTimer -= dt;
	if (displayTimer < 0)
		SetActive(false);

	opacityController += dt * 511 / opacityDuration;

	sf::Uint8 opacity = 0;

	if (opacityController > 511)
		opacityController -= 511;
	if (opacityController > 255)
		opacity = 255 -(int)opacityController;
	else
		opacity = (int)opacityController;

	textCenterMsg.setFillColor({ sf::Color::White.r,  sf::Color::White.g, sf::Color::White.b, opacity });
}

void UiNotification::Draw(sf::RenderWindow& window)
{
	window.draw(textCenterMsg);
}

void UiNotification::IndicaterReloading()
{
	opacityController = 0.f;
	displayTimer = 4.f;
	textCenterMsg.setString("Reloading...");
	Utils::SetOrigin(textCenterMsg, Origins::MC);	
	auto wSize = FRAMEWORK.GetWindowSizeF();
	textCenterMsg.setPosition({ wSize.x/2, wSize.y/2 });
}

void UiNotification::IndicaterNoBullet()
{
	opacityController = 0.f;
	displayTimer = 3.f;
	textCenterMsg.setString("Press \"R\" to reload");
	Utils::SetOrigin(textCenterMsg, Origins::MC);
	auto wSize = FRAMEWORK.GetWindowSizeF();
	textCenterMsg.setPosition({ wSize.x / 2, wSize.y / 2 });
}

void UiNotification::IndicaterNoSpareBullet()
{
	opacityController = 0.f;
	displayTimer = 3.f;
	textCenterMsg.setString("bullets insufficient");
	Utils::SetOrigin(textCenterMsg, Origins::MC);
	auto wSize = FRAMEWORK.GetWindowSizeF();
	textCenterMsg.setPosition({ wSize.x / 2, wSize.y / 2 });
}
