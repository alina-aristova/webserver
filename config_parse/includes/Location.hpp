#ifndef LOCATION_HPP
# define LOCATION_HPP

# include "Configuration.hpp"
# include <map>
# include <list>

class Server;

class Location
{
	typedef void (Location::*parseFunc)(std::vector<std::string> &);

	private:
		std::string		_rootDirectory;
		std::string		_indexingFilePath;
		std::string		_storageDirectory;
		std::string		_clientMaxBodySize;
		std::vector<std::string>  _allowedMethods;
		std::map<std::string, std::string> _cgi;
		std::map<std::string, parseFunc> _parseDirectiveFields;

		/* --------------------------------- Парсинг -------------------------------- */
		void parseLocationBlock(std::ifstream & ifs, std::string & buf);
		void parseAllowedMethods(std::vector<std::string> & methods);
		void parseRootDirectory(std::vector<std::string> & root_directory);
		void parseIndexingFilePath(std::vector<std::string> & index);
		void parseCgi(std::vector<std::string> & cgi_line);
		void parseClientMaxBodySize(std::vector<std::string> & body_size);

	public:
		Location(std::ifstream & ifs, std::string & buf, Server const & serverPlace);
		~Location(void) {};

		/* ------------------------------- Интерфейсы ------------------------------- */
		std::string getRootDirectory(void) const {return _rootDirectory;}
		std::string getIndexingFilePath(void) const {return _indexingFilePath;}
		std::string		getStorageDirectory(void) const {return _storageDirectory;}
		std::vector<std::string>  getAllowedMethods(void) const {return _allowedMethods;}
		std::map<std::string, std::string> getCgi(void) const {return _cgi;}

		/* ------------------------------- Исключения ------------------------------- */
		class UnknownToken : public std::exception
		{
			const char *what(void) const throw()
			{
				return "Unknown token found in location!";
			}		
		};

		class SyntaxError: public std::exception
		{
			const char *what(void) const throw()
			{
				return "SyntaxError in location!";
			}		
	};
};

#endif
