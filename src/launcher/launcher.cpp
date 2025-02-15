#include <io.h>
#include <stdio.h>
#include "platform.h"
#include "KeyValues.h"
#include "utlbuffer.h"
#include "launcher.h"

static char g_szBasedir[MAX_PATH];



bool DesiredApp(int appid)
{
	switch (appid)
	{
	case 563560: //asw:rd
	case 620: //p2
	case 550: //l4d2
		return true;
	default:
		return false;
	}
}

void PatchSearchPath(KeyValues* keyv, const char* find, const char* basepath)
{
	char replace[MAX_PATH];
	V_strncpy(replace, basepath, MAX_PATH);
	V_strncat(replace, find, MAX_PATH);
	int findlen = V_strlen(find);
	for (KeyValues* value = keyv->GetFirstValue(); value; value = value->GetNextValue())
	{
		const char* str = value->GetString();
		if (!str)
			continue;
		int len = V_strlen(str);
		if (len >= findlen && strncmp(str + len - findlen, find, findlen) == 0)
		{
			value->SetStringValue(replace);
			return;
		}
	}
}//xorusr: sum changes in this func


KeyValues* GetKeyvaluesFromFile(const char* dir, const char* name)
{
	FILE* fp = fopen(dir, "r");
	if (!fp)
	{
		MessageBoxA(0, dir, name, MB_OK);
		return 0;
	}
	fseek(fp, 0L, SEEK_END);
	int sz = ftell(fp);
	char* fileRead = (char*)g_pMemAlloc->IndirectAlloc(sz + 2);
	V_memset(fileRead, 0, sz + 1);
	rewind(fp);
	fread((void*)fileRead, 1, sz, fp);
	fileRead[sz] = '\x00';
	KeyValues* keyv = new KeyValues(name);
	keyv->LoadFromBuffer(name, fileRead);
	fclose(fp);
	g_pMemAlloc->Free(fileRead);
	return keyv;
}

void GetAppManifest(const char* appid, const char* path)
{
	char manifestDir[MAX_PATH];
	V_strncpy(manifestDir, path, MAX_PATH);
	V_AppendSlash(manifestDir, MAX_PATH);
	V_strncat(manifestDir, "steamapps", MAX_PATH);
	V_AppendSlash(manifestDir, MAX_PATH);
	V_strncat(manifestDir, "appmanifest_", MAX_PATH);
	V_strncat(manifestDir, appid, MAX_PATH);
	V_strncat(manifestDir, ".acf", MAX_PATH);
	KeyValues* appmanifest = GetKeyvaluesFromFile(manifestDir, "AppState");
	const char* installDir = appmanifest->GetString("installdir");
	char appDir[MAX_PATH];
	V_strncpy(appDir, path, MAX_PATH);
	V_AppendSlash(appDir, MAX_PATH);
	V_strncat(appDir, "steamapps", MAX_PATH);
	V_AppendSlash(appDir, MAX_PATH);
	V_strncat(appDir, "common", MAX_PATH);
	V_AppendSlash(appDir, MAX_PATH);
	V_strncat(appDir, installDir, MAX_PATH);
	V_AppendSlash(appDir, MAX_PATH);

	V_FixSlashes(appDir, '\\');
	V_FixDoubleSlashes(appDir);

	char gameinfoDir[MAX_PATH];
	V_strncpy(gameinfoDir, g_szBasedir, MAX_PATH);
	V_AppendSlash(gameinfoDir, MAX_PATH);
	V_strncat(gameinfoDir, "alienbox", MAX_PATH);
	V_AppendSlash(gameinfoDir, MAX_PATH);
	V_strncat(gameinfoDir, "gameinfo.txt", MAX_PATH);
	KeyValues* gameinfo = GetKeyvaluesFromFile(gameinfoDir, "GameInfo");
	KeyValues* searchpaths = gameinfo->FindKey("FileSystem", true)->FindKey("SearchPaths", true);

	switch (Q_atoi(appid))
	{
	case 620: //p2
		PatchSearchPath(searchpaths, "portal2", appDir);
		PatchSearchPath(searchpaths, "portal2_dlc1", appDir);
		PatchSearchPath(searchpaths, "portal2_dlc2", appDir);
		PatchSearchPath(searchpaths, "update", appDir);
		break;
	case 550: //l4d2
		PatchSearchPath(searchpaths, "left4dead2_dlc3", appDir);
		PatchSearchPath(searchpaths, "left4dead2_dlc2", appDir);
		PatchSearchPath(searchpaths, "left4dead2_dlc1", appDir);
		PatchSearchPath(searchpaths, "left4dead2", appDir);
		break;
	case 563560: //asw:rd
		PatchSearchPath(searchpaths, "reactivedrop", appDir);
		PatchSearchPath(searchpaths, "platform", appDir);
		break;
	default:
		char stupidwarning[128];
		snprintf(stupidwarning, 128, "YOU FORGOT TO ADD THE GAME (%s) TO GetAppManifest() IN launcher/launcher.cpp", appid);
		MessageBoxA(NULL, stupidwarning, "YOU BUFFOON", MB_OK | MB_ICONEXCLAMATION);
		break;
	}

	appmanifest->deleteThis();
	CUtlBuffer buf(0, 0, 0);
	gameinfo->RecursiveSaveToFile(buf, 0);
	gameinfo->deleteThis();

	FILE* fp = fopen(gameinfoDir, "w");
	fwrite(buf.Base(), 1, buf.PeekStringLength(), fp);
	fclose(fp);

}




int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	char* pPath = getenv("PATH");
	
	//xorusr
#if !defined _DEBUG
	if (!isProcessRunning("steam.exe"))
	{
		MessageBoxW(NULL, L"Start the steam client first!", L"Error", MB_OK | MB_ICONERROR);
		exit(0); // closing the launcher if steam is offline
	}
#endif

	// Use the .EXE name to determine the root directory
	if (!::GetModuleFileNameA((HINSTANCE)GetModuleHandle(NULL), g_szBasedir, MAX_PATH))
	{
		return 1;
	}
	char* pBuffer = strrchr(g_szBasedir, '\\');
	if (pBuffer)
	{
		*(pBuffer + 1) = '\0';
	}

	char szBuffer[4096];
	_snprintf(szBuffer, sizeof(szBuffer), "PATH=%s\\bin\\;%s", g_szBasedir, pPath);
	szBuffer[sizeof(szBuffer) - 1] = '\0';
	_putenv(szBuffer);
	printf("%x %x %s %x\n", (unsigned int)hInstance, (unsigned int)hPrevInstance, lpCmdLine, nCmdShow);

	_snprintf(szBuffer, sizeof(szBuffer), "%s\n", g_szBasedir);

	char steamDir[MAX_PATH];
	char steamLibraryFolders[MAX_PATH];
	DWORD BufferSize = 8192;
	RegGetValueA(HKEY_LOCAL_MACHINE, "SOFTWARE\\WOW6432Node\\Valve\\Steam", "InstallPath", RRF_RT_REG_SZ, NULL, (PVOID)&steamDir, &BufferSize);
	if (!steamDir)
		return 1;
	V_strncpy(steamLibraryFolders, steamDir, MAX_PATH);
	V_strncat(steamLibraryFolders, "\\steamapps\\libraryfolders.vdf", MAX_PATH);
	KeyValues* libraryfolders = GetKeyvaluesFromFile(steamLibraryFolders, "libraryfolders");
	char aswrddir[MAX_PATH] = "";
	for (KeyValues* folder = libraryfolders->GetFirstTrueSubKey(); folder; folder = folder->GetNextTrueSubKey())
	{
		//Msg("%s %x\n", folder->GetName(), folder->FindKey("apps")->GetFirstValue());
		for (KeyValues* app = folder->FindKey("apps", true)->GetFirstValue(); app; app = app->GetNextValue())
		{
			const char* appid = app->GetName();

			if (DesiredApp(Q_atoi(appid)))
			{
				GetAppManifest(appid, folder->GetString("path"));
			}
			if (Q_atoi(appid) == 563560)
			{
				V_strncpy(aswrddir, folder->GetString("path"), MAX_PATH);
				V_strncat(aswrddir, "\\steamapps\\common\\Alien Swarm Reactive Drop\\reactivedrop.exe", MAX_PATH);
			}
		}
	}
	libraryfolders->deleteThis();
	if (aswrddir[0] == '\x00')
	{
		MessageBoxA(0, "Install Alien Swarm Reactive Drop to play this.", "Launcher error", MB_OK);
	}
	char gameParam[1024];
	V_strncpy(gameParam, "-steam -game \"", 1024);
	V_strncat(gameParam, g_szBasedir, 1024);
	V_AppendSlash(gameParam, 1024);
	V_strncat(gameParam, "alienbox\" ", 1024);
	V_strncat(gameParam, lpCmdLine, 1024);

	ShellExecuteA(0, "open", aswrddir, gameParam, steamDir, SW_SHOWDEFAULT);
	return 0;
}