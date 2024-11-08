#include "stdafx.h"
#include "Zombie.h"
#include "Player.h"
#include "SceneGame.h"
#include "DebugBox.h"

enum class Types;

Zombie::Zombie(const std::string& name)
	: GameObject(name)
{
}

void Zombie::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	body.setPosition(position);
}

void Zombie::SetRotation(float angle)
{
	rotation = angle;
	body.setRotation(rotation);
}

void Zombie::SetScale(const sf::Vector2f& s)
{
	scale = s;
	body.setScale(scale);
}

void Zombie::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{
		origin = Utils::SetOrigin(body, originPreset);
	}
}

void Zombie::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
	body.setOrigin(origin);
}

sf::FloatRect Zombie::GetLocalBounds()
{
	return body.getLocalBounds();
}

sf::FloatRect Zombie::GetGlobalBounds()
{
	return body.getGlobalBounds();
}

sf::RectangleShape& Zombie::GetDebugBox()
{
	return debugBox.GetShape();
}

void Zombie::Init()
{
	sortingLayer = SortingLayers::Foreground;
	sortingOrder = 0;
	SetType(types);
}

void Zombie::Release()
{
}

void Zombie::Reset()
{
	player = dynamic_cast<Player*>(SCENE_MGR.GetCurrentScene()->FindGo("Player"));
	sceneGame = (SceneGame*)(SCENE_MGR.GetCurrentScene());

	body.setTexture(TEXTURE_MGR.Get(textureId));
	SetOrigin(Origins::MC);
	SetPosition({ 0.f,0.f });
	SetRotation(0.f);
	SetScale({ 1.f,1.f });
}

void Zombie::Update(float dt)
{
	attackTimer += dt;
	actionRecoveryTimer += dt;
	if (actionRecoveryTimer > actionRecoveryTime)
	{
		if (player != nullptr && Utils::Distance(position, player->GetPosition()))
		{
			direction = Utils::GetNormal(player->GetPosition() - position);
			body.setRotation(Utils::Angle(direction));
			SetPosition(position + direction * speed * dt);
		}
	}

	debugBox.SetVisible((SceneGame*)(SCENE_MGR.GetCurrentScene())->GetDebugMode());
	
		//debugBox.SetBounds(GetLocalBounds(), body.getTransform()); // 도형에 박힌 연산만 받아옴
		// GlobalBound로 받을때는 Origin Trans Scale 값 가지고 있지만 Local에는 없음
		// sprite도 SRT 값 가지고있음 position
		//debugBox.SetDirection(direction);

	debugBox.GetShape().setSize({ body.getLocalBounds().width, body.getLocalBounds().height });
	debugBox.GetShape().setOrigin(body.getOrigin());
	debugBox.GetShape().setRotation(body.getRotation());
	debugBox.GetShape().setScale(body.getScale());
	debugBox.GetShape().setPosition(body.getPosition());
	if (debugBox.IsVisible())
	{
		if (debugHitTimer > 0.f)
		{
			debugHitTimer -= dt;
			debugBox.SetDebugBoxColor(sf::Color::Red);
		}
		else
			debugBox.SetDebugBoxColor(sf::Color::Green);
	}
}

void Zombie::Draw(sf::RenderWindow& window)
{
	window.draw(body);
	if (debugBox.IsVisible())
		debugBox.Draw(window);
}

void Zombie::SetType(Types type)
{
	this->types = type;
	switch (this->types)
	{
	case Types::Bloater:
		textureId = "graphics/bloater.png";
		name = "Bloater";
		maxHp = 60 * statMultiplier;
		hp = maxHp;
		attackInterval = 4.f;
		damage = 10 * statMultiplier;
		speed = 40.f * statMultiplier;
		actionRecoveryTime = 3.f;
		actionRecoveryTimer = 0.f;
		score = 500 * statMultiplier;
		break;
	case Types::Chaser:
		textureId = "graphics/chaser.png";
		name = "Chaser";
		maxHp = 20 * statMultiplier;
		hp = maxHp;
		attackInterval = 2.f;
		damage = 3 * statMultiplier;
		speed = 150.f * statMultiplier;
		actionRecoveryTime = 1.f;
		actionRecoveryTimer = 0.f;
		score = 200 * statMultiplier;
		break;
	case Types::Crawler:
		textureId = "graphics/crawler.png";
		name = "Crawler";
		maxHp = 40 * statMultiplier;
		hp = maxHp;
		attackInterval = 3.f;
		damage = 5 * statMultiplier;
		speed = 90.f * statMultiplier;
		actionRecoveryTime = 2.f;
		actionRecoveryTimer = 0.f;
		score = 300 * statMultiplier;
		break;
	case Types::EliteBloater: // 체력4배, 공격력 1.5배 스피드 1.5배 스코어 10배
		textureId = "graphics/bloater.png";
		name = "EliteBloater";
		maxHp = 240 * statMultiplier;
		hp = maxHp;
		attackInterval = 4.f;
		damage = 15 * statMultiplier;
		speed = 60.f * statMultiplier;
		actionRecoveryTime = 4.f;
		actionRecoveryTimer = 0.f;
		score = 3000 * statMultiplier;
		break;
	case Types::EliteChaser: // 체력 4배, 공격력 2배 스피드 1.2배 스코어 10배
		textureId = "graphics/chaser.png";
		name = "EliteChaser";
		maxHp = 80 * statMultiplier;
		hp = maxHp;
		attackInterval = 2.f;
		damage = 6 * statMultiplier;
		speed = 180.f * statMultiplier;
		actionRecoveryTime = 2.f;
		actionRecoveryTimer = 0.f;
		score = 2000 * statMultiplier;
		break;
	case Types::EliteCrawler: // 체력 4배, 공격력 2배 스피드 1.33배 스코어 10배
		textureId = "graphics/crawler.png";
		name = "EliteCrawler";
		maxHp = 160 * statMultiplier;
		hp = maxHp;
		attackInterval = 3.f;
		damage = 10 * statMultiplier;
		speed = 120.f * statMultiplier;
		actionRecoveryTime = 3.f;
		actionRecoveryTimer = 0.f;
		score = 3000 * statMultiplier;
		break;
	}
	body.setTexture(TEXTURE_MGR.Get(textureId), true); // true 왜넣는다고?
	if ((int)type > 2)
	{
		body.setColor(sf::Color::Red);
		body.setScale(2.f, 2.f);
	}
}

int Zombie::Attack()
{
	if (attackTimer < attackInterval)
		return 0;
	attackTimer = 0;
	actionRecoveryTimer = 0;
	return damage;
}

void Zombie::TurnDebugBox(bool active, sf::Color color)
{
	debugBox.SetVisible(active);
	if (active)
	{
		debugBox.SetDebugBoxColor(color);
	}
}

int Zombie::GetZombieHp()
{
	return hp;
}

void Zombie::OnDamage(int dmg_in)
{
	int actualDmg = dmg_in;
	sf::Color color_in = sf::Color::Red;
	if (Utils::RandomRange(0, 6) == 6)
	{
		actualDmg *= 2;
		actualDmg + Utils::RandomRange(-2, 1);
		color_in = sf::Color::Yellow;
	}

	hp -= actualDmg;
	debugHitTimer = 0.15f;
	sceneGame->SetIndicator(actualDmg, body, color_in);
	if (hp <= 0 && sceneGame!=nullptr)
	{
		player->SetScore(player->GetScore() + score);
		sceneGame->OnZombieDie(this);
	}
}

void Zombie::SetStatMultiplier(int wave)
{
	statMultiplier = (float)(9 + wave) / 10;
}
