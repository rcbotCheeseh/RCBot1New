#ifndef __RCBOT_COMMANDS_H__
#define __RCBOT_COMMANDS_H__

#include <vector>
#include <stdint.h>
#include "meta_api.h"
#include "extdll.h"


#define RCBOT_COMMAND_ACCESS_ROOT 1

class RCBotCommandAccessor
{
public:
	RCBotCommandAccessor(const char* szSteamID, uint32_t iLevel);
private:
	const char* m_szSteamID;
	uint32_t m_iLevel;
};

class RCBotCommandAccessors
{
public:
	RCBotCommandAccessors();
private:
	std::vector<RCBotCommandAccessor*> m_Accessors;
};

enum class RCBotCommandReturn
{
	Ok,
	Continue
};

class RCBotCommand
{
public:
	RCBotCommand()
	{
		m_szCommand = nullptr;
		m_szHelp = nullptr;
		m_szUsage = nullptr;
	}

	~RCBotCommand()
	{

	}

	RCBotCommand(const char* szCommand, const char* szHelp, const char* szUsage);

	virtual RCBotCommandReturn execute(edict_t* pClient, const char* arg1, const char* arg2,const char *arg3, const char *arg4, const char *arg5) = 0;

	bool isCommand(const char* szCommand);

	const char* getCommand() const
	{
		return m_szCommand;
	}

	const char* getHelp() const
	{
		return m_szHelp;
	}

	void showUsage(edict_t* pClient);

private:
	const char* m_szCommand;
	const char* m_szHelp;
	const char *m_szUsage;
};

class RCBotCommands : public RCBotCommand
{
public:

	RCBotCommands(const char* m_szCommand);

	RCBotCommand* findCommand(const char* m_szCommand);

	RCBotCommandReturn execute(edict_t* pClient, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);

protected:
	void addCommand(RCBotCommand* pCommand)
	{
		m_Commands.push_back(pCommand);
	}

private:

	std::vector<RCBotCommand*> m_Commands;

};

class RCBotCommands_MainCommand : public RCBotCommands
{
public:
	RCBotCommands_MainCommand();

	static void setClient(edict_t* pClient)
	{
		m_pClient = pClient;
	}
	
	static RCBotCommandReturn ClientCommand();
	static bool IsFakeClientCommand;
	static int NumArgs;
	static edict_t* m_pClient;
};


class RCBotCommand_AddProfileCommand : public RCBotCommand
{
public:
	RCBotCommand_AddProfileCommand() : RCBotCommand("add", "adds a profile", "<name>,<skill>,<model>,<visrevs>")
	{

	}

	RCBotCommandReturn execute(edict_t* pClient, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};


class RCBotCommand_RemoveProfileCommand : public RCBotCommand
{
public:
	RCBotCommand_RemoveProfileCommand() : RCBotCommand("remove", "removes a profile", "<id>")
	{

	}

	RCBotCommandReturn execute(edict_t* pClient, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};

class RCBotCommand_ListProfilesCommand : public RCBotCommand
{
public:
	RCBotCommand_ListProfilesCommand() : RCBotCommand("list", "lists profile","")
	{

	}

	RCBotCommandReturn execute(edict_t* pClient, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};


class RCBotCommand_TeleportToPlayerCommand : public RCBotCommand
{
public:
	RCBotCommand_TeleportToPlayerCommand() : RCBotCommand("teleport_player", "teleport to a player", "<playername>")
	{

	}

	RCBotCommandReturn execute(edict_t* pClient, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};


class RCBotCommand_NoClipCommand : public RCBotCommand
{
public:
	RCBotCommand_NoClipCommand() : RCBotCommand("noclip", "clipping enable/disable", "toggle")
	{

	}

	RCBotCommandReturn execute(edict_t* pClient, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};

class RCBotCommand_GodModeCommand : public RCBotCommand
{
public:
	RCBotCommand_GodModeCommand() : RCBotCommand("god", "god mode enable/disable", "toggle")
	{

	}

	RCBotCommandReturn execute(edict_t* pClient, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};

class RCBotCommands_UtilCommand : public RCBotCommands
{
public:
	RCBotCommands_UtilCommand() : RCBotCommands("util")
	{
		addCommand(new RCBotCommand_TeleportToPlayerCommand());
		addCommand(new RCBotCommand_NoClipCommand());
		addCommand(new RCBotCommand_GodModeCommand());
	}
};


class RCBotCommands_ProfileCommand : public RCBotCommands
{
public:
	RCBotCommands_ProfileCommand() : RCBotCommands("profile")
	{
		addCommand(new RCBotCommand_AddProfileCommand());
		//addCommand(new RCBotCommand_RemoveProfileCommand());
		addCommand(new RCBotCommand_ListProfilesCommand());
	}
};

class RCBotCommands_BotCommand : public RCBotCommands
{
public:
	RCBotCommands_BotCommand();
};

class RCBotCommand_AddBotCommand : public RCBotCommand
{
public:
	RCBotCommand_AddBotCommand() : RCBotCommand("add", "adds a bot", nullptr)
	{

	}

	RCBotCommandReturn execute(edict_t* pClient, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};


class RCBotCommands_WaypointCommand : public RCBotCommands
{
public:
	RCBotCommands_WaypointCommand();
};

class RCBotCommand_WaypointOnCommand : public RCBotCommand
{
public:
	RCBotCommand_WaypointOnCommand() : RCBotCommand("on", "show waypoints", nullptr)
	{

	}

	RCBotCommandReturn execute(edict_t* pClient, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};

class RCBotCommand_WaypointOffCommand : public RCBotCommand
{
public:
	RCBotCommand_WaypointOffCommand() : RCBotCommand("off", "stop showing waypoints", nullptr)
	{

	}

	RCBotCommandReturn execute(edict_t* pClient, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};


class RCBotCommand_WaypointLoadCommand : public RCBotCommand
{
public:
	RCBotCommand_WaypointLoadCommand() : RCBotCommand("load", "load waypoints", nullptr)
	{

	}

	RCBotCommandReturn execute(edict_t* pClient, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};


class RCBotCommand_WaypointInfoCommand : public RCBotCommand
{
public:
	RCBotCommand_WaypointInfoCommand() : RCBotCommand("info", "show waypoint info", nullptr)
	{

	}

	RCBotCommandReturn execute(edict_t* pClient, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};


class RCBotCommand_WaypointLoadOldCommand : public RCBotCommand
{
public:
	RCBotCommand_WaypointLoadOldCommand() : RCBotCommand("load_old", "load old waypoints", nullptr)
	{

	}

	RCBotCommandReturn execute(edict_t* pClient, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};

class RCBotCommand_WaypointSaveCommand : public RCBotCommand
{
public:
	RCBotCommand_WaypointSaveCommand() : RCBotCommand("save", "save waypoints", nullptr)
	{

	}

	RCBotCommandReturn execute(edict_t* pClient, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};


class RCBotCommand_WaypointAddCommand : public RCBotCommand
{
public:
	RCBotCommand_WaypointAddCommand() : RCBotCommand("add", "add a waypoint", nullptr)
	{

	}

	RCBotCommandReturn execute(edict_t* pClient, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};

class RCBotCommand_WaypointDeleteCommand : public RCBotCommand
{
public:
	RCBotCommand_WaypointDeleteCommand() : RCBotCommand("delete", "delete nearest waypoint", nullptr)
	{

	}

	RCBotCommandReturn execute(edict_t* pClient, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};



class RCBotCommands_PathWaypointCommand : public RCBotCommands
{
public:
	RCBotCommands_PathWaypointCommand();
};

class RCBotCommand_PathWaypoint_Create1_Command : public RCBotCommand
{
public:
	RCBotCommand_PathWaypoint_Create1_Command() : RCBotCommand("create1", "create a path from current waypoint", nullptr)
	{

	}

	RCBotCommandReturn execute(edict_t* pClient, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};

class RCBotCommand_PathWaypoint_Create2_Command : public RCBotCommand
{
public:
	RCBotCommand_PathWaypoint_Create2_Command() : RCBotCommand("create2", "create a path to current waypoint", nullptr)
	{

	}

	RCBotCommandReturn execute(edict_t* pClient, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};



class RCBotCommand_PathWaypoint_Remove1_Command : public RCBotCommand
{
public:
	RCBotCommand_PathWaypoint_Remove1_Command() : RCBotCommand("remove1", "delete a path from current waypoint", nullptr)
	{

	}

	RCBotCommandReturn execute(edict_t* pClient, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};

class RCBotCommand_PathWaypoint_Remove2_Command : public RCBotCommand
{
public:
	RCBotCommand_PathWaypoint_Remove2_Command() : RCBotCommand("remove2", "delete a path to current waypoint", nullptr)
	{

	}

	RCBotCommandReturn execute(edict_t* pClient, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};


extern RCBotCommands_MainCommand *gRCBotCommands;


#endif 
