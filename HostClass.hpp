#ifndef HOST_CLASS_HPP
# define HOST_CLASS_HPP

# include <string>
# include <list>

# define DEFAULT_HOSTNAME "default"
# define DEFAULT_ROOT_DIRECTORY "/"
# define DEFAULT_INDEXING_FILE_PATH "/index.html"
# define DEFAULT_ERROR_FILE_PAGE "/404.html"
# define DEFAULT_STORAGE_DIRECTORY "/storage/"

class HostClass {
private:
    int                     _port;
    std::string             _hostName;
    std::string             _rootDirectory;
    std::string             _indexingFilePath;
    std::string             _errorFilePath;
    std::string             _storageDirectory;
    std::list<std::string>  _listOfAllowedMethods;
public:
    HostClass();
    int getPort() const;
    const std::string & getHostName() const;
    const std::string & getRootDirectory() const;
    const std::string & getIndexingFilePath() const;
    const std::string & getErrorFilePath() const;
    const std::string & getStorageDirectory() const;
    const std::list<std::string> & getListOfAllowedMethods() const;
    void setPort(int port);
    void setHostName(const std::string & hostName);
    void setRootDirectory(const std::string & rootDirectory);
    void setIndexingFilePath(const std::string & indexingFilePath);
    void setErrorFilePath(const std::string & errorFilePath);
    void setStorageDirectory(const std::string & storageDirectory);
    void setListOfAllowedMethods(const std::list<std::string> listOfAllowedMethods);
};

#endif