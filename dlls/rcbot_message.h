#ifndef __RCBOT_MESSAGE_H__
#define __RCBOT_MESSAGE_H__


#include "extdll.h"
#include "rcbot_base.h"
#include <vector>
#include <stdint.h>


/// <summary>
/// Base Network Message Class
/// </summary>
class RCBotMessage
{

public:

	RCBotMessage(const char* szMessageName);

	~RCBotMessage()
	{

	}

	void writeByte(uint8_t bByte )
	{
		m_Bytes.push_back(bByte);
	}

	void writeChar(char chChar )
	{
		m_Chars.push_back(chChar);
	}

	void writeShort(uint16_t iShort)
	{
		m_Shorts.push_back(iShort);
	}

	void writeLong(uint32_t iValue)
	{
		m_Longs.push_back(iValue);
	}

	void writeCoord(float fCoord)
	{
		m_Coords.push_back(fCoord);
	}

	void writeAngle(float fAngle)
	{
		m_fAngles.push_back(fAngle);
	}

	void writeString(const char *szValue)
	{
		// dont write strings now
		m_Strings.push_back(szValue);
	}

	void writeEntity( uint32_t iEntity )
	{
		m_iEntities.push_back(iEntity);
	}

	void messageEnd()
	{
		Execute();
		m_fAngles.clear();
		m_fFloats.clear();
		m_vVectors.clear();
		m_Bytes.clear();
		m_Shorts.clear();
		m_Longs.clear();
		m_iEntities.clear();
		m_Coords.clear();
		m_Chars.clear();
		m_Strings.clear();
	}

	bool isName(const char* szName);

	void messageBegin(uint32_t _msg_dest, const float* _pOrigin, edict_t* _ed)
	{
		this->msg_dest = _msg_dest;
		this->pOrigin = _pOrigin;
		this->ed = _ed;
	}

	bool isID(uint32_t id);

	virtual void Execute() = 0;

protected:
	std::vector<float> m_fAngles;
	std::vector<float> m_fFloats;
	std::vector<Vector> m_vVectors;
	std::vector<uint8_t> m_Bytes;
	std::vector<int16_t> m_Shorts;
	std::vector<int32_t> m_Longs;
	std::vector<uint32_t> m_iEntities;
	std::vector<float> m_Coords;
	std::vector<char> m_Chars;
	std::vector<const char*> m_Strings;
	uint32_t msg_dest;
	const float* pOrigin;
	edict_t* ed;
private:
	const char* m_szMessageName;
	uint32_t msg_type;
};

class RCBotMessages
{
public:

	RCBotMessages();

	void add(RCBotMessage* message)
	{
		m_Messages.push_back(message);
	}

	RCBotMessage* findMessageById(uint32_t id)
	{
		for (auto msg : m_Messages)
		{
			if (msg->isID(id))
				return msg;
		}

		return nullptr;
	}

	RCBotMessage* findMessageByName(const char* szName)
	{
		for (auto msg : m_Messages)
		{
			if (msg->isName(szName))
				return msg;
		}

		return nullptr;
	}

	RCBotMessage* getCurrentMessage()
	{
		return m_CurrentMessage;
	}

	void clearCurrentMessage()
	{
		m_CurrentMessage = nullptr;
	}

	void setCurrentMessage( RCBotMessage *message )
	{
		m_CurrentMessage = message;
	}
private:
	std::vector<RCBotMessage*> m_Messages;
	RCBotMessage* m_CurrentMessage;
};

class RCBotMessage_WeaponList_Generic : public RCBotMessage
{
public:
	RCBotMessage_WeaponList_Generic() : RCBotMessage("WeaponList")
	{

	}

	void Execute();
};

class RCBotMessage_WeaponInfo : public RCBotMessage
{
public :
	RCBotMessage_WeaponInfo() : RCBotMessage("WeaponInfo")
	{

	}

	void Execute();
};

class RCBotMessages_TheSpecialists : public RCBotMessages
{
public:

protected:

private:

};

extern RCBotMessages* g_Messages;

#endif 