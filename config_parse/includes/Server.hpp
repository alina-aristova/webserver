#ifndef SERVER_HPP
# define SERVER_HPP

# include "Configuration.hpp"
# include "Location.hpp"

class Server
{
	typedef void (Configuration::*parseFunc)(std::vector<std::string> &);

	private:
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
		Server(void);
		Server(Server const & other);
		~Server(void);

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

};

#endif
