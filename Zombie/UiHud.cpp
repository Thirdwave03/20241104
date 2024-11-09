#include "stdafx.h"
#include "UiHud.h"
#include "TextGo.h"
#include "Player.h"
#include "SceneGame.h"

UiHud::UiHud(const std::string& name)
	: GameObject(name)
{
}

void UiHud::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
}

void UiHud::SetRotation(float angle)
{
	rotation = angle;
}

void UiHud::SetScale(const sf::Vector2f& s)
{
	scale = s;
}

void UiHud::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{

	}
}

void UiHud::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
}

void UiHud::Init()
{
	sortingLayer = SortingLayers::UI;
	sortingOrder = 2;
}

void UiHud::Release()
{
}

void UiHud::Reset()
{
	player = dynamic_cast<Player*>(SCENE_MGR.GetCurrentScene()->FindGo("Player"));
	float textSize = 50.f;
	sf::Font& font = FONT_MGR.Get("fonts/zombiecontrol.ttf"); // reference 중요
	textScore.setFont(font);
	textScore.setCharacterSize(textSize);
	textScore.setFillColor(sf::Color::White);
	Utils::SetOrigin(textScore, Origins::TL);
	textHighScore.setFont(font);
	textHighScore.setCharacterSize(textSize);
	textHighScore.setFillColor(sf::Color::White);
	Utils::SetOrigin(textHighScore, Origins::TR);
	textAmmo.setFont(font);
	textAmmo.setCharacterSize(textSize);
	textAmmo.setFillColor(sf::Color::White);
	Utils::SetOrigin(textAmmo, Origins::BL);
	textSpareAmmo.setFont(font);
	textSpareAmmo.setCharacterSize(textSize);
	textSpareAmmo.setFillColor(sf::Color::White);
	Utils::SetOrigin(textSpareAmmo, Origins::BL);
	textWave.setFont(font);
	textWave.setCharacterSize(textSize);
	textWave.setFillColor(sf::Color::White);
	Utils::SetOrigin(textWave, Origins::BR);
	textZombieCount.setFont(font);
	textZombieCount.setCharacterSize(textSize);
	textZombieCount.setFillColor(sf::Color::White);
	Utils::SetOrigin(textZombieCount, Origins::BR);

	gaugeMaxHp.setFillColor(sf::Color(140,140,140));
	gaugeMaxHp.setSize(gaugeHpMaxSize);
	Utils::SetOrigin(gaugeMaxHp, Origins::BL);

	gaugeHp.setFillColor(sf::Color::Red);
	gaugeHp.setSize(gaugeHpMaxSize);
	Utils::SetOrigin(gaugeHp, Origins::BL);

	iconAmmoIcon.setTexture(TEXTURE_MGR.Get("graphics/ammo_icon.png"));
	Utils::SetOrigin(iconAmmoIcon, Origins::BL);


	sf::Vector2f windowSize = FRAMEWORK.GetWindowSizeF();

	float topY = 25.f;
	float bottomY = windowSize.y - 25.f;

	textScore.setPosition(25.f, topY);
	textHighScore.setPosition(windowSize.x - 25.f, topY);

	iconAmmoIcon.setPosition(25.f, bottomY + 10.f);
	gaugeMaxHp.setPosition(400.f, bottomY);
	gaugeHp.setPosition(400.f, bottomY);

	textAmmo.setPosition(100.f, bottomY);
	textSpareAmmo.setPosition(270.f, bottomY); // Why? 싱크 안맞음?

	textWave.setPosition(windowSize.x - 400.f, bottomY);
	textZombieCount.setPosition(windowSize.x - 25.f, bottomY);

	SetScore(0);
	SetHighScore(0);
	SetAmmo(0, 0);
	SetSpareAmmo(0);
	SetHp(50, 100);
	SetWave(0);
	SetZombieCount(0);
}

void UiHud::Update(float dt)
{
	// HP Bar
	float value = (float)player->GetPlayerHp() / (float)player->GetPlayerMaxHp();
	gaugeHp.setSize({ gaugeHpMaxSize.x * value ,gaugeHpMaxSize.y });

	// Bullets
	textAmmo.setString(std::to_string(player->GetBulletCnt()) + " / " + std::to_string(player->GetClipSize()));
	textSpareAmmo.setString(std::to_string(player->GetSpareBullet()));

	// Score
	textScore.setString("SCORE: " + std::to_string(player->GetScore()));
	Utils::SetOrigin(textScore, Origins::TL);
	textHighScore.setString("HIGH SCORE: " + std::to_string(player->GetHighScore()));
	Utils::SetOrigin(textHighScore, Origins::TR);

	// Zombie Count
	textZombieCount.setString("ZOMBIES: " + std::to_string(player->GetSpawnCnt()*5 + player->GetZombieCount()));
	Utils::SetOrigin(textZombieCount, Origins::BR);
	textWave.setString("WAVE: " + std::to_string(player->GetWave()));
	Utils::SetOrigin(textWave, Origins::BR);
}

void UiHud::Draw(sf::RenderWindow& window)
{
	window.draw(textScore);
	window.draw(textHighScore);
	window.draw(textAmmo);
	window.draw(iconAmmoIcon);
	window.draw(textSpareAmmo);
	window.draw(gaugeMaxHp);
	window.draw(gaugeHp);
	window.draw(textWave);
	window.draw(textZombieCount);	
}

void UiHud::SetScore(int score)
{
	textScore.setString("SCORE: " + std::to_string(score));
	Utils::SetOrigin(textScore, Origins::TL);	
}

void UiHud::SetHighScore(int score)
{
	textHighScore.setString("HIGH SCORE: " + std::to_string(score));
	Utils::SetOrigin(textHighScore, Origins::TR);
}

void UiHud::SetAmmo(int current, int total)
{
	textAmmo.setString(std::to_string(current) + " / " + std::to_string(total));
	Utils::SetOrigin(textAmmo, Origins::BL);
}

void UiHud::SetSpareAmmo(int spareAmmo)
{
	textSpareAmmo.setString(std::to_string(player->GetSpareBullet()));
	Utils::SetOrigin(textSpareAmmo, Origins::BL);
}

void UiHud::SetHp(int hp, int max)
{
	float value = (float)hp / max;
	gaugeMaxHp.setSize({ gaugeHpMaxSize.x, gaugeHpMaxSize.y });
	gaugeHp.setSize({ gaugeHpMaxSize.x * value, gaugeHpMaxSize.y });
	Utils::SetOrigin(gaugeHp, Origins::BL);
}

void UiHud::SetWave(int w)
{
	textWave.setString("WAVE: " + std::to_string(w));
	Utils::SetOrigin(textWave, Origins::BR);
}

void UiHud::SetZombieCount(int count)
{
	textZombieCount.setString("ZOMBIES: " + std::to_string(count));
	Utils::SetOrigin(textZombieCount, Origins::BR);
}
