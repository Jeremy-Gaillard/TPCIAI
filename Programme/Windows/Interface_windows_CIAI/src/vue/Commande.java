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
public class Commande extends javax.swing.JFrame {

    InterfaceWindows app;
    vue.Suivi suivi;
    /**
     * Creates new form Commande
     */
    public Commande(InterfaceWindows inter, vue.Suivi fenetre_suivi, int nb_palette_A, int nb_palette_B) {
        app = inter;
        suivi = fenetre_suivi;
        setLocationByPlatform(true);
        initComponents();
        initData(nb_palette_A, nb_palette_B);
    }

    /**
     * This method is called from within the constructor to initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is always
     * regenerated by the Form Editor.
     */
    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        jLabel1 = new javax.swing.JLabel();
        jLabel2 = new javax.swing.JLabel();
        j_nb_palette_commande_B = new javax.swing.JTextField();
        j_nb_palette_commande_A = new javax.swing.JTextField();
        jLabel3 = new javax.swing.JLabel();
        jLabel4 = new javax.swing.JLabel();
        j_nb_palette_actuel_A = new javax.swing.JTextField();
        j_nb_palette_actuel_B = new javax.swing.JTextField();
        B_annuler_commande = new javax.swing.JButton();
        B_envoyer_commande = new javax.swing.JButton();

        setDefaultCloseOperation(javax.swing.WindowConstants.DISPOSE_ON_CLOSE);
        setTitle("Commande");

        jLabel1.setText("Quantité actuelle de palettes A");

        jLabel2.setText("Quantité actuelle de palettes B");

        j_nb_palette_commande_B.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                j_nb_palette_commande_BActionPerformed(evt);
            }
        });

        jLabel3.setText("Nombre de palettes A commandées");

        jLabel4.setText("Nombre de palettes B commandées");

        j_nb_palette_actuel_A.setEditable(false);

        j_nb_palette_actuel_B.setEditable(false);
        j_nb_palette_actuel_B.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                j_nb_palette_actuel_BActionPerformed(evt);
            }
        });

        B_annuler_commande.setText("Annuler");
        B_annuler_commande.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                B_annuler_commandeActionPerformed(evt);
            }
        });

        B_envoyer_commande.setText("Envoyer");
        B_envoyer_commande.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                B_envoyer_commandeActionPerformed(evt);
            }
        });

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(layout.createSequentialGroup()
                        .addGap(58, 58, 58)
                        .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING)
                            .addComponent(jLabel1)
                            .addComponent(jLabel2)
                            .addComponent(jLabel3)
                            .addComponent(jLabel4))
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                        .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.TRAILING, false)
                            .addComponent(j_nb_palette_commande_B, javax.swing.GroupLayout.Alignment.LEADING, javax.swing.GroupLayout.DEFAULT_SIZE, 43, Short.MAX_VALUE)
                            .addComponent(j_nb_palette_commande_A, javax.swing.GroupLayout.Alignment.LEADING)
                            .addComponent(j_nb_palette_actuel_B, javax.swing.GroupLayout.Alignment.LEADING)
                            .addComponent(j_nb_palette_actuel_A, javax.swing.GroupLayout.Alignment.LEADING))
                        .addGap(0, 59, Short.MAX_VALUE))
                    .addGroup(layout.createSequentialGroup()
                        .addContainerGap()
                        .addComponent(B_annuler_commande)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                        .addComponent(B_envoyer_commande)))
                .addContainerGap())
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addContainerGap()
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(j_nb_palette_actuel_A, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(jLabel1, javax.swing.GroupLayout.PREFERRED_SIZE, 14, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(jLabel2)
                    .addComponent(j_nb_palette_actuel_B, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addGap(18, 18, 18)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(jLabel3)
                    .addComponent(j_nb_palette_commande_A, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(j_nb_palette_commande_B, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(jLabel4))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, 50, Short.MAX_VALUE)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(B_annuler_commande)
                    .addComponent(B_envoyer_commande))
                .addContainerGap())
        );

        pack();
    }// </editor-fold>//GEN-END:initComponents

    /**
     * Initialise avec le nombre de cartons actuellement dans l'entrepôt
     */
    private void initData(int nb_palette_A, int nb_palette_B) {
        
        int nb_palette_actuel_A = nb_palette_A;
        int nb_palette_actuel_B = nb_palette_B;

        String palette_actuel_A = String.valueOf(nb_palette_actuel_A);
        String palette_actuel_B = String.valueOf(nb_palette_actuel_B);
        j_nb_palette_actuel_A.setText(palette_actuel_A);
        j_nb_palette_actuel_B.setText(palette_actuel_B);
        j_nb_palette_commande_A.setText("0");
        j_nb_palette_commande_B.setText("0");
        
    }
    
    private void j_nb_palette_actuel_BActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_j_nb_palette_actuel_BActionPerformed

    }//GEN-LAST:event_j_nb_palette_actuel_BActionPerformed

    /**
     * Vérifie si la commande est possible.
     */
    private void B_envoyer_commandeActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_B_envoyer_commandeActionPerformed
        // TODO add your handling code here:
        
        int nb_palette_commande_A = 0;
        int nb_palette_commande_B = 0;

        
        try {
            nb_palette_commande_A = Integer.parseInt(j_nb_palette_commande_A.getText());
            nb_palette_commande_B = Integer.parseInt(j_nb_palette_commande_B.getText());           
        }
        catch (NumberFormatException ex){
            app.error("Format numérique", "Entrez des valeurs valides pour les différentes paramètres.", ex);
        }

        if (nb_palette_commande_A < 0 && nb_palette_commande_B < 0)
        {
            app.error("Format numérique", "Veuillez entrer des nombres positifs.");
        }
            
        else if(nb_palette_commande_A > suivi.nb_palette_actuel_A)
        {
            app.error("Stock", "Pas assez de palettes A en stock, veuillez modifier votre commande ou attendre leur production.");
        }
        
        else if(nb_palette_commande_B > suivi.nb_palette_actuel_B)
        {
            app.error("Stock", "Pas assez de palettes B en stock, veuillez modifier votre commande ou attendre leur production.");
        }
        
        else
        {
            try {
                String commande = "1 " + nb_palette_commande_A + " " + nb_palette_commande_B;
                app.network.send_message(commande);
                
                //Envoie l'information pour mettre à jour le suivi de l'entrepôt
                suivi.commande(nb_palette_commande_A, nb_palette_commande_B);
                this.dispose();
                app.info("Commande réussie", "La commande a bien été envoyée au serveur.");
            } catch (IOException ex) {
                app.error("IO Exception", "Could not send the command to the host!");
            }
        }
        
    }//GEN-LAST:event_B_envoyer_commandeActionPerformed

    private void j_nb_palette_commande_BActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_j_nb_palette_commande_BActionPerformed
        
    }//GEN-LAST:event_j_nb_palette_commande_BActionPerformed

    private void B_annuler_commandeActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_B_annuler_commandeActionPerformed
        this.dispose();

    }//GEN-LAST:event_B_annuler_commandeActionPerformed

    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JButton B_annuler_commande;
    private javax.swing.JButton B_envoyer_commande;
    private javax.swing.JLabel jLabel1;
    private javax.swing.JLabel jLabel2;
    private javax.swing.JLabel jLabel3;
    private javax.swing.JLabel jLabel4;
    private javax.swing.JTextField j_nb_palette_actuel_A;
    private javax.swing.JTextField j_nb_palette_actuel_B;
    private javax.swing.JTextField j_nb_palette_commande_A;
    private javax.swing.JTextField j_nb_palette_commande_B;
    // End of variables declaration//GEN-END:variables

}
