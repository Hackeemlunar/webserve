#include "../include/Client.hpp"
#include "../include/RequestHandler.hpp"
#include "../include/SessionMiddleware.hpp"
#include "../include/Logger.hpp"
#include <unistd.h>
#include <ctime>
#include <sstream>
#include <arpa/inet.h>

namespace {
	static std::string ipToString(const struct sockaddr_in& address) {
		unsigned long ip = ntohl(address.sin_addr.s_addr);
		std::ostringstream oss;
		oss << ((ip >> 24) & 0xFF) << '.' << ((ip >> 16) & 0xFF) << '.'
			<< ((ip >> 8) & 0xFF) << '.' << (ip & 0xFF);
		return oss.str();
	}
}


// Orthodox Canonical Form
Client::Client() : _fd(-1), _writeOffset(0), _bodyOffset(0), _continueSent(false), _keepAlive(true), _lastActivity(std::time(NULL)), _serverConfig(NULL), _cgi(NULL), _bodyFd(-1), _bodyFileSize(0), _bodyFileSent(0) {
}

Client::Client(int fd, const struct sockaddr_in& address) : _fd(fd), _address(address), _writeOffset(0), _bodyOffset(0), _continueSent(false), _keepAlive(true), _lastActivity(std::time(NULL)), _serverConfig(NULL), _cgi(NULL), _bodyFd(-1), _bodyFileSize(0), _bodyFileSent(0) {
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
		_writeOffset = other._writeOffset;
		_bodyOffset = other._bodyOffset;
		_continueSent = other._continueSent;
		_keepAlive = other._keepAlive;
		_lastActivity = other._lastActivity;
		_serverConfig = other._serverConfig;
		_cgi = NULL;
		_bodyFd = -1;
		_bodyFileSize = 0;
		_bodyFileSent = 0;
	}
	return *this;
}

Client::~Client() {
	if (_cgi != NULL) {
		_cgi->killProcess();
		delete _cgi;
		_cgi = NULL;
	}
	closeBodyFile();
}

// Client operations
ssize_t Client::read() {
	char buffer[8192];
	ssize_t bytes = ::recv(_fd, buffer, sizeof(buffer), 0);
	if (bytes > 0) {
		_request.appendData(std::string(buffer, static_cast<size_t>(bytes)));
		if (!_continueSent && _request.expects100Continue()) {
			const char* cont = "HTTP/1.1 100 Continue\r\n\r\n";
			::send(_fd, cont, 25, MSG_NOSIGNAL);
			_continueSent = true;
		}
		updateLastActivity();
		return bytes;
	}
	if (bytes == 0)
		return 0;
	return -1;
}

ssize_t Client::write() {
	// Headers first (small). One send per call, as select() requires.
	if (_writeOffset < _writeBuffer.size()) {
		ssize_t bytes = ::send(_fd, _writeBuffer.c_str() + _writeOffset, _writeBuffer.size() - _writeOffset, MSG_NOSIGNAL);
		if (bytes > 0) {
			_writeOffset += static_cast<size_t>(bytes);
			updateLastActivity();
			return bytes;
		}
		return -1;
	}
	if (_response.isBodySuppressed())
		return 0;
	// A large CGI body lives in a temp file, streamed in bounded chunks so the
	// server never holds the whole 100MB entity in memory.
	if (_bodyFd >= 0) {
		if (_bodyFileSent >= _bodyFileSize)
			return 0;
		char buffer[65536];
		if (::lseek(_bodyFd, static_cast<off_t>(_bodyFileSent), SEEK_SET) < 0)
			return -1;
		ssize_t n = ::read(_bodyFd, buffer, sizeof(buffer));
		if (n <= 0)
			return -1;
		ssize_t bytes = ::send(_fd, buffer, static_cast<size_t>(n), MSG_NOSIGNAL);
		if (bytes > 0) {
			_bodyFileSent += static_cast<size_t>(bytes);
			updateLastActivity();
			return bytes;
		}
		return -1;
	}
	// Otherwise the body is in the response buffer, streamed straight from there
	// so it is never duplicated into _writeBuffer.
	const std::string& body = _response.getBody();
	if (_bodyOffset < body.size()) {
		ssize_t bytes = ::send(_fd, body.c_str() + _bodyOffset, body.size() - _bodyOffset, MSG_NOSIGNAL);
		if (bytes > 0) {
			_bodyOffset += static_cast<size_t>(bytes);
			updateLastActivity();
			return bytes;
		}
		return -1;
	}
	return 0;
}

void Client::processRequest() {
	if (_serverConfig == NULL)
		return;

	_response.clear();
	closeBodyFile();
	if (_cgi != NULL) {
		_cgi->killProcess();
		delete _cgi;
		_cgi = NULL;
	}

	if (!_request.isValid()) {
		_response.buildErrorResponse(_request.getErrorCode(), "");
		setKeepAlive(false);
		_response.addHeader("Connection", "close");
		return;
	}

	SessionMiddleware sessionMiddleware;
	sessionMiddleware.processRequest(_request);

	RequestHandler handler(_request, _response, *_serverConfig);
	CgiHandler* cgi = new CgiHandler(_request, _response);
	cgi->setRemoteAddr(ipToString(_address));
	bool handled = false;
	if (handler.startCgiIfNeeded(*cgi, handled)) {
		_cgi = cgi;
		updateLastActivity();
		return;
	}
	delete cgi;

	if (!handled)
		handler.handle();

	sessionMiddleware.processResponse(_request, _response);
	setKeepAlive(_request.keepAlive());
	if (_keepAlive)
		_response.addHeader("Connection", "keep-alive");
	else
		_response.addHeader("Connection", "close");
}

void Client::prepareResponse() {
	if (_request.getMethod() == "HEAD")
		_response.setSuppressBody(true);
	_writeBuffer = _response.buildHead();
	_writeOffset = 0;
	_bodyOffset = 0;

	std::ostringstream oss;
	oss << ipToString(_address) << " \""
		<< (_request.getMethod().empty() ? "-" : _request.getMethod()) << " "
		<< (_request.getUri().empty() ? "-" : _request.getUri()) << "\" "
		<< _response.getStatusCode() << " "
		<< (_response.hasExternalBody() ? _response.getExternalBodyLength() : _response.getBody().size());
	Logger::getInstance()->info(oss.str());
}

void Client::processCgiInput() {
	if (_cgi == NULL)
		return;
	_cgi->writeInput();
	updateLastActivity();
}

void Client::processCgiOutput() {
	if (_cgi == NULL)
		return;
	_cgi->readOutput();
	updateLastActivity();
}

void Client::finishCgi() {
	if (_cgi == NULL)
		return;
	_cgi->finish();
	// Take ownership of the spilled-body temp file (if any) so it can be
	// streamed to the client after the CGI handler is gone.
	closeBodyFile();
	if (_response.hasExternalBody() && _cgi->hasBodyFile())
		_bodyFd = _cgi->releaseBodyFile(_bodyFileSize);
	delete _cgi;
	_cgi = NULL;
	SessionMiddleware sessionMiddleware;
	sessionMiddleware.processResponse(_request, _response);
	setKeepAlive(_request.keepAlive());
	if (_keepAlive)
		_response.addHeader("Connection", "keep-alive");
	else
		_response.addHeader("Connection", "close");
}

void Client::failCgiTimeout() {
	if (_cgi == NULL)
		return;
	_cgi->killProcess();
	_cgi->setGatewayError(504);
	delete _cgi;
	_cgi = NULL;
	setKeepAlive(false);
	_response.addHeader("Connection", "close");
}

bool Client::isReadComplete() const {
	return _request.isComplete();
}

bool Client::isWriteComplete() const {
	if (_writeOffset < _writeBuffer.size())
		return false;
	if (_response.isBodySuppressed())
		return true;
	if (_bodyFd >= 0)
		return _bodyFileSent >= _bodyFileSize;
	return _bodyOffset >= _response.getBody().size();
}

void Client::close() {
	if (_cgi != NULL) {
		_cgi->killProcess();
		delete _cgi;
		_cgi = NULL;
	}
	closeBodyFile();
	if (_fd != -1) {
		::close(_fd);
		_fd = -1;
	}
}

// Private helper methods
void Client::updateLastActivity() {
	_lastActivity = std::time(NULL);
}

void Client::closeBodyFile() {
	if (_bodyFd >= 0)
		::close(_bodyFd);
	_bodyFd = -1;
	_bodyFileSize = 0;
	_bodyFileSent = 0;
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

bool Client::getKeepAlive() const {
	return _keepAlive;
}

bool Client::hasActiveCgi() const {
	return _cgi != NULL;
}

bool Client::isCgiComplete() {
	return _cgi != NULL && _cgi->isComplete();
}

bool Client::isCgiTimeout(time_t currentTime, time_t timeout) const {
	return _cgi != NULL && _cgi->isTimeout(currentTime, timeout);
}

int Client::getCgiInputFd() const {
	if (_cgi == NULL || !_cgi->wantsInputWrite())
		return -1;
	return _cgi->getInputFd();
}

int Client::getCgiOutputFd() const {
	if (_cgi == NULL || !_cgi->wantsOutputRead())
		return -1;
	return _cgi->getOutputFd();
}

// Setters
void Client::setFd(int fd) {
	_fd = fd;
}

void Client::setServerConfig(ServerConfig* config) {
	_serverConfig = config;
	if (config != NULL)
		_request.setMaxBodySize(config->getClientMaxBodySize());
}

void Client::setKeepAlive(bool keepAlive) {
	_keepAlive = keepAlive;
}

// Buffer operations
void Client::clearReadBuffer() {
	_readBuffer.clear();
	_continueSent = false;
}

void Client::clearWriteBuffer() {
	_writeBuffer.clear();
	_writeOffset = 0;
	_bodyOffset = 0;
	closeBodyFile();
}

// Utility methods
bool Client::isTimeout(time_t currentTime, time_t timeout) const {
	return (currentTime - _lastActivity) > timeout;
}
