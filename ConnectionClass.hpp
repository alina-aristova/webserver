#ifndef CONNECTION_CLASS_HPP
# define CONNECTION_CLASS_HPP

# include "ReadingTransmitterClass.hpp"
# include "WritingTransmitterClass.hpp"
# include "HostClass.hpp"
# include <iostream>
# include <map>
# include "ConnectionStateENUM.hpp"

class ConnectionClass {
private:
    int _socket;
    bool _closeConnection;
    std::string _responseStatus;
    std::string _writingBuff;
    ConnectionState _stateOfConnection;
    std::map<std::string, HostClass> _hosts;
    WritingTransmitterClass *_writer;
    ReadingTransmitterClass *_reader;
public:
    ConnectionClass(int socket, std::map<std::string, HostClass> hosts);
    void transmit();
};

#endif