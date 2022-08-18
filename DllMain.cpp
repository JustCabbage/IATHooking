#include "Hooking/Hooking.hpp"

BOOL __stdcall DllMain(HMODULE Handle, DWORD Reason, LPVOID Reserved)
{
    switch(Reason)
    {
        case DLL_PROCESS_ATTACH:
        {
            HANDLE ThreadHandle = CreateThread(0, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(Hooking::Entry), Handle, 0, 0);
            if(ThreadHandle)
            {
                CloseHandle(ThreadHandle);
            }
            break;
        }
    }
    return TRUE;
}