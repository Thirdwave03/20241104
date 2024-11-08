#include "stdafx.h"
#include "UiUpgrade.h"
#include "SceneGame.h"
#include "Player.h"

UiUpgrade::UiUpgrade(const std::string& name)
	: GameObject(name)
{
}

void UiUpgrade::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
}

void UiUpgrade::SetRotation(float angle)
{
	rotation = angle;
}

void UiUpgrade::SetScale(const sf::Vector2f& scale)
{
	this->scale = scale;
}

void UiUpgrade::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{

	}
}

void UiUpgrade::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
}

void UiUpgrade::Init()
{
	sortingLayer = SortingLayers::UI;
	sortingOrder = 5;
}

void UiUpgrade::Release()
{
}

void UiUpgrade::Reset()
{
	player = dynamic_cast<Player*>(SCENE_MGR.GetCurrentScene()->FindGo("Player"));
	float textSize = 70.f;
	sf::Font& font = FONT_MGR.Get("fonts/zombiecontrol.ttf");
	background.setTexture(TEXTURE_MGR.Get("graphics/background.png"));
	Utils::SetOrigin(background, Origins::MC);	
	auto wSize = FRAMEWORK.GetWindowSizeF();
	background.setPosition(wSize.x / 2, wSize.y / 2);

	for (int i = 0; i < noOfUpgrades; i++)
	{
		textUpgrades[i].setFont(font);
		textUpgrades[i].setCharacterSize(textSize);
		textUpgrades[i].setFillColor(sf::Color::White);
		Utils::SetOrigin(textUpgrades[i], Origins::ML);
		textUpgrades[i].setPosition({ 500.f, 250.f + i * 100 });
	}
	SetUpgradeText();
}

void UiUpgrade::Update(float dt)
{
	clickBlockTimer -= dt;
}

void UiUpgrade::FixedUpdate(float dt)
{
	SceneGame* sceneGame = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene());
	sf::Vector2f mPos = sceneGame->ScreenToUi(InputMgr::GetMousePosition());

	for (int i = 0; i < noOfUpgrades; i++)
	{
		if (textUpgrades[i].getGlobalBounds().contains(mPos))
		{
			textUpgrades[i].setFillColor(sf::Color::Red);
			if(!mouseLocated[i])
				SOUND_MGR.PlaySfx("sound/mouseover1.wav");
			mouseLocated[i] = true;
		}
		else
		{
			textUpgrades[i].setFillColor(sf::Color::White);
			mouseLocated[i] = false;
		}
	}
	if (InputMgr::GetMouseButtonDown(sf::Mouse::Left) && clickBlockTimer<0.f)
	{
		for (int i = 0; i < noOfUpgrades; i++)
		{
			if (textUpgrades[i].getGlobalBounds().contains(mPos))
			{
				SetUpgrade((UpgradeTypes)i);
			}
		}
	}
}

void UiUpgrade::Draw(sf::RenderWindow& window)
{
	window.draw(background);
	for (int i = 0; i < noOfUpgrades; i++)
	{
		window.draw(textUpgrades[i]);
	}
}

void UiUpgrade::SetUpgradeText()
{
	textUpgrades[0].setString("1.INCREASED FIRE RATE");
	textUpgrades[1].setString("2.INCREASED Damage");
	textUpgrades[2].setString("3.INCREASED CLIP SIZE"); // ÅºÃ¢»çÀÌÁî
	textUpgrades[3].setString("4.INCREASED MAX HEALTH");
	textUpgrades[4].setString("5.MORE AND BETTER HEALTH PICKUPS");
	textUpgrades[5].setString("6.MORE AND BETTER AMMO PICKUPS");
	textUpgrades[6].setString("7.INCREASED RUN SPEED");
}

void UiUpgrade::SetClickBlocker(float timer)
{
	clickBlockTimer = timer;
}

void UiUpgrade::SetUpgrade(UpgradeTypes ugType)
{
	ugTypes = ugType;
	switch (ugTypes)
	{
	case UpgradeTypes::FireRate:
		player->SetShootReloadDelay(player->GetShootReloadDelay()+40);
		SOUND_MGR.PlaySfx("sound/mousedown1.wav");
		break;
	case UpgradeTypes::Damage:
		player->SetPlayerDamage(player->GetPlayerDamage() + 10);
		SOUND_MGR.PlaySfx("sound/mousedown1.wav");
		break;
	case UpgradeTypes::ClipSize:
		player->SetClipSize(player->GetClipSize() + 3);
		SOUND_MGR.PlaySfx("sound/mousedown1.wav");
		break;
	case UpgradeTypes::MaxHp:
		player->SetPlayerMaxHp(player->GetPlayerMaxHp() + 25);
		SOUND_MGR.PlaySfx("sound/mousedown1.wav");
		break;
	case UpgradeTypes::HpPickup:
		player->SetHpPickupMultiplier(player->GetHpPickupMultiplier() + 2);
		player->AddItemSpawnSpeed(25);
		SOUND_MGR.PlaySfx("sound/mousedown1.wav");
		break;
	case UpgradeTypes::AmmoPickup:
		player->SetBulletPickupMultiplier(player->GetBulletPickupMultiplier() + 2);
		player->AddItemSpawnSpeed(25);
		SOUND_MGR.PlaySfx("sound/mousedown1.wav");
		break;
	case UpgradeTypes::Speed:
		player->SetSpeed(player->GetSpeed() + 50);
		SOUND_MGR.PlaySfx("sound/mousedown1.wav");
		break;
	}
	dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene())->SetUpgrading(false);
}
