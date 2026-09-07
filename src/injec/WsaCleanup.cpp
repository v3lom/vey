#include "WsaCleanup.hpp"


namespace injec::shellcode
{
    int shellcode(shelldata* data)
    {
        if (!data)
            return 1;

        HMODULE dll = data->GetModuleHandleA(data->Ws2_32dllStr);
        if (!dll)
            return 2;

        data->WSACleanup = reinterpret_cast<decltype(data->WSACleanup)>(data->GetProcAddress(dll, data->WSACleanupStr));
        if (!data->WSACleanup)
            return 3;

        while (data->WSACleanup() == 0)
        {
        }

        return 0;
    }

    void padding() {}

} // namespace injec::shellcode
