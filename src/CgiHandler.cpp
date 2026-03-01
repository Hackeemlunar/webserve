#include "../include/CgiHandler.hpp"
#include <unistd.h>
#include <cstdlib>

// Orthodox Canonical Form
CgiHandler::CgiHandler(HttpRequest& request, HttpResponse& response) : _request(request), _response(response), _exitStatus(0) {
}

CgiHandler::CgiHandler(const CgiHandler& other) : _request(other._request), _response(other._response) {
	_scriptPath = other._scriptPath;
	_cgiExecutable = other._cgiExecutable;
	_env = other._env;
	_output = other._output;
	_exitStatus = other._exitStatus;
}

CgiHandler& CgiHandler::operator=(const CgiHandler& other) {
	if (this != &other) {
		_scriptPath = other._scriptPath;
		_cgiExecutable = other._cgiExecutable;
		_env = other._env;
		_output = other._output;
		_exitStatus = other._exitStatus;
	}
	return *this;
}

CgiHandler::~CgiHandler() {
}

// CGI execution
bool CgiHandler::execute(const std::string& scriptPath, const Route& route) {
	// TODO: Implementation
	(void)scriptPath;
	(void)route;
	return false;
}

void CgiHandler::setEnvVariable(const std::string& key, const std::string& value) {
	_env[key] = value;
}

// Private helper methods
void CgiHandler::setupEnvironment(const Route& route) {
	// TODO: Implementation
	(void)route;
}

void CgiHandler::parseOutput() {
	// TODO: Implementation
}

char** CgiHandler::getEnvArray() const {
	// TODO: Implementation
	return NULL;
}

void CgiHandler::freeEnvArray(char** env) const {
	// TODO: Implementation
	(void)env;
}

// Getters
const std::string& CgiHandler::getOutput() const {
	return _output;
}

int CgiHandler::getExitStatus() const {
	return _exitStatus;
}

const std::string& CgiHandler::getScriptPath() const {
	return _scriptPath;
}

const std::string& CgiHandler::getCgiExecutable() const {
	return _cgiExecutable;
}

// Setters
void CgiHandler::setScriptPath(const std::string& path) {
	_scriptPath = path;
}

void CgiHandler::setCgiExecutable(const std::string& executable) {
	_cgiExecutable = executable;
}

// Static utility methods
bool CgiHandler::isCgiRequest(const std::string& path, const Route& route) {
	// TODO: Implementation
	(void)path;
	(void)route;
	return false;
}

std::string CgiHandler::getCgiExecutablePath(const std::string& extension, const Route& route) {
	// TODO: Implementation
	(void)extension;
	(void)route;
	return "";
}
