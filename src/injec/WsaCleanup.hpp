#pragma once
#include <ws2tcpip.h>

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

    void shellcode();
    void padding();
} // namespace injec::shellcode
