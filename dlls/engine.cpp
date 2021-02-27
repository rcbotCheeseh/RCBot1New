/*
 *    This file is part of RCBot.
 *
 *    RCBot by Paul Murphy adapted from botman's template 3.
 *
 *    RCBot is free software; you can redistribute it and/or modify it
 *    under the terms of the GNU General Public License as published by the
 *    Free Software Foundation; either version 2 of the License, or (at
 *    your option) any later version.
 *
 *    RCBot is distributed in the hope that it will be useful, but
 *    WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *    General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with RCBot; if not, write to the Free Software Foundation,
 *    Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *    In addition, as a special exception, the author gives permission to
 *    link the code of this program with the Half-Life Game Engine ("HL
 *    Engine") and Modified Game Libraries ("MODs") developed by Valve,
 *    L.L.C ("Valve").  You must obey the GNU General Public License in all
 *    respects for all of the code used other than the HL Engine and MODs
 *    from Valve.  If you modify this file, you may extend this exception
 *    to your version of the file, but you are not obligated to do so.  If
 *    you do not wish to do so, delete this exception statement from your
 *    version.
 *
 */
 //////////////////////////////////////////////////
 // RCBOT : Paul Murphy @ {cheeseh@rcbot.net}
 //
 // (http://www.rcbot.net)
 //
 // Based on botman's High Ping Bastard bot
 //
 // (http://planethalflife.com/botman/)
 //
 // engine.cpp
 //
 //////////////////////////////////////////////////
 //
 // engine functions from half-life
 //

#include "extdll.h"

#include "h_export_meta.h"
#include "dllapi.h"
#include "meta_api.h"
#include "engine.h"
#include "rcbot_message.h"
#include "rcbot_commands.h"
#include "rcbot_manager.h"

extern globalvars_t* gpGlobals;
extern enginefuncs_t g_engfuncs;
extern int debug_engine;

const char* GetArg(const char* command, int arg_number);

static FILE* fp;
/*
void pfnAlertMessage( ALERT_TYPE atype, char *szFmt, ... )
{
	assert ( atype != at_error );

#ifdef RCBOT_META_BUILD
	RETURN_META(MRES_IGNORED);
#else
	(*g_engfuncs.pfnAlertMessage)(atype,szFmt,...);
#endif
}
*/
int pfnPrecacheModel(char* s)
{
	if (debug_engine) { fp = fopen("bot.txt", "a"); fprintf(fp, "pfnPrecacheModel: %s\n", s); fclose(fp); }

	RETURN_META_VALUE(MRES_IGNORED, 0);

}
int pfnPrecacheSound(char* s)
{
	if (debug_engine) { fp = fopen("bot.txt", "a"); fprintf(fp, "pfnPrecacheSound: %s\n", s); fclose(fp); }

	RETURN_META_VALUE(MRES_IGNORED, 0);

}
void pfnSetModel(edict_t* e, const char* m)
{
	if (debug_engine) { fp = fopen("bot.txt", "a");
		fprintf(fp, "pfnSetModel: edict=%x %s\n", unsigned(e), m);
		fclose(fp); }

	RETURN_META(MRES_IGNORED);

}
int pfnModelIndex(const char* m)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnModelIndex: %s\n",m); fclose(fp); }

	RETURN_META_VALUE(MRES_IGNORED, 0);

}
int pfnModelFrames(int modelIndex)
{
	if (debug_engine) { fp = fopen("bot.txt", "a"); fprintf(fp, "pfnModelFrames:\n"); fclose(fp); }

	RETURN_META_VALUE(MRES_IGNORED, 0);

}
void pfnSetSize(edict_t* e, const float* rgflMin, const float* rgflMax)
{
	if (debug_engine) { fp = fopen("bot.txt", "a");
		fprintf(fp, "pfnSetSize: %x\n", unsigned(e));
		fclose(fp); }

	RETURN_META(MRES_IGNORED);

}
void pfnChangeLevel(char* s1, char* s2)
{
	if (debug_engine) { fp = fopen("bot.txt", "a"); fprintf(fp, "pfnChangeLevel:\n"); fclose(fp); }

	gRCBotManager.ChangeLevel();

	RETURN_META(MRES_IGNORED);

}
void pfnGetSpawnParms(edict_t* ent)
{
	if (debug_engine) { fp = fopen("bot.txt", "a"); fprintf(fp, "pfnGetSpawnParms:\n"); fclose(fp); }

	RETURN_META(MRES_IGNORED);

}
void pfnSaveSpawnParms(edict_t* ent)
{
	if (debug_engine) { fp = fopen("bot.txt", "a"); fprintf(fp, "pfnSaveSpawnParms:\n"); fclose(fp); }

	RETURN_META(MRES_IGNORED);

}
float pfnVecToYaw(const float* rgflVector)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnVecToYaw:\n"); fclose(fp); }

	RETURN_META_VALUE(MRES_IGNORED, 0);

}
void pfnVecToAngles(const float* rgflVectorIn, float* rgflVectorOut)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnVecToAngles:\n"); fclose(fp); }

	RETURN_META(MRES_IGNORED);

}
void pfnMoveToOrigin(edict_t* ent, const float* pflGoal, float dist, int iMoveType)
{
	if (debug_engine) { fp = fopen("bot.txt", "a"); fprintf(fp, "pfnMoveToOrigin:\n"); fclose(fp); }

	RETURN_META(MRES_IGNORED);

}
void pfnChangeYaw(edict_t* ent)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnChangeYaw:\n"); fclose(fp); }

	RETURN_META(MRES_IGNORED);

}
void pfnChangePitch(edict_t* ent)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnChangePitch:\n"); fclose(fp); }

	RETURN_META(MRES_IGNORED);

}
edict_t* pfnFindEntityByString(edict_t* pEdictStartSearchAfter, const char* pszField, const char* pszValue)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnFindEntityByString: %s\n",pszValue); fclose(fp); }

	RETURN_META_VALUE(MRES_IGNORED, nullptr);

}
int pfnGetEntityIllum(edict_t* pEnt)
{
	if (debug_engine) { fp = fopen("bot.txt", "a"); fprintf(fp, "pfnGetEntityIllum:\n"); fclose(fp); }

	RETURN_META_VALUE(MRES_IGNORED, 0);

}
edict_t* pfnFindEntityInSphere(edict_t* pEdictStartSearchAfter, const float* org, float rad)
{
	if (debug_engine) { fp = fopen("bot.txt", "a"); fprintf(fp, "pfnFindEntityInSphere:\n"); fclose(fp); }

	RETURN_META_VALUE(MRES_IGNORED, nullptr);

}
edict_t* pfnFindClientInPVS(edict_t* pEdict)
{
	if (debug_engine) { fp = fopen("bot.txt", "a"); fprintf(fp, "pfnFindClientInPVS:\n"); fclose(fp); }

	RETURN_META_VALUE(MRES_IGNORED, nullptr);

}
edict_t* pfnEntitiesInPVS(edict_t* pplayer)
{
	if (debug_engine) { fp = fopen("bot.txt", "a"); fprintf(fp, "pfnEntitiesInPVS:\n"); fclose(fp); }

	RETURN_META_VALUE(MRES_IGNORED, nullptr);

}
void pfnMakeVectors(const float* rgflVector)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnMakeVectors:\n"); fclose(fp); }

	RETURN_META(MRES_IGNORED);

}
void pfnAngleVectors(const float* rgflVector, float* forward, float* right, float* up)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnAngleVectors:\n"); fclose(fp); }

	RETURN_META(MRES_IGNORED);

}
edict_t* pfnCreateEntity(void)
{

	RETURN_META_VALUE(MRES_IGNORED, nullptr);

}
void pfnRemoveEntity(edict_t* e)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnRemoveEntity: %x\n",e); fclose(fp); }
	if (debug_engine)
	{
		fp = fopen("bot.txt", "a");
		fprintf(fp, "pfnRemoveEntity: %x\n", unsigned(e));
		if (e->v.model != 0)
			fprintf(fp, " model=%s\n", STRING(e->v.model));
		fclose(fp);
	}

	RETURN_META(MRES_IGNORED);

}
edict_t* pfnCreateNamedEntity(int className)
{
	RETURN_META_VALUE(MRES_IGNORED, nullptr);
}
void pfnMakeStatic(edict_t* ent)
{
	if (debug_engine) { fp = fopen("bot.txt", "a"); fprintf(fp, "pfnMakeStatic:\n"); fclose(fp); }

	RETURN_META(MRES_IGNORED);

}
int pfnEntIsOnFloor(edict_t* e)
{
	if (debug_engine) { fp = fopen("bot.txt", "a"); fprintf(fp, "pfnEntIsOnFloor:\n"); fclose(fp); }

	RETURN_META_VALUE(MRES_IGNORED, 0);

}
int pfnDropToFloor(edict_t* e)
{
	if (debug_engine) { fp = fopen("bot.txt", "a"); fprintf(fp, "pfnDropToFloor:\n"); fclose(fp); }

	RETURN_META_VALUE(MRES_IGNORED, 0);

}
int pfnWalkMove(edict_t* ent, float yaw, float dist, int iMode)
{
	if (debug_engine) { fp = fopen("bot.txt", "a"); fprintf(fp, "pfnWalkMove:\n"); fclose(fp); }

	RETURN_META_VALUE(MRES_IGNORED, 0);

}
void pfnSetOrigin(edict_t* e, const float* rgflOrigin)
{
	if (debug_engine) { fp = fopen("bot.txt", "a"); fprintf(fp, "pfnSetOrigin:\n"); fclose(fp); }

	RETURN_META(MRES_IGNORED);

}
void pfnEmitSound(edict_t* entity, int channel, const char* sample, /*int*/float volume, float attenuation, int fFlags, int pitch)
{
	/*
	if (entity != nullptr)
	{
		int i;
		CBot* pBot;

		Vector vOrigin;

		vOrigin = EntityOrigin(entity);

		eSoundType iSound = SOUND_UNKNOWN;

		if (gBotGlobals.IsMod(MOD_SVENCOOP))
		{
			if (entity->v.flags & FL_CLIENT)
			{
				if (strncmp(sample, "speech/saveme", 13) == 0)
					iSound = SOUND_NEEDHEALTH;
				else if (strncmp(sample, "speech/grenade", 14) == 0)
					iSound = SOUND_TAKE_COVER;
			}
		}
		else if (gBotGlobals.IsMod(MOD_TFC))
		{
			if (strncmp(sample, "speech/saveme", 13) == 0)
				iSound = SOUND_NEEDHEALTH;
		}

		if (iSound == SOUND_UNKNOWN)
		{
			// common sounds, like doors etc.
			if (sample[0] == 'd' && !strncmp(sample, "doors/", 6))
				iSound = SOUND_DOOR;
			else if (sample[0] == 'p' && !strncmp(sample, "plats/", 6))
				iSound = SOUND_DOOR;
			else if (sample[0] == 'w' && !strncmp(sample, "weapons/", 8))
				iSound = SOUND_WEAPON;
			else if (sample[0] == 'p' && !strncmp(sample, "player/", 7))
			{
				if (strncmp(&sample[7], "pain", 4) == 0)
					iSound = SOUND_PLAYER_PAIN;
				else
					iSound = SOUND_PLAYER;
			}
			else if (sample[0] == 'b' && !strncmp(sample, "buttons/", 8))
				iSound = SOUND_BUTTON;
		}

		if (iSound == SOUND_UNKNOWN)
		{
			switch (gBotGlobals.m_iCurrentMod)
			{
			case MOD_NS:
			{
				//NS sounds like gorges / taunts/ radio etc
				int sample_num = 0;

				// Starts with "vox/" ?? (Do it manually...)
				if (sample[0] == 'v' && sample[1] == 'o' &&
					sample[2] == 'x' && sample[3] == '/')
				{
					if (!strncmp(&sample[4], "ssay", 4)) // Marine Said Something
					{
						sample_num = atoi(&sample[8]);

						if (sample_num > 10 && sample_num < 20)
							iSound = SOUND_FOLLOW;
						else if (sample_num < 30)
							iSound = SOUND_COVERING;
						else if (sample_num < 40)
							iSound = SOUND_TAUNT;
						else if (sample_num < 50)
							iSound = SOUND_NEEDHEALTH;
						else if (sample_num < 60)
							iSound = SOUND_NEEDAMMO;
						else if (sample_num < 70)
							iSound = SOUND_INPOSITION;
						else if (sample_num < 80)
							iSound = SOUND_INCOMING;
						else if (sample_num < 90)
							iSound = SOUND_MOVEOUT;
						else if (sample_num < 100)
							iSound = SOUND_ALLCLEAR;
					}
					else if (!strncmp(&sample[4], "asay", 4)) // Alien Sound?
					{
						sample_num = atoi(&sample[8]);

						if (sample_num < 20)
							iSound = SOUND_UNKNOWN;

						if (sample_num < 30) // healing sounds end in ..21, ..22.wav etc
							iSound = SOUND_NEEDHEALTH;
						else if (sample_num < 40)
							iSound = SOUND_NEEDBACKUP;
						else if (sample_num < 50)
							iSound = SOUND_INCOMING;
						else if (sample_num < 60)
							iSound = SOUND_ATTACK;
						else if (sample_num < 70)
							iSound = SOUND_BUILDINGHERE;
					}
				}
			}
			break;
			case MOD_BUMPERCARS:
				// bumpercars sounds, like horns, taunts etc
				break;
			default:
				break;
			}
		}

		edict_t* pEntityOwner = entity->v.owner;

		for (i = 0; i < 32; i++)
		{
			pBot = &gBotGlobals.m_Bots[i];

			if (pBot == nullptr)
				continue;
			if (pBot->m_pEdict == nullptr)
				continue;
			if (pBot->m_pEdict == pEntityOwner)
				continue;
			if (!pBot->m_bIsUsed)
				continue;
			if (pBot->m_pEdict == entity)
				continue;
			if (pBot->m_pEnemy != nullptr)
				continue;

			if (pBot->DistanceFrom(vOrigin) < BOT_HEAR_DISTANCE)
				pBot->HearSound(iSound, vOrigin, entity);
		}
	}
	*/
	if (debug_engine) { fp = fopen("bot.txt", "a"); fprintf(fp, "pfnEmitSound:\n"); fclose(fp); }

	RETURN_META(MRES_IGNORED);

}
void pfnEmitAmbientSound(edict_t* entity, float* pos, const char* samp, float vol, float attenuation, int fFlags, int pitch)
{
	if (debug_engine) { fp = fopen("bot.txt", "a"); fprintf(fp, "pfnEmitAmbientSound:\n"); fclose(fp); }

	RETURN_META(MRES_IGNORED);

}
void pfnTraceLine(const float* v1, const float* v2, int fNoMonsters, edict_t* pentToSkip, TraceResult* ptr)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnTraceLine:\n"); fclose(fp); }

	RETURN_META(MRES_IGNORED);

}
void pfnTraceToss(edict_t* pent, edict_t* pentToIgnore, TraceResult* ptr)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnTraceToss:\n"); fclose(fp); }

	RETURN_META(MRES_IGNORED);

}
int pfnTraceMonsterHull(edict_t* pEdict, const float* v1, const float* v2, int fNoMonsters, edict_t* pentToSkip, TraceResult* ptr)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnTraceMonsterHull:\n"); fclose(fp); }

	RETURN_META_VALUE(MRES_IGNORED, 0);

}
void pfnTraceHull(const float* v1, const float* v2, int fNoMonsters, int hullNumber, edict_t* pentToSkip, TraceResult* ptr)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnTraceHull:\n"); fclose(fp); }

	RETURN_META(MRES_IGNORED);

}
void pfnTraceModel(const float* v1, const float* v2, int hullNumber, edict_t* pent, TraceResult* ptr)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnTraceModel:\n"); fclose(fp); }

	RETURN_META(MRES_IGNORED);

}
const char* pfnTraceTexture(edict_t* pTextureEntity, const float* v1, const float* v2)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnTraceTexture:\n"); fclose(fp); }

	RETURN_META_VALUE(MRES_IGNORED, nullptr);

}
void pfnTraceSphere(const float* v1, const float* v2, int fNoMonsters, float radius, edict_t* pentToSkip, TraceResult* ptr)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnTraceSphere:\n"); fclose(fp); }

	RETURN_META(MRES_IGNORED);

}
void pfnGetAimVector(edict_t* ent, float speed, float* rgflReturn)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnGetAimVector:\n"); fclose(fp); }

	RETURN_META(MRES_IGNORED);

}
void pfnServerCommand(char* str)
{
	if (debug_engine) { fp = fopen("bot.txt", "a"); fprintf(fp, "pfnServerCommand: %s\n", str); fclose(fp); }

	RETURN_META(MRES_IGNORED);

}
void pfnServerExecute(void)
{
	if (debug_engine) { fp = fopen("bot.txt", "a"); fprintf(fp, "pfnServerExecute:\n"); fclose(fp); }

	RETURN_META(MRES_IGNORED);

}
void pfnClientCommand(edict_t* pEdict, char* szFmt, ...)
{
	if (debug_engine) { fp = fopen("bot.txt", "a"); fprintf(fp, "pfnClientCommand=%s\n", szFmt); fclose(fp); }

	if (pEdict->v.flags & FL_FAKECLIENT)
		RETURN_META(MRES_SUPERCEDE);
	RETURN_META(MRES_IGNORED);

}
void pfnParticleEffect(const float* org, const float* dir, float color, float count)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnParticleEffect:\n"); fclose(fp); }

	RETURN_META(MRES_IGNORED);

}
void pfnLightStyle(int style, char* val)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnLightStyle:\n"); fclose(fp); }

	RETURN_META(MRES_IGNORED);

}
int pfnDecalIndex(const char* name)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnDecalIndex:\n"); fclose(fp); }

	RETURN_META_VALUE(MRES_IGNORED, 0);

}
int pfnPointContents(const float* rgflVector)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnPointContents:\n"); fclose(fp); }

	RETURN_META_VALUE(MRES_IGNORED, 0);

}

void pfnMessageBegin(int msg_dest, int msg_type, const float* pOrigin, edict_t* ed)
{
	if (RCBotMessage::CurrentMessage != nullptr)
	{
		delete RCBotMessage::CurrentMessage;
	}

	RCBotMessage::CurrentMessage = RCBotMessage::FindMessage(msg_type);

	if (RCBotMessage::CurrentMessage )
		RCBotMessage::CurrentMessage->messageBegin(msg_dest, pOrigin, ed);


	RETURN_META(MRES_IGNORED);

}

void pfnMessageEnd(void)
{
	if (gpGlobals->deathmatch)
	{
		if (debug_engine) { fp = fopen("bot.txt", "a"); fprintf(fp, "pfnMessageEnd:\n"); fclose(fp); }

		/*if (gBotGlobals.m_CurrentMessage)
		{
			if (gBotGlobals.m_pDebugMessage == gBotGlobals.m_CurrentMessage)//gBotGlobals.IsDebugLevelOn(BOT_DEBUG_MESSAGE_LEVEL) )
				ALERT(at_console, "---------MESSAGE_END(\"%s\")-------\n", gBotGlobals.m_CurrentMessage->getMessageName());

			if (gBotGlobals.m_CurrentMessage->isStateMsg())
				static_cast<CBotStatedNetMessage*>(gBotGlobals.m_CurrentMessage)->messageEnd();
			else
				gBotGlobals.m_CurrentMessage->execute(nullptr, gBotGlobals.m_iBotMsgIndex);  // nullptr indicated msg end
		}*/
	}

	// clear out the bot message function pointers...

	/*gBotGlobals.m_CurrentMessage = nullptr;
	gBotGlobals.m_iCurrentMessageState = 0;
	gBotGlobals.m_iCurrentMessageState2 = 0;

	gBotGlobals.m_bNetMessageStarted = FALSE;
	*/
	RETURN_META(MRES_IGNORED);

}
void pfnWriteByte(int iValue)
{
	if (gpGlobals->deathmatch)
	{
		if (debug_engine) { fp = fopen("bot.txt", "a"); fprintf(fp, "pfnWriteByte: %d\n", iValue); fclose(fp); }

		// if this message is for a bot, call the client message function...
		/*if (gBotGlobals.m_CurrentMessage)
		{
			if (gBotGlobals.m_pDebugMessage == gBotGlobals.m_CurrentMessage)//gBotGlobals.IsDebugLevelOn(BOT_DEBUG_MESSAGE_LEVEL) )
				ALERT(at_console, "WRITE_BYTE(%d)\n", iValue);

			if (gBotGlobals.m_CurrentMessage->isStateMsg())
				static_cast<CBotStatedNetMessage*>(gBotGlobals.m_CurrentMessage)->writeByte(iValue);
			else
				gBotGlobals.m_CurrentMessage->execute(static_cast<void*>(&iValue), gBotGlobals.m_iBotMsgIndex);
		}*/
	}

	RETURN_META(MRES_IGNORED);

}
void pfnWriteChar(int iValue)
{
	if (gpGlobals->deathmatch)
	{
		//if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnWriteChar: %d\n",iValue); fclose(fp); }

		// if this message is for a bot, call the client message function...
		if (RCBotMessage::CurrentMessage)
		{
			RCBotMessage::CurrentMessage->writeChar(static_cast<char>(iValue));
			/*
			if (gBotGlobals.m_pDebugMessage == gBotGlobals.m_CurrentMessage)//gBotGlobals.IsDebugLevelOn(BOT_DEBUG_MESSAGE_LEVEL) )
				ALERT(at_console, "WRITE_CHAR(%c)\n", (char)iValue);

			if (gBotGlobals.m_CurrentMessage->isStateMsg())
				static_cast<CBotStatedNetMessage*>(gBotGlobals.m_CurrentMessage)->writeChar((char)iValue);
			else
				gBotGlobals.m_CurrentMessage->execute(static_cast<void*>(&iValue), gBotGlobals.m_iBotMsgIndex);*/
		}
	}

	RETURN_META(MRES_IGNORED);

}
void pfnWriteShort(int iValue)
{
	if (gpGlobals->deathmatch)
	{
		if (debug_engine) { fp = fopen("bot.txt", "a"); fprintf(fp, "pfnWriteShort: %d\n", iValue); fclose(fp); }

		// if this message is for a bot, call the client message function...
		if (RCBotMessage::CurrentMessage)
		{
			RCBotMessage::CurrentMessage->writeShort(iValue);
			/*
			if (gBotGlobals.m_pDebugMessage == gBotGlobals.m_CurrentMessage)//gBotGlobals.IsDebugLevelOn(BOT_DEBUG_MESSAGE_LEVEL) )
				ALERT(at_console, "WRITE_SHORT(%d)\n", iValue);

			if (gBotGlobals.m_CurrentMessage->isStateMsg())
				static_cast<CBotStatedNetMessage*>(gBotGlobals.m_CurrentMessage)->writeShort(iValue);
			else
				gBotGlobals.m_CurrentMessage->execute(static_cast<void*>(&iValue), gBotGlobals.m_iBotMsgIndex);*/
		}
	}

	RETURN_META(MRES_IGNORED);

}
void pfnWriteLong(int iValue)
{
	if (gpGlobals->deathmatch)
	{
		if (debug_engine) { fp = fopen("bot.txt", "a"); fprintf(fp, "pfnWriteLong: %d\n", iValue); fclose(fp); }

		// if this message is for a bot, call the client message function...
		if (RCBotMessage::CurrentMessage)
		{
			/*if (gBotGlobals.m_pDebugMessage == gBotGlobals.m_CurrentMessage)
				//gBotGlobals.IsDebugLevelOn(BOT_DEBUG_MESSAGE_LEVEL) )
				ALERT(at_console, "WRITE_LONG(%d)\n", iValue);

			if (gBotGlobals.m_CurrentMessage->isStateMsg())
				static_cast<CBotStatedNetMessage*>(gBotGlobals.m_CurrentMessage)->writeLong(iValue);
			else
				gBotGlobals.m_CurrentMessage->execute(static_cast<void*>(&iValue), gBotGlobals.m_iBotMsgIndex);*/

			RCBotMessage::CurrentMessage->writeLong(iValue);
		}
	}

	RETURN_META(MRES_IGNORED);

}
void pfnWriteAngle(float flValue)
{
	if (gpGlobals->deathmatch)
	{
		if (debug_engine) { fp = fopen("bot.txt", "a"); fprintf(fp, "pfnWriteAngle: %f\n", flValue); fclose(fp); }

		// if this message is for a bot, call the client message function...
		if (RCBotMessage::CurrentMessage)
		{
			RCBotMessage::CurrentMessage->writeAngle(flValue);
			/*if (gBotGlobals.m_pDebugMessage == gBotGlobals.m_CurrentMessage)
				//gBotGlobals.IsDebugLevelOn(BOT_DEBUG_MESSAGE_LEVEL) )
				ALERT(at_console, "WRITE_ANGLE(%0.3f)\n", flValue);

			if (gBotGlobals.m_CurrentMessage->isStateMsg())
				static_cast<CBotStatedNetMessage*>(gBotGlobals.m_CurrentMessage)->writeAngle(flValue);
			else
				gBotGlobals.m_CurrentMessage->execute(static_cast<void*>(&flValue), gBotGlobals.m_iBotMsgIndex);*/
		}
	}

	RETURN_META(MRES_IGNORED);

}
void pfnWriteCoord(float flValue)
{
	if (gpGlobals->deathmatch)
	{
		if (debug_engine) { fp = fopen("bot.txt", "a"); fprintf(fp, "pfnWriteCoord: %f\n", flValue); fclose(fp); }

		// if this message is for a bot, call the client message function...
		if (RCBotMessage::CurrentMessage)
		{
			RCBotMessage::CurrentMessage->writeCoord(flValue);
			/*
			if (gBotGlobals.m_pDebugMessage == gBotGlobals.m_CurrentMessage)
				//gBotGlobals.IsDebugLevelOn(BOT_DEBUG_MESSAGE_LEVEL) )
				ALERT(at_console, "WRITE_COORD(%0.3f)\n", flValue);

			if (gBotGlobals.m_CurrentMessage->isStateMsg())
				static_cast<CBotStatedNetMessage*>(gBotGlobals.m_CurrentMessage)->writeCoord(flValue);
			else
				gBotGlobals.m_CurrentMessage->execute(static_cast<void*>(&flValue), gBotGlobals.m_iBotMsgIndex);*/
		}
	}

	RETURN_META(MRES_IGNORED);

}
void pfnWriteString(const char* sz)
{
	if (gpGlobals->deathmatch)
	{
		if (debug_engine) { fp = fopen("bot.txt", "a"); fprintf(fp, "pfnWriteString: %s\n", sz); fclose(fp); }

		// if this message is for a bot, call the client message function...
		if (RCBotMessage::CurrentMessage)
		{
			RCBotMessage::CurrentMessage->writeString(sz);
			/*
			if (gBotGlobals.m_pDebugMessage == gBotGlobals.m_CurrentMessage)
				//gBotGlobals.IsDebugLevelOn(BOT_DEBUG_MESSAGE_LEVEL) )
				ALERT(at_console, "WRITE_STRING(%s)\n", sz);

			if (gBotGlobals.m_CurrentMessage->isStateMsg())
				static_cast<CBotStatedNetMessage*>(gBotGlobals.m_CurrentMessage)->writeString(sz);
			else
				gBotGlobals.m_CurrentMessage->execute((void*)sz, gBotGlobals.m_iBotMsgIndex);*/
		}
	}

	RETURN_META(MRES_IGNORED);

}
void pfnWriteEntity(int iValue)
{
	if (gpGlobals->deathmatch)
	{
		if (debug_engine) { fp = fopen("bot.txt", "a"); fprintf(fp, "pfnWriteEntity: %d\n", iValue); fclose(fp); }

		// if this message is for a bot, call the client message function...
		if (RCBotMessage::CurrentMessage)
		{
			RCBotMessage::CurrentMessage->writeEntity(iValue);

			/*if (gBotGlobals.m_pDebugMessage == gBotGlobals.m_CurrentMessage)
				//gBotGlobals.IsDebugLevelOn(BOT_DEBUG_MESSAGE_LEVEL) )
				ALERT(at_console, "WRITE_ENTITY(%d)\n", iValue);

			if (gBotGlobals.m_CurrentMessage->isStateMsg())
				static_cast<CBotStatedNetMessage*>(gBotGlobals.m_CurrentMessage)->writeEntity(INDEXENT(iValue));
			else
				gBotGlobals.m_CurrentMessage->execute(static_cast<void*>(&iValue), gBotGlobals.m_iBotMsgIndex);*/
		}
	}

	RETURN_META(MRES_IGNORED);

}
void pfnCVarRegister(cvar_t* pCvar)
{
	if (debug_engine) { fp = fopen("bot.txt", "a"); fprintf(fp, "pfnCVarRegister:\n"); fclose(fp); }

	RETURN_META(MRES_IGNORED);

}
float pfnCVarGetFloat(const char* szVarName)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnCVarGetFloat: %s\n",szVarName); fclose(fp); }

	RETURN_META_VALUE(MRES_IGNORED, 0);

}
const char* pfnCVarGetString(const char* szVarName)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnCVarGetString:\n"); fclose(fp); }

	RETURN_META_VALUE(MRES_IGNORED, nullptr);

}
void pfnCVarSetFloat(const char* szVarName, float flValue)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnCVarSetFloat:\n"); fclose(fp); }

	RETURN_META(MRES_IGNORED);

}
void pfnCVarSetString(const char* szVarName, const char* szValue)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnCVarSetString:\n"); fclose(fp); }

	RETURN_META(MRES_IGNORED);

}
void* pfnPvAllocEntPrivateData(edict_t* pEdict, long cb)
{
	if (debug_engine) { fp = fopen("bot.txt", "a"); fprintf(fp, "pfnPvAllocEntPrivateData:\n"); fclose(fp); }

	RETURN_META_VALUE(MRES_IGNORED, nullptr);

}
void* pfnPvEntPrivateData(edict_t* pEdict)
{
	if (debug_engine) { fp = fopen("bot.txt", "a"); fprintf(fp, "pfnPvEntPrivateData:\n"); fclose(fp); }

	RETURN_META_VALUE(MRES_IGNORED, nullptr);

}
void pfnFreeEntPrivateData(edict_t* pEdict)
{
	if (debug_engine) { fp = fopen("bot.txt", "a"); fprintf(fp, "pfnFreeEntPrivateData:\n"); fclose(fp); }

	RETURN_META(MRES_IGNORED);

}
const char* pfnSzFromIndex(int iString)
{
	if (debug_engine) { fp = fopen("bot.txt", "a"); fprintf(fp, "pfnSzFromIndex:\n"); fclose(fp); }

	RETURN_META_VALUE(MRES_IGNORED, nullptr);

}
int pfnAllocString(const char* szValue)
{
	if (debug_engine) { fp = fopen("bot.txt", "a"); fprintf(fp, "pfnAllocString:\n"); fclose(fp); }

	RETURN_META_VALUE(MRES_IGNORED, 0);

}
entvars_t* pfnGetVarsOfEnt(edict_t* pEdict)
{
	if (debug_engine) { fp = fopen("bot.txt", "a"); fprintf(fp, "pfnGetVarsOfEnt:\n"); fclose(fp); }

	RETURN_META_VALUE(MRES_IGNORED, nullptr);

}
edict_t* pfnPEntityOfEntOffset(int iEntOffset)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnPEntityOfEntOffset:\n"); fclose(fp); }

	RETURN_META_VALUE(MRES_IGNORED, nullptr);

}
int pfnEntOffsetOfPEntity(const edict_t* pEdict)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnEntOffsetOfPEntity: %x\n",pEdict); fclose(fp); }

	RETURN_META_VALUE(MRES_IGNORED, 0);

}
int pfnIndexOfEdict(const edict_t* pEdict)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnIndexOfEdict: %x\n",pEdict); fclose(fp); }

	RETURN_META_VALUE(MRES_IGNORED, 0);

}
edict_t* pfnPEntityOfEntIndex(int iEntIndex)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnPEntityOfEntIndex:\n"); fclose(fp); }

	RETURN_META_VALUE(MRES_IGNORED, nullptr);

}
edict_t* pfnFindEntityByVars(entvars_t* pvars)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnFindEntityByVars:\n"); fclose(fp); }
	RETURN_META_VALUE(MRES_IGNORED, nullptr);

}
void* pfnGetModelPtr(edict_t* pEdict)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnGetModelPtr: %x\n",pEdict); fclose(fp); }
	RETURN_META_VALUE(MRES_IGNORED, nullptr);
}

int pfnRegUserMsg(const char* pszName, int iSize)
{
	int msg = 0;

	extern plugin_info_t Plugin_info;

	msg = GET_USER_MSG_ID(&Plugin_info, pszName, &iSize);

	//gBotGlobals.m_NetEntityMessages.UpdateMessage(pszName, msg, iSize);
	//gBotGlobals.m_NetAllMessages.UpdateMessage(pszName, msg, iSize);

	RETURN_META_VALUE(MRES_IGNORED, 0);

}

void pfnAnimationAutomove(const edict_t* pEdict, float flTime)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnAnimationAutomove:\n"); fclose(fp); }

	RETURN_META(MRES_IGNORED);
}

void pfnGetBonePosition(const edict_t* pEdict, int iBone, float* rgflOrigin, float* rgflAngles)
{
	RETURN_META(MRES_IGNORED);
}

unsigned long pfnFunctionFromName(const char* pName)
{
	if (debug_engine) { fp = fopen("bot.txt", "a"); fprintf(fp, "pfnFunctionFromName:\n"); fclose(fp); }

	RETURN_META_VALUE(MRES_IGNORED, 0);
}

const char* pfnNameForFunction(unsigned long function)
{
	if (debug_engine) { fp = fopen("bot.txt", "a"); fprintf(fp, "pfnNameForFunction:\n"); fclose(fp); }

	RETURN_META_VALUE(MRES_IGNORED, nullptr);
}

void pfnClientPrintf(edict_t* pEdict, PRINT_TYPE ptype, const char* szMsg)
{
	if (debug_engine) { fp = fopen("bot.txt", "a"); fprintf(fp, "pfnClientPrintf:\n"); fclose(fp); }

	RETURN_META(MRES_IGNORED);
}

void pfnServerPrint(const char* szMsg)
{
	if (debug_engine) { fp = fopen("bot.txt", "a"); fprintf(fp, "pfnServerPrint: %s\n", szMsg); fclose(fp); }

	RETURN_META(MRES_IGNORED);

}
void pfnGetAttachment(const edict_t* pEdict, int iAttachment, float* rgflOrigin, float* rgflAngles)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnGetAttachment:\n"); fclose(fp); }

	RETURN_META(MRES_IGNORED);

}
void pfnCRC32_Init(CRC32_t* pulCRC)
{
	if (debug_engine) { fp = fopen("bot.txt", "a"); fprintf(fp, "pfnCRC32_Init:\n"); fclose(fp); }

	RETURN_META(MRES_IGNORED);

}
void pfnCRC32_ProcessBuffer(CRC32_t* pulCRC, void* p, int len)
{
	if (debug_engine) { fp = fopen("bot.txt", "a"); fprintf(fp, "pfnCRC32_ProcessBuffer:\n"); fclose(fp); }

	RETURN_META(MRES_IGNORED);

}
void pfnCRC32_ProcessByte(CRC32_t* pulCRC, unsigned char ch)
{
	if (debug_engine) { fp = fopen("bot.txt", "a"); fprintf(fp, "pfnCRC32_ProcessByte:\n"); fclose(fp); }

	RETURN_META(MRES_IGNORED);

}
CRC32_t pfnCRC32_Final(CRC32_t pulCRC)
{
	if (debug_engine) { fp = fopen("bot.txt", "a"); fprintf(fp, "pfnCRC32_Final:\n"); fclose(fp); }

	RETURN_META_VALUE(MRES_IGNORED, pulCRC);

}
long pfnRandomLong(long lLow, long lHigh)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnRandomLong: lLow=%d lHigh=%d\n",lLow,lHigh); fclose(fp); }

	RETURN_META_VALUE(MRES_IGNORED, 0);

}
float pfnRandomFloat(float flLow, float flHigh)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnRandomFloat:\n"); fclose(fp); }

	RETURN_META_VALUE(MRES_IGNORED, 0);

}
void pfnSetView(const edict_t* pClient, const edict_t* pViewent)
{
	if (debug_engine) { fp = fopen("bot.txt", "a"); fprintf(fp, "pfnSetView:\n"); fclose(fp); }

	RETURN_META(MRES_IGNORED);

}
float pfnTime(void)
{
	if (debug_engine) { fp = fopen("bot.txt", "a"); fprintf(fp, "pfnTime:\n"); fclose(fp); }

	RETURN_META_VALUE(MRES_IGNORED, 0);

}
void pfnCrosshairAngle(const edict_t* pClient, float pitch, float yaw)
{
	if (debug_engine) { fp = fopen("bot.txt", "a"); fprintf(fp, "pfnCrosshairAngle:\n"); fclose(fp); }

	RETURN_META(MRES_IGNORED);

}
byte* pfnLoadFileForMe(char* filename, int* pLength)
{
	if (debug_engine) { fp = fopen("bot.txt", "a"); fprintf(fp, "pfnLoadFileForMe: filename=%s\n", filename); fclose(fp); }

	RETURN_META_VALUE(MRES_IGNORED, 0);

}
void pfnFreeFile(void* buffer)
{
	if (debug_engine) { fp = fopen("bot.txt", "a"); fprintf(fp, "pfnFreeFile:\n"); fclose(fp); }

	RETURN_META(MRES_IGNORED);

}
void pfnEndSection(const char* pszSectionName)
{
	if (debug_engine) { fp = fopen("bot.txt", "a"); fprintf(fp, "pfnEndSection:\n"); fclose(fp); }

	RETURN_META(MRES_IGNORED);

}
int pfnCompareFileTime(char* filename1, char* filename2, int* iCompare)
{
	if (debug_engine) { fp = fopen("bot.txt", "a"); fprintf(fp, "pfnCompareFileTime:\n"); fclose(fp); }

	RETURN_META_VALUE(MRES_IGNORED, 0);

}
void pfnGetGameDir(char* szGetGameDir)
{
	if (debug_engine) { fp = fopen("bot.txt", "a"); fprintf(fp, "pfnGetGameDir:\n"); fclose(fp); }

	RETURN_META(MRES_IGNORED);

}
void pfnCvar_RegisterVariable(cvar_t* variable)
{
	if (debug_engine) { fp = fopen("bot.txt", "a"); fprintf(fp, "pfnCvar_RegisterVariable:\n"); fclose(fp); }

	RETURN_META(MRES_IGNORED);

}
void pfnFadeClientVolume(const edict_t* pEdict, int fadePercent, int fadeOutSeconds, int holdTime, int fadeInSeconds)
{
	if (debug_engine) { fp = fopen("bot.txt", "a"); fprintf(fp, "pfnFadeClientVolume:\n"); fclose(fp); }

	RETURN_META(MRES_IGNORED);

}
void pfnSetClientMaxspeed(const edict_t* pEdict, const float fNewMaxspeed)
{
	// Is this player a bot?
	//CBot* pBot = UTIL_GetBotPointer(pEdict);

	/*if (pBot)
	{
		pBot->m_fMaxSpeed = fNewMaxspeed;
	}*/

	if (debug_engine) 
	{ 
		fp = fopen("bot.txt", "a");
		fprintf(fp, "pfnSetClientMaxspeed: edict=%x %f\n", unsigned(pEdict), fNewMaxspeed);
		fclose(fp); 
	}

	RETURN_META(MRES_IGNORED);

}
edict_t* pfnCreateFakeClient(const char* netname)
{
	if (debug_engine) { fp = fopen("bot.txt", "a"); fprintf(fp, "pfnCreateFakeClient:\n"); fclose(fp); }

	RETURN_META_VALUE(MRES_IGNORED, nullptr);

}
void pfnRunPlayerMove(edict_t* fakeclient, const float* viewangles, float forwardmove, float sidemove, float upmove, unsigned short buttons, byte impulse, byte msec)
{
	if (debug_engine) { fp = fopen("bot.txt", "a"); fprintf(fp, "pfnRunPlayerMove:\n"); fclose(fp); }

	RETURN_META(MRES_IGNORED);

}
int pfnNumberOfEntities(void)
{
	if (debug_engine) { fp = fopen("bot.txt", "a"); fprintf(fp, "pfnNumberOfEntities:\n"); fclose(fp); }

	RETURN_META_VALUE(MRES_IGNORED, 0);

}
char* pfnGetInfoKeyBuffer(edict_t* e)
{
	if (debug_engine) { fp = fopen("bot.txt", "a"); fprintf(fp, "pfnGetInfoKeyBuffer:\n"); fclose(fp); }

	RETURN_META_VALUE(MRES_IGNORED, nullptr);

}
char* pfnInfoKeyValue(char* infobuffer, char* key)
{
	if (debug_engine) { fp = fopen("bot.txt", "a"); fprintf(fp, "pfnInfoKeyValue: %s %s\n", infobuffer, key); fclose(fp); }

	RETURN_META_VALUE(MRES_IGNORED, nullptr);

}
void pfnSetKeyValue(char* infobuffer, char* key, char* value)
{
	if (debug_engine) { fp = fopen("bot.txt", "a"); fprintf(fp, "pfnSetKeyValue: %s %s\n", key, value); fclose(fp); }

	RETURN_META(MRES_IGNORED);

}
void pfnSetClientKeyValue(int clientIndex, char* infobuffer, char* key, char* value)
{
	edict_t* pEdict = INDEXENT(clientIndex);

	if (debug_engine) { fp = fopen("bot.txt", "a"); fprintf(fp, "pfnSetClientKeyValue: %s %s\n", key, value); fclose(fp); }

	RETURN_META(MRES_IGNORED);

}
int pfnIsMapValid(char* filename)
{
	if (debug_engine) { fp = fopen("bot.txt", "a"); fprintf(fp, "pfnIsMapValid:\n"); fclose(fp); }

	RETURN_META_VALUE(MRES_IGNORED, 0);

}
void pfnStaticDecal(const float* origin, int decalIndex, int entityIndex, int modelIndex)
{
	if (debug_engine) { fp = fopen("bot.txt", "a"); fprintf(fp, "pfnStaticDecal:\n"); fclose(fp); }

	RETURN_META(MRES_IGNORED);

}
int pfnPrecacheGeneric(char* s)
{
	if (debug_engine) { fp = fopen("bot.txt", "a"); fprintf(fp, "pfnPrecacheGeneric: %s\n", s); fclose(fp); }

	RETURN_META_VALUE(MRES_IGNORED, 0);

}
int pfnGetPlayerUserId(edict_t* e)
{
	if (gpGlobals->deathmatch)
	{
		if (debug_engine) { fp = fopen("bot.txt", "a");
			fprintf(fp, "pfnGetPlayerUserId: %x\n", unsigned(e));
			fclose(fp); }
	}

	RETURN_META_VALUE(MRES_IGNORED, 0);

}

const char* pfnGetPlayerAuthId(edict_t* e)
{
	/*static const char* BOT_STEAM_ID = "BOT";
	BOOL bIsBot = UTIL_GetBotPointer(e) != nullptr;

	if (bIsBot)
		RETURN_META_VALUE(MRES_SUPERCEDE, BOT_STEAM_ID);
*/
	RETURN_META_VALUE(MRES_IGNORED, nullptr);

}

void pfnBuildSoundMsg(edict_t* entity, int channel, const char* sample, /*int*/float volume, float attenuation, int fFlags, int pitch, int msg_dest, int msg_type, const float* pOrigin, edict_t* ed)
{
	if (debug_engine) { fp = fopen("bot.txt", "a"); fprintf(fp, "pfnBuildSoundMsg:\n"); fclose(fp); }

	RETURN_META(MRES_IGNORED);

}
int pfnIsDedicatedServer(void)
{
	if (debug_engine) { fp = fopen("bot.txt", "a"); fprintf(fp, "pfnIsDedicatedServer:\n"); fclose(fp); }

	RETURN_META_VALUE(MRES_IGNORED, 0);

}
cvar_t* pfnCVarGetPointer(const char* szVarName)
{
	if (debug_engine) { fp = fopen("bot.txt", "a"); fprintf(fp, "pfnCVarGetPointer: %s\n", szVarName); fclose(fp); }

	RETURN_META_VALUE(MRES_IGNORED, nullptr);

}
unsigned int pfnGetPlayerWONId(edict_t* e)
{
	if (debug_engine) { fp = fopen("bot.txt", "a");
		fprintf(fp, "pfnGetPlayerWONId: %x\n", unsigned(e));
		fclose(fp); }

	RETURN_META_VALUE(MRES_IGNORED, 0);

}

// new stuff for SDK 2.0

void pfnInfo_RemoveKey(char* s, const char* key)
{
	if (debug_engine) { fp = fopen("bot.txt", "a"); fprintf(fp, "pfnInfo_RemoveKey:\n"); fclose(fp); }

	RETURN_META(MRES_IGNORED);

}
const char* pfnGetPhysicsKeyValue(const edict_t* pClient, const char* key)
{
	if (debug_engine) { fp = fopen("bot.txt", "a"); fprintf(fp, "pfnGetPhysicsKeyValue:\n"); fclose(fp); }

	RETURN_META_VALUE(MRES_IGNORED, nullptr);

}
void pfnSetPhysicsKeyValue(const edict_t* pClient, const char* key, const char* value)
{
	if (debug_engine) { fp = fopen("bot.txt", "a"); fprintf(fp, "pfnSetPhysicsKeyValue:\n"); fclose(fp); }

	RETURN_META(MRES_IGNORED);

}
const char* pfnGetPhysicsInfoString(const edict_t* pClient)
{
	if (debug_engine) { fp = fopen("bot.txt", "a"); fprintf(fp, "pfnGetPhysicsInfoString:\n"); fclose(fp); }

	RETURN_META_VALUE(MRES_IGNORED, nullptr);

}
unsigned short pfnPrecacheEvent(int type, const char* psz)
{
	if (debug_engine) { fp = fopen("bot.txt", "a"); fprintf(fp, "pfnPrecacheEvent:\n"); fclose(fp); }

	RETURN_META_VALUE(MRES_IGNORED, 0);

}
void pfnPlaybackEvent(int flags, const edict_t* pInvoker, unsigned short eventindex, float delay,
	float* origin, float* angles, float fparam1, float fparam2, int iparam1, int iparam2, int bparam1, int bparam2)
{
	if (debug_engine) { fp = fopen("bot.txt", "a"); fprintf(fp, "pfnPlaybackEvent:\n"); fclose(fp); }

	RETURN_META(MRES_IGNORED);

}
unsigned char* pfnSetFatPVS(float* org)
{
	if (debug_engine) { fp = fopen("bot.txt", "a"); fprintf(fp, "pfnSetFatPVS:\n"); fclose(fp); }

	RETURN_META_VALUE(MRES_IGNORED, nullptr);

}
unsigned char* pfnSetFatPAS(float* org)
{
	if (debug_engine) { fp = fopen("bot.txt", "a"); fprintf(fp, "pfnSetFatPAS:\n"); fclose(fp); }

	RETURN_META_VALUE(MRES_IGNORED, nullptr);

}
int pfnCheckVisibility(const edict_t* entity, unsigned char* pset)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnCheckVisibility:\n"); fclose(fp); }

	RETURN_META_VALUE(MRES_IGNORED, 0);

}
void pfnDeltaSetField(struct delta_s* pFields, const char* fieldname)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnDeltaSetField:\n"); fclose(fp); }

	RETURN_META(MRES_IGNORED);

}
void pfnDeltaUnsetField(struct delta_s* pFields, const char* fieldname)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnDeltaUnsetField:\n"); fclose(fp); }

	RETURN_META(MRES_IGNORED);

}
void pfnDeltaAddEncoder(char* name, void (*conditionalencode)(struct delta_s* pFields, const unsigned char* from, const unsigned char* to))
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnDeltaAddEncoder:\n"); fclose(fp); }

	RETURN_META(MRES_IGNORED);

}
int pfnGetCurrentPlayer(void)
{
	if (debug_engine) { fp = fopen("bot.txt", "a"); fprintf(fp, "pfnGetCurrentPlayer:\n"); fclose(fp); }

	RETURN_META_VALUE(MRES_IGNORED, 0);

}
int pfnCanSkipPlayer(const edict_t* player)
{
	if (debug_engine) { fp = fopen("bot.txt", "a"); fprintf(fp, "pfnCanSkipPlayer:\n"); fclose(fp); }

	RETURN_META_VALUE(MRES_IGNORED, 0);

}
int pfnDeltaFindField(struct delta_s* pFields, const char* fieldname)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnDeltaFindField:\n"); fclose(fp); }

	RETURN_META_VALUE(MRES_IGNORED, 0);

}
void pfnDeltaSetFieldByIndex(struct delta_s* pFields, int fieldNumber)
{
	if (debug_engine) { fp = fopen("bot.txt", "a"); fprintf(fp, "pfnDeltaSetFieldByIndex:\n"); fclose(fp); }

	RETURN_META(MRES_IGNORED);

}
void pfnDeltaUnsetFieldByIndex(struct delta_s* pFields, int fieldNumber)
{
	//   if (debug_engine) { fp=fopen("bot.txt","a"); fprintf(fp,"pfnDeltaUnsetFieldByIndex:\n"); fclose(fp); }

	RETURN_META(MRES_IGNORED);

}
void pfnSetGroupMask(int mask, int op)
{
	if (debug_engine) { fp = fopen("bot.txt", "a"); fprintf(fp, "pfnSetGroupMask:\n"); fclose(fp); }

	RETURN_META(MRES_IGNORED);

}
int pfnCreateInstancedBaseline(int classname, struct entity_state_s* baseline)
{
	if (debug_engine) { fp = fopen("bot.txt", "a"); fprintf(fp, "pfnCreateInstancedBaseline:\n"); fclose(fp); }

	RETURN_META_VALUE(MRES_IGNORED, 0);

}
void pfnCvar_DirectSet(struct cvar_s* var, char* value)
{
	if (debug_engine) { fp = fopen("bot.txt", "a"); fprintf(fp, "pfnCvar_DirectSet:\n"); fclose(fp); }

	RETURN_META(MRES_IGNORED);

}
void pfnForceUnmodified(FORCE_TYPE type, float* mins, float* maxs, const char* filename)
{
	if (debug_engine) { fp = fopen("bot.txt", "a"); fprintf(fp, "pfnForceUnmodified:\n"); fclose(fp); }

	RETURN_META(MRES_IGNORED);

}
void pfnGetPlayerStats(const edict_t* pClient, int* ping, int* packet_loss)
{
	if (debug_engine) { fp = fopen("bot.txt", "a"); fprintf(fp, "pfnGetPlayerStats:\n"); fclose(fp); }

	RETURN_META(MRES_IGNORED);

}

int pfnCmd_Argc(void)
{
	// this function returns the number of arguments the current client command string has. Since
	// bots have no client DLL and we may want a bot to execute a client command, we had to
	// implement a g_argv string in the bot DLL for holding the bots' commands, and also keep
	// track of the argument count. Hence this hook not to let the engine ask an unexistent client
	// DLL for a command we are holding here. Of course, real clients commands are still retrieved
	// the normal way, by asking the engine.

	if (RCBotCommands_MainCommand::IsFakeClientCommand)
		RETURN_META_VALUE(MRES_SUPERCEDE, RCBotCommands_MainCommand::NumArgs);

	RETURN_META_VALUE(MRES_IGNORED, 0);

}

const char* pfnCmd_Args(void)
{
	// this function returns a pointer to the whole current client command string. Since bots
	// have no client DLL and we may want a bot to execute a client command, we had to implement
	// a g_argv string in the bot DLL for holding the bots' commands, and also keep track of the
	// argument count. Hence this hook not to let the engine ask an unexistent client DLL for a
	// command we are holding here. Of course, real clients commands are still retrieved the
	// normal way, by asking the engine.

	extern char* g_argv;

	// is this a bot issuing that client command ?
	if (RCBotCommands_MainCommand::IsFakeClientCommand)
	{
		// is it a "say" or "say_team" client command ?
		if (strncmp("say ", g_argv, 4) == 0)
			RETURN_META_VALUE(MRES_SUPERCEDE, &g_argv[0] + 4); // skip the "say" bot client command (bug in HL engine)
		else if (strncmp("say_team ", g_argv, 9) == 0)
			RETURN_META_VALUE(MRES_SUPERCEDE, &g_argv[0] + 9); // skip the "say_team" bot client command (bug in HL engine)

		RETURN_META_VALUE(MRES_SUPERCEDE, &g_argv[0]); // else return the whole bot client command string we know
	}

	RETURN_META_VALUE(MRES_IGNORED, nullptr);

}

const char* pfnCmd_Argv(int argc)
{
	// this function returns a pointer to a certain argument of the current client command. Since
	// bots have no client DLL and we may want a bot to execute a client command, we had to
	// implement a g_argv string in the bot DLL for holding the bots' commands, and also keep
	// track of the argument count. Hence this hook not to let the engine ask an unexistent client
	// DLL for a command we are holding here. Of course, real clients commands are still retrieved
	// the normal way, by asking the engine.

	extern char* g_argv;

	if (RCBotCommands_MainCommand::IsFakeClientCommand)
	{
		RETURN_META_VALUE(MRES_SUPERCEDE, GetArg(g_argv, argc));
	}
	else
	{
		RETURN_META_VALUE(MRES_IGNORED, nullptr);
		//return ((*g_engfuncs.pfnCmd_Argv) (argc));
	}
}

const char* GetArg(const char* command, int arg_number)
{
	// the purpose of this function is to provide fakeclients (bots) with the same Cmd_Argv
	// convenience the engine provides to real clients. This way the handling of real client
	// commands and bot client commands is exactly the same, just have a look in engine.cpp
	// for the hooking of pfnCmd_Argc, pfnCmd_Args and pfnCmd_Argv, which redirects the call
	// either to the actual engine functions (when the caller is a real client), either on
	// our function here, which does the same thing, when the caller is a bot.

	int length, i, index = 0, arg_count = 0, fieldstart, fieldstop;

	static char arg[1024];

	//	if ( arg_number == 0 )
	arg[0] = 0; // reset arg

	if (!command || !*command)
		return nullptr;

	length = strlen(command); // get length of command

	// while we have not reached end of line
	while (index < length && arg_count <= arg_number)
	{
		while (index < length && command[index] == ' ')
			index++; // ignore spaces

		// is this field multi-word between quotes or single word ?
		if (command[index] == '"')
		{
			index++; // move one step further to bypass the quote
			fieldstart = index; // save field start position
			while (index < length && command[index] != '"')
				index++; // reach end of field
			fieldstop = index - 1; // save field stop position
			index++; // move one step further to bypass the quote
		}
		else
		{
			fieldstart = index; // save field start position
			while (index < length && command[index] != ' ')
				index++; // reach end of field
			fieldstop = index - 1; // save field stop position
		}

		// is this argument we just processed the wanted one ?
		if (arg_count == arg_number)
		{
			for (i = fieldstart; i <= fieldstop; i++)
				arg[i - fieldstart] = command[i]; // store the field value in a string
			arg[i - fieldstart] = 0; // terminate the string
		}

		arg_count++; // we have processed one argument more
	}

	return &arg[0]; // returns the wanted argument
}