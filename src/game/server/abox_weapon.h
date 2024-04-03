#ifndef ABOXWEAPON_H
#define ABOXWEAPON_H
#ifdef _WIN32
#pragma once
#endif

#include "basecombatweapon_shared.h"

class CABox_Weapon : public CBaseCombatWeapon
{
	DECLARE_CLASS(CABox_Weapon, CBaseCombatWeapon);
	DECLARE_SERVERCLASS();
public:
	CABox_Weapon();
	~CABox_Weapon();
};
#endif

