#ifndef LOCATION_HPP
# define LOCATION_HPP

# include "Configuration.hpp"
# include <map>
# include <list>

class Location
{
	typedef void (Location::*parseFunc)(std::vector<std::string> &);

	private:
		Location(void);
		std::string		_rootDirectory;
		std::string		_indexingFilePath;
		std::string		_storageDirectory;
		std::list<std::string>  _listOfAllowedMethods;
		std::map<std::string, std::string> _cgi;
		std::map<std::string, parseFunc> _directiveFields;

	public:
		Location(std::ifstream & ifs, std::string & buf);
		Location(Location const & other);
		~Location(void);

		/* ------------------------------- Интерфейсы ------------------------------- */
		std::string getRootDirectory(void) const {return _rootDirectory;}
		std::string getIndexingFilePath(void) const {return _indexingFilePath;}
		std::string		getStorageDirectory(void) const {return _storageDirectory;}
		std::list<std::string>  getListOfAllowedMethods(void) const {return _listOfAllowedMethods;}
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