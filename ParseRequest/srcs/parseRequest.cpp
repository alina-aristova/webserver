#include "../Includes/parseRequest.hpp"
#include "../utils/utils.hpp"
ParseRequest::ParseRequest() : _bodyLength(0) {}

const String   &ParseRequest::getMethod() const { return(this->_method); }

const String   &ParseRequest::getStr() const { return(this->_str); }

int ParseRequest::getBodyLength() const { return _bodyLength; }

const String   &ParseRequest::getStrPath() const { return(this->_strPath); }

const String   &ParseRequest::getContentType() const { return(this->_contentType); }

const String   &ParseRequest::getBody() const{return(this->_body);}

const String   &ParseRequest::getPath() const{return(this->_path);}

const String   &ParseRequest::getCode() const{return(this->_code);}

const String   &ParseRequest::getVersProtocol() const{return(this->_versProtocol);}

const unsigned long    &ParseRequest::getSizeFile() const{return(this->_sizeFile);}

const Map   &ParseRequest::getMap() const{return(this->_heading);}

 bool   ParseRequest::getForCgi() const{return(this->_forCgi);}

const Map  &ParseRequest::getType() const{return(this->_types);}

const std::string & ParseRequest::getRootDirectory() const { return (_rootDirectory); }

const std::string & ParseRequest::getIndexingFilePath() const { return (_indexingFilePath); }

const std::map<std::string,std::string>  &ParseRequest::getErrorFilePath() const { return (_errorFilePath); }

//=============================================================================
// Функция parsingStartLine проверяет первую строку запроса на наличие
// трех параметов.
// 1. Название запроса (_method);
// 2. URL (_path);
// 3. Версия протокола (_versProtocol);
// Проверяет три параметра на валидность, в случае ошибки устанавливает 
// в статус код ошибки (_code) значение 400 (Bad Request);
// При удачном выполении в статус код устанавливается значение 200 (OK)
//=============================================================================

error ParseRequest::parsingStartLine(String &line)
{
    String temp;

    std::vector<String> newLine = split(line);

    if (newLine.size() != 3)
    {
        this->_code = "400";
        return(BadRequest);
    }
    this->_method = newLine[0];
    this->_path = newLine[1];
    this->_versProtocol = newLine[2];
    if (this->_versProtocol != "HTTP/1.1")
    {
        this->_code = "400";
        return(BadRequest);
    }
    if (this->_method !=  "DELETE"
        && this->_method !=  "POST"
        && this->_method !=  "GET")
    { 
       this->_code = "400";
       return(BadRequest);
    }
    this->_code = "200";
    return(OK);

}

void print(Map m) // функция для тестирования мапы
{
    Map::iterator it;
    
    for (it=m.begin(); it!=m.end(); it++)
    std::cout << "Ключ: " << it->first << "| Значение: " << it->second << '\n';
}

void ParseRequest::addTypes()
{
    this->_types.insert( std::make_pair("aac","audio/aac"));
    this->_types.insert( std::make_pair("abw","application/x-abiword"));
    this->_types.insert( std::make_pair("arc","application/x-freearc"));
    this->_types.insert( std::make_pair("avi","video/x-msvideo"));
    this->_types.insert( std::make_pair("azw","application/vnd.amazon.ebook"));
    this->_types.insert( std::make_pair("bin","application/octet-stream"));
    this->_types.insert( std::make_pair("bmp","image/bmp"));
    this->_types.insert( std::make_pair("bz","application/x-bzip"));
    this->_types.insert( std::make_pair("bz2","application/x-bzip2"));
    this->_types.insert( std::make_pair("csh","application/x-csh"));
    this->_types.insert( std::make_pair("css","text/css"));
    this->_types.insert( std::make_pair("csv","text/csv"));
    this->_types.insert( std::make_pair("doc","application/msword"));
    this->_types.insert( std::make_pair("docx","application/vnd.openxmlformats-officedocument.wordprocessingml.document"));
    this->_types.insert( std::make_pair("eot","application/vnd.ms-fontobject"));
    this->_types.insert( std::make_pair("epub","application/epub+zip"));
    this->_types.insert( std::make_pair("gz","application/gzip"));
    this->_types.insert( std::make_pair("gif","image/gif"));
    this->_types.insert( std::make_pair("htm","text/html"));
    this->_types.insert( std::make_pair("html","text/html"));
    this->_types.insert( std::make_pair("ico","image/vnd.microsoft.icon"));
    this->_types.insert( std::make_pair("ics","text/calendar"));
    this->_types.insert( std::make_pair("jar","application/java-archive"));
    this->_types.insert( std::make_pair("jpeg","image/jpeg"));
    this->_types.insert( std::make_pair("jpg","image/jpeg"));
    this->_types.insert( std::make_pair("js","text/javascript"));
    this->_types.insert( std::make_pair("json","application/json"));
    this->_types.insert( std::make_pair("jsonld","application/ld+json"));
    this->_types.insert( std::make_pair("mid","audio/midi audio/x-midi"));
    this->_types.insert( std::make_pair("midi","audio/midi audio/x-midi"));
    this->_types.insert( std::make_pair("mjs","text/javascript"));
    this->_types.insert( std::make_pair("mp3","audio/mpeg"));
    this->_types.insert( std::make_pair("mpeg","video/mpeg"));
    this->_types.insert( std::make_pair("mpkg","application/vnd.apple.installer+xml"));
    this->_types.insert( std::make_pair("odp","application/vnd.oasis.opendocument.presentation"));
    this->_types.insert( std::make_pair("ods","application/vnd.oasis.opendocument.spreadsheet"));
    this->_types.insert( std::make_pair("odt","application/vnd.oasis.opendocument.text"));
    this->_types.insert( std::make_pair("oga","audio/ogg"));
    this->_types.insert( std::make_pair("ogv","video/ogg"));
    this->_types.insert( std::make_pair("ogx","application/ogg"));
    this->_types.insert( std::make_pair("opus","audio/opus"));
    this->_types.insert( std::make_pair("otf","font/otf"));
    this->_types.insert( std::make_pair("png","image/png"));
    this->_types.insert( std::make_pair("pdf","application/pdf"));
    this->_types.insert( std::make_pair("php","application/x-httpd-php"));
    this->_types.insert( std::make_pair("ppt","application/vnd.ms-powerpoint"));
    this->_types.insert( std::make_pair("pptx","application/vnd.openxmlformats-officedocument.presentationml.presentation"));
    this->_types.insert( std::make_pair("rar","application/vnd.rar"));
    this->_types.insert( std::make_pair("rtf","application/rtf"));
    this->_types.insert( std::make_pair("sh","application/x-sh"));
    this->_types.insert( std::make_pair("svg","image/svg+xml"));
    this->_types.insert( std::make_pair("swf","application/x-shockwave-flash"));
    this->_types.insert( std::make_pair("tar","application/x-tar"));
    this->_types.insert( std::make_pair("tif","image/tiff"));
    this->_types.insert( std::make_pair("tiff","image/tiff"));
    this->_types.insert( std::make_pair("ts","video/mp2t"));
    this->_types.insert( std::make_pair("ttf","font/ttf"));
    this->_types.insert( std::make_pair("txt","text/plain"));
    this->_types.insert( std::make_pair("vsd","application/vnd.visio"));
    this->_types.insert( std::make_pair("wav","audio/wav"));
    this->_types.insert( std::make_pair("weba","audio/webm"));
    this->_types.insert( std::make_pair("webm","video/webm"));
    this->_types.insert( std::make_pair("webp","image/webp"));
    this->_types.insert( std::make_pair("woff","font/woff"));
    this->_types.insert( std::make_pair("woff2","font/woff2"));
    this->_types.insert( std::make_pair("xhtml","application/xhtml+xml"));
    this->_types.insert( std::make_pair("xls","application/vnd.ms-excel"));
    this->_types.insert( std::make_pair("xlsx","application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"));
    this->_types.insert( std::make_pair("xul","application/vnd.mozilla.xul+xml"));
    this->_types.insert( std::make_pair("zip","application/zip"));
    this->_types.insert( std::make_pair("7z","application/x-7z-compressed"));
}

//=============================================================================
// Метод добавления всех возможных ключей 
//=============================================================================

void ParseRequest::addArrKeys()
{
    this->_keys.push_back("host");
    this->_keys.push_back("accept");
    this->_keys.push_back("accept-charset");
    this->_keys.push_back("accept-encoding");
    this->_keys.push_back("accept-language");
    this->_keys.push_back("authorization");
    this->_keys.push_back("content-disposition");
    this->_keys.push_back("expect");
    this->_keys.push_back("from");
    this->_keys.push_back("if-match");
    this->_keys.push_back("if-modified-since");
    this->_keys.push_back("if-none-match");
    this->_keys.push_back("content-length");
    this->_keys.push_back("content-location");
    this->_keys.push_back("content-type");
    this->_keys.push_back("content-version");
    this->_keys.push_back("title");
}

//=============================================================================
// Функция checkKey проверяет встретился ли поданный ключ (value) в массиве
// доступных ключей. Если ключ удалось найти возвращается true иначе false
//=============================================================================

bool ParseRequest::checkKey(String value)
{
    for(unsigned long i = 0; i < this->_keys.size(); i++)
    {
        if(this->_keys[i] == value)
            return true; 
    }
    return false;
}

//=============================================================================
// Функция parsingHeading добавляет заголовки в map контейнер заголовков.
// 1. Переводит строку в нижний регистр.
// 2. Делит по пробелам на токены.
// 3. Проверяет первый токен (название заголовка) на валидность (можем ли мы
// его добавить, не пришел ли заголовок второй раз)
// 4. Если проверка прошла успешно, добавляет пару в массив заголовков
// (ключ = название заголовка : значение) и устанавливает код ошибки 200 (ОК)
// иначе, устанавливает код ошибки 400 (Bad Request) и возвращает ошибку.
//=============================================================================

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
    if(this->_heading.count(key) == 1)
    {
        this-> _code = "400";
        return BadRequest;
    }
    this->_heading.insert(make_pair(key,temp[1]));
    this-> _code = "200";
    return OK;
}

void ParseRequest::parsGet()
{
    if (fileToString(this->getStrPath()) == IS_DIR) 
    {   
        dirToString(this->_indexingFilePath);
    }
    findType(this->getStrPath());
}

error ParseRequest::findLocation()
{

    this->_rootDirectory = this->_locations[this->_locationName].getRootDirectory();
    this->_indexingFilePath = this->_locations[this->_locationName].getIndexingFilePath();
    this->_cgi = this->_locations[this->_locationName].getCgi();
    this->_listOfAllowedMethods = this->_locations[this->_locationName].getAllowedMethods();
    this->_autoindex = this->_locations[this->_locationName].getAutoIndex();
    return(OK);
}
error ParseRequest::requestForCgi()
{
    return(OK);

}

error ParseRequest::typeDefinitionMethod()
{
    std::string fn = this->_path.substr(this->_path.find_last_of(".") + 1);
	std::cout << this->_path.find_last_of(".") + 1 << std::endl;
    std::cout << fn << "fn"<< std::endl;
    if (this->_path.find_last_of(".") != std::string::npos && this->_cgi.count(fn) == 1)
    {
        this->_forCgi = true;
        std::cout << this->_forCgi<< "this->_forCgi"<< std::endl;
    }
    else
	{
		this->_forCgi = false;
        requestForNotCgi();
	}
	std::cout << "Ошибка! " << this->_forCgi << std::endl;
    this->_rashirenie = fn;
    return(OK);
}

error ParseRequest::requestForNotCgi()
{

    if (this->_method == "GET")
    {
        //std::cout << "govno!!!\n";
        if (fileToString(this->getStrPath()) == IS_DIR) 
        {
            if (this->_autoindex)
            {
                DIR *myDirectory = opendir(this->getStrPath().c_str());
                std::stringstream ss;
                if (myDirectory != nullptr)
                {
                    struct dirent* entity;
                    entity = readdir(myDirectory);
                    ss << "<!DOCTYPE html>" << std::endl;
                    ss << "<html>\n<body>" << std::endl;
                    while (entity != NULL) {
                        ss << "<p><a href=\"./" << entity->d_name << "\">" << entity->d_name << "</a></p>" << std::endl;
                        entity = readdir(myDirectory);
                    }
                    ss << "</body>\n</html>" << std::endl;
                    closedir(myDirectory);
                }
                this->_str = ss.str();
                this->_sizeFile = _str.size();
                this->_code = "200";
                this->_contentType = "html";
            }
            else
                dirToString(this->_indexingFilePath);
        }
        findType(this->getStrPath());
        
    }
    else if (this->_method == "POST")
    {
        if (this->_body.find("\r\n") == std::string::npos) {
            this->_code = "400";
            this->_sizeFile = 0;
            this->_str = "";
            return (BadRequest);
        }
        std::string boundaryStr = this->_body.substr(0, this->_body.find("\r\n"));
        size_t boundaryStrSize = boundaryStr.size() + 2;
        size_t fileNameStartIndex = this->_body.find("filename=\"") + 10;
        if (this->_body.find("filename=\"") == std::string::npos) {
            this->_code = "400";
            this->_sizeFile = 0;
            this->_str = "";
            return (BadRequest);
        }
        size_t fileNameLength = this->_body.substr(fileNameStartIndex).find("\"");
        if (fileNameLength == std::string::npos) {
            this->_code = "400";
            this->_sizeFile = 0;
            this->_str = "";
            return (BadRequest);
        }
        std::string fileName = this->_body.substr(fileNameStartIndex, fileNameLength);
        //std::cout << "*****************************" << std::endl;
        //std::cout << fileName << std::endl;
        //std::cout << "*****************************" << std::endl;
        //std::cout << getStrPath() << std::endl;
        //std::cout << "*****************************" << std::endl;
        //std::cout << this->_body;
        if (this->_body.find("\r\n\r\n") == std::string::npos) {
            this->_code = "400";
            this->_sizeFile = 0;
            this->_str = "";
            return (BadRequest);
        }
        this->_body = this->_body.substr(this->_body.find("\r\n\r\n") + 4);
        size_t bodySize = this->_body.size() - boundaryStrSize - 4;
        this->_body = this->_body.substr(0, bodySize);
        //std::cout << "*****************************" << std::endl;

        std::string pathStr = getStrPath();
        if (pathStr[pathStr.size() - 1] != '/')
            pathStr += '/';
        std::fstream fs;
        fs.open(pathStr + fileName, std::fstream::in | std::fstream::out | std::fstream::app);
        if (fs.is_open()) {
            fs << this->_body << std::endl;
            fs.close();
            this->_str = "";
            this->_sizeFile = _str.size();
            this->_code = "200";
            this->_contentType = "html";
        }
        else {
            this->_code = "400";
            this->_sizeFile = 0;
            this->_str = "";
            return (BadRequest);
        }
    }
    else if (this->_method == "DELETE")
    {
        std::cout << this->_path;
        findPath(this->_rootDirectory);
         if(remove((getStrPath()).c_str()) == 0)
            std::cout << "удадилили\n";
         else
             std::cout << "------------------------" << std::endl << "не удалили" << std::endl;
        this->_code = "200";
        this->_sizeFile = 0;
    }
    return(OK);
}

//=============================================================================
// главная функция парсинга запроса
//
//
//
//
//=============================================================================

error ParseRequest::parsRequest(String request, Server host, String NumCode)
{
    this->_errorFilePath = host.getErrorFilesPath();
    this->_rootDirectory = host.getRootDirectory();
    this->_indexingFilePath = host.getIndexingFilePath();
    this->_locations = host.getLocations();
    this->_autoindex = host.getAutoIndex();
    if (NumCode != "200")
    {
        this->_code = NumCode;
        return(BadRequest);
    }
    addArrKeys();
    addTypes();
    String str =  getLine(request);
    if (parsingStartLine(str) != OK)
        return(BadRequest);
    std::cout << "govno!!!\n";
    int i = 1;
   // int count = 0;
   while(this->_path[i] && this->_path[i] != '/' )
        i++;
    this->_locationName = this->_path.substr(0,i);
    std::cout << this->_locationName << "\n";
    if(this->_locations.count(this->_locationName) != 0)
    {
        std::cout << "stop123\n";
        findLocation();
    }

    std::cout << "stop\n";
    findPath(this->_rootDirectory);
    str =  getLine(request);
    while(str != "")
    {
        if (parsingHeading(str) == BadRequest)
            return BadRequest;
        str = getLine(request);
    }
    std::cout << "stop1\n";
    if(this->_heading.count("host") == 0)
    {
        this-> _code = "400";
        return BadRequest;
    }
    std::cout << "stop2\n";
    print(this->_heading);
    if(!request.empty())
	{
		std::cout << "Hello Govno!" << std::endl;
        this->_body = request;
	}
    typeDefinitionMethod();

    return OK;
}

//=============================================================================
//
//
// //какая то функция которую написал Федя, вроде она ищет Content-length
//
//
//=============================================================================

error ParseRequest::parsBodyLength(std::string &request) {
    size_t contentLengthStart;
    size_t contentLengthEnd;
    if ((contentLengthStart = request.find("Content-length: "))
        == String::npos) {
        if ((contentLengthStart = request.find("Content-Length: "))
            == String::npos)
        {   this->_code = "200"; 
            return OK;}
    }
    if ((contentLengthEnd = request.find("\r\n", contentLengthStart)) == String::npos) {
        if ((contentLengthEnd = request.find("\r\n", contentLengthStart)) == String::npos)
           { this->_code = "200";
               return OK;}
    }
    String contentLengthString = request.substr(contentLengthStart, contentLengthEnd - contentLengthStart);
    std::vector<String> contentLengthParts = split(contentLengthString);

    if (contentLengthParts.size() != 2)
    {
        this->_code = "400";
        return BadRequest;
    }
    int i = 0;
    while(contentLengthParts[1][i] != '\0')
    {
        if(!(isdigit(contentLengthParts[1][i])))
        {   
            this->_code = "400";  
            return BadRequest;
        }
        i++;
    }
    _bodyLength = std::stoi(contentLengthParts[1]);
    this->_code = "200";
    return (OK);
}

//=============================================================================
//
//
//
//
//
//=============================================================================

void ParseRequest::findPath(std::string root)
{
    
    if (root.back() == '/' && this->_path.front() == '/')
    {
        this->_strPath = (root.substr(0,root.length() - 1) + this->_path);
        return;
    } 
    if(root.back() != '/' && this->_path.front() != '/')
    { 
          this->_strPath = (root + "/" + this->_path);
           return;
    }
    this->_strPath = (root + this->_path);
}

//=============================================================================
//
//
//
//
//
//=============================================================================

void ParseRequest::findNewPath(std::string indexFile)
{
    
    if ( this->_strPath.back() == '/' && indexFile.front() == '/')
    {
        this->_strPath = (this->_strPath.substr(0,this->_strPath.length() - 1) + indexFile);
        return;
    } 
    if(this->_strPath.back() != '/' && indexFile.front() != '/')
    { 
          this->_strPath = (this->_strPath + "/" + indexFile);
           return;
    }
    this->_strPath = (this->_strPath + indexFile);
}

//=============================================================================
// Функция принимает на вход имя файла (fn), находит его расширение, 
// проверяет есть ли данное расширение в заданном заранее Map контейнере, 
// если ключ удалось найти, устанавливает в MIME type (this->_contentType)
// значение найденное по ключу. 
//=============================================================================

void ParseRequest::findType(std::string fn)
{   
    std::string fn1 = fn.substr(fn.find_last_of(".") + 1);
    if(this->_types.count(fn1) == 1)
        this->_contentType = _types[fn1];
}

//=============================================================================
// Функция dirToString вызывается в том случае, если в URL подали путь не до
// файла, а до директории. (в котором нужно найти Index File)
// Получаем путь до Index File (std::string indexFile). Если файл удалось найти
// и открыть, считываем из него информацию в буфер (this->_str). В переменную
// this->_sizeFile помещаем длину буфера и устанавливаем статус код 200 (OK).
// Иначе устанавливаем статус код 404 (Not Found) и возвращаем ошибку.
//=============================================================================

error ParseRequest::dirToString(std::string indexFile)
{
    findNewPath(indexFile);
    std::ifstream file(this->_strPath);  
     std::cout  << "|"<< this->_strPath << "|"<< std::endl;
    if (!file.is_open()) 
    {
        this->_code = "404";
        return(NotFound);
    }
    std::string   str((std::istreambuf_iterator<char>(file)),
                    std::istreambuf_iterator<char>());
    this->_str = str;
    this->_sizeFile = str.size();
    this->_code = "200";
    return(OK);
}

//=============================================================================
//
//
//
//
//
//=============================================================================

 error ParseRequest::fileToString(std::string root)
 {
    struct stat stat1;
    
    if (root.back() == '/' || stat(root.c_str(), &stat1))//!!!
        return(IS_DIR);
    std::cout  << "|"<< root << "|"<< std::endl;
    std::ifstream file(root);  
    if (!file.is_open()) 
    {
        this-> _code = "404";
        return(NotFound);
    }
     std::string   str((std::istreambuf_iterator<char>(file)),
                    std::istreambuf_iterator<char>());
    this->_str = str;
    this->_sizeFile = str.size();
    this-> _code = "200";
    return(OK);
 }
