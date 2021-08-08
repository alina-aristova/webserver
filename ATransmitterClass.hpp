#ifndef ATRANSMITTER_CLASS_HPP
# define ATRANSMITTER_CLASS_HPP

# include <string>
# include <iostream>
# include <fcntl.h>
# include <unistd.h>
# include "ConnectionStateENUM.hpp"
# include "ParseRequest/utils/utils.hpp"
# include <cstdlib>
# include "ParseRequest/Includes/parseRequest.hpp"
# include "ParseRequest/response/response.hpp"

# define BUFF_SIZE 1024

class ATransmitterClass {
protected:
    int _socket;
    std::string & _responseStatus;
    std::string &_writingBuffer;
    bool & _closeConnection;
public:
    ConnectionState _connectionState;
    ATransmitterClass(int socket, std::string & responseStatus, ConnectionState connectionState, std::string &writingBuffer, bool & closeConnection);
    virtual void operate() = 0;
    int getsocket() const;
    const std::string & getResponseStatus() const;
    const ConnectionState & getConnectionState() const;
    const std::string & getWritingBuffer() const;
    virtual ~ATransmitterClass();
};

#endif
