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
		m_iValue = 0;
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


	void newVisible(edict_t* pEntity)
	{

	}

	void lostVisible(edict_t* pEntity)
	{

	}

	edict_t* getEdict()
	{
		return m_pEdict;
	}
private:
	RCBotProfile* m_pProfile;
	RCBotVisibles* m_pVisibiles;
	edict_t* m_pEdict;
	float m_fFovCos;

	RCBotDemand<Vector> m_vMoveTo;
	RCBotDemand<Vector> m_vLookAt;
};

#endif 