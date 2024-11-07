#pragma once

class Player;

class UserInterface : public GameObject
{
	friend class SceneGame;  // �������� ���庸��
private:
	UserInterface(const std::string& name = "");

protected:
	sf::RectangleShape maxHpBar;
	sf::RectangleShape HpBar;

	Player* player = nullptr;

public:
	~UserInterface() = default;

	void Init();
	void Release();
	void Reset();
	void Update(float dt);
	void Draw(sf::RenderWindow& window);

	void SetPlayer(Player* player);
};