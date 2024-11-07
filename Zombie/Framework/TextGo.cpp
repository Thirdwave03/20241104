#include "stdafx.h"
#include "TextGo.h"

TextGo::TextGo()
	: GameObject(""), fontId("")
{
}

TextGo::TextGo(const std::string& fontId, const std::string& name)
	: GameObject(name), fontId(fontId)
{
}

void TextGo::SetOrigin(Origins preset)
{
	originPreset = preset;
	origin = Utils::SetOrigin(text, originPreset);
}

void TextGo::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
	text.setOrigin(origin);
}

void TextGo::Reset()
{
	text.setFont(FONT_MGR.Get(fontId));
	SetOrigin(originPreset);
}

void TextGo::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	text.setPosition(pos);
}

void TextGo::SetRotation(float angle)
{
	rotation = angle;
	text.setRotation(angle);
}

void TextGo::SetScale(const sf::Vector2f& s)
{
	scale = s;
	text.setScale(scale);
}

void TextGo::Draw(sf::RenderWindow& window)
{
	window.draw(text);
}

void TextGo::Set(int size, const sf::Color &color)
{
	text.setCharacterSize(size);
	text.setFillColor(color);
}

void TextGo::SetString(const std::string& str)
{
	text.setString(str);
}

void TextGo::Init()
{
}

void TextGo::Release()
{
}

void TextGo::Update(float dt)
{
}
