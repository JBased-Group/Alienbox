#include "squirrel.h"
#include "../public/squirrel/squirrel.h"
#include "tier1/interface.h"
#include <stdarg.h>
#include "sqstdstring.h"
#include "sqstdmath.h"
#include "utlvector.h"

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
	virtual void GetStackInt(SquirrelScript script, int i, int* val);
	virtual void GetStackFloat(SquirrelScript script, int i, float* val);
	virtual void GetStackString(SquirrelScript script, int i, const char** val);
	virtual void GetStackPtr(SquirrelScript script, int i, void** val, const int* typetag);
	virtual void PushInt(SquirrelScript script, int val);
	virtual void PushFloat(SquirrelScript script, float val);
	virtual void PushString(SquirrelScript script, const char* val);
	virtual void PushPtr(SquirrelScript script, void* val, const int* typetag);
	virtual void GetStackObjectUserdata(SquirrelScript script, void** ptr);
	virtual void RegisterClasses(SquirrelScript script, SquirrelClassDecl* classes);
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

void CSquirrel::GetStackInt(SquirrelScript script, int i, int* val)
{
	sq_getinteger((HSQUIRRELVM)script, i, val);
}

void CSquirrel::GetStackFloat(SquirrelScript script, int i, float* val)
{
	sq_getfloat((HSQUIRRELVM)script, i, val);
}

void CSquirrel::GetStackString(SquirrelScript script, int i, const char** val)
{
	sq_getstring((HSQUIRRELVM)script, i, val);
}

void CSquirrel::GetStackPtr(SquirrelScript script, int i, void** val, const int* typetag)
{
	sq_getinstanceup((HSQUIRRELVM)script, i, val, (void*)typetag, SQFalse);
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

void CSquirrel::PushPtr(SquirrelScript script, void* val, const int* typetag)
{
	sq_pushuserpointer((HSQUIRRELVM)script, val);
}

void CSquirrel::GetStackObjectUserdata(SquirrelScript script, void** ptr)
{
	sq_getinstanceup((HSQUIRRELVM)script, 1, ptr, 0, SQFalse);
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
			sq_pushstring(v, classes->funcs->name, -1);
			sq_newclosure(v, (SQFUNCTION)classes->funcs->ptr, 0);
			sq_newslot(v, -3, false);
			classes->funcs++;
		}
		sq_newslot(v, -3, false);
		classes++;
	}
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
		Msg("%s\n",err);

		ret.type = SQUIRREL_INVALID;
		return ret;
	}
	sq_getstackobj(v, -1, (HSQOBJECT*)&ret.val_obj);
	sq_addref(v, (HSQOBJECT*)&ret.val_obj);
	sq_pop(v, 2);
	ret.type = SQUIRREL_OBJECT;
	return ret;
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
			sq_pushstring(v, va_arg(args, const char*), -1);
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
	if (sq_call(v, count + 1, 1, 0))
	{
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
	sq_compilebuffer(v, script, strlen(script),"squirrel",SQTrue);

	sq_pushroottable(v);
	SQRESULT callres = sq_call(v, 1, false, true);
	for (int i = 0; i < objects.Count(); i++)
	{
		if (sq_getrefcount(v, &objects[i]))
		{
			sq_release(v, &objects[i]);
		}
	}
	sq_settop(v, 0);
	sq_collectgarbage(v);
	if (SQ_FAILED(callres))
	{
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
