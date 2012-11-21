/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package vue;

import interface_windows.InterfaceWindows;
import java.io.IOException;

/**
 *
 * @author michael
 */
public class Parametrage extends javax.swing.JFrame {

    InterfaceWindows app;
    
    /**
     * Creates new form Parametrage
     */
    public Parametrage(InterfaceWindows inter) {
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

        jLabel2 = new javax.swing.JLabel();
        jLabel3 = new javax.swing.JLabel();
        jLabel4 = new javax.swing.JLabel();
        B_envoi_parametrage = new javax.swing.JButton();
        j_nb_palette_A = new javax.swing.JTextField();
        j_nb_palette_B = new javax.swing.JTextField();
        j_nb_pourcentage = new javax.swing.JTextField();
        B_annuler = new javax.swing.JButton();

        setDefaultCloseOperation(javax.swing.WindowConstants.DO_NOTHING_ON_CLOSE);
        setTitle("Interface de paramétrage du lot de fabrication");
        addWindowListener(new java.awt.event.WindowAdapter() {
            public void windowClosing(java.awt.event.WindowEvent evt) {
                closing(evt);
            }
        });

        jLabel2.setText("Nombre de palettes A");

        jLabel3.setText("Nombre de palettes B");

        jLabel4.setText("% d'erreurs autorisées par carton");

        B_envoi_parametrage.setText("Confirmer");
        B_envoi_parametrage.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                B_envoi_parametrageActionPerformed(evt);
            }
        });

        j_nb_palette_A.setText("42");

        j_nb_palette_B.setText("42");

        j_nb_pourcentage.setText("50");

        B_annuler.setText("Annuler");
        B_annuler.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                B_annulerActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addContainerGap()
                .addComponent(B_annuler)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addComponent(B_envoi_parametrage)
                .addContainerGap())
            .addGroup(layout.createSequentialGroup()
                .addGap(35, 35, 35)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                    .addComponent(jLabel3)
                    .addComponent(jLabel2)
                    .addComponent(jLabel4))
                .addGap(18, 18, 18)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING, false)
                    .addComponent(j_nb_palette_A)
                    .addComponent(j_nb_palette_B)
                    .addComponent(j_nb_pourcentage, javax.swing.GroupLayout.PREFERRED_SIZE, 81, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addContainerGap(58, Short.MAX_VALUE))
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, layout.createSequentialGroup()
                .addGap(39, 39, 39)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(jLabel2)
                    .addComponent(j_nb_palette_A, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(18, 18, 18)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(jLabel3)
                    .addComponent(j_nb_palette_B, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(18, 18, 18)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(jLabel4)
                    .addComponent(j_nb_pourcentage, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, 55, Short.MAX_VALUE)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(B_annuler)
                    .addComponent(B_envoi_parametrage))
                .addContainerGap())
        );

        pack();
    }// </editor-fold>//GEN-END:initComponents

    private void B_envoi_parametrageActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_B_envoi_parametrageActionPerformed

        int nb_palette_commande_A = -1;
        int nb_palette_commande_B = -1;
        int pourcentage = -1;
        try {
            nb_palette_commande_A = Integer.parseInt(j_nb_palette_A.getText());
            nb_palette_commande_B = Integer.parseInt(j_nb_palette_B.getText());
            pourcentage = Integer.parseInt(j_nb_pourcentage.getText());
            if (nb_palette_commande_A < 0 || nb_palette_commande_B < 0 || pourcentage < 0)
            {
                app.error("Données invalides", "Entrez des valeurs positives pour les différentes paramètres.");
            }
            else
            {
                try {
                    //Envoie le message d'une commande au serveur Linux
                    String parametrage_lot = "0 " + nb_palette_commande_A + " " + nb_palette_commande_B + " " +  pourcentage;
                    app.network.send_message(parametrage_lot);
                    this.dispose();
                } catch (IOException ex) {
                    app.error("IO Exception", "Could not send the command to the host!");
                }    
                //ouverture de la fenêtre de suivi
                Suivi fv = new Suivi(app);
                fv.setVisible(true);
                this.dispose();
            }
        } catch (java.lang.NumberFormatException ex){
            app.error("Format numérique", "Entrez des valeurs valides pour les différentes paramètres.", ex);
        }
        
    }//GEN-LAST:event_B_envoi_parametrageActionPerformed

    private void B_annulerActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_B_annulerActionPerformed
        dispose();
        app.quit();
    }//GEN-LAST:event_B_annulerActionPerformed

    private void closing(java.awt.event.WindowEvent evt) {//GEN-FIRST:event_closing
        dispose();
        app.quit();
    }//GEN-LAST:event_closing

    
    /*public static void main(String args[]) {
       
        java.awt.EventQueue.invokeLater(new Runnable() {

            public void run() {
                new Parametrage().setVisible(true);
            }
        });
    }*/
    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JButton B_annuler;
    private javax.swing.JButton B_envoi_parametrage;
    private javax.swing.JLabel jLabel2;
    private javax.swing.JLabel jLabel3;
    private javax.swing.JLabel jLabel4;
    private javax.swing.JTextField j_nb_palette_A;
    private javax.swing.JTextField j_nb_palette_B;
    private javax.swing.JTextField j_nb_pourcentage;
    // End of variables declaration//GEN-END:variables
}
