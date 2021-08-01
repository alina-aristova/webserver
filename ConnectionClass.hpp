#ifndef CONNECTION_CLASS_HPP
# define CONNECTION_CLASS_HPP

# include "ReadingTransmitterClass.hpp"
# include "WritingTransmitterClass.hpp"
# include "config_parse/includes/Configuration.hpp"
# include "config_parse/includes/Server.hpp"
# include "config_parse/includes/Location.hpp"
# include <iostream>
# include <map>
# include "ConnectionStateENUM.hpp"

class ConnectionClass {
private:
    int _socket;
    bool _closeConnection;
    std::string _responseStatus;
    std::string _writingBuff;
    std::map<std::string, Server *> _hosts;
public:
    WritingTransmitterClass *_writer;
    ReadingTransmitterClass *_reader;
    ConnectionState _stateOfConnection;
    char **env;
    ConnectionClass(int socket, std::map<std::string, Server *> hosts);
    ConnectionState getConnectionStatus();
    void setEnv(char **env);
    void receive();
    void transmit();
};

#endif