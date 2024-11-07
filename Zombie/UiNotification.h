#pragma once
#include "GameObject.h"
class UiNotification :
	public GameObject
{
protected:
	sf::Text textCenterMsg;

	float opacityController;
	float opacityDuration = 0.8f;

	float displayTimer = 0;

public:
	UiNotification(const std::string& name = "");
	~UiNotification() = default;

	void SetPosition(const sf::Vector2f& pos) override;
	void SetRotation(float angle) override;
	void SetScale(const sf::Vector2f& scale) override;

	void SetOrigin(Origins preset) override;
	void SetOrigin(const sf::Vector2f& newOrigin) override;

	void Init() override;
	void Release() override;
	void Reset() override;
	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;

	void IndicaterReloading();
	void IndicaterNoBullet();
	void IndicaterNoSpareBullet();
};
