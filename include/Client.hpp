#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <string>
# include <netinet/in.h>
# include "HttpRequest.hpp"
# include "HttpResponse.hpp"
# include "ServerConfig.hpp"

class Client {
private:
	int					_fd;
	struct sockaddr_in	_address;
	HttpRequest			_request;
	HttpResponse		_response;
	std::string			_readBuffer;
	std::string			_writeBuffer;
	bool				_keepAlive;
	time_t				_lastActivity;
	ServerConfig*		_serverConfig;

	// Helper methods
	void		updateLastActivity();

public:
	// Orthodox Canonical Form
	Client();
	Client(int fd, const struct sockaddr_in& address);
	Client(const Client& other);
	Client& operator=(const Client& other);
	~Client();

	// Client operations
	ssize_t		read();
	ssize_t		write();
	void		processRequest();
	void		prepareResponse();
	bool		isReadComplete() const;
	bool		isWriteComplete() const;
	void		close();

	// Getters
	int					getFd() const;
	HttpRequest&		getRequest();
	HttpResponse&		getResponse();
	const std::string&	getReadBuffer() const;
	const std::string&	getWriteBuffer() const;
	bool				getKeepAlive() const;
	time_t				getLastActivity() const;
	ServerConfig*		getServerConfig() const;

	// Setters
	void		setFd(int fd);
	void		setServerConfig(ServerConfig* config);
	void		setKeepAlive(bool keepAlive);

	// Buffer operations
	void		appendReadBuffer(const std::string& data);
	void		appendWriteBuffer(const std::string& data);
	void		clearReadBuffer();
	void		clearWriteBuffer();

	// Utility methods
	bool		isTimeout(time_t currentTime, time_t timeout) const;
	std::string	getClientIp() const;
};

#endif
