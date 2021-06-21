# include "ParseRequest/Includes/parseRequest.hpp"

int main()
{
    ParseRequest parse;
    std::string line = "POST /login.php HTTP/1.1 \r\n Server: Apache \r\n Accept-Ranges: bytes \r\n Content-Length: 29769 \r\n\r\n это тело \r\n\r\n";
    try{
        parse.parsRequest(line);
    }
    catch (std::exception const & ex)
    {
       std::cout << ex.what() << std::endl;
    }
    

    std::cout << "Метод: " << parse.getMethod() << std::endl;
    std::cout << "Путь: " << parse.getPath() << std::endl;
    std::cout << "Версия: " << parse.getVersProtocol() << std::endl;
    std::cout << "body: " << parse.getBody() << std::endl;
}
