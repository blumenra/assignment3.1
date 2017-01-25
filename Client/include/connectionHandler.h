#ifndef CONNECTION_HANDLER__
#define CONNECTION_HANDLER__
                                           
#include <string>
#include <iostream>
#include <boost/asio.hpp>
#include "BidiEncDec.h"
#include "ClientProtocol.h"

//1
using boost::asio::ip::tcp;
 
class ConnectionHandler {
private:
    const std::string host_;
    const short port_;
    boost::asio::io_service io_service_;   // Provides core I/O functionality
    tcp::socket socket_;
    BidiEncDec encDec;
    ClientProtocol& protocol;

public:
    ConnectionHandler(std::string host, short port, BidiEncDec& encDec, ClientProtocol& protocol);
    virtual ~ConnectionHandler();
 
    // Connect to the remote machine
    bool connect();
 
    // Read a fixed number of bytes from the server - blocking.
    // Returns false in case the connection is closed before bytesToRead bytes can be read.
    bool getBytes(char bytes[], unsigned int bytesToRead);
 
    // Send a fixed number of bytes from the client - blocking.
    // Returns false in case the connection is closed before all the data is sent.
    bool sendBytes(const char bytes[], int bytesToWrite);
    
    // Close down the connection properly.
    void close();

    bool getMessage(BidiMessage& message);

    bool sendMessage(BidiMessage& message);

    bool processMessage();

}; //class ConnectionHandler
 
#endif