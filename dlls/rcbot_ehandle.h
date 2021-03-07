#ifndef __RCBOT_EHANDLE_H__
#define __RCBOT_EHANDLE_H__

#include "extdll.h"
#include "meta_api.h"

// use EHandle for storing edicts in tasks
// incase edict changes when the task eventually gets to work.
class EHandle
{
private:
	int m_iSerialNumber;
	edict_t* m_pEdict;
public:
	EHandle()
	{
		m_iSerialNumber = 0;
		m_pEdict = nullptr;
	}

	edict_t* Get()
	{
		try
		{
			if (m_pEdict)
			{
				if (m_iSerialNumber == m_pEdict->serialnumber)
					return m_pEdict;
			}
		}

		catch (...)
		{
		}

		m_pEdict = nullptr;

		return NULL;
	}

	void Set(edict_t* pEdict)
	{
		try
		{
			m_pEdict = pEdict;

			if (pEdict != nullptr)
			{
				m_iSerialNumber = m_pEdict->serialnumber;
			}
			else
				m_iSerialNumber = 0;
		}

		catch (...)
		{
			m_pEdict = nullptr;
			m_iSerialNumber = 0;
		}
	}
};

#endif 
