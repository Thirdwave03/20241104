#pragma once
#include "GameObject.h"

class Player;

class UiUpgrade :
    public GameObject
{
	enum class UpgradeTypes
	{
		FireRate,
		ClipSize,
		MaxHp,
		HpPickup,
		AmmoPickup,
		Speed,
	};

protected:
	static const int noOfUpgrades = 6;

	UpgradeTypes ugTypes = UpgradeTypes::FireRate;
	
	sf::Text textUpgrades[noOfUpgrades];
	sf::Sprite background;

	Player* player;

	bool mouseLocated[noOfUpgrades] = { false };

public:
	UiUpgrade(const std::string& name = "");
	~UiUpgrade() = default;

	void SetPosition(const sf::Vector2f& pos) override;
	void SetRotation(float angle) override;
	void SetScale(const sf::Vector2f& scale) override;

	void SetOrigin(Origins preset) override;
	void SetOrigin(const sf::Vector2f& newOrigin) override;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void FixedUpdate(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	void SetUpgradeText();

	void SetUpgrade(UpgradeTypes ugType);
};

