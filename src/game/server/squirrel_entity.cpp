#include "cbase.h"
#include "squirrel_entity.h"

extern ISquirrel* g_pSquirrel;

ServerClass* CSquirrelEntity::GetServerClass() 
{
	return m_serverClass;
}
int CSquirrelEntity::YouForgotToImplementOrDeclareServerClass() 
{
	return 0;
} 

datamap_t* CSquirrelEntity::GetDataDescMap(void)
{
	return m_dataMap;
} 

datamap_t* CSquirrelEntity::GetBaseMap() 
{	
	return &CBaseEntity::m_DataMap;
}


CSquirrelEntity::CSquirrelEntity(SquirrelObject sqobj, SquirrelScript sqscript)
{
	obj = sqobj;
	script = sqscript;
}

void CSquirrelEntity::Spawn()
{
	g_pSquirrel->CallObjectFunction(script, obj, "Spawn", "");
}

void CSquirrelEntity::Think()
{
	if (!ThinkFunc[0])
	{
		return;
	}
	if (!g_pSquirrel->PushObjFunc(script, obj, ThinkFunc))
	{
		return;
	}
	g_pSquirrel->Call(script, 1);
	g_pSquirrel->Pop(script);
}

void CSquirrelEntity::Touch(CBaseEntity* pOther)
{
	if (!TouchFunc[0])
	{
		return;
	}
	if (!g_pSquirrel->PushObjFunc(script, obj, TouchFunc))
	{
		return;
	}
	CSquirrelEntity* sqother = dynamic_cast<CSquirrelEntity*>(pOther);
	if (sqother)
	{
		g_pSquirrel->PushObject(script,sqother->obj);
	}
	else
	{
		ConvertFromCpp(script, pOther);
	}
	g_pSquirrel->Call(script, 2);
	g_pSquirrel->Pop(script);
}

int CSquirrelEntity::UpdateTransmitState()
{
	SquirrelValue val = g_pSquirrel->CallObjectFunction(script, obj, "UpdateTransmitState", "");
	if (val.type == SQUIRREL_INT)
	{
		return val.val_int;
	}
	return 8;
}



