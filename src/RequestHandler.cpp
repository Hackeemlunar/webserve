#include "../include/RequestHandler.hpp"
#include "../include/CgiHandler.hpp"

#include <dirent.h>
#include <fstream>
#include <sstream>
#include <sys/stat.h>

namespace {
	static bool pathExists(const std::string& path) {
		struct stat st;
		return ::stat(path.c_str(), &st) == 0;
	}

	static bool isDirectory(const std::string& path) {
		struct stat st;
		if (::stat(path.c_str(), &st) != 0)
			return false;
		return S_ISDIR(st.st_mode);
	}

	static std::string extensionOf(const std::string& path) {
		size_t dot = path.find_last_of('.');
		if (dot == std::string::npos)
			return "";
		return path.substr(dot + 1);
	}

	static std::string mimeFromExt(const std::string& ext) {
		if (ext == "html" || ext == "htm") return "text/html";
		if (ext == "css") return "text/css";
		if (ext == "js") return "application/javascript";
		if (ext == "json") return "application/json";
		if (ext == "txt") return "text/plain";
		if (ext == "png") return "image/png";
		if (ext == "jpg" || ext == "jpeg") return "image/jpeg";
		if (ext == "gif") return "image/gif";
		if (ext == "ico") return "image/x-icon";
		return "application/octet-stream";
	}
}

// Orthodox Canonical Form
RequestHandler::RequestHandler(HttpRequest& request, HttpResponse& response, ServerConfig& config) 
	: _request(request), _response(response), _config(config), _route(NULL) {
}

RequestHandler::RequestHandler(const RequestHandler& other)
	: _request(other._request), _response(other._response), _config(other._config) {
	_route = other._route;
}

RequestHandler& RequestHandler::operator=(const RequestHandler& other) {
	if (this != &other) {
		_route = other._route;
	}
	return *this;
}

RequestHandler::~RequestHandler() {
}

// Main handler
void RequestHandler::handle() {
	_route = _config.matchRoute(_request.getPath());

	if (_route != NULL && !_route->getRedirect().empty()) {
		handleRedirect(_route->getRedirect());
		return;
	}

	if (!isAllowedMethod(_request.getMethod())) {
		handleError(405);
		return;
	}

	if (_request.getMethod() == "GET")
		handleGet();
	else if (_request.getMethod() == "POST")
		handlePost();
	else if (_request.getMethod() == "DELETE")
		handleDelete();
	else
		handleError(405);
}

// Private handler methods
void RequestHandler::handleGet() {
	std::string filePath = resolveFilePath();
	if (filePath.empty()) {
		handleError(403);
		return;
	}

	if (!pathExists(filePath)) {
		handleError(404);
		return;
	}

	if (isDirectory(filePath)) {
		if (_route != NULL && _route->getAutoindex()) {
			generateDirectoryListing(filePath);
			return;
		}
		handleError(403);
		return;
	}

	serveStaticFile(filePath);
}

void RequestHandler::handlePost() {
	handleError(501);
}

void RequestHandler::handleDelete() {
	handleError(501);
}

void RequestHandler::handleHead() {
	handleGet();
	_response.setBody("");
}

void RequestHandler::handlePut() {
	handleError(501);
}

// Private helper methods
void RequestHandler::serveStaticFile(const std::string& path) {
	std::ifstream file(path.c_str(), std::ios::in | std::ios::binary);
	if (!file.is_open()) {
		handleError(404);
		return;
	}

	std::ostringstream body;
	body << file.rdbuf();

	_response.setStatusCode(200);
	_response.setBody(body.str());
	_response.setContentType(mimeFromExt(extensionOf(path)));
}

void RequestHandler::generateDirectoryListing(const std::string& path) {
	DIR* dir = opendir(path.c_str());
	if (dir == NULL) {
		handleError(403);
		return;
	}

	std::ostringstream html;
	html << "<html><body><h1>Index of " << _request.getPath() << "</h1><ul>";

	struct dirent* entry;
	while ((entry = readdir(dir)) != NULL) {
		std::string name(entry->d_name);
		if (name == ".")
			continue;
		html << "<li><a href=\"" << name << "\">" << name << "</a></li>";
	}
	closedir(dir);

	html << "</ul></body></html>";
	_response.setStatusCode(200);
	_response.setContentType("text/html");
	_response.setBody(html.str());
}

void RequestHandler::handleRedirect(const std::string& location) {
	_response.setStatusCode(301);
	_response.setLocation(location);
	_response.setContentType("text/html");
	_response.setBody("<html><body><h1>301 Moved Permanently</h1></body></html>");
}

void RequestHandler::handleCgi() {
	// TODO: Implementation
}

void RequestHandler::handleFileUpload() {
	// TODO: Implementation
}

std::string RequestHandler::resolveFilePath() {
	std::string root = _config.getRoot();
	if (_route != NULL && !_route->getRoot().empty())
		root = _route->getRoot();

	std::string reqPath = _request.getPath();
	if (reqPath.empty())
		reqPath = "/";

	if (reqPath.find("..") != std::string::npos)
		return "";

	std::string relative = reqPath;
	if (_route != NULL && _route->getPath() != "/" && relative.find(_route->getPath()) == 0)
		relative = relative.substr(_route->getPath().size());
	if (relative.empty())
		relative = "/";

	std::string fullPath = root;
	if (!fullPath.empty() && fullPath[fullPath.size() - 1] == '/' && !relative.empty() && relative[0] == '/')
		fullPath += relative.substr(1);
	else if (!fullPath.empty() && fullPath[fullPath.size() - 1] != '/' && !relative.empty() && relative[0] != '/')
		fullPath += "/" + relative;
	else
		fullPath += relative;

	if (isDirectory(fullPath)) {
		if (_route != NULL) {
			const std::vector<std::string>& routeIndexes = _route->getIndex();
			for (size_t i = 0; i < routeIndexes.size(); ++i) {
				std::string candidate = fullPath;
				if (!candidate.empty() && candidate[candidate.size() - 1] != '/')
					candidate += "/";
				candidate += routeIndexes[i];
				if (pathExists(candidate))
					return candidate;
			}
		}

		const std::vector<std::string>& indexes = _config.getIndex();
		for (size_t i = 0; i < indexes.size(); ++i) {
			std::string candidate = fullPath;
			if (!candidate.empty() && candidate[candidate.size() - 1] != '/')
				candidate += "/";
			candidate += indexes[i];
			if (pathExists(candidate))
				return candidate;
		}
	}

	return fullPath;
}

bool RequestHandler::isAllowedMethod(const std::string& method) {
	if (_route == NULL)
		return method == "GET" || method == "POST" || method == "DELETE";
	return _route->isMethodAllowed(method);
}

// Error handling
void RequestHandler::handleError(int statusCode) {
	_response.setStatusCode(statusCode);
	sendErrorPage(statusCode);
}

void RequestHandler::sendErrorPage(int statusCode) {
	std::string errorPath = _config.getErrorPage(statusCode);
	if (!errorPath.empty()) {
		std::ifstream file(errorPath.c_str(), std::ios::in | std::ios::binary);
		if (!file.is_open() && !errorPath.empty() && errorPath[0] == '/')
			file.open(("." + errorPath).c_str(), std::ios::in | std::ios::binary);
		if (file.is_open()) {
			std::ostringstream body;
			body << file.rdbuf();
			_response.setContentType("text/html");
			_response.setBody(body.str());
			return;
		}
	}

	std::ostringstream fallback;
	fallback << "<html><body><h1>" << statusCode << " " << _response.getStatusMessage() << "</h1></body></html>";
	_response.setContentType("text/html");
	_response.setBody(fallback.str());
}

// Utility methods
void RequestHandler::setRoute(Route* route) {
	_route = route;
}

Route* RequestHandler::getRoute() const {
	return _route;
}
