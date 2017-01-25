package bgu.spl171.net.impl.TFTPreactor;

import bgu.spl171.net.impl.TFTPimplDown.BidiEncDecImpl;
import bgu.spl171.net.impl.TFTPimplDown.BidiFile;
import bgu.spl171.net.impl.TFTPimplDown.BidiServerProtocolImpl;
import bgu.spl171.net.impl.TFTPimplDown.ConnectionsImpl;
import bgu.spl171.net.srv.Server;

import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

/**
 * Created by blumenra on 1/17/17.
 */
public class ReactorMain {

    public static void main(String[] args) {

        int port = Integer.parseInt(args[0]);
        int nThreads = 4;

        System.out.println(port + "************************");

        Map<String, BidiFile> filesList = new ConcurrentHashMap<>();

        //start the server
        Server.reactor(
                nThreads, //nThreads
                port, //port
                () -> new BidiServerProtocolImpl(filesList), //protocol factory
                BidiEncDecImpl::new //message encoder decoder factory
                ).serve();
    }
}
