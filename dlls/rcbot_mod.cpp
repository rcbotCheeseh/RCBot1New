#include "rcbot_mod.h"
#include "rcbot_strings.h"
#include "extdll.h"
#include "h_export_meta.h"
#include "meta_api.h"
#include "rcbot_specialists.h"
#include "rcbot_hldm.h"

RCBotModifications gRCBotModifications;

RCBotModification::RCBotModification(const char* gameFolder)
{
	m_szGameFolder = gRCBotStrings.add(gameFolder);
}

void RCBotModification::putClientInServer(edict_t* pEdict)
{

}

bool RCBotModification::isModification(const char* gameFolder)
{
	return gRCBotStrings.stringMatch(gameFolder, m_szGameFolder);
}

RCBotBase* RCBotModification_TheSpecialists::createBot()
{
	return new RCBotSpecialists(); 
}

RCBotModifications::RCBotModifications()
{
	m_Mods.push_back(new RCBotModification_TheSpecialists());
	m_Mods.push_back(new RCBotModification_HLDM());
}

RCBotModifications::~RCBotModifications()
{
	m_Mods.clear();
}

void RCBotModifications::GameInit()
{
	char game_dir[MODIFICATION_GAME_FOLDER_LEN_MAX];
	// Find mod
	GET_GAME_DIR(game_dir);

	for (auto* pMod : m_Mods)
	{
		if (pMod->isModification(game_dir))
		{
			m_pCurrentMod = pMod;
			break;
		}
	}
}


RCBotBase* RCBotModification_HLDM :: createBot()
{
	return new RCBotHLDM();
}

