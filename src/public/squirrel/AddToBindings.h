static constexpr auto CONCAT(CONCAT(LIBRARY_NAME, COUNTER_A), _SIG) = GetSignature(&SQ_FUNCTION_CLSNAME(SQ_FUNCTION()));

#define GetStackArg(a) \
if constexpr (count > a) \
{ \
	if constexpr (CONCAT(CONCAT(LIBRARY_NAME, COUNTER_A), _SIG)[a] == 'i') \
	{ \
		g_pSquirrel->GetStackInt(script, a + 1, &argi[a - 1].v_i); \
	} \
	else if constexpr (CONCAT(CONCAT(LIBRARY_NAME, COUNTER_A), _SIG)[a] == 'f') \
	{ \
		g_pSquirrel->GetStackFloat(script, a + 1, &argi[a - 1].v_f); \
	} \
	else if constexpr (CONCAT(CONCAT(LIBRARY_NAME, COUNTER_A), _SIG)[a] == 's') \
	{ \
		g_pSquirrel->GetStackString(script, a + 1, &argi[a - 1].v_s); \
	} \
	else if constexpr (CONCAT(CONCAT(LIBRARY_NAME, COUNTER_A), _SIG)[a] == 'p') \
	{ \
		g_pSquirrel->GetStackPtr(script, a + 1, &argi[a - 1].v_p, CONCAT(CONCAT(LIBRARY_NAME, COUNTER_A), _SIG).TypeNames[a]); \
	} \
}

#define ReturnToStack() \
if constexpr (CONCAT(CONCAT(LIBRARY_NAME, COUNTER_A), _SIG)[0] == 'i') \
{ \
	g_pSquirrel->PushInt(script, returnvar); \
} \
else if constexpr (CONCAT(CONCAT(LIBRARY_NAME, COUNTER_A), _SIG)[0] == 'f') \
{ \
	g_pSquirrel->PushFloat(script, returnvar); \
} \
else if constexpr (CONCAT(CONCAT(LIBRARY_NAME, COUNTER_A), _SIG)[0] == 's') \
{ \
	g_pSquirrel->PushString(script, returnvar); \
} \
else if constexpr (CONCAT(CONCAT(LIBRARY_NAME, COUNTER_A), _SIG)[0] == 'p') \
{ \
	g_pSquirrel->PushPtr(script, returnvar, CONCAT(CONCAT(LIBRARY_NAME, COUNTER_A), _SIG).TypeNames[0]); \
}

template<typename Class_Name, typename Return_Type>
int SQ_FUNCTION_CLS_NAME_WRAPPED(SQ_FUNCTION())(SquirrelScript script)
{
	static constexpr unsigned int count = sizeof(CONCAT(CONCAT(LIBRARY_NAME, COUNTER_A), _SIG)).Data - 1;
	SQ_FUNCTION_CLS(SQ_FUNCTION())* obj;
	g_pSquirrel->GetStackObjectUserdata(script, (void**)&obj);
	auto func = &SQ_FUNCTION_CLSNAME(SQ_FUNCTION());
	if constexpr (count == 1)
	{
		if constexpr (CONCAT(CONCAT(LIBRARY_NAME, COUNTER_A), _SIG)[0] == 'v')
		{
			(obj->*((Return_Type(__thiscall Class_Name::*)())func))();
		}
		else
		{
			auto returnvar = (obj->*((Return_Type(__thiscall Class_Name::*)())func))();
			ReturnToStack()
			return 1;
		}
	}
	else
	{
		FourByteValue argi[count - 1];

		GetStackArg(1)
		GetStackArg(2)
		GetStackArg(3)
		GetStackArg(4)
		GetStackArg(5)
		GetStackArg(6)
		GetStackArg(7)
		GetStackArg(8)
		GetStackArg(9)
		GetStackArg(10)
		GetStackArg(11)
		GetStackArg(12)

		if constexpr (CONCAT(CONCAT(LIBRARY_NAME, COUNTER_A), _SIG)[0] == 'v')
		{
			if constexpr (count == 2)
			{
				(obj->*((Return_Type(__thiscall Class_Name::*)(FourByteValue))func))(argi[0]);
			}
			else if constexpr (count == 3)
			{
				(obj->*((Return_Type(__thiscall Class_Name::*)(FourByteValue, FourByteValue))func))(argi[0], argi[1]);
			}
			else if constexpr (count == 4)
			{
				(obj->*((Return_Type(__thiscall Class_Name::*)(FourByteValue, FourByteValue, FourByteValue))func))(argi[0], argi[1], argi[2]);
			}
			else if constexpr (count == 5)
			{
				(obj->*((Return_Type(__thiscall Class_Name::*)(FourByteValue, FourByteValue, FourByteValue, FourByteValue))func))(argi[0], argi[1], argi[2], argi[3]);
			}
			else if constexpr (count == 6)
			{
				(obj->*((Return_Type(__thiscall Class_Name::*)(FourByteValue, FourByteValue, FourByteValue, FourByteValue, FourByteValue))func))(argi[0], argi[1], argi[2], argi[3], argi[4]);
			}
			else if constexpr (count == 7)
			{
				(obj->*((Return_Type(__thiscall Class_Name::*)(FourByteValue, FourByteValue, FourByteValue, FourByteValue, FourByteValue, FourByteValue))func))(argi[0], argi[1], argi[2], argi[3], argi[4], argi[5]);
			}
			else if constexpr (count == 8)
			{
				(obj->*((Return_Type(__thiscall Class_Name::*)(FourByteValue, FourByteValue, FourByteValue, FourByteValue, FourByteValue, FourByteValue, FourByteValue))func))(argi[0], argi[1], argi[2], argi[3], argi[4], argi[5], argi[6]);
			}
			else if constexpr (count == 9)
			{
				(obj->*((Return_Type(__thiscall Class_Name::*)(FourByteValue, FourByteValue, FourByteValue, FourByteValue, FourByteValue, FourByteValue, FourByteValue, FourByteValue))func))(argi[0], argi[1], argi[2], argi[3], argi[4], argi[5], argi[6], argi[7]);
			}
			else if constexpr (count == 10)
			{
				(obj->*((Return_Type(__thiscall Class_Name::*)(FourByteValue, FourByteValue, FourByteValue, FourByteValue, FourByteValue, FourByteValue, FourByteValue, FourByteValue, FourByteValue))func))(argi[0], argi[1], argi[2], argi[3], argi[4], argi[5], argi[6], argi[7], argi[8]);
			}
			else if constexpr (count == 11)
			{
				(obj->*((Return_Type(__thiscall Class_Name::*)(FourByteValue, FourByteValue, FourByteValue, FourByteValue, FourByteValue, FourByteValue, FourByteValue, FourByteValue, FourByteValue, FourByteValue))func))(argi[0], argi[1], argi[2], argi[3], argi[4], argi[5], argi[6], argi[7], argi[8], argi[9]);
			}
			else if constexpr (count == 12)
			{
				(obj->*((Return_Type(__thiscall Class_Name::*)(FourByteValue, FourByteValue, FourByteValue, FourByteValue, FourByteValue, FourByteValue, FourByteValue, FourByteValue, FourByteValue, FourByteValue, FourByteValue))func))(argi[0], argi[1], argi[2], argi[3], argi[4], argi[5], argi[6], argi[7], argi[8], argi[9], argi[10]);
			}
			else if constexpr (count == 13)
			{
				(obj->*((Return_Type(__thiscall Class_Name::*)(FourByteValue, FourByteValue, FourByteValue, FourByteValue, FourByteValue, FourByteValue, FourByteValue, FourByteValue, FourByteValue, FourByteValue, FourByteValue, FourByteValue))func))(argi[0], argi[1], argi[2], argi[3], argi[4], argi[5], argi[6], argi[7], argi[8], argi[9], argi[10], argi[11]);
			}
		}
		else
		{
			Return_Type returnvar;
			if constexpr (count == 2)
			{
				returnvar = (obj->*((Return_Type(__thiscall Class_Name::*)(FourByteValue))func))(argi[0]);
			}
			else if constexpr (count == 3)
			{
				returnvar = (obj->*((Return_Type(__thiscall Class_Name::*)(FourByteValue, FourByteValue))func))(argi[0], argi[1]);
			}
			else if constexpr (count == 4)
			{
				returnvar = (obj->*((Return_Type(__thiscall Class_Name::*)(FourByteValue, FourByteValue, FourByteValue))func))(argi[0], argi[1], argi[2]);
			}
			else if constexpr (count == 5)
			{
				returnvar = (obj->*((Return_Type(__thiscall Class_Name::*)(FourByteValue, FourByteValue, FourByteValue, FourByteValue))func))(argi[0], argi[1], argi[2], argi[3]);
			}
			else if constexpr (count == 6)
			{
				returnvar = (obj->*((Return_Type(__thiscall Class_Name::*)(FourByteValue, FourByteValue, FourByteValue, FourByteValue, FourByteValue))func))(argi[0], argi[1], argi[2], argi[3], argi[4]);
			}
			else if constexpr (count == 7)
			{
				returnvar = (obj->*((Return_Type(__thiscall Class_Name::*)(FourByteValue, FourByteValue, FourByteValue, FourByteValue, FourByteValue, FourByteValue))func))(argi[0], argi[1], argi[2], argi[3], argi[4], argi[5]);
			}
			else if constexpr (count == 8)
			{
				returnvar = (obj->*((Return_Type(__thiscall Class_Name::*)(FourByteValue, FourByteValue, FourByteValue, FourByteValue, FourByteValue, FourByteValue, FourByteValue))func))(argi[0], argi[1], argi[2], argi[3], argi[4], argi[5], argi[6]);
			}
			else if constexpr (count == 9)
			{
				returnvar = (obj->*((Return_Type(__thiscall Class_Name::*)(FourByteValue, FourByteValue, FourByteValue, FourByteValue, FourByteValue, FourByteValue, FourByteValue, FourByteValue))func))(argi[0], argi[1], argi[2], argi[3], argi[4], argi[5], argi[6], argi[7]);
			}
			else if constexpr (count == 10)
			{
				returnvar = (obj->*((Return_Type(__thiscall Class_Name::*)(FourByteValue, FourByteValue, FourByteValue, FourByteValue, FourByteValue, FourByteValue, FourByteValue, FourByteValue, FourByteValue))func))(argi[0], argi[1], argi[2], argi[3], argi[4], argi[5], argi[6], argi[7], argi[8]);
			}
			else if constexpr (count == 11)
			{
				returnvar = (obj->*((Return_Type(__thiscall Class_Name::*)(FourByteValue, FourByteValue, FourByteValue, FourByteValue, FourByteValue, FourByteValue, FourByteValue, FourByteValue, FourByteValue, FourByteValue))func))(argi[0], argi[1], argi[2], argi[3], argi[4], argi[5], argi[6], argi[7], argi[8], argi[9]);
			}
			else if constexpr (count == 12)
			{
				returnvar = (obj->*((Return_Type(__thiscall Class_Name::*)(FourByteValue, FourByteValue, FourByteValue, FourByteValue, FourByteValue, FourByteValue, FourByteValue, FourByteValue, FourByteValue, FourByteValue, FourByteValue))func))(argi[0], argi[1], argi[2], argi[3], argi[4], argi[5], argi[6], argi[7], argi[8], argi[9], argi[10]);
			}
			else if constexpr (count == 13)
			{
				returnvar = (obj->*((Return_Type(__thiscall Class_Name::*)(FourByteValue, FourByteValue, FourByteValue, FourByteValue, FourByteValue, FourByteValue, FourByteValue, FourByteValue, FourByteValue, FourByteValue, FourByteValue, FourByteValue))func))(argi[0], argi[1], argi[2], argi[3], argi[4], argi[5], argi[6], argi[7], argi[8], argi[9], argi[10], argi[11]);
			}
			ReturnToStack()
			return 1;
		}
	}
	return 0;
}
template<typename Class_Name, typename Return_Type, typename... Argument_Types>
constexpr auto SQ_FUNCTION_CLS_NAME_EXPAND(SQ_FUNCTION())(Return_Type(Class_Name::*)(Argument_Types...))
{
	return &SQ_FUNCTION_CLS_NAME_WRAPPED(SQ_FUNCTION())<Class_Name, Return_Type>;
}

constexpr auto CONCAT(LIBRARY_NAME, COUNTER_A) = Append(CONCAT(LIBRARY_NAME, COUNTER_B), (SQ_FUNCTION_CLS_NAME_EXPAND(SQ_FUNCTION())(&SQ_FUNCTION_CLSNAME(SQ_FUNCTION()))), STRINGG(SQ_FUNCTION_NAME(SQ_FUNCTION())));
#include INCREMENT_COUNTER_A
#include INCREMENT_COUNTER_B

SQ_FUNCTION_DECL(SQ_FUNCTION())