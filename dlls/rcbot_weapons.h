#ifndef __RCBOT_WEAPONS_H__
#define __RCBOT_WEAPONS_H__

#include "rcbot_base.h"
#include <stdint.h>
#include <vector>

#define RCBOT_WEAPON_CONFIG_FOLDER "config"
#define RCBOT_WEAPON_CONFIG_NAME "weapons"

enum class RCBotWeaponFlags
{
	None = 0,
	PrimaryFire = 1,
	SecondaryFire = 2,
	Underwater = 4,
	Melee = 8,
	Explosive = 16,
	Projectile = 32,
	Grenade = 64
};


inline RCBotWeaponFlags operator & (RCBotWeaponFlags lhs, RCBotWeaponFlags rhs)
{
	using T = std::underlying_type_t <RCBotWeaponFlags>;
	return static_cast<RCBotWeaponFlags>(static_cast<T>(lhs) & static_cast<T>(rhs));
}


inline RCBotWeaponFlags operator | (RCBotWeaponFlags lhs, RCBotWeaponFlags rhs)
{
	using T = std::underlying_type_t <RCBotWeaponFlags>;
	return static_cast<RCBotWeaponFlags>(static_cast<T>(lhs) | static_cast<T>(rhs));
}

inline RCBotWeaponFlags& operator |= (RCBotWeaponFlags& lhs, RCBotWeaponFlags rhs)
{
	lhs = lhs | rhs;
	return lhs;
}

class RCBotWeaponInfo
{
public:
	RCBotWeaponInfo ();

	const char* getClassname()
	{
		return m_szClassname;
	}

	void setAmmo1(uint8_t iAmmo1, uint8_t iAmmo1Max)
	{
		m_iAmmo1 = iAmmo1;
		m_iAmmo1Max = iAmmo1Max;
	}

	void setAmmo2(uint8_t iAmmo2, uint8_t iAmmo2Max)
	{
		m_iAmmo2 = iAmmo2;
		m_iAmmo2Max = iAmmo2Max;
	}

	void setSlot(uint8_t iSlot, uint8_t iPosition)
	{
		m_iSlot = iSlot;
		m_iPosition = m_iPosition;
	}

	void setID(uint8_t iWeaponID)
	{
		m_iID = iWeaponID;
	}

	static RCBotWeaponInfo *Parse(std::vector<const char*> *split);

	//void setNonCustomisableData(uint32_t iID, uint8_t iAmmo1, uint8_t iAmmo2, uint8_t iAmmo1Max, uint8_t iAmmo2Max, uint32_t iSlot, uint32_t iPosition, uint32_t iFlags);

	inline bool CanUsePrimary(double distance, bool isUnderWater)
	{
		return hasFlags(RCBotWeaponFlags::PrimaryFire) && (distance > m_fPrimaryMinDistance) && (distance < m_fPrimaryMaxDistance) && (!isUnderWater || hasFlags(RCBotWeaponFlags::Underwater) );
	}
	inline bool CanUseSecondary(double distance, bool isUnderWater)
	{
		return hasFlags(RCBotWeaponFlags::SecondaryFire) && (distance > m_fSecondaryMinDistance) && (distance < m_fSecondaryMaxDistance) && (!isUnderWater || hasFlags(RCBotWeaponFlags::Underwater));
	}
	inline int Priority()
	{
		return m_iPriority;
	}
	bool isID(int id)
	{
		return m_iID == id;
	}


	inline bool hasFlags(RCBotWeaponFlags flags)
	{
		return (m_iFlags & flags) == flags;
	}

	inline bool HasWeapon(edict_t* pEdict)
	{
		return (pEdict->v.weapons & m_iID) == m_iID;
	}

	bool isClassname(const char* szClassname);
private:
	// non-customisable items
	const char* m_szClassname;

	int m_iAmmo1;
	int m_iAmmo2;
	int m_iAmmo1Max;
	int m_iAmmo2Max;
	int m_iSlot;
	int m_iPosition;
	//int m_iEngineFlags;

	// customisable items
	int m_iID;

	float m_fPrimaryMinDistance;
	float m_fPrimaryMaxDistance;

	float m_fSecondaryMinDistance;
	float m_fSecondaryMaxDistance;

	RCBotWeaponFlags m_iFlags;

	uint8_t m_iPriority;
};

class RCBotWeaponInfoList
{
public:
	RCBotWeaponInfoList()
	{
		Load();
	}

	void Load();

	RCBotWeaponInfo* getByClassname(const char* szClassname)
	{
		for (auto ret : m_pWeaponInfo)
		{
			if (ret->isClassname(szClassname))
			{
				return ret;
			}
		}

		return nullptr;
	}

	RCBotWeaponInfo* getByID(int id)
	{
		for (auto ret : m_pWeaponInfo)
		{
			if (ret->isID(id))
			{
				return ret;
			}
		}

		return nullptr;

	}

private:
	std::vector<RCBotWeaponInfo*> m_pWeaponInfo;
};

class RCBotWeapon
{
public:
	RCBotWeapon(RCBotWeaponInfo* pWeaponInfo)
	{
		m_pWeaponInfo = pWeaponInfo;
		m_iAmmo1 = nullptr;
		m_iAmmo2 = nullptr;
		m_iClip = 0;
		m_iReserve = 0;
		m_iMaxClip = 0;
	}

	const char* getClassname()
	{
		return m_pWeaponInfo->getClassname();
	}

	bool isClipEmpty()
	{
		return m_iClip == 0;
	}

	void setClip(uint8_t iClip)
	{
		m_iClip = iClip;
	}

	bool needToReload(uint8_t minBullets)
	{
		return m_iClip < minBullets;
	}

	inline bool CanUsePrimary(double distance, bool isUnderWater)
	{
		if (m_iAmmo1 != nullptr)
		{
			if ( *m_iAmmo1 > 0 )
				return m_pWeaponInfo->CanUsePrimary(distance, isUnderWater);

			return false; // out of ammo
		}
		// no ammo index (melee?)
		return true;
	}

	inline bool CanUseSecondary(double distance, bool isUnderWater)
	{
		if (m_iAmmo2 != nullptr)
		{
			if (*m_iAmmo2 > 0)
				return m_pWeaponInfo->CanUseSecondary(distance, isUnderWater);

			return false; // out of ammo
		}
		// no ammo index (melee?)
		return true;
	}
	inline int Priority()
	{
		return m_pWeaponInfo->Priority();
	}

	inline bool HasWeapon(edict_t* pEdict)
	{
		return m_pWeaponInfo->HasWeapon(pEdict);
	}

	int32_t getID()
	{
		return m_iId;
	}

	bool isID(int32_t iID)
	{
		return m_iId = iID;
	}

	inline bool hasFlags(RCBotWeaponFlags flags)
	{
		return (m_pWeaponInfo->hasFlags(flags));
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
	RCBotWeapons(RCBotBase* pBot)
	{
		m_pBot = pBot;
		m_iWeapons = 0;
		memset(m_iAmmo,0,sizeof(uint8_t)* MAX_AMMO_SLOTS);
	}

	~RCBotWeapons()
	{
		for (uint32_t i = 0; i < m_Weapons.size(); i++)
			delete m_Weapons[i];

		m_Weapons.clear();
	}

	void setAmmo(uint8_t index, uint8_t amount);

	void weaponPickup(uint8_t iID);

	RCBotWeapon* getBestWeapon(edict_t* pEnemy);

	void update(RCBotBase* pBot);

	RCBotWeapon* findById(int32_t id);
private:
	std::vector<RCBotWeapon*> m_Weapons;
	int32_t m_iWeapons;
	RCBotBase* m_pBot;
	uint8_t m_iAmmo[MAX_AMMO_SLOTS];

};

extern RCBotWeaponInfoList* gWeaponList;
#endif 
