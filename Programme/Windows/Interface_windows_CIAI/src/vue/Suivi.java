/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package vue;

import interface_windows_ciai.Carton;
import interface_windows_ciai.Interface_windows_CIAI;
import interface_windows_ciai.Palette;
import java.io.IOException;
import java.util.LinkedList;
import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 *
 * @author michael
 */
public class Suivi extends javax.swing.JFrame {
    
    class MessageReceiver extends Thread {
            Suivi suivi;
            public MessageReceiver(Suivi suivi) {
                this.suivi = suivi;
            }
            
            public void run() {
                System.out.println("Starting message receiver...");
                String msg = "";
                int i = 0;
                while (true) {
                    try {
                        msg = suivi.app.network.listen_messages();
                    } catch (IOException ex) {
                        System.err.println("Could not establish server socket!");
                    }
                    
                    if ("".equals(msg))
                    {

                    }
                    else if ("E".equals(msg.substring(0, 1))){
                            //Reception et traitement d'une erreur
                            j_erreur.setText("erreur détectée");
                    }
                    else if ("L C".equals(msg.substring(0, 3))){
                            //Reception d'un carton et ajout dans sa palette

                            String decoupe[] = msg.split(" ");
                            int id_carton = Integer.parseInt(decoupe[2]);
                            String type_piece = decoupe[3];
                            int pourcentage = Integer.parseInt(decoupe[4]);
                            int horaire = Integer.parseInt(decoupe[5]);

                            Carton carton = new Carton(id_carton, type_piece, pourcentage, horaire);
                            liste_palette[i].Ajouter_carton(carton);

                    }
                    else if ("L P".equals(msg.substring(0, 3))){
                            i++;
                            //Reception d'une palette

                            String decoupe[] = msg.split(" ");
                            int id_palette = Integer.parseInt(decoupe[2]);
                            String type_palette = decoupe[3];
                            int horaire = Integer.parseInt(decoupe[4]); 

                            Palette palette = new Palette(id_palette, type_palette, horaire);
                            liste_palette[i] = palette;

                            //MAJ de la liste de palettes
                            liste_def_palette[i] = palette.ToString();
                            j_palette.setListData(liste_def_palette);
                    }
                }
            }

        }
    
    //Variable ici car l'entrepôt est toujours consideré comme vide au début de l'application
    int MAXPAL = 100;
    Interface_windows_CIAI app;
    /*Utilisé pour avoir un bon format dans les listes*/
    String[] liste_def_palette;
    String[] liste_def_carton;
    Palette[] liste_palette;
    List liste_carton = new LinkedList();
    /**
     * Creation d'une nouvelle forme Suivi
     */
    public Suivi(Interface_windows_CIAI inter) {
        app = inter;
        setLocationByPlatform(true);
        initComponents();
        new MessageReceiver(this).start();
        
        /*
        int i = 0;
        Palette test_palette = new Palette(1, "A", 5);
        String msg ="";
        liste_def_palette = new String[MAXPAL];
        liste_def_palette[i] = test_palette.ToString();
        j_palette.setListData(liste_def_palette);
        for(;;){
            // Boucle infinie pour écouter les messages provenant de Linux
            try {
                msg = app.network.listen_messages();
            } catch (IOException ex) {
                Logger.getLogger(Suivi.class.getName()).log(Level.SEVERE, null, ex);
            }
            if ("".equals(msg))
            {
                
            }
            else if ("E".equals(msg.substring(0, 1))){
                //Reception et traitement d'une erreur
                j_erreur.setText("erreur détectée");
            }
            else if ("L C".equals(msg.substring(0, 3))){
                //Reception d'un carton et ajout dans sa palette
                
                String decoupe[] = msg.split(" ");
                int id_carton = Integer.parseInt(decoupe[2]);
                String type_piece = decoupe[3];
                int pourcentage = Integer.parseInt(decoupe[4]);
                int horaire = Integer.parseInt(decoupe[5]);
                
                Carton carton = new Carton(id_carton, type_piece, pourcentage, horaire);
                liste_palette[i].Ajouter_carton(carton);
                
            }
            else if ("L P".equals(msg.substring(0, 3))){
                i++;
                //Reception d'une palette
                
                String decoupe[] = msg.split(" ");
                int id_palette = Integer.parseInt(decoupe[2]);
                String type_palette = decoupe[3];
                int horaire = Integer.parseInt(decoupe[4]); 
                
                Palette palette = new Palette(id_palette, type_palette, horaire);
                liste_palette[i] = palette;
                
                //MAJ de la liste de palettes
                liste_def_palette[i] = test_palette.ToString();
                j_palette.setListData(liste_def_palette);
            }
        }*/
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
        B_commande = new javax.swing.JButton();
        B_reprise = new javax.swing.JButton();
        B_arret = new javax.swing.JButton();
        j_erreur = new javax.swing.JLabel();
        jScrollPane1 = new javax.swing.JScrollPane();
        j_palette = new javax.swing.JList();
        jScrollPane2 = new javax.swing.JScrollPane();
        j_carton = new javax.swing.JList();

        setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);

        jLabel1.setText("Interface de suivi du lot");

        jLabel2.setText("Erreur detectées :");

        B_commande.setText("Commande");
        B_commande.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                B_commandeActionPerformed(evt);
            }
        });

        B_reprise.setText("Reprise");
        B_reprise.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                B_repriseActionPerformed(evt);
            }
        });

        B_arret.setText("Arrêt");
        B_arret.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(java.awt.event.ActionEvent evt) {
                B_arretActionPerformed(evt);
            }
        });

        j_erreur.setText("Pas d'erreur detectée");

        j_palette.setSelectionMode(javax.swing.ListSelectionModel.SINGLE_SELECTION);
        j_palette.addListSelectionListener(new javax.swing.event.ListSelectionListener() {
            public void valueChanged(javax.swing.event.ListSelectionEvent evt) {
                j_paletteValueChanged(evt);
            }
        });
        jScrollPane1.setViewportView(j_palette);

        j_carton.setModel(new javax.swing.AbstractListModel() {
            String[] strings = { "Pas de carton selectionné" };
            public int getSize() { return strings.length; }
            public Object getElementAt(int i) { return strings[i]; }
        });
        jScrollPane2.setViewportView(j_carton);

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(layout.createSequentialGroup()
                        .addGap(137, 137, 137)
                        .addComponent(jLabel1)
                        .addGap(0, 248, Short.MAX_VALUE))
                    .addGroup(layout.createSequentialGroup()
                        .addGap(42, 42, 42)
                        .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                            .addGroup(layout.createSequentialGroup()
                                .addGap(10, 10, 10)
                                .addComponent(jScrollPane1, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                .addComponent(jScrollPane2, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                                .addComponent(B_commande))
                            .addGroup(layout.createSequentialGroup()
                                .addComponent(B_reprise)
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                                .addComponent(B_arret))
                            .addGroup(layout.createSequentialGroup()
                                .addComponent(jLabel2)
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                                .addComponent(j_erreur, javax.swing.GroupLayout.PREFERRED_SIZE, 281, javax.swing.GroupLayout.PREFERRED_SIZE)
                                .addGap(0, 0, Short.MAX_VALUE)))))
                .addContainerGap())
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addContainerGap()
                .addComponent(jLabel1)
                .addGap(39, 39, 39)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(jLabel2)
                    .addComponent(j_erreur))
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addGroup(layout.createSequentialGroup()
                        .addGap(64, 64, 64)
                        .addComponent(B_commande))
                    .addGroup(layout.createSequentialGroup()
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                        .addComponent(jScrollPane1, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE))
                    .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, layout.createSequentialGroup()
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(jScrollPane2, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)))
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, 31, Short.MAX_VALUE)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.BASELINE)
                    .addComponent(B_reprise)
                    .addComponent(B_arret))
                .addGap(27, 27, 27))
        );

        pack();
    }// </editor-fold>//GEN-END:initComponents
    
    /*
     * ouverure de la fenêtre commande
     * 
     */
    private void B_commandeActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_B_commandeActionPerformed
        // Envoi d'une commande
        System.out.println("Commande");
        Commande fc = new Commande(app, liste_palette);
        fc.setVisible(true);
    }//GEN-LAST:event_B_commandeActionPerformed

    /*
     * Envoi d'un message de reprise après une erreur.
     * 
     */
    private void B_repriseActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_B_repriseActionPerformed
        System.out.println("Message de reprise");
        try {
            app.network.send_message("2"); //Ajouter ici l'id de l'erreur
            //app.network.send_message(null);
        } catch (IOException ex) {
            app.error("IO Exception", "Could not send the command to the host!");
            ex.printStackTrace(System.err);
        }
    }//GEN-LAST:event_B_repriseActionPerformed
    
    /*
     * Envoi d'un message d'arrêt de production après une erreur.
     * 
     */
    private void B_arretActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_B_arretActionPerformed
        System.out.println("Message d'arrêt");
        try {
            app.network.send_message("3");
        } catch (IOException ex) {
            app.error("IO Exception", "Could not send the command to the host!");
            ex.printStackTrace(System.err);
        }        
    }//GEN-LAST:event_B_arretActionPerformed
    /*
     * Selection d'une palette dans la liste
     * 
     */
    private void j_paletteValueChanged(javax.swing.event.ListSelectionEvent evt) {//GEN-FIRST:event_j_paletteValueChanged
        
        int index_palette = j_palette.getSelectedIndex();
        liste_carton = liste_palette[index_palette].getListeCarton();
        for(int j = 0; j < liste_carton.size(); j++){
            
            liste_def_carton[j]=liste_carton.toString();
            
        }
        j_carton.setListData(liste_def_carton);
        
    }//GEN-LAST:event_j_paletteValueChanged

    /*
    public static void main(String args[]) {
        
        java.awt.EventQueue.invokeLater(new Runnable() {

            public void run() {
                new Suivi().setVisible(true);
            }
        });
    }*/
    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JButton B_arret;
    private javax.swing.JButton B_commande;
    private javax.swing.JButton B_reprise;
    private javax.swing.JLabel jLabel1;
    private javax.swing.JLabel jLabel2;
    private javax.swing.JScrollPane jScrollPane1;
    private javax.swing.JScrollPane jScrollPane2;
    private javax.swing.JList j_carton;
    private javax.swing.JLabel j_erreur;
    private javax.swing.JList j_palette;
    // End of variables declaration//GEN-END:variables
}
