# include "ParseRequest/Includes/parseRequest.hpp"
#include "ParseRequest/response/response.hpp"

void test(ParseRequest &parse)
{
    
    std::string line = "GETe /acase/Desktop/testfile/ HTTP/1.1\r\nHost: bannette\r\nContent-length: 12\r\n\r\n123456789012\r\n\r\n";
 
    parse.parsRequest(line);


}
int main()
{
    
    ParseRequest parse;
    Response  response;
    test(parse);
    response.addTypes();
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