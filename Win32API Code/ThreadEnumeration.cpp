//#include "stdafx.h"
//#include "CRC.h"
#define _CRT_SECURE_NO_WARNINGS //Enables the use of unsafe functions in the crt
#include <iostream>
#include <string>
#include <fstream>
#include <Windows.h>
#include <TlHelp32.h> 
#include <tchar.h>

using namespace std;

DWORD ThreadEnumeration(DWORD ProcThread);

int main()
{
    //Use the one in the comment if a process is already open
    ThreadEnumeration(GetCurrentProcessId());

    system("pause");
    return 0;
}

DWORD ThreadEnumeration(DWORD ProcThread)
{
    THREADENTRY32 te;
    HANDLE hndl;
    
    te.dwSize = sizeof(THREADENTRY32);
    hndl = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);

    if(Thread32First(hndl, &te))
    {
        while(Thread32Next(hndl, &te))
        {
            if(te.th32OwnerProcessID == ProcThread)
            {   cout << "here" << endl;
                //cout << te.th32ThreadID << endl;
                //cout << te.lpDeltaPri << endl;
                //cout << te.lpBasePri << endl;
                break;
            }
        }
    }
    cout << te.th32ThreadID << endl;
    
    return te.th32ThreadID;
}