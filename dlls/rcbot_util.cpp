#include "rcbot_utils.h"
#include "meta_api.h"
#include "rcbot_const.h"
#include "extdll.h"
#include "rcbot_message.h"

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
			case MessageErrorLevel::MessageErrorLevel_Warning:
			{
				UTIL_LogPrintf("!!!!ERROR : %s%s\n", BOT_DBG_MSG_TAG, string);

				ALERT(at_console, BOT_DBG_MSG_TAG);

				//BotFile_Write(string);
				ALERT(at_error, string);

				exit(0);
			}
			break;
			case MessageErrorLevel::MessageErrorLevel_Critical:
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
