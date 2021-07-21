#include "ConnectionClass.hpp"

ConnectionClass::ConnectionClass(int socket, std::map<std::string, Server *> hosts) : _socket(socket),
_closeConnection(false), _responseStatus("200"), _writingBuff(""), _stateOfConnection(IS_PROCESSING_FIRST_LINE), _hosts(hosts),
_writer(new WritingTransmitterClass(_socket, _responseStatus, _stateOfConnection, _writingBuff, _closeConnection)),
_reader(new ReadingTransmitterClass(_socket, _responseStatus, _stateOfConnection, _writingBuff, _closeConnection, _hosts)) {
}

void ConnectionClass::transmit() {
    if (_stateOfConnection == IS_PROCESSING_FIRST_LINE
    || _stateOfConnection == IS_PROCESSING_HEADERS
    || _stateOfConnection == IS_READING_BODY
    || _stateOfConnection == ERROR
    || _stateOfConnection == IS_FORMING_RESPONSE)
        _reader->operate();
//    _writer.operate();
}