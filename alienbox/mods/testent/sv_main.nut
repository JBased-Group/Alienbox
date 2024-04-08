class ExplosiveThing extends CBaseEntity
{
	constructor()
	{
	   //...
	}
	
	function Spawn()
	{
		base.PrecacheModel("models/props_c17/furniturecouch001a.mdl");
		::UTIL_SetModel(this,"models/props_c17/furniturecouch001a.mdl");
	}
	
	function UpdateTransmitState()
	{
		return base.SetTransmitState(8);
	}

	
	</ datadesc = true, externalname = "explosion_power", flags = 6 />
	explosionpwr = 10;
	
	</ datadesc = true, externalname = "epicstring", flags = 6 />
	epicstring = "HELLO";
	
	</ sendprop = true />
	testsend = 23;
}

LinkEntityToClass("my_squirrel_ent",ExplosiveThing);

/*
function OnGameFrame()
{
	local A = EntityByIndex(1);
	local B = EntityByIndex(19);
	if(A != null && B != null && A.Intersects(B))
	{
		A.VelocityPunch(CreateVector(0.0,0.0,1000.0));
	}
}
*/