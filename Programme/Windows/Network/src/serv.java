import java.io.BufferedOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.net.Socket;

class Main {

	public static void main(String[] args) throws IOException {
		//Socket client = new Socket("localhost", 32768);
		Socket client = new Socket("127.0.0.1", 32768);
		//Socket client = new Socket("192.168.242.128", 32768);
		//Socket client = new Socket("134.214.161.17", 32768);
		//Socket client = new Socket("10.0.2.15", 32768);
		//Socket client = new Socket("fe80::a00:27ff:fe4f:db5b", 32768);
		
		OutputStream oStream = client.getOutputStream();
		BufferedOutputStream bOStream = new BufferedOutputStream(oStream);
		//int c = 300;
		char c = 'a';
		bOStream.write(c);
		bOStream.flush();
	}
	
}
