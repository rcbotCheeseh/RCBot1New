/*
 *    This file is part of RCBot.
 *
 *    RCBot by Paul Murphy adapted from botman's template 3.
 *
 *    RCBot is free software; you can redistribute it and/or modify it
 *    under the terms of the GNU General Public License as published by the
 *    Free Software Foundation; either version 2 of the License, or (at
 *    your option) any later version.
 *
 *    RCBot is distributed in the hope that it will be useful, but
 *    WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *    General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with RCBot; if not, write to the Free Software Foundation,
 *    Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *    In addition, as a special exception, the author gives permission to
 *    link the code of this program with the Half-Life Game Engine ("HL
 *    Engine") and Modified Game Libraries ("MODs") developed by Valve,
 *    L.L.C ("Valve").  You must obey the GNU General Public License in all
 *    respects for all of the code used other than the HL Engine and MODs
 *    from Valve.  If you modify this file, you may extend this exception
 *    to your version of the file, but you are not obligated to do so.  If
 *    you do not wish to do so, delete this exception statement from your
 *    version.
 *
 */
 //////////////////////////////////////////////////
 // RCBOT : Paul Murphy @ {cheeseh@rcbot.net}
 //
 // (http://www.rcbot.net)
 //
 // Based on botman's High Ping Bastard bot
 //
 // (http://planethalflife.com/botman/)
 //
 // dll.cpp
 //
 //////////////////////////////////////////////////
 //
 // engine + initializing functions + bot interface
 //

#include "extdll.h"

#include "h_export_meta.h"
#include "dllapi.h"
#include "meta_api.h"
#include "engine.h"
#include "rcbot_const.h"
#include "entity_state.h"
#include "rcbot_engine_funcs.h"
#include <stdint.h>
#include "rcbot_commands.h"
#include "rcbot_manager.h"
#include "rcbot_navigator.h"


#ifdef _WIN32
#define strdup _strdup
#endif

extern enginefuncs_t g_engfuncs;
extern int debug_engine;
extern globalvars_t* gpGlobals;
extern char* g_argv;

DLL_FUNCTIONS other_gFunctionTable;
DLL_GLOBAL const Vector g_vecZero = Vector(0, 0, 0);

cvar_t bot_ver_cvar = { BOT_VER_CVAR,BOT_VER,FCVAR_SERVER };

FILE* fpMapConfig = nullptr;

void UpdateClientData(const struct edict_s* ent, int sendweapons, struct clientdata_s* cd);

//////////////////////////////////////////////////////////////////////
void GameDLLInit(void)
{
	RETURN_META(MRES_IGNORED);
}

///////////////////////////////////////////////////////////////////
/*

	DispatchSpawn is called when an entity spawns in the game
	the entity that spawns is the *pent argument

*/
///////////////////////////////////////////////////////////////////
int DispatchSpawn(edict_t* pent)
{
	if (gpGlobals->deathmatch)
	{
		char* pClassname = const_cast<char*>(STRING(pent->v.classname));

		if (debug_engine)
		{
			FILE* fp;

			fp = fopen("bot.txt", "a");
			fprintf(fp, "DispatchSpawn: %p %s\n", pent, pClassname);
			if (pent->v.model != 0)
				fprintf(fp, " model=%s\n", STRING(pent->v.model));
			fclose(fp);
		}

		if (strcmp(pClassname, "worldspawn") == 0)
		{
			// do level initialization stuff here...
			gRCBotManager.LevelInit();
		}
	}

	RETURN_META_VALUE(MRES_IGNORED, 0);
}

///////////////////////////////////////////////////////////////////////////
/*
	DispatchThink is called when an entity thinks...
*/
///////////////////////////////////////////////////////////////////////////
void DispatchThink(edict_t* pent)
{

	RETURN_META(MRES_IGNORED);

}
///////////////////////////////////////////////////////////////////////////
/*
	DispatchUse is called when pentOther uses pentUsed
*/
///////////////////////////////////////////////////////////////////////////
void DispatchUse(edict_t* pentUsed, edict_t* pentOther)
{
	RETURN_META(MRES_IGNORED);

}
///////////////////////////////////////////////////////////////////////////
void DispatchTouch(edict_t* pentTouched, edict_t* pentOther)
{
	RETURN_META(MRES_IGNORED);

}
///////////////////////////////////////////////////////////////////////////
void DispatchBlocked(edict_t* pentBlocked, edict_t* pentOther)
{
	RETURN_META(MRES_IGNORED);

}
///////////////////////////////////////////////////////////////////////////
void DispatchKeyValue(edict_t* pentKeyvalue, KeyValueData* pkvd)
{
	//gBotGlobals.KeyValue(pentKeyvalue, pkvd);

	RETURN_META(MRES_IGNORED);

}
///////////////////////////////////////////////////////////////////////////
void DispatchSave(edict_t* pent, SAVERESTOREDATA* pSaveData)
{

	RETURN_META(MRES_IGNORED);

}
///////////////////////////////////////////////////////////////////////////
int DispatchRestore(edict_t* pent, SAVERESTOREDATA* pSaveData, int globalEntity)
{

	RETURN_META_VALUE(MRES_IGNORED, 0);

}
///////////////////////////////////////////////////////////////////////////
void DispatchObjectCollsionBox(edict_t* pent)
{

	RETURN_META(MRES_IGNORED);

}
///////////////////////////////////////////////////////////////////////////
void SaveWriteFields(SAVERESTOREDATA* pSaveData, const char* pname, void* pBaseData, TYPEDESCRIPTION* pFields, int fieldCount)
{

	RETURN_META(MRES_IGNORED);

}
///////////////////////////////////////////////////////////////////////////
void SaveReadFields(SAVERESTOREDATA* pSaveData, const char* pname, void* pBaseData, TYPEDESCRIPTION* pFields, int fieldCount)
{

	RETURN_META(MRES_IGNORED);

}
///////////////////////////////////////////////////////////////////////////
void SaveGlobalState(SAVERESTOREDATA* pSaveData)
{

	RETURN_META(MRES_IGNORED);

}
///////////////////////////////////////////////////////////////////////////
void RestoreGlobalState(SAVERESTOREDATA* pSaveData)
{

	RETURN_META(MRES_IGNORED);

}
///////////////////////////////////////////////////////////////////////////
void ResetGlobalState(void)
{

	RETURN_META(MRES_IGNORED);

}
///////////////////////////////////////////////////////////////////////////
BOOL ClientConnect(edict_t* pEntity, const char* pszName, const char* pszAddress, char szRejectReason[128])
{
	

	RETURN_META_VALUE(MRES_IGNORED, 0);

}
///////////////////////////////////////////////////////////////////////////
void ClientDisconnect(edict_t* pEntity)
{
	gRCBotNavigatorNodes->removeEditor(pEntity);

	RETURN_META(MRES_IGNORED);

}
///////////////////////////////////////////////////////////////////////////
void ClientKill(edict_t* pEntity)
{
	if (debug_engine) { FILE* fp; fp = fopen("bot.txt", "a");
		fprintf(fp, "ClientKill: %x\n", unsigned(pEntity));
		fclose(fp); }

	RETURN_META(MRES_IGNORED);

}
///////////////////////////////////////////////////////////////////////////
void ClientPutInServer(edict_t* pEntity)
{
	if (debug_engine) { FILE* fp; fp = fopen("bot.txt", "a");
		fprintf(fp, "ClientPutInServer: %x\n", unsigned(pEntity));
		fclose(fp); }

	RETURN_META(MRES_IGNORED);

}


///////////////////////////////////////////////////////////////////////////
void ClientCommand(edict_t* pEntity)
{
	RCBotCommands_MainCommand::setClient(pEntity);

	if (RCBotCommands_MainCommand::ClientCommand() == RCBotCommandReturn::Ok)
		RETURN_META(MRES_SUPERCEDE);

	RETURN_META(MRES_IGNORED);
}
///////////////////////////////////////////////////////////////////////////
void ClientUserInfoChanged(edict_t* pEntity, char* infobuffer)
{
	if (debug_engine) { FILE* fp; fp = fopen("bot.txt", "a");
		fprintf(fp, "ClientUserInfoChanged: pEntity=%x infobuffer=%s\n", unsigned(pEntity), infobuffer);
		fclose(fp); }

	RETURN_META(MRES_IGNORED);
}

void ServerActivate(edict_t* pEdictList, int edictCount, int clientMax)
{
	//	gBotGlobals.m_iJoiningClients = 0;

	RETURN_META(MRES_IGNORED);

}
///////////////////////////////////////////////////////////////////////////
void ServerDeactivate(void)
{
	// server has finished (map changed for example) but new map
	// hasn't loaded yet!!

	RETURN_META(MRES_IGNORED);

}
///////////////////////////////////////////////////////////////////////////
void PlayerPreThink(edict_t* pEntity)
{
	RETURN_META(MRES_IGNORED);
}

void PlayerPostThink(edict_t* pEntity)
{
	RETURN_META(MRES_IGNORED);
}

void StartFrame(void)
{
	gRCBotManager.Think();

	RETURN_META(MRES_IGNORED);
}

void ParmsNewLevel(void)
{
	gRCBotManager.LevelInit();
	gRCBotNavigatorNodes->clearEditors();
	gRCBotNavigatorNodes->Load(STRING(gpGlobals->mapname));

	RETURN_META(MRES_IGNORED);
}

void ParmsChangeLevel(void)
{
	RETURN_META(MRES_IGNORED);
}

const char* GetGameDescription(void)
{
	RETURN_META_VALUE(MRES_IGNORED, nullptr);

}

void PlayerCustomization(edict_t* pEntity, customization_t* pCust)
{
	if (debug_engine) { FILE* fp; fp = fopen("bot.txt", "a");
		fprintf(fp, "PlayerCustomization: %x\n", unsigned(pEntity));
		fclose(fp); }

	RETURN_META(MRES_IGNORED);

}

void SpectatorConnect(edict_t* pEntity)
{

	RETURN_META(MRES_IGNORED);

}

void SpectatorDisconnect(edict_t* pEntity)
{
	RETURN_META(MRES_IGNORED);

}

void SpectatorThink(edict_t* pEntity)
{
	RETURN_META(MRES_IGNORED);

}

void Sys_Error(const char* error_string)
{
	char szError[1024];

	sprintf(szError, "System Error: \"%s\"\n", error_string);
	szError[1023] = 0;

	RETURN_META(MRES_IGNORED);

}

void PM_Move(struct playermove_s* ppmove, int server)
{

	RETURN_META(MRES_IGNORED);

}

void PM_Init(struct playermove_s* ppmove)
{

	RETURN_META(MRES_IGNORED);

}

char PM_FindTextureType(char* name)
{

	RETURN_META_VALUE(MRES_IGNORED, 0);

}

void SetupVisibility(edict_t* pViewEntity, edict_t* pClient, unsigned char** pvs, unsigned char** pas)
{

	RETURN_META(MRES_IGNORED);

}

void UpdateClientData(const struct edict_s* ent, int sendweapons, struct clientdata_s* cd)
{
	RETURN_META(MRES_IGNORED);

}

int AddToFullPack(struct entity_state_s* state, int e, edict_t* ent, edict_t* host, int hostflags, int player, unsigned char* pSet)
{
	RETURN_META_VALUE(MRES_IGNORED, 0);

}

void CreateBaseline(int player, int eindex, struct entity_state_s* baseline, struct edict_s* entity, int playermodelindex, vec3_t player_mins, vec3_t player_maxs)
{
	RETURN_META(MRES_IGNORED);

}

void RegisterEncoders(void)
{
	RETURN_META(MRES_IGNORED);

}

int GetWeaponData(struct edict_s* player, struct weapon_data_s* info)
{
	RETURN_META_VALUE(MRES_IGNORED, 0);

}

void CmdStart(const edict_t* player, const struct usercmd_s* cmd, unsigned int random_seed)
{
	RETURN_META(MRES_IGNORED);

}

void CmdEnd(const edict_t* player)
{
	RETURN_META(MRES_IGNORED);

}

int ConnectionlessPacket(const struct netadr_s* net_from, const char* args, char* response_buffer, int* response_buffer_size)
{
	RETURN_META_VALUE(MRES_IGNORED, 0);

}

int GetHullBounds(int hullnumber, float* mins, float* maxs)
{
	RETURN_META_VALUE(MRES_IGNORED, 0);

}

void CreateInstancedBaselines(void)
{

	RETURN_META(MRES_IGNORED);

}

int InconsistentFile(const edict_t* player, const char* filename, char* disconnect_message)
{
	if (debug_engine) { FILE* fp; fp = fopen("bot.txt", "a");
		fprintf(fp, "InconsistentFile: %x filename=%s\n", unsigned(player), filename);
		fclose(fp); }

	RETURN_META_VALUE(MRES_IGNORED, 0);

}

int AllowLagCompensation(void)
{
	RETURN_META_VALUE(MRES_IGNORED, 0);

}

DLL_FUNCTIONS gFunctionTable =
{
   GameDLLInit,               //pfnGameInit
   DispatchSpawn,             //pfnSpawn
   DispatchThink,             //pfnThink
   DispatchUse,               //pfnUse
   DispatchTouch,             //pfnTouch
   DispatchBlocked,           //pfnBlocked
   DispatchKeyValue,          //pfnKeyValue
   DispatchSave,              //pfnSave
   DispatchRestore,           //pfnRestore
   DispatchObjectCollsionBox, //pfnAbsBox

   SaveWriteFields,           //pfnSaveWriteFields
   SaveReadFields,            //pfnSaveReadFields

   SaveGlobalState,           //pfnSaveGlobalState
   RestoreGlobalState,        //pfnRestoreGlobalState
   ResetGlobalState,          //pfnResetGlobalState

   ClientConnect,             //pfnClientConnect
   ClientDisconnect,          //pfnClientDisconnect
   ClientKill,                //pfnClientKill
   ClientPutInServer,         //pfnClientPutInServer
   ClientCommand,             //pfnClientCommand
   ClientUserInfoChanged,     //pfnClientUserInfoChanged
   ServerActivate,            //pfnServerActivate
   ServerDeactivate,          //pfnServerDeactivate

   PlayerPreThink,            //pfnPlayerPreThink
   PlayerPostThink,           //pfnPlayerPostThink

   StartFrame,                //pfnStartFrame
   ParmsNewLevel,             //pfnParmsNewLevel
   ParmsChangeLevel,          //pfnParmsChangeLevel

   GetGameDescription,        //pfnGetGameDescription    Returns string describing current .dll game.
   PlayerCustomization,       //pfnPlayerCustomization   Notifies .dll of new customization for player.

   SpectatorConnect,          //pfnSpectatorConnect      Called when spectator joins server
   SpectatorDisconnect,       //pfnSpectatorDisconnect   Called when spectator leaves the server
   SpectatorThink,            //pfnSpectatorThink        Called when spectator sends a command packet (usercmd_t)

   Sys_Error,                 //pfnSys_Error          Called when engine has encountered an error

   PM_Move,                   //pfnPM_Move
   PM_Init,                   //pfnPM_Init            Server version of player movement initialization
   PM_FindTextureType,        //pfnPM_FindTextureType

   SetupVisibility,           //pfnSetupVisibility        Set up PVS and PAS for networking for this client
   UpdateClientData,          //pfnUpdateClientData       Set up data sent only to specific client
   AddToFullPack,             //pfnAddToFullPack
   CreateBaseline,            //pfnCreateBaseline        Tweak entity baseline for network encoding, allows setup of player baselines, too.
   RegisterEncoders,          //pfnRegisterEncoders      Callbacks for network encoding
   GetWeaponData,             //pfnGetWeaponData
   CmdStart,                  //pfnCmdStart
   CmdEnd,                    //pfnCmdEnd
   ConnectionlessPacket,      //pfnConnectionlessPacket
   GetHullBounds,             //pfnGetHullBounds
   CreateInstancedBaselines,  //pfnCreateInstancedBaselines
   InconsistentFile,          //pfnInconsistentFile
   AllowLagCompensation,      //pfnAllowLagCompensation
};

// Fakeclient Command code
//
// by Pierre-Marie Baty
// racc bot (Check it out! : racc.bots-united.com)

void FakeClientCommand(edict_t* pFakeClient, const char* fmt, ...)
{
	// the purpose of this function is to provide fakeclients (bots) with the same client
	// command-scripting advantages (putting multiple commands in one line between semicolons)
	// as real players. It is an improved version of botman's FakeClientCommand, in which you
	// supply directly the whole string as if you were typing it in the bot's "console". It
	// is supposed to work exactly like the pfnClientCommand (server-sided client command).

	va_list argptr;
	static char command[256];
	int length, fieldstart, fieldstop, i, index, stringindex = 0;

	if (!pFakeClient)
	{
		//BugMessage(nullptr, "FakeClientCommand : No fakeclient!");
		return; // reliability check
	}

	if (fmt == nullptr)
		return;

	// concatenate all the arguments in one string
	va_start(argptr, fmt);
	vsprintf(command, fmt, argptr);
	va_end(argptr);

	if (command == nullptr || *command == 0 || *command == '\n')
	{
		//BugMessage(nullptr, "FakeClientCommand : No command!");
		return; // if nothing in the command buffer, return
	}

    RCBotCommands_MainCommand::IsFakeClientCommand = TRUE; // set the "fakeclient command" flag
	length = strlen(command); // get the total length of the command string

	// process all individual commands (separated by a semicolon) one each a time
	while (stringindex < length)
	{
		fieldstart = stringindex; // save field start position (first character)
		while (stringindex < length && command[stringindex] != ';')
			stringindex++; // reach end of field
		if (command[stringindex - 1] == '\n')
			fieldstop = stringindex - 2; // discard any trailing '\n' if needed
		else
			fieldstop = stringindex - 1; // save field stop position (last character before semicolon or end)
		for (i = fieldstart; i <= fieldstop; i++)
			g_argv[i - fieldstart] = command[i]; // store the field value in the g_argv global string
		g_argv[i - fieldstart] = 0; // terminate the string
		stringindex++; // move the overall string index one step further to bypass the semicolon

		index = 0;
		RCBotCommands_MainCommand::NumArgs = 0; // let's now parse that command and count the different arguments

		// count the number of arguments
		while (index < i - fieldstart)
		{
			while (index < i - fieldstart && g_argv[index] == ' ')
				index++; // ignore spaces

			// is this field a group of words between quotes or a single word ?
			if (g_argv[index] == '"')
			{
				index++; // move one step further to bypass the quote
				while (index < i - fieldstart && g_argv[index] != '"')
					index++; // reach end of field
				index++; // move one step further to bypass the quote
			}
			else
				while (index < i - fieldstart && g_argv[index] != ' ')
					index++; // this is a single word, so reach the end of field

			RCBotCommands_MainCommand::NumArgs++; // we have processed one argument more
		}

		MDLL_ClientCommand(pFakeClient);
	}

	g_argv[0] = 0; // when it's done, reset the g_argv field
	RCBotCommands_MainCommand::IsFakeClientCommand = FALSE; // reset the "fakeclient command" flag
	RCBotCommands_MainCommand::NumArgs = 0; // and the argument count
}

//////////////////////////////////////////////////////////////////////////////
// METAMOD REQUIRED...

#ifdef RCBOT_META_BUILD
C_DLLEXPORT int GetEntityAPI2(DLL_FUNCTIONS* pFunctionTable, int* interfaceVersion)
{
	// this is one of the initialization functions hooked by metamod in the gamedll API
	if (!pFunctionTable) {
		//UTIL_LogPrintf("GetEntityAPI2 called with null pFunctionTable");
		return FALSE;
	}
	else if (*interfaceVersion != INTERFACE_VERSION) 
	{
		//UTIL_LogPrintf("GetEntityAPI2 version mismatch; requested=%d ours=%d", *interfaceVersion, INTERFACE_VERSION);
		//! Tell engine what version we had, so it can figure out who is out of date.
		*interfaceVersion = INTERFACE_VERSION;
		return FALSE;
	}

	// gFunctionTable defined in dll.cpp
	// copy the whole table for metamod to know which functions we are using here
	memcpy(pFunctionTable, &gFunctionTable, sizeof(DLL_FUNCTIONS));

	return TRUE; // alright
}

#endif