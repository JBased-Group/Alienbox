#include "cbase.h"
#include "rd_vgui_player_reporting.h"
#include "rd_player_reporting.h"
#include "rd_vgui_settings.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

// TODO: player reporting menu (that is, reports from players; not necessarily reports about players)
// goals:
// - make it clear what will happen when the submit button is clicked (the report message, an optional attached screenshot, the selections from dropdown menus, list of witness SteamIDs, and some performance debugging data will be sent to a Reactive Drop Team controlled server over the internet, where it can be seen by the devs.)
// - make it clear what will NOT happen when the submit button is clicked (no other action will be automatically taken)
// - implement button that takes a screenshot of what the game looks like without this dialog up and attaches it as a jpeg.
// - implement file selector for arbitrary jpegs.
// - decide on categories for reports (targets: game, server, player, self).
// - remember recent players on the last connected server so reports don't get prevented by a player exiting immediately after something worthy of a report or if a kick on the current player is report-worthy.
// - add a method to access this from InGameMainMenu and one from out-of-gameplay.

struct PlayerReportCategory_t
{
	const char *APIName;
	const char *DisplayName;
	const char *Hint;
	bool RequiresServer : 1;
	bool PlayerTarget : 1;
};

static const PlayerReportCategory_t s_ReportCategories[] =
{
	{ "my_account", "#rd_reporting_category_my_account", "#rd_reporting_category_my_account_desc", false, false },
	{ "player_cheating", "#rd_reporting_category_player_cheating", "#rd_reporting_category_player_cheating_desc", true, true },
	{ "player_abusive_gameplay", "#rd_reporting_category_player_abusive_gameplay", "#rd_reporting_category_player_abusive_gameplay_desc", true, true },
	{ "player_abusive_communication", "#rd_reporting_category_player_abusive_communication", "#rd_reporting_category_player_abusive_communication_desc", true, true },
	{ "server_technical", "#rd_reporting_category_server_technical", "#rd_reporting_category_server_technical_desc", true, false },
	{ "server_abuse", "#rd_reporting_category_server_abuse", "#rd_reporting_category_server_abuse_desc", true, false },
	{ "server_other", "#rd_reporting_category_server_other", "#rd_reporting_category_server_other_desc", true, false },
	{ "game_bug", "#rd_reporting_category_game_bug", "#rd_reporting_category_game_bug_desc", false, false },
	{ "other", "#rd_reporting_category_other", "#rd_reporting_category_other_desc", false, false },
};

CRD_VGUI_Player_Reporting::CRD_VGUI_Player_Reporting( vgui::Panel *parent, const char *panelName ) :
	BaseClass{ parent, panelName }
{
}