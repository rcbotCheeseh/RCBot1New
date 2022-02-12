#include "rcbot_utils.h"
#include "meta_api.h"
#include "rcbot_const.h"
#include "extdll.h"
#include "rcbot_message.h"
#include "rcbot_colour.h"
#include "rcbot_strings.h"
#include <string.h>
/// <summary>
/// Gets the yaw angle between the edict and its view angle and an origin
/// </summary>
/// <param name="pEdict"></param>
/// <param name="vOrigin"></param>
/// <returns>yaw angle</returns>
float RCBotUtils::yawAngle(edict_t* pEdict, Vector& vOrigin)
{
	float fAngle;
	Vector vBotAngles = pEdict->v.v_angle;
	Vector vAngles;

	MakeVectors(vBotAngles);

	vAngles = vOrigin - pEdict->v.origin;
	vAngles = VectorToAngles(vAngles);

	fAngle = vBotAngles.y - vAngles.y;

	FixAngle(&fAngle);

	return fAngle;
}
/// <summary>
/// Performs a traceline 
/// </summary>
/// <param name="vecStart"></param>
/// <param name="vecEnd"></param>
/// <param name="igmon"></param>
/// <param name="ignoreGlass"></param>
/// <param name="pentIgnore"></param>
/// <returns></returns>
TraceResult *RCBotUtils::Traceline (const Vector& vecStart, const Vector& vecEnd, IGNORE_MONSTERS igmon, IGNORE_GLASS ignoreGlass, edict_t* pentIgnore)
{
	static TraceResult ptr;

	TRACE_LINE(vecStart, vecEnd, (igmon == ignore_monsters ? TRUE : FALSE) | (ignoreGlass ? 0x100 : 0), pentIgnore, &ptr);

	return &ptr;
}
/// <summary>
/// returns classname of edict
/// </summary>
/// <param name="pEdict"></param>
/// <returns>classname of edict</returns>
const char* RCBotUtils::GetClassname(edict_t* pEdict)
{
	return STRING(pEdict->v.classname);
}
/// <summary>
/// converts a vector to angles
/// </summary>
/// <param name="vec"></param>
/// <returns></returns>
Vector RCBotUtils::VectorToAngles(Vector& vec)
{
	Vector vOut;

	VEC_TO_ANGLES(vec, vOut);

	return vOut;
}
/// <summary>
/// Converts angles to vectors
/// </summary>
/// <param name="vec"></param>
void RCBotUtils::MakeVectors(Vector &vec)
{
	MAKE_VECTORS(vec);
}
/// <summary>
/// displays a message to pEntity in Console
/// if pEntity is nullptr, displays on server
/// </summary>
/// <param name="pEntity">client to show (may be nullptr)</param>
/// <param name="errorlevel"></param>
/// <param name="fmt"></param>
/// <param name="">formatting objects</param>
void RCBotUtils:: Message(edict_t* pEntity, MessageErrorLevel errorlevel, char* fmt, ...)
{
	va_list argptr;
	static char string[1024];

	va_start(argptr, fmt);
	vsprintf(string, fmt, argptr);
	va_end(argptr);

	if (pEntity != nullptr)
	{
		if (IS_DEDICATED_SERVER())
		{
			printf("%s%s Message : %s\n", BOT_DBG_MSG_TAG, STRING(pEntity->v.netname), string);
		}

		if (g_Messages->getCurrentMessage() ==nullptr)
			// cant do a message at the moment
		{
			CLIENT_PRINTF(pEntity, print_console, BOT_DBG_MSG_TAG);
			CLIENT_PRINTF(pEntity, print_console, string);
			CLIENT_PRINTF(pEntity, print_console, "\n");
		}
		else
			ALERT(at_console, string);
	}
	else
	{
			switch (errorlevel)
			{
			case MessageErrorLevel::Warning:
			{
				UTIL_LogPrintf("!!!!ERROR : %s%s\n", BOT_DBG_MSG_TAG, string);

				ALERT(at_console, BOT_DBG_MSG_TAG);

				//BotFile_Write(string);
				ALERT(at_error, string);

				exit(0);
			}
			break;
			case MessageErrorLevel::Critical:
				UTIL_LogPrintf("!!!!ERROR : %s%s\n", BOT_DBG_MSG_TAG, string);

				ALERT(at_console, BOT_DBG_MSG_TAG);

				//BotFile_Write(string);
				ALERT(at_error, string);

				exit(0);

				break;
			default:
				g_engfuncs.pfnServerPrint(BOT_DBG_MSG_TAG);
				g_engfuncs.pfnServerPrint(string);
				g_engfuncs.pfnServerPrint("\n");
			}
	}
}

bool RCBotUtils::isVisible(Vector &vFrom, Vector &vTo, IGNORE_MONSTERS im, IGNORE_GLASS ig, edict_t *pIgnore)
{
	TraceResult *tr;

	tr = Traceline(vFrom, vTo, im, ig, pIgnore);

	return tr->flFraction >= 1.0f;
}

void RCBotUtils::mapInit()
{
	
}
/// <summary>
/// draws a beam for one second 
/// </summary>
/// <param name="pClient"></param>
/// <param name="vFrom"></param>
/// <param name="vTo"></param>
/// <param name="vColour"></param>
void RCBotUtils::drawBeam(edict_t* pClient, const Vector& vFrom, const Vector& vTo, const Colour& vColour, int iTexture)
{
	const uint8_t beamWidth = 32;
	const uint8_t beamNoise = 8;
	const uint8_t beamSpeed = 8;

	// PM - Use MSG_ONE_UNRELIABLE
//    - no overflows!
	MESSAGE_BEGIN(MSG_ONE_UNRELIABLE, SVC_TEMPENTITY, NULL, pClient);
	WRITE_BYTE(TE_BEAMPOINTS);
	WRITE_COORD(vFrom.x);
	WRITE_COORD(vFrom.y);
	WRITE_COORD(vFrom.z);
	WRITE_COORD(vTo.x);
	WRITE_COORD(vTo.y);
	WRITE_COORD(vTo.z);
	WRITE_SHORT(iTexture);
	WRITE_BYTE(1); // framestart
	WRITE_BYTE(10); // framerate
	WRITE_BYTE(10); // life in 0.1's
	WRITE_BYTE(beamWidth); // width
	WRITE_BYTE(beamNoise);  // noise

	WRITE_BYTE(vColour.r);   // r, g, b
	WRITE_BYTE(vColour.g);   // r, g, b
	WRITE_BYTE(vColour.b);   // r, g, b

	WRITE_BYTE(vColour.a);   // brightness
	WRITE_BYTE(beamSpeed);    // speed
	MESSAGE_END();
}
/// <summary>
/// finds a player with the name
/// </summary>
/// <param name="szName">name of play to find</param>
/// <returns></returns>
edict_t* RCBotUtils::findPlayer(const char* szName)
{
	edict_t* pent = NULL;
	const int length = strlen(szName);
	int i;

	for (i = 1; i <= gpGlobals->maxClients; i++)
	{

		char arg_lwr[128];
		char pent_lwr[128];

		pent = INDEXENT(i);

		if (FNullEnt(pent))
			continue;

		strncpy(arg_lwr, szName,127);
		arg_lwr[127] = 0;
		strncpy(pent_lwr, STRING(pent->v.netname),127);
		pent_lwr[127] = 0;

		RCBotStrings::stringLower(arg_lwr);
		RCBotStrings::stringLower(pent_lwr);

		if (strncmp(arg_lwr, pent_lwr, length) == 0)
		{
			return pent;
		}
	}

	return nullptr;
}
/// <summary>
/// wraps angles between -180 and 180
/// </summary>
/// <param name="vAngles"></param>
void RCBotUtils::FixAngles(Vector *vAngles)
{
	FixAngle(&vAngles->x);
	FixAngle(&vAngles->y);
	FixAngle(&vAngles->z);
}
/// <summary>
/// fix single angle between -180 and +180
/// </summary>
/// <param name="fAngle"></param>
void RCBotUtils::FixAngle(float* fAngle)
{
	if (*fAngle < -180)
	{
		*fAngle += 360.0;
	}
	else if (*fAngle > 180)
	{
		*fAngle -= 360.0;
	}
}
/// <summary>
/// 
/// </summary>
/// <param name="fSpeed"></param>
/// <param name="fIdeal"></param>
/// <param name="fCurrent"></param>
/// <param name="fUpdate"></param>
void RCBotUtils::ChangeAngle(float* fSpeed, const float* fIdeal, float* fCurrent, float* fUpdate)
{
	float fCurrent180;  // current +/- 180 degrees
	float fDiff;

	// turn from the current v_angle yaw to the ideal_yaw by selecting
	// the quickest way to turn to face that direction

	// find the difference in the current and ideal angle
	fDiff = fabs(*fCurrent - *fIdeal);

	// check if the bot is already facing the ideal_yaw direction...
	if (fDiff <= 0.1)
	{
		*fSpeed = fDiff;

		return;
	}

	// check if difference is less than the max degrees per turn
	if (fDiff < *fSpeed)
		*fSpeed = fDiff;  // just need to turn a little bit (less than max)

	 // here we have four cases, both angle positive, one positive and
	 // the other negative, one negative and the other positive, or
	 // both negative.  handle each case separately...

	if (*fCurrent >= 0 && *fIdeal >= 0)  // both positive
	{
		if (*fCurrent > *fIdeal)
			*fCurrent -= *fSpeed;
		else
			*fCurrent += *fSpeed;
	}
	else if (*fCurrent >= 0 && *fIdeal < 0)
	{
		fCurrent180 = *fCurrent - 180;

		if (fCurrent180 > *fIdeal)
			*fCurrent += *fSpeed;
		else
			*fCurrent -= *fSpeed;
	}
	else if (*fCurrent < 0 && *fIdeal >= 0)
	{
		fCurrent180 = *fCurrent + 180;
		if (fCurrent180 > *fIdeal)
			*fCurrent += *fSpeed;
		else
			*fCurrent -= *fSpeed;
	}
	else  // (current < 0) && (ideal < 0)  both negative
	{
		if (*fCurrent > *fIdeal)
			*fCurrent -= *fSpeed;
		else
			*fCurrent += *fSpeed;
	}

	FixAngle(fCurrent);

	*fUpdate = *fCurrent;
}

// -------------------
// Legacy functions
// -------------------


//=========================================================
// UTIL_LogPrintf - Prints a logged message to console.
// Preceded by LOG: ( timestamp ) < message >
//=========================================================
void UTIL_LogPrintf(char* fmt, ...)
{
	/*va_list        argptr;
	static char    string[1024];

	va_start(argptr, fmt);
	vsprintf(string, fmt, argptr);
	va_end(argptr);

	// Print to server console
	ALERT(at_logged, "%s", string);*/
}
