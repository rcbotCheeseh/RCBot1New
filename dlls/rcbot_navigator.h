#ifndef __RCBOT_NAVIGATOR_H__
#define __RCBOT_NAVIGATOR_H__

#include <stdint.h>

#include "rcbot_base.h"

#define RCBOT_MAX_NAVIGATOR_NODES 1024

class RCBotNavigatorNode
{
public:
	RCBotNavigatorNode()
	{
		m_vOrigin = Vector(0,0,0);
		m_fRadius = 0;
		m_bIsUsed = false;
		iFlags = 0;
		iIndex = 0;
	}

	void AddPathTo(RCBotNavigatorNode* pNode)
	{
		m_PathsTo.push_back(pNode);
	}

	void AddPathFrom(RCBotNavigatorNode* pNode)
	{
		m_PathsFrom.push_back(pNode);
	}

	void RemovePathFrom(RCBotNavigatorNode* pNode)
	{
		for (unsigned int i = 0; i < m_PathsFrom.size(); i++)
		{
			if (m_PathsFrom[i] == pNode)
			{
				m_PathsFrom.erase(m_PathsFrom.begin() + i);
				return;
			}
		}
	}

	void RemovePathTo(RCBotNavigatorNode* pNode)
	{
		for (unsigned int i = 0; i < m_PathsTo.size(); i++)
		{
			if (m_PathsTo[i] == pNode)
			{
				m_PathsTo.erase(m_PathsTo.begin() + i);
				return;
			}
		}
	}

	bool Execute(RCBotBase* pBot)
	{
		pBot->setMoveTo(m_vOrigin);

		if (pBot->distanceFrom(m_vOrigin) < m_fRadius)
		{
			return true;
		}

		return false;
	}

private:
	std::vector<RCBotNavigatorNode*> m_PathsTo;
	std::vector<RCBotNavigatorNode*> m_PathsFrom;
	Vector m_vOrigin;
	float m_fRadius;
	bool m_bIsUsed;
	uint32_t iFlags;
	uint16_t iIndex;
};

class RCBotNavigator
{
public:

private:
	RCBotNavigatorNode Nodes[RCBOT_MAX_NAVIGATOR_NODES];
};

class RCBotNodeType
{
public:
	enum class RCBotNodeMovement { Touch, Walk };

	RCBotNodeType(uint8_t iIndex, const char* szName, const char* szDescription, Vector vColour);

	virtual void Touched(RCBotBase* pBot)
	{
		// do nothing
	}

	virtual void MovingTowards(RCBotBase* pBot)
	{
		// do nothing
	}

	uint32_t getBitMask()
	{
		return 1 << m_iIndex;
	}
private:
	const char * m_szName;
	const char* m_szDescription;
	Vector m_vColour;
	uint8_t m_iIndex;
};

class RCBotNodeJump : public RCBotNodeType
{
public:

	RCBotNodeJump(uint8_t iIndex) : RCBotNodeType(iIndex,"jump", "bot will jump here", Vector(170, 180, 200))
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

	RCBotNodeCrouch(uint8_t iIndex) : RCBotNodeType(iIndex, "crouch", "bot will crouch here", Vector(200, 160, 100))
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
		m_NodeTypes.push_back(new RCBotNodeJump(0));
		m_NodeTypes.push_back(new RCBotNodeCrouch(1));
	}

	void Touched( RCBotBase *pBot, int iFlags )
	{
		if (iFlags)
		{
			for (auto* node : m_NodeTypes)
			{
				if (iFlags&node->getBitMask() )
					node->Touched(pBot);
			}
		}
	}

	void MovingTowards(RCBotBase* pBot, int iFlags)
	{
		if (iFlags)
		{
			for (auto* node : m_NodeTypes)
			{
				if (iFlags & node->getBitMask())
					node->MovingTowards(pBot);
			}
		}
	}
private:
	std::vector<RCBotNodeType*> m_NodeTypes;
};



class RCBotWaypoint : RCBotNavigatorNode
{
public:
	RCBotWaypoint()
	{
		
	}

private:

	
};

#endif 
