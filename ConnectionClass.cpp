#include "ConnectionClass.hpp"

ConnectionClass::ConnectionClass(int socket, std::map<std::string, Server *> hosts) : _socket(socket),
_closeConnection(false), _responseStatus("200"), _writingBuff(""), _stateOfConnection(IS_PROCESSING_FIRST_LINE), _hosts(hosts),
_writer(new WritingTransmitterClass(_socket, _responseStatus, _stateOfConnection, _writingBuff, _closeConnection)),
_reader(new ReadingTransmitterClass(_socket, _responseStatus, _stateOfConnection, _writingBuff, _closeConnection, _hosts)) {
}

void ConnectionClass::receive() {
    if (_stateOfConnection == IS_PROCESSING_FIRST_LINE
    || _stateOfConnection == IS_PROCESSING_HEADERS
    || _stateOfConnection == IS_READING_BODY
    || _stateOfConnection == ERROR_WHILE_READING
    || _stateOfConnection == IS_FORMING_RESPONSE) {
        _reader->env = this->env;
        _reader->operate();
        _stateOfConnection = _reader->getConnectionState();
    }
}

void ConnectionClass::transmit() {
    if (_stateOfConnection == IS_WRITING_RESPONSE) {
        _writer->operate();
        _stateOfConnection = _writer->getConnectionState();
    }
}

ConnectionState ConnectionClass::getConnectionStatus() { return _stateOfConnection; }

void ConnectionClass::setEnv(char **env) { this->env = env; }

