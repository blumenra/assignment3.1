package bgu.spl171.net.impl.TFTPimplDown;

import bgu.spl171.net.api.bidi.Connections;
import bgu.spl171.net.srv.ConnectionHandler;

import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;

/**
 * Created by blumenra on 1/18/17.
 */
public class ConnectionsImpl<T> implements Connections<T> {

    private Map<Integer, ConnectionHandler<T>> clients = new HashMap<>();
    private Map<Integer, String> loggedInUsers = new HashMap<>();

    @Override
    public boolean send(int connectionId, T msg) {

        clients.get(connectionId).send(msg);

        return false;
    }

    @Override
    public void broadcast(T msg) {

        for(ConnectionHandler<T> client : clients.values()) {

            client.send(msg);
        }
    }

    @Override
    public void disconnect(int connectionId) {


        loggedInUsers.remove(connectionId);
        clients.remove(connectionId);
    }

    public Map<Integer, ConnectionHandler<T>> getClients() {

        return clients;
    }

    public Map<Integer, String> getLoggedInUsers() {
        return loggedInUsers;
    }

    public boolean isLoggedIn(int clientId) {

        return loggedInUsers.containsKey(clientId);
    }

    public void addConnector(int clientId, ConnectionHandler<T> client) {

        clients.put(clientId, client);
    }
}
