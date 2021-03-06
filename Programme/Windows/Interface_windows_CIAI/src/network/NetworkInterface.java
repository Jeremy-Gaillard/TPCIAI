package network;

import java.io.BufferedOutputStream;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.Socket;
import java.net.UnknownHostException;

/*
 * Classe de gestion de l'interface réseau : envoi/réception de messages.
 */
public class NetworkInterface {
    
    Socket client;
    BufferedReader client_reader;
    boolean open;
    
    public NetworkInterface(String ip, int port) throws UnknownHostException, IOException {
        System.out.println("Starting client socket...");
        client = new Socket(ip, port);
        client_reader = new BufferedReader(new InputStreamReader(client.getInputStream()));
        open = true;
    }

    /*
    * Fonction envoyant des messages par socket au serveur Linux
    */
    public void send_message(String msg) throws IOException {
            System.out.println("Sending message: '"+msg+"'");
            OutputStream oStream = client.getOutputStream();
            BufferedOutputStream bOStream = new BufferedOutputStream(oStream);
            for (byte c : msg.getBytes()) {
                    bOStream.write(c);
            }
            bOStream.flush();
    }

    /*
    * Fonction écoutant les messages par socket du serveur Linux
    */
    public String listen_message() throws IOException {
        
        System.out.println("Listening...");
        
        StringBuilder sb = new StringBuilder();
        char r;
        while ((r = (char) client_reader.read()) != '\n') {
                sb.append(r);
        }
        String msg = sb.toString();
        
        return msg;
    }
    /*
    * Fonction fermant proprement la connection avec le serveur
    */
    public void close() throws IOException {
        if (open) {
            client.close();
            open = false;
        }
    }

    @Override
    protected void finalize() throws Throwable {
        close();
        super.finalize();
    }

}
