#ifndef __RCBOT_PROFILE_H__
#define __RCBOT_PROFILE_H__

#include <vector>
#include "extdll.h"
#include "meta_api.h"

#define RCBOT_PROFILE_SKILL_INVALID -1.0
#define RCBOT_PROFILE_VIS_REVS_DEFAULT 200

#define RCBOT_PROFILES_FILENAME "profiles"
#define RCBOT_PROFILES_FOLDER "profiles"
#define RCBOT_PROFILES_EXTENSION "ini"

class RCBotProfile
{
public:

	static RCBotProfile* FromLine(const char* szLine);

	RCBotProfile()
	{
		m_szName = nullptr;
		m_fSkill = RCBOT_PROFILE_SKILL_INVALID;
		m_szModel = nullptr;
		m_iVisRevs = RCBOT_PROFILE_VIS_REVS_DEFAULT;
	}

	const char* getName()
	{
		return m_szName;
	}

	float getSkill()
	{
		return m_fSkill;
	}

	const char* getModel()
	{
		return m_szModel;
	}

	int getVisRevs()
	{
		return m_iVisRevs;
	}
private:
	RCBotProfile(
		const char* szName,
		float fSkill,
		const char *szModel,
		int iVisRevs
	);

	const char* m_szName;
	float m_fSkill;
	const char* m_szModel;
	int m_iVisRevs;
};

class RCBotProfiles
{
public:
	RCBotProfiles()
	{
		Load(RCBOT_PROFILES_FILENAME);
	}

	~RCBotProfiles();

	RCBotProfile* getRandomUnused();
	RCBotProfile* UseProfile(int iIndex);
	void UnuseProfiles(RCBotProfile* profile);
	void addProfile(RCBotProfile* profile)
	{
		m_ProfilePool.push_back(profile);
		m_UnusedProfiles.push_back(profile);
	}

	void removeProfile(RCBotProfile* profile)
	{
		
	}

	void List(edict_t* pClient);

	void Reset();

	bool Load( const char *szFilename );

	static void GameInit();

private:
	std::vector<RCBotProfile*> m_ProfilePool;

	std::vector<RCBotProfile*> m_UnusedProfiles;

};

extern RCBotProfiles *gRCBotProfiles;

#endif 

