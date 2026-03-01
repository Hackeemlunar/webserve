#include "../include/ServerConfig.hpp"

// Orthodox Canonical Form
ServerConfig::ServerConfig() : _host("0.0.0.0"), _port(8080), _clientMaxBodySize(1048576) {
}

ServerConfig::ServerConfig(const ServerConfig& other) {
	*this = other;
}

ServerConfig& ServerConfig::operator=(const ServerConfig& other) {
	if (this != &other) {
		_host = other._host;
		_port = other._port;
		_serverNames = other._serverNames;
		_errorPages = other._errorPages;
		_clientMaxBodySize = other._clientMaxBodySize;
		_routes = other._routes;
		_root = other._root;
		_index = other._index;
	}
	return *this;
}

ServerConfig::~ServerConfig() {
}

// Setters
void ServerConfig::setHost(const std::string& host) {
	_host = host;
}

void ServerConfig::setPort(int port) {
	_port = port;
}

void ServerConfig::addServerName(const std::string& name) {
	_serverNames.push_back(name);
}

void ServerConfig::setErrorPage(int code, const std::string& path) {
	_errorPages[code] = path;
}

void ServerConfig::setClientMaxBodySize(size_t size) {
	_clientMaxBodySize = size;
}

void ServerConfig::addRoute(const Route& route) {
	_routes.push_back(route);
}

void ServerConfig::setRoot(const std::string& root) {
	_root = root;
}

void ServerConfig::addIndexFile(const std::string& file) {
	_index.push_back(file);
}

// Getters
const std::string& ServerConfig::getHost() const {
	return _host;
}

int ServerConfig::getPort() const {
	return _port;
}

const std::vector<std::string>& ServerConfig::getServerNames() const {
	return _serverNames;
}

const std::map<int, std::string>& ServerConfig::getErrorPages() const {
	return _errorPages;
}

size_t ServerConfig::getClientMaxBodySize() const {
	return _clientMaxBodySize;
}

const std::vector<Route>& ServerConfig::getRoutes() const {
	return _routes;
}

const std::string& ServerConfig::getRoot() const {
	return _root;
}

const std::vector<std::string>& ServerConfig::getIndex() const {
	return _index;
}

// Utility methods
std::string ServerConfig::getErrorPage(int code) const {
	// TODO: Implementation
	(void)code;
	return "";
}

Route* ServerConfig::matchRoute(const std::string& path) {
	// TODO: Implementation
	(void)path;
	return NULL;
}

bool ServerConfig::isServerName(const std::string& name) const {
	// TODO: Implementation
	(void)name;
	return false;
}
