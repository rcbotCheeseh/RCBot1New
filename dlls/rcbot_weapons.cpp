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
			ret->m_bPrimaryFire = atoi(split->at(i)) > 0;
			break;
		case RCBotWeaponInfoParameter::SecondaryFire:
			ret->m_bSecondaryFire = atoi(split->at(i)) > 0;
			break;
		case RCBotWeaponInfoParameter::IsExplosive:
			ret->m_bIsExplosive = atoi(split->at(i)) > 0;
			break;
		case RCBotWeaponInfoParameter::Priority:
			ret->m_iPriority = atoi(split->at(i));
			break;
		case RCBotWeaponInfoParameter::CanUseUnderwater:
			ret->m_bCanUseUnderWater = atoi(split->at(i)) > 0;
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
RCBotWeapon* RCBotWeapons::getBestWeapon(RCBotBase *bot, edict_t *pEnemy)
{
	double distance = bot->distanceFrom(pEnemy);

	//RCBotWeapon* bestMelee = nullptr;
	RCBotWeapon* bestPrimary = nullptr;
	RCBotWeapon* bestSecondary = nullptr;

	bool isUnderWater = bot->isUnderWater();

	for ( auto iter : m_Weapons)
	{
		if (iter->CanUsePrimary(distance, isUnderWater))
			bestPrimary = ((bestPrimary == nullptr) || (bestPrimary->Priority()<iter->Priority())) ? iter : bestPrimary;
		if (iter->CanUseSecondary(distance, isUnderWater))
			bestSecondary = ((bestSecondary == nullptr) || (bestSecondary->Priority() < iter->Priority())) ? iter : bestSecondary;
	}
	if (bestSecondary != nullptr)
		return bestSecondary;
	return bestPrimary;
}
