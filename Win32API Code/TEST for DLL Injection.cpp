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


DWORD GetProcessID(string Process);
void Virus();

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

void Virus()
{

    cout << "This process works" << endl;

}

//For easier readability, allocate all needed spaces first in order, then write to those spaces in order, then execute their remote threads in order
int main()
{   //Load the Kernel32.dll Library // Use the FreeLibrary() to free the library when done 
    HINSTANCE LoadDll = LoadLibrary("kernel32.dll");
    LPVOID LoadLibDll = (LPVOID) GetProcAddress(LoadDll, "LoadLibraryA");
    
    //Open the target Process
    //system("calc.exe");

    //Open the object of the process and override the setting to give full access to the process 
    HANDLE CalcProc = OpenProcess(
        PROCESS_ALL_ACCESS,
        FALSE,
        GetProcessID("Calculator.exe")
    );

    //Allocate memory in the regin of the memory page of the process to load the DLL 
    LPVOID AllocLibrary = VirtualAllocEx(
        CalcProc,
        NULL,
        MAX_PATH,
        MEM_COMMIT | MEM_RESERVE,
        PAGE_READWRITE
    );
    
    //Write the DLL module into the reserved address space of the target process
    WriteProcessMemory(
        CalcProc,
        AllocLibrary,
        LoadDll,
        sizeof(LoadDll) + 1,
        0
    );
    
    //Begin allocating and writing out any viruses to hook into the process
    LPVOID AllocCalc = VirtualAllocEx(
        CalcProc,
        NULL,
        MAX_PATH,
        MEM_COMMIT | MEM_RESERVE,
        PAGE_EXECUTE_READWRITE
    );

   WriteProcessMemory(
        CalcProc, 
        AllocCalc,
        (LPVOID)Virus,
        sizeof(MAX_PATH),
        0
    );
    //Execute the Remote Thread of the DLL FIRST!!!!
    HANDLE ExecuteDLL = CreateRemoteThread(
        CalcProc,
        0,
        0,
        (LPTHREAD_START_ROUTINE) LoadLibDll, //Starts in the address of the DLL
        AllocLibrary, //Located from the address of the Mem space
        0,
        0
    );
  
    if(!ExecuteDLL)
    { cout << "hello " << endl;}

    HANDLE DllInjection = CreateRemoteThread(
        CalcProc,
        0,
        0,
        (LPTHREAD_START_ROUTINE) AllocCalc,
        AllocLibrary,
        0,
        0
    );
  /**
   if( !VirtualFreeEx(
        CalcProc,
        AllocCalc,
        sizeof(NULL),
        MEM_RELEASE
    ))
    {
        cout << "VirtualFreeEx Not Working" << endl;
        DWORD test = GetLastError();

        cout << "ERROR CODE -> " << test << endl;

    }
**/
   

    //CloseHandle(CalcProc);
    system("pause");
    return 0;
}