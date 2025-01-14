#include "stdafx.h"
#include "Player.h"
#include "Bullet.h"
#include "TileMap.h"
#include "SceneGame.h"
#include "DebugBox.h"
#include "Zombie.h"
#include "Item.h"

Player::Player(const std::string& texId, const std::string& name_in)
	: GameObject(name_in)
{
	textureId = texId;
}

void Player::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	body.setPosition(position);
}

void Player::SetRotation(float angle)
{
	rotation = angle;
	body.setRotation(rotation);
}

void Player::SetScale(const sf::Vector2f& s)
{
	scale = s;
	body.setScale(s);
}

void Player::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{
		origin = Utils::SetOrigin(body, originPreset);
	}
}

void Player::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	body.setOrigin(newOrigin);
}

sf::FloatRect Player::GetLocalBounds()
{
	return body.getLocalBounds();
}

sf::FloatRect Player::GetGlobalBounds()
{
	return body.getGlobalBounds();
}

void Player::Init()
{
	sortingLayer = SortingLayers::Foreground;
	sortingOrder = 0;
	SetOrigin(Origins::MC);
}

void Player::Release()
{
}

void Player::Reset()
{
	playerMaxHp = 100;
	playerHp = 100;
	speedShootReload = 100;
	playerDamage = 20;
	bulletCnt = 6;
	clipSize = 6;
	score = 0;
	speed = 150;
	spareBullet = 12;
	hpPickupMtp = 2;
	bulletPickupMtp = 3;
	wave = 1;
	spawnCnt = 1;
	sceneGame = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene());
	body.setTexture(TEXTURE_MGR.Get(textureId));
	SetOrigin(originPreset);
	tileMap = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene())->GetTileMap();
	SetPosition(
		{ (tileMap->GetGlobalBounds().width + tileMap->GetGlobalBounds().left * 2) / 2,
		(tileMap->GetGlobalBounds().height + tileMap->GetGlobalBounds().top * 2) / 2
		}
	);
	movableBounds = tileMap->GetGlobalBounds();
	SetRotation(Pi / 2);
	direction = { 0.f,0.f };		
}

void Player::Update(float dt)
{	
	if (playerHp <= 0)
		return;
	direction.x = InputMgr::GetAxis(Axis::Horizontal);
	direction.y = InputMgr::GetAxis(Axis::Vertical);
	float dirMagnitude = Utils::Magnitude(direction);
	if (reloadTimer < 4.f && reloadTimer > 0.f)
	{
		reloadTimer -= dt * speedShootReload/100;
	}
	if (reloadTimer <= 0)
	{
		reloadTimer = 10.f;
		Reload();
		SOUND_MGR.PlaySfx("sound/reload.wav");
	}

	if (dirMagnitude > 1.f)
	{
		Utils::Normailize(direction);
	}

	sf::Vector2i mPos = InputMgr::GetMousePosition();
	sf::Vector2f mouseWorldPos = (sf::Vector2f)mPos;

	if (sceneGame != nullptr)
	{
		mouseWorldPos = sceneGame->ScreenToWorld(mPos);
	}
	look = Utils::GetNormal(mouseWorldPos - position);

	if (GetPosition().x < movableBounds.left)
	{
		SetPosition({ movableBounds.left,GetPosition().y });
	}
	if (GetPosition().x > movableBounds.left + movableBounds.width)
	{
		SetPosition({ movableBounds.left + movableBounds.width,GetPosition().y });
	}
	if (GetPosition().y < movableBounds.top)
	{
		SetPosition({ GetPosition().x, movableBounds.top});
	}
	if (GetPosition().y > movableBounds.top + movableBounds.height)
	{
		SetPosition({ GetPosition().x, movableBounds.top + movableBounds.height});
	}
	SetRotation(Utils::Angle(look));
	SetPosition(position + direction * speed * dt);

	Utils::Clamp(bulletCnt, 0, 50);
	shootTimer += dt * speedShootReload / 100;

	if (shootTimer > shootDelay && InputMgr::GetMouseButtonDown(sf::Mouse::Left))
	{
		shootTimer = 0.f;
		Shoot();
	}

	
	debugBox.GetShape().setSize({ body.getLocalBounds().width, body.getLocalBounds().height });
	debugBox.GetShape().setOrigin(body.getOrigin());
	debugBox.GetShape().setRotation(body.getRotation());
	debugBox.GetShape().setScale(body.getScale());
	debugBox.GetShape().setPosition(body.getPosition());
	
	
	if (debugHitTimer >= 0)
	{
		debugHitTimer -= dt;
		debugItemTimer -= dt;
		debugBox.SetDebugBoxColor(sf::Color::Red);
	}
	else if (debugItemTimer >= 0)
	{
		debugItemTimer -= dt;
		debugBox.SetDebugBoxColor(sf::Color::Blue);
	}
	else
		debugBox.SetDebugBoxColor(sf::Color::Green);
	
}

void Player::FixedUpdate(float dt)
{
	zombieCount = 0;
	auto& zlist = sceneGame->GetZombieList();
	for (auto& zombie : zlist)
	{
		zombieCount++;
		if (Utils::CheckCollision(debugBox.GetShape(), zombie->GetDebugBox()))
		{
			int temp = zombie->Attack();
			playerHp -= temp;
			playerHp = Utils::Clamp(playerHp, 0, playerMaxHp);
			if (temp)
				debugHitTimer = 0.15f;
		}
	}

	auto& iList = sceneGame->GetItemList();
	for (auto& item : iList)
	{
		if (Utils::CheckCollision(debugBox.GetShape(), item->GetDebugBox()))
		{
			sf::Vector2i temp = item->PickedUp();
			spareBullet += temp.x * bulletPickupMtp;
			playerHp += temp.y * hpPickupMtp;
			debugItemTimer = 0.25f;
			playerHp = Utils::Clamp(playerHp, 0, playerMaxHp);
			spareBullet = Utils::Clamp(spareBullet, 0, 60);
			dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene())->OnItemPickedUp(item);
			break;
		}
	}
}

void Player::Draw(sf::RenderWindow& window)
{
	window.draw(body);
	if (debugBox.IsVisible())
		debugBox.Draw(window);
}

void Player::SetPlayerHp(int Hp_in)
{
	playerHp = Hp_in;
}

void Player::ReducePlayerHp(int dmg_in)
{
	playerHp -= dmg_in;
}

void Player::SetPlayerMaxHp(int maxHp)
{
	playerMaxHp = maxHp;
}

int Player::GetPlayerMaxHp()
{
	return playerMaxHp;
}

int Player::GetPlayerHp()
{
	return playerHp;
}

int Player::GetBulletCnt()
{
	return bulletCnt;
}

void Player::SetBulletCnt(int bulletCnt)
{
	this->bulletCnt = bulletCnt;
}

int Player::GetClipSize()
{
	return clipSize;
}

void Player::SetClipSize(int clipSize)
{
	this->clipSize = clipSize;
}

int Player::GetSpareBullet()
{
	return spareBullet;
}

int Player::GetZombieCount()
{
	return zombieCount;
}

int Player::GetScore()
{
	return score;
}

void Player::SetScore(int score)
{
	this->score = score;
}

int Player::GetWave()
{
	return wave;
}

void Player::SetWave(int wave)
{
	this->wave = wave;
}

int Player::GetSpawnCnt()
{
	return spawnCnt;
}

void Player::SetSpawnCnt(int spawnCnt)
{
	this->spawnCnt = spawnCnt;
}

int Player::GetShootReloadDelay()
{
	return speedShootReload;
}

void Player::SetShootReloadDelay(int delay)
{
	speedShootReload = delay;
}

float Player::GetSpeed()
{
	return speed;
}

void Player::SetSpeed(float speed)
{
	this->speed = speed;
}

int Player::GetHpPickupMultiplier()
{
	return hpPickupMtp;
}

void Player::SetHpPickupMultiplier(int mtplier)
{
	hpPickupMtp = mtplier;
}

int Player::GetBulletPickupMultiplier()
{
	return bulletPickupMtp;
}

void Player::SetBulletPickupMultiplier(int mtplier)
{
	bulletPickupMtp = mtplier;
}

void Player::TurnDebugBox(bool active, sf::Color color)
{	
	debugBox.SetVisible(active);
	if (active)
	{
		debugBox.SetBounds(this->GetGlobalBounds());
		debugBox.SetDebugBoxColor(color);
	}
}

void Player::CallReload()
{
	if(!isReloading)
		SOUND_MGR.PlaySfx("sound/reload_failed.wav");
	if (!(bulletCnt == clipSize) && spareBullet != 0)
	{
		isReloading = true;
		reloadTimer = reloadSpeed;
	}
}

void Player::Reload()
{
	if (clipSize - bulletCnt <= spareBullet)
	{
		spareBullet -= (clipSize - bulletCnt);
		bulletCnt = clipSize;
	}
	else
	{
		bulletCnt += spareBullet;
		spareBullet = 0;
	}
	isReloading = false;
}

void Player::Shoot()
{
	if (bulletCnt > 0)
	{
		if (isReloading)
			return;
		SOUND_MGR.PlaySfx("sound/shoot.wav");
		Bullet* bullet = sceneGame->TakeBullet();
		bullet->Fire(position, look, bulletSpeed, playerDamage);
		bulletCnt--;
	}
}