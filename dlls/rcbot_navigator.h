#ifndef __RCBOT_NAVIGATOR_H__
#define __RCBOT_NAVIGATOR_H__


#include "rcbot_base.h"

class RCBotNavigator
{
public:
private:
};

class RCBotNavigatorNode
{
public:
	virtual bool Execute(RCBotBase* pBot) = 0;

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
private:
	std::vector<RCBotNavigatorNode*> m_PathsTo;
	std::vector<RCBotNavigatorNode*> m_PathsFrom;
};

class RCBotWaypoint : RCBotNavigatorNode
{
public:
	RCBotWaypoint()
	{
		m_bIsUsed = false;
	}

	bool Execute(RCBotBase* pBot)
	{
		pBot->setMoveTo(m_vOrigin);

		if (pBot->distanceFrom(m_vOrigin) < pBot->touchDistance())
		{
			return true;
		}
	}


private:
	Vector m_vOrigin;
	bool m_bIsUsed;
};


class RCBotWaypointNavigator : RCBotNavigator
{
public:
private:
	static const unsigned int MAX_WAYPOINTS = 1024;
	RCBotWaypoint Waypoints[MAX_WAYPOINTS];


	RCBotWaypointNavigator()
	{

	}
};

#endif 
