#include "rcbot_navigator.h"
#include "extdll.h"
#include "meta_api.h"
#include "rcbot_strings.h"

RCBotNodeType :: RCBotNodeType (uint8_t iIndex, const char* szName, const char* szDescription, Vector vColour)
{
	m_szName = gRCBotStrings.add(szName);
	m_szDescription = gRCBotStrings.add(szDescription);
	m_vColour = vColour;
	m_iIndex = iIndex;
}