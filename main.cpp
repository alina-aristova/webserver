# include "ParseRequest/Includes/parseRequest.hpp"
#include "ParseRequest/response/response.hpp"
#include "Server.hpp"

int main()
{
    // HostClass host;
    // host.setErrorFilePath("/Users/acase/Desktop/Errodr/error.pdf");
    // host.setIndexingFilePath("good.txt");
    // host.setRootDirectory("/Users");
    Server serv;
    ParseRequest parse;    
    Response  response;
    std::string line = "GET /acase/Desktop/testfile/copy.txt HTTP/1.1\r\nHost: bannette\r\nContent-length: 12\r\n\r\n123456789012\r\n\r\n";
    std::string NumCode = "200";
    parse.parsRequest(line,serv,NumCode);
    if (parse.getForCgi() == true)
    {
        std::cout << "наш cgi еще не готов:(\nМы не можем выполнить ваш запрос, но мы обязательно его доделаем, приходите позже!\n";
        return(0);
    }
    std::string res = response.creatRespons(parse,parse.getCode());
    std::cout << res << std::endl;
    // std::cout << "Метод: " << parse.getMethod() << std::endl;
    // std::cout << "Путь до файла: " << parse.getPath() << std::endl;
    // std::cout << "Версия: " << parse.getVersProtocol() << std::endl;
    // std::cout << "body: " << parse.getBody() << std::endl;
    // std::cout << "то что считали с файла: " << parse.getStr() << std::endl;
    // std::cout << "склееный путь вместе с файлом: " << parse.getStrPath() << std::endl;
    // std::cout << "Type: " << parse.getContentType() << std::endl;
}