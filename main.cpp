
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



int main(int argc, char **argv, char **env) {
	(void)argc;
	(void)argv;
	
    Configuration *ourConfig = new Configuration("default.conf");
	if (ourConfig->result_code == -1)
	{
		std::cout << "Syntax Error!" << std::endl;
		return 1;
	}
    std::vector <Server> ourServers = ourConfig->getServers();

    std::map<int, std::map<std::string, Server *> > portServerMap;
    std::vector<Server>::iterator hostMapBeginning = ourServers.begin();
    for (; hostMapBeginning != ourServers.end(); hostMapBeginning++) {
        std::map < std::string, Server * > HostMap;
//        std::map<std::string, Server *>::iterator currentHost = HostMap.find(hostMapBeginning->getHostName());
//        if (currentHost != HostMap.end() && currentHost->second->getPort() == hostMapBeginning->getPort()) {
//            std::cout << "Syntax Error!" << std::endl;
//            return (0);
//        }
        HostMap[hostMapBeginning->getHostName()] = &(*hostMapBeginning);

        int port = HostMap.begin()->second->getPort();
        if (portServerMap.find(port) != portServerMap.end()) {
            portServerMap[port][HostMap.begin()->first] = HostMap.begin()->second;
        } else {
            std::map < std::string, Server * > tempMap;
            tempMap[HostMap.begin()->first] = HostMap.begin()->second;
            portServerMap[port] = tempMap;
        }
    }

//    std::map<int, std::map<std::string, Server *> > portServerMap;
//    std::map<std::string, Server *>::iterator portServerMapBeginning = HostMap.begin();
//    for (; portServerMapBeginning != HostMap.end(); portServerMapBeginning++) {
//        int port = portServerMapBeginning->second->getPort();
//        if (portServerMap.find(port) != portServerMap.end()) {
//            portServerMap[port][portServerMapBeginning->first] = portServerMapBeginning->second;
//        } else {
//            std::map < std::string, Server * > tempMap;
//            tempMap[portServerMapBeginning->first] = portServerMapBeginning->second;
//            portServerMap[port] = tempMap;
//        }
//    }


    std::map<int, ConnectionClass *> socketConnectionMap;

    std::map<int, std::map<std::string, Server *> > socketServerMap;

    std::map<int, std::string> socketTypeMap;

    fd_set current_reading_sockets, ready_reading_sockets, current_writing_sockets, ready_writing_sockets;
    FD_ZERO(&current_reading_sockets);
    FD_ZERO(&current_writing_sockets);

    int max_socket_fd = 0;
    std::map < int, std::map < std::string, Server * > > ::iterator
    it = portServerMap.begin();
    for (; it != portServerMap.end(); it++) {
        int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (socket_fd == -1) {
            std::cout << "???? ???????????????? ??????????" << std::endl;
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

        if (bind(socket_fd, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
            std::cout << "???? ???????????????????? ?? ????????????" << std::endl;
            return (0);
        }

        if (listen(socket_fd, QLEN) < 0) {
            std::cout << "???? ?????????? ??????????????" << std::endl;
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
            std::cout << "???????????? ???? ????????????????" << std::endl;
            return (0);
        }
        std::map<int, std::string>::iterator iter = socketTypeMap.begin();
        for (; iter != socketTypeMap.end();) {
            if (FD_ISSET(iter->first, &ready_reading_sockets)) {
                if (iter->second == "listening") {
                    int new_socket = accept(iter->first, nullptr, nullptr);
                    if (new_socket < 0) {
                        std::cout << "???????????? ???? ????????????????" << std::endl;
                        return (0);
                    }
                    fcntl(new_socket, F_SETFL, O_NONBLOCK);
                    if (new_socket > max_socket_fd)
                        max_socket_fd = new_socket;
                    ConnectionClass *newConnection = new ConnectionClass(new_socket, socketServerMap.at(iter->first));
                    socketConnectionMap.insert(
                            std::make_pair(new_socket, newConnection));
                    socketTypeMap[new_socket] = "reading/writing";
                    ++iter;
                }
                else {
                    socketConnectionMap.at(iter->first)->setEnv(env);
                    socketConnectionMap.at(iter->first)->_reader->_connectionState = socketConnectionMap.at(iter->first)->_stateOfConnection;
                    socketConnectionMap.at(iter->first)->receive();
                    if (socketConnectionMap.at(iter->first)->getConnectionStatus() == CLOSE) {
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
        for (; iter != socketTypeMap.end();) {
            if (FD_ISSET(iter->first, &ready_writing_sockets)) {
                socketConnectionMap.at(iter->first)->setEnv(env);
                socketConnectionMap.at(iter->first)->transmit();
                if (socketConnectionMap.at(iter->first)->getConnectionStatus() == CLOSE) {
                    delete socketConnectionMap.at(iter->first);
                    socketConnectionMap.erase(iter->first);
                    socketServerMap.erase(iter->first);
                    socketTypeMap.erase(iter++);
                } else
                    ++iter;
            } else
                ++iter;
        }

        FD_ZERO(&current_reading_sockets);
        FD_ZERO(&current_writing_sockets);
        iter = socketTypeMap.begin();
        int temp_max_fd = 0;
        for (; iter != socketTypeMap.end(); iter++) {
            if (iter->first > temp_max_fd)
                temp_max_fd = iter->first;
            if (iter->second == "reading/writing")
                FD_SET(iter->first, &current_writing_sockets);
            FD_SET(iter->first, &current_reading_sockets);
        }
        max_socket_fd = temp_max_fd;
    }
}
