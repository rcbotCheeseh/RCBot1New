#ifndef __RCBOT_MESSAGE_H__
#define __RCBOT_MESSAGE_H__


#include "extdll.h"

#include <vector>
#include <stdint.h>
/// <summary>
/// Base Network Message Class
/// </summary>
class RCBotMessage
{

public:

	~RCBotMessage()
	{

	}

	RCBotMessage(uint32_t msg_type)
	{
		this->msg_type = msg_type;

		this->msg_dest = 0;
		this->pOrigin = nullptr;
		this->ed= nullptr;
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
	}

	void writeEntity( uint32_t iEntity )
	{
		m_iEntities.push_back(iEntity);
	}

	void messageEnd()
	{
		m_fAngles.clear();
		m_fFloats.clear();
		m_vVectors.clear();
		m_Bytes.clear();
		m_Shorts.clear();
		m_Longs.clear();
		m_iEntities.clear();
		m_Coords.clear();
		m_Chars.clear();
	}

	void messageBegin(uint32_t _msg_dest, const float* _pOrigin, edict_t* _ed)
	{
		this->msg_dest = _msg_dest;
		this->pOrigin = _pOrigin;
		this->ed = _ed;
	}

	static RCBotMessage *FindMessage(uint32_t msg_type)
	{
		return nullptr;
	}

	static RCBotMessage* CurrentMessage;
	static std::vector<RCBotMessage*> Messages;

	std::vector<float> m_fAngles;
	std::vector<float> m_fFloats;
	std::vector<Vector> m_vVectors;
	std::vector<uint8_t> m_Bytes;
	std::vector<int16_t> m_Shorts;
	std::vector<int32_t> m_Longs;
	std::vector<uint32_t> m_iEntities;
	std::vector<float> m_Coords;
	std::vector<char> m_Chars;

private:
	uint32_t msg_dest;
	uint32_t msg_type;
	const float* pOrigin;
	edict_t* ed;
};


#endif 