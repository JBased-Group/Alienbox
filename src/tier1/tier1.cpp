//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: A higher level link library for general use in the game and tools.
//
//===========================================================================//
#include "pch.h"
#include <tier1/tier1.h>
#include "tier0/dbg.h"
#include "vstdlib/iprocessutils.h"
#include "icvar.h"

#pragma warning (disable:4273)

bool HushAsserts() { return true; }

enum SpewType_t
{
	SPEW_MESSAGE = 0,
	SPEW_WARNING,
	SPEW_ASSERT,
	SPEW_ERROR,
	SPEW_LOG,

	SPEW_TYPE_COUNT
};

enum SpewRetval_t
{
	SPEW_DEBUGGER = 0,
	SPEW_CONTINUE,
	SPEW_ABORT
};


DLL_EXPORT void   _SpewInfo(SpewType_t type, const tchar* pFile, int line)
{
	return;
}

DLL_EXPORT SpewRetval_t   _SpewMessage(const tchar* pMsg, ...)
{
	return SPEW_CONTINUE;
}

typedef char* HTELEMETRY;

struct TelemetryData
{
	HTELEMETRY tmContext[32];
	float flRDTSCToMilliSeconds;	// Conversion from tmFastTime() (rdtsc) to milliseconds.
	uint32 FrameCount;				// Count of frames to capture before turning off.
	char ServerAddress[128];		// Server name to connect to.
	int playbacktick;				// GetPlaybackTick() value from demo file (or 0 if not playing a demo).
	uint32 DemoTickStart;			// Start telemetry on demo tick #
	uint32 DemoTickEnd;				// End telemetry on demo tick #
	uint32 Level;					// Current Telemetry level (Use TelemetrySetLevel to modify)
};

DLL_EXPORT TelemetryData g_Telemetry;
TelemetryData g_Telemetry;

DLL_IMPORT void Plat_ConvertToLocalTime(uint64 nTime, struct tm* pNow);

struct tm* Plat_localtime(const time_t* timep, struct tm* result)
{
	Plat_ConvertToLocalTime(*timep, result);
	return result;
}

DLL_EXPORT void CallAssertFailedNotifyFunc(const char* a, int b, const char* c)
{
	
}

#include "utlsoacontainer.h"

CSOAContainer::~CSOAContainer()
{
	Msg(__FUNCTION__ "UNIMPLEMENTED!!!!\n");
}


//-----------------------------------------------------------------------------
// These tier1 libraries must be set by any users of this library.
// They can be set by calling ConnectTier1Libraries or InitDefaultFileSystem.
// It is hoped that setting this, and using this library will be the common mechanism for
// allowing link libraries to access tier1 library interfaces
//-----------------------------------------------------------------------------
extern ICvar *g_pCVar;
extern IProcessUtils *g_pProcessUtils;
static bool s_bConnected = false;

// for utlsortvector.h
#ifndef _WIN32
	void *g_pUtlSortVectorQSortContext = NULL;
#endif


//-----------------------------------------------------------------------------
// Call this to connect to all tier 1 libraries.
// It's up to the caller to check the globals it cares about to see if ones are missing
//-----------------------------------------------------------------------------
void ConnectTier1Libraries( CreateInterfaceFn *pFactoryList, int nFactoryCount )
{
	// Don't connect twice..
	if ( s_bConnected )
		return;

	s_bConnected = true;

	for ( int i = 0; i < nFactoryCount; ++i )
	{
		if ( !g_pCVar )
		{
			cvar = g_pCVar = ( ICvar * )pFactoryList[i]( CVAR_INTERFACE_VERSION, NULL );
		}
		//if ( !g_pProcessUtils )
		//{
		//	g_pProcessUtils = ( IProcessUtils * )pFactoryList[i]( PROCESS_UTILS_INTERFACE_VERSION, NULL );
		//}
	}
}

void DisconnectTier1Libraries()
{
	if ( !s_bConnected )
		return;

	g_pCVar = cvar = 0;
	g_pProcessUtils = NULL;
	s_bConnected = false;
}

