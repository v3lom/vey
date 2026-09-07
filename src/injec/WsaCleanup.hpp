#pragma once
#include <ws2tcpip.h>
#include <windows.h>

namespace injec::shellcode
{
    struct shelldata
    {
        decltype(&GetProcAddress) GetProcAddress;
        decltype(&GetModuleHandleA) GetModuleHandleA;
        decltype(&WSACleanup) WSACleanup;
        char Ws2_32dllStr[16];
        char WSACleanupStr[16];
    };

    int shellcode(shelldata* data);
    void padding();
} // namespace injec::shellcode
