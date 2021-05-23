//#include "stdafx.h"
#include <iostream>
#include <string>
#include <fstream>
//#include <T1Help32>
#include <Windows.h>
//#include <tchar>
using namespace std;

int main()
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    
    ZeroMemory(&si, sizeof(si));
    ZeroMemory(&pi, sizeof(pi));
    si.cb = sizeof(si);

    CreateProcess(
        "C:\\WINDOWS\\system32\\calc.exe",
        NULL,
        NULL,
        NULL,
        FALSE, 
        0,
        NULL,
        NULL,
        &si,
        &pi
    );    
        
    WaitForSingleObject(pi.hProcess, INFINITE); 
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    system("pause");
    return 0;
}