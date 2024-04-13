class CSquirrelPlayer extends C_BaseEntity
{
	function CalcView(pos,angles,znear,zFar,fov)
	{
		pos.Set(base.GetAbsOrigin());
		angles.Set(::CreateVector(x,y,0.0));
	}
	
	</ recvprop = true />
	x = 0.0;
	
	</ recvprop = true />
	y = 0.0;
	
}

DeclareClientClass(CSquirrelPlayer);

class CSquirrelCombatWeapon extends C_BaseEntity
{
	function SetupBones(pBoneToWorldOut)
	{
		pBoneToWorldOut.Set(base.RenderableToWorldTransform());
		
		return true;
	}
}

DeclareClientClass(CSquirrelCombatWeapon);