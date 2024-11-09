#pragma once

class SceneGame;
class TileMap;

class Player : public GameObject
{
protected:
	sf::Sprite body;
	DebugBox debugBox;

	float debugHitTimer = 0.f;
	float debugItemTimer = 0.f;

	std::string textureId = "graphics/player.png";

	sf::Vector2f direction;
	sf::Vector2f look;

	float speed = 300;
	float bulletSpeed = 800;

	const float shootDelay = 1.f;
	float shootTimer = 0.f;

	int speedShootReload = 100;

	int playerDamage = 10;

	int playerMaxHp = 0;
	int playerHp = 0;

	int bulletCnt = 0;
	int clipSize = 0;
	int spareBullet = 0;
	int tobeReloadedBulletCnt = 0;

	int score = 0;
	int highScore = 0;

	int zombieCount = 0;

	int wave = 1;
	int spawnCnt = 0;

	int hpPickupMtp = 0;
	int bulletPickupMtp = 0;

	int upgradeCnt[8] = { 0 };
	int newUpgradeCnt[8] = { 0 };

	bool isReloading = false;
	float reloadTimer = 0.f;
	const float reloadSpeed = 3.f;

	bool powerOverwhelming = false;

	int cheatMtp = 1;

	SceneGame* sceneGame;
	TileMap* tileMap;

	sf::FloatRect movableBounds;

public:
	Player(const std::string& texId, const std::string& name_in = "");
	~Player() = default;

	void SetPosition(const sf::Vector2f& pos) override;
	void SetRotation(float angle) override;
	void SetScale(const sf::Vector2f& scale) override;

	void SetOrigin(Origins preset) override;
	void SetOrigin(const sf::Vector2f& newOrigin) override;

	sf::FloatRect GetLocalBounds() override;
	sf::FloatRect GetGlobalBounds() override;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void FixedUpdate(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	void ResetMovableBounds();
	sf::FloatRect GetMovableBounds();
	void RepositionInBounds();

	void SetPlayerHp(int Hp_in);
	void ReducePlayerHp(int dmg_in);
	void SetPlayerMaxHp(int maxHp);
	int GetPlayerMaxHp();
	int GetPlayerHp();
	int GetBulletCnt();
	void SetBulletCnt(int bulletCnt);
	int GetClipSize();
	void SetClipSize(int clipSize);
	int GetSpareBullet();
	int GetZombieCount();

	int GetScore();
	void SetScore(int score);

	int GetHighScore();
	void SetHighScore(int score_in);


	int GetWave();
	void SetWave(int wave);

	int GetSpawnCnt();
	void SetSpawnCnt(int spawnCnt);

	int GetShootReloadDelay();
	void SetShootReloadDelay(int delay);

	int GetPlayerDamage();
	void SetPlayerDamage(int damage);

	float GetSpeed();
	void SetSpeed(float speed);

	int GetHpPickupMultiplier();
	void SetHpPickupMultiplier(int mtplier);

	int GetBulletPickupMultiplier();
	void SetBulletPickupMultiplier(int mtplier);

	void TurnDebugBox(bool active, sf::Color color = sf::Color::Green) override;

	void CallReload();
	void Reload();

	void Shoot();
	bool GetReloadingStatus();

	void AddItemSpawnSpeed(int spawnspeed);
	void ResetItemSpawnSpeed();

	void SetPowerOverwhelming(bool active);
	void WhenPowerOverWhemling();

	void StageClearReset();

	int GetUpgradeCnt(int index);
	int* GetUpgradeCntArr();
	void SetUpgradeCnt(int index, int value);

	void GetSavedUpgradeCnt();
};
