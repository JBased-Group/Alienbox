#ifndef C_ABOX_PLAYER_H
#define C_ABOX_PLAYER_H
#ifdef _WIN32
#pragma once
#endif

#include "c_baseplayer.h"

class C_ABox_Player : public C_BasePlayer
{
public:
	DECLARE_CLASS(C_ABox_Player, C_BasePlayer);
	DECLARE_CLIENTCLASS();
	C_ABox_Player();
	~C_ABox_Player();
	virtual int DrawModel(int flags, const RenderableInstance_t& instance);
};

inline C_ABox_Player* ToABox_Player(CBaseEntity* pEntity)
{
	if (!pEntity || !pEntity->IsPlayer())
		return NULL;

	Assert(dynamic_cast<C_ABox_Player*>(pEntity) != NULL);
	return static_cast<C_ABox_Player*>(pEntity);
}

inline C_ABox_Player* ToABox_Player(CBasePlayer* pPlayer)
{
	if (!pPlayer)
		return NULL;

	//Assert(dynamic_cast<C_ASW_Player*>(pPlayer) != NULL);
	return dynamic_cast<C_ABox_Player*>(pPlayer);
}

#endif