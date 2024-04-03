class Hi extends CBaseEntity
{
	constructor()
	{
	
	}
}

LinkEntityToClass("my_squirrel_ent",Hi);

function OnLevelInit()
{
	local HELP = CreateEntityByName("my_squirrel_ent");
	HELP.SetFadeDistance(HELP.GetReceivedDamageScale(HELP),100.0);
	HELP.DrawBBoxOverlay(10.0);
	HELP.EntityText(0,"Hello from squirrel", 10.0, 255,255, 127, 255);
	HELP.DrawAbsBoxOverlay();
	
}