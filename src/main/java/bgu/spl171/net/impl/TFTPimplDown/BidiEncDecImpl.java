package bgu.spl171.net.impl.TFTPimplDown;

import bgu.spl171.net.api.MessageEncoderDecoder;

import java.util.ArrayList;
import java.util.Arrays;

/**
 * Created by blumenra on 1/18/17.
 */
public class BidiEncDecImpl implements MessageEncoderDecoder<BidiMessage>{

    private ArrayList<Byte> incomingBytes;
    private BidiMessage incomingMessage;
    private int currentMessageFieldNumber;


    @Override
    public BidiMessage decodeNextByte(byte nextByte) {

        boolean done = false;

        if(incomingBytes == null){

            incomingBytes = new ArrayList<>();
            incomingMessage = new BidiMessage();
            currentMessageFieldNumber = 0;
        }

        if(currentMessageFieldNumber == 0){

            bytesToOpcode(nextByte);
//            DIRQ, DISC
            if(incomingMessage.getOpcode() == 6 || incomingMessage.getOpcode() == 10){

                done = true;
            }
        }
        else{

            short opcode = incomingMessage.getOpcode();

            switch (opcode){

//                DELRQ, RRQ, WRQ
                case 1:
                case 2:
                case 8: {

                    done = delrqRrqWrqMessageCreator(nextByte);
                    break;
                }

//                DATA
                case 3: {

                    done = dataMessageCreator(nextByte);
                    break;
                }

//                ACK
                case 4: {

                    done = ackMessageCreator(nextByte);
                    break;
                }

//                ERROR
                case 5: {

                    done = errorMessageCreator(nextByte);
                    break;
                }
//
////                DIRQ, DISC
//                case 6:
//                case 10: {
//
//                    return incomingMessage;
//                }

//                LOGRQ
                case 7: {

                    done = logrqMessageCreator(nextByte);
                    break;
                }

//                BCAST
                case 9: {

                    done = bcastMessageCreator(nextByte);
                    break;
                }

//                unKnown opcode
                default: {

                    incomingMessage.setOpcode((short) -1);
                    return incomingMessage;
                }
            }
        }

        if(done){
            BidiMessage newMessage = new BidiMessage(this.incomingMessage);

            doneWithMessage();
            return newMessage;
        }
        else {

            return null;
        }
    }



    //    After determining the message type, these methods create the relevant message
    private boolean delrqRrqWrqMessageCreator(byte nextByte) {

        boolean done = false;

        if(currentMessageFieldNumber == 1){

            if(bytesToFileName(nextByte)){

                incomingMessage.setaByte(nextByte);
                done = true;
            }
        }
        return done;
    }

    private boolean dataMessageCreator(byte nextByte) {

        boolean done = false;

        switch (currentMessageFieldNumber){

//            working on the packetSize
            case 1: {

                bytesToPacketSize(nextByte);
                break;
            }

//            working on blockNumber
            case 2: {

                bytesToBlockNumber(nextByte);

                if(currentMessageFieldNumber == 3 && incomingMessage.getPacketSize() == 0){

                    done = true;
                }

                break;
            }

//            working on data
            case 3: {

                done = bytesToData(nextByte);
                break;
            }
        }

        return done;
    }

    private boolean ackMessageCreator(byte nextByte) {

        boolean done = false;

        if(currentMessageFieldNumber == 1){

            done = bytesToBlockNumber(nextByte);
        }
        return done;
    }

    private boolean errorMessageCreator(byte nextByte) {

        boolean done = false;

        switch(currentMessageFieldNumber){

//            working on errorCode
            case 1: {

                bytesToErrorCode(nextByte);
                break;
            }

//            working on errMsg
            case 2: {

                done = bytesToErrMsg(nextByte);

                if(done){

                    incomingMessage.setaByte(nextByte);
                }

                break;
            }
        }

        return done;
    }

    private boolean logrqMessageCreator(byte nextByte) {

        boolean done = false;

        if(currentMessageFieldNumber == 1){

            if(bytesToUserName(nextByte)){

                incomingMessage.setaByte(nextByte);
                done = true;
            }
        }
        return done;
    }

    private boolean bcastMessageCreator(byte nextByte) {

        boolean done = false;

        switch (currentMessageFieldNumber){

//            deletedAdded
            case 1: {

                incomingMessage.setDeletedAdded(nextByte);
                doneWithField();
                break;
            }

//            fileName
            case 2: {

                done = bytesToFileName(nextByte);

                if(done){

                    incomingMessage.setaByte(nextByte);
                }

                break;
            }
        }

        return done;
    }



    //    These methods set the relevant field in the incomingMessage from the stream of bytes
    private void bytesToOpcode(byte nextByte) {

        short opcode = incomingBytesToShort(nextByte);

        if(opcode != -1){

            incomingMessage.setOpcode(opcode);
            doneWithField();
        }
    }

    private boolean bytesToFileName(byte nextByte){

        String fileName = incomingBytesToString(nextByte);

        if(fileName != null){

            incomingMessage.setFileName(fileName);
            doneWithField();
            return true;
        }

        return false;
    }

    private boolean bytesToBlockNumber(byte nextByte) {

        short blockNumber = incomingBytesToShort(nextByte);

        if(blockNumber != -1){

            incomingMessage.setBlockNumber(blockNumber);
            doneWithField();
            return true;
        }

        return false;
    }

    private void bytesToPacketSize(byte nextByte) {

        short packetSize = incomingBytesToShort(nextByte);

        if(packetSize != -1){

            incomingMessage.setPacketSize(packetSize);
            doneWithField();
        }
    }

    private boolean bytesToData(byte nextByte) {

        incomingBytes.add(nextByte);

        if(incomingBytes.size() == incomingMessage.getPacketSize()){

            byte[] data = incomingBytesToArr();
            incomingMessage.setData(data);
            doneWithField();
            return true;
        }

        return false;
    }

    private void bytesToErrorCode(byte nextByte) {

        short errorCode = incomingBytesToShort(nextByte);

        if(errorCode != -1){

            incomingMessage.setErrorCode(errorCode);
            doneWithField();
        }
    }

    private boolean bytesToErrMsg(byte nextByte) {

        String errMsg = incomingBytesToString(nextByte);

        if(errMsg != null){

            incomingMessage.setErrMsg(errMsg);
            doneWithField();
            return true;
        }

        return  false;
    }

    private boolean bytesToUserName(byte nextByte) {

        String userName = incomingBytesToString(nextByte);

        if(userName != null){

            incomingMessage.setUserName(userName);
            doneWithField();
            return true;
        }

        return false;
    }



//    These methods convert the stream of bytes to the relevant data type
    private short incomingBytesToShort(byte nextByte) {

        incomingBytes.add(nextByte);

        if(incomingBytes.size() == 2){

            byte[] shortBytes = {incomingBytes.get(0), incomingBytes.get(1)};
            short num = bytesToShort(shortBytes);

            return num;
        }
        return (short) -1;
    }

    private String incomingBytesToString(byte nextByte) {

        if(nextByte == (byte) 0){

            byte[] fileNameBytes = incomingBytesToArr();

            String string = new String(fileNameBytes);

            return string;
        }
        else{

            incomingBytes.add(nextByte);
            return null;
        }
    }


//    Other helper methods
    private byte[] incomingBytesToArr() {

        byte[] byteArr = new byte[incomingBytes.size()];

        for(int i=0; i<byteArr.length; i++){

            byteArr[i] = incomingBytes.get(i);
        }

        return byteArr;
    }


//    These methods set this Object's fields to the desired state when needed
    private void doneWithMessage() {

        this.incomingBytes = null;
        this.incomingMessage = null;
        currentMessageFieldNumber = 0;
    }

    private void doneWithField() {

        emptyIncomingBytes();
        currentMessageFieldNumber++;
    }

    private void emptyIncomingBytes(){

        while(!incomingBytes.isEmpty()){

            incomingBytes.remove(0);
        }

    }


    @Override
    public byte[] encode(BidiMessage message) {

        byte[] encoded;

        short msgType = message.getOpcode();
        byte[] msgTypeBytes = shortToBytes(msgType);

        switch (msgType){

//            DELRQ, RRQ, WRQ
            case 1:
            case 2:
            case 8:{

                String fileName = message.getFileName();
                byte[] fileNameBytes = fileName.getBytes();

                byte[] aByte = {message.getaByte()};

                encoded = new byte[fileNameBytes.length + 3];

                putInByteArray(msgTypeBytes, encoded, 0);
                putInByteArray(fileNameBytes, encoded, 2);
                putInByteArray(aByte, encoded, encoded.length-1);

                break;
            }

//            DATA
            case 3: {

                short packetSize = message.getPacketSize();
                byte[] packetSizeBytes = shortToBytes(packetSize);

                short blockNumber = message.getBlockNumber();
                byte[] blockNumberBytes = shortToBytes(blockNumber);

                byte[] data = message.getData();

                encoded = new byte[data.length + 6];

                putInByteArray(msgTypeBytes, encoded, 0);
                putInByteArray(packetSizeBytes, encoded, 2);
                putInByteArray(blockNumberBytes, encoded, 4);
                putInByteArray(data, encoded, 6);

                break;
            }

//            ACK
            case 4: {

                short blockNumber = message.getBlockNumber();
                byte[] blockNumberBytes = shortToBytes(blockNumber);

                encoded = new byte[4];

                putInByteArray(msgTypeBytes, encoded, 0);
                putInByteArray(blockNumberBytes, encoded, 2);

                break;
            }

//            ERROR
            case 5: {

                short errorCode = message.getErrorCode();
                byte[] errorCodeBytes = shortToBytes(errorCode);

                String errMsg = message.getErrMsg();
                byte[] errMsgBytes = errMsg.getBytes();

                byte[] aByte = {message.getaByte()};

                encoded = new byte[errMsgBytes.length + 5];

                putInByteArray(msgTypeBytes, encoded, 0);
                putInByteArray(errorCodeBytes, encoded, 2);
                putInByteArray(errMsgBytes, encoded, 4);
                putInByteArray(aByte, encoded, encoded.length-1);

                break;
            }

//            DIRQ, DISC
            case 6:
            case 10: {

                encoded = msgTypeBytes;
                break;
            }

//            LOGRQ
            case 7: {

                String userName = message.getUserName();
                byte[] userNameBytes = userName.getBytes();

                byte[] aByte = {message.getaByte()};

                encoded = new byte[userNameBytes.length + 3];

                putInByteArray(msgTypeBytes, encoded, 0);
                putInByteArray(userNameBytes, encoded, 2);
                putInByteArray(aByte, encoded, encoded.length-1);

                break;
            }

//            BCAST
            case 9: {

                byte[] deletedAdded = {message.getDeletedAdded()};

                String fileName = message.getFileName();
                byte[] fileNameBytes = fileName.getBytes();

                byte[] aByte = {message.getaByte()};

                encoded = new byte[fileNameBytes.length + 4];

                putInByteArray(msgTypeBytes, encoded, 0);
                putInByteArray(deletedAdded, encoded, 2);
                putInByteArray(fileNameBytes, encoded, 3);
                putInByteArray(aByte, encoded, encoded.length-1);

                break;
            }

            default:{
                encoded = new byte[0];
            }
        }

        return encoded;
    }

    private byte[] shortToBytes(short num) {

        byte[] bytesArr = new byte[2];
        bytesArr[0] = (byte)((num >> 8) & 0xFF);
        bytesArr[1] = (byte)(num & 0xFF);
        return bytesArr;
    }

    private short bytesToShort(byte[] byteArr) {

        short result = (short)((byteArr[0] & 0xff) << 8);
        result += (short)(byteArr[1] & 0xff);
        return result;
    }

    private void putInByteArray(byte[] toPut, byte[] container, int fromIndex){

        for(int putIndex = 0, containerIndex = fromIndex; putIndex < toPut.length ; putIndex++, containerIndex++){

            container[containerIndex] = toPut[putIndex];
        }
    }

}
