#include "rcbot_navigator.h"
#include "extdll.h"
#include "meta_api.h"
#include "rcbot_strings.h"
#include "rcbot_utils.h"
#include "rcbot_file.h"

const Colour RCBotNavigatorNode::defaultNodeColour = Colour(0, 0, 255);
const Colour RCBotNavigatorNode::defaultPathColour = Colour(255, 255, 255);

RCBotNavigatorNodes* gRCBotNavigatorNodes = new RCBotNavigatorNodes();
RCBotNodeTypes* gRCBotNavigatorNodeTypes; 

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

void RCBotNavigatorNodes::RemovePathsTo(RCBotNavigatorNode* pNode)
{
	for (auto* pUsedNode : m_UsedNodes)
	{
		pUsedNode->RemovePathTo(pNode);
	}
}

void RCBotNavigatorNodes::mapInit()
{
	Load(STRING(gpGlobals->mapname));
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
		for (auto* pNode : m_Paths)
		{
			RCBotUtils::drawBeam(pClient, m_vOrigin, pNode->getOrigin(), defaultPathColour);
		}
	
	}
}

bool RCBotNavigatorNode::Load( RCBotFile* file, uint8_t iVersion, uint16_t iIndex, RCBotNavigatorNode *nodes)
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
		m_Paths.push_back(&nodes[file->readUInt16()]);
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
		file->writeUInt16(m_Paths[i]->getIndex());
	}

	return true;
}


RCBotNavigatorNode * RCBotNavigatorNodes::Add(const Vector& vOrigin)
{
	RCBotNavigatorNode *node = nextFree();

	if (node != nullptr)
	{
		node->Add(vOrigin);

		m_UsedNodes.push_back(node);
	}

	return node;
}

RCBotNavigatorNode* RCBotNavigatorNodes::Nearest(const Vector& vOrigin,float fDistance)
{
	RCBotNavigatorNode* pRet = nullptr;

	for (auto *node : m_UsedNodes)
	{
		float fNodeDistance = node->distanceFrom(vOrigin);

		if (fNodeDistance < fDistance)
		{
			fDistance = fNodeDistance;
			pRet = node;
		}
	}

	return pRet;
}


bool RCBotNavigatorNodes::Remove(const Vector& vOrigin, float fDistance)
{
	RCBotNavigatorNode* pNode = Nearest(vOrigin, fDistance);

	if (pNode != nullptr)
	{
		uint32_t i;

		for (i = 0; i < m_UsedNodes.size(); i++)
		{
			if (m_UsedNodes.at(i) == pNode)
			{
				m_UsedNodes.erase(m_UsedNodes.begin() + i);
				break;
			}
		}

		pNode->Delete();

		return true;
	}

	return false;
}

void RCBotNavigatorNodes::draw(edict_t* pClient, bool bDrawPaths, RCBotNodeTypes *pNodeTypes )
{
	Vector vOrigin = RCBotUtils::entityOrigin(pClient);

	for (auto* pNode : m_UsedNodes)
	{
		pNode->draw(pClient, bDrawPaths, pNodeTypes);
	}
}
/// <summary>
/// Initialise nodes
/// </summary>
RCBotNavigatorNodes::RCBotNavigatorNodes()
{
	for (uint16_t i = 0; i < RCBOT_MAX_NAVIGATOR_NODES; i++)
	{
		m_Nodes[i].init(i);
	}

	m_bDrawingOn = false;
	m_iVersion = 0;
}

#define RCBOT_NAVIGATOR_FILE_HEADER "RCBOT1NODES"

/// <summary>
/// Clear all nodes
/// </summary>
void RCBotNavigatorNodes::Clear()
{
	for (uint16_t i = 0; i < RCBOT_MAX_NAVIGATOR_NODES; i++)
	{
		m_Nodes[i].Delete();
		m_Nodes[i].init(i);
	}

	m_UsedNodes.clear();
}
/// <summary>
/// RCBotNavigatorNodes::Load
/// Load Nodes
/// </summary>
/// <param name="szFilename">absolute path name</param>
/// <returns>true: success, false: failure</returns>
bool RCBotNavigatorNodes::Load(const char* szFilename)
{
	RCBotFile* file = RCBotFile::Open(RCBOT_NAVIGATOR_NODES_FOLDER, szFilename, RCBOT_NAVIGATOR_NODES_EXTENSION, "rb");

	if (file != nullptr)
	{
		const char* szHeader = file->readString();

		// read the file header
		if (gRCBotStrings.stringMatch(szHeader, RCBOT_NAVIGATOR_FILE_HEADER))
		{
			// read the file version
			m_iVersion = file->readByte();

			// read the number of used nodes
			uint16_t iNumNodes = file->readUInt16();

			Clear();
			// read all used nodes
			for (uint16_t i = 0; i < iNumNodes; i++)
			{
				// read the index
				uint16_t iIndex = file->readUInt16();
				RCBotNavigatorNode* pNode = nullptr;
				// invalid index
				if (iIndex >= RCBOT_MAX_NAVIGATOR_NODES)
				{
					// error
					return false;
				}
				// get the node at the index
				pNode = &m_Nodes[iIndex];
				// load the node details
				pNode->Load(file, m_iVersion, i, m_Nodes);
				// add the node to the used list
				m_UsedNodes.push_back(pNode);

			}

			return m_UsedNodes.size() > 0;
		}
	}

	return false;
}
/// <summary>
/// RCBotNavigatorNodes::Save
/// Save nodes
/// </summary>
/// <param name="szFilename">absolute path name</param>
/// <returns>true: success, false : failure</returns>
bool RCBotNavigatorNodes::Save(const char* szFilename)
{
	RCBotFile* file = RCBotFile::Open(RCBOT_NAVIGATOR_NODES_FOLDER,szFilename, RCBOT_NAVIGATOR_NODES_EXTENSION, "wb");

	if (file != nullptr)
	{
		// write the file header
		file->writeString(RCBOT_NAVIGATOR_FILE_HEADER);
		// write the file version
		file->writeByte(RCBOT_NAVIGATOR_VERSION);

		// Write the number of nodes
		file->writeUInt16((uint16_t)m_UsedNodes.size());

		for (auto* pNode : m_UsedNodes)
		{
			// Write the node's index
			file->writeUInt16(pNode->getIndex());
			// Write the node's details
			pNode->Save(file);
		}

		return true;
	}

	return false;
}


RCBotNodePickup::RCBotNodePickup(RCBotNodeTypeBitMasks bitMask, const char* szName, const char* szDescription, Colour colour, const char* szPickupEntityName) : RCBotNodeType(bitMask, szName, szDescription, colour)
{
	m_szPickupEntityName = gRCBotStrings.add(szPickupEntityName);
}

void RCBotNodePickup::Touched(RCBotBase* pBot)
{
	// TO DO - search for entity classname m_szPickupEntityName
	// add pickup task to bot
}

