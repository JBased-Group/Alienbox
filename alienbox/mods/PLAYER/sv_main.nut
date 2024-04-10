/*
class CSquirrelCombatWeapon extends CBaseEntity
{
	function Spawn()
	{
		Precache();
		base.SetSolid(2);
		base.RemoveEFlags(262144);
		GiveDefaultAmmo();
		base.SetModel(GetWorldModel());
		FallInit();
		base.SetCollisionGroup(11);
		base.CollisionProp().UseTriggerBounds(true,36);
	}
	
	function FallInit()
	{
		base.VPhysicsDestroyObject();
		if(!base.VPhysicsInitNormal(2, base.GetSolidFlags() | 8, false))
		{
			base.SetMoveType(5);
			base.SetSolid(2);
			base.AddSolidFlags(8);
		}
		
		SetPickupTouch();
		base.ThinkSet(FallThink,0,0);
		base.SetNextThink(0.1);
	}
	
	function FallThink()
	{
		
	}
	
	function GetWorldModel()
	{
		return "models/weapons/w_portalgun.mdl"
	}
	
	function Precache()
	{
	
	}
	
	function Equip(pOwner)
	{
	
	}
	
	function Drop(vecVelocity)
	{
	
	}
	
	function CanBeSelected()
	{
	
	}
	
	function IsAllowedToSwitch()
	{
	
	}
	
	function VisibleInWeaponSelection()
	{
	
	}
	
	function HasAmmo()
	{
	
	}
	
	function SetPickupTouch()
	{
		base.TouchSet(DefaultTouch);
	}
	
	function DefaultTouch(pOther)
	{
	
	}

	function ItemPreFrame()
	{
	
	}
	
	function ItemPostFrame()
	{
	
	}
	
	function ItemBusyFrame()
	{
	
	}
	
	function ItemHolsterFrame()
	{
	
	}
	
	function WeaponIdle()
	{
	
	}
	
	function HandleFireOnEmpty()
	{
	
	}
	
	function PrimaryAttack()
	{
	
	}
	
	function SecondaryAttack()
	{
	
	}
	
	function GetOWner()
	{
	
	}
	
	function SetOwner()
	{
	
	}
	
}
*/

class CSquirrelPlayer extends CBaseEntity
{
	function Spawn()
	{
		SharedSpawn();
	}
	
	function SharedSpawn()
	{
		local solid = ::CreateSolid();
		::PhysGetDefaultAABBSolid(solid);
		local vphys = ::PhysSphereCreate(this,16.0,::CreateVector(0.0,0.0,0.0),solid);
		base.VPhysicsSetObject(vphys);
		vphys.Wake();
		base.SetMoveType(6,0);
		base.SetSolid(6);
		base.AddSolidFlags(16);
	}
	
	function ProcessUsercmds(cmds,numcmds,totalcmds,dropped_packets,paused)
	{
		base.SetAbsAngles(cmds.Get_viewangles());
		local forward = ::CreateVector(0.0,0.0,0.0);
		local right = ::CreateVector(0.0,0.0,0.0);
		local up = ::CreateVector(0.0,0.0,0.0);
		
		base.GetVectors(forward,right,up);
		::VectorScale(forward,cmds.Get_forwardmove(),forward);
		::VectorScale(right,cmds.Get_sidemove(),right);
		::VectorAdd(forward,right,forward);
		
		//base.SetAbsVelocity(forward);
	}
}

LinkEntityToClass("player",CSquirrelPlayer);