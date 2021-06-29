#include "Configuration.hpp"

int main(void)
{
	std::vector<Server> servers;

	try
	{
		Configuration test("default.conf");
		servers = test.getServers();	
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	if (servers.size() == 0)
		return 1;
	return 0;
}
