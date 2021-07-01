#include "cgi.hpp"
#include <map>
#include <unistd.h>

Cgi::Cgi(std::string cgi_path) : _cgi_path(cgi_path)
{}

char **Cgi::createArgv(void) const
{
	char **argv = static_cast<char**>(malloc(sizeof(char*) * 2));
	argv[0] = strdup(this->_cgi_path.c_str());
	argv[1] = nullptr;
}

char **Cgi::createEnv(ParseRequest & request) const
{
	char pwd[1024];
	std::map<std::string, std::string> env_map;
	std::map<std::string, std::string> headers(request.getHeading());

	getcwd(pwd, 1024);
	env_map["AUTH_TYPE"] = headers.count("Authorization") != 0 ? headers["Authorization"] : "";
	env_map["CONTENT_LENGTH"] = std::to_string(request.getBody().length());
	env_map["CONTENT_TYPE"] = headers.count("Content-type") != 0 ? headers["Content-type"] : "";
	env_map["GATEWAY_INTERFACE"] = "CGI/1.1";
	env_map["PATH_INFO"] = request.getPath();
	env_map["PATH_TRANSLATED"] = pwd + env_map["PATH_INFO"];
	env_map["QUERY_STRING"] = "";
	// env_map["REMOTE_ADDR"] = 
	// env_map["REMOTE_HOST"] = 
	if (env_map["AUTH_TYPE"] == "Basic")
	{
		env_map["REMOTE_USER"] = "UIViewController";
		env_map["REMOTE_IDENT"] = "UIViewController";
	}
	else
	{
		env_map["REMOTE_USER"] = "";
		env_map["REMOTE_IDENT"] = "";
	}
	env_map["REQUEST_METHOD"] = request.getMethod();
	std::size_t index = request.getPath().find('/');
	if (index != std::string::npos)
		env_map["SCRIPT_NAME"] = request.getPath().substr(index + 1);
	else
		env_map["SCRIPT_NAME"] = request.getPath();
	/*
		SERVER_NAME
		SERVER_PORT
	*/

  	env_map["SERVER_PROTOCOL"]   = "HTTP/1.1";
  	env_map["SERVER_SOFTWARE"]   = "web_server/1.0";
	return NULL;
}