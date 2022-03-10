#pragma once


#include <cstring>
#include <stdexcept>
#include <iostream>
#include <map>
#include <cerrno>
#include <vector>
#include <set>
#include <queue>
#include <unistd.h>
#include <cstdlib>
#include <algorithm>
#include <sys/socket.h>
#include <fcntl.h>
#include <ctime>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/poll.h>
#include <sys/event.h>
#include <csignal>
#include <sstream>
#include "fstream"

class Socket {
private:

public:
	Socket();
	Socket(const Socket &ref);
	Socket& operator=(const Socket &ref);
	Socket(int xfd, struct sockaddr_in&, socklen_t&);
	virtual	~Socket();

	int					fd;
	struct sockaddr_in	sockaddrIn;
	socklen_t			sockLen;

	int getFd();
	sockaddr_in &getAddress();
	socklen_t getLen();


};