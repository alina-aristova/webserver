#include "server.hpp"
#include "webserver.hpp"

int main() {
    Server *one = new Server(18000, 2, "/root");
    Server *two = new Server(18001, 2, "/root");
    WebServer * web = new WebServer();
    web->addServer(*one);
    web->addServer(*two);
    web->work();
    delete(one);
    delete(web);
    delete(two);
}