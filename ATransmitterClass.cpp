# include "ATransmitterClass.hpp"

ATransmitterClass::ATransmitterClass(int socket, std::string &responseStatus, ConnectionState connectionState,
std::string &writingBuffer, bool & closeConnection) : _socket(socket), _responseStatus(responseStatus), _connectionState(connectionState),
_writingBuffer(writingBuffer), _closeConnection(closeConnection) {}

ATransmitterClass::~ATransmitterClass() {}

int ATransmitterClass::getsocket() const { return (_socket); }

const std::string & ATransmitterClass::getResponseStatus() const { return (_responseStatus); }

const ConnectionState & ATransmitterClass::getConnectionState() const { return (_connectionState); }

const std::string & ATransmitterClass::getWritingBuffer() const { return (_writingBuffer); }