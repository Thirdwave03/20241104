#include "stdafx.h"
#include "Item.h"
#include "SceneGame.h"

Item::Item(const std::string& name)
	: GameObject(name)
{
}

void Item::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	body.setPosition(position);
}

void Item::SetRotation(float angle)
{
	rotation = angle;
	body.setRotation(rotation);
}

void Item::SetScale(const sf::Vector2f& s)
{
	scale = s;
	body.setScale(scale);
}

void Item::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{
		origin = Utils::SetOrigin(body, originPreset);
	}
}

void Item::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
	body.setOrigin(origin);
}

sf::FloatRect Item::GetLocalBounds()
{
	return body.getLocalBounds();
}

sf::FloatRect Item::GetGlobalBounds()
{
	return body.getGlobalBounds();
}

sf::RectangleShape& Item::GetDebugBox()
{
	return debugBox.GetShape();
}

void Item::Init()
{
	sortingLayer = SortingLayers::Foreground;
	sortingOrder = 0;
	SetType(iTypes);
}

void Item::Release()
{
}

void Item::Reset()
{
	body.setTexture(TEXTURE_MGR.Get(textureId));
	SetOrigin(Origins::MC);
	SetPosition({ 0.f,0.f });
	SetRotation(0.f);
	SetScale({ 1.f,1.f });
}

void Item::Update(float dt)
{
	SetRotation(GetRotation() + 60 * dt);
	debugBox.SetVisible((SceneGame*)(SCENE_MGR.GetCurrentScene())->GetDebugMode());
	
	debugBox.GetShape().setSize({ body.getLocalBounds().width, body.getLocalBounds().height });
	debugBox.GetShape().setOrigin(body.getOrigin());
	debugBox.GetShape().setRotation(body.getRotation());
	debugBox.GetShape().setScale(body.getScale());
	debugBox.GetShape().setPosition(body.getPosition());
	debugBox.SetDebugBoxColor(sf::Color::Blue);	
}

void Item::Draw(sf::RenderWindow& window)
{
	window.draw(body);
	if (debugBox.IsVisible())
		debugBox.Draw(window);
}

void Item::TurnDebugBox(bool active, sf::Color color)
{
	debugBox.SetVisible(active);
	if (active)
	{		
		debugBox.SetDebugBoxColor(sf::Color::Blue);
	}
}

void Item::SetType(ItemTypes type)
{
	this->iTypes = type;
	switch (this->iTypes)
	{
	case ItemTypes::Ammo:
		textureId = "graphics/ammo_pickup.png";
		addAmmo = 2;
		addHp = 0;
		break;
	case ItemTypes::Health:
		textureId = "graphics/health_pickup.png";
		addAmmo = 0;
		addHp = 5;
		break;
	}
	body.setTexture(TEXTURE_MGR.Get(textureId));
}

sf::Vector2i Item::PickedUp()
{
	SOUND_MGR.PlaySfx("sound/pickup.wav");
	return sf::Vector2i(addAmmo, addHp);
}
