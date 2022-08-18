#include "Hooking.hpp"

namespace Hooking
{
    void __stdcall Entry(HMODULE Handle)
    {
        AllocConsole();
        FILE* OutputBuffer = nullptr;
        freopen_s(&OutputBuffer, "CONOUT$", "w", stdout);
        
        Hooks::OriginalCreateFileW = reinterpret_cast<FnCreateFileW>(CreateHook("CreateFileW", (ULONG_PTR)Hooks::HookedCreateFileW));
        Hooks::OriginalReadFile = reinterpret_cast<FnReadFile>(CreateHook("ReadFile", (ULONG_PTR)Hooks::HookedReadFile));

        while(!GetAsyncKeyState(VK_END) & 1)
        {
            Sleep(1000);
        }

        if(OutputBuffer != nullptr)
        {
            fclose(OutputBuffer);
        }

        // Remove Hooks & Free Library
        CreateHook("CreateFileW", (ULONG_PTR)Hooks::OriginalCreateFileW);
        CreateHook("ReadFile", (ULONG_PTR)Hooks::OriginalReadFile);
        FreeConsole();
        FreeLibraryAndExitThread(Handle, 0);
    }

    ULONGLONG CreateHook(const std::string& EntryName, ULONG_PTR HookedFunction)
    {
        const PIMAGE_THUNK_DATA Entry = GetIATEntry(EntryName);

        DWORD OldProtect = NULL;
        DWORD NewProtect = PAGE_READWRITE;
        VirtualProtect(&Entry->u1.Function, sizeof(ULONGLONG), NewProtect, &OldProtect);

        ULONGLONG OriginalAddress = Entry->u1.Function;
        Entry->u1.Function = HookedFunction;

        VirtualProtect(&Entry->u1.Function, sizeof(ULONGLONG), OldProtect, &NewProtect);

        return OriginalAddress;
    }

    PIMAGE_THUNK_DATA GetIATEntry(const std::string& EntryName)
    {
        std::uintptr_t BaseAddress = reinterpret_cast<std::uintptr_t>(GetModuleHandle(NULL));
        PIMAGE_DOS_HEADER DOSHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(BaseAddress);
        PIMAGE_NT_HEADERS NTHeader = reinterpret_cast<PIMAGE_NT_HEADERS>(BaseAddress + DOSHeader->e_lfanew);
        IMAGE_DATA_DIRECTORY ImportsDirectory = NTHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
        PIMAGE_IMPORT_DESCRIPTOR ImportsDescriptor = reinterpret_cast<PIMAGE_IMPORT_DESCRIPTOR>(BaseAddress + ImportsDirectory.VirtualAddress);

        for(; ImportsDescriptor->Name; ImportsDescriptor++)
        {
            const char* LibraryName = reinterpret_cast<const char*>(BaseAddress + ImportsDescriptor->Name);
            HMODULE Library = LoadLibraryA(LibraryName);
            if(Library)
            {
                PIMAGE_THUNK_DATA OriginalThunk = reinterpret_cast<PIMAGE_THUNK_DATA>(BaseAddress + ImportsDescriptor->OriginalFirstThunk);
                PIMAGE_THUNK_DATA FirstThunk = reinterpret_cast<PIMAGE_THUNK_DATA>(BaseAddress + ImportsDescriptor->FirstThunk);
                
                for(; OriginalThunk->u1.AddressOfData; OriginalThunk++, FirstThunk++)
                {
                    const char* MethodName = reinterpret_cast<PIMAGE_IMPORT_BY_NAME>(BaseAddress + OriginalThunk->u1.AddressOfData)->Name;
                    if(!std::string(MethodName).compare(EntryName))
                    {
                        return FirstThunk;
                    }
                }
            }
        }

        return PIMAGE_THUNK_DATA{};
    }
}