# include "ReadingTransmitterClass.hpp"

ReadingTransmitterClass::ReadingTransmitterClass(int socket, std::string &responseStatus, ConnectionState &connectionState,
std::string &writingBuffer, bool & closeConnection, std::map<std::string, Server *> availableHosts)
: ATransmitterClass(socket, responseStatus, connectionState, writingBuffer, closeConnection), _availableHosts(availableHosts) {
    _readingBuffer = std::string("");
    _bufferToBeProcessed = std::string("");
    _host = std::string("default");
    _readingBodyFunType = CLASSIC;
    _contentLength = 0;
}

ReadingTransmitterClass::~ReadingTransmitterClass() {}

const std::string & ReadingTransmitterClass::getReadingBuffer() const { return (_readingBuffer); }

void ReadingTransmitterClass::_readIntoBuffer() {
    char temporaryBuffer[BUFF_SIZE];
    int bytesRead;
    bzero(temporaryBuffer, BUFF_SIZE);
    bytesRead = read(_socket, temporaryBuffer, BUFF_SIZE);
    if (bytesRead == 0) {
        close(_socket);
        _connectionState = CLOSE;
        return ;
    }
    if (bytesRead < 0) {
        _connectionState = ERROR_WHILE_READING;
        _responseStatus = "500";
//        std::cout << _socket << std::endl;
        size_t endOfLine = _readingBuffer.find("\r\n");
        if (endOfLine == std::string::npos)
            _readingBuffer = "";
        else
            _readingBuffer = _readingBuffer.substr(endOfLine + 2);
        return ;
    }
    _readingBuffer += std::string(temporaryBuffer, bytesRead);

}

void ReadingTransmitterClass::_processingFirstLine() {
    /// Валидируем первую строку

    size_t length = _readingBuffer.find("\r\n");
    std::vector<std::string> firstLineParts = split(_readingBuffer.substr(0, length));

    /// Проверяем, что в первой строке три составляющих
    if (firstLineParts.size() != 3) {
        _connectionState = ERROR_WHILE_READING;
        _responseStatus = "400";
        _readingBuffer = _readingBuffer.substr(length + 2);
        return ;
    }

    /// Проверяем, что нам пришли действительные методы
    if (firstLineParts[0] != "GET" &&
        firstLineParts[0] != "get" &&
        firstLineParts[0] != "POST" &&
        firstLineParts[0] != "post" &&
        firstLineParts[0] != "DELETE" &&
        firstLineParts[0] != "delete") {
        _connectionState = ERROR_WHILE_READING;
        _responseStatus = "400";
        _readingBuffer = _readingBuffer.substr(length + 2);
        return ;
    }

    /// Проверяем, что нужный протокол соответствует пришедшему протоколу
    if (firstLineParts[2] != "HTTP/1.1") {
        _connectionState = ERROR_WHILE_READING;
        _responseStatus = "505";
        _readingBuffer = _readingBuffer.substr(length + 2);
        return ;
    }

    /// Первая строка обработана, можно менять статус
    _connectionState = IS_PROCESSING_HEADERS;
    return ;
}

void ReadingTransmitterClass::_processingHeaders() {

    /// Проверяем наличие хоста и находим хост
    size_t hostBeginning = _readingBuffer.find("Host:");
    if (hostBeginning == std::string::npos) {
        _connectionState = ERROR_WHILE_READING;
        _responseStatus = "400";
        size_t headersEnd = 4 + _readingBuffer.find("\r\n\r\n");
        _readingBuffer = _readingBuffer.substr(headersEnd);
        return ;
    }
    else {
        /// вот тут находим значение хоста

        std::string hostString = _readingBuffer.substr(hostBeginning);
        hostString = hostString.substr(0, hostString.find("\r\n"));
        std::vector<std::string> hostStringParts = split(hostString);
        if (hostStringParts.size() < 2) {
            /// Если в хосте нет слов, то будем брать дефолтное значение

            _host = "default";
        }
        else
            _host = hostStringParts[1];
    }

    /// Определяем тип записи тела запроса
    _readingBodyFunType = CLASSIC;
    _contentLength = 0;
    size_t startIndex;
    size_t finishIndex;
    if (_readingBuffer.find("Transfer-Encoding: chunked") != std::string::npos
        || _readingBuffer.find("Transfer-encoding: chunked") != std::string::npos) {
        /// Если чанкд энкодинг, то устанавливаем соответствующий тип функции чтения тела

        _readingBodyFunType = CHUNKED_TRANSFER_ENCODING;
    }
    else if ((startIndex = _readingBuffer.find("Content-Length:")) != std::string::npos
             || (startIndex = _readingBuffer.find("Content-length:")) != std::string::npos) {
        /// Если контент лэнгс, то устанавливаем соответствующий тип функции чтения тела

        _readingBodyFunType = CONTENT_LENGTH;

        /// Находим и устанавливаем значение длины тела
        startIndex += 15;
        std::string lengthInStr = _readingBuffer.substr(startIndex);
        finishIndex = lengthInStr.find("\r\n");
        lengthInStr = lengthInStr.substr(0, finishIndex);
        try {
            /// В случай если значение длины было передано неправильно, отлавливаем исключение и меняем статус
            /// ответа, перескакиваем на формирование ответа

            int lengthInInt = std::stoi(lengthInStr.c_str());
            _contentLength = lengthInInt;
        } catch (std::exception & ex) {
            _responseStatus = "400";
            _connectionState = ERROR_WHILE_READING;
            size_t headersEnd = 4 + _readingBuffer.find("\r\n\r\n");
            _readingBuffer = _readingBuffer.substr(headersEnd);
            return ;
        }
    }

    /// находим указания относительно поддержавния соединения
    size_t startOfKeepConnectionLine = _readingBuffer.find("Connection: ");
    if (startOfKeepConnectionLine == std::string::npos)
        _closeConnection = false;
    else {
        size_t endOfConnectionLine = _readingBuffer.substr(startOfKeepConnectionLine).find("\r\n");
        std::string connectionLineStr = _readingBuffer.substr(startOfKeepConnectionLine, endOfConnectionLine + 2);
        if (connectionLineStr.find("close\r\n") != std::string::npos)
            _closeConnection = true;
        else
            _closeConnection = false;
    }

    /// Можно переходить к чтению тела запроса
    _connectionState = IS_READING_BODY;
}

void ReadingTransmitterClass::_classicReading() {
    /// Находим длину запроса, это понадобится для обработки
    size_t totalLengthOfRequest = _readingBuffer.find("\r\n\r\n") + 4;

    /// Меняем статус на формироание ответа
    _connectionState = IS_FORMING_RESPONSE;

    /// Формируем буффер для обработки
    _bufferToBeProcessed = _readingBuffer.substr(0, totalLengthOfRequest);

    /// Оставляем в буффере только то, что относится к сл запросу
    _readingBuffer = _readingBuffer.substr(totalLengthOfRequest);
    return ;
}

void ReadingTransmitterClass::_contentLengthReading() {

    /// Находим длину запроса, понадобится для обработки
    size_t totalLengthOfRequest = _readingBuffer.find("\r\n\r\n") + 4 + _contentLength;

    std::cout << totalLengthOfRequest << " " << _readingBuffer.size() << std::endl;
    /// Проверяем настал ли момент начинать обработку текущего запроса
    if (totalLengthOfRequest > _readingBuffer.size())
        return ;

    /// Меняем статус
    _connectionState = IS_FORMING_RESPONSE;

    /// Формируем буффер для обработки
    _bufferToBeProcessed = _readingBuffer.substr(0, totalLengthOfRequest);

    /// Оставляем в буффере только то, что относится к следующему запросу
    _readingBuffer = _readingBuffer.substr(totalLengthOfRequest);
    return ;
}

void ReadingTransmitterClass::_chunkedEncodingReading(int & unchunkedPartLength) {

    /// Находим, где у нас начинается "тело" запроса
    size_t beginningOfBody = _readingBuffer.find("\r\n\r\n") + 4;

    /// Находим прочитанную часть тела запроса
    std::string bodyPart = _readingBuffer.substr(beginningOfBody);

    /// Если прочитано меньше, чем можно по чанку, читаем еще
    if (bodyPart.size() < unchunkedPartLength)
        return ;

    /// Находим текущий чанк
    std::string currentChunk = bodyPart.substr(unchunkedPartLength);

    /// Проверяем не получили ли мы конец файла
    if (currentChunk.size() >= 5 &&
        currentChunk.substr(0, 5) == "0\r\n\r\n") {

        /// Подчищаем буффер от указанаия размера строки
        _readingBuffer.erase(beginningOfBody + unchunkedPartLength, 5);

        /// Переключаем статус на формирование ответа
        _connectionState = IS_FORMING_RESPONSE;

        /// Формируем ответ
        _bufferToBeProcessed = _readingBuffer.substr(0,beginningOfBody + 4 + unchunkedPartLength);

        /// Оставляем в буффере только то, что относится к следующему запросу
        _readingBuffer = _readingBuffer.substr(beginningOfBody + unchunkedPartLength);

        /// Восстанавливаем первоначальное значение для длины разчанкованного промежутка
        unchunkedPartLength = 0;
        return ;
    }

    if (currentChunk.substr(0, 3) == "0\r\n")
        return ;

    /// Находим конец текущего чанка
    size_t endOfChunk = currentChunk.find("\r\n");
    if (endOfChunk == std::string::npos)
        return ;

    try {

        /// Находим длинну следующего чанка
        std::string oldLengthStr = currentChunk.substr(0, endOfChunk);
        std::string newLengthStr = std::string("");
        for(std::string::iterator c = oldLengthStr.begin(); c != oldLengthStr.end(); c++)
            newLengthStr += std::tolower(*c);
        int chunkLength = std::stoi(newLengthStr, nullptr, 16) + 2;

        /// Подчищаем буффер от указания длины
        _readingBuffer.erase(beginningOfBody + unchunkedPartLength, endOfChunk + 2);

        /// Увеливаем длинну разчанкованного куска
        unchunkedPartLength += chunkLength;
    } catch (std::exception & ex) {

        /// Восстанавливаем первоначальное значение для длины разчанкованного промежутка
        unchunkedPartLength = 0;

        /// Меняем статус на ОШИБКУ
        _connectionState = ERROR_WHILE_READING;

        /// Указываем статус ошибки
        _responseStatus = "402";

        /// Отставляем в буффере только то, что относится к следующему запросу
        size_t newBeginning = _readingBuffer.substr(beginningOfBody + unchunkedPartLength).find("\r\n");
        _readingBuffer = _readingBuffer.substr(beginningOfBody + unchunkedPartLength + newBeginning);
        return ;
    }
}

void ReadingTransmitterClass::_readingBody() {
    static int unchunkedPartLength = 0;

    if (_readingBodyFunType == CLASSIC)
        _classicReading();
    else if (_readingBodyFunType == CONTENT_LENGTH)
        _contentLengthReading();
    else
        _chunkedEncodingReading(unchunkedPartLength);
}

Server *ReadingTransmitterClass::findRightHost() {
    std::map<std::string, Server *>::iterator it;

    /// Находим сервер с таким же названием
    it = _availableHosts.find(_host);

    /// Если такого сервера нет, то возвращаем первый имеющийся хост
    if (it == _availableHosts.end())
        it = _availableHosts.begin();
    return (it->second);
}

void ReadingTransmitterClass::returnDefaultValues() {
    _contentLength = 0;
    _host = "default";
    _bufferToBeProcessed = "";
    _readingBodyFunType = CLASSIC;
}

void ReadingTransmitterClass::operate() {
    /// читаем в буффер
    _readIntoBuffer();

    /// обрабатываем первую строку
    if (_connectionState == IS_PROCESSING_FIRST_LINE && _readingBuffer.find("\r\n") != std::string::npos)
        _processingFirstLine();

    /// Обрабатываем хэдэры
    if (_connectionState == IS_PROCESSING_HEADERS && _readingBuffer.find("\r\n\r\n") != std::string::npos)
        _processingHeaders();

    /// Считываем тело
    if (_connectionState == IS_READING_BODY)
        _readingBody();

    /// Обрабатываем ошибки
    if (_connectionState == ERROR_WHILE_READING) {
        std::cout << "статус " << _responseStatus << std::endl; // +
        std::cout << "------------------------" << std::endl; // +
        std::cout << "уходит в обработчик" << std::endl; // +
        std::cout << _bufferToBeProcessed << std::endl; // +
        std::cout << "-------------------------" << std::endl; // +
        std::cout << "остается" << std::endl; // +
        std::cout << _readingBuffer << std::endl; // +
        std::cout << "-------------------------" << std::endl; // +
        std::cout << "соединение будет "; // +
        if (_closeConnection) // +
            std::cout << "закрыто" << std::endl; // +
        else // +
            std::cout << "поддержано" << std::endl; // +
        std::cout << "--------------------------" << std::endl; // +
        std::cout << "хост - " << _host << std::endl; // +
        // находим нужного хоста
        Server *applicableHost = findRightHost();

//        std::cout << applicableHost->getHostName() << std::endl;
        // формируем ответ ошибки
        ParseRequest requestParser = ParseRequest();
        requestParser.parsRequest(_bufferToBeProcessed, *applicableHost, _responseStatus);
        Response response = Response();
        std::string numErrorStr = requestParser.getCode();
        if (numErrorStr == "400")
            _closeConnection = true;
        _writingBuffer = response.creatRespons(requestParser, numErrorStr);
        // возвращаем исходное значение полям класса
        returnDefaultValues();
        // меняем статус
        _connectionState = IS_WRITING_RESPONSE;
    }
    else if (_connectionState == IS_FORMING_RESPONSE) {
        std::cout << "статус " << _responseStatus << std::endl; // +
        std::cout << "------------------------" << std::endl; // +
        std::cout << "уходит в обработчик" << std::endl; // +
        std::cout << _bufferToBeProcessed << std::endl; // +
        std::cout << "-------------------------" << std::endl; // +
        std::cout << "остается" << std::endl; // +
        std::cout << _readingBuffer << std::endl; // +
        std::cout << "-------------------------" << std::endl; // +
        std::cout << "соединение будет "; // +
        if (_closeConnection) // +
            std::cout << "закрыто" << std::endl; // +
        else // +
            std::cout << "поддержано" << std::endl; // +
        std::cout << "--------------------------" << std::endl; // +
        std::cout << "хост - " << _host << std::endl; // +
        // находим нужного хоста
        Server *applicableHost = findRightHost();

//        std::cout << applicableHost->getHostName() << std::endl; // +
        // формируем ответ
        ParseRequest requestParser = ParseRequest();
        requestParser.parsRequest(_bufferToBeProcessed, *applicableHost, _responseStatus);
        std::string numErrorStr = requestParser.getCode();
        Response response = Response();
        if (numErrorStr == "400")
            _closeConnection = true;
        _writingBuffer = response.creatRespons(requestParser, numErrorStr);
        // возвращаем исходное значение полей класса
        returnDefaultValues();
        // меняем статус
        _connectionState = IS_WRITING_RESPONSE;
    }
}