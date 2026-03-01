#ifndef CGIHANDLER_HPP
# define CGIHANDLER_HPP

# include <string>
# include <map>
# include <vector>
# include "HttpRequest.hpp"
# include "HttpResponse.hpp"
# include "Route.hpp"

class CgiHandler {
private:
	std::string							_scriptPath;
	std::string							_cgiExecutable;
	HttpRequest&						_request;
	HttpResponse&						_response;
	std::map<std::string, std::string>	_env;
	std::string							_output;
	int									_exitStatus;

	// Helper methods
	void		setupEnvironment(const Route& route);
	void		parseOutput();
	char**		getEnvArray() const;
	void		freeEnvArray(char** env) const;

public:
	// Orthodox Canonical Form
	CgiHandler(HttpRequest& request, HttpResponse& response);
	CgiHandler(const CgiHandler& other);
	CgiHandler& operator=(const CgiHandler& other);
	~CgiHandler();

	// CGI execution
	bool		execute(const std::string& scriptPath, const Route& route);
	void		setEnvVariable(const std::string& key, const std::string& value);

	// Getters
	const std::string&	getOutput() const;
	int					getExitStatus() const;
	const std::string&	getScriptPath() const;
	const std::string&	getCgiExecutable() const;

	// Setters
	void		setScriptPath(const std::string& path);
	void		setCgiExecutable(const std::string& executable);

	// Utility methods
	static bool	isCgiRequest(const std::string& path, const Route& route);
	static std::string	getCgiExecutablePath(const std::string& extension, const Route& route);
};

#endif
