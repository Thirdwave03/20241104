#pragma once
#include "Scene.h"

class Player;
class TileMap;
class Zombie;
class Bullet;
class Item;
class DebugBox;
class UiHud;
class UiUpgrade;
class UiGameOver;
class UiNotification;
class ZombieDieEffect;
class Indicator;

enum class UpgradeTypes;

class SceneGame :
    public Scene
{
protected:
    Player* player;
	TileMap* tileMap;
	UiHud* uiHud;
	UiUpgrade* uiUpgrade = nullptr;
	UiGameOver* uiGameOver = nullptr;
	UiNotification* uiNotification = nullptr;

	std::list<Zombie*> zombies; // active zombies
	ObjectPool<Zombie> zombiePool;

	std::list<Bullet*> bullets;
	ObjectPool<Bullet> bulletPool;

	sf::Sprite cursor;

	std::list<Item*> items;
	ObjectPool<Item> itemPool;

	std::list<ZombieDieEffect*> zombieDieEffects;
	ObjectPool<ZombieDieEffect> zombieDieEffectPool;

	std::list<Indicator*> indicators;
	ObjectPool<Indicator> indicatorPool;

	float itemSpawnTimer = 0.f;
	float itemSpawnDuration = 12.f;
	int itemSpawnSpeed = 100.f;

	float zombieSpawnTimer = 0.f;
	float zombieSpawnDuration = 9.f;

	bool isDead = false;
	bool isUpgrading = false;
	bool skipPreventer = false;

public:
	SceneGame();
	virtual ~SceneGame() = default;

	virtual void Init() override;
	virtual void Release() override;

	virtual void Enter() override;
	virtual void Exit() override;

	virtual void Update(float dt) override;
	
	virtual void Draw(sf::RenderWindow& window) override;

	//void CollisionCheck();

	void SpawnZombies(int count);
	void SpawnEliteZombies(int count);
	void SpawnItem(int count);

	Bullet* TakeBullet();
	void ReturnBullet(Bullet* bullet);

	TileMap* GetTileMap() { return tileMap; }

	const std::list<Zombie*>& GetZombieList() { return zombies; }
	const std::list<Item*>& GetItemList() { return items; }
	const std::list<Indicator*>& GetIndicatorList() { return indicators; }

	void OnZombieDie(Zombie* zombie);
	void OnBulletHit(Bullet* bullet);
	void OnItemPickedUp(Item* item);
	void OnUpgrade(UpgradeTypes upgrade);

	void SetUpgrading(bool isUpgrading);

	void CallUiNoBullet();
	void CallUiReloading();
	void CallUiNoSpare();
	void SetUiNotificationActive(bool active);
	void SetItemSpawnSpeed(int spawnSpeed);
	int GetItemSpawnSpeed();

	void SetIndicator(int info, sf::Sprite sprite, sf::Color color = sf::Color::Red);
};

