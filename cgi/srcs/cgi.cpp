#include "cgi.hpp"
#include <map>
#include <unistd.h>
#include <fcntl.h>
#include <algorithm> 

void freeMat(char **mat)
{
	if (mat)
	{
		for (int i = 0; mat[i]; i++)
			free(mat[i]);
		free(mat);
	}
}

Cgi::Cgi(ParseRequest &request, std::string const & cgi_path) : _cgi_path(cgi_path), _request_file("request.cache"),
	_response_file("response.cache")
{
	execCgi(request);
}

/* --------- Create arguments and environment variables for our cgi --------- */
char **Cgi::createArgv(void) const
{
	char **argv = static_cast<char**>(malloc(sizeof(char*) * 2));
	argv[0] = strdup(this->_cgi_path.c_str());
	argv[1] = nullptr;

	return argv;
}

char **Cgi::createEnv(ParseRequest & request) const
{
	char pwd[1024];
	std::map<std::string, std::string> env_map;
	std::map<std::string, std::string> headers(request.getHeading());

	getcwd(pwd, 1024);
	env_map["AUTH_TYPE"] = headers.count("Authorization") != 0 ? headers["Authorization"] : "";
	env_map["REDIRECT_STATUS"] = "200";
	env_map["CONTENT_LENGTH"] = std::to_string(request.getBody().length());
	env_map["CONTENT_TYPE"] = headers.count("Content-type") != 0 ? headers["Content-type"] : "";
	env_map["GATEWAY_INTERFACE"] = "CGI/1.1";
	env_map["PATH_INFO"] = request.getPath();
	env_map["PATH_TRANSLATED"] = pwd + env_map["PATH_INFO"];
	env_map["QUERY_STRING"] = "";
	env_map["REMOTE_USER"] = headers["Authorization"];
	env_map["REMOTE_IDENT"] = headers["Authorization"];
	env_map["REQUEST_METHOD"] = request.getMethod();
	env_map["REQUEST_URI"] = request.getPath();
	// std::size_t index = request.getPath().find('/');
	// if (index != std::string::npos)
	// 	env_map["SCRIPT_NAME"] = request.getPath().substr(index + 1);
	// else
		env_map["SCRIPT_NAME"] = request.getPath();
	std::cout << env_map["SCRIPT_NAME"] << std::endl;
	env_map["SCRIPT_FILENAME"] = request.getPath();
	env_map["SERVER_NAME"] = request.getServerName();
	env_map["SERVER_PORT"] = request.getServerPort();
  	env_map["SERVER_PROTOCOL"]   = "HTTP/1.1";
  	env_map["SERVER_SOFTWARE"]   = "web_server/1.0";
	for (std::map<std::string, std::string>::iterator it = headers.begin(); it != headers.end(); it++)
	{
		std::string head(it->first);
		size_t found = head.find('-');
		if (found != std::string::npos)
			head.replace(found, 1, "_");
		std::transform(head.begin(), head.end(), head.begin(), toupper);
		env_map["HTTP_" + head] = it->second;
	}
	try
	{
		int i = 0;
		char **env = static_cast<char **>(malloc(sizeof(char *) * (env_map.size() + headers.size()) + 1));
		for (std::map<std::string, std::string>::iterator it = env_map.begin(); it != env_map.end(); it++)
		{
			env[i] = strdup(static_cast<std::string>(it->first + "=" + it->second).c_str());
			i++;
		}
		for (std::map<std::string, std::string>::iterator it = headers.begin(); it != headers.end(); it++)
		{
			env[i] = strdup(static_cast<std::string>(it->first + "=" + it->second).c_str());
			i++;
		}
		env[i] = nullptr;
		// for (int i = 0; env[i]; i++)
		// {
		// 	std::cout << env[i] << std::endl;
		// }
		return env;
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	return NULL;
}
/* -------------------------------------------------------------------------- */

/* ------------------------- Buffer files operations ------------------------ */
std::string	Cgi::readData(std::string const & file_name) const 
{
	std::ifstream file(file_name);
	std::string data;
	std::string buf;

	if (file.is_open())
	{
		while (std::getline(file, buf))
		{
			data.append(buf);
		}
	}
	file.close();
	return data;
}

void	Cgi::putData(const char *data, std::string const & file_name) const 
{
	std::ofstream file(file_name);
	
	if (file.is_open())
		file << data;
	file.close();
}
/* -------------------------------------------------------------------------- */

/* ------ Function that execute cgi binary and got result for response ------ */
void Cgi::execCgi(ParseRequest & request)
{
	char **ev = createEnv(request);
	char **av = createArgv();

	if (ev == NULL || av == NULL)
		return;
	putData(request.getBody().c_str(), this->_request_file);
	putData("", this->_response_file);

	int pid;
	int status = 0;
	this->_request_fd = open(this->_request_file.c_str(), O_RDWR);
	this->_response_fd = open(this->_response_file.c_str(), O_RDWR);
	printf("request = %d\nresponse = %d\n", this->_request_fd, this->_response_fd);
	if (this->_response_fd < 0 || this->_request_fd < 0)
		throw Cgi::OpenFileError();
	pid = fork();
	if (pid)
	{
		close(this->_request_fd);
		close(this->_response_fd);
	}
	else if (!pid)
	{
		dup2(this->_request_fd, 0);
		dup2(this->_response_fd, 1);
		status = execve(this->_cgi_path.c_str(), av, ev);
		close(this->_request_fd);
		close(this->_response_fd);
		exit(status);
	}
	waitpid(pid, &status, 0);
	std::string result = readData(this->_response_file);
	std::cout << WEXITSTATUS(status) << std::endl;
	freeMat(av);
	freeMat(ev);
	
	/* ------ Дальше нужно обернуть всю инфу в response file в сам респонс ------ */
}
/* -------------------------------------------------------------------------- */
