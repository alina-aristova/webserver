#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <map>
# define MAX_PORT_NUMBER 65535

enum socket_state {
    STARTED_READING = 0,
    KEEPS_READING = 1,
    STARTED_WRITING = 2;
};

class Server {
private:
    int                         _readingFrom;
    int                         _writingTo;
    int                         _listeningSocket;
    std::map<int, std::string>  _accessedSockets;
    std::map<int, socket_state> _accessedSocketsStatus;
    std::string                 _root;
public:
    Server(const int readingFrom,
           const int writingTo,
           std::string root);
    int getWhereReadsFrom() const;
    int getWhereWritesTo() const;
    const std::string & getRoot() const;
    int getListeningSocket() const;
    bool isSocketReading(int index) const;
    void socketStartsReading(int index);
    void socketStartsWring(int index);
    void setListeningSocket(int listeningSocket);
    void changeAccessedSocketStatus(socket_state newState);
    socket_state getAccessedSocketState(int index) const;
    std::map<int, std::string> & getAccessedSockets();
    void addAccessedSockets(int accessedSocket);
    class wrongPortException : public std::exception {
        const char * what() const throw() {
            return ("Ports cannot be over 65535 or below 1.");
        }
    };
};

#endif