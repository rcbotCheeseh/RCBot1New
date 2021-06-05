#include <stdlib.h>
#include "extdll.h"
#include "h_export_meta.h"
#include "dllapi.h"
#include "meta_api.h"
#include "rcbot_message.h"
#include "rcbot_strings.h"
#include "rcbot_manager.h"
#include "rcbot_weapons.h"

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
	add(new RCBotMessage_CurrentWeapon());
	add(new RCBotMessage_AmmoX());
	add(new RCBotMessage_AmmoPickup());
}

void RCBotMessage :: messageEnd()
{
	m_pBot = gRCBotManager.getBotByEdict(this->ed);

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

void RCBotMessage_WeaponInfo::Execute()
{

}

void RCBotMessage_WeaponPickup::Execute()
{
	if (m_pBot != nullptr)
	{
		int iID = m_Bytes[0];

		// m_pBot->WeaponPickup(iId);
	}
}

void RCBotMessage_AmmoPickup::Execute()
{
	if (m_pBot != nullptr)
	{
		int index = m_Bytes[0];
		int amount = m_Bytes[1];

		//m_pBot->setAmmo(index, amount);
	}
}

void RCBotMessage_AmmoX::Execute()
{
	if (m_pBot != nullptr)
	{
		int index = m_Bytes[0];
		int amount = m_Bytes[1];

		//m_pBot->setAmmo(index, amount);
	}
}

void RCBotMessage_CurrentWeapon::Execute()
{
	if (m_pBot != nullptr)
	{
		int iState = m_Bytes[0];
		int iId = m_Bytes[1];
		int iClip = m_Bytes[2];

		// do stuff
		// m_pBot->setCurrentWeapon(iState,iId,iClip);
	}
}

void RCBotMessage_WeaponList_Generic::Execute()
{
	if (m_pBot != nullptr)
	{
		char* szClassname = (char*)(m_Strings[0]);
		int iAmmo1 = m_Bytes[0];
		int iAmmo2 = m_Bytes[1];
		int iAmmo1Max = m_Bytes[2];
		int iAmmo2Max = m_Bytes[3];
		int iSlot = m_Bytes[4];
		int iPosition = m_Bytes[5];
		int iWeaponID = m_Bytes[6];
		int iFlags = m_Bytes[7];

		
		RCBotWeaponInfo* pWeaponInfo = gWeaponList->getByClassname(szClassname);

		if (pWeaponInfo != nullptr)
		{
			//pWeaponInfo->setNonCustomisableData(iID, iAmmo1, iAmmo2, iAmmo1Max, iAmmo2Max, iSlot, iPosition, iFlags);
		}

		// gWeapons->addWeapon(iWeaponID,szClassname,iAmmo1Max,iAmmo2Max,iSlot,iPosition,iAmmo1,iAmmo2);
	}
	

}