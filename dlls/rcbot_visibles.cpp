#include "rcbot_visibles.h"
#include "extdll.h"
#include "h_export_meta.h"
#include "meta_api.h"
#include "rcbot_utils.h"
#include "rcbot_base.h"
#include <memory.h>

// engine functions
extern DLL_FUNCTIONS gFunctionTable;

RCBotVisibles::RCBotVisibles(RCBotBase* pBot)
{
	m_bVisibleBool = new bool[gpGlobals->maxEntities];
	memset(m_bVisibleBool, 0, sizeof(bool) * gpGlobals->maxEntities);
	m_pBot = pBot;
	m_iCurrentIndex = 0;
}

RCBotVisibles::~RCBotVisibles()
{
	delete[] m_bVisibleBool;
}
/// <summary>
/// does an actual POV / multi traceline check
/// </summary>
/// <param name="pEdict"></param>
/// <returns></returns>
bool RCBotVisibles::checkVisible(edict_t* pEnt)
{
	Vector vOrigin;
	// invalid entity
	if (pEnt == nullptr)
		return false;
	// not being used by engine
	if (pEnt->free) 
		return false;
	// not being drawn by engine
	if (pEnt->v.effects & EF_NODRAW)
		return false; 

	vOrigin = RCBotUtils::entityOrigin(pEnt);
	// behind me or something
	if (!m_pBot->inViewCone(vOrigin))
		return false; 
	
	pvs = ENGINE_SET_PVS(reinterpret_cast<float*>(&vOrigin));
	// not in view range
	if (!ENGINE_CHECK_VISIBILITY(m_pEdict, pvs))
		return false;
	
	// Finally do a traceline (expensive part)
	TraceResult *tr = RCBotUtils::Traceline(m_pBot->getViewOrigin(), vOrigin, ignore_monsters, ignore_glass, m_pEdict);

	return (tr->pHit == pEnt || tr->flFraction >= 1.0);
}

void RCBotVisibles::tasks(int iNumThisFrame)
{

	pvs = nullptr;
	pas = nullptr;

	m_pEdict = m_pBot->getEdict();

	// Setup the bots visibility, for quick vis checking
	(*gFunctionTable.pfnSetupVisibility)(nullptr, m_pEdict, &pvs, &pas);

	while (iNumThisFrame--)
	{
		edict_t* pEnt = INDEXENT(m_iCurrentIndex);
		bool isVisible = checkVisible(pEnt);

		if (!FNullEnt(pEnt))
		{
			if (isVisible)
			{
				if (!m_bVisibleBool[m_iCurrentIndex])
				{
					// Gained visibility
					m_pBot->newVisible(pEnt);
				}
			}
			else
			{
				if (m_bVisibleBool[m_iCurrentIndex])
				{
					// Lost visibility
					m_pBot->lostVisible(pEnt);
				}
			}
		}

		m_bVisibleBool[m_iCurrentIndex] = isVisible;

		m_iCurrentIndex++;

		if (m_iCurrentIndex > gpGlobals->maxEntities)
			m_iCurrentIndex = 1;

	}
}