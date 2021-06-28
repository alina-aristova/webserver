#include "Configuration.hpp"

int main(void)
{
	try
	{
		Configuration test("default.conf");
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	
	return 0;
}
