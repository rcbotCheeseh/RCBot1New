#include "rcbot_specialists.h"
#include "rcbot_navigator.h"
#include "rcbot_mod.h"
#include "rcbot_message.h"
#include "extdll.h"
#include "meta_api.h"
#include "rcbot_utils.h"
#include "dll.h"

void RCBotSpecialists::setUpClientInfo()
{

	RCBotBase::setUpClientInfo();

}

bool RCBotSpecialists::isEnemy(edict_t* pEdict)
{
	if (pEdict == m_pEdict)
		return false;

	if (pEdict->v.flags & FL_CLIENT)
	{
		return !gpGlobals->teamplay || (pEdict->v.team != m_pEdict->v.team);
	}

	return false;
}

void RCBotSpecialists::spawnInit()
{
	m_RespawnState = RCBotSpecialists_RespawnState::Initialise;
}


void RCBotSpecialists::Think()
{
	RCBotBase::Think();

	if (m_pEnemy.Get() != nullptr)
	{
		edict_t* pEnemy = m_pEnemy.Get();

		if (isEnemy(pEnemy))
		{
			// Enemy still alive and still an "enemy"
			Vector vEnemy = RCBotUtils::entityOrigin(pEnemy);

			setLookAt(vEnemy, 3);
			setMoveTo(vEnemy);

			if (RANDOM_FLOAT(0.0, 100.0) > 75.0f)
				primaryAttack();
		}
		else
			m_pEnemy.Set(nullptr);
	}
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
		m_RespawnState = RCBotSpecialists_RespawnState::BuyGuns2;
		break;
	case RCBotSpecialists_RespawnState::BuyGuns2:
		FakeClientCommand(m_pEdict, "menuselect 7"); // buy random
		m_RespawnState = RCBotSpecialists_RespawnState::Respawn;
		break;
	case RCBotSpecialists_RespawnState::Respawn:
		//FakeClientCommand(m_pEdict, "say \"respawn\"");
		FakeClientCommand(m_pEdict, "respawn");
		m_RespawnState = RCBotSpecialists_RespawnState::Wait;
		break;
	case RCBotSpecialists_RespawnState::Wait:
		if ( RANDOM_LONG(0,100) >= 50 )
			primaryAttack();
		m_RespawnState = RCBotSpecialists_RespawnState::Respawn;
		break;
	}
}

#define RCBOT_NODETYPE_EXTRACOST_STUNT -10


class RCBotStuntNodeType : public RCBotNodeType
{
public:
	RCBotStuntNodeType() : RCBotNodeType(RCBotNodeTypeBitMasks::W_FL_STUNT, "stunt", "bot will do a stunt here", Colour(100, 200, 250), RCBOT_NODETYPE_EXTRACOST_STUNT)
	{

	}

	void Touched(RCBotBase* pBot)
	{
		// let bot do a stunt here
		pBot->pressButton(IN_ALT1);
	}
};

class RCBotNavigatorNodes_TheSpecialists : public RCBotNavigatorNodes
{
public:

	RCBotNavigatorNodes_TheSpecialists() : RCBotNavigatorNodes()
	{
		m_NodeTypes->addType(new RCBotStuntNodeType());
	}
};

void RCBotModification_TheSpecialists::GameInit()
{
	gRCBotNavigatorNodes = new RCBotNavigatorNodes_TheSpecialists();

	g_Messages = new RCBotMessages_TheSpecialists();

	RCBotModification::GameInit();
}

RCBotBase* RCBotModification_TheSpecialists::createBot()
{
	return new RCBotSpecialists();
}
