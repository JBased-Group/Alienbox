#include "cbase.h"
#include "abox_weapon.h"

CABox_Weapon::CABox_Weapon() : BaseClass()
{

}

CABox_Weapon::~CABox_Weapon()
{

}


LINK_ENTITY_TO_CLASS(weapon_sample, CABox_Weapon);

IMPLEMENT_SERVERCLASS_ST(CABox_Weapon, DT_ABox_Weapon)
END_SEND_TABLE()