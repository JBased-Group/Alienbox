#ifndef SQUIRREL_H
#define SQUIRREL_H
#define INTERFACESQUIRREL_VERSION				"ABoxSquirrel001"
#include "counter.h"
#include "string_t.h"
#include "datamap.h"
#include "../game/server/variant_t.h"
#include "dt_common.h"
#include "dt_send.h"
typedef void* SquirrelScript;

enum SquirrelType
{
	SQUIRREL_INT,
	SQUIRREL_FLOAT,
	SQUIRREL_STRING,
	SQUIRREL_BOOL,
	SQUIRREL_USERDATA,
	SQUIRREL_CLASS,
	SQUIRREL_OBJECT,

	SQUIRREL_INVALID // must be last
};


#pragma pack(push, 1)
union SquirrelHandle
{
	struct PoolId
	{
		unsigned char pool;
		unsigned char id[3];
	} p;
	int poolid;
};
#pragma pack(pop)

#define SPMASK 0x00ffffff

enum SquirrelPool
{
	SP_VGUI,

	SPCOUNT
};

struct SquirrelObject
{
	int unused1;
	int unused2;
};

struct SquirrelValue  // I FUCKING LOVE TYPED UNIONS GRAAAAAAAAAAAAAAAAAAAH
{
	union
	{
		int val_int;
		float val_float;
		const char* val_string;
		bool val_bool;
		SquirrelHandle val_userdata;
		SquirrelObject val_obj;
	};
	SquirrelType type;
};

typedef int (*SquirrelFunction)(SquirrelScript);

struct SquirrelFunctionDecl
{
	const char* name;
	SquirrelFunction ptr;
};

struct SquirrelClassDecl
{
	const char* name;
	const SquirrelFunctionDecl* funcs;
	SquirrelObject* clsobj;
	const int* typetag;
};

typedef void (*SquirrelRegisterFunction)(SquirrelScript);

class ISquirrel
{
public:
	virtual SquirrelScript LoadScript(const char* script, SquirrelFunctionDecl* initfuncs, SquirrelRegisterFunction sqregfunc) = 0;
	virtual SquirrelValue CallFunction(SquirrelScript script, const char* fun, const char* types, ...) = 0;
	virtual void ShutdownScript(SquirrelScript script) = 0;
	virtual void AddFunction(SquirrelScript script, const char* name, SquirrelFunction fun) = 0;
	virtual bool GetArgs(SquirrelScript script, const char* types, ...) = 0;
	virtual void PushValue(SquirrelScript script, SquirrelValue val) = 0;
	virtual void PushArray(SquirrelScript script) = 0;
	virtual void AppendToArray(SquirrelScript script) = 0;
	virtual SquirrelValue InstantiateClass(SquirrelScript script, SquirrelObject cls) = 0;
	virtual SquirrelValue CallObjectFunction(SquirrelScript script, SquirrelObject obj, const char* fun, const char* types, ...) = 0;
	virtual void IncrementRefCount(SquirrelScript script, SquirrelObject obj) = 0;
	virtual void AddClassFunction(SquirrelScript script, SquirrelObject cls, const char* name, SquirrelFunction fun) = 0;
	virtual bool GetSQObject(SquirrelScript script, SquirrelObject& obj) = 0;
	virtual SquirrelObject CreateClass(SquirrelScript script) = 0;
	virtual void SetObjectUserdata(SquirrelScript script, SquirrelObject obj, void* ptr, const int* typetag) = 0;
	virtual void* GetObjectUserdata(SquirrelScript script, SquirrelObject obj) = 0;
	virtual bool GetStackInt(SquirrelScript script, int i, int* val) = 0;
	virtual bool GetStackFloat(SquirrelScript script, int i, float* val) = 0;
	virtual bool GetStackString(SquirrelScript script, int i, const char** val) = 0;
	virtual bool GetStackPtr(SquirrelScript script, int i, void** val, const int* typetag) = 0;
	virtual void PushInt(SquirrelScript script, int val) = 0;
	virtual void PushFloat(SquirrelScript script, float val) = 0;
	virtual void PushString(SquirrelScript script, const char* val) = 0;
	virtual void PushPtr(SquirrelScript script, void* val, const int* typetag) = 0;
	virtual bool GetStackObjectUserdata(SquirrelScript script, void** ptr) = 0;
	virtual void RegisterClasses(SquirrelScript script, SquirrelClassDecl* classes) = 0;
	virtual bool SetObjectVariant(SquirrelScript script, SquirrelObject obj, const char* name, variant_t* var, fieldtype_t ftype) = 0;
	virtual bool GetObjectVariant(SquirrelScript script, SquirrelObject obj, const char* name, variant_t* var) = 0;
	virtual datamap_t* GenerateDatamap(SquirrelScript script, SquirrelObject obj, datamap_t* basemap) = 0;
	virtual SendTable* GenerateSendtable(SquirrelScript script, SquirrelObject obj, SendTable* basemap, size_t objOffset) = 0;
};




template <typename Type>
constexpr bool IsPointer = false;

template <typename Type>
constexpr bool IsPointer<Type*> = true;

template <typename Type>
constexpr bool IsPointer<Type* const> = true;

template <typename Type>
constexpr bool IsPointer<Type&> = true;



template <typename Type, typename NotTheSameType>
constexpr bool Same = false;

template <typename Type>
constexpr bool Same<Type, Type> = true;

template <typename Return_Type, typename Class_Name, typename... Argument_Types, typename Type>
constexpr bool IsThiscall(Return_Type(Class_Name::*)(Argument_Types...), Type t)
{
	return Same<Return_Type(__thiscall Class_Name::*)(Argument_Types...), Type>;
}

template <typename Return_Type, typename... Argument_Types, typename Type>
constexpr bool IsCDecl(Return_Type(*)(Argument_Types...), Type t)
{
	return Same<Return_Type(__cdecl *)(Argument_Types...), Type>;
}



template <typename Value_Type>
constexpr const char GetType()
{
	if (Same<Value_Type, bool>)
		return 'b';
	if (Same<Value_Type, float>)
		return 'f';
	if (Same<Value_Type, int>)
		return 'i';
	if (Same<Value_Type, void>)
		return 'v';
	if (Same<Value_Type, const char*>)
		return 's';
	if (Same<Value_Type, char*>)
		return 's';
	if (IsPointer<Value_Type>)
		return 'p';
	return '?';
}

typedef bool (*GenericConverterToCpp)(SquirrelScript, void**, int);
typedef bool (*GenericConverterFromCpp)(SquirrelScript, void*);

template <unsigned int Size>
struct ReturnableString
{
	char Data[Size];
	const int* TypeNames[Size];
	void* ConvertCpp[Size];

	operator char* () const
	{
		return Data;
	}

	operator const char* () const
	{
		return Data;
	}

	constexpr char operator[](int val) const
	{
		return Data[val];
	}
};

template <unsigned int Size>
constexpr ReturnableString<Size> operator +(ReturnableString<Size> Left, char Character)
{
	for (int i = 0; i < Size - 1; i++)
	{
		if (Left.Data[i] == '\x00')
		{
			Left.Data[i] = Character;
			return Left;
		}
	}
	return Left;
}

template <unsigned int Size>
constexpr ReturnableString<Size> operator +(ReturnableString<Size> Left, const int* TypeId)
{
	for (int i = 0; i < Size - 1; i++)
	{
		if (Left.TypeNames[i] == 0)
		{
			Left.TypeNames[i] = TypeId;
			return Left;
		}
	}
	return Left;
}

template <unsigned int Size>
constexpr ReturnableString<Size> operator +(ReturnableString<Size> Left, void* TypeId)
{
	for (int i = 0; i < Size - 1; i++)
	{
		if (Left.ConvertCpp[i] == 0)
		{
			Left.ConvertCpp[i] = TypeId;
			return Left;
		}
	}
	return Left;
}

template <typename Type>
bool ConvertToCpp(SquirrelScript script, Type* valOut, int a);


// Thanks https://stackoverflow.com/a/77093313
template <class C>
struct TypeIdentifier {
	constexpr static int _id{};
	constexpr static const int* id() {
		return &_id;
	}
	static const GenericConverterToCpp ConvertToCpp()
	{
		return (GenericConverterToCpp)(::ConvertToCpp<C>);
	}
	static bool ConvertFromCpp(SquirrelScript script, C val);
};

template <class C>
struct TypeIdentifier<C&>
{
	constexpr static const int* id() {
		return TypeIdentifier<C*>::id();
	}
	static const GenericConverterToCpp ConvertToCpp()
	{
		return (GenericConverterToCpp)(::ConvertToCpp<C*>);
	}
	static bool ConvertFromCpp(SquirrelScript script, C* val);
};

template <class C>
struct TypeIdentifier<const C*>
{
	constexpr static const int* id() {
		return TypeIdentifier<C*>::id();
	}
	static const GenericConverterToCpp ConvertToCpp()
	{
		return (GenericConverterToCpp)(::ConvertToCpp<C*>);
	}
	static bool ConvertFromCpp(SquirrelScript script, C* val);
};

template <class C>
struct TypeIdentifier<const C&>
{
	constexpr static const int* id() {
		return TypeIdentifier<C*>::id();
	}
	static const GenericConverterToCpp ConvertToCpp()
	{
		return (GenericConverterToCpp)(::ConvertToCpp<C*>);
	}
	static bool ConvertFromCpp(SquirrelScript script, C* val);
};

template <typename Class_Name, typename Return_Type, typename... Argument_Types>
constexpr ReturnableString<sizeof...(Argument_Types) + 2> GetSignature(Return_Type(Class_Name::*)(Argument_Types...))
{
	constexpr unsigned int Size = sizeof...(Argument_Types) + 2;
	ReturnableString<Size> Ret{};
	Ret.Data[0] = GetType<Return_Type>();
	if constexpr (!Same<Return_Type, void>)
	{
		Ret.TypeNames[0] = TypeIdentifier<Return_Type>::id();
		Ret.ConvertCpp[0] = (void*)TypeIdentifier<Return_Type>::ConvertToCpp;
	}
	else
	{
		Ret.TypeNames[0] = TypeIdentifier<void*>::id();
		Ret.ConvertCpp[0] = (void*)TypeIdentifier<int>::ConvertToCpp;
	}
	((Ret = Ret + GetType<Argument_Types>()), ...);
	((Ret = Ret + TypeIdentifier<Argument_Types>::id()), ...);
	((Ret = Ret + (void*)TypeIdentifier<Argument_Types>::ConvertToCpp), ...);

	return Ret;
}


template <typename Class_Name, typename Return_Type, typename... Argument_Types>
constexpr ReturnableString<sizeof...(Argument_Types) + 2> GetSignature(Return_Type(Class_Name::*)(Argument_Types...) const)
{
	constexpr unsigned int Size = sizeof...(Argument_Types) + 2;
	ReturnableString<Size> Ret{};
	Ret.Data[0] = GetType<Return_Type>();
	if constexpr (!Same<Return_Type, void>)
	{
		Ret.TypeNames[0] = TypeIdentifier<Return_Type>::id();
		Ret.ConvertCpp[0] = (void*)TypeIdentifier<Return_Type>::ConvertToCpp;
	}
	else
	{
		Ret.TypeNames[0] = TypeIdentifier<void*>::id();
		Ret.ConvertCpp[0] = (void*)TypeIdentifier<int>::ConvertToCpp;
	}
	((Ret = Ret + GetType<Argument_Types>()), ...);
	((Ret = Ret + TypeIdentifier<Argument_Types>::id()), ...);
	((Ret = Ret + (void*)TypeIdentifier<Argument_Types>::ConvertToCpp), ...);
	return Ret;
}

template <typename Return_Type, typename... Argument_Types>
constexpr ReturnableString<sizeof...(Argument_Types) + 2> GetSignature(Return_Type(*)(Argument_Types...))
{
	constexpr unsigned int Size = sizeof...(Argument_Types) + 2;
	ReturnableString<Size> Ret{};
	Ret.Data[0] = GetType<Return_Type>();
	if constexpr (!Same<Return_Type, void>)
	{
		Ret.TypeNames[0] = TypeIdentifier<Return_Type>::id();
		Ret.ConvertCpp[0] = (void*)TypeIdentifier<Return_Type>::ConvertToCpp;
	}
	else
	{
		Ret.TypeNames[0] = TypeIdentifier<void*>::id();
		Ret.ConvertCpp[0] = (void*)TypeIdentifier<int>::ConvertToCpp;
	}
	((Ret = Ret + GetType<Argument_Types>()), ...);
	((Ret = Ret + TypeIdentifier<Argument_Types>::id()), ...);
	((Ret = Ret + (void*)TypeIdentifier<Argument_Types>::ConvertToCpp), ...);
	return Ret;
}

union FourByteValue
{
	const char* v_s;
	int v_i;
	float v_f;
	void* v_p;
};


template <unsigned int Size>
struct ListOfStuff
{
	SquirrelFunctionDecl Data[Size];
};

template <>
struct ListOfStuff<0>
{
};

template <unsigned int Count>
constexpr ListOfStuff<Count + 1> Append(ListOfStuff<Count> List, SquirrelFunction Function, const char* name)
{
	ListOfStuff<Count + 1> Output{};
	Output.Data[0].ptr = Function;
	Output.Data[0].name = name;
	if constexpr (Count != 0)
	{
		for (int i = 0; i < Count; i++)
		{
			Output.Data[i + 1] = List.Data[i];
		}
	}

	return Output;
}


#define CONCAT_A(a,b) a##b
#define CONCAT(a,b) CONCAT_A(a,b)

#define CONCATFUNC_A(a,b,c,d) a b::c d
#define CONCATFUNC(a,b,c,d) CONCATFUNC_A(a,b,c,d)

#define STRINGG_1(a) #a
#define STRINGG_2(a) STRINGG_1(a)
#define STRINGG_3(a) STRINGG_2(a)
#define STRINGG(a) STRINGG_3(a)

#include INCREMENT_COUNTER_A



#define SQ_FUNCTION_RET_IMPL(a,b,c,d) a
#define SQ_FUNCTION_CLS_IMPL(a,b,c,d) b
#define SQ_FUNCTION_CLSNAME_IMPL(b,NO,...) b##__VA_ARGS__
#define SQ_FUNCTION_CLSNAME_FINALIZER_2(a) SQ_FUNCTION_CLSNAME_IMPL a
#define SQ_FUNCTION_CLSNAME_FINALIZER(a) SQ_FUNCTION_CLSNAME_FINALIZER_2(a)

#define SQ_FUNCTION_CLSNAME_WRAPPED_IMPL(b,NO,...) b##__VA_ARGS__##_WRAPPED
#define SQ_FUNCTION_CLSNAME_WRAPPED_FINALIZER_2(a) SQ_FUNCTION_CLSNAME_WRAPPED_IMPL a
#define SQ_FUNCTION_CLSNAME_WRAPPED_FINALIZER(a) SQ_FUNCTION_CLSNAME_WRAPPED_FINALIZER_2(a)

#define SQ_FUNCTION_CLSNAME_EXPAND_IMPL(b,NO,...) b##__VA_ARGS__##_EXPAND
#define SQ_FUNCTION_CLSNAME_EXPAND_FINALIZER_2(a) SQ_FUNCTION_CLSNAME_EXPAND_IMPL a
#define SQ_FUNCTION_CLSNAME_EXPAND_FINALIZER(a) SQ_FUNCTION_CLSNAME_EXPAND_FINALIZER_2(a)

#define SQ_HASPARENS(...) ,
#define SQ_COMMA_X(a,b,c,...) SQ_IF_ ## c
#define SQ_COMMA(a) SQ_COMMA_X a
#define SQ_IF_0 
#define SQ_IF_1 ,

#define SQ_FUNCTION_CLSNAME_PROXY(a,b,c,...) SQ_FUNCTION_CLSNAME_FINALIZER((b , Hi SQ_COMMA((Hi SQ_HASPARENS __VA_ARGS__ Hi ,1, 0)) ::c))
#define SQ_FUNCTION_CLS_NAME_WRAPPED_IMPL(a,b,c,...) SQ_FUNCTION_CLSNAME_WRAPPED_FINALIZER((b , Hi SQ_COMMA((Hi SQ_HASPARENS __VA_ARGS__ Hi ,1, 0)) _##c))
#define SQ_FUNCTION_CLS_NAME_EXPAND_IMPL(a,b,c,...) SQ_FUNCTION_CLSNAME_EXPAND_FINALIZER((b , Hi SQ_COMMA((Hi SQ_HASPARENS __VA_ARGS__ Hi ,1, 0)) _##c))


#define SQ_CHECK_X(a,b,c,...) c
#define SQ_CHECK(a) SQ_CHECK_X a
#define SQ_FUNCTION_NAME_IMPL(a,b,c,...) SQ_CHECK((Hi SQ_HASPARENS c Hi , b , c ))
#define SQ_FUNCTION_NAME(a) SQ_FUNCTION_NAME_IMPL a
#define SQ_FUNCTION_DECL_IMPL(a,b,c,...) SQ_CHECK((Hi SQ_HASPARENS c Hi ,a b c, a b::c __VA_ARGS__))
#define SQ_FUNCTION_DECL(a) SQ_FUNCTION_DECL_IMPL a
#define SQ_FUNCTION_CLS(a) SQ_FUNCTION_CLS_IMPL a
#define SQ_FUNCTION_RET(a) SQ_FUNCTION_RET_IMPL a
#define SQ_FUNCTION_CLSNAME(a) SQ_FUNCTION_CLSNAME_PROXY a
#define SQ_FUNCTION_CLS_NAME_WRAPPED(a) SQ_FUNCTION_CLS_NAME_WRAPPED_IMPL a
#define SQ_FUNCTION_CLS_NAME_EXPAND(a) SQ_FUNCTION_CLS_NAME_EXPAND_IMPL a

#define SQ_FUNCTION_DECLFUNC_PROXY(a,b,c,...) SQ_CHECK((Hi SQ_HASPARENS c Hi ,a b c;, ))
#define SQ_FUNCTION_DECLFUNC(a) SQ_FUNCTION_DECLFUNC_PROXY a

#define STARTLIBRARY constexpr ListOfStuff<COUNTER_A> CONCAT(LIBRARY_NAME,COUNTER_B) = { 0,0 };



template <>
static inline bool ConvertToCpp<float>(SquirrelScript script, float* valOut, int a)
{
	return true;
}

template <>
static inline bool ConvertToCpp<int>(SquirrelScript script, int* valOut, int a)
{
	return true;
}

template <>
static inline bool ConvertToCpp<char*>(SquirrelScript script, char** valOut, int a)
{
	return true;
}

template <>
static inline bool ConvertToCpp<bool>(SquirrelScript script, bool* valOut, int a)
{
	return true;
}


#endif