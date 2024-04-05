#include "cbase.h"
#include "squirrel_entity.h"

extern ISquirrel* g_pSquirrel;

//IMPLEMENT_SERVERCLASS_ST(CSquirrelEntity,DT_SquirrelEntity)
//END_SEND_TABLE()


datamap_t* CSquirrelEntity::GetDataDescMap(void)
{
	return m_dataMap;
} 
datamap_t* CSquirrelEntity::GetBaseMap() 
{	
	return &CBaseEntity::m_DataMap; // TODO : Change this to the parent squirrel class ent
}

CSquirrelEntity::CSquirrelEntity(SquirrelObject sqobj, SquirrelScript sqscript)
{
	obj = sqobj;
	script = sqscript;
}

void CSquirrelEntity::Think()
{
	BaseClass::Think();
	g_pSquirrel->CallObjectFunction(script, obj, "Think", "");
}

