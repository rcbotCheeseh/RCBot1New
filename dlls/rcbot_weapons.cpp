#include "rcbot_weapons.h"
#include "rcbot_strings.h"
#include "rcbot_file.h"

RCBotWeaponInfoList* gWeaponList;

enum class RCBotWeaponInfoParameter
{
	Classname,
	PrimaryMinDistance,
	PrimaryMaxDistance,
	SecondaryMinDistance,
	SecondaryMaxDistance,
	PrimaryFire,
	SecondaryFire,
	IsExplosive,
	CanUseUnderwater,
	Priority
};

/// <summary>
/// 
/// </summary>
RCBotWeaponInfo::RCBotWeaponInfo()
{
	memset(this, 0, sizeof(RCBotWeaponInfo));
}
bool RCBotWeaponInfo::isClassname(const char *szClassname)
{
	return gRCBotStrings.stringMatch(szClassname,m_szClassname);
}
/// <summary>
/// Example weapons ini
/// Classname,PrimaryMinDistance,PrimaryMaxDistance,SecondaryMinDistance,SecondaryMaxDistance,PrimaryFire,SecondaryFire,IsExplosive,CanUseUnderwater,Priority
/// weapon_handgun,100,1000,100,500,1,1,0,1,1
/// weapon_crowbar,0,100,0,0,1,0,0,1,0
/// 
/// </summary>
/// <param name="split"></param>
/// <returns></returns>
RCBotWeaponInfo* RCBotWeaponInfo::Parse(std::vector<const char*> *split)
{
	RCBotWeaponInfo* ret = new RCBotWeaponInfo();

	for (uint16_t i = 0; i < split->size(); i++)
	{
		switch ((RCBotWeaponInfoParameter)i)
		{
		case RCBotWeaponInfoParameter::Classname:
			ret->m_szClassname = split->at(i);
			break;
		case RCBotWeaponInfoParameter::PrimaryMinDistance:
			ret->m_fPrimaryMinDistance = atof(split->at(i));
			break;
		case RCBotWeaponInfoParameter::PrimaryMaxDistance:
			ret->m_fPrimaryMinDistance = atof(split->at(i));
			break;
		case RCBotWeaponInfoParameter::SecondaryMinDistance:
			ret->m_fSecondaryMinDistance = atof(split->at(i));
			break;
		case RCBotWeaponInfoParameter::SecondaryMaxDistance:
			ret->m_fSecondaryMaxDistance = atof(split->at(i));
			break;
		case RCBotWeaponInfoParameter::PrimaryFire:
			ret->m_iFlags |= ((atoi(split->at(i)) == 1) ? RCBotWeaponFlags::PrimaryFire : RCBotWeaponFlags::None);
			break;
		case RCBotWeaponInfoParameter::SecondaryFire:
			ret->m_iFlags |= ((atoi(split->at(i)) == 1) ? RCBotWeaponFlags::SecondaryFire : RCBotWeaponFlags::None);
			break;
		case RCBotWeaponInfoParameter::IsExplosive:
			ret->m_iFlags |= ((atoi(split->at(i)) == 1) ? RCBotWeaponFlags::Explosive : RCBotWeaponFlags::None);
			break;
		case RCBotWeaponInfoParameter::Priority:
			ret->m_iPriority = atoi(split->at(i));
			break;
		case RCBotWeaponInfoParameter::CanUseUnderwater:
			ret->m_iFlags |= ((atoi(split->at(i)) == 1) ? RCBotWeaponFlags::Underwater : RCBotWeaponFlags::None);
		}
	}

	return ret;
}

void RCBotWeaponInfoList::Load()
{
	RCBotFile* file = RCBotFile::Open(RCBOT_WEAPON_CONFIG_FOLDER, RCBOT_WEAPON_CONFIG_NAME, ".ini", "r", true);

	if (file != nullptr)
	{
		const char *line = file->readLine();
		std::vector<const char*> list;
		
		gRCBotStrings.split(line, &list, ',');

		RCBotWeaponInfo* info = RCBotWeaponInfo::Parse(&list);

		m_pWeaponInfo.push_back(info);

		file->close();
	}
}
/// <summary>
/// 
/// </summary>
/// <param name="bot"></param>
/// <param name="pEnemy"></param>
/// <returns></returns>
RCBotWeapon* RCBotWeapons::getBestWeapon( edict_t *pEnemy )
{
	double distance = m_pBot->distanceFrom(pEnemy);

	//RCBotWeapon* bestMelee = nullptr;
	RCBotWeapon* bestPrimary = nullptr;
	RCBotWeapon* bestSecondary = nullptr;

	bool isUnderWater = m_pBot->isUnderWater();

	RCBotWeaponFlags iFlags = RCBotWeaponFlags::None;// m_pBot->getEnemyWeaponFlags(pEnemy);

	for ( auto iter : m_Weapons)
	{
		if ( (iFlags == RCBotWeaponFlags::None) || iter->hasFlags(iFlags) )
		{
			if (iter->CanUsePrimary(distance, isUnderWater))
				bestPrimary = ((bestPrimary == nullptr) || (bestPrimary->Priority() < iter->Priority())) ? iter : bestPrimary;
			if (iter->CanUseSecondary(distance, isUnderWater))
				bestSecondary = ((bestSecondary == nullptr) || (bestSecondary->Priority() < iter->Priority())) ? iter : bestSecondary;
		}
	}
	if (bestSecondary != nullptr)
		return bestSecondary;
	return bestPrimary;
}

RCBotWeapon* RCBotWeapons::findById(int32_t iID)
{
	for (auto ret : m_Weapons)
	{
		if (ret->isID(iID))
			return ret;
	}

	return nullptr;
}

void RCBotWeapons::weaponPickup(uint8_t iID)
{
	RCBotWeaponInfo *pWeaponInfo = gWeaponList->getByID(iID);

	if (this->findById(iID) == nullptr)
	{
		RCBotWeapon* pWeapon = new RCBotWeapon(pWeaponInfo);

		m_Weapons.push_back(pWeapon);
	}
}

void RCBotWeapons::update(RCBotBase* pBot)
{
	if (m_iWeapons != pBot->getEdict()->v.weapons)
	{
		m_iWeapons = pBot->getEdict()->v.weapons;

		// Change made to weapons
		for (uint8_t i = 0; i < 32; i ++ )
		{
			RCBotWeapon* hadWeapon = findById(i);
			bool hasWeapon = (m_iWeapons & (1 << i)) > 0;

			if (!hadWeapon && hasWeapon)
			{
				RCBotWeaponInfo *pWeapInfo = gWeaponList->getByID(i);

				if ( pWeapInfo != nullptr )
					m_Weapons.push_back(new RCBotWeapon(pWeapInfo));
			}
		}
	}
}