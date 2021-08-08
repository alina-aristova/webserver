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
    ConnectionState _stateOfConnection;
    std::map<std::string, Server *> _hosts;
    WritingTransmitterClass *_writer;
    ReadingTransmitterClass *_reader;
public:
    char **env;
    ConnectionClass(int socket, std::map<std::string, Server *> hosts);
    ConnectionState getConnectionStatus();
    void setEnv(char **env);
    void receive();
    void transmit();
};

#endif
