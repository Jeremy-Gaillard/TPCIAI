/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package interface_windows_ciai;

import java.io.IOException;
import java.io.PrintWriter;
import java.io.StringWriter;
import java.net.UnknownHostException;
import javax.swing.JOptionPane;
import network.NetworkInterface;
import vue.Identification;

/**
 *
 * @author michael
 */
public class Interface_windows_CIAI {

    public NetworkInterface network;
    
    public Interface_windows_CIAI() throws UnknownHostException, IOException{
        Identification fi = new Identification(this);
        fi.setVisible(true);        
    }
    /**
     * @param args the command line arguments
     */
    @SuppressWarnings("ResultOfObjectAllocationIgnored")
    public static void main(String[] args) throws UnknownHostException, IOException {
        // TODO code application logic here
        new Interface_windows_CIAI();
    }
    
    public void error(String title, String msg, Exception ex) {
        //throw new UnsupportedOperationException("Not yet implemented");
        /*
        StringWriter sw = new StringWriter();
        ex.printStackTrace(new PrintWriter(sw));
        JOptionPane.showMessageDialog(null, msg+"\n\n"+sw.toString(), title, 0);*/
        JOptionPane.showMessageDialog(null, msg+"\n\n"+ex.toString(), title, 0);
    }
    
    public void error(String title, String msg) {
        //throw new UnsupportedOperationException("Not yet implemented");
        JOptionPane.showMessageDialog(null, msg, title, 0);
        System.err.println("Error: "+title+"\n\t"+msg);
    }
}
