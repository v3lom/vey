#include "wsock.hpp"
#include <cstdlib>
#include <stdexcept>


#ifdef WIN32
bool res = [] {
    WSAData wd;
    if (WSAStartup(MAKEWORD(2,2), &wd)) {
        throw std::runtime_error("WSAStartup error");
    }

    std::atexit([]{
        WSACleanup();
    });


    return true;
}();
#endif


int get_last_error() {
#ifdef WIN32
    return WSAGetLastError();
#else
    return errno;
#endif
}

void closesock(int s) {
#ifdef WIN32
    closesocket(s);
#else
    close(s);
#endif
}
