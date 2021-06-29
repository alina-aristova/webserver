#include "../includes/Configuration.hpp"

// Configuration::Configuration(std::string const & config_file) : _hostName(DEFAULT_HOSTNAME), _rootDirectory(DEFAULT_ROOT_DIRECTORY),
// _indexingFilePath(DEFAULT_INDEXING_FILE_PATH), _storageDirectory(DEFAULT_STORAGE_DIRECTORY)
// {
// 	_listOfAllowedMethods.push_back("ADD");
// 	_listOfAllowedMethods.push_back("POST");
// 	_listOfAllowedMethods.push_back("DELETE");
// 	_directiveFields["allowed_methods"] = &Configuration::parseAllowedMethods;
// 	_serverFields["listen"] = &Configuration::parseListen;
// 	_serverFields["server_name"] = &Configuration::parseServerName;
// 	_serverFields["error_page"] = &Configuration::parseErrorPage;

// 	parseConfig(config_file);
// }

void Configuration::parseConfig(std::string const & config_file)
{
	std::string buf;
	std::string word;

    std::ifstream ifs(config_file, std::ios::in);
	if (ifs.fail())
	{
		std::cerr << "Error: " << strerror(errno) << std::endl;
		exit(-1);
	}
	while (std::getline(ifs, buf))
	{
		if (buf == "}" || buf.find_first_not_of(" \t\n\v\f\r") == std::string::npos)
			continue;
		if (buf == "{")
		{
			try
			{
				Server serverBlock(ifs, buf);
				this->_servers.push_back(serverBlock);
			}
			catch(const std::exception& e)
			{
				std::cerr << e.what() << std::endl;
			}
		}
	// 	std::istringstream streamForLine(buf);
	// 	std::vector<std::string> wordsInLine;
	// 	std::map<std::string, parseFunc>::iterator fieldsIterator;

	// /* ---------------------------- Server parsing --------------------------- */
	// 	if ((buf.find("location")) == std::string::npos)
	// 	{
	// 		streamForLine >> word;
	// 		fieldsIterator = this->_serverFields.find(word);
	// 		if (fieldsIterator == this->_serverFields.end())
	// 			throw Configuration::UnknownToken();
	// 		parseFunc handler = (*fieldsIterator).second;
	// 		while (streamForLine >> word)
	// 		{
	// 			// std::cout << word << std::endl;
	// 			wordsInLine.push_back(word);
	// 		}
	// 		(this->*handler)(wordsInLine);
			// break;
		}	
	}

}

// void Configuration::parseAllowedMethods(std::vector<std::string> & methods)
// {
// 	(void)methods;
// 	std::cout << "Found allowed_methods!" << std::endl;
// 	return;
// }

// void Configuration::parseListen(std::vector<std::string> & port)
// {
// 	if (port.size() != 1)
// 		throw SyntaxError();
// 	try
// 	{
// 		int listen_port = std::stoi(port[0]);
// 		this->_ports.push_back(listen_port);
// 	}
// 	catch(const std::exception& e)
// 	{
// 		std::cerr << "Syntax Error!" << std::endl;
// 	}
// 	return;
// }

// void Configuration::parseServerName(std::vector<std::string> & server_names)
// {
// 	if (server_names.size() != 1)
// 		throw SyntaxError();
// 	try
// 	{
// 		std::string host_name = server_names[0].erase(server_names[0].find(';'));
// 	}
// 	catch(const std::exception& e)
// 	{
// 		std::cerr << "Syntax Error!" << std::endl;
// 	}
// }

// void Configuration::parseErrorPage(std::vector<std::string> & error_info)
// {
// 	if (error_info.size() != 2)
// 		throw SyntaxError();
// 	try
// 	{
// 		int error_code = std::stoi(error_info[0]);
// 		this->_errorFilesPath[error_code] = error_info[1].erase(error_info[1].find(';'));
// 	}
// 	catch(const std::exception& e)
// 	{
// 		std::cerr << "Syntax Error!" << std::endl;
// 	}
// }
