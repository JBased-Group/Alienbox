#include "cbase.h"
#include "c_squirrel_entity.h"

int C_SquirrelEntity::YouForgotToImplementOrDeclareClientClass() 
{
    return 0;
} 
ClientClass* C_SquirrelEntity::GetClientClass() 
{
    return clientclass;
} 

C_SquirrelEntity::C_SquirrelEntity() : BaseClass()
{
}

C_SquirrelEntity::~C_SquirrelEntity()
{
}
