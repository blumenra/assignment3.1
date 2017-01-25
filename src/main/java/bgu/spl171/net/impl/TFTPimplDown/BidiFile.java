package bgu.spl171.net.impl.TFTPimplDown;

/**
 * Created by brukes on 1/20/17.
 */
public class BidiFile {

    private final String fileName;
    private boolean readable;
    private boolean uploading;



    private boolean deletable;

    public BidiFile(String fileName) {

        this.fileName = fileName;
        this.readable = true;
        this.uploading = false;
        this.deletable = true;
    }

    public void setReadable(boolean readable) {
        this.readable = readable;
    }

    public synchronized void setUploading(boolean uploading) {

        this.uploading = uploading;

        if(uploading) {

            setDeletable(false);
            setReadable(false);
        }
        else {

            setDeletable(true);
            setReadable(true);
        }

    }

    public void setDeletable(boolean deletable) {
        this.deletable = deletable;
    }

    public String getFileName() {
        return fileName;
    }

    public boolean isReadable() {
        return readable;
    }

    public boolean isUploading() {
        return uploading;
    }

    public boolean isDeletable() {
        return deletable;
    }
}
