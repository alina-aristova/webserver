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

		char **createArgv(void) const;
		char **createEnv(ParseRequest &request) const;
		Cgi();
	
	public:
		Cgi(std::string cgi_path);
		~Cgi() {}

		class AllocationError : public std::exception
		{
			const char *what() const
			{
				return "Allocation error!";
			}
		};
};

#endif