#include "rcbot_hldm.h"
#include "rcbot_utils.h"

void RCBotHLDM::spawnInit()
{

}

bool RCBotHLDM::isEnemy(edict_t* pEdict)
{
	if (pEdict == m_pEdict)
		return false;

	if (pEdict->v.deadflag == DEAD_NO)
	{
		if (pEdict->v.flags & FL_CLIENT)
		{
			return !gpGlobals->teamplay || (pEdict->v.team != m_pEdict->v.team);
		}
	}

	return false;
}

void RCBotHLDM::Think()
{
	RCBotBase::Think();

	if (m_pEnemy.Get() != nullptr)
	{
		edict_t* pEnemy = m_pEnemy.Get();

		if (isEnemy(pEnemy))
		{
			// Enemy still alive and still an "enemy"
			Vector vEnemy = RCBotUtils::entityOrigin(pEnemy);

			setLookAt(vEnemy);
			setMoveTo(vEnemy);

			if (RANDOM_FLOAT(0.0, 100.0) > 75.0f)
				primaryAttack();
		}
		else
			m_pEnemy.Set(nullptr);
	}
}