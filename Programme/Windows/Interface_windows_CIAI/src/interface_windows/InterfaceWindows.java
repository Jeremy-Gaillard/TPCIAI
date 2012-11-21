/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package interface_windows;

import java.io.IOException;
import java.io.PrintWriter;
import java.io.StringWriter;
import java.net.UnknownHostException;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.swing.JOptionPane;
import network.NetworkInterface;
import vue.Connexion;

/**
 *
 * @author michael
 */
public class InterfaceWindows {

    public NetworkInterface network;
    
    public InterfaceWindows() throws UnknownHostException, IOException{
        Connexion fi = new Connexion(this);
        fi.setVisible(true);        
    }
    /**
     * @param args the command line arguments
     */
    @SuppressWarnings("ResultOfObjectAllocationIgnored")
    public static void main(String[] args) throws UnknownHostException, IOException {
        // TODO code application logic here
        new InterfaceWindows();
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
        JOptionPane.showMessageDialog(null, msg, title, 0);
        System.err.println("Error: "+title+"\n\t"+msg);
    }
    
    public void info(String title, String msg) {
        JOptionPane.showMessageDialog(null, msg, title, 1);
        System.out.println("Info: "+title+"\n\t"+msg);
    }
    
    public void quit() {
        System.out.println("Quitting...");
        try {
            network.close();
        } catch (IOException ex) {
            error("Error on quitting", "Error closing the client connection", ex);
        }
    }
}
