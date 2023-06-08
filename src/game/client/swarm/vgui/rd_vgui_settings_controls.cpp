#include "cbase.h"
#include "rd_vgui_settings.h"
#include <vgui_controls/Label.h>
#include "gameui/swarm/vhybridbutton.h"
#include "gameui/cvartogglecheckbutton.h"
#include "rd_steam_input.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

DECLARE_BUILD_FACTORY( CRD_VGUI_Settings_Controls );

CRD_VGUI_Bind::CRD_VGUI_Bind( vgui::Panel *parent, const char *panelName, const char *szLabel, const char *szBind, bool bUseRowLayout ) :
	BaseClass( parent, panelName )
{
	Assert( g_RD_Steam_Input.IsSteamInputBind( szBind ) );

	V_strncpy( m_szBind, szBind, sizeof( m_szBind ) );
	m_bUseRowLayout = bUseRowLayout;

	m_pLblDescription = new vgui::Label( this, "LblDescription", szLabel );
}

void CRD_VGUI_Bind::AddFallbackBind( const char *szBind )
{
	m_AlternateBind.CopyAndAddToTail( szBind );
}

CRD_VGUI_Settings_Controls::CRD_VGUI_Settings_Controls( vgui::Panel *parent, const char *panelName ) :
	BaseClass( parent, panelName )
{
	// Movement
	m_pBindMoveForward = new CRD_VGUI_Bind( this, "BindMoveForward", "#ASW_GameUI_MoveForward", "+forward", false );
	m_pBindMoveLeft = new CRD_VGUI_Bind( this, "BindMoveLeft", "#ASW_GameUI_MoveLeft", "+moveleft", false );
	m_pBindMoveBack = new CRD_VGUI_Bind( this, "BindMoveBack", "#ASW_GameUI_MoveBack", "+back", false );
	m_pBindMoveRight = new CRD_VGUI_Bind( this, "BindMoveRight", "#ASW_GameUI_MoveRight", "+moveright", false );
	m_pBindWalk = new CRD_VGUI_Bind( this, "BindWalk", "#Valve_Walk", "+walk", false );
	m_pBindJump = new CRD_VGUI_Bind( this, "BindJump", "#ASW_GameUI_MoveRoll", "+jump", false );

	// Controller
	// TODO
	// Movement stick... left [joy_movement_stick]
	// Automatically crouch... no [joy_autowalk]
	// Automatically shoot when aiming... no [joy_autoattack]
	// Automatically aim to movement direction... yes [joy_aim_to_movement]
	// Stick up in first person looks... down [joy_inverty]
	// Use controller icon set... auto [rd_force_controller_glyph_set]

	// Actions
	m_pBindPrimaryAttack = new CRD_VGUI_Bind( this, "BindPrimaryAttack", "#Valve_Primary_Attack", "+attack", false );
	m_pBindSecondaryAttack = new CRD_VGUI_Bind( this, "BindSecondaryAttack", "#Valve_Secondary_Attack", "+attack2", false );
	m_pBindUse = new CRD_VGUI_Bind( this, "BindUse", "#asw_use", "+use", false );
	m_pBindSelectPrimary = new CRD_VGUI_Bind( this, "BindSelectPrimary", "#nb_select_weapon_one", "ASW_SelectPrimary", false );
	m_pBindDropWeapon = new CRD_VGUI_Bind( this, "BindDropWeapon", "#asw_drop", "ASW_Drop", false );
	m_pBindSwapWeapons = new CRD_VGUI_Bind( this, "BindSwapWeapons", "#asw_switch_primary_secondary", "ASW_InvLast", false );
	m_pBindSwapWeapons->AddFallbackBind( "ASW_InvNext" );
	m_pBindSwapWeapons->AddFallbackBind( "ASW_InvPrev" );
	m_pBindMeleeAttack = new CRD_VGUI_Bind( this, "BindMeleeAttack", "#asw_melee_attack", "+alt1", false );
	m_pBindReload = new CRD_VGUI_Bind( this, "BindReload", "#Valve_Reload_Weapon", "+reload", false );
	m_pBindSelectSecondary = new CRD_VGUI_Bind( this, "BindSelectSecondary", "#nb_select_weapon_two", "ASW_SelectSecondary", false );
	m_pBindDropEquipment = new CRD_VGUI_Bind( this, "BindDropEquipment", "#rd_str_drop_extra", "ASW_DropExtra", false );

	// Social / Extras
	m_pBindTextChat = new CRD_VGUI_Bind( this, "BindTextChat", "#Valve_Chat_Message", "say", true );
	m_pBindVoiceChat = new CRD_VGUI_Bind( this, "BindVoiceChat", "#Valve_Use_Voice_Communication", "+voicerecord", true );
	m_pBindWheelDefault = new CRD_VGUI_Bind( this, "BindWheelDefault", "#rd_wheel_default", "+mouse_menu", true );
	m_pBindEmoteGo = new CRD_VGUI_Bind( this, "BindEmoteGo", "#asw_order_marines_follow", "asw_orderMarinesFollow", true );
	m_pBindEmoteStop = new CRD_VGUI_Bind( this, "BindEmoteStop", "#asw_order_marines_hold", "asw_orderMarinesHold", true );
	m_pBindMarinePosition = new CRD_VGUI_Bind( this, "BindMarinePosition", "#rd_order_marine_hold", "+holdorder", true );
	m_pBindEmoteMedic = new CRD_VGUI_Bind( this, "BindEmoteMedic", "#asw_emote_medic", "cl_emote 0", true );
	m_pBindEmoteAmmo = new CRD_VGUI_Bind( this, "BindEmoteAmmo", "#asw_emote_ammo", "cl_emote 1", true );
	m_pBindEmoteQuestion = new CRD_VGUI_Bind( this, "BindEmoteQuestion", "#asw_emote_question", "cl_emote 7", true );
	m_pBindEmoteExclaim = new CRD_VGUI_Bind( this, "BindEmoteExclaim", "#asw_emote_watch_out", "cl_emote 5", true );
	m_pBindVoteYes = new CRD_VGUI_Bind( this, "BindVoteYes", "#asw_vote_yes_key", "vote_yes", true );
	m_pBindVoteNo = new CRD_VGUI_Bind( this, "BindVoteNo", "#asw_vote_no_key", "vote_no", true );
	m_pBindMissionOverview = new CRD_VGUI_Bind( this, "BindMissionOverview", "#asw_ingame_map", "ingamebriefing", true );
	m_pBindPlayerList = new CRD_VGUI_Bind( this, "BindPlayerList", "#asw_playerlist", "playerlist", true );
	m_pBindRotateCameraLeft = new CRD_VGUI_Bind( this, "BindRotateCameraLeft", "#rd_str_rotatecameraleft", "rotatecameraleft", true );
	m_pBindRotateCameraRight = new CRD_VGUI_Bind( this, "BindRotateCameraRight", "#rd_str_rotatecameraright", "rotatecameraright", true );
	m_pBindSecondaryAttackAlt = new CRD_VGUI_Bind( this, "BindSecondaryAttackAlt", "#Valve_Secondary_Attack", "+secondary", true );
	m_pBindChooseMarine = new CRD_VGUI_Bind( this, "BindChooseMarine", "#rd_str_selectloadout", "cl_select_loadout", true );

	// Use Equipment
	m_pBindActivatePrimary = new CRD_VGUI_Bind( this, "BindActivatePrimary", "#rd_bind_ActivatePrimary", "ASW_ActivatePrimary", true );
	m_pBindActivateSecondary = new CRD_VGUI_Bind( this, "BindActivateSecondary", "#rd_bind_ActivateSecondary", "ASW_ActivateSecondary", true );
	m_pBindActivateEquipment[0] = new CRD_VGUI_Bind( this, "BindActivateEquipment0", "#asw_activate_extra", "+grenade1", true );
	for ( int i = 1; i < NELEMS( m_pBindActivateEquipment ); i++ )
	{
		m_pBindActivateEquipment[i] = new CRD_VGUI_Bind( this, CFmtStr{ "BindActivateEquipment%d", i }, CFmtStr{ "#asw_order_marine_%d_equip", i + 1 }, CFmtStr{ "asw_squad_hotbar %d", i }, true );
	}
	m_pBindWheelEquipment = new CRD_VGUI_Bind( this, "BindWheelEquipment", "#rd_wheel_equipment", "+mouse_menu ASW_UseExtra", true );
	m_pBindWheelEquipment1 = new CRD_VGUI_Bind( this, "BindWheelEquipment1", "#rd_wheel_equipment1", "+mouse_menu ASW_UseExtra1", true );
	m_pBindWheelEquipment2 = new CRD_VGUI_Bind( this, "BindWheelEquipment2", "#rd_wheel_equipment2", "+mouse_menu ASW_UseExtra2", true );

	// Select Marine
	for ( int i = 0; i < NELEMS( m_pBindSelectMarine ); i++ )
	{
		m_pBindSelectMarine[i] = new CRD_VGUI_Bind( this, CFmtStr{ "BindSelectMarine%d", i }, CFmtStr{ "#asw_select_marine_%d", i + 1 }, CFmtStr{ "+selectmarine%d", i + 1 }, true );
	}
	m_pBindWheelMarine = new CRD_VGUI_Bind( this, "BindWheelMarine", "#rd_wheel_marine", "+mouse_menu ASW_SelectMarine", true );

	m_pBtnCustomWheels = new BaseModUI::BaseModHybridButton( this, "BtnCustomWheels", "#rd_manage_custom_chat_wheels", this, "ManageWheels" );
	m_pBtnResetDefaults = new BaseModUI::BaseModHybridButton( this, "BtnResetDefaults", "#L4D360UI_Controller_Default", this, "ResetDefaults" );
	m_pChkDeveloperConsole = new CCvarToggleCheckButton( this, "ChkDeveloperConsole", "#GameUI_DeveloperConsoleCheck", "con_enable" );
}