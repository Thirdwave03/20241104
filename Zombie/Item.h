#pragma once

class Item : public GameObject
{
public:
	enum class ItemTypes
	{
		Ammo,
		Health
	};

	static const int TotalItemTypes = 2;

protected:
	ItemTypes iTypes = ItemTypes::Ammo;
	sf::Sprite body;
	std::string textureId;

	int addAmmo = 0;
	int addHp = 0;

	float spawnInterval = 5.f;
	float spawnTimer = 0.f;

public:
	Item(const std::string& name = "");
	~Item() = default;

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

	void TurnDebugBox(bool active, sf::Color color = sf::Color::Blue) override;

	void SetType(ItemTypes type);

	sf::Vector2i PickedUp();
};
