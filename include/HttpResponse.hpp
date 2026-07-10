#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP

# include <string>
# include <map>
# include "CookieParser.hpp"

class HttpResponse {
	private:
	int									_statusCode;
	std::string							_statusMessage;
	std::string							_httpVersion;
	std::map<std::string, std::string>	_headers;
	std::string							_body;
	bool								_setCookie;
	std::string							_cookieHeader;
	bool								_suppressBody;
	// When the entity is served from a file (large CGI output) rather than
	// _body, this holds its length for Content-Length; -1 means "use _body".
	long								_externalBodyLength;

	// Helper methods
	std::string		getStatusMessage(int code) const;
	std::string		getHttpDate() const;
	std::string		buildHeaders();
	void			setDefaultHeaders();

public:
	// Orthodox Canonical Form
	HttpResponse();
	HttpResponse(const HttpResponse& other);
	HttpResponse& operator=(const HttpResponse& other);
	~HttpResponse();

	// Setters
	void		setStatusCode(int code);
	void		addHeader(const std::string& key, const std::string& value);
	void		setBody(const std::string& body);
	// Move-in a large body without copying (swaps buffers); src is left empty.
	void		setBodySwap(std::string& body);
	void		setCookieHeader(const std::string& cookie);
	// For HEAD: build() omits the body from the wire output while
	// Content-Length still reflects the entity size, as RFC 7231 requires.
	void		setSuppressBody(bool suppress);

	// Getters
	int									getStatusCode() const;
	const std::string&					getStatusMessage() const;
	const std::string&					getBody() const;
	const std::string&					getCookieHeader() const;
	bool								isBodySuppressed() const;
	// Large-body streaming from a file (see CgiHandler spill). When set, the
	// client streams Content-Length bytes from its own fd, not from getBody().
	void								setExternalBodyLength(size_t length);
	bool								hasExternalBody() const;
	size_t								getExternalBodyLength() const;

	// Response building
	std::string		build();
	// Headers only (sets Content-Length + Date). The body is streamed
	// separately straight from getBody() so a large entity is never copied
	// into a second buffer — see Client::write().
	std::string		buildHead();
	void			clear();

	// Utility methods
	void		setContentType(const std::string& type);
	void		setContentLength(size_t length);
	void		setLocation(const std::string& location);

	// Error responses
	void		buildErrorResponse(int code, const std::string& errorPage);
};

#endif
