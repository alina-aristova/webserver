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
    int max_fd = 0;
    fd_set current_reading_sockets, ready_reading_sockets;
    fd_set current_writing_sockets, ready_writing_sockets;
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
        if (socket_fd > max_fd)
            max_fd = socket_fd;

    }
    while (true) {

        /// Так-как select деструктивный каждый раз обновляем сэт.
        ready_reading_sockets = current_reading_sockets;
        ready_writing_sockets = current_writing_sockets;

        /// Вызываем select, чтобы получить сокеты, на котрых можно принять
        /// или отправить информацию.
        select(max_fd + 1, &ready_reading_sockets, &ready_writing_sockets, NULL, NULL);

        /// Проверяем соекты и выяснем, какие из них могут сейчас принять или передать
        /// информацию.

        for(int i = 0; i < max_fd + 1; i++) {

            /// Смотрим кто из них может передавать или принимать информаицю
            if (FD_ISSET(i, &ready_reading_sockets) || FD_ISSET(i, &ready_writing_sockets)) {

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
                        FD_SET(new_socket, &current_writing_sockets);

                        /// Проверяем не изменился ли максимальный номер доступного сокета,
                        /// и, если изменился, корретируем значение.
                        if (new_socket > max_fd)
                            max_fd = new_socket;

                    }
                    else {

                        /// Если сокет из тех, которые не принимают новые соединения,
                        /// а принимают отправляюмую нам с сервера информацию или отправляют
                        /// данные серверу , то читаем из него или записываесм в него соответстенно.

                        std::map<int, std::string>::iterator it = _servers[z].getAccessedSockets().find(i);
                        if (it != _servers[z].getAccessedSockets().end()
                        && _servers[z].getAccessedSocketState(it->first) != STARTED_WRITING) {

                            /// Считываем прешедшую информацию в буфер и конкатинируем ее с тем, что пришло раньше
                            char buff[BUFF_SIZE];
                            memset(buff, '\0', BUFF_SIZE);
                            int readBytes = read(i, buff, BUFF_SIZE - 1);
                            buff[readBytes] = '\0';
                            it->second += std::string(buff);

                            /// Если это первая строка запроса, то проверяем на валидность
                            if (_servers[z].getAccessedSocketState(it->first) == STARTED_READING) {
                                size_t endOfFirstLine;
                                if ((endOfFirstLine = it->second.find("\r\n")) != std::string::npos) {
                                    _servers[z].addRequestForSocket(it->first);
                                    std::string firstLine = it->second.substr(0, endOfFirstLine);
                                    if (_servers[z].getRequestForSocket(it->first).parsingStartLine(firstLine) == BadRequest)
                                        return;
                                    _servers[z].changeAccessedSocketStatus(it->first, KEEPS_READING);
                                }
                            }

                            /// Если нашли двойной перенос строки, то это либо конец запроса, либо
                            /// нужно отсчитывать кол-во байтов в теле запроса.

                            size_t endOfHeaders;
                            if ((endOfHeaders = it->second.find("\r\n\r\n")) != std::string::npos) {

                                /// Проверяю задан ли у нас на данный момент Content Length
                                if (_servers[z].getRequestForSocket(it->first).getBodyLength() == 0) {

                                    /// Если не задан, то пытаемся задать
                                    if (_servers[z].getRequestForSocket(it->first).parsBodyLength(it->second) == BadRequest)
                                        return ;

                                    /// Если не задается, то обрезаем запрос по /r/n/r/n, остальное запихиваем в строку
                                    /// для последующей обработки, меняем состояние сокета с чтения на зпаись
                                    int bodyLength;
                                    if ((bodyLength = _servers[z].getRequestForSocket(it->first).getBodyLength()) == 0) {
                                        std::cout << "оригинальная строка " << it->second << std::endl;

                                        /// вот тут запаршиваем нашу строку в реквест, до /r/n/r/n
                                        if (_servers[z].getRequestForSocket(it->first).parsRequest(it->second.substr(0, endOfHeaders)) == BadRequest)
                                            return ;

                                        /// Сохраняем остаток для последующей обработки
                                        it->second = it->second.substr(endOfHeaders);
                                        std::cout << "осталось в строке "<< it->second << std::endl;
                                        /// Меняем статус
                                        _servers[z].changeAccessedSocketStatus(it->first, STARTED_WRITING);
                                    }
                                    else {
                                        /// Если Content-Length есть смотрим достаточно ли мы уже считали символов

                                        int readSymbols = (int)it->second.substr(endOfHeaders).size() - 4;


                                        /// Если достаточно, то
                                        if (readSymbols >= _servers[z].getRequestForSocket(it->first).getBodyLength()) {
                                            std::cout << "оригинальная строка " << it->second << std::endl;

                                            /// вот тут запрашиваем нашу строку до endOfHeaders + content-length элемента
                                            if (_servers[z].getRequestForSocket(it->first).parsRequest(it->second.substr(0, endOfHeaders + bodyLength)) == BadRequest)
                                                return ;


                                            /// Сохраняем остаток для последующей обработки
                                            it->second = it->second.substr(endOfHeaders + 4 +
                                                    _servers[z].getRequestForSocket(it->first).getBodyLength());
                                            std::cout << "осталось в строке "<< it->second << std::endl;
                                            /// Меняем статус
                                            _servers[z].changeAccessedSocketStatus(it->first, STARTED_WRITING);
                                        }
                                    }
                                }
                                else {
                                    /// Если Content-Length есть смотрим достаточно ли мы уже считали символов

                                    int readSymbols = (int)it->second.substr(endOfHeaders).size() - 4;
                                    int bodyLength = _servers[z].getRequestForSocket(it->first).getBodyLength();

                                    /// Если достаточно, то
                                    if (readSymbols >= bodyLength) {
                                        std::cout << "оригинальная строка" << it->second << std::endl;

                                        /// вот тут запрашиваем нашу строку до endOfHeaders + content-length элемента
                                        if (_servers[z].getRequestForSocket(it->first).parsRequest(it->second) == BadRequest)
                                            return ;

                                        /// Сохраняем остаток для последующей обработки
                                        it->second = it->second.substr(endOfHeaders + 4 +
                                                _servers[z].getRequestForSocket(it->first).getBodyLength());
                                        std::cout << "осталось в строке "<< it->second << std::endl;
                                        /// Меняем статус
                                        _servers[z].changeAccessedSocketStatus(it->first, STARTED_WRITING);
                                    }
                                }
                            }
                        }
                        else if (it != _servers[z].getAccessedSockets().end()) {
                            char buff[] = "HTTP/1.1 404 Not Found\r\nServer: nginx/0.8.54\r\nDate: Mon, 02 Jan 2012 02:33:17 GMT\r\nContent-Type: text/html\r\nContent-Length: 175\r\nConnection: keep-alive\r\n\r\n<html>\r\n<head><title>404 Not Found</title></head>\r\n<body bgcolor=\"white\">\r\n<center><h1>404 Not Found</h1></center>\r\n<hr><center>nginx/0.8.54</center>\r\n</body>\r\n</html>";
                            /// Записываем в сокет
                            int start = _servers[z].getStartingIndexWriting();
                            int bytesWritten = write(it->first, &(buff[start]), BUFF_SIZE);
                            _servers[z].setStartingIndexWriting(start + bytesWritten);
                            /// Меняем статус
                            if (bytesWritten == 0)
                                _servers[z].changeAccessedSocketStatus(it->first, STARTED_READING);
                        }
                    }
                }
            }
        }
    }
}