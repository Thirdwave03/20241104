#pragma once

class DebugBox
{
protected:
	sf::RectangleShape shape; // transformable 상속받음
	sf::Transform transformShape;

	//sf::RenderStates test // 얘가 Vertex 대신 texture를 가지고 Allign해준다

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