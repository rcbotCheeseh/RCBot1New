#include "rcbot_hldm.h"

void RCBotHLDM::spawnInit()
{

}

bool RCBotHLDM::isEnemy(edict_t* pEdict)
{
	if (pEdict == m_pEdict)
		return false;

	if (pEdict->v.flags == FL_CLIENT)
	{
		return !gpGlobals->teamplay || (pEdict->v.team != m_pEdict->v.team);
	}

	return false;
}