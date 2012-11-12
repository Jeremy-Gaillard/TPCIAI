/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package interface_windows_ciai;

import java.io.IOException;
import java.net.UnknownHostException;
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
    public static void main(String[] args) throws UnknownHostException, IOException {
        // TODO code application logic here
        //Interface_windows_CIAI test = new Interface_windows_CIAI();
        Identification fi = new Identification(null);
        fi.setVisible(true);  
    }
}
