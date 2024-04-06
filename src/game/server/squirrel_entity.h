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
	DECLARE_SERVERCLASS();
	DECLARE_DATADESC();

	CSquirrelEntity(SquirrelObject sqobj, SquirrelScript sqscript);
	virtual void Think();
	virtual int UpdateTransmitState();
	SquirrelObject obj;
	SquirrelScript script;
	datamap_t* m_dataMap;
	ServerClass* m_serverClass;
};

#endif

