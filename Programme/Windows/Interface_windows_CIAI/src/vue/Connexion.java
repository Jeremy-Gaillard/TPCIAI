/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package vue;

import interface_windows.InterfaceWindows;
import java.io.IOException;
import java.io.PrintStream;
import java.io.PrintWriter;
import java.io.StringWriter;
import java.net.UnknownHostException;
import java.util.logging.Level;
import java.util.logging.Logger;
import network.NetworkInterface;

/**
 *
 * @author michael
 */
public class Connexion extends javax.swing.JFrame {

    InterfaceWindows app;
    
    /**
     * Creates new form Identification
     */
    public Connexion(InterfaceWindows inter) throws UnknownHostException, IOException {
        //inter.network = new NetworkInterface();
        app = inter;
        setLocationByPlatform(true);
        initComponents();
    }

    /**
     * This method is called from within the constructor to initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is always
     * regenerated by the Form Editor.
     */
    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        j_port = new javax.swing.JTextField();
        jLabel1 = new javax.swing.JLabel();
        B_connect = new javax.swing.JButton();
        jLabel3 = new javax.swing.JLabel();
        j_IP_linux = new javax.swing.JTextField();

        setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);
        setTitle("Connexion serveur");

        j_port.setText("32768");

        jLabel1.setText("Port");

        B_connect.setText("Connexion");
        B_connect.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                B_connectActionPerformed(evt);
            }
        });

        jLabel3.setText("IP de la machine Linux");

        j_IP_linux.setText("127.0.0.1");

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addContainerGap()
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(layout.createSequentialGroup()
                        .addGap(0, 0, Short.MAX_VALUE)
                        .addComponent(B_connect))
                    .addGroup(layout.createSequentialGroup()
                        .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                            .addComponent(jLabel1)
                            .addComponent(jLabel3))
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addComponent(j_IP_linux, javax.swing.GroupLayout.DEFAULT_SIZE, 151, Short.MAX_VALUE)
                            .addComponent(j_port))))
                .addContainerGap())
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addContainerGap()
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(j_IP_linux, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(jLabel3))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(jLabel1)
                    .addComponent(j_port, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, 15, Short.MAX_VALUE)
                .addComponent(B_connect)
                .addContainerGap())
        );

        pack();
    }// </editor-fold>//GEN-END:initComponents

    private void B_connectActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_B_connectActionPerformed
        //Connection au serveur linux
        
        if ("".equals(j_port.getText())) 
        {
            app.error(null, "Please enter the server port to use (default is 32768).");
        } 
        
        else if ("".equals(j_IP_linux.getText())) 
        {
            app.error(null, "Please enter the server address.");
        } 
        else 
        {
            String host = j_IP_linux.getText();
            int port = Integer.parseInt(j_port.getText());
            try {
                app.network = new NetworkInterface(host, port);
            } catch (UnknownHostException ex) {
                app.error("Unknown Host", "The host '"+host+"' is unknown!");
            } catch (IOException ex) {
                app.error("IO Exception", "Could not send a request to '"+host+":"+port+"'!");
            } catch (IllegalArgumentException ex) {
                app.error("Argument exception", "Please enter a valid host and port.");
            } finally {
                
            }
            if (app.network != null) {
                Parametrage fp = new Parametrage(app);
                fp.setVisible(true);
                this.dispose();
            }
        }
    }//GEN-LAST:event_B_connectActionPerformed

    /**
     * @param args the command line arguments
     */
    public static void main(String args[]) {

    }
    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JButton B_connect;
    private javax.swing.JLabel jLabel1;
    private javax.swing.JLabel jLabel3;
    private javax.swing.JTextField j_IP_linux;
    private javax.swing.JTextField j_port;
    // End of variables declaration//GEN-END:variables
}
