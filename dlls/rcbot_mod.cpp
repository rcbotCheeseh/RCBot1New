#include "rcbot_mod.h"
#include "rcbot_strings.h"
#include "extdll.h"
#include "h_export_meta.h"
#include "meta_api.h"
#include "rcbot_specialists.h"
#include "rcbot_hldm.h"
#include "rcbot_utils.h"
#include "rcbot_navigator.h"
#include "rcbot_message.h"

RCBotModifications gRCBotModifications;

RCBotModification::RCBotModification(const char* gameFolder, const char *szName)
{
	m_szGameFolder = gRCBotStrings.add(gameFolder);
	m_szName = gRCBotStrings.add(szName);
}

class RCBotStuntNodeType : public RCBotNodeType
{
public:
	RCBotStuntNodeType() : RCBotNodeType(RCBotNodeTypeBitMasks::W_FL_STUNT, "stunt", "bot will do a stunt here", Colour(100, 200, 250))
	{

	}

	void Touched(RCBotBase* pBot)
	{
		// let bot do a stunt here
		pBot->pressButton(IN_ALT1);
	}
};

void RCBotModification::GameInit()
{

}

bool RCBotModification::isModification(const char* gameFolder)
{
	return gRCBotStrings.stringMatch(gameFolder, m_szGameFolder);
}

void RCBotModification_TheSpecialists::GameInit()
{
	RCBotNodeTypes* nodeTypes = gRCBotNavigatorNodes->getNodeTypes();

	g_Messages = new RCBotMessages_TheSpecialists();

	RCBotModification::GameInit();

	nodeTypes->addType(new RCBotStuntNodeType());
}

RCBotBase* RCBotModification_TheSpecialists::createBot()
{
	return new RCBotSpecialists(); 
}

RCBotModifications::RCBotModifications()
{
	m_pCurrentMod = nullptr;

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

			pMod->GameInit();

			RCBotUtils::Message(NULL, MessageErrorLevel::Information, "Detected game '%s'", pMod->getName());

			break;
		}
	}
}


RCBotBase* RCBotModification_HLDM :: createBot()
{
	return new RCBotHLDM();
}

void RCBotModification_HLDM::GameInit()
{
	g_Messages = new RCBotMessages();

	RCBotModification::GameInit();
}
