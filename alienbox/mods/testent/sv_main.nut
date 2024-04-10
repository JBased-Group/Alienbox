class EpicCouch extends CBaseEntity
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
	
	</ datadesc = true, externalname = "someint", flags = 6 />
	yeahint = 10;
	
	</ datadesc = true, externalname = "epicstring", flags = 6 />
	epicstring = "HELLO";
	
	</ sendprop = true />
	testsend = 23;
}

LinkEntityToClass("my_squirrel_ent",EpicCouch);
