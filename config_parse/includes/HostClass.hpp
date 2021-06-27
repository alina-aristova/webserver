#ifndef HOSTCLASS_HPP
# define HOSTCLASS_HPP

# include <string>
# include <list>
# include <map>
# include <vector>
# include <fstream>
# include <iostream>

# define DEFAULT_HOSTNAME "default"
# define DEFAULT_ROOT_DIRECTORY "/"
# define DEFAULT_INDEXING_FILE_PATH "/index.html"
# define DEFAULT_ERROR_FILE_PAGE "/404.html"
# define DEFAULT_STORAGE_DIRECTORY "/storage/"

class HostClass 
{

    private:
        int        _port;
		std::string        _hostName;
		std::string             _rootDirectory;
        std::string             _indexingFilePath;
        std::string             _errorFilePath;
        std::string             _storageDirectory;
        std::string             _config_file;
        std::list<std::string>  _listOfAllowedMethods;
        std::map<std::string, void (HostClass::*)(std::string const &)> _directiveFields;
        std::map<std::string, void (HostClass::*)(std::string const &)> _serverFields;

    public:
        HostClass();
        HostClass(std::string const & config_file);
        // int getPort() const;
        // const std::string & getHostName() const;
        const std::string & getRootDirectory() const;
        const std::string & getIndexingFilePath() const;
        const std::string & getErrorFilePath() const;
        const std::string & getStorageDirectory() const;
        const std::list<std::string> & getListOfAllowedMethods() const;

        /* ------------------------------- Parser funcs ------------------------------ */
        void parseConfig(void);
        void parseAllowedMethods(std::string const & method);
        void parseListen(std::string const & port);
};

#endif
