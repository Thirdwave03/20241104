#pragma once

class DebugBox
{
protected:
	sf::RectangleShape shape; // transformable ��ӹ���
	sf::Transform transformShape;

	//sf::RenderStates test // �갡 Vertex ��� texture�� ������ Allign���ش�

	bool isVisible = false;

	sf::Vector2f direction;

public:
	DebugBox();

	void SetBounds(const sf::FloatRect& bounds);
	void SetBounds(const sf::FloatRect& bounds, const sf::Transform& transform);
	
	void SetDirection(const sf::Vector2f direction);
	
	void SetVisible(bool v) { isVisible = v; }
	bool IsVisible() const { return isVisible; }

	sf::RectangleShape& GetShape() { return shape; }

	void Draw(sf::RenderWindow& window);

	void SetDebugBoxColor(sf::Color color);
};