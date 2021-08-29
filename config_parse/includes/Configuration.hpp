#ifndef CONFIGURATION_HPP
# define CONFIGURATION_HPP

# include <string>
# include <vector>
# include <fstream>
# include <iostream>
# include <sstream>
# include <exception>
# include "Server.hpp"

class Server;

class Configuration 
{

	private:
		std::vector<Server> _servers;

		/* ----------------------- Парсинг всего конфиг файла ----------------------- */
		void parseConfig(std::string const & config_file);

	public:
		Configuration(std::string const & config_file);
		~Configuration(void) {}

		/*-----------------------------------------------------------------------------
		 *  Основное взаимодействие с данным классом:
		 *		1) получить вектор серверов
		 *		2) внутри сервера определены его поля и locations
		 *		3) через геттеры получать любые поля
		 *-----------------------------------------------------------------------------*/
		std::vector<Server> getServers(void) const {return this->_servers;}	

		int result_code;

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
