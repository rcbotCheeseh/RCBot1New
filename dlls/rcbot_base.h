#ifndef __RCBOT_BASE_H__
#define __RCBOT_BASE_H__

#include "rcbot_profile.h"
#include "extdll.h"
#include "rcbot_visibles.h"

#define RCBOT_DEFAULT_FOV 100

template <class T>
class RCBotDemand
{
public:
	RCBotDemand()
	{
		m_iPriority = 0;
	}

	void setValue(T& value, uint8_t iPriority )
	{
		if (iPriority > m_iPriority)
		{
			m_Value = value;
			m_iPriority = iPriority;
		}
	}

	T getValue(void) const
	{
		return m_Value;
	}

	void reset()
	{
		m_Value = 0;
	}
private:
	T m_Value;
	uint8_t m_iPriority;
};

class RCBotBase
{
public:

	RCBotBase();

	~RCBotBase();

	void Think();

	void setProfile(RCBotProfile *profile);
	void setEdict(edict_t *pEdict);
	
	void setUpClientInfo();

	bool inViewCone(Vector &vOrigin);

	void setFOV(float fFov);
	Vector getViewOrigin();

	void newVisible(edict_t* pEntity);

	void lostVisible(edict_t* pEntity);

	edict_t* getEdict()
	{
		return m_pEdict;
	}

	void setMoveTo(Vector vMoveTo, uint8_t iPriority = 1 )
	{
		m_vMoveTo.setValue(vMoveTo, iPriority);
	}

	void setLookAt(Vector vMoveTo, uint8_t iPriority = 1)
	{
		m_vLookAt.setValue(vMoveTo, iPriority);
	}

	void jump()
	{
		// buttons
	}


	void duck()
	{
		// buttons
	}

	float distanceFrom(const edict_t* pEntity);
	float distanceFrom(const Vector &vOrigin);

	void RunPlayerMove()
	{

	}

private:
	RCBotProfile* m_pProfile;
	RCBotVisibles* m_pVisibles;
	edict_t* m_pEdict;
	float m_fFovCos;

	RCBotDemand<Vector> m_vMoveTo;
	RCBotDemand<Vector> m_vLookAt;
};

#endif 