class EpicCouch extends C_BaseEntity
{
	constructor()
	{
	   //...
	}
	
	function Spawn()
	{
		
	}
	
	function GetRenderData(pData)
	{
		//pData.Set(6);
		return true;
	}
	
	function SetupBones(pBoneToWorldOut)
	{
		local toshear = base.RenderableToWorldTransform();
		pBoneToWorldOut.Set(toshear);
		
		::MatrixShear(pBoneToWorldOut,::CreateVector(1.0,0.0,0.0));
		
		return true;
	}
	
	</ recvprop = true />
	testsend = 23;
}

DeclareClientClass(EpicCouch);