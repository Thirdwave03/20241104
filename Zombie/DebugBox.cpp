#include "stdafx.h"
#include "DebugBox.h"

DebugBox::DebugBox()
{
	shape.setFillColor(sf::Color::Transparent);
	shape.setOutlineColor(sf::Color::Green);
	shape.setOutlineThickness(1.f);
}

void DebugBox::SetBounds(const sf::FloatRect& bounds)
{
	shape.setSize({ bounds.width, bounds.height });
	shape.setPosition(bounds.left, bounds.top);
	transformShape = sf::Transform::Identity; // Identity : 기본행렬
}

void DebugBox::SetBounds(const sf::FloatRect& bounds, const sf::Transform& transform)
{
	SetBounds(bounds);
	transformShape = transform;
}

void DebugBox::SetDirection(const sf::Vector2f direction)
{
	this->direction = direction;
	shape.setRotation(Utils::Angle(this->direction));
}

void DebugBox::Draw(sf::RenderWindow& window)
{
	window.draw(shape);
	//window.draw(shape,transformShape); // 2번째 매개변수(transform or renderstate 변환 쳐서 그려라)
}

void DebugBox::SetDebugBoxColor(sf::Color color)
{
	shape.setOutlineColor(color);
}
