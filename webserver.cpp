#include "webserver.hpp"
#include <iostream>

WebServer::WebServer() {
    _servers.reserve(MAX_PORT_NUMBER);
}

void WebServer::addServer(const Server & server) {
    _servers.push_back(server);
}

void WebServer::work() {
    /// Заводим два набора сокетов для чтения: изначальный и готовый. Изначальный необходим нам так-как select
    /// является разрушающим методом.
    int max_reading_fd = 0;
    fd_set current_reading_sockets, ready_reading_sockets;
    FD_ZERO(&current_reading_sockets);

    for (int z = 0; z < (int)_servers.size(); z++) {
        /// Создаем сокет.
        int socket_fd;
        if ((socket_fd = socket(AF_INET, AF_UNIX, 0)) < 0)
            throw socketNotCreatedException();

        /// Привязываем его к необходимому порту.
        struct sockaddr_in address;
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = htonl(INADDR_ANY);
        address.sin_port = htons( _servers[z].getWhereReadsFrom() );

        /// Вот эта штука нужна, чтобы не было залипание TCP
        int opt = 1;
        if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < -1)
            throw reusableAddressFailed();

        if (bind(socket_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
            throw socketNotBoundException();

        /// Вносим его в структуру текущего сервера как сокет, принимающий новые соединения.
        _servers[z].setListeningSocket(socket_fd);

        /// Сокет начинает слушать и принимать новые соединения.
        if (listen(socket_fd, 5) < 0)
            throw socketNotListeningException();

        /// Добавляем сокет в сэт готовых сокетов для функции select.
        FD_SET(socket_fd, &current_reading_sockets);

        /// Смотрим увеличился ли максимальный fd.
        if (socket_fd > max_reading_fd)
            max_reading_fd = socket_fd;

    }
    while (true) {

        /// Так-как select деструктивный каждый раз обновляем сэт.
        ready_reading_sockets = current_reading_sockets;

        /// Вызываем select, чтобы получить сокеты, на котрых можно принять
        /// или отправить информацию.
        select(max_reading_fd + 1, &ready_reading_sockets, NULL, NULL, NULL);

        /// Проверяем соекты и выяснем, какие из них могут сейчас принять или передать
        /// информацию.
        for(int i = 0; i < max_reading_fd + 1; i++) {
            if (FD_ISSET(i, &ready_reading_sockets)) {

                /// Проходим по всем серверам и смотрим, для кого из них сокеты
                /// готовы читать или записывать информацию.
                for (int z = 0; z < (int)_servers.size(); z++) {

                    /// Если сокет из принимающих новые соединения, то создаем новое соединение.
                    if (i == _servers[z].getListeningSocket()) {

                        /// Акцептуем новое соединение.
                        int new_socket = accept(i, NULL, NULL);

                        /// Проверяем на то, не произошла ли ошибка.
                        if (new_socket < 0)
                            throw selectFailed();

                        /// Включаем неблокирующий режим.
                        fcntl(new_socket, F_SETFL, O_NONBLOCK);

                        /// Добавляем новый сокет в список сокетов конкретного сервера.
                        _servers[z].addAccessedSockets(new_socket);

                        /// Добавляем сокет в главный сэт.
                        FD_SET(new_socket, &current_reading_sockets);

                        /// Проверяем не изменился ли максимальный номер доступного сокета,
                        /// и, если изменился, корретируем значение.
                        if (new_socket > max_reading_fd)
                            max_reading_fd = new_socket;

                    }
                    else {

                        /// Если сокет из тех, которые не принимают новые соединения,
                        /// а принимают отправляюмую нам с сервера информацию,
                        /// читаем из него.

                        std::map<int, std::string>::iterator it = _servers[z].getAccessedSockets().find(i);
                        if (it != _servers[z].getAccessedSockets().end()
                        && _servers[z].getAccessedSocketState(it->first) != STARTED_WRITING) {
                            char buff[BUFF_SIZE];
                            memset(buff, '\0', BUFF_SIZE);
                            int readBytes = read(i, buff, BUFF_SIZE - 1);
                            buff[readBytes] = '\0';
                            it->second += std::string(buff);
                            if (_servers[z].getAccessedSocketState(it->first) == STARTED_READING) {
                                size_t endOfFirstLine;
                                if ((endOfFirstLine = it->second.find("\r\n")) != std::string::npos) {
                                    
                                }
                            }
                            if (it->second.find("\r\n\r\n") != std::string::npos) {
                                std::cout << it->second;
                                it->second = "";
                            }
                        }
                    }
                }
            }
        }
    }
}