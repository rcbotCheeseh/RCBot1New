#ifndef __RCBOT_MANAGER_H__
#define __RCBOT_MANAGER_H__

#include <vector>
#include <stdint.h>
#include "rcbot_base.h"

#define BOT_MANAGER_DEFAULT_ADD_REMOVE_BOT_PERIOD 5.0f

class RCBotManager
{
public:
	~RCBotManager();
	RCBotManager();
	void Think();

	void KickBot();
	void LevelInit();
	void OnLevelChange();
	bool SetQuota(uint8_t iQuota);
	void IncreaseQuota()
	{
		m_iQuota++;
	}

	RCBotBase* getBotByEdict(edict_t* pEdict);

private:
	RCBotBase* AddBot();

	std::vector<RCBotBase*> m_Bots;
	uint8_t m_iQuota;
	float m_fAddRemoveBotTime;
	float m_fNodeDrawTime;
};

extern RCBotManager gRCBotManager;


#endif 