#include "Socket.hpp"

Socket::Socket() {
	fd = -1;
	sockaddrIn = sockaddr_in();
	sockLen = socklen_t();
}

Socket::Socket(const Socket &ref) {
	this->fd = ref.fd;
	this->sockaddrIn = ref.sockaddrIn;
	this->sockLen = ref.sockLen;
}

Socket &Socket::operator=(const Socket &ref) {
	if (this != &ref)
	{
		fd = ref.fd;
		sockaddrIn = ref.sockaddrIn;
		sockLen = ref.sockLen;
	}
	return *this;
}

Socket::Socket(int xfd, struct sockaddr_in &sockaddrIn, socklen_t &xlen) {
	this->fd = xfd;
	this->sockaddrIn = sockaddrIn;
	this->sockLen = xlen;
}

Socket::~Socket() {

}

int Socket::getFd()
{
	return fd;
}

sockaddr_in &Socket::getAddress()
{
	return sockaddrIn;
}

socklen_t Socket::getLen()
{
	return sockLen;
}

//void Socket::setSockLen(socklen_t sockLen)
//{
//	Socket::sockLen = sockLen;
//}
