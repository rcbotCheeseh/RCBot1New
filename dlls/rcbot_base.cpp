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
}

RCBotBase :: ~RCBotBase()
{
	delete m_pVisibles;
}

void RCBotBase::Think()
{
	m_pVisibles->tasks(m_pProfile->getVisRevs());
}

void RCBotBase::setProfile(RCBotProfile* profile)
{
	m_pProfile = profile;
}

void RCBotBase::setEdict(edict_t* pEdict)
{
	m_pEdict = pEdict;
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

}

void RCBotBase::lostVisible(edict_t* pEntity)
{

}

float RCBotBase::distanceFrom(const edict_t* pEntity)
{
	return distanceFrom(RCBotUtils::entityOrigin(pEntity));
}


float RCBotBase::distanceFrom(const Vector& vOrigin)
{
	return (vOrigin - getViewOrigin()).Length();
}