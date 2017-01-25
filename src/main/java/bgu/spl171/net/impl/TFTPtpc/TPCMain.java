package bgu.spl171.net.impl.TFTPtpc;


import bgu.spl171.net.impl.TFTPimplDown.*;
import bgu.spl171.net.srv.Server;

import java.io.File;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentSkipListSet;

/**
 * Created by blumenra on 1/17/17.
 */
public class TPCMain {

    public static void main(String[] args) {

        int port = Integer.parseInt(args[0]);

        Map<String, BidiFile> filesList = new ConcurrentHashMap<>();

        //start the server
        Server.threadPerClient(
                port, //port
                () -> new BidiServerProtocolImpl(filesList), //protocol factory
                BidiEncDecImpl::new //message encoder decoder factory
        ).serve();
    }
}
