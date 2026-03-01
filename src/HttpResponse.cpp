#include "../include/HttpResponse.hpp"

// Orthodox Canonical Form
HttpResponse::HttpResponse() : _statusCode(200), _statusMessage("OK"), _httpVersion("HTTP/1.1"), _headersSent(false) {
	setDefaultHeaders();
}

HttpResponse::HttpResponse(const HttpResponse& other) {
	*this = other;
}

HttpResponse& HttpResponse::operator=(const HttpResponse& other) {
	if (this != &other) {
		_statusCode = other._statusCode;
		_statusMessage = other._statusMessage;
		_httpVersion = other._httpVersion;
		_headers = other._headers;
		_body = other._body;
		_headersSent = other._headersSent;
	}
	return *this;
}

HttpResponse::~HttpResponse() {
}

// Setters
void HttpResponse::setStatusCode(int code) {
	_statusCode = code;
	_statusMessage = getStatusMessage(code);
}

void HttpResponse::setHttpVersion(const std::string& version) {
	_httpVersion = version;
}

void HttpResponse::addHeader(const std::string& key, const std::string& value) {
	_headers[key] = value;
}

void HttpResponse::setBody(const std::string& body) {
	_body = body;
}

void HttpResponse::appendBody(const std::string& data) {
	_body.append(data);
}

// Getters
int HttpResponse::getStatusCode() const {
	return _statusCode;
}

const std::string& HttpResponse::getStatusMessage() const {
	return _statusMessage;
}

const std::string& HttpResponse::getHttpVersion() const {
	return _httpVersion;
}

const std::map<std::string, std::string>& HttpResponse::getHeaders() const {
	return _headers;
}

const std::string& HttpResponse::getBody() const {
	return _body;
}

// Response building
std::string HttpResponse::build() {
	// TODO: Implementation
	return "";
}

std::string HttpResponse::buildHeaders() {
	// TODO: Implementation
	return "";
}

void HttpResponse::clear() {
	// TODO: Implementation
}

// Utility methods
void HttpResponse::setContentType(const std::string& type) {
	addHeader("Content-Type", type);
}

void HttpResponse::setContentLength(size_t length) {
	// TODO: Implementation
	(void)length;
}

void HttpResponse::setLocation(const std::string& location) {
	addHeader("Location", location);
}

void HttpResponse::setCookie(const std::string& name, const std::string& value) {
	// TODO: Implementation
	(void)name;
	(void)value;
}

bool HttpResponse::isHeadersSent() const {
	return _headersSent;
}

void HttpResponse::buildErrorResponse(int code, const std::string& errorPage) {
	// TODO: Implementation
	(void)code;
	(void)errorPage;
}

// Private helper methods
std::string HttpResponse::getStatusMessage(int code) const {
	// TODO: Implementation
	(void)code;
	return "OK";
}

void HttpResponse::setDefaultHeaders() {
	// TODO: Implementation
}
