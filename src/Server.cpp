#include "../include/Server.hpp"
#include "../include/Logger.hpp"

// Orthodox Canonical Form
Server::Server() : _maxFd(0) {
	FD_ZERO(&_masterReadFds);
	FD_ZERO(&_masterWriteFds);
}

Server::Server(const Server& other) {
	*this = other;
}

Server& Server::operator=(const Server& other) {
	if (this != &other) {
		_serverConfigs = other._serverConfigs;
		_listenSockets = other._listenSockets;
		_clients = other._clients;
		_maxFd = other._maxFd;
		_readFds = other._readFds;
		_writeFds = other._writeFds;
		_masterReadFds = other._masterReadFds;
		_masterWriteFds = other._masterWriteFds;
	}
	return *this;
}

Server::~Server() {
	closeAllSockets();
}

// Configuration
void Server::loadConfig(const std::string& configFile) {
	// TODO: Implementation
}

void Server::addServerConfig(const ServerConfig& config) {
	// TODO: Implementation
}

// Main server operations
void Server::init() {
	// TODO: Implementation
}

void Server::run() {
	// TODO: Implementation
}

void Server::stop() {
	// TODO: Implementation
}

// Socket management
void Server::setupSockets() {
	// TODO: Implementation
}

void Server::closeAllSockets() {
	// TODO: Implementation
}

// Private helper methods
void Server::setupListenSockets() {
	// TODO: Implementation
}

void Server::acceptNewConnection(int listenFd) {
	// TODO: Implementation
}

void Server::handleClientRead(int clientFd) {
	// TODO: Implementation
}

void Server::handleClientWrite(int clientFd) {
	// TODO: Implementation
}

void Server::closeClientConnection(int clientFd) {
	// TODO: Implementation
}

void Server::updateMaxFd() {
	// TODO: Implementation
}

// Getters
const std::vector<ServerConfig>& Server::getServerConfigs() const {
	return _serverConfigs;
}

int Server::getMaxFd() const {
	return _maxFd;
}
