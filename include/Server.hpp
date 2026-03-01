#ifndef SERVER_HPP
# define SERVER_HPP

# include <string>
# include <vector>
# include <map>
# include "ServerConfig.hpp"
# include "Client.hpp"
# include "Socket.hpp"

class Server {
private:
	std::vector<ServerConfig>		_serverConfigs;
	std::vector<Socket>				_listenSockets;
	std::map<int, Client>			_clients;
	int								_maxFd;
	fd_set							_readFds;
	fd_set							_writeFds;
	fd_set							_masterReadFds;
	fd_set							_masterWriteFds;

	// Private helper methods
	void		setupListenSockets();
	void		acceptNewConnection(int listenFd);
	void		handleClientRead(int clientFd);
	void		handleClientWrite(int clientFd);
	void		closeClientConnection(int clientFd);
	void		updateMaxFd();

public:
	// Orthodox Canonical Form
	Server();
	Server(const Server& other);
	Server& operator=(const Server& other);
	~Server();

	// Configuration
	void		loadConfig(const std::string& configFile);
	void		addServerConfig(const ServerConfig& config);

	// Main server operations
	void		init();
	void		run();
	void		stop();

	// Socket management
	void		setupSockets();
	void		closeAllSockets();

	// Getters
	const std::vector<ServerConfig>&	getServerConfigs() const;
	int									getMaxFd() const;
};

#endif
