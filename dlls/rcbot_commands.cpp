#include "extdll.h"

#include "h_export_meta.h"
#include "dllapi.h"
#include "meta_api.h"
#include "engine.h"
#include "rcbot_commands.h"
#include "rcbot_strings.h"
#include "rcbot_file.h"
#include "rcbot_utils.h"
#include "rcbot_manager.h"
#include "rcbot_navigator.h"
#include <stdint.h>

#define RCBOT_ACCESSORS_FILE "rcbot_accesslevels"
#define RCBOT_ACCESSORS_FOLDER "accessors"
#define RCBOT_ACCESSORS_EXTENSION "ini"

bool RCBotCommands_MainCommand::IsFakeClientCommand = false;
int RCBotCommands_MainCommand::NumArgs = 0;
edict_t *RCBotCommands_MainCommand::m_pClient = nullptr;

RCBotCommands_MainCommand gRCBotCommands;

bool RCBotCommand::isCommand(const char* szCommand)
{
	return gRCBotStrings.stringMatch(szCommand, m_szCommand);
}

RCBotCommand *RCBotCommands::findCommand(const char* szCommand)
{
	for (auto *command : m_Commands)
	{
		if (command->isCommand(szCommand))
			return command;
	}

	return nullptr;
}

RCBotCommandAccessor::RCBotCommandAccessor(const char* szSteamID, uint32_t iLevel)
{
	this->m_szSteamID = szSteamID;
	this->m_iLevel = iLevel;
}

RCBotCommandAccessors :: RCBotCommandAccessors()
{
	// Load From File
	RCBotFile* file = RCBotFile::Open(RCBOT_ACCESSORS_FOLDER,RCBOT_ACCESSORS_FILE, RCBOT_ACCESSORS_EXTENSION, "r");
	const char* szLine;

	while ((szLine = file->readLine()) != nullptr)
	{
		std::vector<const char*> split;
		const char* szSteamID = nullptr;
		int iLevel = 0;

		gRCBotStrings.split(szLine, &split, ',');

		for (uint32_t i = 0; i < split.size(); i++)
		{
			switch (i)
			{
			case 0: // Steam ID
				szSteamID = split[0];
				break;
			case 1: // Level
				iLevel = atoi(split[1]);
				break;
			}
		}

		if (szSteamID != nullptr && iLevel)
		{
			m_Accessors.push_back(new RCBotCommandAccessor(szSteamID, iLevel));
		}
	}

	file->close();
}

RCBotCommand::RCBotCommand(const char* szCommand, const char* szHelp, const char *szUsage)
{
	m_szCommand = gRCBotStrings.add(szCommand);
	m_szHelp = gRCBotStrings.add(szHelp);
	m_szUsage = gRCBotStrings.add(szUsage);
}

void RCBotCommand::showUsage(edict_t* pClient)
{
	if ( m_szUsage != nullptr )
		RCBotUtils::Message(pClient, MessageErrorLevel::Information, "\"%s\" usage: [%s]\n", m_szCommand, m_szUsage);
}

RCBotCommandReturn RCBotCommands::execute(edict_t* pClient, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	for (auto* command : m_Commands)
	{
		if (command->isCommand(arg1))
		{
			return command->execute(pClient, arg2, arg3, arg4, arg5, nullptr);
		}

		//RCBotUtils::Message(pClient, MessageErrorLevel_Information, "%s : [%s]",command->getCommand(),command->getHelp());
	}
	for (auto* command : m_Commands)
	{
		RCBotUtils::Message(pClient, MessageErrorLevel::Information, "%s : [%s]",command->getCommand(),command->getHelp());
	}

	return RCBotCommandReturn::Ok;
}

RCBotCommands::RCBotCommands(const char* szCommand) : RCBotCommand (szCommand,"[sub command]","")
{

}


RCBotCommandReturn RCBotCommand_AddProfileCommand:: execute(edict_t* pClient, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	if (arg1 && *arg1)
	{
		RCBotProfile* profile = RCBotProfile::FromLine(arg1);

		if (profile != nullptr)
		{
			gRCBotProfiles->addProfile(profile);

			RCBotUtils::Message(pClient,MessageErrorLevel::Information,"Bot Profile Added");
		}
		else
		{
			showUsage(pClient);
		}
	}
	else
		showUsage(pClient);

	return RCBotCommandReturn::Ok;
}

RCBotCommandReturn RCBotCommand_RemoveProfileCommand::execute(edict_t* pClient, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	/*if (arg1 && *arg1)
	{
		int id = atoi(arg1);

		if (gRCBotProfiles.remove(id))
		{
			RCBotUtils::Message(pClient, MessageErrorLevel::MessageErrorLevel_Information, "Removed profile...\n");
		}
		else
			RCBotUtils::Message(pClient, MessageErrorLevel::MessageErrorLevel_Information, "Couldn't find profile...\n");
	}
	else
		showUsage(pClient);*/
	return RCBotCommandReturn::Ok;
};


RCBotCommandReturn RCBotCommand_ListProfilesCommand::execute(edict_t* pClient, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	RCBotUtils::Message(pClient, MessageErrorLevel::Information, "Listing profiles...\n");
	gRCBotProfiles->List(pClient);
	return RCBotCommandReturn::Ok;
};


RCBotCommands_MainCommand::RCBotCommands_MainCommand() : RCBotCommands("rcbot")
{
	
	addCommand(new RCBotCommands_BotCommand());
	addCommand(new RCBotCommands_ProfileCommand());
	addCommand(new RCBotCommands_UtilCommand());
	addCommand(new RCBotCommands_WaypointCommand());
	addCommand(new RCBotCommands_PathWaypointCommand());
}

RCBotCommands_BotCommand::RCBotCommands_BotCommand() : RCBotCommands("bot")
{
	addCommand(new RCBotCommand_AddBotCommand());
}

RCBotCommandReturn RCBotCommand_AddBotCommand::execute(edict_t* pClient, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	gRCBotManager.IncreaseQuota();

	return RCBotCommandReturn::Ok;
}

RCBotCommandReturn RCBotCommands_MainCommand::ClientCommand()
{
	RCBotCommandReturn ret = RCBotCommandReturn::Continue;

	if (!IsFakeClientCommand)
	{
		const char* pcmd = CMD_ARGV(0);
		const char* arg1 = CMD_ARGV(1);
		const char* arg2 = CMD_ARGV(2);
		const char* arg3 = CMD_ARGV(3);
		const char* arg4 = CMD_ARGV(4);
		const char* arg5 = CMD_ARGV(5);

		if (pcmd)
		{
			// Do Stuff

			if (gRCBotCommands.isCommand(pcmd))
			{
				ret = gRCBotCommands.execute(m_pClient, arg1, arg2, arg3, arg4, arg5);
			}
		}
	}

	return ret;

}


RCBotCommandReturn RCBotCommand_TeleportToPlayerCommand::execute(edict_t* pClient, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	edict_t* pTeleportTo = nullptr;

	if (*arg1 && arg1)
	{
		pTeleportTo = RCBotUtils::findPlayer(arg1);
	}

	if (pClient != nullptr && pTeleportTo != nullptr)
	{
		SET_ORIGIN(pClient, pTeleportTo->v.origin + pTeleportTo->v.view_ofs);

		RCBotUtils::Message(NULL, MessageErrorLevel::Information, "%s teleported to %s", STRING(pClient->v.netname),STRING(pTeleportTo->v.netname));
	}
	else
		showUsage(pClient);

	return RCBotCommandReturn::Ok;
}

RCBotCommandReturn RCBotCommand_NoClipCommand::execute(edict_t* pClient, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	edict_t* pPlayer = pClient;

	if (*arg1 && arg1 )
	{
		pPlayer = RCBotUtils::findPlayer(arg1);
	}

	if (pPlayer != nullptr)
	{
		if (pPlayer->v.movetype != MOVETYPE_NOCLIP)
			pPlayer->v.movetype = MOVETYPE_NOCLIP;
		else
			pPlayer->v.movetype = MOVETYPE_WALK;

		RCBotUtils::Message(NULL, MessageErrorLevel::Information, "noclip mode %s on %s", pPlayer->v.movetype == MOVETYPE_NOCLIP ? "enabled" : "disabled", STRING(pPlayer->v.netname));
	}
	else 
		showUsage(pClient);

	return RCBotCommandReturn::Ok;
}

RCBotCommandReturn RCBotCommand_GodModeCommand::execute(edict_t* pClient, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	edict_t* pPlayer = pClient;

	if (*arg1 && arg1)
	{
		pPlayer = RCBotUtils::findPlayer(arg1);
	}

	if (pPlayer != nullptr)
	{
		if (pPlayer->v.takedamage != DAMAGE_NO)
			pPlayer->v.takedamage = DAMAGE_NO;
		else
			pPlayer->v.takedamage = DAMAGE_YES;

		RCBotUtils::Message(NULL, MessageErrorLevel::Information, "god mode %s on %s", pPlayer->v.takedamage == DAMAGE_NO ? "enabled" : "disabled", STRING(pPlayer->v.netname));
	}
	else 
		showUsage(pClient);

	return RCBotCommandReturn::Ok;
}

//////////////////////////////////////////////////
// WAYPOINT COMMANDS
//////////////////////////////////////////////////

RCBotCommands_WaypointCommand :: RCBotCommands_WaypointCommand() : RCBotCommands("waypoint")
{
	addCommand(new RCBotCommand_WaypointOnCommand());
	addCommand(new RCBotCommand_WaypointOffCommand());
	addCommand(new RCBotCommand_WaypointLoadCommand());
	addCommand(new RCBotCommand_WaypointSaveCommand());
	addCommand(new RCBotCommand_WaypointAddCommand());
	addCommand(new RCBotCommand_WaypointDeleteCommand());
}


RCBotCommandReturn RCBotCommand_WaypointAddCommand::execute(edict_t* pClient, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	RCBotNodeEditor *pEditor = gRCBotNavigatorNodes->getEditor(pClient);

	if (pEditor != nullptr)
		pEditor->AddNode();

	return RCBotCommandReturn::Ok;
}

RCBotCommandReturn RCBotCommand_WaypointDeleteCommand::execute(edict_t* pClient, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	RCBotNodeEditor* pEditor = gRCBotNavigatorNodes->getEditor(pClient);

	if (pEditor != nullptr)
		pEditor->RemoveNode();

	return RCBotCommandReturn::Ok;
}



RCBotCommandReturn RCBotCommand_WaypointOnCommand::execute(edict_t* pClient, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	if (pClient != nullptr)
	{
		gRCBotNavigatorNodes->removeEditor(pClient);
		gRCBotNavigatorNodes->addEditor(pClient);
	}

	return RCBotCommandReturn::Ok;
}

RCBotCommandReturn RCBotCommand_WaypointOffCommand::execute(edict_t* pClient, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	if (pClient != nullptr)
	{
		gRCBotNavigatorNodes->removeEditor(pClient);
	}

	return RCBotCommandReturn::Ok;
}

RCBotCommandReturn RCBotCommand_WaypointLoadCommand::execute(edict_t* pClient, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	if (*arg1 && arg1)
		gRCBotNavigatorNodes->Load(arg1);
	else
		gRCBotNavigatorNodes->Load(STRING(gpGlobals->mapname));

	return RCBotCommandReturn::Ok;
}

RCBotCommandReturn RCBotCommand_WaypointSaveCommand::execute(edict_t* pClient, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	if (*arg1 && arg1)
		gRCBotNavigatorNodes->Save(arg1);
	else
		gRCBotNavigatorNodes->Save(STRING(gpGlobals->mapname));

	return RCBotCommandReturn::Ok;
}


//////////////////////////////////////////////////
// PATH WAYPOINT COMMANDS
//////////////////////////////////////////////////

RCBotCommands_PathWaypointCommand::RCBotCommands_PathWaypointCommand() : RCBotCommands("pathwaypoint")
{
	addCommand(new RCBotCommand_PathWaypoint_Create1_Command());
	addCommand(new RCBotCommand_PathWaypoint_Create2_Command());
	addCommand(new RCBotCommand_PathWaypoint_Remove1_Command());
	addCommand(new RCBotCommand_PathWaypoint_Remove2_Command());
}

RCBotCommandReturn RCBotCommand_PathWaypoint_Create1_Command::execute(edict_t* pClient, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	RCBotNodeEditor* pEditor = gRCBotNavigatorNodes->getEditor(pClient);

	if (pEditor != nullptr)
		pEditor->Create1();


	return RCBotCommandReturn::Ok;
}

RCBotCommandReturn RCBotCommand_PathWaypoint_Create2_Command::execute(edict_t* pClient, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	RCBotNodeEditor* pEditor = gRCBotNavigatorNodes->getEditor(pClient);

	if (pEditor != nullptr)
		pEditor->Create2();


	return RCBotCommandReturn::Ok;
}


RCBotCommandReturn RCBotCommand_PathWaypoint_Remove1_Command::execute(edict_t* pClient, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	RCBotNodeEditor* pEditor = gRCBotNavigatorNodes->getEditor(pClient);

	if (pEditor != nullptr)
		pEditor->Remove1();


	return RCBotCommandReturn::Ok;
}

RCBotCommandReturn RCBotCommand_PathWaypoint_Remove2_Command::execute(edict_t* pClient, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	RCBotNodeEditor* pEditor = gRCBotNavigatorNodes->getEditor(pClient);

	if (pEditor != nullptr)
		pEditor->Remove2();


	return RCBotCommandReturn::Ok;
}
