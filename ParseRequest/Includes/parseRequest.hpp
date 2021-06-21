#ifndef PARSEREQUEST_HPP
# define PARSEREQUEST_HPP

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
 # define String std::string
 # define Map std::map<std::string,std::string>
class ParseRequest
{
    private:
        String      _method;
        String      _path;
        String      _versProtocol;
         String      _body;
        Map         _heading;
    public: //  private?
        ParseRequest();

        //==================get/set================
        const String   &getMethod() const;
        const String   &getPath() const;
        const String   &getVersProtocol() const;
        const String   &getBody() const;
        void  parsingStartLine(String &line);
        void  parsingBody(String &line);
        void  parsingHeading(std::string res);
        void  parsRequest(String request);
        void  parsBody(String request);
};

#endif