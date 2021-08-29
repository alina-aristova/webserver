#ifndef SERVER_HPP
# define SERVER_HPP

# include "Configuration.hpp"
# include "Location.hpp"
# include <map>

# define DEFAULT_PORT 8080
# define DEFAULT_HOSTNAME "localhost"
# define DEFAULT_ROOT_DIRECTORY "/"
# define DEFAULT_INDEXING_FILE_PATH "/index.html"
# define DEFAULT_ERROR_FILE_PAGE "/404.html"
# define DEFAULT_STORAGE_DIRECTORY "/storage/"
# define DEFAULT_MAX_BODY_SIZE "-1"

class Location;

class Server
{
	typedef void (Server::*parseFunc)(std::vector<std::string> &);

	private:
		int _port;
		bool _autoindex;
		std::string	_hostName;
		std::string		_rootDirectory;
		std::string		_indexingFilePath;
		std::string		_storageDirectory;
		std::string		_clientMaxBodySize;
		std::map<std::string, std::string>	_errorFilesPath;
		std::vector<std::string>  _allowedMethods;
		std::map<std::string, std::string> _cgi;
		std::map<std::string, parseFunc> _parseServerFields;
		std::map<std::string, Location> _locations;

		/* --------------------------------- Парсинг -------------------------------- */
		void parseServerBlock(std::ifstream & ifs, std::string & buf);
		void parseListen(std::vector<std::string> & port);
		void parseServerName(std::vector<std::string> & server_names);
		void parseRootDirectory(std::vector<std::string> & root_directory);
		void parseIndexingFilePath(std::vector<std::string> & index);
		void parseErrorPage(std::vector<std::string> & error_info);
		void parseCgi(std::vector<std::string> & cgi_line);
		void parseClientMaxBodySize(std::vector<std::string> & body_size);
		void parseAutoIndex(std::vector<std::string> & index);

	public:
		Server(void);
		Server(std::ifstream & ifs, std::string & buf);
		~Server(void) {}

		/* ------------------------------- Интерфейсы ------------------------------- */
		int		getPort(void) const {return _port;}
		bool	getAutoIndex(void) const {return _autoindex;}
		std::string getHostName(void) const {return _hostName;}
		std::string getRootDirectory(void) const {return _rootDirectory;}
		std::string getIndexingFilePath(void) const {return _indexingFilePath;}
		std::map<std::string, std::string>	getErrorFilesPath(void) const {return _errorFilesPath;}
		std::string		getStorageDirectory(void) const {return _storageDirectory;}
		std::vector<std::string>  getAllowedMethods(void) const {return _allowedMethods;}
		std::map<std::string, std::string> getCgi(void) const {return _cgi;}
		std::map<std::string, Location> getLocations(void) const {return _locations;}
		std::string getMaxBodySize(void) const {return _clientMaxBodySize;}



		/* ------------------------------- Исключения ------------------------------- */
		class UnknownToken : public std::exception
		{
			const char *what(void) const throw()
			{
				return "Unknown token found!";
			}		
		};

		class SyntaxError: public std::exception
		{
			const char *what(void) const throw()
			{
				return "SyntaxError!";
			}		
	};
};

#endif
