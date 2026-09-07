#include "inet.hpp"
#include <boost/asio.hpp>
#include <utility>


static bool Verbose;
static bool is_v4;
static bool is_v6;
static bool is_tcp;
static bool is_udp;
static std::pair<bool, unsigned long long> reconnectCount;
static std::pair<bool, unsigned long long> writeDelay;

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

                                       if (reconnectCount.first)
                                       {
                                           if (reconnectCount.second == count++)
                                               return;
                                       }

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

                                if (writeDelay.first)
                                {
                                    std::this_thread::sleep_for(std::chrono::milliseconds(writeDelay.second));
                                }

                                write(Buff);
                            });
        }

    private:
        boost::asio::io_context& Io;
        SockType Sock;
        EndPointType Endpoint;
        unsigned long long count = 0;
    };

} // namespace

namespace inet
{
    static auto getNeedEndpoint(const auto& Endpoints)
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

    static int tcpva(const argparse::ArgumentParser& Program)
    {
        boost::asio::io_context Io;

        boost::asio::ip::tcp::resolver Resolver(Io);
        boost::asio::ip::tcp::resolver::results_type Endpoints =
            Resolver.resolve(Program.get("ip"), Program.get("port"));
        boost::asio::ip::tcp::resolver::endpoint_type Endpoint = getNeedEndpoint(Endpoints);

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

    static int udpva(const argparse::ArgumentParser& Program)
    {
        boost::asio::io_context Io;

        boost::asio::ip::udp::resolver Resolver(Io);
        boost::asio::ip::udp::resolver::results_type Endpoints =
            Resolver.resolve(Program.get("ip"), Program.get("port"));
        boost::asio::ip::udp::resolver::endpoint_type Endpoint = getNeedEndpoint(Endpoints);

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
        is_v4 = Program.get<bool>("-v4");
        is_v6 = Program.get<bool>("-v6");
        is_tcp = Program.get<bool>("-t");
        is_udp = Program.get<bool>("-u");
        Verbose = Program.get<bool>("--verbose");
        reconnectCount.first = Program.present<unsigned long long>("-rc").has_value();
        writeDelay.first = Program.present<unsigned long long>("-dw").has_value();

        if (reconnectCount.first)
            reconnectCount.second = Program.get<unsigned long long>("-rc");

        if (writeDelay.first)
            writeDelay.second = Program.get<unsigned long long>("-dw");


        if (is_tcp)
            return tcpva(Program);
        if (is_udp)
            return udpva(Program);

        std::cerr << Program;
        return 1;
    }
} // namespace inet
