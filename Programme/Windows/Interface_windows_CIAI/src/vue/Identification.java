/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package vue;

import interface_windows_ciai.Interface_windows_CIAI;
import java.io.IOException;
import java.io.PrintStream;
import java.io.PrintWriter;
import java.io.StringWriter;
import java.net.UnknownHostException;
import java.util.logging.Level;
import java.util.logging.Logger;
import network.NetworkInterface;
//import java.net.NetworkInterface;

/**
 *
 * @author michael
 */
public class Identification extends javax.swing.JFrame {

    Interface_windows_CIAI app;
    
    /**
     * Creates new form Identification
     */
    public Identification(Interface_windows_CIAI inter) throws UnknownHostException, IOException {
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

        j_identification = new javax.swing.JTextField();
        jLabel1 = new javax.swing.JLabel();
        jLabel2 = new javax.swing.JLabel();
        B_connect = new javax.swing.JButton();
        j_mot_de_passe = new javax.swing.JPasswordField();
        jLabel3 = new javax.swing.JLabel();
        j_IP_linux = new javax.swing.JTextField();

        setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);

        j_identification.setText("JB");

        jLabel1.setText("Identification");

        jLabel2.setText("Mot de passe");

        B_connect.setText("Connection");
        B_connect.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                B_connectActionPerformed(evt);
            }
        });

        j_mot_de_passe.setText("C");

        jLabel3.setText("IP de la machine Linux");

        j_IP_linux.setText("134.214.161.152");

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(layout.createSequentialGroup()
                        .addGap(31, 31, 31)
                        .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addGroup(layout.createSequentialGroup()
                                .addComponent(jLabel3)
                                .addGap(36, 36, 36)
                                .addComponent(j_IP_linux, javax.swing.GroupLayout.DEFAULT_SIZE, 148, Short.MAX_VALUE))
                            .addGroup(layout.createSequentialGroup()
                                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                    .addComponent(jLabel1)
                                    .addComponent(jLabel2))
                                .addGap(32, 32, 32)
                                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                                    .addComponent(j_identification)
                                    .addComponent(j_mot_de_passe)))))
                    .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, layout.createSequentialGroup()
                        .addContainerGap(javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                        .addComponent(B_connect)))
                .addContainerGap())
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addGap(30, 30, 30)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(jLabel3)
                    .addComponent(j_IP_linux, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(j_identification, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(jLabel1))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(jLabel2)
                    .addComponent(j_mot_de_passe, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, 40, Short.MAX_VALUE)
                .addComponent(B_connect)
                .addContainerGap())
        );

        pack();
    }// </editor-fold>//GEN-END:initComponents

    private void B_connectActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_B_connectActionPerformed
        //Connection d'un utilisateur
        
        if ("".equals(j_identification.getText()))
        {
            System.out.println("insert your login");
        }
        else if ("".equals(j_mot_de_passe.getText()))
        {
            System.out.println("insert your password");
        }
        else if ("".equals(j_IP_linux.getText()))
        {
            System.out.println("insert your IP");
        }
        else
        {
            //System.out.println("Ok");
            String host = j_IP_linux.getText();
            try {
                app.network = new NetworkInterface(host);
            } catch (UnknownHostException ex) {
                //Logger.getLogger(Identification.class.getName()).log(Level.SEVERE, null, ex);
                /*String stack = new String();
                PrintWriter pw = new PrintStream();
                ex.printStackTrace(pw);
                ex.g*/
                app.error("Unknown Host", "The host '"+host+"' is unknown!");
                ex.printStackTrace(System.err);
            } catch (IOException ex) {
                //Logger.getLogger(Identification.class.getName()).log(Level.SEVERE, null, ex);
                app.error("IO Exception", "Could not send a request to '"+host+"'!");
                ex.printStackTrace(System.err);
            } finally {
                //if (app.network == null) System.exit(-1);
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
        /*
         * Set the Nimbus look and feel
         */
        //<editor-fold defaultstate="collapsed" desc=" Look and feel setting code (optional) ">
        /*
         * If Nimbus (introduced in Java SE 6) is not available, stay with the
         * default look and feel. For details see
         * http://download.oracle.com/javase/tutorial/uiswing/lookandfeel/plaf.html
         */
        try {
            for (javax.swing.UIManager.LookAndFeelInfo info : javax.swing.UIManager.getInstalledLookAndFeels()) {
                if ("Nimbus".equals(info.getName())) {
                    javax.swing.UIManager.setLookAndFeel(info.getClassName());
                    break;
                }
            }
        } catch (ClassNotFoundException ex) {
            java.util.logging.Logger.getLogger(Identification.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (InstantiationException ex) {
            java.util.logging.Logger.getLogger(Identification.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (IllegalAccessException ex) {
            java.util.logging.Logger.getLogger(Identification.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        } catch (javax.swing.UnsupportedLookAndFeelException ex) {
            java.util.logging.Logger.getLogger(Identification.class.getName()).log(java.util.logging.Level.SEVERE, null, ex);
        }
        //</editor-fold>

        /*
         * Create and display the form
         */
       /* java.awt.EventQueue.invokeLater(new Runnable() {

            public void run() {
                new Identification().setVisible(true);
            }
        });*/
    }
    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JButton B_connect;
    private javax.swing.JLabel jLabel1;
    private javax.swing.JLabel jLabel2;
    private javax.swing.JLabel jLabel3;
    private javax.swing.JTextField j_IP_linux;
    private javax.swing.JTextField j_identification;
    private javax.swing.JPasswordField j_mot_de_passe;
    // End of variables declaration//GEN-END:variables
}
