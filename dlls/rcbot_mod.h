#ifndef __RCBOT_MODIFICATION_H__
#define __RCBOT_MODIFICATION_H__

#include "rcbot_base.h"
#include <vector>

#define MODIFICATION_GAME_FOLDER_LEN_MAX 256

class RCBotModification
{
public:
	RCBotModification(const char *gameFolder);

	virtual RCBotBase* createBot() = 0;

	bool isModification(const char* gameFolder);

	void setUpBotClientInfo (edict_t* pEdict);

	void putClientInServer(edict_t* pEdict);

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

extern RCBotModifications gRCBotModifications;

class RCBotModification_TheSpecialists : public RCBotModification
{
public:
	RCBotModification_TheSpecialists() : RCBotModification("ts")
	{

	}

	RCBotBase* createBot();
private:
};


#endif 
