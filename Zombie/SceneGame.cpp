#include "stdafx.h"
#include "SceneGame.h"
#include "Player.h"
#include "Bullet.h"
#include "Zombie.h"
#include "Item.h"
#include "TileMap.h"
#include "DebugBox.h"
#include "UiHud.h"
#include "UiUpgrade.h"
#include "UiGameOver.h"
#include "UiNotification.h"
#include "ZombieDieEffect.h"
#include "Indicator.h"

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
	uiNotification = AddGo(new UiNotification("UiNotification"));
	uiNotification->SetActive(false);

	player->SetHighScore(SAVE_MGR.GetSavedHighScore());
	for (int i = 0; i < 7; i++)
	{
		player->SetUpgradeCnt(i, SAVE_MGR.GetSavedUpgradeCnt(i));
	}
	itemSpawnSpeed += 5 * (player->GetUpgradeCnt(4) + player->GetUpgradeCnt(5));
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
	SOUND_MGR.PlayBgm("sound/deadByDaylight.mp3", true);
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

	for (auto indicator : indicators)
	{
		RemoveGo(indicator);
		indicatorPool.Return(indicator);
	}
	indicators.clear();

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
			player->ResetItemSpawnSpeed();
			uiGameOver->SetActive(false);
			SCENE_MGR.ChangeScene(SceneIds::Game);
		}
	}
	if (isUpgrading)
	{
		uiUpgrade->Update(FRAMEWORK.GetRealDeltaTime());
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
	if (InputMgr::GetKeyDown(sf::Keyboard::Num3))
	{
		SpawnEliteZombies(1);
	}
	if (InputMgr::GetKeyDown(sf::Keyboard::U))
	{
		player->SetPowerOverwhelming(true);
		uiNotification->SetActive(true);
		uiNotification->IndicaterNoBullet();
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
	if (InputMgr::GetKeyDown(sf::Keyboard::F1))
	{
		SAVE_MGR.SaveUpgradeCnt();
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
			eliteZombieSpawnCnt--;
			if (eliteZombieSpawnCnt == 0)
			{
				SpawnEliteZombies(1);
				eliteZombieSpawnCnt = player->GetWave()+1;
				if (player->GetWave() > 8)
					eliteZombieSpawnCnt = (player->GetWave()+1) / 2;
				if (player->GetWave() > 12)
					eliteZombieSpawnCnt = 3;
				if (player->GetWave() > 16)
					eliteZombieSpawnCnt = 2;
				if (player->GetWave() > 20)
					eliteZombieSpawnCnt = 1;
			}

			skipPreventer = true;
		}
	}
	if (itemSpawnTimer > itemSpawnDuration && items.size() < currentTileSize/75)
	{
		itemSpawnTimer = 0;
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
			player->StageClearReset();
			isUpgrading = true;
			uiUpgrade->SetClickBlocker(0.5);
			uiUpgrade->SetActive(true);
			ResizeTile();
			player->ResetMovableBounds();
			FRAMEWORK.SetTimeScale(0.f);
		}
	}
}

void SceneGame::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);

	const sf::View& saveview = window.getView();

	window.draw(cursor);
}

void SceneGame::SpawnZombies(int count)
{
	for (int i = 0; i < count; i++)
	{
		Zombie* zombie = zombiePool.Take();
		zombies.push_back(zombie);

		zombie->SetStatMultiplier(player->GetWave());
		Zombie::Types zombieType = (Zombie::Types)Utils::RandomRange(0, 2);
		zombie->SetType(zombieType);

		sf::Vector2f pos;
		pos.x = Utils::RandomRange(player->GetMovableBounds().left,
			player->GetMovableBounds().left + player->GetMovableBounds().width);
		pos.y = Utils::RandomRange(player->GetMovableBounds().top,
			player->GetMovableBounds().top + player->GetMovableBounds().height);
		zombie->SetPosition(pos);

		AddGo(zombie);
	}
}

void SceneGame::SpawnEliteZombies(int count)
{
	for (int i = 0; i < count; i++)
	{
		Zombie* zombie = zombiePool.Take();
		zombies.push_back(zombie);

		zombie->SetStatMultiplier(player->GetWave());
		Zombie::Types zombieType = (Zombie::Types)Utils::RandomRange(3, 5);
		zombie->SetType(zombieType);

		sf::Vector2f pos;
		pos.x = Utils::RandomRange(player->GetMovableBounds().left,
			player->GetMovableBounds().left + player->GetMovableBounds().width);
		pos.y = Utils::RandomRange(player->GetMovableBounds().top,
			player->GetMovableBounds().top + player->GetMovableBounds().height);
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

		Item::ItemTypes itemType = (Item::ItemTypes)Utils::Clamp(Utils::RandomRange(-1, Item::TotalItemTypes - 1), 0, 1);
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
	dieEffect->SetTimer();
	dieEffect->SetOrigin(Origins::MC);
	dieEffect->SetPosition(zombie->GetPosition());
	zombieDieEffects.push_back(dieEffect);
	if ((int)zombie->GetType() < 3)
		dieEffect->SetScale({ 1.f,1.f });
	if (zombie->GetName() == "Bloater")
		dieEffect->SetScale({ 1.6f,1.6f });
	if ((int)zombie->GetType() > 3)
		dieEffect->SetScale({ 2.f,2.f });
	if ((int)zombie->GetType() == 3)
		dieEffect->SetScale({ 3.2f, 3.2f });
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
		uiNotification->SetActive(false);
		auto it = items.begin();
		while (it != items.end())
		{
			RemoveGo(*it);
			itemPool.Return(*it);
			it = items.erase(it);
		}
	}
}

void SceneGame::CallUiNoBullet()
{
	uiNotification->SetActive(true);
	uiNotification->IndicaterNoBullet();
}

void SceneGame::CallUiReloading()
{
	uiNotification->SetActive(true);
	uiNotification->IndicaterReloading();
}

void SceneGame::CallUiNoSpare()
{
	uiNotification->SetActive(true);
	uiNotification->IndicaterNoSpareBullet();
}

void SceneGame::SetUiNotificationActive(bool active)
{
	uiNotification->SetActive(false);
}

void SceneGame::SetItemSpawnSpeed(int spawnSpeed)
{
	itemSpawnSpeed = spawnSpeed;
}

int SceneGame::GetItemSpawnSpeed()
{
	return itemSpawnSpeed;
}

void SceneGame::ViewController(sf::RenderWindow& window)
{
	const sf::View& saveview = window.getView();
}

void SceneGame::SetIndicator(int info, sf::Sprite sprite, sf::Color color)
{
	Indicator* indicator = indicatorPool.Take();
	AddGo(indicator);
	indicator->SetIndicator(info, sprite, color);
	indicators.push_back(indicator);
}

void SceneGame::ResizeTile()
{
	tileMap->Set({TileCntX(), TileCntY()});
	currentTileSize = TileCntX() * TileCntY();
}

int SceneGame::TileCntX()
{
	return 15 + (int)(player->GetWave() * 3 / 2);
}

int SceneGame::TileCntY()
{
	return 15 + (int)(player->GetWave());
}
