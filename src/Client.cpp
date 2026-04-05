#include "../include/Client.hpp"
#include "../include/RequestHandler.hpp"
#include <unistd.h>
#include <ctime>
#include <arpa/inet.h>
#include <cerrno>

// Orthodox Canonical Form
Client::Client() : _fd(-1), _keepAlive(true), _lastActivity(std::time(NULL)), _serverConfig(NULL) {
}

Client::Client(int fd, const struct sockaddr_in& address) : _fd(fd), _address(address), _keepAlive(true), _lastActivity(std::time(NULL)), _serverConfig(NULL) {
}

Client::Client(const Client& other) {
	*this = other;
}

Client& Client::operator=(const Client& other) {
	if (this != &other) {
		_fd = other._fd;
		_address = other._address;
		_request = other._request;
		_response = other._response;
		_readBuffer = other._readBuffer;
		_writeBuffer = other._writeBuffer;
		_keepAlive = other._keepAlive;
		_lastActivity = other._lastActivity;
		_serverConfig = other._serverConfig;
	}
	return *this;
}

Client::~Client() {
}

// Client operations
ssize_t Client::read() {
	char buffer[8192];
	ssize_t bytes = ::recv(_fd, buffer, sizeof(buffer), 0);
	if (bytes > 0) {
		appendReadBuffer(std::string(buffer, static_cast<size_t>(bytes)));
		_request.appendData(std::string(buffer, static_cast<size_t>(bytes)));
		updateLastActivity();
		return bytes;
	}
	if (bytes == 0)
		return 0;
	if (errno == EAGAIN || errno == EWOULDBLOCK)
		return 1;
	return -1;
}

ssize_t Client::write() {
	if (_writeBuffer.empty())
		return 0;

	ssize_t bytes = ::send(_fd, _writeBuffer.c_str(), _writeBuffer.size(), 0);
	if (bytes > 0) {
		_writeBuffer.erase(0, static_cast<size_t>(bytes));
		updateLastActivity();
		return bytes;
	}
	if (bytes == 0)
		return 0;
	if (errno == EAGAIN || errno == EWOULDBLOCK)
		return 1;
	return -1;
}

void Client::processRequest() {
	if (_serverConfig == NULL)
		return;

	_response.clear();
	RequestHandler handler(_request, _response, *_serverConfig);
	handler.handle();
	setKeepAlive(_request.keepAlive());
	if (_keepAlive)
		_response.addHeader("Connection", "keep-alive");
	else
		_response.addHeader("Connection", "close");
}

void Client::prepareResponse() {
	_writeBuffer = _response.build();
}

bool Client::isReadComplete() const {
	return _request.isComplete();
}

bool Client::isWriteComplete() const {
	return _writeBuffer.empty();
}

void Client::close() {
	if (_fd != -1) {
		::close(_fd);
		_fd = -1;
	}
}

// Private helper methods
void Client::updateLastActivity() {
	_lastActivity = std::time(NULL);
}

// Getters
int Client::getFd() const {
	return _fd;
}

HttpRequest& Client::getRequest() {
	return _request;
}

HttpResponse& Client::getResponse() {
	return _response;
}

const std::string& Client::getReadBuffer() const {
	return _readBuffer;
}

const std::string& Client::getWriteBuffer() const {
	return _writeBuffer;
}

bool Client::getKeepAlive() const {
	return _keepAlive;
}

time_t Client::getLastActivity() const {
	return _lastActivity;
}

ServerConfig* Client::getServerConfig() const {
	return _serverConfig;
}

// Setters
void Client::setFd(int fd) {
	_fd = fd;
}

void Client::setServerConfig(ServerConfig* config) {
	_serverConfig = config;
}

void Client::setKeepAlive(bool keepAlive) {
	_keepAlive = keepAlive;
}

// Buffer operations
void Client::appendReadBuffer(const std::string& data) {
	_readBuffer.append(data);
}

void Client::appendWriteBuffer(const std::string& data) {
	_writeBuffer.append(data);
}

void Client::clearReadBuffer() {
	_readBuffer.clear();
}

void Client::clearWriteBuffer() {
	_writeBuffer.clear();
}

// Utility methods
bool Client::isTimeout(time_t currentTime, time_t timeout) const {
	return (currentTime - _lastActivity) > timeout;
}

std::string Client::getClientIp() const {
	char ip[INET_ADDRSTRLEN];
	if (::inet_ntop(AF_INET, &_address.sin_addr, ip, sizeof(ip)) == NULL)
		return "0.0.0.0";
	return std::string(ip);
}
