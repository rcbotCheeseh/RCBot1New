#ifndef __RCBOT_HLDM_H__
#define __RCBOT_HLDM_H__

#include "rcbot_base.h"

class RCBotHLDM : public RCBotBase
{
	void spawnInit();

	bool isEnemy(edict_t* pEdict);
};

#endif 