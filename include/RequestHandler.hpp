#ifndef REQUESTHANDLER_HPP
# define REQUESTHANDLER_HPP

# include <string>
# include "HttpRequest.hpp"
# include "HttpResponse.hpp"
# include "ServerConfig.hpp"
# include "Route.hpp"

class RequestHandler {
private:
	HttpRequest&		_request;
	HttpResponse&		_response;
	ServerConfig&		_config;
	Route*				_route;

	// Handler methods
	void		handleGet();
	void		handlePost();
	void		handleDelete();
	void		handleHead();
	void		handlePut();

	// Helper methods
	void		serveStaticFile(const std::string& path);
	void		generateDirectoryListing(const std::string& path);
	void		handleRedirect(const std::string& location);
	void		handleCgi();
	void		handleFileUpload();
	std::string	resolveFilePath();
	bool		isAllowedMethod(const std::string& method);

public:
	// Orthodox Canonical Form
	RequestHandler(HttpRequest& request, HttpResponse& response, ServerConfig& config);
	RequestHandler(const RequestHandler& other);
	RequestHandler& operator=(const RequestHandler& other);
	~RequestHandler();

	// Main handler
	void		handle();

	// Error handling
	void		handleError(int statusCode);
	void		sendErrorPage(int statusCode);

	// Utility methods
	void		setRoute(Route* route);
	Route*		getRoute() const;
};

#endif
