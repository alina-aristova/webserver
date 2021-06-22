# include "ParseRequest/Includes/parseRequest.hpp"

int main()
{
    ParseRequest parse;
    std::string line = "GET / HTTP/1.1\r\nHost: bannette\r\nContent-length: 12\r\n\r\n123456789012\r\n\r\n";
    parse.addArrKeys();
    try
    {
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
