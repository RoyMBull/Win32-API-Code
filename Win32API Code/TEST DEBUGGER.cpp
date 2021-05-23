//#include "stdafx.h"
//#include "CRC.h"
#define _CRT_SECURE_NO_WARNINGS //Enables the use of unsafe functions in the crt
#include <iostream>
#include <string>
#include <fstream>
#include <TlHelp32.h> 
#include <Windows.h>
#include <tchar.h>

using namespace std;

HANDLE CalcProc, CalcThread, EventThread;
CONTEXT Context, EventContext;

MEMORY_BASIC_INFORMATION mbi;
SYSTEM_INFO SysInfo;
size_t RegionSize = mbi.RegionSize;

DEBUG_EVENT StreamEvents;

ofstream MainTContxt, MainQueryContxt, MainExceptions, ExceptionContext;

HINSTANCE LoadDll;
LPVOID LoadLibDll;
LPVOID AllocLibrary;
HANDLE ExecuteDLL;


DWORD ThreadEnumeration(DWORD ProcThread);
DWORD GetProcessID(string Process);

void LoadDllModule();
void Injection();
void HardWareBreakPoint();
void BreakPointInit();
void LogEventContext();
void LogExceptions();
void LogMemPageInfo();
void MainThreadContext();
void MainQueryContext();
void SecureEvents();

void LoadDllModule()
{
    LoadDll = LoadLibrary("kernel32.dll");
    LoadLibDll = (LPVOID) GetProcAddress(LoadDll, "LoadLibraryA");

    AllocLibrary = VirtualAllocEx(
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
        sizeof(LoadDll),
        0
    );

    //Execute the Remote Thread of the DLL FIRST!!!!
    ExecuteDLL = CreateRemoteThread(
        CalcProc,
        0,
        0,
        (LPTHREAD_START_ROUTINE) LoadLibDll, //Starts in the address of the DLL
        AllocLibrary, //Located from the address of the Mem space
        0,
        0
    );

}

void Injection()
{
    LoadDllModule();
    LPVOID AllocateVirus = VirtualAllocEx(
        CalcProc,
        NULL,
        MAX_PATH, 
        MEM_COMMIT | MEM_RESERVE,
        PAGE_EXECUTE_READWRITE
    );

    WriteProcessMemory(
      CalcProc,
      AllocateVirus,
      (LPVOID)Virus,
      sizeof(MAX_PATH),
      0    
    );

    HANDLE Injection = CreateRemoteThread(
        CalcProc,
        0,
        0,
        (LPTHREAD_START_ROUTINE) AllocateVirus,
        AllocLibrary,
        0,
        0
    );

    CloseHandle(Injection);
    CloseHandle(ExecuteDLL);

    VirtualFreeEx(
        CalcProc,
        AllocateVirus,
        MEM_RELEASE
    );

    VirtualFreeEx(
        CalcProc,
        AllocLibrary,
        MEM_RELEASE
    );
}

void HardWareBreakPoint()
{
    string hd;
    cout << "Which thread will you be setting a hardwarebreakpoint for?" << endl;
    cout << "Reference the Thread Context file for the register context you plan to set" << endl;
    cout << "Enter event to run a hardwarebreakpoint reguarding any reported debugging events" << endl;
    cout << "Enter done to close the function" << endl;
    
    while(true)
    {
        cin >> hd;

        if(hd == "main")
        {
            cout << "Enter the value of the Register to set: dr0, dr1, dr2, or dr3" << endl;
            cout << "Check the EventThreadContext File to reference and check which of there registers are free" << endl;
            cout << "A free register will contain the value 0x00000000. You must make sure the register you use is free" << endl;
            cout << "Enter dr0 for the dr0 register" << endl;
            cout << "Enter dr1 for the dr1 register" << endl;
            cout << "Enter dr2 for the dr2 register" << endl;
            cout << "Enter dr3 for the dr3 register" << endl;

            DWORD SuspendMainThread = SuspendThread(CalcThread);

            Context.Dr7 = ((Context.Dr7) & 0xFFF3FFF) | 0x00030001;
           
            string RegEdit, ERegRedit;

            while(true)
            {
                DWORD RegAddress;
                cin >> RegEdit;

                if(RegEdit == "dr0")
                {
                    cout << "Enter the Address of the Breakpoint" << endl;
                    cin >> RegAddress;

                    Context.Dr0 = RegAddress;

                    bool SetBreakPoint = SetThreadContext(
                        CalcThread,
                        &Context
                    );

                    Context.Dr7 |= 1;
                    Context.Dr7 &= 0xFFF0FFFF;
                    Context.Dr6 = 0;
                    Context.Dr0 = 0x00000000;

                    bool ClearBreakPoint = SetThreadContext(
                        CalcThread,
                        &Context
                    );

                }
                
                else if(RegEdit == "dr1")
                {
                    cout << "Enter the Address of the Breakpoint" << endl;
                    cin >> RegAddress;

                    Context.Dr1 = RegAddress;

                    bool SetBreakPoint = SetThreadContext(
                        CalcThread,
                        &Context
                    );

                    Context.Dr7 |= 1;
                    Context.Dr7 &= 0xFFF0FFFF;
                    Context.Dr6 = 0;
                    Context.Dr1 = 0x00000000;

                    bool ClearBreakPoint = SetThreadContext(
                        CalcThread,
                        &Context
                    );

                }

                else if(RegEdit == "dr2")
                {
                    cout << "Enter the Address of the Breakpoint" << endl;
                    cin >> RegAddress;

                    Context.Dr2 = RegAddress;

                    bool SetBreakPoint = SetThreadContext(
                        CalcThread,
                        &Context
                    );

                    Context.Dr7 |= 1;
                    Context.Dr7 &= 0xFFF0FFFF;
                    Context.Dr6 = 0;
                    Context.Dr2 = 0x00000000;

                    bool ClearBreakPoint = SetThreadContext(
                        CalcThread,
                        &Context
                    );

                }

                else if(RegEdit == "dr3")
                {
                    cout << "Enter the Address of the Breakpoint" << endl;
                    cin >> RegAddress;

                    Context.Dr3 = RegAddress;

                    bool SetBreakPoint = SetThreadContext(
                        CalcThread,
                        &Context
                    );

                    Context.Dr7 |= 1;
                    Context.Dr7 &= 0xFFF0FFFF;
                    Context.Dr6 = 0;
                    Context.Dr3 = 0x00000000;

                    bool ClearBreakPoint = SetThreadContext(
                        CalcThread,
                        &Context
                    );

                }

                else if(RegEdit == "done")
                {
                    break;
                }

                else 
                {
                    cout << "Invalid Command" << endl;
                }
            }

            DWORD ResumeMainThread = ResumeThread(CalcThread);
            break;
        }

    }

    
}

void BreakPointInit()
{
    cout << "Which BreakPoint Would you like to perform" << endl;
    cout << "Check all files the program as launched thus far for reference" << endl;
    cout << "Enter hard for hard breakpoint" << endl;
    cout << "Enter inject for DLL/Code Injection" << endl;
    cout << "Enter done to close the function" << endl;

    string Bp;

    while(true)
    {
        cin >> Bp;

        else if(Bp == "hard")
        {
            HardWareBreakPoint();
        }

        else if(Bp == "inject")
        {
            Injection();
        }

        else if(Bp == "done")
        {
            break;
        }

        else
        {
            cout << "Invalid Command" << endl;
        }
    }
}

void LogEventContext()
{
    ExceptionContext.open("ExceptionContext.txt");

    ExceptionContext << "Debug Register Values" << endl;
    ExceptionContext << "" << endl;

    ExceptionContext << "Dr0: " << EventContext.Dr0 << endl;
    ExceptionContext << "Dr1: " << EventContext.Dr1 << endl;
    ExceptionContext << "Dr2: " << EventContext.Dr2 << endl;
    ExceptionContext << "Dr3: " << EventContext.Dr3 << endl;
    ExceptionContext << "Dr6: " << EventContext.Dr6 << endl;
    ExceptionContext << "Dr7: " << EventContext.Dr7 << endl;

    ExceptionContext << "Segment Register Values" << endl;
    ExceptionContext << "" << endl;

    ExceptionContext << "SegCs: " << EventContext.SegCs << endl;
    ExceptionContext << "SegDs: " << EventContext.SegDs << endl;
    ExceptionContext << "SegEs: " << EventContext.SegEs << endl;
    ExceptionContext << "SegFs: " << EventContext.SegFs << endl;
    ExceptionContext << "SegGs: " << EventContext.SegGs << endl;
    ExceptionContext << "SegSs: " << EventContext.SegSs << endl;

    ExceptionContext << "General Purpose Register Values" << endl;
    ExceptionContext << "" << endl;
    
    ExceptionContext << "Eax: " << EventContext.Eax << endl;
    ExceptionContext << "Ebx: " << EventContext.Ebx << endl;
    ExceptionContext << "Ecx: " << EventContext.Ecx << endl;
    ExceptionContext << "Esi: " << EventContext.Esi << endl;
    ExceptionContext << "Edi: " << EventContext.Edi << endl;
    ExceptionContext << "Esp: " << EventContext.Esp << endl;
    ExceptionContext << "Ebx: " << EventContext.Ebx << endl;

    ExceptionContext.close();
}

void LogExceptions()
{
    MainExceptions.open("MainExceptions.txt");
    MainExceptions << "" << endl;
    

}

void SecureEvents()
{
    WaitForDebugEvent(&StreamEvents, INFINITE);
    LogExceptions();

    EventThread = OpenThread(
        THREAD_ALL_ACCESS,
        FALSE,
        StreamEvents.dwThreadId
    );

    DWORD SuspendThread = SuspendThread(EventThread);

    EventContext.ContextFlags = CONTEXT_FULL | CONTEXT_DEBUG_REGISTERS;
    bool GrabEventContext = GetThreadContext(
        EventThread,
        &EventContext
    );

    LogEventContext();

    DWORD StreamConditions = StreamEvents.dwDebugEventCode;
    
    if(StreamEvents.dwDebugEventCode == EXCEPTION_DEBUG_EVENT)
    {
        MainExceptions << "Exception Code " << StreamEvents.u.Exception.ExceptionRecord.ExceptionCode << endl;
        MainExceptions << "Exception Address" << StreamEvents.u.Exception.ExceptionRecord.ExceptionAddress << endl;
    }

    else if(StreamConditions ==  EXCEPTION_ACCESS_VIOLATION)
    {
        MainExceptions << "Access Violation Detected" << endl;
    }

    else if(StreamConditions ==  EXCEPTION_GUARD_PAGE)
    {
        MainExceptions << "Guard Page Detected" << endl;
    }

    else if(StreamConditions ==  EXCEPTION_BREAKPOINT)
    {
        MainExceptions << "Exception Address" << StreamEvents.u.Exception.ExceptionRecord.ExceptionAddress << endl;
    }

    else if(StreamConditions == EXCEPTION_SINGLE_STEP)
    {
        MainExceptions << "Single Step Detected" << endl;
    }

    else 
    {
        cout << "Nothing Detected" << endl;
    }

    DWORD ResumeThread = ResumeThread(EventThread);

    DWORD ContinueStatus = DBG_CONTINUE;

    bool ContinueToDebug = ContinueDebugEvent(
        StreamEvents.dwProcessId,
        StreamEvents.dwThreadId,
        ContinueStatus
    );


}

void LogMemPageInfo()
{
    MainQueryContxt.open("MainThreadContext.txt");

    MainQueryContxt << "Memory Page Information about the Process" << endl;
    MainQueryContxt << "" << endl; 
    
    MainQueryContxt << "BaseAddress: " << mbi.BaseAddress << endl;
    MainQueryContxt << "Allocation Base: " << mbi.AllocationBase << endl;
    MainQueryContxt << "Allocation Protect: " << mbi.AllocationProtect << endl;
    MainQueryContxt << "Region Size: " << mbi.RegionSize << endl;
    MainQueryContxt << "State: " << mbi.State << endl;
    MainQueryContxt << "Protect: " << mbi.Protect << endl;
    MainQueryContxt << "Type" << mbi.Type << endl;

    MainTContxt.close();
}

void MainQueryContext()
{
    ZeroMemory(&SysInfo, sizeof(SYSTEM_INFO));
    GetSystemInfo(&SysInfo);

    DWORD DefaultPageSize = SysInfo.dwPageSize;

    MainTContxt << "Default Page Size: " << DefaultPageSize << endl;

    size_t MainQuery = VirtualQueryEx(
        CalcProc,
        (LPVOID)RegionSize,
        &mbi,
        0
    );

    bool ChangeProtect = VirtualProtectEx(
        CalcProc,
        mbi.BaseAddress,
        256,
        PAGE_GUARD,
        mbi.Protect
    );

}

void MainThreadContext()
{
    MainTContxt.open("MainThreadContext.txt");

    MainTContxt << "Debug Register Values" << endl;
    MainTContxt << "" << endl;

    MainTContxt << "Dr0: " << Context.Dr0 << endl;
    MainTContxt << "Dr1: " << Context.Dr1 << endl;
    MainTContxt << "Dr2: " << Context.Dr2 << endl;
    MainTContxt << "Dr3: " << Context.Dr3 << endl;
    MainTContxt << "Dr6: " << Context.Dr6 << endl;
    MainTContxt << "Dr7: " << Context.Dr7 << endl;

    MainTContxt << "Segment Register Values" << endl;
    MainTContxt << "" << endl;

    MainTContxt << "SegCs: " << Context.SegCs << endl;
    MainTContxt << "SegDs: " << Context.SegDs << endl;
    MainTContxt << "SegEs: " << Context.SegEs << endl;
    MainTContxt << "SegFs: " << Context.SegFs << endl;
    MainTContxt << "SegGs: " << Context.SegGs << endl;
    MainTContxt << "SegSs: " << Context.SegSs << endl;

    MainTContxt << "General Purpose Register Values" << endl;
    MainTContxt << "" << endl;
    
    MainTContxt << "Eax: " << Context.Eax << endl;
    MainTContxt << "Ebx: " << Context.Ebx << endl;
    MainTContxt << "Ecx: " << Context.Ecx << endl;
    MainTContxt << "Esi: " << Context.Esi << endl;
    MainTContxt << "Edi: " << Context.Edi << endl;
    MainTContxt << "Esp: " << Context.Esp << endl;
    MainTContxt << "Ebx: " << Context.Ebx << endl;

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

int main()
{   
    system("calc.exe");

    CalcProc = OpenProcess(
        PROCESS_ALL_ACCESS,
        FALSE,
        GetProcessID("Calculator.exe");
    )

    CalcThread = OpenThread(
        THREAD_ALL_ACCESS,
        FALSE,
        ThreadEnumeration(ProcThread)
    );

    DWORD SuspendCalcT = SuspendThread(
        CalcThread
    );

    Context.ContextFlags = CONTEXT_FULL | CONTEXT_DEBUG_REGISTERS;
    bool CalcContext = GetThreadContext(
        CalcThread,
        &Context
    );

    MainThreadContext();
    MainQueryContext();
    SecureEvents();

    DWORD ResumeCalcT = ResumeThread(
        CalcThread
    );

    BreakPointInit();

    CloseHandle(EventThread);
    CloseHandle(CalcThread);
    CloseHandle(CalcProc);
    MainTContxt.close();
    MainExceptions.close();
    system("pause");
    return 0;
}


