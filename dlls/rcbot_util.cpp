#include "rcbot_utils.h"
#include "meta_api.h"
#include "rcbot_const.h"
#include "extdll.h"
#include "rcbot_message.h"
#include <string.h>

TraceResult *RCBotUtils::Traceline (const Vector& vecStart, const Vector& vecEnd, IGNORE_MONSTERS igmon, IGNORE_GLASS ignoreGlass, edict_t* pentIgnore)
{
	static TraceResult ptr;

	TRACE_LINE(vecStart, vecEnd, (igmon == ignore_monsters ? TRUE : FALSE) | (ignoreGlass ? 0x100 : 0), pentIgnore, &ptr);

	return &ptr;
}

const char* RCBotUtils::GetClassname(edict_t* pEdict)
{
	return STRING(pEdict->v.classname);
}

Vector RCBotUtils::VectorToAngles(Vector& vec)
{
	Vector vOut;

	VEC_TO_ANGLES(vec, vOut);

	return vOut;
}

void RCBotUtils::MakeVectors(Vector &vec)
{
	MAKE_VECTORS(vec);
}

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

		if (RCBotMessage::CurrentMessage==nullptr)
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

edict_t* RCBotUtils::findPlayer(const char* szName)
{
	edict_t* pent = NULL;

	int i;

	for (i = 1; i <= gpGlobals->maxClients; i++)
	{
		pent = INDEXENT(i);

		if (pent != NULL)
		{
			if (!pent->free)
			{
				const int length = strlen(szName);

				char arg_lwr[128];
				char pent_lwr[128];

				strncpy(arg_lwr, szName,127);
				arg_lwr[127] = 0;
				strncpy(pent_lwr, STRING(pent->v.netname),127);
				pent_lwr[127] = 0;

				strlwr(arg_lwr);
				strlwr(pent_lwr);

				if (strncmp(arg_lwr, pent_lwr, length) == 0)
				{
					return pent;
				}
			}
		}
	}

	return nullptr;
	
}

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
