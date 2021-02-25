#ifndef __RCBOT_UTILS_H__
#define __RCBOT_UTILS_H__

#include "extdll.h"
#include "vector.h"
#include "h_export_meta.h"
#include "meta_api.h"

 enum class MessageErrorLevel
{
	MessageErrorLevel_Information,
	MessageErrorLevel_Warning,
	MessageErrorLevel_Critical

};

class RCBotUtils
{
public:
	static Vector entityOrigin(edict_t* pEdict)
	{
		return pEdict->v.absmin + pEdict->v.size / 2;
	}

	static float DegreesToRadians(float fDegrees)
	{
		return (fDegrees * M_PI) / 180.0;
	}

	static float RadiansToDegrees(float fRadians)
	{
		return (fRadians * 180.0) / M_PI;
	}

	static const char* GetClassname(edict_t* pEdict);

	static TraceResult *Traceline(const Vector& vecStart, const Vector& vecEnd, IGNORE_MONSTERS igmon, IGNORE_GLASS ignoreGlass, edict_t* pentIgnore);

	static void Message(edict_t* pEntity, MessageErrorLevel errorlevel, char* fmt, ...);
};

#endif 
