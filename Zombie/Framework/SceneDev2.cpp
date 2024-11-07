#include "stdafx.h"
#include "SceneDev2.h"

SceneDev2::SceneDev2() : Scene(SceneIds::Dev2)
{

}

void SceneDev2::Init()
{	
	auto text = AddGo(new TextGo("fonts/DS-DIGI.ttf", "Scene Name"));

	Scene::Init();

	text->sortingLayer = SortingLayers::UI;
	text->Set(25, sf::Color::White);
	text->SetString("Dev 2");
}

void SceneDev2::Enter()
{
	Scene::Enter();
}

void SceneDev2::Exit()
{
	Scene::Exit();
}

void SceneDev2::Update(float dt)
{
	Scene::Update(dt);

	if (InputMgr::GetKeyDown(sf::Keyboard::Space))
	{
		SCENE_MGR.ChangeScene(SceneIds::Dev1);
	}
}

void SceneDev2::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}
