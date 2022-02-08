#ifndef __RCBOT_BASE_H__
#define __RCBOT_BASE_H__

#include "extdll.h"
#include "rcbot_ehandle.h"
#include <stdint.h>

class RCBotProfile;
class RCBotVisibles;
class RCBotSchedule;
class RCBotUtilities;
class RCBotWeapons;

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
		m_iPriority = 0;
	}

	bool isValid()
	{
		return m_iPriority > 0;
	}
private:
	T m_Value;
	uint8_t m_iPriority;
};

class RCBotWeapon;

class RCBotBase
{
public:

	RCBotBase();

	~RCBotBase();

	void selectWeapon(RCBotWeapon* weapon);
	bool isCurrentWeapon(RCBotWeapon* weapon);

	virtual void Think();

	void Interrupt()
	{
		m_bInterrupted = true;
	}

	void Init();
	virtual void spawnInit();

	void setAmmo(uint8_t index, uint8_t amount);

	void setProfile(RCBotProfile *profile);
	void setEdict(edict_t *pEdict);
	
	virtual void setUpClientInfo();

	bool inViewCone(Vector &vOrigin);

	void setFOV(float fFov);
	Vector getViewOrigin();

	void newVisible(edict_t* pEntity);

	void lostVisible(edict_t* pEntity);

	// do stuff
	virtual void setCurrentWeapon(uint8_t iState, uint8_t iId, uint8_t iClip);
	virtual void weaponPickup(uint8_t iId);

	virtual bool isEnemy(edict_t* pEntity)
	{
		return false;
	}

	virtual float getEnemyFactor(edict_t* pEntity);

	edict_t* getEdict()
	{
		return m_pEdict;
	}

	bool isEdict(const edict_t* pEdict)
	{
		return m_pEdict == pEdict;
	}


	void walk()
	{
		m_fSpeedPercent = 0.5; // half speed
	}

	void setMoveTo(Vector vMoveTo, uint8_t iPriority = 1 )
	{
		m_vMoveTo.setValue(vMoveTo, iPriority);
	}

	void setLookAt(Vector vLookAt, uint8_t iPriority = 1)
	{
		m_vLookAt.setValue(vLookAt, iPriority);
	}

	void jump()
	{
		// buttons
		m_pEdict->v.button |= IN_JUMP;
	}


	void duck()
	{
		// buttons
		m_pEdict->v.button |= IN_DUCK;
	}

	void primaryAttack()
	{
		m_pEdict->v.button |= IN_ATTACK;
	}

	float getActualSpeed()
	{
		return m_pEdict->v.velocity.Length();
	}

	bool isAlive();

	virtual void respawn();

	float distanceFrom(const edict_t* pEntity);
	float distanceFrom(const Vector &vOrigin);
	float distanceFrom2D(const Vector& vOrigin)
	{
		return (vOrigin - getViewOrigin()).Length2D();
	}
	void RunPlayerMove();

	void pressButton(int button);

	inline bool isUnderWater() { return m_pEdict->v.waterlevel > 2; }

	uint32_t getEnemyWeaponFlags( const edict_t *pEnemy ) { return 0; }
protected:
	edict_t* m_pEdict;
	EHandle m_pEnemy;
private:
	RCBotProfile* m_pProfile;
	RCBotVisibles* m_pVisibles;
	RCBotSchedule* m_pSchedule;
	RCBotUtilities *m_Utils;
	RCBotWeapons* m_pWeapons;
	RCBotWeapon* m_pCurrentWeapon;
	bool m_bInterrupted; // bot was interrupted and a new utility may be chosen

	float m_fFovCos;

	RCBotDemand<Vector> m_vMoveTo;
	RCBotDemand<Vector> m_vLookAt;

	float m_fRespawnTime;

	float m_fLastRunPlayerMove;

	bool m_bPreviousAliveState;

	float m_fSpeedPercent;
};

#endif 