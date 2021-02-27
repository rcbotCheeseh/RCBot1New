#include "rcbot_specialists.h"
#include "extdll.h"
#include "meta_api.h"
#include "dll.h"

void RCBotSpecialists::setUpClientInfo()
{
	int index = ENTINDEX(m_pEdict);
	char *sInfoBuffer = GET_INFOKEYBUFFER(m_pEdict);

	RCBotBase::setUpClientInfo();

	(*g_engfuncs.pfnSetClientKeyValue)(index, sInfoBuffer, "_vgui_menus", "1");

}

bool RCBotSpecialists::isEnemy(edict_t* pEdict)
{
	if (pEdict == m_pEdict)
		return false;

	if (pEdict->v.flags == FL_CLIENT)
	{
		return !gpGlobals->teamplay || (pEdict->v.team != m_pEdict->v.team);
	}

	return false;
}

void RCBotSpecialists::spawnInit()
{
	m_RespawnState = RCBotSpecialists_RespawnState::Initialise;
}

void RCBotSpecialists::respawn()
{
	switch (m_RespawnState)
	{
	case RCBotSpecialists_RespawnState::Initialise:
		m_RespawnState = RCBotSpecialists_RespawnState::BuyGuns;
		break;
	case RCBotSpecialists_RespawnState::BuyGuns:
		//FakeClientCommand(m_pEdict, "say \"buy_guns\"");
		FakeClientCommand(m_pEdict, "buy");
		FakeClientCommand(m_pEdict, "menuselect 7"); // buy random
		m_RespawnState = RCBotSpecialists_RespawnState::Respawn;
		break;
	case RCBotSpecialists_RespawnState::Respawn:
		//FakeClientCommand(m_pEdict, "say \"respawn\"");
		FakeClientCommand(m_pEdict, "respawn");
		m_RespawnState = RCBotSpecialists_RespawnState::Wait;
		break;
	case RCBotSpecialists_RespawnState::Wait:
		primaryAttack();
		m_RespawnState = RCBotSpecialists_RespawnState::Respawn;
		break;
	}
}