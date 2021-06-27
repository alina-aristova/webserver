#include "../includes/HostClass.hpp"

HostClass::HostClass(std::string const & config_file) : _port(8080), _hostName(DEFAULT_HOSTNAME), _rootDirectory(DEFAULT_ROOT_DIRECTORY),
_indexingFilePath(DEFAULT_INDEXING_FILE_PATH), _errorFilePath(DEFAULT_ERROR_FILE_PAGE),
_storageDirectory(DEFAULT_STORAGE_DIRECTORY), _config_file(config_file)
{
	_listOfAllowedMethods.push_back("ADD");
	_listOfAllowedMethods.push_back("POST");
	_listOfAllowedMethods.push_back("DELETE");
	_directiveFields["allowed_methods"] = &HostClass::parseAllowedMethods;
	_serverFields["listen"] = &HostClass::parseListen;
}
// const std::string & HostClass::getHostName() const { return (_hostName); }

// int HostClass::getPort() const { return (_port); }

const std::string & HostClass::getRootDirectory() const { return (_rootDirectory); }

const std::string & HostClass::getIndexingFilePath() const { return (_indexingFilePath); }

const std::string & HostClass::getErrorFilePath() const { return (_errorFilePath); }

const std::string & HostClass::getStorageDirectory() const { return (_storageDirectory); }

const std::list<std::string> & HostClass::getListOfAllowedMethods() const { return (_listOfAllowedMethods); }

void HostClass::parseConfig(void)
{
	std::string buf;
	std::size_t found;

    std::ifstream ifs(this->_config_file, std::ios::in);
	if (ifs.fail())
	{
		std::cerr << "Error: " << strerror(errno) << std::endl;
		exit(-1);
	}
	while (std::getline(ifs, buf))
	{
		if (buf == "{" || buf == "}")
			continue;
		if ((found = buf.find("location")) == std::string::npos)
		{
			std::cout << static_cast<int>(buf[0]) << std::endl;
			break;
		}	
	}

}

void HostClass::parseAllowedMethods(std::string const & method)
{
	(void)method;
	return;
}

void HostClass::parseListen(std::string const & port)
{
	(void)port;
	return;
}
