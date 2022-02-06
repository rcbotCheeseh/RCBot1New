#include "rcbot_base.h"

enum class RCBotSpecialists_RespawnState
{
	Initialise,
	BuyGuns,
	BuyGuns2,
	Respawn,
	Wait
};

class RCBotSpecialists : public RCBotBase
{
public:
	RCBotSpecialists()
	{
		m_RespawnState = RCBotSpecialists_RespawnState::Initialise;
	}

	void setUpClientInfo();

	void spawnInit();

	void respawn();

	bool isEnemy(edict_t* pEdict);

	void Think();

private:
	RCBotSpecialists_RespawnState m_RespawnState;
};
