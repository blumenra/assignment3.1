#ifndef CLIENT_PROTOCOL_
#define CLIENT_PROTOCOL_
                                           
#include <string>
#include <iostream>
#include <boost/asio.hpp>
#include "BidiMessage.h"
#include <fstream>
#include <iostream>

//1
class ClientProtocol {
private:
	bool waitingToLogin;
	int lastRqCode;
    vector<char> dataBytesBuffer;
    string receivingFileName;
    int previousReceivedBlock;
    int lastSentBlockNum;
    bool dontReply;
	string sendingFileName;
	vector<unsigned char> uploadFileData;
	bool communicationCompleted;
    bool readyToDisconnect;

	void addDataToBuffer(BidiMessage message);

public:
    ClientProtocol();

    int getLastRqCode() const;

    void setLastRqCode(int lastRqCode);

    void setCommunicationCompleted(bool communicationCompleted);

    void process(BidiMessage& message, BidiMessage& reply);

	bool isComunicationCompleted() const;

	bool isStartReading() const;

	bool isReadyToDisconnect() const;

}; //class ConnectionHandler
 
#endif