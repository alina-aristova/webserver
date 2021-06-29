#ifndef CONFIGURATION_HPP
# define CONFIGURATION_HPP

# include <string>
# include <vector>
# include <fstream>
# include <iostream>
# include <sstream>
# include <exception>
# include "Server.hpp"



class Configuration 
{

	private:
		std::vector<Server> _servers;

	// 	/* ------------------------------- Parser funcs ------------------------------ */

		void parseConfig(std::string const & config_file);
	// 	void parseAllowedMethods(std::vector<std::string> & methods);
	// 	void parseListen(std::vector<std::string> & port);
	// 	void parseServerName(std::vector<std::string> & server_names);
	// 	void parseErrorPage(std::vector<std::string> & error_info);

	public:
	// 	Configuration();
	// 	Configuration(std::string const & config_file);
	
	class UnknownToken : public std::exception
	{
		const char *what(void) const throw()
		{
			return "Unknown token found!";
		}		
	};

	class SyntaxError: public std::exception
	{
		const char *what(void) const throw()
		{
			return "SyntaxError!";
		}		
	};
};

#endif
