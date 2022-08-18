#include "Hooking.hpp"

namespace Hooking
{
    void __stdcall Entry(HMODULE Handle)
    {
        // Test entry point
        MessageBox(NULL, "Testing", "Success", MB_OK);
    }
}