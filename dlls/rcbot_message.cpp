#include <stdlib.h>
#include "extdll.h"
#include "h_export_meta.h"
#include "dllapi.h"
#include "meta_api.h"
#include "rcbot_message.h"
#include "rcbot_strings.h"
#include "rcbot_manager.h"

RCBotMessages* g_Messages = nullptr;

bool RCBotMessage :: isID(uint32_t id)
{
	if (msg_type == 0)
	{
		int32_t iSize = 0;

		msg_type = GET_USER_MSG_ID(PLID, m_szMessageName, &iSize);
	}

	return msg_type == id;
}

RCBotMessage ::RCBotMessage (const char* szMessageName)
{
	this->m_szMessageName = gRCBotStrings.add(szMessageName);
	this->msg_type = 0;

	this->msg_dest = 0;
	this->pOrigin = nullptr;
	this->ed = nullptr;
}

bool RCBotMessage::isName(const char* szName)
{
	return gRCBotStrings.stringMatch(szName, m_szMessageName);
}

RCBotMessages::RCBotMessages()
{
	m_CurrentMessage = nullptr;
	// base messages
	add(new RCBotMessage_WeaponList_Generic());
}

void RCBotMessage_WeaponInfo::Execute()
{

}

void RCBotMessage_WeaponList_Generic::Execute()
{
	RCBotBase* pBot;

	pBot = gRCBotManager.getBotByEdict(this->ed);

	if (pBot != nullptr)
	{
		char* szClassname = (char*)(m_Strings[0]);
		int iAmmo1 = m_Bytes[0];
		int iAmmo2 = m_Bytes[1];
		int iAmmoMax = m_Bytes[2];
		int iAmmo2Max = m_Bytes[3];
		int iSlot = m_Bytes[4];
		int iPosition = m_Bytes[5];
		int iWeaponID = m_Bytes[6];
		int iFlags = m_Bytes[7];
	}
	

}