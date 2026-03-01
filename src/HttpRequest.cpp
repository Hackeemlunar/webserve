#include "../include/HttpRequest.hpp"

// Orthodox Canonical Form
HttpRequest::HttpRequest() : _httpVersion("HTTP/1.1"), _isComplete(false), _isChunked(false), _contentLength(0) {
}

HttpRequest::HttpRequest(const HttpRequest& other) {
	*this = other;
}

HttpRequest& HttpRequest::operator=(const HttpRequest& other) {
	if (this != &other) {
		_method = other._method;
		_uri = other._uri;
		_httpVersion = other._httpVersion;
		_headers = other._headers;
		_body = other._body;
		_queryString = other._queryString;
		_isComplete = other._isComplete;
		_isChunked = other._isChunked;
		_contentLength = other._contentLength;
		_rawRequest = other._rawRequest;
	}
	return *this;
}

HttpRequest::~HttpRequest() {
}

// Parsing
bool HttpRequest::parse(const std::string& rawRequest) {
	// TODO: Implementation
	(void)rawRequest;
	return false;
}

void HttpRequest::appendData(const std::string& data) {
	// TODO: Implementation
	(void)data;
}

bool HttpRequest::isComplete() const {
	return _isComplete;
}

void HttpRequest::clear() {
	// TODO: Implementation
}

// Private parsing helpers
void HttpRequest::parseRequestLine(const std::string& line) {
	// TODO: Implementation
	(void)line;
}

void HttpRequest::parseHeaders(const std::string& headerSection) {
	// TODO: Implementation
	(void)headerSection;
}

void HttpRequest::parseBody(const std::string& bodySection) {
	// TODO: Implementation
	(void)bodySection;
}

void HttpRequest::parseUri() {
	// TODO: Implementation
}

void HttpRequest::parseChunkedBody(const std::string& bodySection) {
	// TODO: Implementation
	(void)bodySection;
}

// Getters
const std::string& HttpRequest::getMethod() const {
	return _method;
}

const std::string& HttpRequest::getUri() const {
	return _uri;
}

const std::string& HttpRequest::getHttpVersion() const {
	return _httpVersion;
}

const std::map<std::string, std::string>& HttpRequest::getHeaders() const {
	return _headers;
}

std::string HttpRequest::getHeader(const std::string& key) const {
	// TODO: Implementation
	(void)key;
	return "";
}

const std::string& HttpRequest::getBody() const {
	return _body;
}

const std::string& HttpRequest::getQueryString() const {
	return _queryString;
}

size_t HttpRequest::getContentLength() const {
	return _contentLength;
}

bool HttpRequest::isChunked() const {
	return _isChunked;
}

// Setters
void HttpRequest::setMethod(const std::string& method) {
	_method = method;
}

void HttpRequest::setUri(const std::string& uri) {
	_uri = uri;
}

void HttpRequest::setHttpVersion(const std::string& version) {
	_httpVersion = version;
}

void HttpRequest::addHeader(const std::string& key, const std::string& value) {
	_headers[key] = value;
}

void HttpRequest::setBody(const std::string& body) {
	_body = body;
}

// Utility methods
bool HttpRequest::hasHeader(const std::string& key) const {
	// TODO: Implementation
	(void)key;
	return false;
}

bool HttpRequest::keepAlive() const {
	// TODO: Implementation
	return false;
}

std::string HttpRequest::getPath() const {
	// TODO: Implementation
	return "";
}
