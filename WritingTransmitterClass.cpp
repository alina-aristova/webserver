#include "WritingTransmitterClass.hpp"

WritingTransmitterClass::WritingTransmitterClass(int socket, std::string & responseStatus, ConnectionState & connectionState,
                                                 std::string & writingBuffer, bool & closeConnection)
: ATransmitterClass(socket, responseStatus, connectionState, writingBuffer, closeConnection) {}

void WritingTransmitterClass::operate() {
    std::cout << "socket: \"" << getsocket() << "\"" << std::endl;
    std::cout << "Connection state: \"" << getConnectionState() << "\"" << std::endl;
    std::cout << "Response status: \"" << getResponseStatus() << "\"" << std::endl;
    std::cout << "Writing buffer: \"" << getWritingBuffer() << "\"" << std::endl;
}

WritingTransmitterClass::~WritingTransmitterClass() {}