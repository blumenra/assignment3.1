
#include "../include/BidiMessage.h"

//1
BidiMessage BidiMessage::createDataMessage(int packetSize, int blockNum, char* fileData) {

    return BidiMessage((short) 3, (short) packetSize, (short) blockNum, fileData);
}

BidiMessage BidiMessage::createAckMessage(int blockckNumber){

    return BidiMessage((short) 4, (short) blockckNumber);
}

BidiMessage BidiMessage::createLoginMessage(string userName) {

    return BidiMessage((short) 7, userName, (char) 0);
}

BidiMessage BidiMessage::createDeleteMessage(string fileName) {

    return BidiMessage((short) 8, fileName, (char) 0);
}

BidiMessage BidiMessage::createRRQMessage(string fileName) {

    return BidiMessage((short) 1, fileName, (char) 0);
}

BidiMessage BidiMessage::createWRQMessage(string fileName) {

    return BidiMessage((short) 2, fileName, (char) 0);
}

BidiMessage BidiMessage::createDirMessage() {

    return BidiMessage((short) 6);
}

BidiMessage BidiMessage::createDiscMessage() {

    return BidiMessage((short) 10);
}







//empty
BidiMessage::BidiMessage():
opcode(-1),
fileName(""),
userName(""),
packetSize(-1),
blockNumber(-1),
deletedAdded((char) 0),
errorCode(-1),
errMsg(""),
aByte((char) 0),
complete(false),
bytesLength(0)
{}

//copy
BidiMessage::BidiMessage(const BidiMessage& original){

    this->opcode = original.getOpcode();
    this->complete = original.isComplete();
    this->bytesLength = original.getBytesLength();

    this->bytesLength = original.getBytesLength();
    switch (opcode){
//            LOGRQ
        case 7: {
            this->userName = original.getUserName();
            this->aByte = original.getaByte();
            break;
        }

//            DELRQ,RRQ,WRQ
        case 1:
        case 2:
        case 8: {
            this->fileName = original.getFileName();
            this->aByte = original.getaByte();
            break;
        }

//            DATA
        case 3: {

            this->packetSize = original.getPacketSize();
            this->blockNumber = original.getBlockNumber();

            original.copyData(this->data);
            break;
        }

//            ACK
        case 4: {
            this->blockNumber = original.getBlockNumber();
            break;
        }

//            BCAST
        case 9: {

            this->deletedAdded = original.getDeletedAdded();
            this->fileName = original.getFileName();
            this->aByte = original.getaByte();
            break;
        }

//            ERROR
        case 5: {

            this->errorCode = original.getErrorCode();
            this->errMsg = original.getErrMsg();
            this->aByte  = original.getaByte();
            break;
        }

        default:
            break;
    }
}


BidiMessage::BidiMessage(short opcode, string aString, char aByte){

    this->opcode = opcode;
    this->complete = true;
    this->bytesLength = (int) aString.length() + 3;
    switch (opcode){

        case 7: {
            this->userName = aString;
            break;
        }

        case 1:
        case 2:
        case 8: {
            this->fileName = aString;
            break;
        }

        default:
            break;
    }
    this->fileName = aString;
    this->aByte = aByte;
}

BidiMessage::BidiMessage(short opcode):
opcode(opcode),
fileName(""),
userName(""),
packetSize(-1),
blockNumber(-1),
deletedAdded((char) 0),
errorCode(-1),
errMsg(""),
aByte((char) 0),
complete(true),
bytesLength(2)
{}

BidiMessage::BidiMessage(short opcode, short packetSize, short blockNumber, char* data):
opcode(opcode),
fileName(""),
userName(""),
packetSize(packetSize),
blockNumber(blockNumber),
deletedAdded((char) 0),
errorCode(-1),
errMsg(""),
aByte((char) 0),
complete(true),
bytesLength((packetSize + 6))
{
    setData(data);
}

BidiMessage::BidiMessage(short opcode, short blockNumber):
opcode(opcode),
fileName(""),
userName(""),
packetSize(-1),
blockNumber(blockNumber),
deletedAdded((char) 0),
errorCode(-1),
errMsg(""),
aByte((char) 0),
complete(true),
bytesLength(4)
{}

BidiMessage::BidiMessage(short opcode, char deletedAdded, string fileName, char aByte):
opcode(opcode),
fileName(fileName),
userName(""),
packetSize(-1),
blockNumber(-1),
deletedAdded(deletedAdded),
errorCode(-1),
errMsg(""),
aByte(aByte),
complete(true),
bytesLength((int) fileName.length() + 4)
{}

BidiMessage::BidiMessage(short opcode, short errorCode, string errMsg, char aByte):
opcode(opcode),
fileName(""),
userName(""),
packetSize(-1),
blockNumber(-1),
deletedAdded((char) 0),
errorCode(errorCode),
errMsg(errMsg),
aByte(aByte),
complete(true),
bytesLength((int) errMsg.length() + 5)
{}


//getters

short BidiMessage::getOpcode()const {

    return opcode;
}

string BidiMessage::getFileName()const {

    return fileName + "";
}

char BidiMessage::getaByte()const {

    return aByte;
}

string BidiMessage::getUserName()const {

    return userName + "";
}

short BidiMessage::getPacketSize()const {

    return packetSize;
}

short BidiMessage::getBlockNumber()const {

    return blockNumber;
}

void BidiMessage::copyData(char* dataCopy)const {

    for(int i=0; i<packetSize; i++){

        dataCopy[i] = this->data[i];
    }

}

char BidiMessage::getDeletedAdded()const {

    return deletedAdded;
}

short BidiMessage::getErrorCode()const {

    return errorCode;
}

string BidiMessage::getErrMsg()const {

    return errMsg + "";
}

bool BidiMessage::isComplete() const {
    return complete;
}

int BidiMessage::getBytesLength() const {
    return bytesLength;
}


//setters

void BidiMessage::setOpcode(short opcode){

    this->opcode = opcode;
}

void BidiMessage::setFileName(string fileName){

    this->fileName = fileName;
}

void BidiMessage::setUserName(string userName){

    this->userName = userName;
}

void BidiMessage::setPacketSize(short packetSize){

    this->packetSize = packetSize;
}

void BidiMessage::setBlockNumber(short blockNumber){

    this->blockNumber = blockNumber;
}

void BidiMessage::setData(char* data){

    for(int i=0; i<packetSize; i++){

        this->data[i] = data[i];
    }
}

void BidiMessage::setDeletedAdded(char deletedAdded){

    this->deletedAdded = deletedAdded;
}

void BidiMessage::setErrorCode(short errorCode){

    this->errorCode = errorCode;
}

void BidiMessage::setErrMsg(string errMsg){

    this->errMsg = errMsg;
}

void BidiMessage::setaByte(char aByte){

    this->aByte = aByte;
}

void BidiMessage::setComplete(bool complete) {
    BidiMessage::complete = complete;
}

void BidiMessage::setBytesLength(int byteLength) {
    BidiMessage::bytesLength = byteLength;
}

BidiMessage::~BidiMessage() {}

BidiMessage& BidiMessage::operator=(const BidiMessage& other) // copy assignment
{
    if (this != &other) { // self-assignment check expected

        this->opcode = other.getOpcode();
        this->fileName = other.getFileName();
        this->userName = other.getUserName();
        this->packetSize = other.getPacketSize();
        this->blockNumber = other.getBlockNumber();
        other.copyData(this->data);
        this->deletedAdded = other.getDeletedAdded();
        this->errorCode = other.getErrorCode();
        this->errMsg = other.getErrMsg();
        this->aByte = other.getaByte();
        this->complete = other.isComplete();
        this->bytesLength = other.getBytesLength();
    }
    return *this;
}




