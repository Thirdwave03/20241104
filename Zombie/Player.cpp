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
	playerMaxHp = 50+25*upgradeCnt[3] * cheatMtp;
	playerHp = playerMaxHp;
	speedShootReload = 100 +20*upgradeCnt[0];
	playerDamage = 20 + 5 * upgradeCnt[1];
	clipSize = 6 + 1 * upgradeCnt[2];
	bulletCnt = clipSize;
	spareBullet = clipSize;
	score = 0;
	speed = 125 + 25 * upgradeCnt[6];
	hpPickupMtp = 2 + 2*upgradeCnt[4];
	bulletPickupMtp = 3 + 1 * upgradeCnt[5];
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
	tileMap->Set({ 15,15 });

	ResetMovableBounds();

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
		sceneGame->SetUiNotificationActive(false);
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

	SetRotation(Utils::Angle(look));
	float reloadDebuff = 1.f;
	if (isReloading)
		reloadDebuff = 0.6;
	SetPosition(position + direction * speed * dt * reloadDebuff);

	Utils::Clamp(bulletCnt, 0, clipSize*6);
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
			spareBullet = Utils::Clamp(spareBullet, 0, clipSize*3);
			dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene())->OnItemPickedUp(item);
			break;
		}
	}

	RepositionInBounds();
	
	if (score > highScore)
	{
		highScore = score;
		SAVE_MGR.ResetHighScore(highScore);
	}

	for (int i = 0; i < 7; i++)
	{
		if (upgradeCnt[i] > newUpgradeCnt[i])
		{
			newUpgradeCnt[i] = upgradeCnt[i];
			SAVE_MGR.SetUpgradeCntFromGame(i, newUpgradeCnt[i]);
			SAVE_MGR.SaveUpgradeCnt();
		}
	}
}

void Player::Draw(sf::RenderWindow& window)
{
	window.draw(body);
	if (debugBox.IsVisible())
		debugBox.Draw(window);
}

void Player::ResetMovableBounds()
{
	movableBounds = tileMap->GetGlobalBounds();
	movableBounds.left += tileMap->GetCellSize().x;
	movableBounds.top += tileMap->GetCellSize().y;
	movableBounds.width -= tileMap->GetCellSize().x * 2;
	movableBounds.height -= tileMap->GetCellSize().y * 2;
}

sf::FloatRect Player::GetMovableBounds()
{
	return movableBounds;
}

void Player::RepositionInBounds()
{
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
		SetPosition({ GetPosition().x, movableBounds.top });
	}
	if (GetPosition().y > movableBounds.top + movableBounds.height)
	{
		SetPosition({ GetPosition().x, movableBounds.top + movableBounds.height });
	}
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

int Player::GetHighScore()
{
	return highScore;
}

void Player::SetHighScore(int score_in)
{
	highScore = score_in;
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

int Player::GetPlayerDamage()
{
	return playerDamage;
}

void Player::SetPlayerDamage(int damage)
{
	playerDamage = damage;
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
	if (!(bulletCnt == clipSize))
	{
		if (spareBullet != 0)
		{
			if (spareBullet < clipSize)
			{
				tobeReloadedBulletCnt = spareBullet;
			}
			else
			{
				tobeReloadedBulletCnt = clipSize;
			}

			tobeReloadedBulletCnt = Utils::Clamp(tobeReloadedBulletCnt, 1, clipSize - bulletCnt);
			if (!isReloading)
			{
				SOUND_MGR.PlaySfx("sound/reload_failed.wav");
				reloadTimer = reloadSpeed;
			}
			isReloading = true;
			sceneGame->CallUiReloading();
			return;
		}
		sceneGame->CallUiNoSpare();
	}
}

void Player::Reload()
{
	bulletCnt += tobeReloadedBulletCnt;
	spareBullet -= tobeReloadedBulletCnt;

	isReloading = false;
}

void Player::Shoot()
{
	if (bulletCnt > 0)
	{
		if (isReloading)
		{
			return;
		}
		SOUND_MGR.PlaySfx("sound/shoot.wav");
		Bullet* bullet = sceneGame->TakeBullet();
		bullet->SetActive(true);
		bullet->Fire(position, look, bulletSpeed, playerDamage);
		bulletCnt--;
		return;
	}
	if(!isReloading)
	sceneGame->CallUiNoBullet();
}

bool Player::GetReloadingStatus()
{
	return isReloading;
}

void Player::AddItemSpawnSpeed(int spawnspeed)
{
	sceneGame->SetItemSpawnSpeed(sceneGame->GetItemSpawnSpeed()+spawnspeed);
}

void Player::ResetItemSpawnSpeed()
{
	sceneGame->SetItemSpawnSpeed(100);
}

void Player::SetPowerOverwhelming(bool active)
{
	powerOverwhelming = active;
	if (active)
	{
		playerMaxHp = 500;
		speedShootReload = 300;
		playerDamage = 100;
		bulletCnt = 18;
		clipSize = 6;
		speed = 300;
		spareBullet = 18;
		hpPickupMtp = 2;
		bulletPickupMtp = 3;
		wave = 1;
		spawnCnt = 1;
	}
}

void Player::WhenPowerOverWhemling()
{
	playerHp = playerMaxHp;
}

void Player::StageClearReset()
{
	playerHp = playerMaxHp;
	bulletCnt = clipSize;
	spareBullet = clipSize;
}

int Player::GetUpgradeCnt(int index)
{
	return upgradeCnt[index];
}

int* Player::GetUpgradeCntArr()
{
	return upgradeCnt;
}

void Player::SetUpgradeCnt(int index, int value)
{
	upgradeCnt[index] = value;
}

void Player::GetSavedUpgradeCnt()
{
	SAVE_MGR.LoadUpgradeCnt();
	for (int i = 0; i < 7; i++)
	{
		upgradeCnt[i] = SAVE_MGR.GetSavedUpgradeCnt(i);
	}
}
