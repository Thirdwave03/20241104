#pragma once
#include "Singleton.h"

class SaveMgr : public Singleton<SaveMgr>
{
	friend Singleton<SaveMgr>;
protected:
	SaveMgr() = default;
	~SaveMgr() = default;
	SaveMgr(const SaveMgr& other) = default;
	SaveMgr& operator=(const SaveMgr& other) = default;

	std::string highScorefileDirectory = "savedData/shs.svfl";
	std::string upgradeCntFileDirectory = "savedData/suc.svfl";

	int savedHighscore;
	int savedUpgradeCnt[7] = { 0 };

public:
	bool LoadHighScore();
	bool ResetHighScore(int score_in);

	int GetSavedHighScore();
	void SetHighScoreFromGame(int inGameScore);

	bool LoadUpgradeCnt();
	bool SaveUpgradeCnt();

	int GetSavedUpgradeCnt(int index);
	void SetUpgradeCntFromGame(int index, int value);
	void SetUpgradeCntFromGame(int* inGameCntArr);
};

#define SAVE_MGR (SaveMgr::Instance())