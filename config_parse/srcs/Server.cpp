#include "Server.hpp"

Server::Server(std::ifstream & ifs, std::string buf) : _port(DEFAULT_PORT), _hostName(DEFAULT_HOSTNAME), _rootDirectory(DEFAULT_ROOT_DIRECTORY),
_indexingFilePath(DEFAULT_INDEXING_FILE_PATH), _storageDirectory(DEFAULT_STORAGE_DIRECTORY)
{
	_listOfAllowedMethods.push_back("ADD");
	_listOfAllowedMethods.push_back("POST");
	_listOfAllowedMethods.push_back("DELETE");
	_parseServerFields["listen"] = &Server::parseListen;
	// _parseServerFields["server_name"] = &Server::parseServerName;
	// _parseServerFields["error_page"] = &Server::parseErrorPage;
	parseServerBlock(ifs, buf);
}

Server::Server(Server const & obj)
{
	this->_port = obj.getPort();
	this->_hostName = obj.getHostName();
	this->_rootDirectory = obj.getRootDirectory();
	this->_indexingFi
}