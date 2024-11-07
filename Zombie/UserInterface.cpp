#include "stdafx.h"
#include "UserInterface.h"
#include "Player.h"

UserInterface::UserInterface(const std::string& name)
{

}

void UserInterface::Init()
{
	maxHpBar.setFillColor(sf::Color(140, 140, 140));
	maxHpBar.setOutlineColor(sf::Color::Black);
	maxHpBar.setOutlineThickness(2.f);
	maxHpBar.setSize({ 50.f, 10.f });
	Utils::SetOrigin(maxHpBar, Origins::TC);

	HpBar.setFillColor(sf::Color::Red);
	HpBar.setSize({ 50.f, 10.f });
	Utils::SetOrigin(HpBar, Origins::TC);

	sortingLayer = SortingLayers::UI;
	sortingOrder = 0;
}

void UserInterface::Release()
{
	UserInterface::~UserInterface();
}

void UserInterface::Reset()
{
	maxHpBar.setFillColor(sf::Color(140, 140, 140));
	maxHpBar.setOutlineColor(sf::Color::Black);
	maxHpBar.setOutlineThickness(2.f);
	maxHpBar.setSize({ 50.f, 10.f });
	Utils::SetOrigin(maxHpBar, Origins::TC);

	HpBar.setFillColor(sf::Color::Red);
	HpBar.setSize({ 50.f, 10.f });
	Utils::SetOrigin(HpBar, Origins::TC);

	sortingLayer = SortingLayers::UI;
	sortingOrder = 0;
}

void UserInterface::Update(float dt)
{
	auto pPos = player->GetPosition();
	maxHpBar.setPosition(pPos.x, pPos.y - 50);
	HpBar.setPosition(pPos.x-2.5f, pPos.y - 50);
	HpBar.setSize({ maxHpBar.getSize().x * player->GetPlayerHp() / player->GetPlayerMaxHp(), maxHpBar.getSize().y});
}

void UserInterface::Draw(sf::RenderWindow& window)
{
	window.draw(maxHpBar);
	window.draw(HpBar);
}

void UserInterface::SetPlayer(Player* player)
{
	this->player = player;
}

