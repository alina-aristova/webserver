# include "ParseRequest/Includes/parseRequest.hpp"

int main()
{
    ParseRequest parse;
    std::string line = "POST /login.php HTTP/1.1 \r\n Server: Apache \r\n Accept-Ranges: bytes \r\nContent-Length: 29769 \r\n\r\n";

    parse.parsRequest(line);

    std::cout << "Метод: " << parse.getMethod() << std::endl;
    std::cout << "Путь: " << parse.getPath() << std::endl;
    std::cout << "Версия: " << parse.getVersProtocol() << std::endl;
}
