#ifndef __RCBOT_WEAPONS_H__
#define __RCBOT_WEAPONS_H__

#include "rcbot_base.h"
#include <stdint.h>
#include <vector>

#define RCBOT_WEAPON_CONFIG_FOLDER "config"
#define RCBOT_WEAPON_CONFIG_NAME "weapons"

class RCBotWeaponInfo
{
public:
	RCBotWeaponInfo ();

	bool Select(RCBotBase* pBot);

	static RCBotWeaponInfo *Parse(std::vector<const char*> *split);

	inline bool CanUsePrimary(double distance, bool isUnderWater)
	{
		return (distance > m_fPrimaryMinDistance) && (distance < m_fPrimaryMaxDistance) && (!isUnderWater || m_bCanUseUnderWater);
	}
	inline bool CanUseSecondary(double distance, bool isUnderWater)
	{
		return (distance > m_fSecondaryMinDistance) && (distance < m_fSecondaryMaxDistance) && (!isUnderWater || m_bCanUseUnderWater);
	}
	inline int Priority()
	{
		return m_iPriority;
	}
	
private:
	const char* m_szClassname;

	uint32_t m_iSelectIndex;

	float m_fPrimaryMinDistance;
	float m_fPrimaryMaxDistance;

	float m_fSecondaryMinDistance;
	float m_fSecondaryMaxDistance;

	bool m_bPrimaryFire;
	bool m_bSecondaryFire;

	bool m_bIsExplosive;
	bool m_bIsProjectile;

	bool m_bCanUseUnderWater;

	uint8_t m_iPriority;
};

class RCBotWeaponInfoList
{
public:
	RCBotWeaponInfoList()
	{

	}

	void Load ( );


private:
	std::vector<RCBotWeaponInfo*> m_pWeaponInfo;
};

class RCBotWeapon
{
public:
	RCBotWeapon()
	{

	}

	inline bool CanUsePrimary(double distance, bool isUnderWater)
	{
		return m_pWeaponInfo->CanUsePrimary(distance, isUnderWater);
	}
	inline bool CanUseSecondary(double distance, bool isUnderWater)
	{
		return m_pWeaponInfo->CanUseSecondary(distance, isUnderWater);
	}
	inline int Priority()
	{
		return m_pWeaponInfo->Priority();
	}
private:
	/// <summary>
	/// containts the information about the weapon
	/// </summary>
	RCBotWeaponInfo* m_pWeaponInfo; 

	int m_iId;

	int m_iClip;

	// Extra ammo for this weapon in (used for TS only right now)
	int m_iReserve;

	// store pointers to ammo stored
	int* m_iAmmo1;
	int* m_iAmmo2;

	int m_iMaxClip;
	
};

class RCBotWeapons
{
public:
	RCBotWeapon* getBestWeapon(RCBotBase* pBot, edict_t* pEnemy);
private:
	std::vector<RCBotWeapon*> m_Weapons;
};

extern RCBotWeaponInfoList* gWeaponList;
#endif 
