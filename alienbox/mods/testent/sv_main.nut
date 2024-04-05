class ExplosiveThing extends CBaseEntity
{
	constructor()
	{
	   //...
	}
	
	
	</ datadesc = true, externalname = "explosion_power", flags = 6 />
	explosionpwr = 10;
}

LinkEntityToClass("my_squirrel_ent",ExplosiveThing);


function OnGameFrame()
{
	local A = EntityByIndex(1);
	local B = EntityByIndex(19);
	if(A != null && B != null && A.Intersects(B))
	{
		A.VelocityPunch(CreateVector(0.0,0.0,1000.0));
	}
}