#ifndef HTTPPARSER_HPP
# define HTTPPARSER_HPP

# include <string>
# include "HttpRequest.hpp"

class HttpParser {
private:
	// Utility methods
	static std::string	trim(const std::string& str);
	static bool			isValidMethod(const std::string& method);
	static bool			isValidUri(const std::string& uri);
	static bool			isValidHttpVersion(const std::string& version);

public:
	// Orthodox Canonical Form
	HttpParser();
	HttpParser(const HttpParser& other);
	HttpParser& operator=(const HttpParser& other);
	~HttpParser();

	// Parsing methods
	static bool		parseRequest(const std::string& rawRequest, HttpRequest& request);
	static bool		parseRequestLine(const std::string& line, HttpRequest& request);
	static bool		parseHeaders(const std::string& headerSection, HttpRequest& request);
	static bool		parseBody(const std::string& bodySection, HttpRequest& request);
	static bool		parseChunkedBody(const std::string& bodySection, HttpRequest& request);

	// Validation
	static bool		validateRequest(const HttpRequest& request);
	static bool		validateMethod(const std::string& method);
	static bool		validateUri(const std::string& uri);
	static bool		validateHeaders(const std::map<std::string, std::string>& headers);
};

#endif
