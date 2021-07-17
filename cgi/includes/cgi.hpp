#ifndef CGI_HPP
# define CGI_HPP

# include "parseRequest.hpp"

class Cgi
{
	private:
		std::string _cgi_path;
		const std::string _request_file;
		const std::string _response_file;
		int _request_fd;
		int _response_fd;

		char	**createArgv(void) const;
		char	**createEnv(ParseRequest &request) const;
		std::string	readData(std::string const & file_name) const;
		void	putData(const char *data, std::string const & file_name) const;
		void	execCgi(ParseRequest &request);
		Cgi();
	
	public:
		Cgi(ParseRequest &request, std::string const & cgi_path);
		~Cgi() {}

		class OpenFileError : public std::exception
		{
			const char *what() const throw()
			{
				return "Can't open file!";
			}
		};
};

#endif
