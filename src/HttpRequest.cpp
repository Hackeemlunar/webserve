#include "../include/HttpRequest.hpp"

#include <cstdlib>
#include <sstream>
#include <algorithm>
#include <cctype>

namespace {
	static std::string toLowerCase(const std::string& value) {
		std::string out = value;
		for (size_t i = 0; i < out.size(); ++i)
			out[i] = static_cast<char>(std::tolower(out[i]));
		return out;
	}
}

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
	clear();
	_rawRequest = rawRequest;

	size_t headerEnd = rawRequest.find("\r\n\r\n");
	size_t sepLength = 4;
	if (headerEnd == std::string::npos) {
		headerEnd = rawRequest.find("\n\n");
		sepLength = 2;
	}
	if (headerEnd == std::string::npos)
		return true;

	std::string headerSection = rawRequest.substr(0, headerEnd);
	std::string bodySection = rawRequest.substr(headerEnd + sepLength);

	size_t lineEnd = headerSection.find("\r\n");
	size_t lineSep = 2;
	if (lineEnd == std::string::npos) {
		lineEnd = headerSection.find('\n');
		lineSep = 1;
	}
	if (lineEnd == std::string::npos)
		return false;

	parseRequestLine(headerSection.substr(0, lineEnd));
	parseHeaders(headerSection.substr(lineEnd + lineSep));

	if (_isChunked) {
		parseChunkedBody(bodySection);
		return true;
	}

	parseBody(bodySection);
	if (_contentLength == 0)
		_isComplete = true;
	else if (_body.size() >= _contentLength) {
		_body = _body.substr(0, _contentLength);
		_isComplete = true;
	}

	return true;
}

void HttpRequest::appendData(const std::string& data) {
	_rawRequest.append(data);
	parse(_rawRequest);
}

bool HttpRequest::isComplete() const {
	return _isComplete;
}

void HttpRequest::clear() {
	_method.clear();
	_uri.clear();
	_httpVersion = "HTTP/1.1";
	_headers.clear();
	_body.clear();
	_queryString.clear();
	_isComplete = false;
	_isChunked = false;
	_contentLength = 0;
}

// Private parsing helpers
void HttpRequest::parseRequestLine(const std::string& line) {
	std::istringstream iss(line);
	iss >> _method >> _uri >> _httpVersion;
	parseUri();
}

void HttpRequest::parseHeaders(const std::string& headerSection) {
	std::istringstream iss(headerSection);
	std::string line;
	while (std::getline(iss, line)) {
		if (!line.empty() && line[line.size() - 1] == '\r')
			line.erase(line.size() - 1);
		if (line.empty())
			continue;

		size_t colon = line.find(':');
		if (colon == std::string::npos)
			continue;

		std::string key = line.substr(0, colon);
		std::string value = line.substr(colon + 1);
		while (!value.empty() && (value[0] == ' ' || value[0] == '\t'))
			value.erase(0, 1);
		_headers[toLowerCase(key)] = value;
	}

	if (hasHeader("content-length"))
		_contentLength = static_cast<size_t>(std::strtoul(getHeader("content-length").c_str(), NULL, 10));
	if (hasHeader("transfer-encoding") && toLowerCase(getHeader("transfer-encoding")) == "chunked")
		_isChunked = true;
}

void HttpRequest::parseBody(const std::string& bodySection) {
	_body = bodySection;
}

void HttpRequest::parseUri() {
	size_t queryPos = _uri.find('?');
	if (queryPos == std::string::npos) {
		_queryString.clear();
		return;
	}
	_queryString = _uri.substr(queryPos + 1);
}

void HttpRequest::parseChunkedBody(const std::string& bodySection) {
	_body = bodySection;
	if (_body.find("\r\n0\r\n\r\n") != std::string::npos || _body.find("\n0\n\n") != std::string::npos)
		_isComplete = true;
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
	std::map<std::string, std::string>::const_iterator it = _headers.find(toLowerCase(key));
	if (it == _headers.end())
		return "";
	return it->second;
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
	return _headers.find(toLowerCase(key)) != _headers.end();
}

bool HttpRequest::keepAlive() const {
	std::string connection = toLowerCase(getHeader("connection"));
	if (_httpVersion == "HTTP/1.1")
		return connection != "close";
	return connection == "keep-alive";
}

std::string HttpRequest::getPath() const {
	size_t queryPos = _uri.find('?');
	if (queryPos == std::string::npos)
		return _uri.empty() ? "/" : _uri;
	return _uri.substr(0, queryPos);
}
