# include <iostream>
# include <string>
# include "ReadingTransmitterClass.hpp"
# include "ATransmitterClass.hpp"
# include "ConnectionClass.hpp"
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <cstdlib>
# include <sys/select.h>
# include <fcntl.h>
# include <unistd.h>
# include <map>
# include "config_parse/includes/Configuration.hpp"

# define QLEN 5

int main() {
    Configuration *ourConfig = new Configuration("/Users/bannette/Desktop/config");
    std::vector<Server> ourServers = ourConfig->getServers();

    std::map<std::string, Server *> HostMap;
    std::vector<Server>::iterator hostMapBeginning = ourServers.begin();
    for( ;hostMapBeginning != ourServers.end(); hostMapBeginning++) {
        HostMap[hostMapBeginning->getHostName()] = &(*hostMapBeginning);
    }

    std::map<int, std::map<std::string, Server *> > portServerMap;
    std::map<std::string, Server *>::iterator portServerMapBeginning = HostMap.begin();
    for( ;portServerMapBeginning != HostMap.end(); portServerMapBeginning++) {
        int port = portServerMapBeginning->second->getPort();
        if (portServerMap.find(port) != portServerMap.end()) {
            portServerMap[port][portServerMapBeginning->first] = portServerMapBeginning->second;
        }
        else {
            std::map<std::string, Server *> tempMap;
            tempMap[portServerMapBeginning->first] = portServerMapBeginning->second;
            portServerMap[port] = tempMap;
        }
    }
    
    
    std::map<int, ConnectionClass> socketConnectionMap;

    std::map<int, std::map<std::string, Server *> > socketServerMap;

    std::map<int, std::string> socketTypeMap;

    fd_set current_reading_sockets, ready_reading_sockets, current_writing_sockets, ready_writing_sockets;
    FD_ZERO(&current_reading_sockets);
    FD_ZERO(&current_writing_sockets);

    int max_socket_fd = 0;
    std::map<int, std::map<std::string, Server *> >::iterator it = portServerMap.begin();
    for( ; it != portServerMap.end(); it++) {
        int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (socket_fd == -1) {
            std::cout << "не создался сокет"  << std::endl;
            return (0);
        }
        socketServerMap[socket_fd] = it->second;
        socketTypeMap[socket_fd] = "listening";

        struct sockaddr_in addr;

        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = INADDR_ANY;
        addr.sin_port = htons(it->first);
        memset(addr.sin_zero, '\0', sizeof(addr.sin_zero));

        int opt = 1;
        setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

        if (bind(socket_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
            std::cout << "не привязался к адресу"  << std::endl;
            return (0);
        }

        if (listen(socket_fd, QLEN) < 0) {
            std::cout << "не начал слушать"  << std::endl;
            return (0);
        }

        FD_SET(socket_fd, &current_reading_sockets);
        fcntl(socket_fd, F_SETFL, O_NONBLOCK);

        if (socket_fd > max_socket_fd)
            max_socket_fd = socket_fd;
    }

    while (true) {
        ready_reading_sockets = current_reading_sockets;
        ready_writing_sockets = current_writing_sockets;

        if (select(max_socket_fd + 1, &ready_reading_sockets, &ready_writing_sockets, nullptr, nullptr) < 0) {
            std::cout << "селект не сработал"  << std::endl;
            return (0);
        }
        std::map<int, std::string>::iterator iter = socketTypeMap.begin();
        for ( ; iter != socketTypeMap.end();) {
            if (FD_ISSET(iter->first, &ready_reading_sockets)) {
                if (iter->second == "listening") {
                    int new_socket = accept(iter->first, nullptr, nullptr);
                    if (new_socket < 0) {
                        std::cout << "аксэсс не сработал"  << std::endl;
                        return (0);
                    }
                    fcntl(new_socket, F_SETFL, O_NONBLOCK);
                    //FD_SET(new_socket, &current_reading_sockets);
                    //FD_SET(new_socket, &current_writing_sockets);
                    if (new_socket > max_socket_fd)
                        max_socket_fd = new_socket;
                    socketConnectionMap.insert(std::make_pair(new_socket, ConnectionClass(new_socket, socketServerMap.at(iter->first))));
                    socketTypeMap[new_socket] = "reading/writing";
                    ++iter;
                }
                else {
                    socketConnectionMap.at(iter->first).receive();
                    if (socketConnectionMap.at(iter->first).getConnectionStatus() == CLOSE) {
                        socketConnectionMap.erase(iter->first);
                        socketServerMap.erase(iter->first);
                        socketTypeMap.erase(iter++);
                    }
                    else
                        ++iter;
                }
            }
            else
                ++iter;
        }

        iter = socketTypeMap.begin();
        for ( ; iter != socketTypeMap.end(); ) {
            if (FD_ISSET(iter->first, &ready_writing_sockets)) {
//                std::cout << "our socket is fd of " << iter->first << std::endl;
//                std::cout << "our socket is fd of " << iter->first << std::endl;
//                std::cout << "our socket is fd of " << iter->first << std::endl;
//                std::cout << "our socket is fd of " << iter->first << std::endl;
                socketConnectionMap.at(iter->first).transmit();
                if (socketConnectionMap.at(iter->first).getConnectionStatus() == CLOSE) {
                    socketConnectionMap.erase(iter->first);
                    socketServerMap.erase(iter->first);
                    socketTypeMap.erase(iter++);
                }
                else
                    ++iter;
            }
            else
                ++iter;
        }

        FD_ZERO(&current_reading_sockets);
        FD_ZERO(&current_writing_sockets);
        iter = socketTypeMap.begin();
        for ( ; iter != socketTypeMap.end(); iter++) {
            if (iter->second == "reading/writing")
                FD_SET(iter->first, &current_writing_sockets);
            FD_SET(iter->first, &current_reading_sockets);
        }
    }
}