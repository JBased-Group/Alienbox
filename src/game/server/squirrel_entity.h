#ifndef SQUIRREL_ENTITY_H
#define SQUIRREL_ENTITY_H
#ifdef _WIN32
#pragma once
#endif

#include "baseentity.h"
#include "squirrel/squirrel.h"

#define PushArg(a) \
if constexpr (count >= a) \
{ \
	if constexpr (types[a] == 'i') \
	{ \
		g_pSquirrel->PushInt(script, va_arg(lol,int)); \
	} \
	else if constexpr (types[a] == 'f') \
	{ \
		g_pSquirrel->PushFloat(script, va_arg(lol,float)); \
	} \
	else if constexpr (types[a] == 's') \
	{ \
		g_pSquirrel->PushString(script, va_arg(lol,const char*)); \
	} \
	else if constexpr (types[a] == 'b') \
	{ \
		g_pSquirrel->PushInt(script, (int)va_arg(lol,bool)); \
	} \
	else if constexpr (types[a] == 'p') \
	{ \
		if(!((GenericConverterFromCpp)(((void* (*)())types.ConvertCpp[a])()))(script,va_arg(lol,void*))) \
		{ \
			if constexpr (Same<ReturnType, void>) \
			{ \
				return;\
			} \
			else \
			{ \
				return 0; \
			} \
		} \
	} \
}

class CSquirrelEntity : public CBaseEntity
{
public:
	DECLARE_CLASS(CSquirrelEntity, CBaseEntity);
	DECLARE_SERVERCLASS();
	DECLARE_DATADESC();


	CSquirrelEntity(SquirrelObject sqobj, SquirrelScript sqscript);
	virtual void Spawn();
	virtual void Think();
	virtual int UpdateTransmitState();

	template <typename ReturnType, typename... ArgumentTypes>
	static constexpr ReturnableString<sizeof...(ArgumentTypes) + 2> GetSig()
	{
		ReturnableString<sizeof...(ArgumentTypes) + 2> types{};
		types = types + GetType<ReturnType>();
		if constexpr (IsPointer<ReturnType>)
		{
			types.ConvertCpp[0] = (const void*)TypeIdentifier<ReturnType>::ConvertToCpp;
		}
		else
		{
			types.ConvertCpp[0] = (void*)TypeIdentifier<int>::ConvertToCpp;
		}
		((types = types + GetType<ArgumentTypes>()), ...);
		((types = types + (void*)TypeIdentifier<ArgumentTypes>::ConvertFromCpp), ...);
		return types;
	}

	template <typename ReturnType, typename... ArgumentTypes>
	ReturnType CallFunction(const char* funcname, ArgumentTypes...)
	{
		extern ISquirrel* g_pSquirrel;
		constexpr ReturnableString<sizeof...(ArgumentTypes) + 2> types = GetSig<ReturnType, ArgumentTypes...>();
		constexpr int count = sizeof...(ArgumentTypes) + 1;
		if (!g_pSquirrel->PushObjFunc(script, obj, funcname))
		{
			if constexpr (Same<ReturnType, void>)
			{
				return;
			}
			else
			{
				return 0;
			}
		}
		va_list lol;
		va_start(lol, funcname);
		PushArg(1);
		PushArg(2);
		PushArg(3);
		PushArg(4);
		PushArg(5);
		PushArg(6);
		PushArg(7);
		PushArg(8);
		PushArg(9);
		PushArg(10);
		PushArg(11);
		PushArg(12);
		g_pSquirrel->Call(script, count);
		g_pSquirrel->Pop(script);
		if constexpr (Same<ReturnType, void>)
		{
			//g_pSquirrel->Pop(script);
			return;
		}

		FourByteValue argi;

		if constexpr (types[0] == 'i')
		{
			if (!g_pSquirrel->GetStackInt(script, -1, &argi.v_i))
				return argi.v_i;
		}
		else if constexpr (types[0] == 'f')
		{
			if (!g_pSquirrel->GetStackFloat(script, -1, &argi.v_f))
				return argi.v_f;
		}
		else if constexpr (types[0] == 's')
		{
			if (!g_pSquirrel->GetStackString(script, -1, &argi.v_s))
				return argi.v_s;
		}
		else if constexpr (types[0] == 'p')
		{
			if (!((GenericConverterToCpp)(((void* (*)())types.ConvertCpp[0])()))(script, &argi.v_p, -1))
				return (ReturnType)argi.v_p;
		}

	}


	SquirrelObject obj;
	SquirrelScript script;
	datamap_t* m_dataMap;
	ServerClass* m_serverClass;
};

inline CSquirrelEntity* ToSquirrelPlayer(CBaseEntity* player)
{
	return static_cast<CSquirrelEntity*>(player);
}

#endif

