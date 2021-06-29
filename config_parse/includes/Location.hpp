#ifndef LOCATION_HPP
# define LOCATION_HPP

# include "Configuration.hpp"
# include <map>
# include <list>

# define DEFAULT_PORT 8080
# define DEFAULT_HOSTNAME "default"
# define DEFAULT_ROOT_DIRECTORY "/"
# define DEFAULT_INDEXING_FILE_PATH "/index.html"
# define DEFAULT_ERROR_FILE_PAGE "/404.html"
# define DEFAULT_STORAGE_DIRECTORY "/storage/"

class Location
{
	typedef void (Location::*parseFunc)(std::vector<std::string> &);

	private:
		
		std::string		_rootDirectory;
		std::string		_indexingFilePath;
		std::string		_storageDirectory;
		std::vector<std::string>  _listOfAllowedMethods;
		std::map<std::string, std::string> _cgi;
		std::map<std::string, parseFunc> _directiveFields;

	public:
		Location(void) : _rootDirectory(DEFAULT_ROOT_DIRECTORY),
		_indexingFilePath(DEFAULT_INDEXING_FILE_PATH), _storageDirectory(DEFAULT_STORAGE_DIRECTORY)
		{}
		~Location(void) {}

		/* ------------------------------- Интерфейсы ------------------------------- */
		std::string getRootDirectory(void) const {return _rootDirectory;}
		std::string getIndexingFilePath(void) const {return _indexingFilePath;}
		std::string		getStorageDirectory(void) const {return _storageDirectory;}
		std::vector<std::string>  getListOfAllowedMethods(void) const {return _listOfAllowedMethods;}
		std::map<std::string, std::string> getCgi(void) const {return _cgi;}

		/* --------------------------------- Парсинг -------------------------------- */
		void parseLocationBlock(std::ifstream & ifs, std::string & buf);
		void parseAllowedMethods(std::vector<std::string> & methods);








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
