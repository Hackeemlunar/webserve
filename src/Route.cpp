#include "../include/Route.hpp"

// Orthodox Canonical Form
Route::Route() : _path("/"), _autoindex(false) {
}

Route::Route(const std::string& path) : _path(path), _autoindex(false) {
}

Route::Route(const Route& other) {
	*this = other;
}

Route& Route::operator=(const Route& other) {
	if (this != &other) {
		_path = other._path;
		_allowedMethods = other._allowedMethods;
		_root = other._root;
		_autoindex = other._autoindex;
		_index = other._index;
		_redirect = other._redirect;
		_uploadPath = other._uploadPath;
		_cgiExtensions = other._cgiExtensions;
	}
	return *this;
}

Route::~Route() {
}

// Setters
void Route::setPath(const std::string& path) {
	_path = path;
}

void Route::addAllowedMethod(const std::string& method) {
	_allowedMethods.push_back(method);
}

void Route::setRoot(const std::string& root) {
	_root = root;
}

void Route::setAutoindex(bool autoindex) {
	_autoindex = autoindex;
}

void Route::addIndexFile(const std::string& file) {
	_index.push_back(file);
}

void Route::setRedirect(const std::string& redirect) {
	_redirect = redirect;
}

void Route::setUploadPath(const std::string& path) {
	_uploadPath = path;
}

void Route::addCgiExtension(const std::string& ext, const std::string& handler) {
	_cgiExtensions[ext] = handler;
}

// Getters
const std::string& Route::getPath() const {
	return _path;
}

const std::vector<std::string>& Route::getAllowedMethods() const {
	return _allowedMethods;
}

const std::string& Route::getRoot() const {
	return _root;
}

bool Route::getAutoindex() const {
	return _autoindex;
}

const std::vector<std::string>& Route::getIndex() const {
	return _index;
}

const std::string& Route::getRedirect() const {
	return _redirect;
}

const std::string& Route::getUploadPath() const {
	return _uploadPath;
}

const std::map<std::string, std::string>& Route::getCgiExtensions() const {
	return _cgiExtensions;
}

// Utility methods
bool Route::isMethodAllowed(const std::string& method) const {
	// TODO: Implementation
	(void)method;
	return false;
}

bool Route::hasCgiExtension(const std::string& ext) const {
	// TODO: Implementation
	(void)ext;
	return false;
}

std::string Route::getCgiHandler(const std::string& ext) const {
	// TODO: Implementation
	(void)ext;
	return "";
}

bool Route::matches(const std::string& requestPath) const {
	// TODO: Implementation
	(void)requestPath;
	return false;
}
