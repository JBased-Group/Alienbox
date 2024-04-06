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

