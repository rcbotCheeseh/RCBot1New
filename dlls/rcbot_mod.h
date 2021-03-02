#ifndef __RCBOT_MODIFICATION_H__
#define __RCBOT_MODIFICATION_H__

#include "rcbot_base.h"
#include <vector>

#define MODIFICATION_GAME_FOLDER_LEN_MAX 256

class RCBotModification
{
public:
	RCBotModification(const char *gameFolder, const char *szName);

	virtual RCBotBase* createBot() = 0;

	bool isModification(const char* gameFolder);

	virtual void GameInit();

	const char* getName() const
	{
		return m_szName;
	}

private:
	const char* m_szName;
	const char* m_szGameFolder;
};

class RCBotModifications
{
public:
	RCBotModifications();
	~RCBotModifications();

	RCBotModification* getCurrentMod()
	{
		return m_pCurrentMod;
	}

	void GameInit ();
private:
	RCBotModification* m_pCurrentMod;
	std::vector< RCBotModification*> m_Mods;
};

class RCBotModification_TheSpecialists : public RCBotModification
{
public:
	RCBotModification_TheSpecialists() : RCBotModification("ts","The Specialists")
	{

	}

	void GameInit();

	RCBotBase* createBot();
private:
};


class RCBotModification_HLDM : public RCBotModification
{
public:
	RCBotModification_HLDM() : RCBotModification("valve","Half-life: Deathmatch")
	{

	}

	void GameInit();

	RCBotBase* createBot();
private:
};



extern RCBotModifications gRCBotModifications;

#endif 
