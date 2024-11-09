#include "stdafx.h"
#include "SaveMgr.h"


bool SaveMgr::LoadHighScore()
{
	std::ifstream loadHighScoreTxtFile(highScorefileDirectory, std::ios::in);

	if (!loadHighScoreTxtFile)
	{
		std::cout << "Failed to load highscore.txt file" << std::endl;
		return false;
	}

	std::string tempString;

	loadHighScoreTxtFile >> tempString;

	savedHighscore = std::stoi(tempString);

	loadHighScoreTxtFile.close();
	return true;
}

bool SaveMgr::ResetHighScore(int score_in)
{
	std::ofstream overwriteHighScoreTxtFile(highScorefileDirectory, std::ios::out);

	if (!overwriteHighScoreTxtFile)
	{
		std::cout << "Failed to find highscore.txt file" << std::endl;
		return false;
	}
	
	SetHighScoreFromGame(score_in);
	overwriteHighScoreTxtFile << savedHighscore;
	overwriteHighScoreTxtFile.close();
	return true;
}

int SaveMgr::GetSavedHighScore()
{
	if (LoadHighScore())
		return savedHighscore;
	else
		return 0;
}

void SaveMgr::SetHighScoreFromGame(int inGameScore)
{
	savedHighscore = inGameScore;
}

bool SaveMgr::LoadUpgradeCnt()
{
	std::ifstream loadUpgradeTxtFile(upgradeCntFileDirectory, std::ios::in);

	if (!loadUpgradeTxtFile)
	{
		std::cout << "Failed to load upgradeCnt.txt file" << std::endl;
		return false;
	}

	std::string tempStr[7];

	for (int i = 0; i < 7; i++)
	{
		loadUpgradeTxtFile >> tempStr[i];
		savedUpgradeCnt[i] = std::stoi(tempStr[i]);
	}

	loadUpgradeTxtFile.close();
	return true;
}

bool SaveMgr::SaveUpgradeCnt()
{	
	std::ofstream overwriteUpgradeTxtFile(upgradeCntFileDirectory, std::ios::out);

	if (!overwriteUpgradeTxtFile)
	{
		std::cout << "Failed to find upgradeCnt.txt file" << std::endl;
		return false;
	}
	
	for (int i = 0; i < 7; i++)
	{
		overwriteUpgradeTxtFile << savedUpgradeCnt[i] << std::endl;
	}

	overwriteUpgradeTxtFile.close();
	return true;
}

int SaveMgr::GetSavedUpgradeCnt(int index)
{
	if (LoadUpgradeCnt())
		return savedUpgradeCnt[index];
	else
		return 0;
}

void SaveMgr::SetUpgradeCntFromGame(int index, int value)
{
	savedUpgradeCnt[index] = value;
}

void SaveMgr::SetUpgradeCntFromGame(int* inGameCntArr)
{
	*savedUpgradeCnt = *inGameCntArr;
}
