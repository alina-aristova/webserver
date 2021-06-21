#ifndef WEBSERVER_HPP
# define WEBSERVER_HPP

# include "server.hpp"
# include <vector>
# include <iostream>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <sys/select.h>
# include <fcntl.h>
# include <cstdlib>
# include <unistd.h>
# define BUFF_SIZE 1024

class WebServer{
private:
    std::vector<Server> _servers;
public:
    WebServer();
    void addServer(const Server & server);
    void work();
    class socketNotCreatedException : public std::exception {
        const char * what() const throw() {
            return ("Function socket() didn't create a socket.");
        }
    };
    class socketNotBoundException : public std::exception {
        const char * what() const throw() {
            return ("Function bind() didn't bind a socket.");
        }
    };
    class socketNotListeningException : public std::exception {
        const char * what() const throw() {
            return ("Function listen() didn't work on a socket.");
        }
    };
    class reusableAddressFailed : public std::exception {
        const char * what() const throw() {
            return ("Setting the address of the socket as reusable failed.");
        }
    };
    class selectFailed : public std::exception {
        const char * what() const throw() {
            return ("Select failed.");
        }
    };
};

#endif