#ifndef __RCBOT_NAVIGATOR_H__
#define __RCBOT_NAVIGATOR_H__

#include <stdint.h>

#include "rcbot_base.h"
#include "rcbot_file.h"
#include "rcbot_colour.h"
#include "rcbot_ehandle.h"

#define RCBOT_NAVIGATOR_REACHABLE_RANGE 300.0f

#define RCBOT_NAVIGATOR_DRAW_DISTANCE 512.0f
#define RCBOT_MAX_NAVIGATOR_NODES 1024
#define RCBOT_NAVIGATOR_DEFAULT_RADIUS 48
#define RCBOT_NAVIGATOR_VERSION 1
#define RCBOT_NAVIGATOR_NODE_HEIGHT 32

#define RCBOT_WAYPOINT_NEAR_DISTANCE 200.0f

#define RCBOT_NAVIGATOR_NODES_RCBOT1_FOLDER "waypoints"
#define RCBOT_NAVIGATOR_NODES_RCBOT1_EXTENSION "rcw"

#define RCBOT_NAVIGATOR_NODES_FOLDER "nodes"
#define RCBOT_NAVIGATOR_NODES_EXTENSION "rcn"
#define RCBOT_NODE_DRAW_PERIOD 1.0f

#define RCBOT_NAVIGATOR_NODE_REACH_TIMEOUT 30.0f
#define RCBOT_NAVIGATOR_NODE_INVISIBLE_TIMEOUT 5.0f
#define RCBOT_NAVIGATOR_NODE_VISIBLE_CHECK_TIME 1.0f

#define RCBOT_NODETYPE_EXTRA_COST_LADDER 100
#define RCBOT_NODETYPE_EXTRA_COST_CROUCH 100
#define RCBOT_NODETYPE_EXTRA_COST_HEALTH -200
#define RCBOT_NODETYPE_EXTRA_COST_ARMOUR -100
#define RCBOT_NODETYPE_EXTRA_COST_WEAPON -100
#define RCBOT_NODETYPE_EXTRA_COST_AMMO -100
#define RCBOT_NODETYPE_EXTRA_COST_JUMP 50

enum class RCBotNodeTypeBitMasks
{
	// defines for waypoint flags field (32 bits are available)
	W_FL_TEAM = ((1 << 0) + (1 << 1)), /* allow for 4 teams (0-3) */
	W_FL_TEAM_SPECIFIC = (1 << 2),  /* waypoint only for specified team */
	W_FL_CROUCH = (1 << 3),  /* must crouch to reach this waypoint */
	W_FL_LADDER = (1 << 4),  /* waypoint on a ladder */
	W_FL_LIFT = (1 << 5),  // lift button
	W_FL_DOOR = (1 << 6),  /* wait for door to open */
	W_FL_HEALTH = (1 << 7),  /* health kit (or wall mounted) location */
	W_FL_ARMOR = (1 << 8),  /* armor (or HEV) location */
	W_FL_AMMO = (1 << 9),  /* ammo location */
	W_FL_CHECK_LIFT = (1 << 10), /* checks for lift at this point */
	W_FL_IMPORTANT = (1 << 11), /* flag position (or hostage or president) */
	W_FL_RESCUE = (1 << 12), /* flag return position (or rescue zone) */
	W_FL_MARINE_BUILDING = (1 << 12), // Ns: marine building put here
	W_FL_SCIENTIST_POINT = (1 << 11),
	W_FL_BARNEY_POINT = (1 << 12),
	W_FL_DEFEND_ZONE = (1 << 13),
	W_FL_AIMING = (1 << 14), /* aiming waypoint */
	W_FL_CROUCHJUMP = (1 << 16), // }
	W_FL_WAIT_FOR_LIFT = (1 << 17),/* wait for lift to be down before approaching this waypoint */
	W_FL_WALL_STICK = (1 << 18),
	W_FL_STUNT = (1 << 18),
	W_FL_PAIN = (1 << 18),
	W_FL_JUMP = (1 << 19),
	W_FL_WEAPON = (1 << 20), // Crouch and jump
	W_FL_TELEPORT = (1 << 21),
	W_FL_TANK = (1 << 22), // func_tank near waypoint
	W_FL_FLY = (1 << 23),
	W_FL_STAY_NEAR = (1 << 24),
	W_FL_ENDLEVEL = (1 << 25), // end of level, in svencoop etc
	W_FL_OPENS_LATER = (1 << 26),
	W_FL_HUMAN_TOWER = (1 << 27), // bot will crouch & wait for a player to jump on them
	W_FL_UNREACHABLE = (1 << 28), // not reachable by bot, used as a reference point for visibility only
	W_FL_PUSHABLE = (1 << 29),
	W_FL_GREN_THROW = (1 << 30),
	W_FL_DELETED = (1 << 31) /* used by waypoint allocation code */
};

#define RCBOT_WAYPOINT_OK_SOUND ""
#define RCBOT_WAYPOINT_BAD_SOUND ""

class RCBotNodeType
{
public:
	RCBotNodeType(RCBotNodeTypeBitMasks iBitMask, const char* szName, const char* szDescription, Colour vColour, float fExtraCost);

	virtual void Touched(RCBotBase* pBot)
	{
		// do nothing
	}

	virtual void MovingTowards(RCBotBase* pBot)
	{
		// do nothing
	}

	bool hasType(uint32_t iFlags)
	{
		return (iFlags & ((uint32_t)m_iBitMask)) == ((uint32_t)m_iBitMask);
	}

	uint32_t setType(uint32_t iFlags)
	{
		return iFlags | ((uint32_t)m_iBitMask);
	}

	uint32_t removeType(uint32_t iFlags)
	{
		return iFlags & ~((uint32_t)m_iBitMask);
	}

	Colour getColour() const
	{
		return m_vColour;
	}

	const char* getDescription() const 
	{
		return m_szDescription;
	}

	float extraCost() const
	{
		return m_fExtraCost;
	}

	const char* getName() const
	{
		return m_szName;
	}

	bool isName(const char* pszName);
private:
	const char* m_szName;
	const char* m_szDescription;
	Colour m_vColour;
	RCBotNodeTypeBitMasks m_iBitMask;
	float m_fExtraCost;
};


class RCBotNodePickup : public RCBotNodeType
{
public:
	RCBotNodePickup(RCBotNodeTypeBitMasks bitMask, const char* szName, const char* szDescription, Colour colour, const char* szPickupEntityName, float fExtraCost);

	void Touched(RCBotBase* pBot);

private:
	const char* m_szPickupEntityName;
};

class RCBotNodeLadder : public RCBotNodeType
{
public:

	RCBotNodeLadder(RCBotNodeTypeBitMasks bitMask) : RCBotNodeType(bitMask, "ladder", "bot will climb ladder here", Colour(255, 255, 0),RCBOT_NODETYPE_EXTRA_COST_LADDER)
	{

	}

};

class RCBotNodeJump : public RCBotNodeType
{
public:

	RCBotNodeJump(RCBotNodeTypeBitMasks bitMask) : RCBotNodeType(bitMask, "jump", "bot will jump here", Colour(170, 180, 200), RCBOT_NODETYPE_EXTRA_COST_JUMP)
	{

	}

	void Touched(RCBotBase* pBot)
	{
		pBot->jump();
	}

};


class RCBotNodeCrouch : public RCBotNodeType
{
public:

	RCBotNodeCrouch(RCBotNodeTypeBitMasks bitMask) : RCBotNodeType(bitMask, "crouch", "bot will crouch here", Colour(200, 160, 100), RCBOT_NODETYPE_EXTRA_COST_CROUCH)
	{

	}

	void MovingTowards(RCBotBase* pBot)
	{
		pBot->duck();
	}

	float extraCost() const
	{
		return 100;
	}
};


class RCBotNodeTypes
{
public:
	RCBotNodeTypes()
	{
		// Basic Types
		m_NodeTypes.push_back(new RCBotNodeLadder(RCBotNodeTypeBitMasks::W_FL_LADDER));
		m_NodeTypes.push_back(new RCBotNodeJump(RCBotNodeTypeBitMasks::W_FL_JUMP));
		m_NodeTypes.push_back(new RCBotNodeCrouch(RCBotNodeTypeBitMasks::W_FL_CROUCH));
		m_NodeTypes.push_back(new RCBotNodePickup(RCBotNodeTypeBitMasks::W_FL_AMMO, "ammo", "bot will pick up ammo here", Colour(100, 200, 100), "item_ammo", RCBOT_NODETYPE_EXTRA_COST_AMMO));
		m_NodeTypes.push_back(new RCBotNodePickup(RCBotNodeTypeBitMasks::W_FL_HEALTH, "health", "bot will pick up health here", Colour(200, 200, 200), "item_health", RCBOT_NODETYPE_EXTRA_COST_HEALTH));
		m_NodeTypes.push_back(new RCBotNodePickup(RCBotNodeTypeBitMasks::W_FL_ARMOR, "armour", "bot will pick up armour here", Colour(255, 100, 0), "item_battery", RCBOT_NODETYPE_EXTRA_COST_ARMOUR));
		m_NodeTypes.push_back(new RCBotNodePickup(RCBotNodeTypeBitMasks::W_FL_WEAPON, "weapon", "bot will pick up a weapon here", Colour(255, 0, 0), "item_weapon", RCBOT_NODETYPE_EXTRA_COST_WEAPON));
	}

	void showNodeTypes(edict_t* pClient);

	RCBotNodeType* findByName( const char *pszName )
	{
		for (auto* pNodeType : m_NodeTypes)
		{
			if (pNodeType->isName(pszName) )
			{
				return pNodeType;
			}
		}

		return nullptr;
	}

	float extraCost(RCBotBase* pBot, uint32_t iFlags)
	{
		float fExtraCost = 0;

		if (iFlags)
		{
			for (auto* node : m_NodeTypes)
			{
				if (node->hasType(iFlags))
					fExtraCost += node->extraCost();
			}
		}

		return fExtraCost;
	}

	void Touched(RCBotBase* pBot, uint32_t iFlags)
	{
		if (iFlags)
		{
			for (auto* node : m_NodeTypes)
			{
				if (node->hasType(iFlags))
					node->Touched(pBot);
			}
		}
	}

	void MovingTowards(RCBotBase* pBot, uint32_t iFlags)
	{
		if (iFlags)
		{
			for (auto* node : m_NodeTypes)
			{
				if (node->hasType(iFlags))
					node->MovingTowards(pBot);
			}
		}
	}


	Colour getColour(uint32_t iFlags)
	{
		Colour ret;
		bool bValid = false;

		for (auto* pNode : m_NodeTypes)
		{
			if (pNode->hasType(iFlags))
			{
				if (bValid == false)
				{
					ret = pNode->getColour();
					bValid = true;
				}
				else
					ret.mix(pNode->getColour());
			}
		}

		return ret;
	}
	/// <summary>
	/// addType
	/// </summary>
	/// <param name="pType"></param>
	void addType(RCBotNodeType* pType)
	{
		m_NodeTypes.push_back(pType);
	}
	/// <summary>
	/// getNodeTypes
	/// </summary>
	/// <param name="pNodeTypes"></param>
	/// <param name="iFlags"></param>
	void getNodeTypes(std::vector< RCBotNodeType*> *pNodeTypes, uint32_t iFlags)
	{
		for (auto* pNodeType : m_NodeTypes)
		{
			if (pNodeType->hasType(iFlags))
			{
				pNodeTypes->push_back(pNodeType);
			}
		}
	}
private:
	std::vector<RCBotNodeType*> m_NodeTypes;
};

class RCBotNavigatorNode
{
public:
	RCBotNavigatorNode()
	{
		init(0);
	}

	void draw(edict_t* pClient, bool bDrawPaths, RCBotNodeTypes *types, int iWaypointTexture);

	float distanceFrom(const Vector& vOrigin);

	bool Load(RCBotFile* file, uint8_t iVersion, uint16_t iIndex, RCBotNavigatorNode *nodes);


	bool Save(RCBotFile* file);

	void Add( const Vector &vOrigin )
	{
		m_bIsUsed = true;
		m_vOrigin = vOrigin;
		m_iFlags = 0;
		m_fRadius = RCBOT_NAVIGATOR_DEFAULT_RADIUS;
		m_Paths.clear();
		//m_PathsFrom.clear();
	}

	void setFlags(uint32_t flags)
	{
		m_iFlags = flags;
	}

	uint32_t getFlags() const
	{
		return m_iFlags;
	}

	float getRadius() const
	{
		return m_fRadius;
	}

	void Delete()
	{
		m_bIsUsed = false;
		m_Paths.clear();
		//m_PathsFrom.clear();
	}

	/*void AddPathTo(RCBotNavigatorNode* pNode)
	{
		m_Paths.push_back(pNode);
	}*/

	bool AddPathTo(RCBotNavigatorNode* pNode)
	{
		// can't add path to self
		if (pNode == this)
			return false;

		for (unsigned int i = 0; i < m_Paths.size(); i++)
		{
			if (m_Paths[i] == pNode)
			{
				// path already exists
				return false;
			}
		}

		m_Paths.push_back(pNode);

		return true;
	}

	bool RemovePathTo(RCBotNavigatorNode* pNode)
	{
		for (unsigned int i = 0; i < m_Paths.size(); i++)
		{
			if (m_Paths[i] == pNode)
			{
				// path found
				m_Paths.erase(m_Paths.begin() + i);
				return true;
			}
		}

		return false; // path not found
	}

	uint16_t getNumPaths()
	{
		return (uint16_t)m_Paths.size();
	}

	RCBotNavigatorNode* getPath(uint16_t index)
	{
		if ( index < m_Paths.size() )
			return m_Paths[index];

		return nullptr;
	}

	/*void RemovePathTo(RCBotNavigatorNode* pNode)
	{
		for (unsigned int i = 0; i < m_PathsTo.size(); i++)
		{
			if (m_PathsTo[i] == pNode)
			{
				m_PathsTo.erase(m_PathsTo.begin() + i);
				return;
			}
		}
	}*/

	Vector getOrigin() const 
	{
		return m_vOrigin;
	}

	bool isUsed()
	{
		return m_bIsUsed;
	}

	uint16_t getIndex()
	{
		return m_iIndex;
	}

	void init(uint16_t iIndex)
	{
		m_iIndex = iIndex;
		m_vOrigin = Vector(0, 0, 0);
		m_fRadius = 0;
		m_bIsUsed = false;
		m_iFlags = 0;
	}

	bool addNodeType(RCBotNodeType *pType)
	{
		if (!pType->hasType(m_iFlags))
		{
			m_iFlags = pType->setType(m_iFlags);
			return true;
		}

		return false;
	}

	void removeNodeTypes()
	{
		m_iFlags = 0;
	}

	bool removeNodeType(RCBotNodeType* pType)
	{
		if (pType->hasType(m_iFlags))
		{
			m_iFlags = pType->removeType(m_iFlags);
			return true;
		}

		return false;
	}

private:
	std::vector<RCBotNavigatorNode*> m_Paths;
	Vector m_vOrigin;
	float m_fRadius;
	bool m_bIsUsed;
	uint32_t m_iFlags;
	uint16_t m_iIndex;
	static const Colour defaultNodeColour;
	static const Colour defaultPathColour;
};


#define RCBOT_NODE_EDIT_DRAW_PERIOD 1.0f;

class RCBotNavigatorNodes;

class RCBotNodeEditor
{
public:
	RCBotNodeEditor(edict_t* pPlayer, RCBotNavigatorNodes *pNodes)
	{
		m_pPlayer.Set(pPlayer);
		m_Create1 = nullptr;
		m_Remove1 = nullptr;
		m_Nodes = pNodes;
	}

	edict_t* getPlayer()
	{
		return m_pPlayer.Get();
	}

	bool Create1();
	bool Create2();
	bool Remove1();
	bool Remove2();

	bool AddNode();

	bool RemoveNode();

	void showNearestNodeInfo();

	bool AddNodeType(const char* pType);
	bool RemoveNodeType(const char* pType);
	bool ClearTypes();

private:
	EHandle m_pPlayer;
	RCBotNavigatorNode* m_Create1;
	RCBotNavigatorNode* m_Remove1;
	RCBotNavigatorNodes* m_Nodes;
};

enum class RCBotNodeVisibility
{
	Unchecked,
	Checked_Visible,
	Checked_Invisible
};

class RCBotNavigatorNodes
{
public:

	RCBotNavigatorNodes();

	virtual bool Load(const char* szFilename);
	virtual bool Save(const char* szFilename);

	bool LoadRCBot1Waypoints(const char* szFilename);

	void Clear();
	void clearEditors()
	{
		m_Editors.clear();
	}

	RCBotNavigatorNode* randomFlagged(uint32_t iFlags);

	uint16_t getNumWaypoints()
	{
		return (uint16_t)m_UsedNodes.size();
	}

	void playSound(edict_t *pClient,bool bGoodSound);

	virtual RCBotNavigatorNode* Add(const Vector& vOrigin);
	virtual bool Remove(const Vector& vOrigin, float fDistance);


	void autoPath(RCBotNavigatorNode* pNode);

	void RemovePathsTo(RCBotNavigatorNode* pNode);

	void mapInit();

	void draw(edict_t* pClient, bool drawPaths, RCBotNodeTypes* pNodeTypes);

	RCBotNavigatorNode* nextFree()
	{
		for (uint16_t i = 0; i < RCBOT_MAX_NAVIGATOR_NODES; i++)
		{
			if (!m_Nodes[i].isUsed())
				return &m_Nodes[i];
		}

		return nullptr;
	}

	RCBotNavigatorNode* Nearest(const Vector& vOrigin, float fDistance, bool bMustBeVisible);

	void addEditor(edict_t* pClient)
	{
		m_Editors.push_back(new RCBotNodeEditor(pClient,this));
	}

	void removeEditor(edict_t* pClient)
	{
		for (uint16_t i = 0; i < m_Editors.size(); i++)
		{
			if ( m_Editors[i]->getPlayer() == pClient )
			{
				m_Editors.erase(m_Editors.begin() + i);
				return;

			}

		}
	}

	RCBotNodeEditor* getEditor(edict_t* pClient)
	{
		for (auto* pEditor : m_Editors)
		{
			if (pEditor->getPlayer() == pClient)
				return pEditor;
		}

		return nullptr;
	}

    RCBotNodeTypes* getNodeTypes()
	{
		return m_NodeTypes;
	}

	void gameFrame();

	void generateFlaggedNodesList()
	{
		m_FlaggedNodes.clear();

		// for use with quickly finding flagged nodes
		for (auto* pNode : m_UsedNodes)
		{
			if (pNode->getFlags() > 0)
				m_FlaggedNodes.push_back(pNode);
		}
	}

	// do visibility etc...
	void DoVisibility();

protected:
	RCBotNodeTypes* m_NodeTypes;

	RCBotNavigatorNode m_Nodes[RCBOT_MAX_NAVIGATOR_NODES];
	RCBotNodeVisibility m_Visibility[RCBOT_MAX_NAVIGATOR_NODES][RCBOT_MAX_NAVIGATOR_NODES];
	uint16_t m_VisibilityIndex_X;
	uint16_t m_VisibilityIndex_Y;
	uint8_t m_iVersion;

	std::vector<RCBotNavigatorNode*> m_FlaggedNodes;
	std::vector<RCBotNavigatorNode*> m_UsedNodes;
	std::vector<RCBotNodeEditor*> m_Editors;
	float m_fDrawNodes;
	int m_iWaypointTexture;

	char* m_szGoodSoundCommand;
	char* m_szBadSoundCommand;

};

// define the waypoint file header structure...
typedef struct
{
	char filetype[8];  // should be "RC_bot\0"
	int  waypoint_file_version;
	int  waypoint_file_flags;  // used for visualisation
	int  number_of_waypoints;
	char mapname[32];  // name of map for these waypoints
} RCBot1WaypointHeader;

// nothing special here
class RCBot1Waypoint
{
public:
	int  flags;		// button, lift, flag, health, ammo, etc.
	Vector origin;   // location

	RCBot1Waypoint() { flags = 0; Vector(0, 0, 0); };
};


extern RCBotNavigatorNodes *gRCBotNavigatorNodes;

enum class RCBotNavigatorTaskState
{
	FindingPath,
	PathNotFound,
	PathFound,
	FollowingPath,
	Complete
};

enum class RCBotAStarNodeFlag
{
	Closed = 1,
	Open = 2,
	Heuristic_set = 4,
	Parent = 8
};

class RCBotAStarNode
{
public:
	RCBotAStarNode() { m_fCost = 0; m_fHeuristic = 0; m_iFlags = 0; m_pParent = nullptr;  m_pNode = nullptr; }
	///////////////////////////////////////////////////////
	void close() { setFlag(RCBotAStarNodeFlag::Closed); }
	void unClose() { removeFlag(RCBotAStarNodeFlag::Closed); }
	bool isOpen() { return hasFlag(RCBotAStarNodeFlag::Open); }
	void unOpen() { removeFlag(RCBotAStarNodeFlag::Open); }
	bool isClosed() { return hasFlag(RCBotAStarNodeFlag::Closed); }
	void open() { setFlag(RCBotAStarNodeFlag::Open); }
	//////////////////////////////////////////////////////	
	void setHeuristic(float fHeuristic) { m_fHeuristic = fHeuristic; setFlag(RCBotAStarNodeFlag::Heuristic_set); }
	bool heuristicSet() { return hasFlag(RCBotAStarNodeFlag::Heuristic_set); }
	float getHeuristic() { return m_fHeuristic; }

	////////////////////////////////////////////////////////
	void setFlag(RCBotAStarNodeFlag iFlag) { m_iFlags |= (int)iFlag; }
	bool hasFlag(RCBotAStarNodeFlag iFlag) { return ((m_iFlags & (int)iFlag) == (int)iFlag); }
	void removeFlag(RCBotAStarNodeFlag iFlag) { m_iFlags &= ~(int)iFlag; }
	/////////////////////////////////////////////////////////
	RCBotNavigatorNode* getParent() { if (hasFlag(RCBotAStarNodeFlag::Parent)) return m_pParent; else return nullptr; }
	void setParent(RCBotNavigatorNode *pParent)
	{
		m_pParent = pParent; 

		if (m_pParent == nullptr)
			removeFlag(RCBotAStarNodeFlag::Parent); // no parent
		else
			setFlag(RCBotAStarNodeFlag::Parent);
	}
	////////////////////////////////////////////////////////
	float getCost() { return m_fCost; }
	void setCost(float fCost) { m_fCost = fCost; }
	////////////////////////////////////////////////////////
	// for comparison
	bool precedes(RCBotAStarNode *other) const
	{
		return (m_fCost + m_fHeuristic) < (other->getCost() + other->getHeuristic());
	}
	void setNode(RCBotNavigatorNode *pNode) { m_pNode = pNode; }
	RCBotNavigatorNode *getNode() { return m_pNode; }
private:
	float m_fCost;
	float m_fHeuristic;
	int  m_iFlags;
	RCBotNavigatorNode* m_pParent;
	RCBotNavigatorNode* m_pNode;
};

// Insertion sorted list
class RCBotNavigatorAStarListNode
{
public:
	RCBotNavigatorAStarListNode(RCBotAStarNode *data)
	{
		m_Data = data;
		m_Next = nullptr;
	}
	~RCBotNavigatorAStarListNode()
	{
		// don't need to delete
		//delete m_Data;
	}
	RCBotAStarNode *m_Data;
	RCBotNavigatorAStarListNode *m_Next;
};

class RCBotNavigatorAStarOpenList
{
public:
	RCBotNavigatorAStarOpenList()
	{
		m_Head = nullptr;
	}


	~RCBotNavigatorAStarOpenList()
	{
		RCBotNavigatorAStarListNode *t;

		while (m_Head != nullptr)
		{
			t = m_Head;

			m_Head = m_Head->m_Next;

			delete t;

			t = nullptr;
		}
	}

	bool empty()
	{
		return (m_Head == nullptr);
	}

	RCBotAStarNode *top()
	{
		if (m_Head != nullptr )
			return m_Head->m_Data;

		return nullptr;
	}

	void pop()
	{
		if (m_Head != nullptr)
		{
			RCBotNavigatorAStarListNode *t = m_Head;

			m_Head = m_Head->m_Next;

			delete t;
		}
	}

	void add(RCBotAStarNode *data)
	{
		RCBotNavigatorAStarListNode* newNode = new RCBotNavigatorAStarListNode(data);
		RCBotNavigatorAStarListNode* t;
		RCBotNavigatorAStarListNode* p;

		if (m_Head == nullptr)
			m_Head = newNode;
		else
		{
			if (data->precedes(m_Head->m_Data))
			{
				newNode->m_Next = m_Head;
				m_Head = newNode;
			}
			else
			{
				p = m_Head;
				t = m_Head->m_Next;

				while (t != nullptr)
				{
					if (data->precedes(t->m_Data))
					{
						p->m_Next = newNode;
						newNode->m_Next = t;
						break;
					}

					p = t;
					t = t->m_Next;
				}

				if (t != nullptr)
					p->m_Next = newNode;

			}
		}
	}


private:
	RCBotNavigatorAStarListNode *m_Head;
};

class RCBotNavigator
{
public:

	RCBotNavigator(RCBotNavigatorNode* pFrom, RCBotNavigatorNode* pTo, Vector& vGoal, RCBotBase *pBot);

	RCBotNavigatorTaskState findPath();

	// returns null if no nearby or goal waypoints found
	static RCBotNavigator* createPath(RCBotBase* pBot, Vector& vTo);
	// returns null if no nearby waypoint found
	static RCBotNavigator* createPath(RCBotBase* pBot, RCBotNavigatorNode *pGoal, Vector &vTo);

	void open(RCBotAStarNode* pNode);

	// AStar Algorithm : get the waypoint with lowest cost
	RCBotAStarNode* nextNode();

private:
	RCBotNavigatorTaskState m_State;
	Vector m_vGoal;
	RCBotNavigatorNode* m_pStart;
	RCBotNavigatorNode* m_pGoal;
	RCBotNavigatorAStarOpenList m_OpenList;
	uint16_t m_iNavRevs;
	RCBotAStarNode m_Nodes[RCBOT_MAX_NAVIGATOR_NODES];
	std::vector<RCBotNavigatorNode*> m_Route;
	uint16_t m_CurrentRouteIndex;
	RCBotBase* m_pBot;
	float m_fTimeout;
	float m_fVisibleCheck;
	float m_fVisibleTimeout;
};

#endif 
