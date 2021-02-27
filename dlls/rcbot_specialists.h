#include "rcbot_base.h"

enum class RCBotSpecialists_RespawnState
{
	Initialise,
	BuyGuns,
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

private:
	RCBotSpecialists_RespawnState m_RespawnState;
};
