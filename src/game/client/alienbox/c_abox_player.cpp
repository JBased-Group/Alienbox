#include "cbase.h"
#include "c_abox_player.h"
#include "in_buttons.h"

IMPLEMENT_CLIENTCLASS_DT(C_ABox_Player, DT_ABox_Player, CABox_Player)
END_RECV_TABLE()

LINK_ENTITY_TO_CLASS(player, C_ABox_Player);

C_ABox_Player::C_ABox_Player() : BaseClass()
{
}

C_ABox_Player::~C_ABox_Player()
{

}

bool C_ABox_Player::CreateMove(float flInputSampleTime, CUserCmd* pCmd)
{
	if (m_hUseEntity != nullptr && (pCmd->buttons & IN_RELOAD))
	{
		VectorCopy(m_vecOldViewAngles, pCmd->viewangles);
	}
	return BaseClass::CreateMove(flInputSampleTime, pCmd);
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

class C_PlayerPickupController : public CBaseEntity
{
public:
	DECLARE_CLASS(C_PlayerPickupController, CBaseEntity);
	DECLARE_CLIENTCLASS();
	C_PlayerPickupController() : BaseClass()
	{

	}
	~C_PlayerPickupController()
	{

	}
};

IMPLEMENT_CLIENTCLASS_DT(C_PlayerPickupController, DT_PlayerPickupController, CPlayerPickupController)
END_RECV_TABLE()
