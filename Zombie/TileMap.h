#pragma once
#include "GameObject.h"

class TileMap : public GameObject
{
protected:
	sf::VertexArray va; // Defines set of one or more 2D primitives.
	std::string spriteSheetId = "graphics/background_sheet.png";
	sf::Texture* texture = nullptr;
	sf::Transform transform;

	sf::Vector2i cellCount; // ���μ��� Ÿ�� ���� ����
	sf::Vector2f cellSize = { 50.f,50.f }; // �� Ÿ�ϴ� ������

public:
	TileMap(const std::string& name = "");
	~TileMap() = default;

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
	void Draw(sf::RenderWindow& window) override;

	sf::Vector2f GetCellSize() { return cellSize; }

	void Set(const sf::Vector2i count_in, const sf::Vector2f size_in = {50.f,50.f});
	void UpdateTransform(); // ���� �ʿ��
};
