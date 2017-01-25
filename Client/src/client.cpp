#include <stdlib.h>
#include "../include/connectionHandler.h"
#include "../include/bidiInputConverter.h"
#include <boost/thread.hpp>


class Task{
private:
    int id;
    boost::mutex * mutex;
    ConnectionHandler& connectionHandler;
    ClientProtocol& protocol;
    BidiEncDec& encDecl;


public:
    Task(int id, boost::mutex *mutex, ConnectionHandler &connectionHandler, ClientProtocol &protocol, BidiEncDec &encDecl):
    id(id),
    mutex(mutex),
    connectionHandler(connectionHandler),
    protocol(protocol),
    encDecl(encDecl)
    {}

    void handleInput(){
//        boost::mutex::scoped_lock lock(*mutex);

        BidiInputConverter converter;

        while(1) {

            const short bufsize = 1024;
            char buf[bufsize];
            std::cin.getline(buf, bufsize);
            std::string line(buf);

            BidiMessage messagePre = converter.convertInput(line);
            BidiMessage messagePost = BidiMessage();

            protocol.process(messagePre, messagePost);

            if(messagePost.isComplete()) {

                if(!connectionHandler.sendMessage(messagePost)) {

                    return;
                }
            }
        }
    }

    void communicate(){

        while(1) {

            if(!connectionHandler.processMessage()){

                return;
            }
        }
    }
};




/**
* This code assumes that the server replies the exact text the client sent it (as opposed to the practical session example)
*/
int main (int argc, char *argv[]) {

    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " host port" << std::endl << std::endl;
        return -1;
    }
    std::string host = argv[1];
    short port = atoi(argv[2]);

    BidiInputConverter converter;
    BidiEncDec encDec = BidiEncDec();
    ClientProtocol protocol = ClientProtocol();

    ConnectionHandler connectionHandler(host, port, encDec, protocol);
    if (!connectionHandler.connect()) {
        std::cerr << "Cannot connect to " << host << ":" << port << std::endl;
        return 1;
    }

    boost::mutex mutex;
    Task inputHandler(1, &mutex, connectionHandler, protocol, encDec);
    Task communicator(2, &mutex, connectionHandler, protocol, encDec);

    boost::thread userThread(&Task::handleInput, &inputHandler);
    boost::thread serverThread(&Task::communicate, &communicator);
    userThread.join();
    serverThread.join();

    return 0;
}