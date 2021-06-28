#ifndef CONFIGURATION_HPP
# define CONFIGURATION_HPP

# include <string>
# include <list>
# include <map>
# include <vector>
# include <fstream>
# include <iostream>
# include <sstream>
# include <exception>

# define DEFAULT_HOSTNAME "default"
# define DEFAULT_ROOT_DIRECTORY "/"
# define DEFAULT_INDEXING_FILE_PATH "/index.html"
# define DEFAULT_ERROR_FILE_PAGE "/404.html"
# define DEFAULT_STORAGE_DIRECTORY "/storage/"


class Configuration 
{
	typedef void (Configuration::*parseFunc)(std::vector<std::string> &);

	private:
		std::vector<int>	_ports;
		std::string	_hostName;
		std::string		_rootDirectory;
		std::string		_indexingFilePath;
		std::map<int, std::string>	_errorFilesPath;
		std::string		_storageDirectory;
		std::list<std::string>  _listOfAllowedMethods;
		std::map<std::string, parseFunc> _directiveFields;
		std::map<std::string, parseFunc> _serverFields;

		/* ------------------------------- Parser funcs ------------------------------ */

		void parseConfig(std::string const & config_file);
		void parseAllowedMethods(std::vector<std::string> & methods);
		void parseListen(std::vector<std::string> & port);
		void parseServerName(std::vector<std::string> & server_names);
		void parseErrorPage(std::vector<std::string> & error_info);

	public:
		Configuration();
		Configuration(std::string const & config_file);
		// int getPort() const;
		// const std::string & getHostName() const;
		const std::string & getRootDirectory() const;
		const std::string & getIndexingFilePath() const;
		const std::map<int, std::string> & getErrorFilePath() const;
		const std::string & getStorageDirectory() const;
		const std::list<std::string> & getListOfAllowedMethods() const;
	
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
