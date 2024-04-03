#include "cbase.h"
#include "c_abox_weapon.h"

C_ABox_Weapon::C_ABox_Weapon() : BaseClass()
{

}

C_ABox_Weapon::~C_ABox_Weapon()
{

}

IMPLEMENT_CLIENTCLASS_DT(C_ABox_Weapon, DT_ABox_Weapon, CABox_Weapon)
END_RECV_TABLE()