#include "../Includes/parseRequest.hpp"
#include "../utils/utils.hpp"
ParseRequest::ParseRequest()
{

}

const String   &ParseRequest::getMethod() const
{
    return(this->_method);
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
    size_t pos = 0;
    size_t i = 0;
    String temp;
    
    std::vector<String> newLine = split(line);
    if (newLine.size() != 3)
        return; 
    this->_method = newLine[0];
    this->_path = newLine[1];
    this->_versProtocol = newLine[2];
    // проверить на валидность параметры
} // еще не работает :(
void print(Map m)
{
std::map<String,String>::iterator it;
 
for (it=m.begin(); it!=m.end(); it++)
std::cout << "Ключ: " << it->first << "| Значение: " << it->second << '\n';

}
void ParseRequest::parsingHeading(std::vector<std::string> res)
{
    int i = 1;
    while(res[i] != "")
    {
        std::vector<std::string> temp = split(res[i]);
       // std::cout << i << "|"<< temp[0]<< "|"<< temp[1] <<'\n';
        this->_heading.insert(make_pair(temp[0].erase(temp[0].size() - 1),temp[1]));
        i++;
    }
    print(this->_heading);
}

void ParseRequest::parsRequest(String request)
{
    std::vector<std::string> res = catLine(request);
    parsingStartLine(res[0]);
    parsingHeading(res);

}