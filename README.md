# Webserv - HTTP Server

A custom HTTP/1.1 server implementation written in C++98 as part of the 42 school curriculum.

## Project Structure

```
webserve/
├── include/          # Header files
│   ├── Server.hpp
│   ├── ServerConfig.hpp
│   ├── Route.hpp
│   ├── HttpRequest.hpp
│   ├── HttpResponse.hpp
│   ├── HttpParser.hpp
│   ├── Socket.hpp
│   ├── Client.hpp
│   ├── CgiHandler.hpp
│   ├── ConfigParser.hpp
│   ├── RequestHandler.hpp
│   ├── Logger.hpp
│   └── Utils.hpp
├── src/              # Source files
│   ├── main.cpp
│   ├── Server.cpp
│   ├── ServerConfig.cpp
│   ├── Route.cpp
│   ├── HttpRequest.cpp
│   ├── HttpResponse.cpp
│   ├── HttpParser.cpp
│   ├── Socket.cpp
│   ├── Client.cpp
│   ├── CgiHandler.cpp
│   ├── ConfigParser.cpp
│   ├── RequestHandler.cpp
│   ├── Logger.cpp
│   └── Utils.cpp
├── config/           # Configuration files
│   ├── default.conf
│   └── test.conf
├── www/              # Web content
│   ├── html/         # Static HTML files
│   ├── error_pages/  # Error page templates
│   └── uploads/      # Upload directory
├── cgi-bin/          # CGI scripts
├── Makefile
└── README.md

```

## Features (To Be Implemented)

- **HTTP/1.1 Protocol Support**
  - GET, POST, DELETE methods
  - Persistent connections (Keep-Alive)
  - Chunked transfer encoding

- **Server Configuration**
  - Multiple virtual hosts (server blocks)
  - Custom port and host binding
  - Server names configuration
  - Error page customization
  - Client body size limits

- **Routing**
  - Location-based routing
  - HTTP method restrictions
  - Directory listing (autoindex)
  - HTTP redirections
  - Default index files

- **Static File Serving**
  - MIME type detection
  - Directory listing generation

- **File Upload**
  - POST method file uploads
  - Configurable upload paths

- **CGI Support**
  - PHP, Python, and other CGI scripts
  - Environment variable setup
  - POST and GET method support

- **Non-blocking I/O**
  - select()/poll()/epoll()/kqueue() support
  - Asynchronous client handling

- **Logging**
  - Console and file logging
  - Configurable log levels

## Compilation

```bash
make        # Compile the project
make clean  # Remove object files
make fclean # Remove object files and executable
make re     # Recompile from scratch
```

## Usage

```bash
./webserv [configuration_file]
```

If no configuration file is specified, the server will use `config/default.conf`.

### Examples

```bash
# Run with default configuration
./webserv

# Run with custom configuration
./webserv config/test.conf

# View help
./webserv --help
```

## Testing

```bash
# Run the server
make run

# Run with debug flags
make debug

# Run with valgrind
make valgrind
```

### Testing with cURL

```bash
# GET request
curl http://localhost:8080/

# POST request
curl -X POST -d "data=test" http://localhost:8080/

# Upload file
curl -X POST -F "file=@test.txt" http://localhost:8080/uploads

# DELETE request
curl -X DELETE http://localhost:8080/uploads/test.txt
```

### Testing with telnet

```bash
telnet localhost 8080
GET / HTTP/1.1
Host: localhost

```

## Configuration File Format

See `config/default.conf` for a comprehensive example.

Basic structure:
```
server {
    listen 127.0.0.1:8080;
    server_name localhost;
    root ./www/html;
    index index.html;
    client_max_body_size 1048576;
    
    error_page 404 /error_pages/404.html;
    
    location / {
        allowed_methods GET POST;
        autoindex off;
    }
}
```

## Development Status

⚠️ **This project is currently in the initial setup phase.**

All header files and method stubs have been created. Implementation is pending.

### TODO

- [ ] Implement configuration parser
- [ ] Implement HTTP request parser
- [ ] Implement HTTP response builder
- [ ] Implement socket operations
- [ ] Implement select/poll/epoll for I/O multiplexing
- [ ] Implement request handler
- [ ] Implement static file serving
- [ ] Implement CGI handler
- [ ] Implement file upload
- [ ] Implement logging system
- [ ] Add error handling
- [ ] Add utility functions
- [ ] Testing and debugging

## Requirements

- C++98 compliant compiler (g++, clang++)
- POSIX-compliant operating system (Linux, macOS)
- Standard C++ library

## External Functions Allowed

According to the subject, the following functions are allowed:
- Socket operations: socket, accept, listen, send, recv, bind, connect
- I/O multiplexing: select, poll, epoll (Linux), kqueue (macOS/BSD)
- Network utilities: htons, htonl, ntohs, ntohl, getaddrinfo, etc.
- File operations: open, close, read, write, stat, access
- Process management: fork, execve, waitpid, kill (for CGI only)
- Directory operations: opendir, readdir, closedir
- Others: fcntl, setsockopt, signal, etc.

## References

- [RFC 7230 - HTTP/1.1: Message Syntax and Routing](https://tools.ietf.org/html/rfc7230)
- [RFC 7231 - HTTP/1.1: Semantics and Content](https://tools.ietf.org/html/rfc7231)
- [RFC 3875 - The Common Gateway Interface (CGI)](https://tools.ietf.org/html/rfc3875)
- [NGINX Configuration](https://nginx.org/en/docs/)

## License

This project is part of the 42 school curriculum.

## Authors

42 Student
