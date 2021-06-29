#ifndef SERVER_HPP
# define SERVER_HPP

# include "Configuration.hpp"
# include "Location.hpp"

# define DEFAULT_PORT 8080
# define DEFAULT_HOSTNAME "default"
# define DEFAULT_ROOT_DIRECTORY "/"
# define DEFAULT_INDEXING_FILE_PATH "/index.html"
# define DEFAULT_ERROR_FILE_PAGE "/404.html"
# define DEFAULT_STORAGE_DIRECTORY "/storage/"

class Server
{
	typedef void (Server::*parseFunc)(std::vector<std::string> &);

	private:
		Server(void);
		int _port;
		std::string	_hostName;
		std::string		_rootDirectory;
		std::string		_indexingFilePath;
		std::map<int, std::string>	_errorFilesPath;
		std::string		_storageDirectory;
		std::list<std::string>  _listOfAllowedMethods;
		std::map<std::string, std::string> _cgi;
		std::map<std::string, parseFunc> _parseServerFields;
		std::vector<Location> _locations;

	public:
		Server(std::ifstream & ifs, std::string & buf);
		Server(Server const & other);
		~Server(void) {}

		/* ------------------------------- Интерфейсы ------------------------------- */
		int		getPort(void) const {return _port;}
		std::string getHostName(void) const {return _hostName;}
		std::string getRootDirectory(void) const {return _rootDirectory;}
		std::string getIndexingFilePath(void) const {return _indexingFilePath;}
		std::map<int, std::string>	getErrorFilesPath(void) const {return _errorFilesPath;}
		std::string		getStorageDirectory(void) const {return _storageDirectory;}
		std::list<std::string>  getListOfAllowedMethods(void) const {return _listOfAllowedMethods;}
		std::map<std::string, std::string> getCgi(void) const {return _cgi;}
		std::vector<Location> getLocations(void) const {return _locations;}

		/* --------------------------------- Парсинг -------------------------------- */
		void parseServerBlock(std::ifstream & ifs, std::string & buf);
		void parseAllowedMethods(std::vector<std::string> & methods);
		void parseListen(std::vector<std::string> & port);
		void parseServerName(std::vector<std::string> & server_names);
		void parseErrorPage(std::vector<std::string> & error_info);





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
