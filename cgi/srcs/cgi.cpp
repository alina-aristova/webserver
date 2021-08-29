#include "../includes/cgi.hpp"
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

Cgi::Cgi(ParseRequest &request, std::string const & cgi_path, char **ev, std::string max_body_size) : _cgi_path(cgi_path),
 _request_file("request.cache"), _response_file("response.cache")
{
	execCgi(request, ev, max_body_size);
}

std::string	Cgi::getCgiResponse(void) const
{
	return this->_cgi_response;
}

/* --------- Create arguments and environment variables for our cgi --------- */
char **Cgi::createArgv(void) const
{
	char **argv;
	if (this->_cgi_path.find(".py"))
	{
		argv = static_cast<char**>(malloc(sizeof(char*) * 3));
		argv[0] = strdup("python3");
		argv[1] = strdup(this->_cgi_path.c_str());
		argv[2] = nullptr;
		return argv;
	}
	argv = static_cast<char**>(malloc(sizeof(char*) * 2));
	argv[0] = strdup(this->_cgi_path.c_str());
	argv[1] = nullptr;

	return argv;
}

char **Cgi::createEnv(ParseRequest & request, char **ev) const
{
	char pwd[1024];
	std::map<std::string, std::string> env_map;
	std::map<std::string, std::string> headers(request.getHeading());

	getcwd(pwd, 1024);
	for(int i = 0; ev[i]; i++)
	{
		std::size_t equal = static_cast<std::string>(ev[i]).find("=");
		env_map[static_cast<std::string>(ev[i]).substr(0, equal)] = static_cast<std::string>(ev[i]).substr(equal, static_cast<std::string>(ev[i]).size());
	}
	env_map["AUTH_TYPE"] = headers.count("Authorization") != 0 ? headers["Authorization"] : "";
	env_map["REDIRECT_STATUS"] = "200";
	env_map["CONTENT_LENGTH"] = std::to_string(request.getBody().length());
	env_map["CONTENT_TYPE"] = headers.count("Content-type") != 0 ? headers["Content-type"] : "";
	env_map["GATEWAY_INTERFACE"] = "CGI/1.1";
	env_map["REQUEST_TARGET"] = request.getPath();
	env_map["PATH_TRANSLATED"] = pwd + env_map["PATH_INFO"];
	env_map["PATH_INFO"] = request.getPath();
	env_map["QUERY_STRING"] = "";
	env_map["REMOTE_USER"] = headers["Authorization"];
	env_map["REMOTE_IDENT"] = headers["Authorization"];
	env_map["REQUEST_METHOD"] = request.getMethod();
	env_map["REQUEST_URI"] = request.getPath();
	env_map["COOKIE"] = headers["cookie"];
	std::size_t index = request.getPath().find('/');
	if (index != std::string::npos)
		env_map["SCRIPT_NAME"] = request.getPath().substr(index + 1);
	else
		env_map["SCRIPT_NAME"] = request.getPath();
	env_map["SCRIPT_FILENAME"] = request.getPath();
	env_map["SERVER_NAME"] = "localhost"; /*request.getServerName();*/
	env_map["SERVER_PORT"] = "8080";/*request.getServerPort();*/
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
		char **env = static_cast<char **>(malloc(sizeof(char *) * env_map.size() + 1));
		for (std::map<std::string, std::string>::iterator it = env_map.begin(); it != env_map.end(); it++)
		{
			env[i] = strdup(static_cast<std::string>(it->first + "=" + it->second).c_str());
			i++;
		}
		env[i] = nullptr;
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
			data.append("\n");
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

std::string Cgi::makeHeader(std::string data, std::string const & max_body_size) const
{
	std::size_t found;
	std::string content_len = "\nContent-Length: ";
	
	if (data.find("Content-Type:") == std::string::npos)
		throw BadCGIException();
	if ((found = data.find("Status:")) != std::string::npos)
		data.replace(found, std::strlen("Status:"), "HTTP/1.1");
	else
		data.insert(0, "HTTP/1.1 200 OK\n");
	found = data.find("\r\n\r\n");
	std::string body = data.substr(data.find("\r\n\r\n") + 4);
	if (body.size() > static_cast<size_t>(std::stoi(max_body_size)) * 1024 * 1024)
		throw BadHttpRequestException();
	content_len += std::to_string(body.length()) + "\r\n\r\n";
	data.replace(found, std::strlen("\r\n\r\n"), content_len);

	return data;
}
/* -------------------------------------------------------------------------- */

/* ------ Function that execute cgi binary and got result for response ------ */
void Cgi::execCgi(ParseRequest & request, char **main_env, std::string max_body_size)
{
	char **ev = createEnv(request, main_env);
	char **av = createArgv();

	if (ev == NULL || av == NULL)
		return;
	putData(request.getBody().c_str(), this->_request_file);
	putData("", this->_response_file);

	int pid;
	int status = 0;
	std::cout << this->_cgi_path << std::endl;
	this->_request_fd = open(this->_request_file.c_str(), O_RDWR);
	this->_response_fd = open(this->_response_file.c_str(), O_RDWR);
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
		if (this->_cgi_path.find(".py") != std::string::npos)
            status = execve("/usr/local/bin/python3", &av[0], ev);
		else
			status = execve(this->_cgi_path.c_str(), av, ev);
		close(this->_request_fd);
		close(this->_response_fd);
		exit(status);
	}
	wait(&status);
	freeMat(av);
	freeMat(ev);
	try
	{
		if (WEXITSTATUS(status) != 0)
			throw BadCGIException();
		std::string data = readData(this->_response_file);
		this->_cgi_response = makeHeader(data, max_body_size);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
}
/* -------------------------------------------------------------------------- */
