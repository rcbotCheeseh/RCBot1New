#ifndef __RCBOT_VISIBLES_H__
#define __RCBOT_VISIBLES_H__

#include "extdll.h"
#include <vector>

class RCBotBase;

#define RCBOT_VISIBLES_MAX 1024

class RCBotVisibles
{
public:

	RCBotVisibles(RCBotBase* pBot);
	/// <summary>
	/// does an actual POV / multi traceline check
	/// </summary>
	/// <param name="pEdict"></param>
	/// <returns></returns>
	bool checkVisible(edict_t* pEdict);

	void tasks (int iNumThisFrame);

private:
	/// <summary>
	/// The bot we're updating visibles for
	/// </summary>
	RCBotBase* m_pBot;
	/// <summary>
	/// list of visible objects
	/// </summary>
	std::vector<edict_t*> m_VisibleList;
	/// <summary>
	/// Let's just use a boolean array - memory not an issue - let's focus on speed
	/// </summary>
	bool m_bVisibleBool[RCBOT_VISIBLES_MAX];
	/// <summary>
	/// current entity index being updated
	/// </summary>
	uint16_t m_iCurrentIndex;
	unsigned char* pvs;
	unsigned char* pas;

	edict_t* m_pEdict; // the bot edict
};

#endif
