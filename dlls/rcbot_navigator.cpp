#include "rcbot_navigator.h"
#include "extdll.h"
#include "meta_api.h"
#include "rcbot_strings.h"
#include "rcbot_utils.h"
#include "rcbot_file.h"

const Colour RCBotNavigatorNode::defaultNodeColour = Colour(0, 0, 255);
const Colour RCBotNavigatorNode::defaultPathColour = Colour(255, 255, 255);

RCBotNavigatorNodes* gRCBotNavigatorNodes = nullptr;

RCBotNodeType :: RCBotNodeType (RCBotNodeTypeBitMasks iBitMask, const char* szName, const char* szDescription, Colour vColour, float fExtraCost)
{
	m_szName = gRCBotStrings.add(szName);
	m_szDescription = gRCBotStrings.add(szDescription);
	m_vColour = vColour;
	m_iBitMask = iBitMask;
	m_fExtraCost = fExtraCost;
}

bool RCBotNodeType::isName(const char* pszName)
{
	return RCBotStrings::stringMatch(pszName, m_szName);
}

void RCBotNavigatorNodes::gameFrame()
{
	if (m_fDrawNodes < gpGlobals->time)
	{
		for ( auto *pEditor : m_Editors )
		{
			edict_t* pPlayer = pEditor->getPlayer();

			if (pPlayer != nullptr)
			{
				draw(pPlayer, true, m_NodeTypes);
			}
		}

		m_fDrawNodes = gpGlobals->time + RCBOT_NODE_DRAW_PERIOD;
	}

	DoVisibility();
}

float RCBotNavigatorNode::distanceFrom(const Vector& vOrigin)
{
	return (m_vOrigin - vOrigin).Length();
}

RCBotNavigatorNode* RCBotNavigatorNodes::randomFlagged(uint32_t iFlags)
{
	std::vector<RCBotNavigatorNode*> pNodes;

	for (auto* pNode : m_FlaggedNodes)
	{
		if (iFlags == 0 || ((pNode->getFlags() & iFlags) == iFlags) )
			pNodes.push_back(pNode);
	}

	if (pNodes.size() > 0)
	{
		return pNodes[RANDOM_LONG(0, pNodes.size()-1)];
	}

	return nullptr;
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
	m_iWaypointTexture = PRECACHE_MODEL("sprites/lgtning.spr");
	m_Editors.clear();
	m_fDrawNodes = 0.0f;
	Load(STRING(gpGlobals->mapname));

}

void RCBotNavigatorNode::draw(edict_t* pClient, bool bDrawPaths, RCBotNodeTypes *nodeTypes , int iWaypointTexture )
{
	Vector vFrom;
	Vector vTo;
	Colour vColour;

	vFrom = m_vOrigin - Vector(0, 0, RCBOT_NAVIGATOR_NODE_HEIGHT);
	vTo = m_vOrigin + Vector(0, 0, RCBOT_NAVIGATOR_NODE_HEIGHT);
	
	if (m_iFlags==0)
		vColour = defaultNodeColour;
	else
	{
		vColour = nodeTypes->getColour(m_iFlags);
	}

	RCBotUtils::drawBeam(pClient, vFrom, vTo, vColour, iWaypointTexture);

	if (bDrawPaths)
	{
		for (auto* pNode : m_Paths)
		{
			RCBotUtils::drawBeam(pClient, m_vOrigin, pNode->getOrigin(), defaultPathColour, iWaypointTexture);
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

#define NODE_AUTOPATH_DISTANCE 384 // To do make a CVAR
#define NODE_AUTOPATH_STEP_HEIGHT 16 // to do make a CVAR
#define NODE_AUTOPATH_STEP_DEPTH 16 // to do make a CVAR
#define PLAYER_HEIGHT 78
/// <summary>
/// Walkable
/// NODE 1                          NODE 2
///                                   |
///   |              _________________|
///   |_____________/
///
/// Not Walkable
/// NODE 1                          NODE 2
///                                   |
///   |						 ________|
///   |_______               /
///		     \             /
///            `-----------'
/// </summary>
/// <param name="vStart"></param>
/// <param name="vEnd"></param>
/// <returns></returns>
static bool IsWalkable(Vector& vStart, Vector& vEnd)
{
	if (RCBotUtils::isVisible(vStart, vEnd))
	{
		TraceResult th;

		(*g_engfuncs.pfnTraceHull)(vStart, vEnd, ignore_monsters, human_hull, nullptr, &th);

		if (th.flFraction >= 1.0f)
		{
			if (((*g_engfuncs.pfnPointContents)(vStart) == CONTENTS_WATER) &&
				((*g_engfuncs.pfnPointContents)(vEnd) == CONTENTS_WATER))
			{
				// can swim there
				return true;
			}
			else
			{
				// Check for stairs/holes
				Vector vComp = vEnd - vStart;

				vComp = vComp / 16; // Break up in to 16 pieces

				// Find height of waypoint up to PLAYER_HEIGHT
				TraceResult* tr = RCBotUtils::Traceline(vStart, vStart - Vector(0, 0, 72), ignore_monsters, dont_ignore_glass, nullptr);

				float fHeight = (vStart.z - tr->vecEndPos.z) + 8.0f; // add a bit of height

				// Check for ground between 
				for (uint8_t i = 0; i < 16; i++)
				{
					Vector vStep = vStart + vComp;

					tr = RCBotUtils::Traceline(vStart, vStart - Vector(0, 0, fHeight), ignore_monsters, dont_ignore_glass, nullptr);

					vStart = vStep;

					if (tr->flFraction >= 1.0f)
						return false;
					// check for ground

				}

				return true;
			}
		}
	}
	
	return false;
}

void RCBotNavigatorNodes :: autoPath(RCBotNavigatorNode* pNode)
{
	for (auto pOther : m_UsedNodes)
	{
		if (pOther != pNode)
		{
			Vector otherOrigin = pOther->getOrigin();
			Vector nodeOrigin = pNode->getOrigin();

			const float stepHeight = NODE_AUTOPATH_STEP_HEIGHT;
			const float stepDepth = NODE_AUTOPATH_STEP_DEPTH;

			float fDist = pOther->distanceFrom(nodeOrigin);

			if ( fDist < NODE_AUTOPATH_DISTANCE)
			{
				if (IsWalkable(nodeOrigin, otherOrigin))
				{
					pOther->AddPathTo(pNode);
					pNode->AddPathTo(pOther);
				}
			}
		}
	}
}

RCBotNavigatorNode * RCBotNavigatorNodes::Add(const Vector& vOrigin)
{
	RCBotNavigatorNode *node = nextFree();

	if (node != nullptr)
	{
		node->Add(vOrigin);

		m_UsedNodes.push_back(node);
		// TO DO Auto path
		autoPath(node);
	}

	return node;
}

RCBotNavigatorNode* RCBotNavigatorNodes::Nearest(const Vector& vOrigin,float fDistance,bool bMustBeVisible = false)
{
	RCBotNavigatorNode* pRet = nullptr;

	for (auto *node : m_UsedNodes)
	{
		float fNodeDistance = node->distanceFrom(vOrigin);

		if (fNodeDistance < fDistance)
		{
			if (bMustBeVisible)
			{
				if (RCBotUtils::Traceline(vOrigin, node->getOrigin(), ignore_monsters, ignore_glass, nullptr)->flFraction < 1.0f)
					continue; // not visible
			}

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

	RCBotNavigatorNode* pNearest = Nearest(vOrigin, RCBOT_NAVIGATOR_DEFAULT_RADIUS);

	for (auto* pNode : m_UsedNodes)
	{
		if (pNode->distanceFrom(vOrigin) < RCBOT_NAVIGATOR_DRAW_DISTANCE)
		{
			pNode->draw(pClient, pNode== pNearest, pNodeTypes, m_iWaypointTexture);
		}
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

	m_iVersion = 0;
	m_fDrawNodes = 0;

	m_NodeTypes = new RCBotNodeTypes();

	m_iWaypointTexture = 0;

	m_szBadSoundCommand = (char*)gRCBotStrings.add("play common/wpn_denyselect.wav\n");
	m_szGoodSoundCommand = (char*)gRCBotStrings.add("play common/wpn_hudon.wav\n");
	
	m_VisibilityIndex_X = 0;
	m_VisibilityIndex_Y = 0;

	memset(m_Visibility, 0, sizeof(RCBotNodeVisibility) * RCBOT_MAX_NAVIGATOR_NODES * RCBOT_MAX_NAVIGATOR_NODES);

	
}

#define RCBOT_NAVIGATOR_FILE_HEADER "RCBOT1NODES"


void RCBotNavigatorNodes::playSound(edict_t *pClient, bool goodSound)
{
	if (goodSound)
	{
		CLIENT_COMMAND(pClient, m_szGoodSoundCommand);
	}
	else
		CLIENT_COMMAND(pClient, m_szBadSoundCommand);
}

void RCBotNavigatorNodes::DoVisibility()
{
	int NodeCount = m_UsedNodes.size();

	if (m_VisibilityIndex_X < NodeCount)
	{
		TraceResult *tr;

		RCBotNavigatorNode* node1 = m_UsedNodes[m_VisibilityIndex_X];

		if (m_VisibilityIndex_Y < NodeCount)
		{
			RCBotNavigatorNode* node2 = m_UsedNodes[m_VisibilityIndex_Y];

			int index1 = m_VisibilityIndex_X;
			int index2 = m_VisibilityIndex_Y;

			if (index1 == index2)
			{
				m_Visibility[index1][index2] = RCBotNodeVisibility::Checked_Visible;
			}
			else if (m_Visibility[index2][index1] != RCBotNodeVisibility::Unchecked)
			{
				// use the other node's visibility check
				m_Visibility[index1][index2] = m_Visibility[m_VisibilityIndex_Y][m_VisibilityIndex_X];
			}
			else
			{
				// do a traceline
				tr = RCBotUtils::Traceline(node1->getOrigin(), node2->getOrigin(), ignore_monsters, ignore_glass, nullptr);

				m_Visibility[index1][index2] = (tr->flFraction >= 1.0f) ? RCBotNodeVisibility::Checked_Visible : RCBotNodeVisibility::Checked_Invisible;
			}
		}
		else
			m_VisibilityIndex_Y = 0;

		m_VisibilityIndex_X++;
	}
	else
	{
		m_VisibilityIndex_X = 0;

		if ( m_VisibilityIndex_Y < m_UsedNodes.size() )
			m_VisibilityIndex_Y++;
	}

}

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

			generateFlaggedNodesList();

			m_VisibilityIndex_X = 0;
			m_VisibilityIndex_Y = 0;

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

		file->close();

		return true;
	}

	return false;
}


RCBotNodePickup::RCBotNodePickup(RCBotNodeTypeBitMasks bitMask, const char* szName, const char* szDescription, Colour colour, const char* szPickupEntityName, float fExtraCost) : RCBotNodeType(bitMask, szName, szDescription, colour, fExtraCost)
{
	m_szPickupEntityName = gRCBotStrings.add(szPickupEntityName);
}

void RCBotNodePickup::Touched(RCBotBase* pBot)
{
	// TO DO - search for entity classname m_szPickupEntityName
	// add pickup task to bot
}



bool RCBotNodeEditor::AddNode()
{
	edict_t* pPlayer = m_pPlayer.Get();

	if (pPlayer != nullptr)
	{
		Vector vOrigin = RCBotUtils::entityOrigin(pPlayer);

		return m_Nodes->Add(vOrigin);
	}

	return false;
}


bool RCBotNodeEditor::RemoveNode()
{

	Vector vOrigin = RCBotUtils::entityOrigin(m_pPlayer.Get());

	return m_Nodes->Remove(vOrigin, 200.0f);
}

bool RCBotNodeEditor::AddNodeType( const char* pszName)
{
	edict_t* pClient = m_pPlayer.Get();
	RCBotNodeTypes* pTypes = gRCBotNavigatorNodes->getNodeTypes();

	if (pszName && *pszName)
	{
		RCBotNodeEditor* pEditor = gRCBotNavigatorNodes->getEditor(pClient);

		if (pEditor != nullptr)
		{
			RCBotNodeType* pType = pTypes->findByName(pszName);

			if (pType != nullptr)
			{
				RCBotNavigatorNode* pNode = gRCBotNavigatorNodes->Nearest(pClient->v.origin, 100, true);

				if (pNode != nullptr)
				{
					pNode->addNodeType(pType);

					return true;
				}
				else
				{
					RCBotUtils::Message(pClient, MessageErrorLevel::Information, "you are not near a waypoint");
				}

				return false;
			}
			else
			{
				RCBotUtils::Message(pClient, MessageErrorLevel::Information, "node type \"%s\" doesn't exist\n", pszName);
			}
		}
	}
	pTypes->showNodeTypes(pClient);


	return false;
}

bool RCBotNodeEditor::RemoveNodeType(const char* pszName)
{
	edict_t* pClient = m_pPlayer.Get();
	RCBotNodeTypes* pTypes = gRCBotNavigatorNodes->getNodeTypes();

	if (pszName && *pszName)
	{
		RCBotNodeEditor* pEditor = gRCBotNavigatorNodes->getEditor(pClient);

		if (pEditor != nullptr)
		{
			RCBotNodeType* pType = pTypes->findByName(pszName);

			if (pType != nullptr)
			{
				RCBotNavigatorNode* pNode = gRCBotNavigatorNodes->Nearest(pClient->v.origin, 100, true);

				if (pNode != nullptr)
				{
					pNode->removeNodeType(pType);

					return true;
				}
				else
				{
					RCBotUtils::Message(pClient, MessageErrorLevel::Information, "you are not near a waypoint");
				}

				return false;
			}
			else
			{
				RCBotUtils::Message(pClient, MessageErrorLevel::Information, "node type \"%s\" doesn't exist\n", pszName);
			}
		}
	}
	pTypes->showNodeTypes(pClient);


	return false;
}

bool RCBotNodeEditor::ClearTypes()
{
	edict_t* pClient = m_pPlayer.Get();
	RCBotNodeTypes* pTypes = gRCBotNavigatorNodes->getNodeTypes();

	RCBotNodeEditor* pEditor = gRCBotNavigatorNodes->getEditor(m_pPlayer.Get());

	if (pEditor != nullptr)
	{
		RCBotNavigatorNode* pNode = gRCBotNavigatorNodes->Nearest(pClient->v.origin, 100, true);

		if (pNode != nullptr)
		{
			pNode->removeNodeTypes();

			return true;
		}
		else
		{
			RCBotUtils::Message(pClient, MessageErrorLevel::Information, "you are not near a waypoint");
		}

		return false;
	}
	
	pTypes->showNodeTypes(pClient);

	return false;
}

void RCBotNodeEditor::showNearestNodeInfo()
{
	edict_t* pPlayer = m_pPlayer.Get();
	Vector vOrigin = RCBotUtils::entityOrigin(pPlayer);
	RCBotNavigatorNode* pNearest = m_Nodes->Nearest(vOrigin, RCBOT_WAYPOINT_NEAR_DISTANCE);
	
	std::vector< RCBotNodeType*> nodeTypes;

	m_Nodes->getNodeTypes()->getNodeTypes(&nodeTypes, pNearest->getFlags());

	for (auto* pNodeTypes : nodeTypes)
	{
		RCBotUtils::Message(pPlayer, MessageErrorLevel::Information, (char*)pNodeTypes->getDescription());
	}
}

bool RCBotNodeEditor::Create1()
{
	Vector vOrigin = RCBotUtils::entityOrigin(m_pPlayer.Get());

	m_Create1 = m_Nodes->Nearest(vOrigin, RCBOT_WAYPOINT_NEAR_DISTANCE);

	return m_Create1 != nullptr;
}

bool RCBotNodeEditor::Create2()
{
	Vector vOrigin = RCBotUtils::entityOrigin(m_pPlayer.Get());

	RCBotNavigatorNode *node = m_Nodes->Nearest(vOrigin, RCBOT_WAYPOINT_NEAR_DISTANCE);

	if (m_Create1 != nullptr && node != nullptr )
	{
		return m_Create1->AddPathTo(node);
	}

	return false;
}

bool RCBotNodeEditor::Remove1()
{
	Vector vOrigin = RCBotUtils::entityOrigin(m_pPlayer.Get());

	m_Remove1 = m_Nodes->Nearest(vOrigin, RCBOT_WAYPOINT_NEAR_DISTANCE);

	return m_Remove1 != nullptr;
}

bool RCBotNodeEditor::Remove2()
{
	Vector vOrigin = RCBotUtils::entityOrigin(m_pPlayer.Get());

	RCBotNavigatorNode* node = m_Nodes->Nearest(vOrigin, RCBOT_WAYPOINT_NEAR_DISTANCE);

	if (m_Remove1 != nullptr && node != nullptr)
	{
		return m_Remove1->RemovePathTo(node);
	}

	return false;
}

bool RCBotNavigatorNodes::LoadRCBot1Waypoints(const char* szFilename)
{
	RCBotFile* file = RCBotFile::Open(RCBOT_NAVIGATOR_NODES_RCBOT1_FOLDER, szFilename, RCBOT_NAVIGATOR_NODES_RCBOT1_EXTENSION, "rb",true);

	if (file != nullptr)
	{
		RCBot1WaypointHeader header;

		file->readBytes(&header, sizeof(RCBot1WaypointHeader));

		if (RCBotStrings::stringMatch(header.filetype, "RCBot"))
		{
			int i;

			if (header.number_of_waypoints < RCBOT_MAX_NAVIGATOR_NODES)
			{
				Clear();

				for (i = 0; i < header.number_of_waypoints; i++)
				{
					RCBot1Waypoint waypoint;

					file->readBytes(&waypoint, sizeof(RCBot1Waypoint));

					RCBotNavigatorNode* pNode = Add(waypoint.origin);

					pNode->setFlags(waypoint.flags);
				}

				// read and add waypoint paths...
				for (i = 0; i < header.number_of_waypoints; i++)
				{
					uint16_t num = file->readUInt16();

					while ( num -- > 0 )
					{
						uint16_t path_index = file->readUInt16();

						if (path_index < RCBOT_MAX_NAVIGATOR_NODES)
							m_Nodes[i].AddPathTo(&m_Nodes[path_index]);
					}
				}
			}

			generateFlaggedNodesList();

			return m_UsedNodes.size() > 0;
		}
	}

	return false;
}

RCBotNavigator::RCBotNavigator(RCBotNavigatorNode *pFrom, RCBotNavigatorNode *pTo, Vector &vGoal, RCBotBase *pBot)
{
	m_pBot = pBot;
	m_State = RCBotNavigatorTaskState::FindingPath;
	m_vGoal = vGoal;
	m_pStart = pFrom;
	m_pGoal = pTo;
	m_CurrentRouteIndex = 0;
	m_fTimeout = 0;
	m_fVisibleTimeout = 0;
	m_fVisibleCheck = 0;

	//m_iNavRevs = pBot->getNavRevs();
	m_iNavRevs = 100;

	m_Nodes[pFrom->getIndex()].setNode(pFrom);

	m_OpenList.add(&m_Nodes[pFrom->getIndex()]);
}

void RCBotNavigator::open(RCBotAStarNode* pNode)
{
	if (pNode->isOpen() == false)
	{
		pNode->open();
		//m_theOpenList.push_back(pNode);
		m_OpenList.add(pNode);
	}
}

// AStar Algorithm : get the waypoint with lowest cost
RCBotAStarNode * RCBotNavigator::nextNode()
{
	RCBotAStarNode* pNode = nullptr;

	pNode = m_OpenList.top();
	m_OpenList.pop();

	return pNode;
}

RCBotNavigatorTaskState RCBotNavigator::findPath()
{
	uint16_t iNavRevs = m_iNavRevs;
	RCBotAStarNode* pCurr;
	RCBotNavigatorNode* pCurrNode;
	switch (m_State)
	{
	case RCBotNavigatorTaskState::FindingPath:
		while (iNavRevs-- > 0 && m_State == RCBotNavigatorTaskState::FindingPath)
		{
			if (m_OpenList.empty())
			{
				// failed to find goal
				m_State = RCBotNavigatorTaskState::PathNotFound;
				return m_State;
			}

			pCurr = nextNode();

			if (pCurr == nullptr)
			{
				// error
				m_State = RCBotNavigatorTaskState::PathNotFound;
				return m_State;
			}

			pCurrNode = pCurr->getNode();

			if (pCurrNode == m_pGoal)
			{
				m_State = RCBotNavigatorTaskState::PathFound;
				return m_State;
			}

			for (uint16_t i = 0; i < pCurrNode->getNumPaths(); i++)
			{
				float fCost;
				RCBotNavigatorNode* pSuccNode = pCurrNode->getPath(i);
				RCBotAStarNode* pSucc;

				if (pSuccNode == nullptr || pSuccNode == pCurrNode)
				{
					// error
					m_State = RCBotNavigatorTaskState::PathNotFound;
					return m_State;
				}

				pSucc = &m_Nodes[pSuccNode->getIndex()];

				/*if (!m_pBot->canUseNode(pSuccNode))
				{
					continue;
				}*/
				fCost = pCurr->getCost();

				if (pSucc->isOpen() || pSucc->isClosed())
				{
					if (pSucc->getParent() != nullptr)
					{
						if (fCost >= pSucc->getCost())
							continue; // ignore route for now
					}
					else
						continue;
				}

				pSucc->unClose();

				pSucc->setParent(pCurrNode);

				//BotMessage("Succ " + iSucc + " parent = " + iCurrentNode);
				pSucc->setCost(fCost);
				pSucc->setNode(pSuccNode);

				if (pSucc->heuristicSet() == false)
				{
					float h;

					h = pSuccNode->distanceFrom(m_pStart->getOrigin());
					h += pSuccNode->distanceFrom(m_pGoal->getOrigin());
					h += gRCBotNavigatorNodes->getNodeTypes()->extraCost(m_pBot,pSuccNode->getFlags());
					pSucc->setHeuristic(h);
				}

				// Fix: do this AFTER setting heuristic and cost!!!!
				if (pSucc->isOpen() == false)
				{
					open(pSucc);
				}

			}

			pCurr->close();
		}
		break;
	case RCBotNavigatorTaskState::PathFound:
	{
		int iMaxLoops = gRCBotNavigatorNodes->getNumWaypoints();
		RCBotAStarNode* pNode = &m_Nodes[m_pGoal->getIndex()];

		while (iMaxLoops>0 && pNode != nullptr)
		{
			RCBotNavigatorNode* pParent;

			m_Route.push_back(pNode->getNode());

			pParent = pNode->getParent();
			pNode = nullptr;

			if (pParent != nullptr)
			{
				pNode = &m_Nodes[pParent->getIndex()];
			}
			iMaxLoops--;
		}

		if (iMaxLoops == 0) // some error occurred
		{
			m_State = RCBotNavigatorTaskState::PathNotFound;
			return m_State;
		}
		
		std::reverse(m_Route.begin(), m_Route.end());

		m_State = RCBotNavigatorTaskState::FollowingPath;

		m_fTimeout = gpGlobals->time + RCBOT_NAVIGATOR_NODE_REACH_TIMEOUT;
		m_fVisibleCheck = 0;
		m_fVisibleTimeout = gpGlobals->time + RCBOT_NAVIGATOR_NODE_INVISIBLE_TIMEOUT;
	}
	break;
	case RCBotNavigatorTaskState::FollowingPath:
		// Follow the path
	{
		RCBotBase* pBot = m_pBot;

		if (m_fTimeout < gpGlobals->time)
		{
			// error
			m_State = RCBotNavigatorTaskState::PathNotFound;
			return m_State;
		}


		if (m_CurrentRouteIndex < m_Route.size())
		{
			RCBotNavigatorNode* node = m_Route[m_CurrentRouteIndex];

			if (m_fVisibleCheck < gpGlobals->time)
			{
				TraceResult* tr;

				tr = RCBotUtils::Traceline(pBot->getViewOrigin(), node->getOrigin(), ignore_monsters, ignore_glass, pBot->getEdict());

				if (tr->flFraction >= 1.0)
				{
					m_fVisibleTimeout = gpGlobals->time + RCBOT_NAVIGATOR_NODE_INVISIBLE_TIMEOUT;
				}
			}

			if (m_fVisibleTimeout < gpGlobals->time)
			{
				// error
				m_State = RCBotNavigatorTaskState::PathNotFound;
				return m_State;
			}

			gRCBotNavigatorNodes->getNodeTypes()->MovingTowards(pBot, node->getFlags());

			pBot->setMoveTo(node->getOrigin(),2);
			pBot->setLookAt(node->getOrigin(), 2);

			if (pBot->distanceFrom(node->getOrigin()) < node->getRadius())
			{
				// touched
				gRCBotNavigatorNodes->getNodeTypes()->Touched(pBot, node->getFlags());
				m_fTimeout = gpGlobals->time + RCBOT_NAVIGATOR_NODE_REACH_TIMEOUT;
				m_CurrentRouteIndex++;
			}
			else
			{
				TraceResult* tr = RCBotUtils::Traceline(pBot->getViewOrigin(), node->getOrigin(), ignore_monsters, ignore_glass, pBot->getEdict());

				if (tr->flFraction < 1.0f) // waypoint not visible
				{
					m_State = RCBotNavigatorTaskState::PathNotFound;
					return m_State;
				}

			}
		}
		else
		{
			// complete
			m_State = RCBotNavigatorTaskState::Complete;
		}
	}
		break;
	default:
		break;
	}


	return m_State;
}

// returns null if no nearby waypoint found
RCBotNavigator* RCBotNavigator::createPath(RCBotBase* pBot, RCBotNavigatorNode* pGoal, Vector& vTo)
{
	RCBotNavigatorNode* pStart = gRCBotNavigatorNodes->Nearest(pBot->getViewOrigin(), RCBOT_NAVIGATOR_REACHABLE_RANGE, true);

	if (pStart == nullptr)
		return nullptr;

	return new RCBotNavigator(pStart, pGoal, vTo, pBot);
}


// returns null if no nearby or goal waypoints found
RCBotNavigator* RCBotNavigator::createPath(RCBotBase* pBot, Vector& vTo)
{
	RCBotNavigatorNode* pGoal = gRCBotNavigatorNodes->Nearest(vTo, RCBOT_NAVIGATOR_REACHABLE_RANGE, true);

	if (pGoal == nullptr)
		return nullptr;

	return RCBotNavigator::createPath(pBot, pGoal, vTo);

}

void RCBotNodeTypes::showNodeTypes(edict_t* pClient)
{
	for (auto* pNodeType : m_NodeTypes)
	{
		RCBotUtils::Message(pClient, MessageErrorLevel::Information, "\"%s\" : %s\n", pNodeType->getName(), pNodeType->getDescription());
	}
}
