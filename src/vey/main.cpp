#include "inet.hpp"

int main(int argc, char* argv[])
{
    argparse::ArgumentParser Program(VEY_PROJECT_NAME, VEY_VERSION);

    // INET
    argparse::ArgumentParser InetCmd("inet");
    InetCmd.add_argument("--verbose").help("increase output verbosity").flag();
    InetCmd.add_argument("ip").help("target ip address");
    InetCmd.add_argument("port").help("target port");

    InetCmd.add_argument("-s", "--socket-count")
        .help("count socket to usage")
        .scan<'u', unsigned long long>()
        .default_value(static_cast<unsigned long long>(1));

    InetCmd.add_argument("-p", "--packet-size")
        .help("packet size bytes")
        .scan<'u', unsigned long long>()
        .default_value(static_cast<unsigned long long>(1024));

    auto& Protocol = InetCmd.add_mutually_exclusive_group(true);
    Protocol.add_argument("-t", "--tcp").help("target protocol").flag();
    Protocol.add_argument("-u", "--udp").help("target protocol").flag();

    auto& IpVersion = InetCmd.add_mutually_exclusive_group(false);
    IpVersion.add_argument("-v4", "--ipv4").help("target ip address version").flag();
    IpVersion.add_argument("-v6", "--ipv6").help("target ip address version").flag();

    // INJEC
    argparse::ArgumentParser InjecCmd("injec");
    auto& Process = InjecCmd.add_mutually_exclusive_group(true);
    Process.add_argument("-n", "--name").help("process name");
    Process.add_argument("-P", "--pid").help("process pid").scan<'u', unsigned int>();


    Program.add_subparser(InetCmd);
    Program.add_subparser(InjecCmd);

    try
    {
        Program.parse_args(argc, argv);

        if (Program.is_subcommand_used("inet"))
        {
            inet::inet(InetCmd);
        }
        else if (Program.is_subcommand_used("injec"))
        {
        }
    }
    catch (const std::exception& err)
    {
        std::cerr << err.what() << std::endl;
        std::cerr << Program;
        return 1;
    }


    return 0;
}
