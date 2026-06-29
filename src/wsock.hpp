#pragma once

#include <cstdio>
#include <cerrno>

#ifdef WIN32
#   include <winsock2.h>
#   include <ws2tcpip.h>
#else
#   include <sys/types.h>
#   include <sys/socket.h>
#   include <netinet/in.h>
#   include <arpa/inet.h>
#   include <unistd.h>
#   include <netdb.h>
#endif


int get_last_error();

#if WIN32
#   define print_error() ((void)0)
#else
#   define print_error() (perror(get_last_error()))
#endif