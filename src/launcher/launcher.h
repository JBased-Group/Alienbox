//xorusr
#include <windows.h>
#include <string.h>
#include <tlhelp32.h>
#include <iostream>

// old shit
/*bool isProcessRunning(const std::string& processName) {
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(entry);

    if (Process32First(snapshot, &entry)) {
        do {
            if (std::string(entry.szExeFile) == processName) {
                CloseHandle(snapshot);
                return true;
            }
        } while (Process32Next(snapshot, &entry));
    }// sum fuk

    CloseHandle(snapshot);
    return false;
}*/
#if !defined _DEBUG
bool isProcessRunning(const std::string& processName)
{
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE)
    {
        return false;
    }
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);
        
    if (Process32First (snapshot,&entry))
    {
        do {
            if (processName == entry.szExeFile)
            {
                CloseHandle(snapshot);
                return true;
            }
        } while (Process32Next(snapshot, &entry));
    }
    CloseHandle(snapshot);
    return false;
}
#endif