#include "cbase.h"
#include "squirrel_entity.h"

//IMPLEMENT_SERVERCLASS_ST(CSquirrelEntity,DT_SquirrelEntity)
//END_SEND_TABLE()

CSquirrelEntity::CSquirrelEntity(SquirrelObject sqobj)
{
	obj = sqobj;
}