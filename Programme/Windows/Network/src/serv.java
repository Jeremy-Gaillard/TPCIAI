import java.io.BufferedOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;

class NetworkInterface {

	public void recovery_order() {
		
	}
	
	public void restart_order() {
		
	}

	public void command() {
		
	}
	
}

class Main {

	public static void main(String[] args) throws IOException {
		//Socket client = new Socket("localhost", 32768);
		//Socket client = new Socket("127.0.0.1", 32768);
		//Socket client = new Socket("192.168.242.128", 32768);
		//Socket client = new Socket("134.214.161.17", 32768);
		//Socket client = new Socket("10.0.2.15", 32768);
		//Socket client = new Socket("fe80::a00:27ff:fe4f:db5b", 32768);
		Socket client = new Socket("134.214.161.78", 32768);
		
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
		
	}
	
}


































