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
	Socket client;
	ServerSocket server;
        
	public NetworkInterface() throws UnknownHostException, IOException {
            //this("134.214.105.197");
            this("if219-06.insa-lyon.fr");
        }
	public NetworkInterface(String ip) throws UnknownHostException, IOException {
            System.out.println("Starting client socket...");
            client = new Socket(ip, 32768);
	}
	/*
	public void recovery_order() {
	
	}
	
	public void restart_order() {
	
	}

	public void command() {
	
	}
	*/
	public String send_message(String msg) throws IOException {
                System.out.println("Sending message: '"+msg+"'");
		OutputStream oStream = client.getOutputStream();
		BufferedOutputStream bOStream = new BufferedOutputStream(oStream);
		for (byte c : msg.getBytes()) {
			bOStream.write(c);
		}
		bOStream.flush();
//System.out.println("lolwrote");
		InputStream feedback = client.getInputStream();
		char r;
		StringBuilder sb = new StringBuilder();
//System.out.println("lolinp");
		while ((r = (char) feedback.read()) != '\n') {
			//System.out.print((int)r);
			//System.out.print(r);
//System.out.println(r);
			sb.append(r);
		}
                String resp = sb.toString();
                System.out.println("Received response message: '"+resp+"'");
		return resp;
	}

	public String listen_messages() throws IOException {
            System.out.println("Starting server socket...");
            server = new ServerSocket(32767);
            Socket clientSocket = null;
            //try {
                    clientSocket = server.accept();
            /*} catch (IOException e) {
                    System.out.println("Accept failed: 4444");
                    System.exit(-1);
            }*/
            BufferedReader in = 
                    new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
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

            char r;
            StringBuilder sb = new StringBuilder();
            while ((r = (char) in.read()) != '!') {
                    sb.append(r);
            }
            //System.out.println(sb.toString());
            String msg = sb.toString();
            System.out.println("Received message: '"+msg+"'");
            return msg;
	}
        @Override
        protected void finalize() throws Throwable {
            client.close();
            server.close();
            super.finalize();
        }

}
