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

        //Variable ici car l'entrepôt est toujours consideré comme vide au début de l'application
    int MAXPAL = 100;
    int nb_palette = 1;
    Interface_windows_CIAI app;
    MessageReceiver message_receiver = new MessageReceiver(this);
    String[] liste_erreur = new String[7];
    
    /*Variable utilisée pour avoir un bon format dans les listes*/
    String[] liste_def_palette = new String[MAXPAL];
    String[] liste_def_carton = new String[MAXPAL];
    Palette liste_palette[] = new Palette[MAXPAL];
    Palette palette_initiale = new Palette(0, "A", 0);
    List<Carton> liste_carton = new LinkedList<Carton>();
    
    class MessageReceiver extends Thread {
            Suivi suivi;
            public MessageReceiver(Suivi suivi) {
                this.suivi = suivi;
            }
            
            public void run() {
                System.out.println("Starting message receiver...");
                String msg = "";
                liste_palette[0] = palette_initiale;
                int i = 0;
                while (true) {
                    try {
                        //ecoute d'un message du serveur
                        msg = suivi.app.network.listen_message();
                    } catch (IOException ex) {
                        //System.err.println("Could not establish server socket!");
                        app.error("Socket", "Could not establish client socket!", ex);
                    }
                    System.out.println("Received message: '"+msg+"'");
                    
                    if (msg.length() == 0)
                    {
                        app.error("Null message received!", "A null message was received. Stopping listening.");
                        throw new Error("Null message received!");
                    }
                    try {
                        if ("E".equals(msg.substring(0, 1)))
                        {

                                //Reception et traitement d'une erreur
                                String decoupe[] = msg.split(" ");
                                if (decoupe[1].equals("Fi")) {
                                    app.info("Quitting", "Received a stop message, quitting...");
                                    quit_app();
                                    return;
                                } else {
                                    int id_erreur = Integer.parseInt(decoupe[1]);
                                    int horaire = Integer.parseInt(decoupe[2]);
                                    
                                    switch(id_erreur){
                                        case 0: liste_erreur[id_erreur] = id_erreur + " : AU"; break;
                                        case 1: liste_erreur[id_erreur] = id_erreur + " : trop de pièces defectueuses"; break;
                                        case 2: liste_erreur[id_erreur] = id_erreur + " : plus de carton"; break;
                                        case 3: liste_erreur[id_erreur] = id_erreur + " : imprimante HS"; break;
                                        case 4: liste_erreur[id_erreur] = id_erreur + " : trop d ecartons dans la file d'attente"; break;
                                        case 5: liste_erreur[id_erreur] = id_erreur + " : plus de palette"; break;
                                        case 6: liste_erreur[id_erreur] = id_erreur + " : problème de conditionnement de palette"; break;    
                                    }                                    
                                    
                                    j_erreur.setListData(liste_erreur);
                                    B_reprise.setEnabled(true);
                                }
                        }
                        else if ("L C".equals(msg.substring(0, 3)))
                        {

                                //Reception d'un carton
                                String decoupe[] = msg.split(" ");
                                int id_carton = Integer.parseInt(decoupe[2]);
                                String type_piece = decoupe[3];
                                int pourcentage = Integer.parseInt(decoupe[4]);
                                int horaire = Integer.parseInt(decoupe[5]);

                                //ajout dans sa palette
                                Carton carton = new Carton(id_carton, type_piece, horaire, pourcentage);
                                liste_palette[i].Ajouter_carton(carton);

                        }
                        else if ("L P".equals(msg.substring(0, 3)))
                        {

                                //Reception d'une palette

                                i++; //mise à jour de l'indice de la liste de palette
                                nb_palette++; //mise à jour du nombre de palette

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
                    } catch (NumberFormatException ex) {
                        app.error("Invalid message received", "An invalid message was received: "+msg, ex);
                    } catch (ArrayIndexOutOfBoundsException ex) {
                        app.error("Invalid message received", "Error while parsing message: "+msg, ex);
                    }
                }
            }

            private void quit_app() {
                dispose();
                app.quit();
            }

        }
    
    /**
     * Creation d'une nouvelle forme Suivi
     */
    public Suivi(Interface_windows_CIAI inter) {
        app = inter;
        setLocationByPlatform(true);
        initComponents();
        for (int j = 0; j<7; j++)
        {
            liste_erreur[j] = "";
        }
        message_receiver.start();
        
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
        jScrollPane1 = new javax.swing.JScrollPane();
        j_palette = new javax.swing.JList();
        jScrollPane2 = new javax.swing.JScrollPane();
        j_carton = new javax.swing.JList();
        jScrollPane3 = new javax.swing.JScrollPane();
        j_erreur = new javax.swing.JList();

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
        B_reprise.setEnabled(false);
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

        j_erreur.setModel(new javax.swing.AbstractListModel() {
            String[] strings = { "Item 1", "Item 2", "Item 3", "Item 4", "Item 5" };
            public int getSize() { return strings.length; }
            public Object getElementAt(int i) { return strings[i]; }
        });
        jScrollPane3.setViewportView(j_erreur);

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
                                .addComponent(B_reprise)
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                                .addComponent(B_arret))
                            .addGroup(layout.createSequentialGroup()
                                .addGap(10, 10, 10)
                                .addComponent(jScrollPane1, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                                .addComponent(jScrollPane2, javax.swing.GroupLayout.PREFERRED_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.PREFERRED_SIZE)
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                                .addComponent(B_commande))
                            .addGroup(layout.createSequentialGroup()
                                .addComponent(jLabel2)
                                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.UNRELATED)
                                .addComponent(jScrollPane3, javax.swing.GroupLayout.PREFERRED_SIZE, 112, javax.swing.GroupLayout.PREFERRED_SIZE)
                                .addGap(0, 0, Short.MAX_VALUE)))))
                .addContainerGap())
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addContainerGap()
                .addComponent(jLabel1)
                .addGap(39, 39, 39)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING, false)
                    .addComponent(jLabel2, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addComponent(jScrollPane3, javax.swing.GroupLayout.PREFERRED_SIZE, 0, Short.MAX_VALUE))
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
        int nb_palette_actuel_A = 0;
        int nb_palette_actuel_B = 0;
        for(int j = 0; j < nb_palette; j++){  
            if("A".equals(liste_palette[j].getType_palette()))
            {
                nb_palette_actuel_A++;
            }
            else
            {
                nb_palette_actuel_B++;
            }           
        }
        Commande fc = new Commande(app, nb_palette_actuel_A, nb_palette_actuel_B);
        fc.setVisible(true);
    }//GEN-LAST:event_B_commandeActionPerformed

    /*
     * Envoi d'un message de reprise après une erreur.
     * 
     */
    private void B_repriseActionPerformed(java.awt.event.ActionEvent evt) {//GEN-FIRST:event_B_repriseActionPerformed
        System.out.println("Message de reprise");
        int erreur = j_erreur.getSelectedIndex();
        try {
            app.network.send_message("2 " + erreur);
            liste_erreur[erreur] = "";
            j_erreur.setListData(liste_erreur);
            boolean vide = true;
            for (int j = 0; j < 7; j++)
            {
                if (!"".equals(liste_erreur[j]))
                {
                    vide = false;
                }
            }
            if (vide == false){
                B_reprise.setEnabled(false);
            }  
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
            //this.dispose(); // attendre message de retour pour quitter !
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
        if (index_palette == -1)
        {
            //cas ou rien n'est sélectionné pour cause de mise à jour de la liste
        }
        else
        {
            
            liste_carton = liste_palette[index_palette].getListeCarton();
            for(int j = 0; j < liste_carton.size(); j++)
            {
                Carton carton = (Carton)liste_carton.get(j);
                liste_def_carton[j]=carton.ToString();
            }
            j_carton.setListData(liste_def_carton);
        }
        
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
    private javax.swing.JScrollPane jScrollPane3;
    private javax.swing.JList j_carton;
    private javax.swing.JList j_erreur;
    private javax.swing.JList j_palette;
    // End of variables declaration//GEN-END:variables
}
