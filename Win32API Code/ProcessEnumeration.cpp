//#include "stdafx.h"
#define _CRT_SECURE_NO_WARNINGS //Enables the use of unsafe functions in the crt
#include <iostream>
#include <string>
#include <fstream>
#include <Windows.h>
#include <TlHelp32.h> 
#include <tchar.h>
using namespace std;
 
DWORD GetProcessID(string Process);

 int main()
{   
    GetProcessID("Calculator.exe");

    system("pause");
    return 0;
}

DWORD GetProcessID(string Process)
{   
    PROCESSENTRY32 pi;
    HANDLE wnhndl;
    
    pi.dwSize = sizeof(PROCESSENTRY32);
    wnhndl = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (Process32First(wnhndl, &pi))
    {  
        while(Process32Next(wnhndl, &pi))
        {
            if (pi.szExeFile == Process)
            {   cout << "Match" << endl;
                 break;
            }
        }
        
    }

    cout << pi.th32ProcessID << endl;

    
        return pi.th32ProcessID;
}

