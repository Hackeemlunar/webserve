#ifndef SOCKET_HPP
# define SOCKET_HPP

# include <string>
# include <netinet/in.h>

class Socket {
private:
	int					_fd;
	int					_port;
	std::string			_host;
	struct sockaddr_in	_address;
	bool				_isListening;

	// Helper methods
	void		setNonBlocking();
	void		setSocketOptions();

public:
	// Orthodox Canonical Form
	Socket();
	Socket(const std::string& host, int port);
	Socket(const Socket& other);
	Socket& operator=(const Socket& other);
	~Socket();

	// Socket operations
	bool		create();
	bool		bind();
	bool		listen(int backlog);
	int			accept();
	void		close();

	// Getters
	int					getFd() const;
	int					getPort() const;
	const std::string&	getHost() const;
	bool				isListening() const;
	struct sockaddr_in	getAddress() const;

	// Setters
	void		setFd(int fd);
	void		setPort(int port);
	void		setHost(const std::string& host);
};

#endif
