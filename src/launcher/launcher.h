//xorusr
#include <windows.h>
#include <string.h>
#include <tlhelp32.h>
#include <iostream>

bool isProcessRunning(const std::string& processName) {
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
}
