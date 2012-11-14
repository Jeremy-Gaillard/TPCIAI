package network;

import java.io.BufferedOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;
import java.net.ServerSocket;
import java.net.UnknownHostException;
import java.io.PrintWriter;
import java.io.BufferedReader;
import java.io.InputStreamReader;

public class NetworkInterface {

        public class ServerInitThread extends Thread {
            
            NetworkInterface network;
            public ServerInitThread(NetworkInterface network) {
                this.network = network;
            }
            public void run() {
            System.out.println("Starting server socket...");
                try {
                    //System.out.println("Hello from a thread!");
                    //network.listen_messages();
network.server = new ServerSocket(32767);
            Socket clientSocket = null;
            //try {
                    clientSocket = server.accept();
            //BufferedReader in = 
network.client_reader = 
                    new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
            System.out.println("Server socket established.");

                } catch (IOException ex) {
                    Logger.getLogger(NetworkInterface.class.getName()).log(Level.SEVERE, null, ex);
                    
                    
                    System.err.println("Could not establish server socket!");
                    
                }
            }

        }
    
	Socket client;
	ServerSocket server;
	BufferedReader client_reader;
        
	public NetworkInterface() throws UnknownHostException, IOException {
            //this("134.214.105.197");
            this("if219-06.insa-lyon.fr");
        }
	public NetworkInterface(String ip) throws UnknownHostException, IOException {
            (new ServerInitThread(this)).start();
            System.out.println("Starting client socket...");
            client = new Socket(ip, 32768);
//client = new Socket("localhost", 32768);
	}
	/*
	public void recovery_order() {
	
	}
	
	public void restart_order() {
	
	}

	public void command() {
	
	}
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

	public void listen_messages() throws IOException {
/*
            System.out.println("Starting server socket...");
            server = new ServerSocket(32767);
            Socket clientSocket = null;
                    clientSocket = server.accept();
            BufferedReader in = 
                    new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
*/
            /*
            PrintWriter out = new PrintWriter(clientSocket.getOutputStream(), true);
            String inputLine, outputLine;

            // initiate conversation with client
            KnockKnockProtocol kkp = new KnockKnockProtocol();
            outputLine = kkp.processInput(null);
            out.println(outputLine);
            while ((inputLine = in.readLine()) != null) {   
            outputLine = kkp.processInput(inputLine);
            out.println(outputLine);
            if (outputLine.equals("Bye."))
            break;
            }*/

            StringBuilder sb = new StringBuilder();
            
            char r;
while ((r = (char) client_reader.read()) != '\n') {
System.out.println(r);
                    sb.append(r);
            }
/*
int r;
while ((r = in.read()) != -1) {
System.out.println(r);
System.out.println((char)r);
                    sb.append((char)r);
            }
*/



            //System.out.println(sb.toString());
            String msg = sb.toString();
            System.out.println("Received message: '"+msg+"'");
	}
        @Override
        protected void finalize() throws Throwable {
            client.close();
            server.close();
            super.finalize();
        }

}
