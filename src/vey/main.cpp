#include "inet.hpp"
#include "injec.hpp"

int main(int argc, char* argv[])
{
    argparse::ArgumentParser Program(VEY_PROJECT_NAME, VEY_VERSION);

    // INET
    argparse::ArgumentParser InetCmd("inet");
    Program.add_subparser(InetCmd);
    InetCmd.add_argument("--verbose").help("enable detailed logging / verbose output").flag();
    InetCmd.add_argument("ip").help("target IP address (IPv4 or IPv6)");
    InetCmd.add_argument("port").help("target port number (1-65535)");
    InetCmd.add_argument("-rc", "--reconnect-count")
        .help("maximum number of reconnection attempts [default: no reconnect]")
        .scan<'u', unsigned long long>();
    InetCmd.add_argument("-dw", "--delay-ms-per-write")
        .help("delay between consecutive write operations in milliseconds [default: no delay]")
        .scan<'u', unsigned long long>();

    InetCmd.add_argument("-s", "--socket-count")
        .help("number of concurrent sockets to use")
        .scan<'u', unsigned long long>()
        .default_value(static_cast<unsigned long long>(1));

    InetCmd.add_argument("-p", "--packet-size")
        .help("packet size in bytes")
        .scan<'u', unsigned long long>()
        .default_value(static_cast<unsigned long long>(1024));

    auto& Protocol = InetCmd.add_mutually_exclusive_group(true);
    Protocol.add_argument("-t", "--tcp").help("use TCP protocol").flag();
    Protocol.add_argument("-u", "--udp").help("use UDP protocol").flag();

    auto& IpVersion = InetCmd.add_mutually_exclusive_group(false);
    IpVersion.add_argument("-v4", "--ipv4").help("force IPv4 addressing").flag();
    IpVersion.add_argument("-v6", "--ipv6").help("force IPv6 addressing").flag();


    // INJEC TODO()
#ifdef WIN32
    argparse::ArgumentParser InjecCmd("injec");
    Program.add_subparser(InjecCmd);
    auto& Process = InjecCmd.add_mutually_exclusive_group(true);
    InjecCmd.add_argument("--verbose").help("enable detailed logging / verbose output").flag();
    Process.add_argument("-p", "--pid").help("process pid").scan<'u', unsigned int>();
    // Process.add_argument("-n", "--name").help("process name");

    auto& Veyon = InjecCmd.add_mutually_exclusive_group(true);
    Veyon.add_argument("-wc", "--wsa-cleanup").help("try by shellcode clean wsa").flag();
    Veyon.add_argument("-idll", "--inject-dll").help("try inject dll to target process");
#endif


    try
    {
        Program.parse_args(argc, argv);

        if (Program.is_subcommand_used("inet"))
        {
            return inet::inet(InetCmd);
        }
#ifdef WIN32
        else if (Program.is_subcommand_used("injec"))
        {
            return injec::injec(InjecCmd);
        }
#endif
    }
    catch (const std::exception& err)
    {
        std::cerr << err.what() << std::endl;
        std::cerr << Program;
        return 1;
    }


    return 0;
}
