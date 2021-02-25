#ifndef __RCBOT_COMMANDS_H__
#define __RCBOT_COMMANDS_H__

#include <vector>
#include "meta_api.h"
#include "extdll.h"

#define RCBOT_COMMAND_ACCESS_ROOT 1

class RCBotCommandAccessor
{
public:
	RCBotCommandAccessor(const char* szSteamID, uint32_t iLevel);
private:
	const char* m_szSteamID;
	uint32_t iLevel;
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
	RCBotCommandReturn_Ok,
	RCBotCommandReturn_Continue
};

class RCBotCommand
{
public:
	RCBotCommand()
	{
		m_szCommand = NULL;
		m_szHelp = NULL;
		m_szUsage = NULL;
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

class RCBotCommands_BotCommand : public RCBotCommands
{
public:
	RCBotCommands_BotCommand();
};

class RCBotCommand_AddBotCommand : public RCBotCommand
{
public:
	RCBotCommand_AddBotCommand() : RCBotCommand("add", "adds a bot", NULL)
	{

	}

	RCBotCommandReturn execute(edict_t* pClient, const char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5);
};

extern RCBotCommands_MainCommand gRCBotCommands;


#endif 
