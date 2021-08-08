#include "WritingTransmitterClass.hpp"

WritingTransmitterClass::WritingTransmitterClass(int socket, std::string & responseStatus, ConnectionState & connectionState,
                                                 std::string & writingBuffer, bool & closeConnection)
: ATransmitterClass(socket, responseStatus, connectionState, writingBuffer, closeConnection) {}

void WritingTransmitterClass::operate() {

    int bytes_written = 0;
    /// записать сколько получится
    bytes_written = write(_socket, _writingBuffer.c_str(), _writingBuffer.size());
    std::cout << "|" << _writingBuffer.substr(0, bytes_written) << "|" << std::endl;

    /// проверить не записалось ли все
    /// если записалось, то отчистить буффер и сменить статус
    if ((size_t)bytes_written == _writingBuffer.size()) {
        _connectionState = IS_PROCESSING_FIRST_LINE;
        _writingBuffer = "";
        if (_closeConnection)
            _connectionState = CLOSE;
        return ;
    }
    /// сдвинуть буффер
    _writingBuffer += bytes_written;
}

WritingTransmitterClass::~WritingTransmitterClass() {}