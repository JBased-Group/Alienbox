class CSquirrelCombatWeapon extends CBaseEntity
{
	function Spawn()
	{
		Precache();
		base.SetSolid(2);
		base.RemoveEFlags(262144);
		GiveDefaultAmmo();
		base.PrecacheModel(GetWorldModel());
		::UTIL_SetModel(this,GetWorldModel());
		FallInit();
		base.SetCollisionGroup(11);
		base.CollisionProp().UseTriggerBounds(true,36.0);
	}
	
	function GiveDefaultAmmo()
	{
	
	}
	
	function FallInit()
	{
		local solid = ::CreateSolid();
		::PhysGetDefaultAABBSolid(solid);
		if(!base.VPhysicsInitNormal(6, base.GetSolidFlags() | 8, false,solid))
		{
			base.SetMoveType(5);
			base.SetSolid(2);
			base.AddSolidFlags(8);
		}
		else
		{
			base.SetMoveType(6,0);
			base.SetSolid(6);
			base.AddSolidFlags(8);
		}
		
		SetPickupTouch();
		base.SetThinkFunc("FallThink");
		base.SetNextThink(0.1);
	}
	
	function FallThink()
	{
		
	}
	
	function GetWorldModel()
	{
		return "models/weapons/plasmalauncher/plasmalauncher.mdl";
	}
	
	function GetViewModel()
	{
		return "models/weapons/v_portalgun.mdl";
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
		base.SetTouchFunc("DefaultTouch");
	}
	
	function DefaultTouch(pOther)
	{
		if(pOther.BumpWeapon)
		{
			pOther.BumpWeapon(this);
		}
	}

	function Equip(owner)
	{
		base.RemoveSolidFlags(8);
		base.FollowEntity(owner,true);
		SetOwner(owner);
		base.SetOwnerEntity(owner);
		base.RemoveThinkFunc();
		base.RemoveTouchFunc();
		base.VPhysicsDestroyObject();
		
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
	
	function GetOwner()
	{
		return m_hOwner
	}
	
	function SetOwner(owner)
	{
		m_hOwner = owner
	}
	
	m_hOwner = null;
	
}

LinkEntityToClass("weapon_base",CSquirrelCombatWeapon);

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
		vphys.EnableGravity(0);
		base.SetMoveType(6,0);
		base.SetSolid(6);
		base.AddSolidFlags(16);
		base.SetCollisionGroup(5);
	}
	
	function ProcessUsercmds(cmds,numcmds,totalcmds,dropped_packets,paused)
	{
		base.SetAbsAngles(cmds.Get_viewangles());
		local va = ::QAngleVector(cmds.Get_viewangles());
		x = ::VectorX(va);
		y = ::VectorY(va);
		local forward = ::CreateVector(0.0,0.0,0.0);
		local right = ::CreateVector(0.0,0.0,0.0);
		local up = ::CreateVector(0.0,0.0,0.0);
		
		base.GetVectors(forward,right,up);
		::VectorScale(forward,cmds.Get_forwardmove()*10.0,forward);
		::VectorScale(right,cmds.Get_sidemove()*10.0,right);
		::VectorAdd(forward,right,forward);
		//::VectorAdd(forward,::CreateVector(0.0,0.0,600.0),forward);
		
		local vphys = base.VPhysicsGetObject();
		vphys.ApplyForceCenter(forward);
		//base.SetAbsVelocity(forward);
	}
	
	function BumpWeapon(pWeapon)
	{
		pWeapon.AddSolidFlags(4);
		Weapon_Equip(pWeapon);
	}
	
	function Weapon_Equip(pWeapon)
	{
		m_hMyWeapons.append(pWeapon);
		pWeapon.Equip(this);
	}
	
	</ sendprop = true />
	x = 0.0;
	
	</ sendprop = true />
	y = 0.0;
	
	m_hMyWeapons = [];
}

LinkEntityToClass("player",CSquirrelPlayer);