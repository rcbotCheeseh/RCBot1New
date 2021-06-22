#include "rcbot_base.h"
#include "extdll.h"
#include "meta_api.h"
#include "dll.h"
#include "h_export_meta.h"
#include "meta_api.h"
#include "rcbot_utils.h"
#include "rcbot_task_utility.h"
#include "rcbot_profile.h"
#include "rcbot_visibles.h"
#include "rcbot_weapons.h"

#include <math.h>

RCBotBase ::RCBotBase()
{
	m_pVisibles = new RCBotVisibles(this);
	m_Utils = new RCBotUtilities();
	setFOV(RCBOT_DEFAULT_FOV);

	Init();
}

RCBotBase :: ~RCBotBase()
{
	delete m_pVisibles;
}

void RCBotBase::Init()
{
	m_fLastRunPlayerMove = gpGlobals->time;
	m_bPreviousAliveState = false;
	m_pWeapons = new RCBotWeapons(this);

	spawnInit();
}

void RCBotBase::spawnInit()
{
	m_vMoveTo.reset();
	m_vLookAt.reset();

	m_fRespawnTime = gpGlobals->time;
	m_fLastRunPlayerMove = gpGlobals->time;
	m_pEnemy.Set(nullptr);
}

#define RCBOT_RESPAWN_WAIT_TIME 2.0f

void RCBotBase::Think()
{
	m_pEdict->v.button = 0;
	m_pEdict->v.impulse = 0;
	m_fSpeedPercent = 1.0f; // full speed
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
		if (m_bPreviousAliveState == false)
		{
			m_bPreviousAliveState = true;

			m_pEdict->v.v_angle = m_pEdict->v.angles;
		}
	}

	m_pVisibles->tasks(m_pProfile->getVisRevs());

	if (m_pSchedule != nullptr)
	{
		switch (m_pSchedule->Execute(this))
		{
		case RCBotTaskState::RCBotTaskState_Complete:
		case RCBotTaskState::RCBotTaskState_Fail:
			m_pSchedule = nullptr;
			break;
		}
	}
	else
	{
		RCBotUtility *pUtil = m_Utils->getBestUtility(this);

		if (pUtil != nullptr)
		{
			m_pSchedule = pUtil->execute(this);
		}
	}
}

#define BOT_MOVE_TO_MIN_DISTANCE 16.0f
#define BOT_MOVE_TO_MAX_SPEED 320.0f

void RCBotBase::setCurrentWeapon(uint8_t iState, uint8_t iId, uint8_t iClip)
{
	RCBotWeapon* weapon = m_pWeapons->findById(iId);

	if (weapon != nullptr)
	{
		//weapon->setState();
		weapon->setClip(iClip);
		m_pCurrentWeapon = weapon;
	}
}

bool RCBotBase::isCurrentWeapon(RCBotWeapon* weapon)
{
	return m_pCurrentWeapon == weapon;
}

void RCBotBase::weaponPickup(uint8_t iID)
{
	m_pWeapons->weaponPickup(iID);
}

void RCBotBase::selectWeapon(RCBotWeapon* weapon)
{
	if ( !isCurrentWeapon(weapon) )
		FakeClientCommand(m_pEdict, weapon->getClassname());
}

void RCBotBase::pressButton(int button)
{
	m_pEdict->v.button |= button;
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
		float fTurnSpeed = 10.0f;
		entvars_t* pev = &m_pEdict->v;
		Vector vLook = m_vLookAt.getValue() - vOrigin;
		Vector vAngles = RCBotUtils::VectorToAngles(vLook);
		RCBotUtils::FixAngles(&vAngles);

		pev->idealpitch = -vAngles.x;
		pev->ideal_yaw = vAngles.y;

		vAngles.z = 0;
		pev->v_angle.z = pev->v_angle.z = 0;

		pev->ideal_yaw = vAngles.y;

		// change angles smoothly

		float temp;

		//temp = 1/1+exp(-fabs((pev->ideal_yaw+180.0f)-(pev->v_angle.y+180.0f))/180);
		temp = fabs(pev->ideal_yaw + 180.0f - (pev->v_angle.y + 180.0f));

		fTurnSpeed = temp / fTurnSpeed;//fabs((pev->ideal_yaw+180.0f)-(pev->v_angle.y+180.0f))/20;//m_fTurnSpeed;
		// change yaw
		RCBotUtils::ChangeAngle(&fTurnSpeed, &pev->ideal_yaw, &pev->v_angle.y, &pev->angles.y); // 5 degrees

		//temp = 1/1+exp(-fabs((pev->idealpitch+180.0f)-(pev->v_angle.x+180.0f))/180);
		temp = fabs(pev->idealpitch + 180.0f - (pev->v_angle.x + 180.0f));

		// set by ChangeAngles... remove this functionality soon...
		fTurnSpeed = temp / fTurnSpeed;
		//fTurnSpeed = fabs((pev->idealpitch+180.0f)-(pev->v_angle.x+180.0f))/20;//m_fTurnSpeed;

		// change pitch
		RCBotUtils::ChangeAngle(&fTurnSpeed, &pev->idealpitch, &pev->v_angle.x, &pev->angles.x);

		//pev->v_angle.x = -pev->v_angle.x;
		pev->angles.x = -pev->v_angle.x / 3;

		pev->angles.y = pev->v_angle.y;//*/
	}

	if (m_vMoveTo.isValid())
	{
		Vector vMoveTo = m_vMoveTo.getValue();

		if (distanceFrom2D(vMoveTo) > BOT_MOVE_TO_MIN_DISTANCE)
		{
			Vector vComponent = m_vMoveTo.getValue() - vOrigin;
			Vector vMove = RCBotUtils::VectorToAngles(vComponent);
			vMove = vMove.Normalize();

			float fAngle = RCBotUtils::yawAngle(m_pEdict, vMoveTo);

			float radians = fAngle * 3.141592f / 180.f; // degrees to radians
			fForwardSpeed = cos(radians) * BOT_MOVE_TO_MAX_SPEED;
			fSideSpeed = sin(radians) * BOT_MOVE_TO_MAX_SPEED;

			fUpSpeed = 0;
		}

		if (getActualSpeed() < 1)
		{
			if (RANDOM_LONG(0, 100) > 50)
				jump();
		}

		if (m_pEdict->v.movetype == MOVETYPE_FLY)
			m_pEdict->v.button |= IN_FORWARD;
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

	vecLOS = vOrigin - getViewOrigin();
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
		m_pEnemy.Set(nullptr);
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