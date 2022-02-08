#ifndef __RCBOT_TASK_UTILITY_H__
#define __RCBOT_TASK_UTILITY_H__

#include "rcbot_base.h"
#include "rcbot_navigator.h"

class RCBotSchedule;

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

	RCBotUtility(const char* szDebugName);

	virtual float getUtility(RCBotBase* pBot) = 0;

	virtual bool canDo(RCBotBase* pBot) = 0;

	virtual RCBotSchedule *execute(RCBotBase* pBot) = 0;

private:
	float m_fUtility;
	float m_fNextDo;
	const char* m_szDebugName;
};

class RCBotUtility_Roam : public RCBotUtility
{
public:
	RCBotUtility_Roam() : RCBotUtility("Roam")
	{

	}

	float getUtility(RCBotBase* pBot)
	{
		return 0.001;
	}

	bool canDo(RCBotBase* pBot)
	{
		return true;
	}

	RCBotSchedule* execute(RCBotBase* pBot);
};

class RCBotUtilities
{
public:
	RCBotUtilities()
	{
		addUtility(new RCBotUtility_Roam());
	}

	void addUtility(RCBotUtility* pUtil)
	{
		m_Utilities.push_back(pUtil);
	}

	bool isCurrentUtility(RCBotUtility* pUtil)
	{
		return m_pCurrentUtility == pUtil;
	}

	RCBotUtility* getBestUtility(RCBotBase* pBot)
	{
		RCBotUtility* pBest = nullptr;
		float fBestUtil = 0.0f;

		for (RCBotUtility* pUtil : m_Utilities)
		{
			if (pUtil->canDo(pBot))
			{
				float fUtil = pUtil->getUtility(pBot);

				if (pBest == nullptr || (fUtil > fBestUtil))
				{
					pBest = pUtil;
					fBestUtil = fUtil;
				}
			}
		}

		return pBest;
	}

	void setUtility(RCBotUtility* pUtil)
	{
		m_pCurrentUtility = pUtil;
	}

	void clearUtility()
	{
		m_pCurrentUtility = nullptr;
	}
private:
	std::vector<RCBotUtility*> m_Utilities;
	RCBotUtility *m_pCurrentUtility;
};

class RCBotTask
{
public:

	RCBotTask(const char* m_szDebugName, RCBotTaskInterrupt* interrupt = nullptr);

	~RCBotTask()
	{
		//if (m_pInterrupt != nullptr)
		//	delete m_pInterrupt;
	}

	virtual RCBotTaskState Execute(RCBotBase* pBot) = 0;

	RCBotTaskState ExecuteTask(RCBotBase* pBot)
	{
		if (m_pInterrupt != nullptr)
		{
			RCBotTaskState interruptState = m_pInterrupt->Execute(pBot);

			if (interruptState != RCBotTaskState::RCBotTaskState_InProgress)
				return interruptState;
		}

		return Execute(pBot);
	}

private:

	RCBotTaskInterrupt* m_pInterrupt;
	const char *m_szDebugName;
};

class RCBotFindPathTask : public RCBotTask
{
public:
	~RCBotFindPathTask()
	{
		delete m_pNav;
	}

	RCBotFindPathTask(RCBotNavigator* pNav, RCBotTaskInterrupt *interrupt = nullptr);

	RCBotTaskState Execute(RCBotBase* pBot);
private:
	RCBotNavigator* m_pNav;
};

class RCBotSchedule
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