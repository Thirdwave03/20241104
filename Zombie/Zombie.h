#pragma once

class Player;
class SceneGame;
class Indicator;

class Zombie : public GameObject
{
public:
	enum class Types
	{
		Bloater,
		Chaser,
		Crawler,
		EliteBloater,
		EliteChaser,
		EliteCrawler,
	};

	static const int TotalTypes = 6;

protected:
	Types types = Types::Bloater;
	sf::Sprite body;
	std::string textureId;

	float debugHitTimer = 0;

	sf::Vector2f direction;
	
	int maxHp = 0;
	int hp = 0;

	float speed = 0.f;

	int damage = 0;

	int score = 0;

	float attackInterval = 0.f;
	float attackTimer = 0.f;

	float actionRecoveryTime = 0.f;
	float actionRecoveryTimer = 0.f;

	Player* player = nullptr;
	SceneGame* sceneGame = nullptr;

	float statMultiplier = 1.f;

public:
	Zombie(const std::string& name = "");
	~Zombie() = default;

	void SetPosition(const sf::Vector2f& pos) override;
	void SetRotation(float angle) override;
	void SetScale(const sf::Vector2f& scale) override;

	void SetOrigin(Origins preset) override;
	void SetOrigin(const sf::Vector2f& newOrigin) override;

	sf::FloatRect GetLocalBounds() override;
	sf::FloatRect GetGlobalBounds() override;

	sf::RectangleShape& GetDebugBox() override;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	void SetType(Types type);
	Types GetType() { return types; }

	int Attack();

	void TurnDebugBox(bool active, sf::Color color = sf::Color::Green) override;

	int GetZombieHp();

	void OnDamage(int dmg_in);

	void SetStatMultiplier(int wave);
};
