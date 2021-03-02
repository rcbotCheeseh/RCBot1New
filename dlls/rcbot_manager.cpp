#include "rcbot_manager.h"
#include "rcbot_profile.h"
#include "rcbot_mod.h"
#include "extdll.h"
#include "h_export_meta.h"
#include "meta_api.h"
#include "rcbot_engine_funcs.h"
#include "rcbot_utils.h"
#include "rcbot_navigator.h"

RCBotManager gRCBotManager;

extern globalvars_t* gpGlobals;

RCBotManager::~RCBotManager()
{
	for (auto* pBot : m_Bots)
	{
		delete pBot;
	}

	m_Bots.clear();
}

RCBotManager::RCBotManager()
{
	m_iQuota = 0;
	m_fAddRemoveBotTime = 0.0f;
}

void RCBotManager::Think()
{
	for (uint32_t i = 0; i < m_Bots.size(); i++)
	{
		RCBotBase* pBot = m_Bots[i];
		pBot->Think();
		pBot->RunPlayerMove();
	}

	if (m_fAddRemoveBotTime < gpGlobals->time)
	{
		m_fAddRemoveBotTime = gpGlobals->time + BOT_MANAGER_DEFAULT_ADD_REMOVE_BOT_PERIOD;

		if (m_Bots.size() > m_iQuota)
		{
			KickBot();
		}
		else if (m_Bots.size() < m_iQuota)
		{
			RCBotBase * pBot = AddBot();

			if (pBot != nullptr)
			{
				m_Bots.push_back(pBot);
			}
			else 
			{
				if ( m_iQuota > 0 )
					m_iQuota--; // decrease Quota 
			}
		}
	}

	if (m_fNodeDrawTime < gpGlobals->time)
	{
		edict_t* pPlayer = INDEXENT(1);

		m_fNodeDrawTime = gpGlobals->time + RCBOT_NODE_DRAW_PERIOD;

		if (!FNullEnt(pPlayer))
		{
			gRCBotNavigatorNodes->draw(pPlayer, true, gRCBotNavigatorNodeTypes);
		}
	}
}

bool RCBotManager::SetQuota(uint8_t iQuota)
{
	if (m_iQuota > gpGlobals->maxClients)
		return false;

	m_iQuota = iQuota;

	return true;
}

RCBotBase *RCBotManager::AddBot()
{
	RCBotProfile * profile = gRCBotProfiles->getRandomUnused();

	if (profile != nullptr)
	{
		edict_t *pBotEdict = (*g_engfuncs.pfnCreateFakeClient)(profile->getName());

		if (pBotEdict != nullptr)
		{
			RCBotModification *pMod = gRCBotModifications.getCurrentMod();

			if (pMod == nullptr)
			{
				RCBotUtils::Message(nullptr, MessageErrorLevel::Information, "Unknown modification!");
				return nullptr;
			}

			RCBotBase *pBot = pMod->createBot();

			if (pBot != nullptr)
			{
				char ptr[128];  // allocate space for message from ClientConnect

				pBot->setEdict(pBotEdict);
				pBot->setProfile(profile);
				pBot->setUpClientInfo();

				MDLL_ClientConnect(pBotEdict, nullptr, "127.0.0.1", ptr);
				ClientConnect(pBotEdict, nullptr, "127.0.0.1", ptr);

				MDLL_ClientPutInServer(pBotEdict);
				ClientPutInServer(pBotEdict);

				return pBot;
			}
		}
	}

	return nullptr;
}

void RCBotManager::ChangeLevel()
{
	for (auto* pBot : m_Bots)
	{
		delete pBot;
	}

	m_Bots.clear();
}

void RCBotManager::KickBot()
{
	if (m_Bots.size() > 0)
	{
		RCBotBase* pBot = m_Bots[m_Bots.size() - 1];

		const char* szName = STRING(pBot->getEdict()->v.netname);

		char cmd[128];

		sprintf(cmd, "kick \"%s\"\n", szName);

		SERVER_COMMAND(cmd);

		m_Bots.erase(m_Bots.end());
	}
}
	
void RCBotManager::LevelInit()
{
	ChangeLevel();
}
