#pragma once

class SceneGame;
class Zombie;

class Bullet : public GameObject
{
protected:
	sf::Sprite body;
	std::string textureId = "graphics/bullet.png";

	sf::Vector2f direction;
	float speed = 0.f;
	int damage = 0.f;

	float activeDuration = 4.f;

	SceneGame* sceneGame;

public:
	Bullet(const std::string& name = "");
	~Bullet() = default;

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

	void Fire(const sf::Vector2f& initPos, const sf::Vector2f& dir, float s, int d);
	void TurnDebugBox(bool active, sf::Color color = sf::Color::Green) override;
	
	int GetBulletDmg() { return damage; }
};
