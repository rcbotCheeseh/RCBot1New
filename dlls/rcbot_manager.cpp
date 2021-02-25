#include "rcbot_manager.h"
#include "rcbot_profile.h"
#include "rcbot_mod.h"
#include "extdll.h"
#include "h_export_meta.h"
#include "meta_api.h"
#include "rcbot_engine_funcs.h"
#include "rcbot_utils.h"

RCBotManager gRCBotManager;

extern globalvars_t* gpGlobals;

RCBotManager::~RCBotManager()
{
	for each (RCBotBase* pBot in m_Bots)
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
			RCBotBase *newBot = AddBot();

			if (newBot == NULL)
			{
				if ( m_iQuota > 0 )
					m_iQuota--; // decrease Quota 
			}
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
	RCBotProfile * profile = gRCBotProfiles.getRandomUnused();

	if (profile != NULL)
	{
		edict_t *pBotEdict = (*g_engfuncs.pfnCreateFakeClient)(profile->getName());

		if (pBotEdict != NULL)
		{
			RCBotModification *pMod = gRCBotModifications.getCurrentMod();

			if (pMod == NULL)
			{
				RCBotUtils::Message(NULL, MessageErrorLevel::MessageErrorLevel_Information, "Unknown modification!");
				return NULL;
			}

			RCBotBase *pBot = pMod->createBot();

			if (pBot != NULL)
			{
				char ptr[128];  // allocate space for message from ClientConnect

				pBot->setEdict(pBotEdict);
				pBot->setProfile(profile);
				pBot->setUpClientInfo();

				MDLL_ClientConnect(pBotEdict, NULL, "127.0.0.1", ptr);
				ClientConnect(pBotEdict, NULL, "127.0.0.1", ptr);

				MDLL_ClientPutInServer(pBotEdict);
				ClientPutInServer(pBotEdict);

				m_Bots.push_back(pBot);
			}
		}
	}

	return NULL;
}
void RCBotManager::KickBot()
{

}
	
void RCBotManager::LevelInit()
{

}
