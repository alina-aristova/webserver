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
        std::string line = "POST /cgi/cgi_tester HTTP/1.1\r\nHost: bannette\r\nContent-length: 16\r\n\r\n123456789012\r\n\r\n";
        std::string NumCode = "200";
        parse.parsRequest(line, servers[0], NumCode);
		std::cout << parse.getBody() << std::endl;
		if (parse.getForCgi() == false)
		{
			/* ----------------------- Просто тестирую работу cgi ----------------------- */
			Cgi cgi(parse, "cgi/cgi_tester", ev, servers[0].getLocations()[parse.getLocationName()].getMaxBodySize());
			/* -------------------------------------------------------------------------- */
			res = cgi.getCgiResponse();
		}
		else
			res = response.creatRespons(parse,parse.getCode());
		std::cout << res << std::endl;
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}
