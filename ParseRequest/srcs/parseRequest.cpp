#include "../Includes/parseRequest.hpp"
#include "../utils/utils.hpp"
ParseRequest::ParseRequest() : _bodyLength(0) {}

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
const Map   &ParseRequest::getMap() const
{
    return(this->_heading);
}

error ParseRequest::parsingStartLine(String &line)
{
    String temp;

    std::vector<String> newLine = split(line);
    if (newLine.size() != 3)
        return (BadRequest);
    this->_method = newLine[0];
    this->_path = newLine[1];
    this->_versProtocol = newLine[2];
    if (this->_versProtocol != "HTTP/1.1")
        return(BadRequest);
    if (this->_method !=  "DELETE"
        && this->_method !=  "POST"
        && this->_method !=  "GET")
        return(BadRequest);
    return(OK);
    // проверить на валидность параметры
} // еще не работает :(

void print(Map m) // функция для тестирования мапы
{
Map::iterator it;
 
for (it=m.begin(); it!=m.end(); it++)
std::cout << "Ключ: " << it->first << "| Значение: " << it->second << '\n';

}

// //=========проверяет ключь на валидность===========
// static bool checkKey(String value)
// {
//     for(int i = 0; i < this->Keys.size(); i++)
//     {
//         if(Keys[i] == value)
//             return true; 
//     }
//     return false;
// }

//=========добавляет в map ключ:значение===========
error ParseRequest::parsingHeading(String request)
{
    std::vector<String> temp = split(request);
    String key = temp[0].erase(temp[0].size() - 1);
    // if(!checkKey(key))
    //      return BadRequest;
    this->_heading.insert(make_pair(key,temp[1]));
    return OK;
}

//=========главная функция парсинга запроса========
error ParseRequest::parsRequest(String request)
{
    String str =  getLine(request);
    parsingStartLine(str);
    str =  getLine(request);
    while(str != "")
    {
        if (parsingHeading(str) == BadRequest)
            return BadRequest;
        str =  getLine(request);
    }
    
    if(this->_heading.count("Host") == 0)
       return BadRequest;

    print(this->_heading);
    if(!request.empty())
    {
        this->_body = request;
    }
    return OK;
}

int ParseRequest::getBodyLength() const { return _bodyLength; }

error ParseRequest::parsBodyLength(std::string &request) {
    size_t contentLengthStart;
    size_t contentLengthEnd;
    if ((contentLengthStart = request.find("Content-length: "))
        == String::npos) {
        if ((contentLengthStart = request.find("Content-Length: "))
            == String::npos)
            return OK;
    }
    if ((contentLengthEnd = request.find("\r\n", contentLengthStart)) == String::npos) {
        if ((contentLengthEnd = request.find("\r\n", contentLengthStart)) == String::npos)
            return OK;
    }
    String contentLengthString = request.substr(contentLengthStart, contentLengthEnd - contentLengthStart);
    std::vector<String> contentLengthParts = split(contentLengthString);

    if (contentLengthParts.size() != 2)
        return (BadRequest);

    _bodyLength = std::stoi(contentLengthParts[1]);
    return (OK);
}