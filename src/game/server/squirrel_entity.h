#ifndef SQUIRREL_ENTITY_H
#define SQUIRREL_ENTITY_H
#ifdef _WIN32
#pragma once
#endif

#include "baseentity.h"
#include "squirrel/squirrel.h"
class CSquirrelEntity : public CBaseEntity
{
public:
	DECLARE_CLASS(CSquirrelEntity, CBaseEntity);
	//DECLARE_SERVERCLASS();
	CSquirrelEntity(SquirrelObject sqobj, SquirrelScript sqscript);
	virtual void Think();
	SquirrelObject obj;
	SquirrelScript script;
};

#endif

