#include "stdafx.h"
#include "SceneGame.h"
#include "Player.h"
#include "Bullet.h"
#include "Zombie.h"
#include "Item.h"
#include "TileMap.h"
#include "DebugBox.h"
#include "UserInterface.h"
#include "UiHud.h"
#include "UiUpgrade.h"
#include "UiGameOver.h"
#include "ZombieDieEffect.h"

SceneGame::SceneGame()
	:Scene(SceneIds::Game)
{
}

void SceneGame::Init()
{
	player = AddGo(new Player("graphics/player.png", "Player"));
	tileMap = AddGo(new TileMap());
	//ui = AddGo(new UserInterface());
	//ui->SetPlayer(player);
	uiHud = AddGo(new UiHud());
	uiUpgrade = AddGo(new UiUpgrade("UiUpgrade"));
	uiUpgrade->SetActive(false);
	uiGameOver = AddGo(new UiGameOver("UiGameOver"));
	uiGameOver->SetActive(false);

	Scene::Init();
}

void SceneGame::Release()
{
	Scene::Release();
}

void SceneGame::Enter()
{
	isDead = false;
	FRAMEWORK.SetTimeScale(1.f);
	SOUND_MGR.StopBgm();
	SOUND_MGR.PlayBgm("sound/deadByDaylight.mp3");
	SOUND_MGR.SetBgmVolume(50.f);

	FRAMEWORK.GetWindow().setMouseCursorVisible(false);
	cursor.setTexture(TEXTURE_MGR.Get("graphics/crosshair.png"));
	Utils::SetOrigin(cursor, Origins::MC);

	sf::Vector2f size = FRAMEWORK.GetWindowSizeF();

	worldView.setSize(size);
	worldView.setCenter(0.f, 0.f);

	uiView.setSize(size);
	uiView.setCenter(size.x * 0.5f, size.y * 0.5f); // 월드뷰의 절반사이즈 ( 왜? )

	FONT_MGR.Load("fonts/DS-DIGI.ttf");

	Scene::Enter();
}

void SceneGame::Exit()
{
	SOUND_MGR.StopBgm();
	for (auto zombie : zombies)
	{
		RemoveGo(zombie);
		zombiePool.Return(zombie);
	}
	zombies.clear();

	for (auto bullet : bullets)
	{
		RemoveGo(bullet);
		bulletPool.Return(bullet);
	}
	bullets.clear();

	for (auto item : items)
	{
		RemoveGo(item);
		itemPool.Return(item);
	}
	items.clear();

	for (auto effect : zombieDieEffects)
	{
		RemoveGo(effect);
		zombieDieEffectPool.Return(effect);
	}
	zombieDieEffects.clear();

	FONT_MGR.Unload("fonts/DS-DIGI.ttf");

	Scene::Exit();
}

void SceneGame::Update(float dt)
{
	skipPreventer = false;
	sf::Vector2f mPos = ScreenToUi(InputMgr::GetMousePosition());
	cursor.setPosition(mPos);

	/*worldView.setSize(FRAMEWORK.GetWindowSizeF());*/

	if (isDead)
	{
		uiGameOver->SetActive(true);
		FRAMEWORK.SetTimeScale(0.f);
		if (InputMgr::GetKeyDown(sf::Keyboard::Enter))
		{
			uiGameOver->SetActive(false);
			SCENE_MGR.ChangeScene(SceneIds::Game);
		}
	}
	if (isUpgrading)
	{
		return;
	}
	uiUpgrade->SetActive(false);

	itemSpawnTimer += dt;
	zombieSpawnTimer += dt;

	Scene::Update(dt);

	if (InputMgr::GetKeyDown(sf::Keyboard::Grave))
	{
		SCENE_MGR.ChangeScene(SceneIds::Game);
	}

	if (InputMgr::GetKeyDown(sf::Keyboard::Num1))
	{
		SpawnZombies(10);
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::Num2))
	{
		SpawnItem(1);
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::Num9))
	{
		this->TurnDebugBox(true);
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::Num0))
	{
		this->TurnDebugBox(false);
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::Num6))
	{
		uiUpgrade->SetActive(!uiUpgrade->IsActive());
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::Num7))
	{
		uiGameOver->SetActive(!uiGameOver->IsActive());
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::R))
	{
		player->CallReload();
	}

	if (zombieSpawnTimer > zombieSpawnDuration)
	{
		zombieSpawnTimer -= zombieSpawnDuration;
		if (player->GetSpawnCnt() > 0)
		{
			SpawnZombies(5);
			player->SetSpawnCnt(player->GetSpawnCnt() - 1);
			skipPreventer = true;
		}
	}
	if (itemSpawnTimer > itemSpawnDuration)
	{
		itemSpawnTimer -= itemSpawnDuration;
		SpawnItem(1);
	}

	if (player != nullptr)
	{
		worldView.setCenter(player->GetPosition());
	}

	auto it = zombieDieEffects.begin();
	while (it != zombieDieEffects.end())
	{
		if (!(*it)->IsActive())
		{
			zombieDieEffectPool.Return(*it);
			RemoveGo(*it);
			it = zombieDieEffects.erase(it);
		}
		else
		{
			it++;
		}
	}
	if (player->GetPlayerHp() <= 0)
		isDead = true;

	if (player->GetSpawnCnt() + player->GetZombieCount() == 0)
	{
		if (!skipPreventer)
		{
			isUpgrading = true;
			uiUpgrade->SetActive(true);
			FRAMEWORK.SetTimeScale(0.f);
		}
	}
}

void SceneGame::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);

	const sf::View& saveveiw = window.getView();

	window.draw(cursor);
}

void SceneGame::SpawnZombies(int count)
{
	for (int i = 0; i < count; i++)
	{
		Zombie* zombie = zombiePool.Take();
		zombies.push_back(zombie);

		Zombie::Types zombieType = (Zombie::Types)Utils::RandomRange(0, Zombie::TotalTypes -1);
		zombie->SetType(zombieType);

		sf::Vector2f pos;
		pos.x = Utils::RandomRange(tileMap->GetGlobalBounds().left, 
			tileMap->GetGlobalBounds().left +tileMap->GetGlobalBounds().width);
		pos.y = Utils::RandomRange(tileMap->GetGlobalBounds().top, 
			tileMap->GetGlobalBounds().top + tileMap->GetGlobalBounds().height);
		zombie->SetPosition(pos);

		AddGo(zombie);
	}
}

void SceneGame::SpawnItem(int count)
{
	for (int i = 0; i < count; i++)
	{
		Item* item = itemPool.Take();
		items.push_back(item);

		Item::ItemTypes itemType = (Item::ItemTypes)Utils::RandomRange(0, Item::TotalItemTypes -1);
		item->SetType(itemType);

		sf::Vector2f pos;
		pos.x = Utils::RandomRange(tileMap->GetGlobalBounds().left,
			tileMap->GetGlobalBounds().left + tileMap->GetGlobalBounds().width);
		pos.y = Utils::RandomRange(tileMap->GetGlobalBounds().top,
			tileMap->GetGlobalBounds().top + tileMap->GetGlobalBounds().height);
		item->SetPosition(pos);

		AddGo(item);
	}
}

Bullet* SceneGame::TakeBullet()
{
	Bullet* bullet = bulletPool.Take();
	bullets.push_back(bullet);
	AddGo(bullet);
	return bullet;
}

void SceneGame::ReturnBullet(Bullet* bullet)
{
	RemoveGo(bullet);
	bulletPool.Return(bullet);
	bullets.remove(bullet);
}

void SceneGame::OnZombieDie(Zombie* zombie)
{
	SOUND_MGR.PlaySfx("sound/splat.wav");
	ZombieDieEffect* dieEffect = zombieDieEffectPool.Take();
	AddGo(dieEffect);
	dieEffect->SetOrigin(Origins::MC);
	dieEffect->SetPosition(zombie->GetPosition());
	zombieDieEffects.push_back(dieEffect);
	if (zombie->GetName() == "Bloater")
		dieEffect->SetScale({ 1.6f,1.6f });
	RemoveGo(zombie);
	zombiePool.Return(zombie);
	zombies.remove(zombie);
}

void SceneGame::OnBulletHit(Bullet* bullet)
{
	RemoveGo(bullet);
	bulletPool.Return(bullet);
	bullets.remove(bullet);
}

void SceneGame::OnItemPickedUp(Item* item)
{
	RemoveGo(item);
	itemPool.Return(item);
	items.remove(item);
}

void SceneGame::OnUpgrade(UpgradeTypes upgrade)
{

}

void SceneGame::SetUpgrading(bool isUpgrading)
{
	this->isUpgrading = isUpgrading;
	if (!(this->isUpgrading))
	{
		player->SetWave(player->GetWave() + 1);
		player->SetSpawnCnt(player->GetWave());
		FRAMEWORK.SetTimeScale(1.f);
	}
}
