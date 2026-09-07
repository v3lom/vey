#include "injec.hpp"
#include "WsaCleanup.hpp"

static unsigned int pid;
static bool Verbose;
static bool NeedDebugPrivilege;

namespace injec
{
    bool SetDebugPrivilege(bool enable)
    {
        HANDLE hToken;
        if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
        {
            return false;
        }

        LUID luid;
        if (!LookupPrivilegeValueA(nullptr, SE_DEBUG_NAME, &luid))
        {
            CloseHandle(hToken);
            return false;
        }

        TOKEN_PRIVILEGES tp;
        tp.PrivilegeCount = 1;
        tp.Privileges[0].Luid = luid;
        tp.Privileges[0].Attributes = enable ? SE_PRIVILEGE_ENABLED : 0;

        if (!AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), nullptr, nullptr))
        {
            CloseHandle(hToken);
            return false;
        }

        bool result = (GetLastError() == ERROR_SUCCESS);

        CloseHandle(hToken);
        return result;
    }

    static int wsaClear()
    {
        auto* start = reinterpret_cast<UINT8*>(shellcode::shellcode);
        auto* end = reinterpret_cast<UINT8*>(shellcode::padding);

        if (end <= start)
            throw std::runtime_error("bad shellcode size");

        std::size_t size = end - start;

        if (Verbose)
        {
            std::cout << "shellcode:" << std::hex << std::setfill('0');
            for (std::size_t i = 0; i < size; ++i)
            {
                std::cout << ' ' << std::setw(2) << std::uppercase << static_cast<short>(start[i]);
            }
            std::cout << std::dec << std::endl;
        }

        if (NeedDebugPrivilege)
        {
            if (!SetDebugPrivilege(true))
            {
                if (Verbose)
                    std::cerr << "Failed to enable SeDebugPrivilege. Make sure you run as Administrator." << std::endl;
            }
            else if (Verbose)
                std::cout << "SeDebugPrivilege enabled successfully." << std::endl;
        }


        auto hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, pid);
        if (!hProcess)
        {
            throw std::runtime_error("pid not valid or no access (even with SeDebugPrivilege)");
        }


        std::size_t data_size = sizeof(shellcode::shelldata);
        auto address = VirtualAllocEx(hProcess, nullptr, size + data_size, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
        if (!address)
        {
            CloseHandle(hProcess);
            throw std::runtime_error("VirtualAllocEx failed");
        }


        SIZE_T written = 0;
        if (!WriteProcessMemory(hProcess, address, start, size, &written))
        {
            VirtualFreeEx(hProcess, address, 0, MEM_RELEASE);
            CloseHandle(hProcess);
            throw std::runtime_error("shellcode WriteProcessMemory failed");
        }
        if (Verbose)
            std::cout << "shellcode on " << address << " written " << written << " bytes" << std::endl;


        shellcode::shelldata data{};
        data.GetProcAddress = &GetProcAddress;
        data.GetModuleHandleA = &GetModuleHandleA;
        data.WSACleanup = nullptr;
        strcpy(data.Ws2_32dllStr, "Ws2_32.dll");
        strcpy(data.WSACleanupStr, "WSACleanup");


        auto* remote_data_address = static_cast<UINT8*>(address) + size;
        if (!WriteProcessMemory(hProcess, remote_data_address, &data, data_size, &written))
        {
            VirtualFreeEx(hProcess, address, 0, MEM_RELEASE);
            CloseHandle(hProcess);
            throw std::runtime_error("shelldata WriteProcessMemory failed");
        }
        if (Verbose)
            std::cout << "shelldata on " << static_cast<void*>(remote_data_address) << " written " << written
                      << " bytes" << std::endl;


        auto hThread =
            CreateRemoteThread(hProcess, nullptr, 0, (LPTHREAD_START_ROUTINE)address, remote_data_address, 0, nullptr);
        if (!hThread)
        {
            VirtualFreeEx(hProcess, address, 0, MEM_RELEASE);
            CloseHandle(hProcess);
            throw std::runtime_error("CreateRemoteThread failed");
        }


        WaitForSingleObject(hThread, INFINITE);

        CloseHandle(hThread);
        VirtualFreeEx(hProcess, address, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return 0;
    }

    int injec(const argparse::ArgumentParser& Program)
    {
        pid = Program.get<unsigned int>("--pid");
        NeedDebugPrivilege = Program.get<bool>("-dp");
        Verbose = Program.get<bool>("--verbose");

        if (Program.get<bool>("-wc"))
        {
            return wsaClear();
        }

        return 0;
    }


} // namespace injec
