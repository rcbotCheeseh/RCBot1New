#include "rcbot_base.h"
#include "extdll.h"
#include "h_export_meta.h"
#include "meta_api.h"
#include "rcbot_utils.h"
#include <math.h>

RCBotBase ::RCBotBase()
{
	m_pVisibles = new RCBotVisibles(this);
	setFOV(RCBOT_DEFAULT_FOV);

	Init();
}

RCBotBase :: ~RCBotBase()
{
	delete m_pVisibles;
}

void RCBotBase::Init()
{
	m_vMoveTo.reset();
	m_vLookAt.reset();

	m_fRespawnTime = gpGlobals->time;
	m_fLastRunPlayerMove = gpGlobals->time;
	m_pEnemy.Set(nullptr);
	m_bPreviousAliveState = false;

	spawnInit();
}

void RCBotBase::spawnInit()
{
	
}

#define RCBOT_RESPAWN_WAIT_TIME 2.0f

void RCBotBase::Think()
{
	m_pEdict->v.button = 0;
	m_pEdict->v.impulse = 0;
	m_vMoveTo.reset();
	m_vLookAt.reset();

	if (!isAlive())
	{
		if (m_bPreviousAliveState == true)
		{
			spawnInit();
			m_bPreviousAliveState = false;
		}

		if (m_fRespawnTime < gpGlobals->time)
		{
			m_fRespawnTime = gpGlobals->time + RCBOT_RESPAWN_WAIT_TIME;

			respawn();
			return;
		}
	}
	else
	{
		m_bPreviousAliveState = true;
	}

	if (m_pEnemy.Get() != nullptr)
	{
		edict_t* pEnemy = m_pEnemy.Get();

		Vector vEnemy = RCBotUtils::entityOrigin(pEnemy);
		m_vMoveTo.setValue(vEnemy,1);
		m_vLookAt.setValue(vEnemy,1);
	}

	m_pVisibles->tasks(m_pProfile->getVisRevs());
}

void RCBotBase::RunPlayerMove()
{
	float msec = (gpGlobals->time - m_fLastRunPlayerMove) * 1000;

	Vector vOrigin = getViewOrigin();

	float fForwardSpeed = 0;
	float fSideSpeed = 0;
	float fUpSpeed = 0;

	if (msec > 255)
		msec = 255;

	if (m_vLookAt.isValid() )
	{
		Vector vLook = m_vLookAt.getValue() - vOrigin;

		m_pEdict->v.v_angle = m_pEdict->v.angles = RCBotUtils::VectorToAngles(vLook);
	}

	if (m_vMoveTo.isValid())
	{
		Vector vComponent = m_vMoveTo.getValue() - vOrigin;
		Vector vMove = RCBotUtils::VectorToAngles(vComponent);

		fForwardSpeed = vMove.x/180;
		fSideSpeed = vMove.y/180;

		fUpSpeed = 0;
	}

	m_fLastRunPlayerMove = gpGlobals->time;

	(*g_engfuncs.pfnRunPlayerMove)(m_pEdict, m_pEdict->v.angles, fForwardSpeed, fSideSpeed, fUpSpeed, m_pEdict->v.button,
		m_pEdict->v.impulse, (uint8_t)msec);

}

void RCBotBase::setProfile(RCBotProfile* profile)
{
	m_pProfile = profile;
}

void RCBotBase::setEdict(edict_t* pEdict)
{
	m_pEdict = pEdict;
	m_pEdict->v.flags |= FL_FAKECLIENT;
}

void RCBotBase::setFOV(float fFOV)
{
	m_fFovCos = cos(RCBotUtils::DegreesToRadians(fFOV));
}

Vector RCBotBase::getViewOrigin()
{
	return m_pEdict->v.origin + m_pEdict->v.view_ofs;
}

bool RCBotBase::inViewCone(Vector &vOrigin)
{
	static Vector vecLOS;
	static float flDot;

	// in fov? Check angle to edict
	MAKE_VECTORS(m_pEdict->v.v_angle);

	vecLOS = vOrigin - m_pEdict->v.origin + getViewOrigin();
	vecLOS = vecLOS.Normalize();

	flDot = DotProduct(vecLOS, gpGlobals->v_forward);

	return flDot > m_fFovCos;
}

void RCBotBase::setUpClientInfo()
{
	char* sInfoBuffer; // Bots infobuffer
	int index = ENTINDEX(m_pEdict);

	CALL_GAME_ENTITY(PLID, "player", VARS(m_pEdict)); // Olo

	sInfoBuffer = GET_INFOKEYBUFFER(m_pEdict);

	(*g_engfuncs.pfnSetClientKeyValue)(index, sInfoBuffer, "rate", "3500");
	(*g_engfuncs.pfnSetClientKeyValue)(index, sInfoBuffer, "cl_updaterate", "20");
	(*g_engfuncs.pfnSetClientKeyValue)(index, sInfoBuffer, "cl_dlmax", "128");

	(*g_engfuncs.pfnSetClientKeyValue)(index, sInfoBuffer, "_vgui_menus", "0");

	(*g_engfuncs.pfnSetClientKeyValue)(index, sInfoBuffer, "_ah", "0");
	(*g_engfuncs.pfnSetClientKeyValue)(index, sInfoBuffer, "dm", "0");
	(*g_engfuncs.pfnSetClientKeyValue)(index, sInfoBuffer, "tracker", "0");

	if (m_pProfile->getModel() != nullptr)
	{
		(*g_engfuncs.pfnSetClientKeyValue)(index, sInfoBuffer, "model", (char*) m_pProfile->getModel());
	}

}

void RCBotBase::newVisible(edict_t* pEntity)
{
	if ((m_pEnemy.Get() == nullptr) && isEnemy(pEntity))
	{
		m_pEnemy.Set(pEntity);
	}
}

void RCBotBase::lostVisible(edict_t* pEntity)
{
	if (pEntity == nullptr)
		return;

	if (m_pEnemy.Get() == pEntity)
		m_pEnemy.Set(pEntity);
}

bool RCBotBase::isAlive()
{
	return m_pEdict->v.deadflag == DEAD_NO;
}

void RCBotBase::respawn()
{
	primaryAttack();
}


float RCBotBase::distanceFrom(const edict_t* pEntity)
{
	return distanceFrom(RCBotUtils::entityOrigin(pEntity));
}


float RCBotBase::distanceFrom(const Vector& vOrigin)
{
	return (vOrigin - getViewOrigin()).Length();
}