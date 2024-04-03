#ifndef C_ABOXWEAPON_H
#define C_ABOXWEAPON_H
#ifdef _WIN32
#pragma once
#endif

#include "basecombatweapon_shared.h"

class C_ABox_Weapon : public C_BaseCombatWeapon
{
	DECLARE_CLASS(C_ABox_Weapon, C_BaseCombatWeapon);
	DECLARE_CLIENTCLASS();
public:
	C_ABox_Weapon();
	~C_ABox_Weapon();
};

#endif