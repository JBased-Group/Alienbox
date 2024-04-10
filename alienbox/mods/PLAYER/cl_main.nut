class CSquirrelPlayer extends C_BaseEntity
{
	function CalcView(pos,angles,znear,zFar,fov)
	{
		pos.Set(base.GetAbsOrigin());
		angles.Set(base.GetAbsAngles());
	}
}

DeclareClientClass(CSquirrelPlayer);