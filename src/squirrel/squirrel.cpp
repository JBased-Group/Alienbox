#include "squirrel.h"
#include "../public/squirrel/squirrel.h"

#include "tier1/interface.h"
#include <stdarg.h>
#include "sqstdstring.h"
#include "sqstdmath.h"
#include "utlvector.h"
void print_args(HSQUIRRELVM v);
class CSquirrel : public ISquirrel
{
public:
	virtual SquirrelScript LoadScript(const char* script, SquirrelFunctionDecl* initfuncs, SquirrelRegisterFunction sqregfunc);
	virtual SquirrelValue CallFunction(SquirrelScript script, const char* fun, const char* types, ...);
	virtual void ShutdownScript(SquirrelScript script);
	virtual void AddFunction(SquirrelScript script, const char* name, SquirrelFunction fun);
	virtual bool GetArgs(SquirrelScript script, const char* types, ...);
	virtual void PushValue(SquirrelScript script, SquirrelValue val);
	virtual void PushArray(SquirrelScript script);
	virtual void AppendToArray(SquirrelScript script);
	virtual SquirrelValue InstantiateClass(SquirrelScript script, SquirrelObject cls);
	virtual SquirrelValue CallObjectFunction(SquirrelScript script, SquirrelObject obj, const char* fun, const char* types, ...);
	virtual void IncrementRefCount(SquirrelScript script, SquirrelObject obj);
	virtual void AddClassFunction(SquirrelScript script, SquirrelObject cls, const char* name, SquirrelFunction fun);
	virtual bool GetSQObject(SquirrelScript script, SquirrelObject& obj);
	virtual SquirrelObject CreateClass(SquirrelScript script);
	virtual void SetObjectUserdata(SquirrelScript script, SquirrelObject obj, void* ptr, const int* typetag);
	virtual void* GetObjectUserdata(SquirrelScript script, SquirrelObject obj);
	virtual bool GetStackInt(SquirrelScript script, int i, int* val);
	virtual bool GetStackFloat(SquirrelScript script, int i, float* val);
	virtual bool GetStackString(SquirrelScript script, int i, const char** val);
	virtual bool GetStackPtr(SquirrelScript script, int i, void** val, const int* typetag);
	virtual bool GetStackUserData(SquirrelScript script, int i, void** val, const int* typetag);
	virtual bool GetStackUserPtr(SquirrelScript script, int i, void** val);
	virtual void PushInt(SquirrelScript script, int val);
	virtual void PushFloat(SquirrelScript script, float val);
	virtual void PushString(SquirrelScript script, const char* val);
	virtual void PushObject(SquirrelScript script, SquirrelObject obj);
	virtual SquirrelObject PushPtr(SquirrelScript script, void* val, const int* typetag);
	virtual bool GetStackObjectUserdata(SquirrelScript script, void** ptr);
	virtual void RegisterClasses(SquirrelScript script, SquirrelClassDecl* classes);
	virtual bool SetObjectVariant(SquirrelScript script, SquirrelObject obj, const char* name, variant_t* var, fieldtype_t ftype);
	virtual bool GetObjectVariant(SquirrelScript script, SquirrelObject obj, const char* name, variant_t* var);
	virtual datamap_t* GenerateDatamap(SquirrelScript script, SquirrelObject obj, datamap_t* basemap);
	virtual SendTable* GenerateSendtable(SquirrelScript script, SquirrelObject obj, SendTable* basemap, size_t objOffset, const char* name);
	virtual RecvTable* GenerateRecvtable(SquirrelScript script, SquirrelObject obj, RecvTable* basemap, size_t objOffset, const char* name);
	virtual void SetDelegate(SquirrelScript script, SquirrelObject obj);
	virtual void RegisterDelegates(SquirrelScript script, SquirrelDelegateDecl* classes);
	virtual void Pop(SquirrelScript script);
	virtual bool PushObjFunc(SquirrelScript script, SquirrelObject obj, const char* fun);
	virtual bool Call(SquirrelScript script, int count);
	virtual const char* GetName(SquirrelScript script, SquirrelObject obj);
	virtual bool PushObjectValue(SquirrelScript script, SquirrelObject obj);
};



void CSquirrel::SetObjectUserdata(SquirrelScript script, SquirrelObject obj, void* ptr, const int* typetag)
{
	HSQUIRRELVM v = (HSQUIRRELVM)script;
	HSQOBJECT inst = *(HSQOBJECT*)&obj;
	sq_pushobject(v, inst);
	sq_setinstanceup(v, -1, ptr);
	sq_settypetag(v, -1, (void*)typetag);
	sq_pop(v, 1);
}

void* CSquirrel::GetObjectUserdata(SquirrelScript script, SquirrelObject obj)
{
	HSQUIRRELVM v = (HSQUIRRELVM)script;
	HSQOBJECT inst = *(HSQOBJECT*)&obj;
	sq_pushobject(v, inst);
	void* ptr;
	sq_getinstanceup(v, -1, &ptr, 0, SQFalse);
	sq_pop(v, 1);
	return ptr;
}

bool CSquirrel::GetStackInt(SquirrelScript script, int i, int* val)
{
	return SQ_SUCCEEDED(sq_getinteger((HSQUIRRELVM)script, i, val));
}

bool CSquirrel::GetStackFloat(SquirrelScript script, int i, float* val)
{
	return SQ_SUCCEEDED(sq_getfloat((HSQUIRRELVM)script, i, val));
}

bool CSquirrel::GetStackString(SquirrelScript script, int i, const char** val)
{
	return SQ_SUCCEEDED(sq_getstring((HSQUIRRELVM)script, i, val));
}

bool CSquirrel::GetStackPtr(SquirrelScript script, int i, void** val, const int* typetag)
{
	return SQ_SUCCEEDED(sq_getinstanceup((HSQUIRRELVM)script, i, val, (void*)typetag, SQFalse));
}

bool CSquirrel::GetStackUserData(SquirrelScript script, int i, void** val, const int* typetag)
{
	const int* thetag;
	bool succ = SQ_SUCCEEDED(sq_getuserdata((HSQUIRRELVM)script, i, val, (void**)&thetag));
	if (!succ)
	{
		return false;
	}
	return thetag == typetag;
}

bool CSquirrel::GetStackUserPtr(SquirrelScript script, int i, void** val)
{
	return SQ_SUCCEEDED(sq_getuserpointer((HSQUIRRELVM)script, i, val));
}


void CSquirrel::PushInt(SquirrelScript script, int val)
{
	sq_pushinteger((HSQUIRRELVM)script, val);
}

void CSquirrel::PushFloat(SquirrelScript script, float val)
{
	sq_pushfloat((HSQUIRRELVM)script, val);
}

void CSquirrel::PushString(SquirrelScript script, const char* val)
{
	sq_pushstring((HSQUIRRELVM)script, val, -1);
}

void CSquirrel::PushObject(SquirrelScript script, SquirrelObject obj)
{
	sq_pushobject((HSQUIRRELVM)script, *(HSQOBJECT*)&obj);
}

SquirrelObject CSquirrel::PushPtr(SquirrelScript script, void* val, const int* typetag)
{
	*(void**)sq_newuserdata((HSQUIRRELVM)script, 4) = val;
	sq_settypetag((HSQUIRRELVM)script, -1, (void*)typetag);
	HSQOBJECT lol;
	sq_getstackobj((HSQUIRRELVM)script, -1, &lol);
	return *(SquirrelObject*)&lol;
}

void CSquirrel::SetDelegate(SquirrelScript script, SquirrelObject obj)
{
	sq_pushobject((HSQUIRRELVM)script, *(HSQOBJECT*)&obj);
	sq_setdelegate((HSQUIRRELVM)script, -2);
//	print_args((HSQUIRRELVM)script);
}

bool CSquirrel::GetStackObjectUserdata(SquirrelScript script, void** ptr)
{
	return SQ_SUCCEEDED(sq_getinstanceup((HSQUIRRELVM)script, 1, ptr, 0, SQFalse));
}


void CSquirrel::RegisterDelegates(SquirrelScript script, SquirrelDelegateDecl* delegates)
{
	HSQUIRRELVM v = (HSQUIRRELVM)script;
	while (delegates->funcs)
	{
		//sq_pushstring(v, delegates->name, -1);
		sq_newtable(v);
		sq_getstackobj(v, -1, (HSQOBJECT*)delegates->delegateobj);
		sq_addref(v, (HSQOBJECT*)delegates->delegateobj);
		while (delegates->funcs->ptr)
		{
			if (!delegates->funcs->name)
			{
				sq_pushnull(v);
			}
			else
			{
				sq_pushstring(v, delegates->funcs->name, -1);
			}
			sq_newclosure(v, (SQFUNCTION)delegates->funcs->ptr, 0);
			sq_newslot(v, -3, false);
			delegates->funcs++;
		}
		//sq_newslot(v, -3, false);
		
		delegates++;
	}
}

void CSquirrel::Pop(SquirrelScript script)
{
	sq_pop((HSQUIRRELVM)script, 1);
}

void CSquirrel::RegisterClasses(SquirrelScript script, SquirrelClassDecl* classes)
{
	HSQUIRRELVM v = (HSQUIRRELVM)script;
	while (classes->funcs)
	{
		sq_pushstring(v, classes->name, -1);
		sq_newclass(v, SQFalse);
		sq_getstackobj(v, -1, (HSQOBJECT*)classes->clsobj);
		while (classes->funcs->ptr)
		{
			if (!classes->funcs->name)
			{
				sq_pushnull(v);
			}
			else
			{
				sq_pushstring(v, classes->funcs->name, -1);
			}
			sq_newclosure(v, (SQFUNCTION)classes->funcs->ptr, 0);
			sq_newslot(v, -3, false);
			classes->funcs++;
		}
		sq_settypetag(v, -1, (void*)classes->typetag);
		sq_newslot(v, -3, false);
		classes++;
	}
}

bool CSquirrel::SetObjectVariant(SquirrelScript script, SquirrelObject obj, const char* name, variant_t* var, fieldtype_t ftype)
{
	HSQUIRRELVM v = (HSQUIRRELVM)script;
	HSQOBJECT inst = *(HSQOBJECT*)&obj;
	
	sq_pushobject(v, inst);
	sq_pushstring(v, name, -1);
	SQRESULT succ = sq_get(v, -2);
	switch (ftype)
	{
	case FIELD_INTEGER:
		sq_getinteger(v, -1, (int*)var);
		break;
	case FIELD_FLOAT:
		sq_getfloat(v, -1, (float*)var);
		break;
	case FIELD_BOOLEAN:
		sq_getbool(v, -1, (unsigned int*)var);
		break;
	case FIELD_STRING:
		sq_getstring(v, -1, (const char**)var);
		break;
	}
	sq_pop(v, 1);
	return SQ_SUCCEEDED(succ);
}


//-----------------------------------------------------------------------------
// Purpose: All types must be able to display as strings for debugging purposes.
// Output : Returns a pointer to the string that represents this value.
//
//			NOTE: The returned pointer should not be stored by the caller as
//				  subsequent calls to this function will overwrite the contents
//				  of the buffer!
//-----------------------------------------------------------------------------
const char* variant_t::ToString(void) const
{
	COMPILE_TIME_ASSERT(sizeof(string_t) == sizeof(int));

	static char szBuf[512];

	switch (fieldType)
	{
	case FIELD_STRING:
	{
		return(STRING(iszVal));
	}

	case FIELD_BOOLEAN:
	{
		if (bVal == 0)
		{
			Q_strncpy(szBuf, "false", sizeof(szBuf));
		}
		else
		{
			Q_strncpy(szBuf, "true", sizeof(szBuf));
		}
		return(szBuf);
	}

	case FIELD_INTEGER:
	{
		Q_snprintf(szBuf, sizeof(szBuf), "%i", iVal);
		return(szBuf);
	}

	case FIELD_FLOAT:
	{
		Q_snprintf(szBuf, sizeof(szBuf), "%g", flVal);
		return(szBuf);
	}

	case FIELD_COLOR32:
	{
		Q_snprintf(szBuf, sizeof(szBuf), "%d %d %d %d", (int)rgbaVal.r, (int)rgbaVal.g, (int)rgbaVal.b, (int)rgbaVal.a);
		return(szBuf);
	}

	case FIELD_VECTOR:
	{
		Q_snprintf(szBuf, sizeof(szBuf), "[%g %g %g]", (double)vecVal[0], (double)vecVal[1], (double)vecVal[2]);
		return(szBuf);
	}

	case FIELD_VOID:
	{
		szBuf[0] = '\0';
		return(szBuf);
	}
	}

	return("No conversion to string");
}

bool CSquirrel::GetObjectVariant(SquirrelScript script, SquirrelObject obj, const char* name, variant_t* var)
{
	HSQUIRRELVM v = (HSQUIRRELVM)script;
	HSQOBJECT inst = *(HSQOBJECT*)&obj;

	sq_pushobject(v, inst);
	sq_pushstring(v, name, -1);
	switch (var->FieldType())
	{
	case FIELD_INTEGER:
		sq_pushinteger(v, var->Int());
		break;
	case FIELD_FLOAT:
		sq_pushfloat(v, var->Float());
		break;
	case FIELD_BOOLEAN:
		sq_pushbool(v, var->Bool());
		break;
	case FIELD_STRING:
		sq_pushstring(v, var->String(), -1);
		break;
	}
	SQRESULT succ = sq_set(v, -3);
	sq_pop(v, 1);
	return SQ_SUCCEEDED(succ);
}

datamap_t* CSquirrel::GenerateDatamap(SquirrelScript script, SquirrelObject obj, datamap_t* basemap )
{
	HSQUIRRELVM v = (HSQUIRRELVM)script;
	HSQOBJECT inst = *(HSQOBJECT*)&obj;
	datamap_t* dm = new datamap_t();
	CUtlVector<typedescription_t> typedescs;
	dm->dataClassName = "CSquirrelEntity";
	dm->baseMap = basemap;
	sq_pushobject(v, inst);
	sq_pushnull(v);
	while (SQ_SUCCEEDED(sq_next(v, -2)))
	{
		typedescription_t hi = {};
		bool hasdatadesc = false;
		hi.flags = FTYPEDESC_SQUIRREL;
		switch (sq_gettype(v, -1))
		{
		case OT_FLOAT:
			hi.fieldType = FIELD_FLOAT;
			break;
		case OT_INTEGER:
			hi.fieldType = FIELD_INTEGER;
			break;
		case OT_STRING:
			hi.fieldType = FIELD_STRING;
			break;
		case OT_BOOL:
			hi.fieldType = FIELD_BOOLEAN;
			break;
		default:
			sq_pop(v, 2);
			continue;
		}
		sq_pop(v, 1); // pop value

		if (SQ_FAILED(sq_getattributes(v, -3)))
		{
			sq_pop(v, 1);
			continue;
		}
		if(sq_gettype(v,-1) != OT_TABLE)
		{
			sq_pop(v, 1);
			continue;
		}
		sq_pushnull(v);
		while (SQ_SUCCEEDED(sq_next(v, -2)))
		{
			const char* key;
			sq_getstring(v, -2, &key);
			if (!V_strcmp(key, "datadesc"))
			{
				hasdatadesc = true;
				sq_getstring(v, -1, &hi.fieldName);
				hi.fieldOffset = 0;
				hi.fieldSize = 1;
				hi.fieldSizeInBytes = 4;
			}
			else if (!V_strcmp(key, "externalname"))
			{
				sq_getstring(v, -1, &hi.externalName);
			}
			else if (!V_strcmp(key, "flags"))
			{
				int newflags = 0;
				sq_getinteger(v, -1, &newflags);
				hi.flags |= newflags;
			}
			sq_pop(v, 2);
		}
		sq_pop(v, 1); //pop null
		if(hasdatadesc)
		{
			typedescs.AddToTail(hi);
		}
	}
	dm->dataNumFields = typedescs.Count();
	dm->dataDesc = (typedescription_t*)MemAlloc_Alloc(sizeof(typedescription_t) * dm->dataNumFields);
	V_memcpy(dm->dataDesc, typedescs.Base(), sizeof(typedescription_t) * dm->dataNumFields);
	return dm;

}


void SendProxy_SquirrelToFloat(const SendProp* pProp, const void* pStruct, const void* pData, DVariant* pOut, int iElement, int objectID)
{
	HSQOBJECT inst = *((HSQOBJECT*)pData);
	HSQUIRRELVM v = *((HSQUIRRELVM*)((SquirrelObject*)pData+1));
	sq_pushobject(v, inst);
	sq_pushstring(v, (const char*)pProp->GetExtraData(), -1);
	sq_get(v, -2);
	sq_getfloat(v, -1, &pOut->m_Float);
	sq_pop(v, 1);
}

void SendProxy_SquirrelToInt(const SendProp* pProp, const void* pStruct, const void* pData, DVariant* pOut, int iElement, int objectID)
{
	HSQOBJECT inst = *((HSQOBJECT*)pData);
	HSQUIRRELVM v = *((HSQUIRRELVM*)((SquirrelObject*)pData + 1));
	sq_pushobject(v, inst);
	sq_pushstring(v, (const char*)pProp->GetExtraData(), -1);
	sq_get(v, -2);
	sq_getinteger(v, -1, (int*)&pOut->m_Int);
	sq_pop(v, 1);
}


SendTable* CSquirrel::GenerateSendtable(SquirrelScript script, SquirrelObject obj, SendTable* basemap, size_t objOffset, const char* name)
{
	HSQUIRRELVM v = (HSQUIRRELVM)script;
	HSQOBJECT inst = *(HSQOBJECT*)&obj;
	SendTable* st = new SendTable();
	CUtlVector<SendProp> sendprops;
	sendprops.AddToTail(SendPropDataTable((char*)"baseclass", 0, basemap, SendProxy_DataTableToDataTable));
	sq_pushobject(v, inst);
	sq_pushnull(v);
	while (SQ_SUCCEEDED(sq_next(v, -2)))
	{
		SQObjectType valtype = sq_gettype(v, -1);
		SendProp prop;
		sq_pop(v, 1);
		char* value;
		sq_getstring(v, -1, (const char**)&value);
		if (SQ_FAILED(sq_getattributes(v, -3)))
		{
			sq_pop(v, 1);
			continue;
		}
		if (sq_gettype(v, -1) != OT_TABLE)
		{
			sq_pop(v, 1);
			continue;
		}
		sq_pushnull(v);
		while (SQ_SUCCEEDED(sq_next(v, -2)))
		{
			const char* key;
			sq_getstring(v, -2, &key); // TODO : dont do this
			if (!V_strcmp(key, "sendprop"))
			{
				char* allocedkey;
				switch (valtype)
				{
				case OT_FLOAT:
					prop = SendPropFloat(value, objOffset,4,32,0,0.0f, HIGH_DEFAULT,SendProxy_SquirrelToFloat);
					allocedkey = (char*)MemAlloc_Alloc(strlen(value)+1);
					strcpy(allocedkey, value);
					prop.SetExtraData(allocedkey);
					break;
				case OT_INTEGER:
					prop = SendPropInt(value, objOffset,4,-1,0,SendProxy_SquirrelToInt);
					allocedkey = (char*)MemAlloc_Alloc(strlen(value) + 1);
					strcpy(allocedkey, value);
					prop.SetExtraData(allocedkey);
					break;
				default:
					continue;
				}
				sendprops.AddToTail(prop);
				sq_pop(v, 2);
				break;
			}
			sq_pop(v, 2);
		}
		sq_pop(v, 2);
	}
	SendProp* props = (SendProp*)MemAlloc_Alloc(sendprops.Count() * sizeof(SendProp));
	memcpy(props, sendprops.Base(), sendprops.Count() * sizeof(SendProp));
	st->Construct(props, sendprops.Count(), (char*)name);
	return st;
}



void RecvProxy_IntToSquirrel(const CRecvProxyData* pData, void* pStruct, void* pOut)
{
	HSQOBJECT inst = *((HSQOBJECT*)pOut);
	HSQUIRRELVM v = *((HSQUIRRELVM*)((SquirrelObject*)pOut + 1));
	sq_pushobject(v, inst);
	sq_pushstring(v, (const char*)pData->m_pRecvProp->GetExtraData(), -1);
	sq_pushinteger(v, (int)pData->m_Value.m_Int);
	sq_set(v, -3);
	sq_pop(v, 1);
}

void RecvProxy_FloatToSquirrel(const CRecvProxyData* pData, void* pStruct, void* pOut)
{
	HSQOBJECT inst = *((HSQOBJECT*)pOut);
	HSQUIRRELVM v = *((HSQUIRRELVM*)((SquirrelObject*)pOut + 1));
	sq_pushobject(v, inst);
	sq_pushstring(v, (const char*)pData->m_pRecvProp->GetExtraData(), -1);
	sq_pushfloat(v, pData->m_Value.m_Float);
	sq_set(v, -3);
	sq_pop(v, 1);
}

const char* CSquirrel::GetName(SquirrelScript script, SquirrelObject obj)
{
	HSQOBJECT inst = *(HSQOBJECT*)&obj;
	HSQUIRRELVM v = (HSQUIRRELVM)script;
	sq_pushroottable(v);
	sq_pushnull(v);
	while (SQ_SUCCEEDED(sq_next(v, -2)))
	{
		HSQOBJECT hi;
		sq_getstackobj(v, -1, &hi);
		if (hi._type == inst._type && hi._unVal.nInteger == inst._unVal.nInteger)
		{
			const char* out;
			sq_getstring(v, -2, &out);
			sq_pop(v, 4);
			return out;
		}
		sq_pop(v, 2);
	}
	sq_pop(v, 2);
	return 0;
}

bool CSquirrel::PushObjectValue(SquirrelScript script, SquirrelObject obj)
{
	if (sq_gettype((HSQUIRRELVM)script, -2) == OT_USERDATA)
	{
		sq_getdelegate((HSQUIRRELVM)script, -2);
		sq_remove((HSQUIRRELVM)script, -3);
		HSQOBJECT po;
		sq_getstackobj((HSQUIRRELVM)script, -2, &po);
		sq_pushobject((HSQUIRRELVM)script, po);
		sq_remove((HSQUIRRELVM)script, -3);
		return SQ_SUCCEEDED(sq_rawget((HSQUIRRELVM)script, -2));
	}
	return SQ_SUCCEEDED(sq_rawget((HSQUIRRELVM)script, -2));
}

RecvTable* CSquirrel::GenerateRecvtable(SquirrelScript script, SquirrelObject obj, RecvTable* basemap, size_t objOffset, const char* name)
{
	HSQUIRRELVM v = (HSQUIRRELVM)script;
	HSQOBJECT inst = *(HSQOBJECT*)&obj;
	RecvTable* rt = new RecvTable();
	CUtlVector<RecvProp> recvprops;
	recvprops.AddToTail(RecvPropDataTable((char*)"baseclass", 0, 0, basemap, DataTableRecvProxy_StaticDataTable));
	sq_pushobject(v, inst);
	sq_pushnull(v);
	while (SQ_SUCCEEDED(sq_next(v, -2)))
	{
		SQObjectType valtype = sq_gettype(v, -1);
		RecvProp prop;
		sq_pop(v, 1);
		char* value;
		sq_getstring(v, -1, (const char**)&value);
		if (SQ_FAILED(sq_getattributes(v, -3)))
		{
			sq_pop(v, 1);
			continue;
		}
		if (sq_gettype(v, -1) != OT_TABLE)
		{
			sq_pop(v, 1);
			continue;
		}
		sq_pushnull(v);
		while (SQ_SUCCEEDED(sq_next(v, -2)))
		{
			const char* key;
			sq_getstring(v, -2, &key); // TODO : dont do this
			if (!V_strcmp(key, "recvprop"))
			{
				
				char* allocedkey;
				
				switch (valtype)
				{
				case OT_FLOAT:
					prop = RecvPropFloat(value, objOffset, 4, 0, RecvProxy_FloatToSquirrel);
					allocedkey = (char*)MemAlloc_Alloc(strlen(value) + 1);
					strcpy(allocedkey, value);
					prop.SetExtraData(allocedkey);
					break;
				case OT_INTEGER:
					prop = RecvPropInt(value, objOffset, 4, 0, RecvProxy_IntToSquirrel);
					allocedkey = (char*)MemAlloc_Alloc(strlen(value) + 1);
					strcpy(allocedkey, value);
					prop.SetExtraData(allocedkey);
					break;
				default:
					continue;
				}
				recvprops.AddToTail(prop);
				sq_pop(v, 2);
				break;
			}
			sq_pop(v, 2);
		}
		sq_pop(v, 2);
	}
	RecvProp* props = (RecvProp*)MemAlloc_Alloc(recvprops.Count() * sizeof(RecvProp));
	memcpy(props, recvprops.Base(), recvprops.Count() * sizeof(RecvProp));
	rt->Construct(props, recvprops.Count(), (char*)name);
	return rt;
}


SquirrelValue CSquirrel::InstantiateClass(SquirrelScript script, SquirrelObject cls)
{
	HSQUIRRELVM v = (HSQUIRRELVM)script;
	HSQOBJECT obj = *(HSQOBJECT*)&cls;
	SquirrelValue ret;
	sq_pushobject(v, obj);
	sq_pushnull(v);
	if (SQ_FAILED(sq_call(v, 1, SQTrue, SQFalse)))
	{
		sq_pop(v, 2);
		sq_getlasterror(v);
		const char* err;
		sq_getstring(v, -1, &err);
		Msg("[ERROR" STRINGG(__LINE__) "] %s\n",err);

		ret.type = SQUIRREL_INVALID;
		return ret;
	}
	sq_getstackobj(v, -1, (HSQOBJECT*)&ret.val_obj);
	sq_addref(v, (HSQOBJECT*)&ret.val_obj);
	sq_pop(v, 1);
	ret.type = SQUIRREL_OBJECT;
	return ret;
}


void print_args(HSQUIRRELVM v)
{
	SQInteger nargs = sq_gettop(v); //number of arguments
	for (SQInteger n = 1; n <= nargs; n++)
	{
		Msg("arg %d is ", n);
		switch (sq_gettype(v, n))
		{
		case OT_NULL:
			Msg("null");
			break;
		case OT_INTEGER:
			Msg("integer");
			break;
		case OT_FLOAT:
			Msg("float");
			break;
		case OT_STRING:
			Msg("string");
			break;
		case OT_TABLE:
			Msg("table");
			break;
		case OT_ARRAY:
			Msg("array");
			break;
		case OT_USERDATA:
			Msg("userdata");
			break;
		case OT_CLOSURE:
			Msg("closure(function)");
			break;
		case OT_NATIVECLOSURE:
			Msg("native closure(C function)");
			break;
		case OT_GENERATOR:
			Msg("generator");
			break;
		case OT_USERPOINTER:
			Msg("userpointer");
			break;
		case OT_CLASS:
			Msg("class");
			break;
		case OT_INSTANCE:
			Msg("instance");
			break;
		case OT_WEAKREF:
			Msg("weak reference");
			break;
		default:
			break;
		}
		Msg("\n");
	}
}


bool CSquirrel::PushObjFunc(SquirrelScript script, SquirrelObject obj, const char* fun)
{
	HSQUIRRELVM v = (HSQUIRRELVM)script;
	HSQOBJECT object = *(HSQOBJECT*)&obj;
	sq_pushobject(v, object);
	sq_pushstring(v, fun, -1);
	sq_get(v, -2);
	
	if (sq_gettype(v, -1) != OT_CLOSURE)
	{
		sq_pop(v, 1);
		return false;
	}
	sq_remove(v, -2);
	sq_pushobject(v, object);
	return true;
}

bool CSquirrel::Call(SquirrelScript script, int count)
{
	HSQUIRRELVM v = (HSQUIRRELVM)script;
	//print_args(v);
	if (sq_call(v, count, 1, 0))
	{
		sq_getlasterror(v);
		const char* err;
		sq_getstring(v, -1, &err);
		Msg("%s\n", err);
		sq_pop(v, 1);
		return false;
	}
	return true;
}

SquirrelValue CSquirrel::CallObjectFunction(SquirrelScript script, SquirrelObject obj, const char* fun, const char* types, ...)
{
	SquirrelValue ret;
	ret.type = SQUIRREL_INVALID;
	HSQUIRRELVM v = (HSQUIRRELVM)script;
	HSQOBJECT object = *(HSQOBJECT*)&obj;
	sq_pushobject(v, object);
	sq_pushstring(v, fun, -1);
	sq_get(v, -2);

	if (sq_gettype(v, -1) != OT_CLOSURE)
	{
		sq_pop(v, 1);
		return ret;
	}
	sq_remove(v, -2);
	sq_pushobject(v, object);
	int count = V_strlen(types);

	va_list args;
	va_start(args, types);

	for (int i = 0; i < count; i++)
	{
		switch (types[i])
		{
		case 'i':
			sq_pushinteger(v, va_arg(args, int));
			break;
		case 'b':
			sq_pushbool(v, va_arg(args, bool));
			break;
		case 's':
			sq_pushstring(v, va_arg(args, const char*), -1);
			break;
		case 'f':
			sq_pushfloat(v, (float)(va_arg(args, double)));
			break;
		case 'u':
			sq_pushuserpointer(v, *(void**)&va_arg(args, SquirrelHandle));
			break;
		case 'o':
			sq_pushobject(v, *(HSQOBJECT*)va_arg(args, SquirrelObject*));
			break;
		default:
			sq_pushnull(v);
			break;
		}
	}
	va_end(args);
	if (sq_call(v, count + 1, 1, 0))
	{
		sq_getlasterror(v);
		const char* err;
		sq_getstring(v, -1, &err);
		Msg("[ERROR" STRINGG(__LINE__) "] %s\n", err);
		sq_pop(v, 1);
		return ret;
	}
	SQObjectType type = sq_gettype(v, -1);
	if (type == OT_STRING)
	{
		ret.type = SQUIRREL_STRING;
		sq_getstring(v, -1, &ret.val_string);
	}
	else if (type == OT_INTEGER)
	{
		ret.type = SQUIRREL_INT;
		sq_getinteger(v, -1, &ret.val_int);
	}
	else if (type == OT_FLOAT)
	{
		ret.type = SQUIRREL_FLOAT;
		sq_getfloat(v, -1, &ret.val_float);
	}
	else if (type == OT_BOOL)
	{
		ret.type = SQUIRREL_BOOL;
		SQBool p;
		sq_getbool(v, -1, &p);
		ret.val_bool = p;
	}
	else if (type == OT_USERDATA)
	{
		ret.type = SQUIRREL_USERDATA;
		sq_getuserpointer(v, -1, (void**)&ret.val_userdata);
	}
	sq_pop(v, 2);
	return ret;

}

SquirrelScript CSquirrel::LoadScript(const char* script, SquirrelFunctionDecl* initfuncs, SquirrelRegisterFunction sqregfunc)
{
	HSQUIRRELVM v = sq_open(2048);
	sq_pushroottable(v);
	CUtlVector<HSQOBJECT> objects;
	if (initfuncs)
	{
		while (initfuncs->ptr)
		{
			sq_pushstring(v, initfuncs->name, -1);
			sq_newclosure(v, (SQFUNCTION)initfuncs->ptr, 0);
			HSQOBJECT hi;
			sq_resetobject(&hi);
			sq_getstackobj(v, -1, &hi);
			objects.AddToTail(hi);
			sq_newslot(v, -3, false);
			initfuncs++;
		}
	}
	sqregfunc(v);
	sqstd_register_mathlib(v);
	sqstd_register_stringlib(v);
	if (SQ_FAILED(sq_compilebuffer(v, script, strlen(script), "squirrel", SQTrue)))
	{
		sq_getlasterror(v);
		const char* err;
		sq_getstring(v, -1, &err);
		Msg("[ERROR] [COMPILE] %s\n", err);
		return 0;
	}

	

	sq_pushroottable(v);
	SQRESULT callres = sq_call(v, 1, false, true);
	
	//for (int i = 0; i < objects.Count(); i++)
	//{
	//	if (sq_getrefcount(v, &objects[i]))
	//	{
	//		sq_release(v, &objects[i]);
	//	}
	//}
	sq_settop(v, 0);
	sq_collectgarbage(v);
	if (SQ_FAILED(callres))
	{
		sq_getlasterror(v);
		const char* err;
		sq_getstring(v, -1, &err);
		Msg("[ERROR] [SCRIPT] %s\n", err);
		return 0;
	}
	return (SquirrelScript)v;
}

SquirrelValue CSquirrel::CallFunction(SquirrelScript script, const char* fun, const char* types, ...)
{
	SquirrelValue ret;
	ret.type = SQUIRREL_INVALID;
	HSQUIRRELVM v = (HSQUIRRELVM)script;
	sq_pushroottable(v);
	sq_pushstring(v, fun,-1);
	sq_get(v, -2);

	if (sq_gettype(v, -1) != OT_CLOSURE)
	{
		sq_pop(v, 1);
		return ret;
	}
	sq_pushroottable(v);
	int count = strlen(types);
	
	va_list args;
	va_start(args, types);
	
	for (int i = 0; i < count; i++)
	{
		switch (types[i])
		{
		case 'i':
			sq_pushinteger(v, va_arg(args, int));
			break;
		case 'b':
			sq_pushbool(v, va_arg(args, bool));
			break;
		case 's':
			sq_pushstring(v, va_arg(args, const char*),-1);
			break;
		case 'f':
			sq_pushfloat(v, (float)(va_arg(args, double)));
			break;
		case 'u':
			sq_pushuserpointer(v, *(void**)&va_arg(args, SquirrelHandle));
			break;
		default:
			sq_pushnull(v);
			break;
		}
	}
	va_end(args);
	if (sq_call(v, count+1, 1, 0))
	{
		sq_getlasterror(v);
		const char* msg;
		sq_getstring(v, -1, &msg);
		Msg("ERROR: %s\n", msg);
		return ret;
	}
	SQObjectType type = sq_gettype(v, -1);
	if (type == OT_STRING)
	{
		ret.type = SQUIRREL_STRING;
		sq_getstring(v, -1, &ret.val_string);
	}
	else if (type == OT_INTEGER)
	{
		ret.type = SQUIRREL_INT;
		sq_getinteger(v, -1, &ret.val_int);
	}
	else if (type == OT_FLOAT)
	{
		ret.type = SQUIRREL_FLOAT;
		sq_getfloat(v, -1, &ret.val_float);
	}
	else if (type == OT_BOOL)
	{
		ret.type = SQUIRREL_BOOL;
		SQBool p;
		sq_getbool(v, -1, &p);
		ret.val_bool = p;
	}
	else if (type == OT_USERDATA)
	{
		ret.type = SQUIRREL_USERDATA;
		sq_getuserpointer(v, -1, (void**)&ret.val_userdata);
	}
	sq_pop(v, 2);
	return ret;
}
void CSquirrel::ShutdownScript(SquirrelScript script)
{
	HSQUIRRELVM v = (HSQUIRRELVM)script;
	sq_close(v);
}

void CSquirrel::AddFunction(SquirrelScript script, const char* name, SquirrelFunction fun)
{
	HSQUIRRELVM v = (HSQUIRRELVM)script;
	sq_pushroottable(v);
	sq_pushstring(v, name, -1);
	sq_newclosure(v, (SQFUNCTION)fun, 0);
	sq_newslot(v, -3, false);
	sq_pop(v, 1);
}


void CSquirrel::AddClassFunction(SquirrelScript script, SquirrelObject cls, const char* name, SquirrelFunction fun)
{
	HSQOBJECT* obj = (HSQOBJECT*)&cls;
	HSQUIRRELVM v = (HSQUIRRELVM)script;
	sq_pushobject(v, *obj);
	sq_pushstring(v, name, -1);
	sq_newclosure(v, (SQFUNCTION)fun, 0);
	sq_newslot(v, -3, false);
	sq_pop(v, 1);
}

bool CSquirrel::GetSQObject(SquirrelScript script, SquirrelObject& obj)
{
	HSQUIRRELVM v = (HSQUIRRELVM)script;
	SQObjectType t = sq_gettype(v, 1);
	if (t == OT_INSTANCE)
	{
		sq_getstackobj(v, 1, (HSQOBJECT*)&obj);
		return true;
	}
	return false;
}

SquirrelObject CSquirrel::CreateClass(SquirrelScript script)
{
	HSQUIRRELVM v = (HSQUIRRELVM)script;
	SquirrelObject cls;
	sq_newclass(v, false);
	sq_getstackobj(v, -1, (HSQOBJECT*)&cls);
	sq_pop(v, 1);
	return cls;
}

bool CSquirrel::GetArgs(SquirrelScript script, const char* types, ...)
{
	HSQUIRRELVM v = (HSQUIRRELVM)script;
	va_list args;
	va_start(args, types);
	int count = strlen(types);
	for (int i = 1; i < count+1; ++i)
	{
		SQObjectType type = sq_gettype(v, i + 1);
		switch (types[i-1])
		{
		case 'i':
			if (type == OT_INTEGER)
			{
				sq_getinteger(v, i+1, (va_arg(args, int*)));
			}
			else if (type == OT_FLOAT)
			{
				sq_getinteger(v, i + 1, (va_arg(args, int*)));
			}
			else
			{
				va_end(args);
				return false;
			}
			
			break;
		case 'b':
			if (type == OT_BOOL)
			{
				SQBool p;
				sq_getbool(v, i + 1, &p);
				*(va_arg(args, bool*)) = (bool)p;
			}
			else
			{
				va_end(args);
				return false;
			}
			break;
		case 's':
			if (type == OT_STRING)
			{
				sq_getstring(v, i + 1, (va_arg(args, const char**)));
			}
			else
			{
				va_end(args);
				return false;
			}
			break;
		case 'f':
			if (type == OT_FLOAT)
			{
				sq_getfloat(v, i + 1, (va_arg(args, float*)));
			}
			else
			{
				va_end(args);
				return false;
			}
			break;
		case 'u':
			if (type == OT_USERPOINTER)
			{
				sq_getuserpointer(v, i + 1, (void**)(va_arg(args, SquirrelHandle*)));
			}
			else
			{
				va_end(args);
				return false;
			}
			break;
		case 'c':
			if (type == OT_CLASS)
			{
				sq_getstackobj(v, i + 1, (HSQOBJECT*)(va_arg(args, SquirrelObject*)));
			}
			else
			{
				va_end(args);
				return false;
			}
			break;
		case 'o':
			if (type == OT_INSTANCE)
			{
				sq_getstackobj(v, i + 1, (HSQOBJECT*)(va_arg(args, SquirrelObject*)));
			}
			else
			{
				va_end(args);
				return false;
			}
			break;
		default:
			va_end(args);
			return false;
			break;
		}
	}
	return true;
}
void CSquirrel::PushValue(SquirrelScript script, SquirrelValue val)
{
	HSQUIRRELVM v = (HSQUIRRELVM)script;
	switch (val.type)
	{
	case SQUIRREL_INT:
		sq_pushinteger(v, val.val_int);
		break;
	case SQUIRREL_BOOL:
		sq_pushbool(v, val.val_bool);
		break;
	case SQUIRREL_STRING:
		sq_pushstring(v, val.val_string,-1);
		break;
	case SQUIRREL_FLOAT:
		sq_pushfloat(v, val.val_float);
		break;
	case SQUIRREL_USERDATA:
		sq_pushuserpointer(v, *(void**)&val.val_userdata);
		break;
	case SQUIRREL_OBJECT:
		sq_pushobject(v, *(HSQOBJECT*)&val.val_obj);
		break;
	default:
		sq_pushnull(v);
		break;
	}
}

void CSquirrel::PushArray(SquirrelScript script)
{
	HSQUIRRELVM v = (HSQUIRRELVM)script;
	sq_newarray(v, 0);
}

void CSquirrel::AppendToArray(SquirrelScript script)
{
	HSQUIRRELVM v = (HSQUIRRELVM)script;
	sq_arrayappend(v, -2);
}

void CSquirrel::IncrementRefCount(SquirrelScript script, SquirrelObject obj)
{
	HSQUIRRELVM v = (HSQUIRRELVM)script;
	sq_addref(v, (HSQOBJECT*)&obj);
}

EXPOSE_SINGLE_INTERFACE(CSquirrel, ISquirrel, INTERFACESQUIRREL_VERSION);
