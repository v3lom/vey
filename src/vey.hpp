#pragma once
#include <iostream>

#include "wsock.hpp"

inline int create_socket(const char *ip, const char *port, int sock_type) {
    addrinfo *ai = nullptr;
    addrinfo hints{};
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = sock_type;
    hints.ai_flags = AI_CANONNAME;


    int res = getaddrinfo(ip, port, &hints, &ai);
    if (res != 0) {
        std::cerr << "getaddrinfo error: " << res << "\n" << gai_strerror(res) << std::endl;
        return -1;
    }

    int sock = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
    if (sock == -1) {
        std::cerr << "socket error: " << get_last_error() << std::endl;
        print_error();
        freeaddrinfo(ai);
        return -1;
    }

    res = connect(sock, ai->ai_addr, ai->ai_addrlen);
    if (res == -1) {
        std::cerr << "connect error: " << get_last_error() << std::endl;
        print_error();
        closesock(sock);
        freeaddrinfo(ai);
        return -1;
    }
    std::cout << "connected to " << ai->ai_canonname << " [" << ip << ":" << port << "]"  << std::endl;
    freeaddrinfo(ai);


    return sock;
}
