#include "../include/connectionHandler.h"

//1
using boost::asio::ip::tcp;
 
using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::string;
 
ConnectionHandler::ConnectionHandler(string host, short port, BidiEncDec& encDec, ClientProtocol& protocol):
host_(host), port_(port), io_service_(), socket_(io_service_), encDec(encDec), protocol(protocol){}
    
ConnectionHandler::~ConnectionHandler() {
    close();
}
 
bool ConnectionHandler::connect() {
    std::cout << "Starting connect to " 
        << host_ << ":" << port_ << std::endl;
    try {
        tcp::endpoint endpoint(boost::asio::ip::address::from_string(host_), port_); // the server endpoint
        boost::system::error_code error;
        socket_.connect(endpoint, error);
        if (error)
            throw boost::system::system_error(error);
    }
    catch (std::exception& e) {
        std::cerr << "Connection failed (Error: " << e.what() << ')' << std::endl;
        return false;
    }
    return true;
}
 
bool ConnectionHandler::getBytes(char bytes[], unsigned int bytesToRead) {
    size_t tmp = 0;
    boost::system::error_code error;
    try {
        while (!error && bytesToRead > tmp ) {

            tmp += socket_.read_some(boost::asio::buffer(bytes+tmp, bytesToRead-tmp), error);
        }
        if(error)
            throw boost::system::system_error(error);
    } catch (std::exception& e) {
        std::cerr << "recv failed (Error: " << e.what() << ')' << std::endl;
        return false;
    }
    return true;
}
 
bool ConnectionHandler::sendBytes(const char bytes[], int bytesToWrite) {

    int tmp = 0;

    boost::system::error_code error;

    try {

        while (!error && bytesToWrite > tmp ) {

            tmp += socket_.write_some(boost::asio::buffer(bytes + tmp, (size_t) (bytesToWrite - tmp)), error);
        }
        if(error)
            throw boost::system::system_error(error);
    } catch (std::exception& e) {
        std::cerr << "recv failed (Error: " << e.what() << ')' << std::endl;
        return false;
    }
    return true;
}

 
// Close down the connection properly.
void ConnectionHandler::close() {
    try{
        socket_.close();
    } catch (...) {
        std::cout << "closing failed: connection already closed" << std::endl;
    }
}

bool ConnectionHandler::getMessage(BidiMessage& message) {

    char ch[1];

    try {
        do{

            getBytes(ch, 1);
            encDec.decodeNextByte(ch[0], message);

        }while(!message.isComplete());
    } catch (std::exception& e) {
        std::cerr << "recv failed (Error: " << e.what() << ')' << std::endl;
        return false;
    }

    return true;
}

bool ConnectionHandler::sendMessage(BidiMessage &message) {

    if(message.getOpcode() == (short) 9){
        return true;
    }
    if(message.getOpcode() == (short) 10){

        char encoded[message.getBytesLength()];
        encDec.encode(message, encoded);

        sendBytes(encoded, message.getBytesLength());

        return false;
    }

    char encoded[message.getBytesLength()];
    encDec.encode(message, encoded);

    return sendBytes(encoded, message.getBytesLength());
}

bool ConnectionHandler::processMessage() {

    bool result;

    protocol.setCommunicationCompleted(false);

    while(!protocol.isComunicationCompleted()) {

        BidiMessage answer = BidiMessage();
        BidiMessage reply = BidiMessage();

        result = getMessage(answer);

        if(!result) {

            protocol.setCommunicationCompleted(false);
            return result;
        }

        if(protocol.isReadyToDisconnect()) {

            return false;
        }

        protocol.process(answer, reply);

        if(protocol.isReadyToDisconnect()){

            return false;
        }


        if(reply.getOpcode() == -1){

            break;
        }

        if(!protocol.isStartReading()) {

            sendMessage(reply);
        }
    }

    return result;
}
