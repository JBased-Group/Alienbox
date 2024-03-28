#ifndef CABOX_PLAYER_H
#define CABOX_PLAYER_H
#ifdef _WIN32
#pragma once
#endif

#include "player.h"

class CABox_Player : public CBasePlayer
{
public:
	DECLARE_CLASS(CABox_Player, CBasePlayer);
	DECLARE_SERVERCLASS();
	virtual void Spawn();
	static CABox_Player* CreatePlayer(const char* className, edict_t* ed);
	virtual void	PickupObject(CBaseEntity* pObject, bool bLimitMassAndSize = true);
};

inline CABox_Player* ToABox_Player(CBaseEntity* pEntity)
{
	if (!pEntity || !pEntity->IsPlayer())
		return NULL;

#ifdef _DEBUG
	Assert(dynamic_cast<CABox_Player*>(pEntity) != 0);
#endif
	return static_cast<CABox_Player*>(pEntity);
}

inline CABox_Player* ToABox_Player(CBasePlayer* pPlayer)
{
	if (!pPlayer)
		return NULL;

#ifdef _DEBUG
	Assert(dynamic_cast<CABox_Player*>(pPlayer) != 0);
#endif
	return dynamic_cast<CABox_Player*>(pPlayer);
}

#endif
