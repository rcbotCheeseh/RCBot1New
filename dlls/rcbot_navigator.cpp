#include "rcbot_navigator.h"
#include "extdll.h"
#include "meta_api.h"
#include "rcbot_strings.h"
#include "rcbot_utils.h"
#include "rcbot_file.h"

const Colour RCBotNavigatorNode::defaultNodeColour = Colour(0, 0, 255);
const Colour RCBotNavigatorNode::defaultPathColour = Colour(255, 255, 255);

RCBotNodeType :: RCBotNodeType (RCBotNodeTypeBitMasks iBitMask, const char* szName, const char* szDescription, Colour vColour)
{
	m_szName = gRCBotStrings.add(szName);
	m_szDescription = gRCBotStrings.add(szDescription);
	m_vColour = vColour;
	m_iBitMask = iBitMask;
}

float RCBotNavigatorNode::distanceFrom(const Vector& vOrigin)
{
	return (m_vOrigin - vOrigin).Length();
}

void RCBotNavigatorNode::draw(edict_t* pClient, bool bDrawPaths, RCBotNodeTypes *nodeTypes )
{
	Vector vFrom;
	Vector vTo;
	Colour vColour;

	vFrom = m_vOrigin - Vector(0, 0, RCBOT_NAVIGATOR_NODE_HEIGHT);
	vTo = m_vOrigin + Vector(0, 0, RCBOT_NAVIGATOR_NODE_HEIGHT);
	
	if (m_iFlags)
		vColour = defaultNodeColour;
	else
	{
		vColour = nodeTypes->getColour(m_iFlags);
	}

	RCBotUtils::drawBeam(pClient, vFrom, vTo, vColour );

	if (bDrawPaths)
	{

	}
}

bool RCBotNavigatorNode::Load( RCBotFile* file, uint8_t iVersion, uint16_t iIndex )
{
	uint16_t iNumPaths = 0;

	this->m_iFlags = file->readUInt32();
	this->m_bIsUsed = file->readBool();
	this->m_fRadius = file->readFloat();

	this->m_vOrigin.x = file->readFloat();
	this->m_vOrigin.y = file->readFloat();
	this->m_vOrigin.z = file->readFloat();

	this->m_iIndex = iIndex;
	
	iNumPaths = file->readUInt16();
	m_Paths.clear();

	for (uint16_t i = 0; i < iNumPaths; i++)
	{
		m_Paths.push_back(file->readUInt16());
	}

	return true;
}

bool RCBotNavigatorNode::Save(RCBotFile* file)
{
	file->writeUInt32(this->m_iFlags);
	file->writeBool(this->m_bIsUsed);
	file->writeFloat(this->m_fRadius);
	file->writeFloat(this->m_vOrigin.x);
	file->writeFloat(this->m_vOrigin.y);
	file->writeFloat(this->m_vOrigin.z);
	file->writeUInt16((uint16_t)m_Paths.size());

	for (uint16_t i = 0; i < m_Paths.size(); i++)
	{
		file->writeUInt16(m_Paths[i]);
	}

	return true;
}


RCBotNavigatorNode * RCBotNavigatorNodes::Add(const Vector& vOrigin)
{
	RCBotNavigatorNode *node = nextFree();

	if ( node != nullptr )
		node->Add(vOrigin);

	return node;
}

RCBotNavigatorNode* RCBotNavigatorNodes::Nearest(const Vector& vOrigin,float fDistance)
{
	RCBotNavigatorNode* pRet = nullptr;

	for (auto node : m_Nodes)
	{
		float fNodeDistance = node.distanceFrom(vOrigin);

		if (fNodeDistance < fDistance)
		{
			fDistance = fNodeDistance;
			pRet = &node;
		}
	}

	return pRet;
}


bool RCBotNavigatorNodes::Remove(const Vector& vOrigin, float fDistance)
{
	RCBotNavigatorNode* pNode = Nearest(vOrigin, fDistance);

	if (pNode != nullptr)
	{
		pNode->Delete();
		return true;
	}

	return false;
}

RCBotNavigatorNodes::RCBotNavigatorNodes()
{
	for (uint32_t i = 0; i < RCBOT_MAX_NAVIGATOR_NODES; i++)
	{

	}
}

bool RCBotNavigatorNodes::Load(const char* szFilename)
{
	RCBotFile* file = RCBotFile::Open(szFilename, "r");

	const char* szHeader = file->readString();

	m_iVersion = file->readByte();

	if (gRCBotStrings.stringMatch(szHeader, "RCBOT1N"))
	{
		// Header OK
		uint16_t iNumWaypoints = file->readUInt16();

		for (uint16_t i = 0; i < iNumWaypoints; i++)
		{
			m_Nodes[i].Load(file, m_iVersion,i);
		}

		return iNumWaypoints > 0;
	}

	return false;
}

bool RCBotNavigatorNodes::Save(const char* szFilename)
{
	RCBotFile* file = RCBotFile::Open(szFilename, "w");

	if (file != nullptr)
	{
		file->writeString("RCBOT1N");

		file->writeByte(RCBOT_NAVIGATOR_VERSION);

		// Header OK
		file->writeUInt16(RCBOT_MAX_NAVIGATOR_NODES);

		for (uint16_t i = 0; i < RCBOT_MAX_NAVIGATOR_NODES; i++)
		{
			m_Nodes[i].Save(file);
		}

		return true;
	}

	return false;
}