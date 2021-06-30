#include "../includes/Configuration.hpp"

Configuration::Configuration(std::string const & config_file)
{
	parseConfig(config_file);
}

void Configuration::parseConfig(std::string const & config_file)
{
	std::string buf;
	std::string word;

    std::ifstream ifs(config_file, std::ios::in);
	if (ifs.fail())
	{
		std::cerr << "Error: " << strerror(errno) << std::endl;
		exit(-1);
	}
	while (std::getline(ifs, buf))
	{
		if (buf == "}" || buf.find_first_not_of(" \t\n\v\f\r") == std::string::npos)
			continue;
		if (buf == "{")
		{
			try
			{
				Server serverBlock(ifs, buf);
				this->_servers.push_back(serverBlock);
			}
			catch(const std::exception& e)
			{
				std::cerr << e.what() << std::endl;
			}
		}
	}
}
