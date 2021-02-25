#ifndef __RCBOT_TASK_UTILITY_H__
#define __RCBOT_TASK_UTILITY_H__

#include "rcbot_base.h"

enum class RCBotTaskState
{
	RCBotTaskState_Fail,
	RCBotTaskState_InProgress,
	RCBotTaskState_Complete

};

class RCBotTaskInterrupt
{
public:
	virtual RCBotTaskState Execute(RCBotBase* pBot) = 0;
};

class RCBotUtility
{
public:
	virtual float getUtility(RCBotBase* pBot) = 0;

	virtual bool canDo(RCBotBase* pBot) = 0;

	virtual void execute(RCBotBase* pBot) = 0;
private:

};

class RCBotUtilities
{
public:
	void addUtility(RCBotUtility* pUtil)
	{
		m_Utilities.push_back(pUtil);
	}

	RCBotUtility* getBestUtility(RCBotBase* pBot)
	{
		RCBotUtility* pBest = NULL;
		float fBestUtil = 0.0f;

		for (RCBotUtility* pUtil : m_Utilities)
		{
			if (pUtil->canDo(pBot))
			{
				float fUtil = pUtil->getUtility(pBot);

				if (pBest == NULL || (fUtil > fBestUtil))
				{
					pBest = pUtil;
					fBestUtil = fUtil;
				}
			}
		}

		return pBest;
	}
private:
	std::vector<RCBotUtility*> m_Utilities;
};

class RCBotTask
{
public:

	RCBotTask(RCBotTaskInterrupt* interrupt = NULL)
	{
		m_pInterrupt = interrupt;
	}

	~RCBotTask()
	{
		if (m_pInterrupt != NULL)
			delete m_pInterrupt;
	}

	virtual RCBotTaskState Execute(RCBotBase* pBot) = 0;

public:

	RCBotTaskState ExecuteTask(RCBotBase* pBot)
	{
		if (m_pInterrupt != NULL)
		{
			RCBotTaskState interruptState = m_pInterrupt->Execute(pBot);

			if (interruptState != RCBotTaskState::RCBotTaskState_InProgress)
				return interruptState;
		}

		return Execute(pBot);
	}

private:

	RCBotTaskInterrupt* m_pInterrupt;
};

class RCBotSchedule : RCBotTask
{
public:

	RCBotSchedule()
	{
		m_iTaskIndex = 0;
	}

	~RCBotSchedule()
	{
		for (RCBotTask* t : m_Tasks)
			delete t;
	}

	virtual RCBotTaskState Execute(RCBotBase* pBot)
	{
		if (m_iTaskIndex < m_Tasks.size())
		{
			RCBotTaskState state = RCBotTaskState::RCBotTaskState_InProgress;

			RCBotTask* m_pCurrentTask = m_Tasks[m_iTaskIndex];

			switch (m_pCurrentTask->ExecuteTask(pBot))
			{
			case RCBotTaskState::RCBotTaskState_Complete:
				m_iTaskIndex++;
				break;
			case RCBotTaskState::RCBotTaskState_Fail:
				return RCBotTaskState::RCBotTaskState_Fail;
			}

			return state;
		}

		return RCBotTaskState::RCBotTaskState_Complete;
	}

	void AppendTask(RCBotTask* task)
	{
		m_Tasks.push_back(task);
	}
private:
	unsigned int m_iTaskIndex;
	std::vector<RCBotTask*> m_Tasks;
};




#endif 