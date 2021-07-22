#include "ParseRequest/Includes/parseRequest.hpp"
#include "config_parse/includes/Location.hpp"
#include "config_parse/includes/Server.hpp"
#include "config_parse/includes/Configuration.hpp"
#include "ParseRequest/response/response.hpp"
#include "cgi/includes/cgi.hpp"
int main(int ac, char **av, char **ev)
{
    // HostClass host;
    // host.setErrorFilePath("/Users/acase/Desktop/Errodr/error.pdf");
    // host.setIndexingFilePath("good.txt");
    // host.setRootDirectory("/Users");
    std::vector<Server> servers;
	(void)av;
	(void)ac;

	try
	{
		Configuration test("default.conf");
		servers = test.getServers();
        ParseRequest parse;    
        Response  response;
		std::string res;
        std::string line = "POST /cgi HTTP/1.1\r\nHost: bannette\r\nContent-length: 16\r\n\r\n123456789012\r\n\r\n";
        std::string NumCode = "200";
        parse.parsRequest(line, servers[0], NumCode);
		std::cout << parse.getBody() << std::endl;
    if (parse.getForCgi() == false)
    {
		/* ----------------------- Просто тестирую работу cgi ----------------------- */
		Cgi cgi(parse, "cgi/cgi_tester", ev, servers[0].getLocations()[parse.getPath()].getMaxBodySize());
		/* -------------------------------------------------------------------------- */
		res = cgi.getCgiResponse();
		// res = response.creatRespons(parse, parse.getCode(), cgi.getCgiResponse());
    }
	else
		res = response.creatRespons(parse,parse.getCode(), "");
    std::cout << res << std::endl;
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	if (servers.size() == 0)
		return 1;
    // std::cout << "Метод: " << parse.getMethod() << std::endl;
    // std::cout << "Путь до файла: " << parse.getPath() << std::endl;
    // std::cout << "Версия: " << parse.getVersProtocol() << std::endl;
    // std::cout << "body: " << parse.getBody() << std::endl;
    // std::cout << "то что считали с файла: " << parse.getStr() << std::endl;
    // std::cout << "склееный путь вместе с файлом: " << parse.getStrPath() << std::endl;
    // std::cout << "Type: " << parse.getContentType() << std::endl;
}
