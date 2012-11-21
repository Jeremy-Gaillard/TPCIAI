/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package interface_windows;

import java.io.IOException;
import java.net.UnknownHostException;
import javax.swing.JOptionPane;
import network.NetworkInterface;
import vue.Connexion;

/**
 *
 * @author michael
 */
public class InterfaceWindows {

    public NetworkInterface network;
    
    /*
     * 
     * Classe principale lançant l'application.
     */
    public InterfaceWindows() throws UnknownHostException, IOException{
        Connexion fi = new Connexion(this);
        fi.setVisible(true);        
    }
    /**
     * @param args the command line arguments
     */
    @SuppressWarnings("ResultOfObjectAllocationIgnored")
    public static void main(String[] args) throws UnknownHostException, IOException {
        new InterfaceWindows();
    }
    
    public void error(String title, String msg, Exception ex) {
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
