#pragma once
#include <string>
#include "Hooks.hpp"

namespace Hooking
{
    void __stdcall Entry(HMODULE Handle);
    PIMAGE_THUNK_DATA GetIATEntry(const std::string& EntryName);
    ULONGLONG CreateHook(const std::string& IATName, ULONG_PTR HookedFunction);
}