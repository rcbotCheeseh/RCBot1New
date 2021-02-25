#ifndef __RCBOT_WEAPONS_H__
#define __RCBOT_WEAPONS_H__

#include "rcbot_base.h"
#include <stdint.h>

class RCBotWeapon
{
public:
	RCBotWeapon();

	bool Select(RCBotBase* pBot);

private:
	const char* m_szClassname;

	uint32_t m_iSelectIndex;

	float m_fPrimaryMinDistance;
	float m_fPrimaryMaxDistance;

	float m_fSecondaryMinDistance;
	float m_fSecondaryMaxDistance;

	bool PrimaryAttack;
	bool SecondaryAttack;
};

class RCBotWeapons
{

};


#endif 
