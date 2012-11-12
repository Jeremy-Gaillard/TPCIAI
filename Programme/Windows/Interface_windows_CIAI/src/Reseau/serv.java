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

class NetworkInterface {
	Socket client;
	ServerSocket server;
	
	public NetworkInterface() throws UnknownHostException, IOException {
		client = new Socket("134.214.105.197", 32768);
	}
	
	public void recovery_order() {
	
	}
	
	public void restart_order() {
	
	}

	public void command() {
	
	}
	
	public String send_message(String msg) throws IOException {
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
System.out.println("lolinp");
		while ((r = (char) feedback.read()) != '\n') {
			//System.out.print((int)r);
			//System.out.print(r);
//System.out.println(r);
			sb.append(r);
		}
		return sb.toString();
	}

	public void listen_messages() throws IOException {
		server = new ServerSocket(32767);
		Socket clientSocket = null;
		try {
			clientSocket = server.accept();
		} catch (IOException e) {
			System.out.println("Accept failed: 4444");
			System.exit(-1);
		}
		PrintWriter out = new PrintWriter(clientSocket.getOutputStream(), true);
		BufferedReader in = 
			new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
		String inputLine, outputLine;
		
		/*
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
		System.out.println(sb.toString());

	}

}

class Main {
	
	public static void main(String[] args) throws IOException {
		/*
		//Socket client = new Socket("localhost", 32768);
		//Socket client = new Socket("127.0.0.1", 32768);
		//Socket client = new Socket("192.168.242.128", 32768);
		//Socket client = new Socket("134.214.161.17", 32768);
		//Socket client = new Socket("10.0.2.15", 32768);
		//Socket client = new Socket("fe80::a00:27ff:fe4f:db5b", 32768);
		//Socket client = new Socket("134.214.161.78", 32768);
		Socket client = new Socket("134.214.105.197", 32768);
		
		OutputStream oStream = client.getOutputStream();
		BufferedOutputStream bOStream = new BufferedOutputStream(oStream);
		
		//int c = 300;
		char c = 'a';
		bOStream.write(c);
		bOStream.write('l');
		bOStream.flush();
		InputStream feedback = client.getInputStream();
		char r;
		while ((r = (char) feedback.read()) != '\0') {
			//System.out.print((int)r);
			System.out.print(r);
		}
		System.out.println();
		*/
		//new Socket("134.214.105.197", 32768);
		NetworkInterface ni = new NetworkInterface();
		System.out.println(ni.send_message("lolilol"));
		ni.listen_messages();
	}

}