package bgu.spl171.net.impl.TFTPimplDown;


/**
 *  opcode operation
 *      1 Read request (RRQ)
 *      2 Write request (WRQ)
 *      3 Data (DATA)
 *      4 Acknowledgment (ACK)
 *      5 Error (ERROR)
 *      6 Directory listing request (DIRQ)
 *      7 Login request (LOGRQ)
 *      8 Delete request (DELRQ)
 *      9 Broadcast file added/deleted (BCAST)
 *      10 Disconnect (DISC)
 */
public class BidiMessage {

    private short opcode;
    private String fileName;
    private String userName;
    private short packetSize;
    private short blockNumber;
    private byte[] data;
    private byte deletedAdded;
    private short errorCode;
    private String errMsg;
    private byte aByte;

    public static BidiMessage createDataMessage(int packetSize, int blockNum, byte[] fileData) {

        return new BidiMessage((short) 3, (short) packetSize, (short) blockNum, fileData);
    }

    public static BidiMessage createBcastMessage(int event, String fileName) {

        return new BidiMessage((short) 9, (byte) event, fileName, (byte) 0);
    }

    public static BidiMessage createAckMessage(int ackNum) {

        return new BidiMessage((short) 4, (short) ackNum);
    }

    public static BidiMessage createErrorMessage(int errNum, String errMsg) {

        return new BidiMessage((short) 5, (short) errNum, errMsg, (byte) 0);
    }

//    Constructors

//    empty


    public BidiMessage() {
    }

    //    copy
    public BidiMessage(BidiMessage original) {

        this.opcode = original.getOpcode();
        switch (opcode){
//            LOGRQ
            case 7: {
                this.userName = original.getUserName();
                this.aByte = original.getaByte();

                break;
            }

//            DELRQ,RRQ,WRQ
            case 1:
            case 2:
            case 8: {
                this.fileName = original.getFileName();
                this.aByte = original.getaByte();

                break;
            }

//            DATA
            case 3: {

                this.packetSize = original.getPacketSize();
                this.blockNumber = original.getBlockNumber();
                this.data = original.getData();

                break;
            }

//            ACK
            case 4: {
                this.blockNumber = original.getBlockNumber();

                break;
            }

//            BCAST
            case 9: {

                this.deletedAdded = original.getDeletedAdded();
                this.fileName = original.getFileName();
                this.aByte = original.getaByte();

                break;
            }

//            ERROR
            case 5: {

                this.errorCode = original.getErrorCode();
                this.errMsg = original.getErrMsg();
                this.aByte  = original.getaByte();

                break;
            }
        }
    }



    private BidiMessage(short opcode, String aString, byte aByte) {
        this.opcode = opcode;
        switch (opcode){

            case 7: {
                this.userName = aString;

                break;
            }

            case 1:
            case 2:
            case 8: {
                this.fileName = aString;

                break;
            }


        }
        this.fileName = aString;
        this.aByte = aByte;
    }

    private BidiMessage(short opcode) {

        this.opcode = opcode;
    }

    private BidiMessage(short opcode, short packetSize, short blockNumber, byte[] data) {

        this.opcode = opcode;
        this.packetSize = packetSize;
        this.blockNumber = blockNumber;
        this.data = data;
    }

    private BidiMessage(short opcode, short blockNumber) {

        this.opcode = opcode;
        this.blockNumber = blockNumber;
    }

    private BidiMessage(short opcode, byte deletedAdded, String fileName, byte aByte) {

        this.opcode = opcode;
        this.deletedAdded = deletedAdded;
        this.fileName = fileName;
        this.aByte = aByte;
    }

    private BidiMessage(short opcode, short errorCode, String errMsg, byte aByte) {

        this.opcode = opcode;
        this.errorCode = errorCode;
        this.errMsg = errMsg;
        this.aByte = aByte;
    }


//    getters

    public short getOpcode() {

        return this.opcode;
    }

    public String getFileName() {

        return fileName + "";
    }

    public byte getaByte() {

        return aByte;
    }

    public String getUserName() {
        return userName + "";
    }

    public short getPacketSize() {
        return packetSize;
    }

    public short getBlockNumber() {
        return blockNumber;
    }

    public byte[] getData() {

        byte[] dataCopy = new byte[packetSize];

        for(int i=0; i<packetSize; i++){

            dataCopy[i] = this.data[i];
        }

        return dataCopy;
    }

    public byte getDeletedAdded() {
        return deletedAdded;
    }

    public short getErrorCode() {
        return errorCode;
    }

    public String getErrMsg() {
        return errMsg + "";
    }


//    setters

    public void setOpcode(short opcode) {
        this.opcode = opcode;
    }

    public void setFileName(String fileName) {
        this.fileName = fileName;
    }

    public void setUserName(String userName) {
        this.userName = userName;
    }

    public void setPacketSize(short packetSize) {
        this.packetSize = packetSize;
    }

    public void setBlockNumber(short blockNumber) {
        this.blockNumber = blockNumber;
    }

    public void setData(byte[] data) {
        this.data = data;
    }

    public void setDeletedAdded(byte deletedAdded) {
        this.deletedAdded = deletedAdded;
    }

    public void setErrorCode(short errorCode) {
        this.errorCode = errorCode;
    }

    public void setErrMsg(String errMsg) {
        this.errMsg = errMsg;
    }

    public void setaByte(byte aByte) {
        this.aByte = aByte;
    }
}
