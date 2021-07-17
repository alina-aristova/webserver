#ifndef CGI_HPP
# define CGI_HPP

# include "parseRequest.hpp"

void clearMatrix(char **mat)
{
	if (mat)
	{
		for (int i = 0; mat[i]; i++)
			free(mat[i]);
		free(mat);
	}
}

class Cgi
{
	private:
		std::string _cgi_path;
		const std::string _request_file = ".request_cache";
		const std::string _response_file = ".response_cache";
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
			const char *what() const
			{
				return "Can't open file!";
			}
		};
};

#endif