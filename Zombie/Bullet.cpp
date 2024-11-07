#include "stdafx.h"
#include "Bullet.h"
#include "SceneGame.h"
#include "Zombie.h"
#include "DebugBox.h"

Bullet::Bullet(const std::string& name)
	: GameObject(name)
{

}

void Bullet::SetPosition(const sf::Vector2f& pos)
{
	position = pos;
	body.setPosition(position);
}

void Bullet::SetRotation(float angle)
{
	rotation = angle;
	body.setRotation(rotation);
}

void Bullet::SetScale(const sf::Vector2f& s)
{
	scale = s;
	body.setScale(scale);
}

void Bullet::SetOrigin(Origins preset)
{
	originPreset = preset;
	if (originPreset != Origins::Custom)
	{
		origin = Utils::SetOrigin(body, originPreset);
	}
}

void Bullet::SetOrigin(const sf::Vector2f& newOrigin)
{
	originPreset = Origins::Custom;
	origin = newOrigin;
	body.setOrigin(origin);
}

sf::FloatRect Bullet::GetLocalBounds()
{
	return body.getLocalBounds();
}

sf::FloatRect Bullet::GetGlobalBounds()
{
	return body.getGlobalBounds();
}

void Bullet::Init()
{
	sortingLayer = SortingLayers::Foreground;
	sortingOrder = 1;
}

void Bullet::Release()
{
}

void Bullet::Reset()
{
	body.setTexture(TEXTURE_MGR.Get(textureId));
	sceneGame = dynamic_cast<SceneGame*>(SCENE_MGR.GetCurrentScene());
	SetOrigin(Origins::ML);

	SetPosition({ 0.f,0.f });
	SetRotation(0.f);
	SetScale({ 1.f,1.f });

	direction = { 0.f,0.f };
	speed = 0;
	damage = 0;
}

void Bullet::Update(float dt)
{
	debugBox.SetVisible((SceneGame*)(SCENE_MGR.GetCurrentScene())->GetDebugMode());
	SetPosition(position + direction * speed * dt);
	activeDuration -= dt;
	if (activeDuration < 0)
		SetActive(false);

	debugBox.GetShape().setSize({ body.getLocalBounds().width, body.getLocalBounds().height });
	debugBox.GetShape().setOrigin(body.getOrigin());
	debugBox.GetShape().setRotation(body.getRotation());
	debugBox.GetShape().setScale(body.getScale());
	debugBox.GetShape().setPosition(body.getPosition());
}

void Bullet::FixedUpdate(float dt)
{
	if (sceneGame == nullptr)
		return;

	const auto& list = sceneGame->GetZombieList();
	for (auto zombie : list)
	{
		if (!zombie->IsActive())
			continue;

		sf::RectangleShape& zombieBox = zombie->GetDebugBox();

		if (Utils::CheckCollision(debugBox.GetShape(), zombieBox))
		{
			SOUND_MGR.PlaySfx("sound/hit.wav");
			zombie->OnDamage(damage);
			sceneGame->OnBulletHit(this);
			break;
		}
	}
}

void Bullet::Draw(sf::RenderWindow& window)
{
	window.draw(body);
	if (debugBox.IsVisible())
		debugBox.Draw(window);
}

void Bullet::Fire(const sf::Vector2f& initPos, const sf::Vector2f& dir, float s, int d)
{
	SetPosition(initPos);
	direction = dir;
	speed = s;
	damage = d;

	SetRotation(Utils::Angle(direction));
}

void Bullet::TurnDebugBox(bool active, sf::Color color)
{
	debugBox.SetVisible(active);
	if (active)
	{
		debugBox.SetDebugBoxColor(color);
	}
}
