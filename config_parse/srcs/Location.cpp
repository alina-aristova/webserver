#include "../includes/Location.hpp"
#include <algorithm>
Location::Location(){}
Location::Location(std::ifstream & ifs, std::string & buf, Server const & serverPlace): _autoindex(serverPlace.getAutoIndex()),
_rootDirectory(serverPlace.getRootDirectory()), _indexingFilePath(serverPlace.getIndexingFilePath()), 
_storageDirectory(serverPlace.getStorageDirectory()), _clientMaxBodySize(DEFAULT_MAX_BODY_SIZE),
_allowedMethods(serverPlace.getAllowedMethods())
{
	_parseDirectiveFields["allowed_methods"] = &Location::parseAllowedMethods;
	_parseDirectiveFields["root"] = &Location::parseRootDirectory;
	_parseDirectiveFields["index"] = &Location::parseIndexingFilePath;
	_parseDirectiveFields["cgi"] = &Location::parseCgi;
	_parseDirectiveFields["client_max_body_size"] = &Location::parseClientMaxBodySize;
	_parseDirectiveFields["autoindex"] = &Location::parseAutoIndex;

	parseLocationBlock(ifs, buf);
}

/* ------------------------ Основной парсер директивы ----------------------- */
void Location::parseLocationBlock(std::ifstream & ifs, std::string & buf)
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
		fieldsIterator = this->_parseDirectiveFields.find(word);
		if (fieldsIterator == this->_parseDirectiveFields.end())
			throw Location::UnknownToken();
		parseFunc handler = (*fieldsIterator).second;
		while (streamForLine >> word)
		{
			// std::cout << "PARSED IN LOCATION: " << word << std::endl;
			wordsInLine.push_back(word);
		}
		(this->*handler)(wordsInLine);
	}
}

void Location::parseAutoIndex(std::vector<std::string> & index)
{
	if (index.size() != 1)
		throw SyntaxError();
	index[0].erase(index[0].find(";"));
	if (index[0] == "off")
		this->_autoindex = false;
	else if (index[0] != "on")
		throw SyntaxError();
}

void Location::parseAllowedMethods(std::vector<std::string> & methods)
{
	if (methods.size() == 0)
		throw SyntaxError();
	std::string element;
	std::vector<std::string>::iterator it = methods.begin();
	while (it != methods.end())
	{
		element = *it;
		if (element.find(';') != std::string::npos)
			this->_allowedMethods.push_back(element.erase(element.find(';')));
		else
			this->_allowedMethods.push_back(element);
		it++;
	}
}

void Location::parseRootDirectory(std::vector<std::string> & root_directory)
{
	if (root_directory.size() != 1)
		throw SyntaxError();
	try
	{
		this->_rootDirectory = root_directory[0].erase(root_directory[0].find(';'));
	}
	catch(const std::exception& e)
	{
		std::cerr << "Syntax Error!" << std::endl;
	}
}

void Location::parseIndexingFilePath(std::vector<std::string> & index)
{
	if (index.size() != 1)
		throw SyntaxError();
	this->_indexingFilePath= index[0].erase(index[0].find(';'));
}

void Location::parseCgi(std::vector<std::string> & cgi_line)
{
	if (cgi_line.size() != 2)
		throw SyntaxError();
	std::string cgi_excention = cgi_line[0];
	cgi_excention.erase(0, 2);
	this->_cgi[cgi_excention] = cgi_line[1].erase(cgi_line[1].find(';'));
}

void Location::parseClientMaxBodySize(std::vector<std::string> & body_size)
{
	if (body_size.size() != 1)
		throw SyntaxError();
	this->_clientMaxBodySize = body_size[0].erase(body_size[0].find(';'));
	std::stoi(this->_clientMaxBodySize);
}
