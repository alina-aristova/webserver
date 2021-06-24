#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <map>
# include "ParseRequest/Includes/parseRequest.hpp"
# define MAX_PORT_NUMBER 65535

enum socket_state {
    STARTED_READING = 0,
    KEEPS_READING = 1,
    STARTED_WRITING = 2
};

class Server {
private:
    int                         _readingFrom;
    int                         _writingTo;
    int                         _listeningSocket;
    int                         _startingIndexWrite;
    std::map<int, std::string>  _accessedSockets;
    std::map<int, socket_state> _accessedSocketsStatus;
    std::map<int, ParseRequest> _accessedSocketsRequests;
    std::string                 _root;
public:
    Server(const int readingFrom,
           const int writingTo,
           std::string root);
    int getWhereReadsFrom() const;
    int getStartingIndexWriting() const;
    void setStartingIndexWriting(int setTo);
    int getWhereWritesTo() const;
    const std::string & getRoot() const;
    int getListeningSocket() const;
    bool isSocketReading(int index) const;
    void socketStartsReading(int index);
    void socketStartsWring(int index);
    ParseRequest & getRequestForSocket(int index);
    void setListeningSocket(int listeningSocket);
    void changeAccessedSocketStatus(int index, socket_state newState);
    socket_state getAccessedSocketState(int index) const;
    void addRequestForSocket(int index);
    std::map<int, std::string> & getAccessedSockets();
    void addAccessedSockets(int accessedSocket);
    class wrongPortException : public std::exception {
        const char * what() const throw() {
            return ("Ports cannot be over 65535 or below 1.");
        }
    };
};

#endif