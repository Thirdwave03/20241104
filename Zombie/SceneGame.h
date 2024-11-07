#pragma once
#include "Scene.h"

class Player;
class TileMap;
class Zombie;
class Bullet;
class Item;
class DebugBox;
class UserInterface;
class UiHud;
class UiUpgrade;
class UiGameOver;
class ZombieDieEffect;
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

	std::list<Zombie*> zombies; // active zombies
	ObjectPool<Zombie> zombiePool;

	std::list<Bullet*> bullets;
	ObjectPool<Bullet> bulletPool;

	sf::Sprite cursor;

	std::list<Item*> items;
	ObjectPool<Item> itemPool;

	std::list<ZombieDieEffect*> zombieDieEffects;
	ObjectPool<ZombieDieEffect> zombieDieEffectPool;
	
	UserInterface* ui;

	float itemSpawnTimer = 0.f;
	float itemSpawnDuration = 5.f;
	float zombieSpawnTimer = 0.f;
	float zombieSpawnDuration = 5.f;

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
	void SpawnItem(int count);

	Bullet* TakeBullet();
	void ReturnBullet(Bullet* bullet);

	TileMap* GetTileMap() { return tileMap; }

	const std::list<Zombie*>& GetZombieList() { return zombies; }
	const std::list<Item*>& GetItemList() { return items; }

	void OnZombieDie(Zombie* zombie);
	void OnBulletHit(Bullet* bullet);
	void OnItemPickedUp(Item* item);
	void OnUpgrade(UpgradeTypes upgrade);

	void SetUpgrading(bool isUpgrading);
};

