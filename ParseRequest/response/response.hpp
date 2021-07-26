#ifndef RESPONSE_HPP
# define RESPONSE_HPP

 # include <iostream>
 # include <vector>
 # include <string>
 # include <algorithm>
 # include <exception>
 # include <map>
 # include <stdio.h>
 # include <string>
 # include <iostream>
 # include <sstream>
 # include <iomanip>
 # include <iostream>
 # include <fstream>
 # include <string>
 # include <sys/stat.h>
 # include "../Includes/parseRequest.hpp"
 # define String std::string
 # define Map std::map<std::string,std::string>
 # define Vector std::vector<std::string>
 # define ERROR -1
 # define SUCCESS 1
class Response
{
    private:
        String          _versProtocol;
        String          _NumError;
        String          _descriptionError;
        String          _contentType; 
        String          _FileLength;
        String          _File;
        int             _iNumError;
        unsigned long   _iFileLength;
        Map             _types;
        Map           _errorFilePath;

    public: //  private?
        Response();
        //~ParseRequest();
        //==================get/set================
        const String            &getVersProtocol() const;
        const String            &getNumError() const;
        const String            &getDescriptionError() const;
        const String            &getContentType() const;
        const String            &getFileLength() const;
        const int               &getINumError() const;
        const unsigned long     &getIFileLength() const;
        //===========Parsing methods===============
        String                  creatRespons(ParseRequest &Request, std::string numError, std::string result);
        String                  descriptionError(std::string numError);
        String                  getResponse(std::string Method, std::string numError);
        int                     findFile(std::string NewPath );
        String                  findType(std::string fn);
        void                    addTypes();
};

#endif
