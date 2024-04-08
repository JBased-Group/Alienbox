class ExplosiveThing extends C_BaseEntity
{
	constructor()
	{
	   //...
	}
	
	function Spawn()
	{
		
	}
	
	function DrawModel(flags, instance)
	{
		local info = ClientModelRenderInfo_t();
		info.Setflags(flags);
		info.SetpRenderable(base.ToIClientRenderable());
		info.Setinstance(base.GetModelInstance());
		info.Setindex(base.Getindex());
		info.SetpModel(base.GetModel());
		info.Setorigin(base.GetAbsOrigin());
		info.Setangles(base.GetAbsAngles());
		info.Setskin(0);
		info.Setbody(0);
		info.Sethitboxset(0);
		local state = DrawModelState_t();
		GetModelRender().DrawModelSetup(info,state,0);
		GetModelRender().DrawModelExecute(state,info,0);
	}
	
	</ recvprop = true />
	testsend = 23;
}

DeclareClientClass(ExplosiveThing);