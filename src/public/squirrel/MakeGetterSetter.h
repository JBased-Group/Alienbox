int CONCAT(CONCAT(SQ_CLASSNAME,SQ_VARNAME), Getter)(SquirrelScript script)
{
	extern ISquirrel* g_pSquirrel;
	SQ_CLASSNAME* cls;
	if (!ConvertToCpp<SQ_CLASSNAME*>(script, &cls, 1))
	{
		return 0;  // TODO : MAKE ERROR
	}
	auto returnvar = cls->SQ_VARNAME;
	if constexpr (Same<decltype(returnvar),int>)
	{
		g_pSquirrel->PushInt(script, *(int*)&returnvar);
	}
	else if constexpr (Same<decltype(returnvar), unsigned short>)
	{
		g_pSquirrel->PushInt(script, *(unsigned short*)&returnvar);
	}
	else if constexpr (Same<decltype(returnvar), float>)
	{
		g_pSquirrel->PushFloat(script, *(float*)&returnvar);
	}
	else if constexpr (Same<decltype(returnvar), const char*> || Same<decltype(returnvar), char*>)
	{
		g_pSquirrel->PushString(script, *(const char**)&returnvar);
	}
	else if constexpr (Same<decltype(returnvar), bool>)
	{
		g_pSquirrel->PushInt(script, *(char*)&returnvar);
	}
	else if constexpr (IsPointer<decltype(returnvar)>)
	{
		if (!((GenericConverterFromCpp)(((void* (*)())TypeIdentifier<decltype(returnvar)>::ConvertFromCpp)()))(script, *(void**)&returnvar))
			return 0;
	}
	else // YOU WILL BE A POINTER
	{
		if (!((GenericConverterFromCpp)(((void* (*)())TypeIdentifier<decltype(&cls->SQ_VARNAME)>::ConvertFromCpp)()))(script, (void*)&cls->SQ_VARNAME))
			return 0;
	}
	return 1;
}

int CONCAT(CONCAT(SQ_CLASSNAME, SQ_VARNAME), Setter)(SquirrelScript script)
{
	extern ISquirrel* g_pSquirrel;
	SQ_CLASSNAME* cls;
	if (!ConvertToCpp<SQ_CLASSNAME*>(script, &cls, 1))
	{
		return 0;  // TODO : MAKE ERROR
	}
	if constexpr (Same<decltype(SQ_CLASSNAME::SQ_VARNAME), int>)
	{
		if (!g_pSquirrel->GetStackInt(script, 2, (int*)&cls->SQ_VARNAME))
			return 0;
	}
	else if constexpr (Same<decltype(SQ_CLASSNAME::SQ_VARNAME), float>)
	{
		if (!g_pSquirrel->GetStackFloat(script, 2, (float*)&cls->SQ_VARNAME))
			return 0;
	}
	else if constexpr (Same<decltype(SQ_CLASSNAME::SQ_VARNAME), const char*> || Same<decltype(SQ_CLASSNAME::SQ_VARNAME), char*>)
	{
		if (!g_pSquirrel->GetStackString(script, 2, (const char**)&cls->SQ_VARNAME))
			return 0;
	}
	else if constexpr (IsPointer<decltype(SQ_CLASSNAME::SQ_VARNAME)>)
	{
		if (!((GenericConverterToCpp)(((void* (*)())TypeIdentifier<decltype(SQ_CLASSNAME::SQ_VARNAME)>::ConvertToCpp)()))(script, (void**)&cls->SQ_VARNAME, 2))
			return 0;
	}

	return 0;
}


constexpr auto CONCAT(LIBRARY_NAME, COUNTER_A) = Append(CONCAT(LIBRARY_NAME, COUNTER_B), CONCAT(CONCAT(SQ_CLASSNAME, SQ_VARNAME), Setter), STRINGG(CONCAT(Set_, SQ_VARNAME)));
#include INCREMENT_COUNTER_A
#include INCREMENT_COUNTER_B

constexpr auto CONCAT(LIBRARY_NAME, COUNTER_A) = Append(CONCAT(LIBRARY_NAME, COUNTER_B), CONCAT(CONCAT(SQ_CLASSNAME, SQ_VARNAME), Getter), STRINGG(CONCAT(Get_, SQ_VARNAME)));
#include INCREMENT_COUNTER_A
#include INCREMENT_COUNTER_B

#undef SQ_VARNAME