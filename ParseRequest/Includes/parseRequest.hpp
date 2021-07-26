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
 # include <iostream>
 # include <fstream>
 # include <string>
 # include <sys/stat.h>
 # include "../../config_parse/includes/Configuration.hpp"

 # define String std::string
 # define Map std::map<std::string,std::string>
 # define Vector std::vector<std::string>
//  100 Continue («продолжай»)[2][3];
// 101 Switching Protocols («переключение протоколов»)[2][3];
// 102 Processing («идёт обработка»);
// 103 Early Hints («ранняя метаинформация»);
// 2xx: Success (успешно):
// 200 OK («хорошо»)[2][3];
// 201 Created («создано»)[2][3][4];
// 202 Accepted («принято»)[2][3];
// 203 Non-Authoritative Information («информация не авторитетна»)[2][3];
// 204 No Content («нет содержимого»)[2][3];
// 205 Reset Content («сбросить содержимое»)[2][3];
// 206 Partial Content («частичное содержимое»)[2][3];
// 207 Multi-Status («многостатусный»)[5];
// 208 Already Reported («уже сообщалось»)[6];
// 226 IM Used («использовано IM»).
// 3xx: Redirection (перенаправление):
// 300 Multiple Choices («множество выборов»)[2][7];
// 301 Moved Permanently («перемещено навсегда»)[2][7];
// 302 Moved Temporarily («перемещено временно»)[2][7], 302 Found («найдено»)[7];;
// 303 See Other («смотреть другое»)[2][7];
// 304 Not Modified («не изменялось»)[2][7];
// 305 Use Proxy («использовать прокси»)[2][7];
// 306 — зарезервировано (код использовался только в ранних спецификациях)[7];
// 307 Temporary Redirect («временное перенаправление»)[7];
// 308 Permanent Redirect («постоянное перенаправление»)[8].
// 4xx: Client Error (ошибка клиента):
// 400 Bad Request («неправильный, некорректный запрос»)[2][3][4];
// 401 Unauthorized («не авторизован (не представился)»)[2][3];
// 402 Payment Required («необходима оплата»)[2][3];
// 403 Forbidden («запрещено (не уполномочен)»)[2][3];
// 404 Not Found («не найдено»)[2][3];
// 405 Method Not Allowed («метод не поддерживается»)[2][3];
// 406 Not Acceptable («неприемлемо»)[2][3];
// 407 Proxy Authentication Required («необходима аутентификация прокси»)[2][3];
// 408 Request Timeout («истекло время ожидания»)[2][3];
// 409 Conflict («конфликт»)[2][3][4];
// 410 Gone («удалён»)[2][3];
// 411 Length Required («необходима длина»)[2][3];
// 412 Precondition Failed («условие ложно»)[2][3][9];
// 413 Payload Too Large («полезная нагрузка слишком велика»)[2][3];
// 414 URI Too Long («URI слишком длинный»)[2][3];
// 415 Unsupported Media Type («неподдерживаемый тип данных»)[2][3];
// 416 Range Not Satisfiable («диапазон не достижим»)[3];
// 417 Expectation Failed («ожидание не удалось»)[3];
// 418 I’m a teapot («я — чайник»);
// 419 Authentication Timeout (not in RFC 2616) («обычно ошибка проверки CSRF»);
// 421 Misdirected Request [10];
// 422 Unprocessable Entity («необрабатываемый экземпляр»);
// 423 Locked («заблокировано»);
// 424 Failed Dependency («невыполненная зависимость»);
// 425 Too Early («слишком рано»);
// 426 Upgrade Required («необходимо обновление»);
// 428 Precondition Required («необходимо предусловие»)[11];
// 429 Too Many Requests («слишком много запросов»)[11];
// 431 Request Header Fields Too Large («поля заголовка запроса слишком большие»)[11];
// 449 Retry With («повторить с»)[1];
// 451 Unavailable For Legal Reasons («недоступно по юридическим причинам»)[12].
// 499 Client Closed Request (клиент закрыл соединение);
// 5xx: Server Error (ошибка сервера):
// 500 Internal Server Error («внутренняя ошибка сервера»)[2][3];
// 501 Not Implemented («не реализовано»)[2][3];
// 502 Bad Gateway («плохой, ошибочный шлюз»)[2][3];
// 503 Service Unavailable («сервис недоступен»)[2][3];
// 504 Gateway Timeout («шлюз не отвечает»)[2][3];
// 505 HTTP Version Not Supported («версия HTTP не поддерживается»)[2][3];
// 506 Variant Also Negotiates («вариант тоже проводит согласование»)[13];
// 507 Insufficient Storage («переполнение хранилища»);
// 508 Loop Detected («обнаружено бесконечное перенаправление»)[14];
// 509 Bandwidth Limit Exceeded («исчерпана пропускная ширина канала»);
// 510 Not Extended («не расширено»);
// 511 Network Authentication Required («требуется сетевая аутентификация»)[11];
// 520 Unknown Error («неизвестная ошибка»)[15];
// 521 Web Server Is Down («веб-сервер не работает»)[15];
// 522 Connection Timed Out («соединение не отвечает»)[15];
// 523 Origin Is Unreachable («источник недоступен»)[15];
// 524 A Timeout Occurred («время ожидания истекло»)[15];
// 525 SSL Handshake Failed («квитирование SSL не удалось»)[15];
// 526 Invalid SSL Certificate («недействительный сертификат SSL»)[15].

enum error
{
    BadRequest = 400,
    OK = 200,
    IS_DIR = 42,
    NotFound = 404
};
<<<<<<< HEAD
=======

>>>>>>> sabra
class Server;
class Location;
class ParseRequest
{
    private:

        String      _method;
        String      _code;
        String      _path;
        String      _versProtocol;
        String      _body; //тело в post
        String      _contentType; 
        Map         _heading; //словарь заголовков
        Map         _types; // доступные расширение файла
        Vector      _keys; // доступные заголовки
        int         _bodyLength; // длина тела для post
        String      _str; // буфер в который считали данные из файла
        String      _strPath; // путь
        unsigned long _sizeFile;
<<<<<<< HEAD
=======
		String	_serverName;
		String _serverPort;
>>>>>>> sabra

       std::map<std::string,std::string>      _errorFilePath;
        String      _indexingFilePath;
        String      _rootDirectory;
        std::map<std::string, Location> _locations;
        std::map<std::string, std::string> _cgi;
        std::vector<std::string>  _listOfAllowedMethods;

        bool _forCgi;
    public: //  private?
        ParseRequest();
        //~ParseRequest();
        //==================get/set================
        const String            &getMethod() const;
<<<<<<< HEAD
=======
        const String            &getServerName() const {return this->_serverName;}
        const String            &getServerPort() const {return this->_serverPort;}
>>>>>>> sabra
        const String            &getPath() const;
        const String            &getVersProtocol() const;
        const String            &getBody() const;
        const String            &getStr() const;
        const String            &getStrPath() const;
        const String            &getContentType() const;
<<<<<<< HEAD
        const Map               &getMap() const;
=======
        const Map               &getHeading() const;
>>>>>>> sabra
        const String            &getCode() const;
        int                     getBodyLength() const;
        const unsigned long     &getSizeFile() const;
        const std::string       & getRootDirectory() const;
        const std::string       & getIndexingFilePath() const;
        const std::map<std::string,std::string>   &getErrorFilePath() const;
        bool                     getForCgi() const;
        
        const  Map   &getType()const;
        //===========Parsing methods===============
        error                 parsingStartLine(String &line);
        error                 parsingBody(String &line);
        error                 parsingHeading(String res);
        error                 parsRequest(String request, Server host,String NumCode);
        error                 parsBody(String request);
        error                 parsBodyLength(std::string &request);
        std::vector<String>   splitValues(std::string &value);
        void                  addArrKeys(void);
        void                  addTypes(void);
        bool                  checkKey(String value);
        void                  findType(std::string fn);
        void                  findPath(std::string root);
        error                 fileToString(std::string root);
        error                 dirToString(std::string indexFile);
        void                  findNewPath(std::string indexFile);
        void                  parsGet();
        error                 findLocation();
        error                 typeDefinitionMethod();
        error                 requestForNotCgi();
        error                 requestForCgi();
};

#endif
