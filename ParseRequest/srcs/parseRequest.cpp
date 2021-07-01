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
        throw std::exception();
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

//=========добавляет все возможные ключи=============

void ParseRequest::addArrKeys()
{
    this->Keys.push_back("host");
    this->Keys.push_back("accept");
    this->Keys.push_back("accept-charset");
    this->Keys.push_back("accept-encoding");
    this->Keys.push_back("accept-language");
    this->Keys.push_back("authorization");
    this->Keys.push_back("content-disposition");
    this->Keys.push_back("expect");
    this->Keys.push_back("from");
    this->Keys.push_back("if-match");
    this->Keys.push_back("if-modified-since");
    this->Keys.push_back("if-none-match");
    this->Keys.push_back("content-length");
    this->Keys.push_back("content-location");
    this->Keys.push_back("content-type");
    this->Keys.push_back("content-version");
    this->Keys.push_back("title");

}
//=========проверяет ключ на валидность===========
bool ParseRequest::checkKey(String value)
{
    
    for(unsigned long i = 0; i < this->Keys.size(); i++)
    {
        if(this->Keys[i] == value)
            return true; 
    }
    return false;
}

//=========добавляет в map ключ:значение===========
error ParseRequest::parsingHeading(String request)
{
    int i = 0;
    while(request[i] != '\0')
	{
		request[i] = std::tolower(request[i]);
		i++;
	}
    std::vector<String> temp = split(request);
    String key = temp[0].erase(temp[0].size() - 1);
    if(!checkKey(key) || this->_heading.count(key) == 1)
    {
         std::cout <<" error" << std::endl;
        return BadRequest;
    }
    this->_heading.insert(make_pair(key,temp[1]));
    return OK;
}

//=========главная функция парсинга запроса========
error ParseRequest::parsRequest(String request)
{
    std::cout << request << std::endl;
    String str =  getLine(request);
    parsingStartLine(str);
    str =  getLine(request);
    while(str != "")
    {
       // std::cout <<" request" << std::endl;
        if (parsingHeading(str) == BadRequest)
            return BadRequest;
        str =  getLine(request);
    }
    
    if(this->_heading.count("host") == 0)
       return BadRequest;

    print(this->_heading);
    if(!request.empty())
    {
        this->_body = request;
    }
    return OK;
}

int ParseRequest::getBodyLength() const { return _bodyLength; }

//какая то функция которую написал Федя, вроде она ищет Content-length
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
        return BadRequest;
    int i = 0;
    while(contentLengthParts[1][i] != '\0')
    {
        if(!(isdigit(contentLengthParts[1][i])))
             return BadRequest;
        i++;
    }

    _bodyLength = std::stoi(contentLengthParts[1]);
    return (OK);
}