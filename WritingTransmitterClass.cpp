#include "WritingTransmitterClass.hpp"

WritingTransmitterClass::WritingTransmitterClass(int socket, std::string & responseStatus, ConnectionState connectionState,
                                                 std::string & writingBuffer, bool & closeConnection)
: ATransmitterClass(socket, responseStatus, connectionState, writingBuffer, closeConnection) {}

void WritingTransmitterClass::operate() {

    int bytes_written = 0;

    /// записать сколько получится
    fcntl(_socket, F_SETFL, O_NONBLOCK);
    bytes_written = write(_socket, _writingBuffer.c_str(), _writingBuffer.size());

    std::cout << std::endl << "*******************************************" << std::endl;
    std::cout << "******   Part of out response     *********" << std::endl;
    std::cout << "*******************************************" << std::endl;
    std::cout << _writingBuffer.substr(0, bytes_written) << std::endl; // +
    std::cout << "*******************************************" << std::endl << std::endl;

    /// проверить не записалось ли все
    /// если записалось, то отчистить буффер и сменить статус
    if (bytes_written >= (int)_writingBuffer.size()) {
        _connectionState = IS_PROCESSING_FIRST_LINE;
        _writingBuffer = "";
        if (_closeConnection)
        {
            _connectionState = CLOSE;
            close(_socket);
        }
        return ;
    }
    else if (bytes_written <= 0) {
        _connectionState = CLOSE;
        close(_socket);
    }
    /// сдвинуть буффер
    _writingBuffer = _writingBuffer.substr(bytes_written);
}

WritingTransmitterClass::~WritingTransmitterClass() {}