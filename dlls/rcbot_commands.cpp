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
#include <stdint.h>


#define RCBOT_ACCESSORS_FILE "rcbot_accesslevels.ini"

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
	RCBotFile* file = RCBotFile::Open(RCBOT_ACCESSORS_FILE, "r");
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
		RCBotUtils::Message(pClient, MessageErrorLevel::MessageErrorLevel_Information, "\"%s\" usage: [%s]\n", m_szCommand, m_szUsage);
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
		RCBotUtils::Message(pClient, MessageErrorLevel::MessageErrorLevel_Information, "%s : [%s]",command->getCommand(),command->getHelp());
	}

	return RCBotCommandReturn::RCBotCommandReturn_Ok;
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
			gRCBotProfiles.addProfile(profile);

			RCBotUtils::Message(pClient,MessageErrorLevel::MessageErrorLevel_Information,"Bot Profile Added");
		}
		else
		{
			showUsage(pClient);
		}
	}
	else
		showUsage(pClient);

	return RCBotCommandReturn::RCBotCommandReturn_Ok;
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
	return RCBotCommandReturn::RCBotCommandReturn_Ok;
};


RCBotCommandReturn RCBotCommand_ListProfilesCommand::execute(edict_t* pClient, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	RCBotUtils::Message(pClient, MessageErrorLevel::MessageErrorLevel_Information, "Listing profiles...\n");
	gRCBotProfiles.List(pClient);
	return RCBotCommandReturn::RCBotCommandReturn_Ok;
};


RCBotCommands_MainCommand::RCBotCommands_MainCommand() : RCBotCommands("rcbot")
{
	
	addCommand(new RCBotCommands_BotCommand());
	addCommand(new RCBotCommands_ProfileCommand());
}

RCBotCommands_BotCommand::RCBotCommands_BotCommand() : RCBotCommands("bot")
{
	addCommand(new RCBotCommand_AddBotCommand());
}

RCBotCommandReturn RCBotCommand_AddBotCommand::execute(edict_t* pClient, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
{
	gRCBotManager.IncreaseQuota();

	return RCBotCommandReturn::RCBotCommandReturn_Ok;
}

RCBotCommandReturn RCBotCommands_MainCommand::ClientCommand()
{
	RCBotCommandReturn ret = RCBotCommandReturn::RCBotCommandReturn_Continue;

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
