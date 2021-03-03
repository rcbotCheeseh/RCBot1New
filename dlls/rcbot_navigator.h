#ifndef __RCBOT_NAVIGATOR_H__
#define __RCBOT_NAVIGATOR_H__

#include <stdint.h>

#include "rcbot_base.h"
#include "rcbot_file.h"
#include "rcbot_colour.h"

#define RCBOT_NAVIGATOR_DRAW_DISTANCE 512.0f
#define RCBOT_MAX_NAVIGATOR_NODES 1024
#define RCBOT_NAVIGATOR_DEFAULT_RADIUS 48
#define RCBOT_NAVIGATOR_VERSION 1
#define RCBOT_NAVIGATOR_NODE_HEIGHT 32

#define RCBOT_WAYPOINT_NEAR_DISTANCE 200.0f

#define RCBOT_NAVIGATOR_NODES_FOLDER "nodes"
#define RCBOT_NAVIGATOR_NODES_EXTENSION "rcn"
#define RCBOT_NODE_DRAW_PERIOD 1.0f

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

class RCBotNodeType
{
public:
	RCBotNodeType(RCBotNodeTypeBitMasks iBitMask, const char* szName, const char* szDescription, Colour vColour);

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
private:
	const char* m_szName;
	const char* m_szDescription;
	Colour m_vColour;
	RCBotNodeTypeBitMasks m_iBitMask;
};

class RCBotNodePickup : public RCBotNodeType
{
public:
	RCBotNodePickup(RCBotNodeTypeBitMasks bitMask, const char* szName, const char* szDescription, Colour colour, const char* szPickupEntityName);

	void Touched(RCBotBase* pBot);

private:
	const char* m_szPickupEntityName;
};

class RCBotNodeJump : public RCBotNodeType
{
public:

	RCBotNodeJump(RCBotNodeTypeBitMasks bitMask) : RCBotNodeType(bitMask, "jump", "bot will jump here", Colour(170, 180, 200))
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

	RCBotNodeCrouch(RCBotNodeTypeBitMasks bitMask) : RCBotNodeType(bitMask, "crouch", "bot will crouch here", Colour(200, 160, 100))
	{

	}

	void MovingTowards(RCBotBase* pBot)
	{
		pBot->duck();
	}

};


class RCBotNodeTypes
{
public:
	RCBotNodeTypes()
	{
		// Basic Types
		m_NodeTypes.push_back(new RCBotNodeJump(RCBotNodeTypeBitMasks::W_FL_JUMP));
		m_NodeTypes.push_back(new RCBotNodeCrouch(RCBotNodeTypeBitMasks::W_FL_CROUCH));
		m_NodeTypes.push_back(new RCBotNodePickup(RCBotNodeTypeBitMasks::W_FL_AMMO, "ammo", "bot will pick up ammo here", Colour(100, 200, 100), "item_ammo"));
		m_NodeTypes.push_back(new RCBotNodePickup(RCBotNodeTypeBitMasks::W_FL_HEALTH, "health", "bot will pick up health here", Colour(200, 200, 200), "item_health"));
		m_NodeTypes.push_back(new RCBotNodePickup(RCBotNodeTypeBitMasks::W_FL_ARMOR, "armour", "bot will pick up armour here", Colour(255, 100, 0), "item_battery"));
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

	void addType(RCBotNodeType* pType)
	{
		m_NodeTypes.push_back(pType);
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

private:
	EHandle m_pPlayer;
	RCBotNavigatorNode* m_Create1;
	RCBotNavigatorNode* m_Remove1;
	RCBotNavigatorNodes* m_Nodes;
};


class RCBotNavigatorNodes
{
public:

	RCBotNavigatorNodes();

	virtual bool Load(const char* szFilename);
	virtual bool Save(const char* szFilename);

	void Clear();

	virtual RCBotNavigatorNode* Add(const Vector& vOrigin);
	virtual bool Remove(const Vector& vOrigin, float fDistance);

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

	RCBotNavigatorNode* Nearest(const Vector& vOrigin, float fDistance);

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



protected:
	RCBotNodeTypes* m_NodeTypes;

	RCBotNavigatorNode m_Nodes[RCBOT_MAX_NAVIGATOR_NODES];
	uint8_t m_iVersion;

	std::vector<RCBotNavigatorNode*> m_UsedNodes;
	std::vector<RCBotNodeEditor*> m_Editors;
	float m_fDrawNodes;
	int m_iWaypointTexture;

};

// For RCBot1 Waypoint compatibility
#define WAYPOINT_VERSION 4
#define W_FILE_FL_READ_VISIBILITY (1<<0) // require to read visibility file

// define the waypoint file header structure...
typedef struct
{
	char filetype[8];  // should be "RC_bot\0"
	int  waypoint_file_version;
	int  waypoint_file_flags;  // used for visualisation
	int  number_of_waypoints;
	char mapname[32];  // name of map for these waypoints
} RCBot1WaypointHeader;

class RCBotNavigatorWaypoints_Old :public  RCBotNavigatorNodes
{
public:
	bool Load(const char* szFilename);

	bool Save(const char* szFilename)
	{
		return RCBotNavigatorNodes::Save(szFilename);
	}

};


extern RCBotNavigatorNodes *gRCBotNavigatorNodes;

#endif 
