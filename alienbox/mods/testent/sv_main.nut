class ExplosiveThing extends CBaseEntity
{
	constructor()
	{
	   //...
	}
	
	function UpdateTransmitState()
	{
		return SetTransmitState(8);
	}
	</ sendprop = true />
	testsend = 23;
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