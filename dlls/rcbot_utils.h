#ifndef __RCBOT_UTILS_H__
#define __RCBOT_UTILS_H__

#include "extdll.h"
#include "vector.h"
#include "h_export_meta.h"
#include "meta_api.h"
#include "rcbot_colour.h"

 enum class MessageErrorLevel
{
	Information,
	Warning,
	Critical

};

class RCBotUtils
{
public:
	static Vector entityOrigin(const edict_t* pEdict)
	{
		return pEdict->v.absmin + (pEdict->v.size / 2);
	}

	static float DegreesToRadians(float fDegrees)
	{
		return (fDegrees * M_PI) / 180.0;
	}

	static float RadiansToDegrees(float fRadians)
	{
		return (fRadians * 180.0) / M_PI;
	}
	static Vector VectorToAngles(Vector& vec);

	static void MakeVectors(Vector& vec);

	static const char* GetClassname(edict_t* pEdict);

	static TraceResult *Traceline(const Vector& vecStart, const Vector& vecEnd, IGNORE_MONSTERS igmon, IGNORE_GLASS ignoreGlass, edict_t* pentIgnore);

	static void Message(edict_t* pEntity, MessageErrorLevel errorlevel, char* fmt, ...);

	static void ChangeAngle(float* fSpeed, const float* fIdeal, float* fCurrent, float* fUpdate);

	static void FixAngle(float* fAngle);
	static void FixAngles(Vector* vAngles);
	static edict_t* findPlayer(const char* szName);
	static void drawBeam(edict_t* pClient, const Vector& vFrom, const Vector& vTo, const Colour& vColour, int iTexture);
	static void mapInit();

};

#endif 
