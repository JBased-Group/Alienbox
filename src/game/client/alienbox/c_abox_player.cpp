#include "cbase.h"
#include "c_abox_player.h"

IMPLEMENT_CLIENTCLASS_DT(C_ABox_Player, DT_ABox_Player, CABox_Player)
END_RECV_TABLE()

LINK_ENTITY_TO_CLASS(player, C_ABox_Player);

C_ABox_Player::C_ABox_Player() : BaseClass()
{
}

C_ABox_Player::~C_ABox_Player()
{

}

int C_ABox_Player::DrawModel(int flags, const RenderableInstance_t& instance)
{
	// Not pitch for player
	QAngle saveAngles = GetLocalAngles();

	QAngle useAngles = saveAngles;
	useAngles[PITCH] = 0.0f;

	SetLocalAngles(useAngles);

	int iret = BaseClass::DrawModel(flags,instance);

	SetLocalAngles(saveAngles);

	return iret;
}

