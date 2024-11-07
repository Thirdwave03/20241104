#include "stdafx.h"
#include "TileMap.h"

TileMap::TileMap(const std::string& name)
{
}

void TileMap::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	transform.translate(position);
}

void TileMap::SetRotation(float angle)
{
	rotation = angle;
	transform.rotate(rotation);
}

void TileMap::SetScale(const sf::Vector2f& s)
{
	scale = s;
	UpdateTransform();
}

void TileMap::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{
		sf::FloatRect rect = GetLocalBounds();		
		origin.x = rect.width * ((int)preset % 3) * 0.5f;
		origin.x = rect.height * ((int)preset / 3) * 0.5f;		
	}
	UpdateTransform();
}

void TileMap::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	UpdateTransform();
}

sf::FloatRect TileMap::GetLocalBounds()
{
	return { 0.f,0.f, cellCount.x * cellSize.x, cellCount.y * cellSize.y };
}

sf::FloatRect TileMap::GetGlobalBounds()
{
	sf::FloatRect bounds = GetLocalBounds();
	return transform.transformRect(bounds);
}

void TileMap::Init()
{
	sortingLayer = SortingLayers::Background;
	sortingOrder = -1;

	Set({ 20, 20 }, { 50.f, 50.f});
	SetOrigin(Origins::MC);
	SetScale({ 1.f,1.f });	
}

void TileMap::Release()
{
}

void TileMap::Reset()
{
	texture = &TEXTURE_MGR.Get(spriteSheetId);

	SetOrigin(Origins::MC);
	SetPosition({ 0.f, 0.f });	
}

void TileMap::Update(float dt)
{

}

void TileMap::Draw(sf::RenderWindow& window)
{
	sf::RenderStates state;
	state.texture = texture;
	state.transform = transform;
	window.draw(va, state);
}

void TileMap::Set(const sf::Vector2i count_in, const sf::Vector2f size_in)
{
	cellCount = count_in;
	cellSize = size_in;

	va.clear();
	va.setPrimitiveType(sf::Quads);
	va.resize(count_in.x * count_in.y * 4); 

	sf::Vector2f posOffset[4] =
	{
		{0.f,0.f},
		{size_in.x , 0.f},
		{size_in.x , size_in.y},
		{0.f , size_in.y}
	};

	sf::Vector2f texCoords[4] =
	{
		{0.f,0.f},
		{50.f,0.f},
		{50.f,50.f},
		{0.f,50.f}
	};

	for (int i = 0; i < count_in.y; i++)
	{
		for (int j = 0; j < count_in.x; j++)
		{
			int texIndex = Utils::RandomRange(0, 2);
			if (i == 0 || i == count_in.y - 1 || j == 0 || j == count_in.x - 1)
			{
				texIndex = 3;
			}

			int quadIndex = i * count_in.x + j;
			sf::Vector2f quadPos = { j * size_in.x, i * size_in.y };
			for (int k = 0; k < 4; k++)
			{
				int vertexIndex = quadIndex * 4 + k;
				va[vertexIndex].position = quadPos + posOffset[k];
				va[vertexIndex].texCoords = texCoords[k];
				va[vertexIndex].texCoords.y += texIndex * 50.f;
			}			
		}
	}
}

void TileMap::UpdateTransform()
{
	transform = sf::Transform::Identity;
	transform.translate(position);
	transform.rotate(rotation);
	transform.scale(scale);
	transform.translate(-origin);
}
