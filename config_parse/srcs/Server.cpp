#include "../includes/Server.hpp"

Server::Server(std::ifstream & ifs, std::string & buf) : _port(DEFAULT_PORT), _autoindex(true), _hostName(DEFAULT_HOSTNAME), _rootDirectory(DEFAULT_ROOT_DIRECTORY),
_indexingFilePath(DEFAULT_INDEXING_FILE_PATH), _storageDirectory(DEFAULT_STORAGE_DIRECTORY), _clientMaxBodySize(DEFAULT_MAX_BODY_SIZE)
{
	_allowedMethods.push_back("GET");
	_allowedMethods.push_back("POST");
	_allowedMethods.push_back("DELETE");
	_parseServerFields["listen"] = &Server::parseListen;
	_parseServerFields["server_name"] = &Server::parseServerName;
	_parseServerFields["root"] = &Server::parseRootDirectory;
	_parseServerFields["index"] = &Server::parseIndexingFilePath;
	_parseServerFields["error_page"] = &Server::parseErrorPage;
	_parseServerFields["cgi"] = &Server::parseCgi;
	_parseServerFields["client_max_body_size"] = &Server::parseClientMaxBodySize;
	_parseServerFields["autoindex"] = &Server::parseAutoIndex;
	parseServerBlock(ifs, buf);
}

/* -------------------- Основной парсер серверного блока -------------------- */
void Server::parseServerBlock(std::ifstream & ifs, std::string & buf)
{
	std::map<std::string, parseFunc>::iterator fieldsIterator;
	std::string word;

	while (std::getline(ifs, buf) && buf.find('}') == std::string::npos)
	{
		if (buf.find_first_not_of(" \t\n\v\f\r") == std::string::npos)
			continue;
		std::vector<std::string> wordsInLine;
		std::istringstream streamForLine(buf);

		streamForLine >> word;
		if (word == "location")
		{
			streamForLine >> word;
			Location locationBlock(ifs, buf, *this);
			this->_locations.insert(std::make_pair(word, locationBlock));
		}
		else
		{
			fieldsIterator = this->_parseServerFields.find(word);
			if (fieldsIterator == this->_parseServerFields.end())
				throw Server::UnknownToken();
			parseFunc handler = (*fieldsIterator).second;
			while (streamForLine >> word)
				wordsInLine.push_back(word);
			(this->*handler)(wordsInLine);
		}	
	}
}

/* -------------------------- Методы парсинга полей ------------------------- */
void Server::parseListen(std::vector<std::string> & port)
{
	if (port.size() != 1)
		throw SyntaxError();
	this->_port = std::stoi(port[0]);
}

void Server::parseAutoIndex(std::vector<std::string> & index)
{
	if (index.size() != 1)
		throw SyntaxError();
	index[0].erase(index[0].find(";"));
	if (index[0] == "off")
		this->_autoindex = false;
	else if (index[0] != "on")
		throw SyntaxError();
}

void Server::parseServerName(std::vector<std::string> & server_name)
{
	if (server_name.size() != 1)
		throw SyntaxError();
	this->_hostName = server_name[0].erase(server_name[0].find(';'));
}

void Server::parseRootDirectory(std::vector<std::string> & root_directory)
{
	if (root_directory.size() != 1)
		throw SyntaxError();
	this->_rootDirectory = root_directory[0].erase(root_directory[0].find(';'));
}

void Server::parseIndexingFilePath(std::vector<std::string> & index)
{
	if (index.size() != 1)
		throw SyntaxError();
	this->_indexingFilePath= index[0].erase(index[0].find(';'));
}

void Server::parseErrorPage(std::vector<std::string> & error_info)
{
	if (error_info.size() != 2)
		throw SyntaxError();
	std::string error_code = error_info[0];
	this->_errorFilesPath[error_code] = error_info[1].erase(error_info[1].find(';'));
}

void Server::parseCgi(std::vector<std::string> & cgi_line)
{
	if (cgi_line.size() != 2)
		throw SyntaxError();
	std::string cgi_excention = cgi_line[0];
	this->_cgi[cgi_excention] = cgi_line[1].erase(cgi_line[1].find(';'));
	
}

void Server::parseClientMaxBodySize(std::vector<std::string> & body_size)
{
	if (body_size.size() != 1)
		throw SyntaxError();
	this->_clientMaxBodySize = body_size[0].erase(body_size[0].find(';'));
	std::stoi(this->_clientMaxBodySize);
}
