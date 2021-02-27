#include "rcbot_profile.h"
#include "rcbot_file.h"
#include "rcbot_strings.h"
#include "extdll.h"
#include "enginecallback.h"
#include "rcbot_utils.h"
#include <vector>

RCBotProfiles gRCBotProfiles;

void RCBotProfiles::List(edict_t *pClient)
{
	uint32_t id = 0;

	RCBotUtils::Message(pClient, MessageErrorLevel::Information, "id\tname\tskill\tmodel\tvisrevs");  

	for (auto* pProfile : m_UnusedProfiles)
	{
		RCBotUtils::Message(pClient, MessageErrorLevel::Information, "%d\t%s\t%0.1f\t%s\t%d", id,
			pProfile->getName(), pProfile->getSkill(), pProfile->getModel(), pProfile->getVisRevs());
		id++;
	}
}

RCBotProfile* RCBotProfiles::getRandomUnused()
{
	if (m_UnusedProfiles.size() > 0)
	{
		return UseProfile(RANDOM_LONG(0, m_UnusedProfiles.size() - 1));
	}

	return nullptr;
}

RCBotProfile *RCBotProfiles::UseProfile( int iIndex )
{
	RCBotProfile* profile = m_UnusedProfiles[iIndex];

	m_UnusedProfiles.erase(m_UnusedProfiles.begin()+iIndex);

	return profile;
}

void RCBotProfiles::UnuseProfiles(RCBotProfile* profile)
{
	m_UnusedProfiles.push_back(profile);
}

void RCBotProfiles::Reset()
{
	m_UnusedProfiles.clear();
}

bool RCBotProfiles::Load(const char* szFilename)
{
	RCBotFile* file = RCBotFile::Open(szFilename, "r");

	if (file != nullptr)
	{
		const char* line;

		m_ProfilePool.clear();

		while ((line = file->readLine()) != nullptr)
		{
			RCBotProfile* profile = RCBotProfile::FromLine(line);

			if (profile != nullptr)
			{
				m_ProfilePool.push_back(profile);
				m_UnusedProfiles.push_back(profile);
			}
		}

		file->close();

		return m_ProfilePool.size() > 0;
	}

	return false;
}

RCBotProfiles::~RCBotProfiles()
{
	for (auto* profile : m_ProfilePool)
	{
		delete profile;
	}

	m_ProfilePool.clear();
	m_UnusedProfiles.clear();
}

RCBotProfile::RCBotProfile(const char* szName,float fSkill,const char *szModel,int iVisRevs)
{
	m_szName = szName;
	m_fSkill = fSkill;
	m_szModel = szModel;
	m_iVisRevs = iVisRevs;
}

RCBotProfile* RCBotProfile::FromLine(const char* szLine)
{
	std::vector<const char*> split;

	int iColumn = 0;

	const char* szName = nullptr;
	const char* szModel = nullptr;
	float fSkill = RCBOT_PROFILE_SKILL_INVALID;
	int iVisRevs = RCBOT_PROFILE_VIS_REVS_DEFAULT;
	gRCBotStrings.split(szLine, &split, ',');

	for (auto *splitvar : split )
	{
		switch (iColumn)
		{
		case 0: // name
			szName = splitvar;
			break;
		case 1: // skill
			if (splitvar && *splitvar)
				fSkill = atoi(splitvar);
			break;
		case 2://model
			szModel = splitvar;
			break;
		case 3:
			// vis revs
			if ( splitvar && *splitvar )
				iVisRevs = atoi(splitvar);
			break;
		}
		iColumn++;
	}

	if (szName != nullptr && fSkill > 0)
	{
		return new RCBotProfile(szName,fSkill,szModel,iVisRevs);
	}

	return nullptr;
}