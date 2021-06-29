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

	public:
		int _port; //8080
		std::string	_hostName; //default
		std::string		_rootDirectory; // /
		std::string		_indexingFilePath;// /index.html
		std::map<std::string, std::string>	_errorFilesPath; // 
		std::string		_storageDirectory; //
		std::vector<std::string>  _listOfAllowedMethods; //
		std::map<std::string, std::string> _cgi; //
		std::map<std::string, parseFunc> _parseServerFields; 
		std::map<std::string, Location> _locations; //

	public:
		Server(void);
		~Server(void) {}

		/* ------------------------------- Интерфейсы ------------------------------- */
		int		getPort(void) const {return _port;}
		std::string getHostName(void) const {return _hostName;}
		std::string getRootDirectory(void) const {return _rootDirectory;}
		std::string getIndexingFilePath(void) const {return _indexingFilePath;}
		std::map<std::string, std::string>	getErrorFilesPath(void) const {return _errorFilesPath;}
		std::string		getStorageDirectory(void) const {return _storageDirectory;}
		std::vector<std::string>  getListOfAllowedMethods(void) const {return _listOfAllowedMethods;}
		std::map<std::string, std::string> getCgi(void) const {return _cgi;}
		std::map<std::string,Location> getLocations(void) const {return _locations;}

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
