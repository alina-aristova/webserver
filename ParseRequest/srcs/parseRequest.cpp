#include "../Includes/parseRequest.hpp"
#include "../utils/utils.hpp"
ParseRequest::ParseRequest()
{

}

const String   &ParseRequest::getMethod() const
{
    return(this->_method);
}

const String   &ParseRequest::getBody() const
{
    return(this->_body);
}

const String   &ParseRequest::getPath() const
{
    return(this->_path);
}

const String   &ParseRequest::getVersProtocol() const
{
    return(this->_versProtocol);
}

void ParseRequest::parsingStartLine(String &line)
{
    // size_t pos = 0;
    // size_t i = 0;
    String temp;
    
    std::vector<String> newLine = split(line);
    if (newLine.size() != 3)
        return; 
    this->_method = newLine[0];
    this->_path = newLine[1];
    this->_versProtocol = newLine[2];
    if (this->_versProtocol != "HTTP/1.1")
        throw std::exception();
    // проверить на валидность параметры
} // еще не работает :(

void print(Map m) // функция для тестирования мапы
{
std::map<String,String>::iterator it;
 
for (it=m.begin(); it!=m.end(); it++)
std::cout << "Ключ: " << it->first << "| Значение: " << it->second << '\n';

}

void ParseRequest::parsingHeading(std::string request)
{
    std::vector<std::string> temp = split(request);
    this->_heading.insert(make_pair(temp[0].erase(temp[0].size() - 1),temp[1]));
}

void ParseRequest::parsRequest(String request)
{
    std::string r =  getLine(request);
    parsingStartLine(r);
     r =  getLine(request);
    while(r != "")
    {
        parsingHeading(r);
         r =  getLine(request);
    }
    
    if(this->_heading.count("Host")==0)
        std::cout << "host not found" << '\n';
    print(this->_heading);
    if(!request.empty())
    {
        this->_body = request;
    }

}