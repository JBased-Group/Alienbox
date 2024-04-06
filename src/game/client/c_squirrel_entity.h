#pragma once
#include "squirrel/squirrel.h"
#include "c_baseentity.h"

class C_SquirrelEntity : public C_BaseEntity
{
public:
	DECLARE_CLASS(C_SquirrelEntity, C_BaseEntity);
	DECLARE_CLIENTCLASS();

	C_SquirrelEntity();
	~C_SquirrelEntity();

	SquirrelObject cls;
	SquirrelScript script;
	ClientClass* clientclass;
};

