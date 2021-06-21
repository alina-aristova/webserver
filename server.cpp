#include "server.hpp"

Server::Server(const int readingFrom, const int writingTo, std::string root)
: _readingFrom(readingFrom), _writingTo(writingTo), _root(root) {
    if (readingFrom <= 0 || readingFrom > MAX_PORT_NUMBER)
        throw wrongPortException();
    if (writingTo <= 0 || writingTo > MAX_PORT_NUMBER)
        throw wrongPortException();
}

int Server::getWhereReadsFrom() const { return (_readingFrom); }

int Server::getWhereWritesTo() const { return (_writingTo); }

const std::string & Server::getRoot() const { return (_root); }

std::map<int, std::string> & Server::getAccessedSockets() { return (_accessedSockets); }

int Server::getListeningSocket() const { return (_listeningSocket); }

void Server::addAccessedSockets(int accessedSocket) {
    _accessedSockets[accessedSocket] = "";
    _accessedSocketsStatus[accessedSocket] = STARTED_READING;
}

socket_state Server::getAccessedSocketState(int index) const { return (_accessedSocketsStatus.at(index)); }

void Server::changeAccessedSocketStatus(socket_state newState) { _accessedSocketsStatus = newState; }

void Server::setListeningSocket(int listeningSocket) { _listeningSocket = listeningSocket; }
