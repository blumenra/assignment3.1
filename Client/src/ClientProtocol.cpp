#include <fstream>
#include "../include/ClientProtocol.h"

//1
ClientProtocol::ClientProtocol():
waitingToLogin(false),
lastRqCode(-1),
dataBytesBuffer(),
receivingFileName(""),
previousReceivedBlock(-1),
lastSentBlockNum(0),
dontReply(false),
sendingFileName(""),
uploadFileData(),
communicationCompleted(false),
readyToDisconnect(false)
{}

void ClientProtocol::process(BidiMessage& message, BidiMessage& reply) {

	short opcode = message.getOpcode();
    communicationCompleted = false;

    switch(opcode) {

//        RRQ
        case 1: {

            reply = message;
            receivingFileName = message.getFileName();
            communicationCompleted = true;

            lastRqCode = 1;

            break;
        }

//        WRQ
        case 2: {

            reply = message;
            sendingFileName = message.getFileName();
            communicationCompleted = true;

            // read file
            std::ifstream fileReadStream(sendingFileName, std::ifstream::binary);

            // Find the length of the file
            fileReadStream.seekg(0, fileReadStream.end);
            std::streampos flength = fileReadStream.tellg();
            fileReadStream.seekg(0, fileReadStream.beg);

            // Create a vector to read it into
            std::vector<unsigned char> uploadFileData1(flength);


            // Actually read data
            fileReadStream.read((char *)uploadFileData1.data(), flength);

            uploadFileData = uploadFileData1;

            fileReadStream.close();


            lastRqCode = 2;

            break;
        }
//        DATA
        case 3: {

            addDataToBuffer(message);

            previousReceivedBlock = message.getBlockNumber();

            if (message.getPacketSize() < 512) {

                unsigned long dataSize = dataBytesBuffer.size();
                char receivedData[dataSize];

                switch (lastRqCode) {

//                    RRQ in data
                    case 1: {

                        for (unsigned long i = 0; i < dataSize; ++i) {

                            receivedData[i] = dataBytesBuffer.at(i);
                        }

                        if (dataSize != 0) {

                            ofstream receivedFile(receivingFileName, ios::out | ios::binary);
                            receivedFile.write(receivedData, dataSize);
                            receivedFile.close();
                        } else {

                            ofstream receivedFile(receivingFileName, ios::out);
                            receivedFile.write(receivedData, dataSize);
                            receivedFile.close();
                        }

                        std::cout << "RRQ " << receivingFileName << " complete" << std::endl;

                        break;
                    }

//                    DIRQ in data
                    case 6: {

                        for (unsigned long i = 0; i < dataSize; ++i) {

                            if (dataBytesBuffer[i] == '\0') {
                                dataBytesBuffer[i] = '\n';
                            }

                            receivedData[i] = dataBytesBuffer.at(i);
                        }

                        std::cout << string(receivedData).substr(0, dataSize) << std::endl;

                        break;
                    }

                    default: {

                        break;
                    }
                }

                dataBytesBuffer.clear();
                lastRqCode = -1;
                communicationCompleted = true;
            }

            reply = BidiMessage::createAckMessage(message.getBlockNumber());

            break;
        }

//        ACK
		case 4: {

            std::cout << "ACK " << message.getBlockNumber() << std::endl;

            switch (lastRqCode){

//                RRQ in ACK
                case 1: {

                    dontReply = true;

                    break;
                }

//                WRQ in ACK
                case 2: {

                    int x = 512;  // chunk size

                    if((int) uploadFileData.size() >= x) {
                        char data[x];
                        for (int i = 0; i < x; i++) {

                            data[i] = uploadFileData.at(0);
                            uploadFileData.erase(uploadFileData.begin());
                        }

                        lastSentBlockNum++;
                        reply = BidiMessage::createDataMessage(x,lastSentBlockNum , data);
                    }
                    else{
                        int size = (int) uploadFileData.size();
                        char data[size];

                        for (unsigned int i = 0; i < uploadFileData.size(); i++) {

                            data[i] = uploadFileData.at(0);

                            uploadFileData.erase(uploadFileData.begin());
                        }

                        lastSentBlockNum++;
                        reply = BidiMessage::createDataMessage(size,lastSentBlockNum , data);

                        std::cout << "WRQ " << sendingFileName << " complete" << std::endl;
                        lastRqCode = -1;
                        sendingFileName = "";
                        communicationCompleted = true;
                        lastSentBlockNum = 0;
                    }

                    break;
                }

//                DISC in ACK
                case 10: {

                    readyToDisconnect = true;
                    communicationCompleted = true;
                    lastRqCode = -1;

                    break;
                }

                default: {

                    lastRqCode = -1;
                    receivingFileName = "";
                    communicationCompleted = true;

                    break;
                }
            }

            break;
        }

//        ERROR
        case 5: {

            std::cout << "Error " << message.getErrorCode() << std::endl;
            dontReply = true;
            communicationCompleted = true;
            break;
        }

//        BCAST
        case 9: {

            reply = message;
            if(message.getFileName() == receivingFileName){

                communicationCompleted = false;
            }

            int broadReason = message.getDeletedAdded();

            if(broadReason == 0 || broadReason == 1) {

                std::cout << "BCAST " << broadReason << " " << message.getFileName() << std::endl;
            }

            break;
        }

//        DIRQ, DISQ
        case 6:
        case 10: {

            reply = message;
            communicationCompleted = true;

            lastRqCode = opcode;

            break;
        }

        case 7: { //LOGRQ

            waitingToLogin = true;
            reply = message;
//            communicationCompleted = true;
            break;
        }


//        DELRQ
        case 8: {

            reply = message;
            receivingFileName = message.getFileName();
            communicationCompleted = true;

            lastRqCode = opcode;

            break;
        }

        default:{

            break;
        }

	}
}

void ClientProtocol::addDataToBuffer(BidiMessage message) {

    char data[message.getPacketSize()];
    message.copyData(data);
    int size = message.getPacketSize();

    for (int i = 0; i < size; ++i) {

        dataBytesBuffer.push_back(data[i]);
    }
}

bool ClientProtocol::isComunicationCompleted() const {
    return communicationCompleted;
}

void ClientProtocol::setCommunicationCompleted(bool communicationCompleted) {
    ClientProtocol::communicationCompleted = communicationCompleted;
}

bool ClientProtocol::isStartReading() const {
    return dontReply;
}

bool ClientProtocol::isReadyToDisconnect() const {
    return readyToDisconnect;
}
