#include "./HostClass.hpp"

HostClass::HostClass() : _port(8080), _hostName(DEFAULT_HOSTNAME), _rootDirectory(DEFAULT_ROOT_DIRECTORY),
_indexingFilePath(DEFAULT_INDEXING_FILE_PATH), _errorFilePath(DEFAULT_ERROR_FILE_PAGE),
_storageDirectory(DEFAULT_STORAGE_DIRECTORY) {
    _listOfAllowedMethods.push_back("ADD");
    _listOfAllowedMethods.push_back("POST");
    _listOfAllowedMethods.push_back("DELETE");
}

const std::string & HostClass::getHostName() const { return (_hostName); }

int HostClass::getPort() const { return (_port); }

const std::string & HostClass::getRootDirectory() const { return (_rootDirectory); }

const std::string & HostClass::getIndexingFilePath() const { return (_indexingFilePath); }

const std::string & HostClass::getErrorFilePath() const { return (_errorFilePath); }

const std::string & HostClass::getStorageDirectory() const { return (_storageDirectory); }

const std::list<std::string> & HostClass::getListOfAllowedMethods() const { return (_listOfAllowedMethods); }

void HostClass::setPort(int port) { _port = port; }

void HostClass::setHostName(const std::string & hostName) { _hostName = hostName; }

void HostClass::setRootDirectory(const std::string & rootDirectory) { _rootDirectory = rootDirectory; }

void HostClass::setIndexingFilePath(const std::string & indexingFilePath) { _indexingFilePath = indexingFilePath; }

void HostClass::setErrorFilePath(const std::string & errorFilePath) { _errorFilePath = errorFilePath; }

void HostClass::setStorageDirectory(const std::string & storageDirectory) { _storageDirectory = storageDirectory; }

void HostClass::setListOfAllowedMethods(const std::list <std::string> listOfAllowedMethods) {
    _listOfAllowedMethods = listOfAllowedMethods;
}