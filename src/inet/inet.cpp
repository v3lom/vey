#include "inet.hpp"
#include <boost/asio.hpp>
#include <utility>


static bool Verbose;

namespace
{
    template <typename SockType, typename EndPointType>
    class Sender
    {
    public:
        Sender(boost::asio::io_context& Io, EndPointType Endpoint) : Io(Io), Sock(Io), Endpoint(std::move(Endpoint)) {}
        void connect(const std::vector<char>& Buff)
        {
            Sock.async_connect(Endpoint,
                               [this, &Buff](const boost::system::error_code& Er)
                               {
                                   if (Er)
                                   {
                                       std::cerr << "Connect error for " << Endpoint << ": " << Er.message()
                                                 << std::endl;


                                       if (Er == boost::asio::error::connection_refused)
                                       {
                                           std::cerr << "Reconnecting..." << std::endl;
                                           connect(Buff);
                                       }

                                       return;
                                   }

                                   std::cout << "Successfully connected to " << Endpoint << std::endl;
                                   write(Buff);
                               });
        }

        void write(const std::vector<char>& Buff)
        {
            Sock.async_send(boost::asio::buffer(Buff),
                            [this, &Buff](const boost::system::error_code& Er, const std::size_t Size)
                            {
                                if (Er)
                                {
                                    std::cerr << "Error for " << Endpoint << ": " << Er.message() << std::endl;
                                    Sock.close();
                                    connect(Buff);
                                    return;
                                }

                                if (Verbose)
                                    std::cout << "Sent: " << Size << " bytes" << std::endl;

                                write(Buff);
                            });
        }

    private:
        boost::asio::io_context& Io;
        SockType Sock;
        EndPointType Endpoint;
    };

} // namespace

namespace inet
{
    static auto getNeedEndpoint(const auto& Endpoints, bool is_v4, bool is_v6)
    {
        for (const auto& Endpoint : Endpoints)
        {
            if (is_v4 && Endpoint.endpoint().address().is_v4())
                return Endpoint;
            else if (is_v6 && Endpoint.endpoint().address().is_v6())
                return Endpoint;
            else if (!is_v4 && !is_v6)
                return Endpoint;
        }

        throw std::runtime_error("Resolver not found target");
    }

    static int tcpva(const argparse::ArgumentParser& Program, bool is_v4, bool is_v6)
    {
        boost::asio::io_context Io;

        boost::asio::ip::tcp::resolver Resolver(Io);
        boost::asio::ip::tcp::resolver::results_type Endpoints =
            Resolver.resolve(Program.get("ip"), Program.get("port"));
        boost::asio::ip::tcp::resolver::endpoint_type Endpoint = getNeedEndpoint(Endpoints, is_v4, is_v6);

        std::vector Buffer(Program.get<unsigned long long>("-p"), 'X');
        std::vector<Sender<boost::asio::ip::tcp::socket, boost::asio::ip::tcp::resolver::endpoint_type>> Clients;
        Clients.reserve(Program.get<unsigned long long>("-s"));


        for (int i = 0; i < Clients.capacity(); ++i)
        {
            Clients.emplace_back(Io, Endpoint).connect(Buffer);
        }

        Io.run();
        return 0;
    }

    static int udpva(const argparse::ArgumentParser& Program, bool is_v4, bool is_v6)
    {
        boost::asio::io_context Io;

        boost::asio::ip::udp::resolver Resolver(Io);
        boost::asio::ip::udp::resolver::results_type Endpoints =
            Resolver.resolve(Program.get("ip"), Program.get("port"));
        boost::asio::ip::udp::resolver::endpoint_type Endpoint = getNeedEndpoint(Endpoints, is_v4, is_v6);

        std::vector Buffer(Program.get<unsigned long long>("-p"), 'X');
        std::vector<Sender<boost::asio::ip::udp::socket, boost::asio::ip::udp::resolver::endpoint_type>> Clients;
        Clients.reserve(Program.get<unsigned long long>("-s"));


        for (int i = 0; i < Clients.capacity(); ++i)
        {
            Clients.emplace_back(Io, Endpoint).connect(Buffer);
        }

        Io.run();
        return 0;
    }


    int inet(const argparse::ArgumentParser& Program)
    {
        bool is_v4 = Program.get<bool>("-v4");
        bool is_v6 = Program.get<bool>("-v6");
        bool is_tcp = Program.get<bool>("-t");
        bool is_udp = Program.get<bool>("-u");
        Verbose = Program.get<bool>("--verbose");

        if (is_tcp)
            return tcpva(Program, is_v4, is_v6);
        if (is_udp)
            return udpva(Program, is_v4, is_v6);

        std::cerr << Program;
        return 1;
    }
} // namespace inet
