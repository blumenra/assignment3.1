
#ifndef CLIENT_BIDIMESSAGE_H
#define CLIENT_BIDIMESSAGE_H

#include <string>
#include <iostream>

//1
using namespace std;

class BidiMessage {

private:
    short opcode = -1;
    string fileName = "";
    string userName = "";
    short packetSize = -1;
    short blockNumber = -1;
    char data[512];
    char deletedAdded = (char) 0;
    short errorCode = -1;
    string errMsg = "";
    char aByte = (char) 0;
    bool complete = false;
    int bytesLength = 0;



public:

    static BidiMessage createDataMessage(int packetSize, int blockNum, char* fileData);
    static BidiMessage createAckMessage(int blockckNumber);
    static BidiMessage createLoginMessage(string userName);
    static BidiMessage createDeleteMessage(string fileName);
    static BidiMessage createRRQMessage(string fileName);
    static BidiMessage createWRQMessage(string fileName);
    static BidiMessage createDirMessage();
    static BidiMessage createDiscMessage();

    BidiMessage(short opcode, string aString, char aByte);
    BidiMessage(short opcode);
    BidiMessage(short opcode, short packetSize, short blockNumber, char* data);
    BidiMessage(short opcode, short blockNumber);
    BidiMessage(short opcode, char deletedAdded, string fileName, char aByte);
    BidiMessage(short opcode, short errorCode, string errMsg, char aByte);


//    empty
    BidiMessage();

//    copy
    BidiMessage(const BidiMessage& original);

//    getters
    short getOpcode()const;
    string getFileName()const;
    char getaByte()const;
    string getUserName()const;
    short getPacketSize()const;
    short getBlockNumber()const;
    void copyData(char* dataCopy)const;
    char getDeletedAdded()const;
    short getErrorCode()const;
    string getErrMsg()const;
    bool isComplete() const;
    int getBytesLength() const;

//    setters
    void setOpcode(short opcode);
    void setFileName(string fileName);
    void setUserName(string userName);
    void setPacketSize(short packetSize);
    void setBlockNumber(short blockNumber);
    void setData(char* data);
    void setDeletedAdded(char deletedAdded);
    void setErrorCode(short errorCode);
    void setErrMsg(string errMsg);
    void setaByte(char aByte);
    void setComplete(bool complete);
    void setBytesLength(int byteLength);

    virtual ~BidiMessage();

    BidiMessage& operator=(const BidiMessage& other);
};

#endif //CLIENT_BIDIMESSAGE_H