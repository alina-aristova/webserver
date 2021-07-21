#ifndef CGI_HPP
# define CGI_HPP

# include "parseRequest.hpp"

class Cgi
{
	private:
		std::string _cgi_path;
		const std::string _request_file;
		const std::string _response_file;
		std::string _cgi_response;
		int _request_fd;
		int _response_fd;

		char	**createArgv(void) const;
		char	**createEnv(ParseRequest &request, char **ev) const;
		std::string	readData(std::string const & file_name) const;
		void	putData(const char *data, std::string const & file_name) const;
		void	execCgi(ParseRequest &request, char **ev);
		Cgi();
	
	public:
		Cgi(ParseRequest &request, std::string const & cgi_path, char **ev);
		~Cgi() {}

		std::string getCgiResponse(void) const;

		class OpenFileError : public std::exception
		{
			const char *what() const throw()
			{
				return "Can't open file!";
			}
		};
};

#endif
