int CONCAT(CONCAT(SQ_CLASSNAME, _To), SQ_INHERITNAME)(SquirrelScript script)
{
	extern ISquirrel* g_pSquirrel;
	SQ_CLASSNAME* cls;
	if (!ConvertToCpp<SQ_CLASSNAME*>(script, &cls, 1))
	{
		return 0;  // TODO : MAKE ERROR
	}
	if (!((GenericConverterFromCpp)(((void* (*)())TypeIdentifier<SQ_INHERITNAME*>::ConvertFromCpp)()))(script, (SQ_INHERITNAME*)cls))
		return 0;
	return 1;
}

constexpr auto CONCAT(LIBRARY_NAME, COUNTER_A) = Append(CONCAT(LIBRARY_NAME, COUNTER_B), CONCAT(CONCAT(SQ_CLASSNAME, _To), SQ_INHERITNAME), STRINGG(CONCAT(To, SQ_INHERITNAME)));
#include INCREMENT_COUNTER_A
#include INCREMENT_COUNTER_B

#undef SQ_INHERITNAME