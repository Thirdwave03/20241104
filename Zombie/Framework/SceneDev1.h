#pragma once
#include "Scene.h"

class SceneDev1 : public Scene
{
protected:
	sf::RectangleShape rect1;
	sf::RectangleShape rect2;


public:
	SceneDev1();
	~SceneDev1() = default;

	void Init() override;
	void Enter() override;
	void Exit() override;

	void Update(float dt) override;
	void Draw(sf::RenderWindow& window) override;
};

