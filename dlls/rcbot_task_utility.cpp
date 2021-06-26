#include "rcbot_task_utility.h"
#include "rcbot_strings.h"
#include "rcbot_navigator.h"

RCBotTask::RCBotTask(const char* szDebugName, RCBotTaskInterrupt* interrupt )
{
	m_szDebugName = gRCBotStrings.add(szDebugName);

	m_pInterrupt = interrupt;
}


RCBotFindPathTask::RCBotFindPathTask(RCBotNavigator* pNav, RCBotTaskInterrupt *interrupt ) : RCBotTask ("Find Path",interrupt)
{
	m_pNav = pNav;
}

RCBotTaskState RCBotFindPathTask::Execute(RCBotBase* pBot)
{
	switch (m_pNav->findPath())
	{
	case RCBotNavigatorTaskState::PathNotFound:
		return RCBotTaskState::RCBotTaskState_Fail;
	case RCBotNavigatorTaskState::Complete:
		return RCBotTaskState::RCBotTaskState_Complete;
	default:
		break;
	}
	return RCBotTaskState::RCBotTaskState_InProgress;
}

RCBotUtility::RCBotUtility(const char* szDebugName)
{
	m_szDebugName = gRCBotStrings.add(szDebugName);
	m_fNextDo = 0.0f;
	m_fUtility = 0.0f;
}

RCBotSchedule* RCBotUtility_Roam::execute(RCBotBase* pBot)
{
	
	// find random place to go to
	RCBotNavigatorNode *pNode = gRCBotNavigatorNodes->randomFlagged(0);

	if (pNode != nullptr)
	{
		Vector vOrigin = pNode->getOrigin();
		RCBotNavigator *pNav = RCBotNavigator::createPath(pBot, pNode, vOrigin);

		if (pNav != nullptr)
		{
			RCBotSchedule* sched = new RCBotSchedule();

			sched->AppendTask(new RCBotFindPathTask(pNav));

			return sched;
		}
	}

	return nullptr;
}
