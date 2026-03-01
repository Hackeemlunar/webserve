#include "../include/Socket.hpp"
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>

// Orthodox Canonical Form
Socket::Socket() : _fd(-1), _port(0), _host("0.0.0.0"), _isListening(false) {
	std::memset(&_address, 0, sizeof(_address));
}

Socket::Socket(const std::string& host, int port) : _fd(-1), _port(port), _host(host), _isListening(false) {
	std::memset(&_address, 0, sizeof(_address));
}

Socket::Socket(const Socket& other) {
	*this = other;
}

Socket& Socket::operator=(const Socket& other) {
	if (this != &other) {
		_fd = other._fd;
		_port = other._port;
		_host = other._host;
		_address = other._address;
		_isListening = other._isListening;
	}
	return *this;
}

Socket::~Socket() {
	if (_fd != -1)
		close();
}

// Socket operations
bool Socket::create() {
	// TODO: Implementation
	return false;
}

bool Socket::bind() {
	// TODO: Implementation
	return false;
}

bool Socket::listen(int backlog) {
	// TODO: Implementation
	(void)backlog;
	return false;
}

int Socket::accept() {
	// TODO: Implementation
	return -1;
}

void Socket::close() {
	if (_fd != -1) {
		::close(_fd);
		_fd = -1;
	}
}

// Private helper methods
void Socket::setNonBlocking() {
	// TODO: Implementation
}

void Socket::setSocketOptions() {
	// TODO: Implementation
}

// Getters
int Socket::getFd() const {
	return _fd;
}

int Socket::getPort() const {
	return _port;
}

const std::string& Socket::getHost() const {
	return _host;
}

bool Socket::isListening() const {
	return _isListening;
}

struct sockaddr_in Socket::getAddress() const {
	return _address;
}

// Setters
void Socket::setFd(int fd) {
	_fd = fd;
}

void Socket::setPort(int port) {
	_port = port;
}

void Socket::setHost(const std::string& host) {
	_host = host;
}
