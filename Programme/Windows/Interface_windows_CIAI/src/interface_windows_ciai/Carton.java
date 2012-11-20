/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package interface_windows_ciai;

/**
 *
 * @author michael
 */
public class Carton {
    int id_carton;
    String type_piece;
    int horaire;
    int pourcentage; //de pièces défectueuses durant la réalisation du carton

    public Carton(int id_carton, String type_piece, int horaire, int pourcentage) {
        this.id_carton = id_carton;
        this.type_piece = type_piece;
        this.horaire = horaire;
        this.pourcentage = pourcentage;
    }
    /*
     * Fonction ToString pour récuperer les différents paramètres sous forme d'une string
     * 
     */    
    public String ToString(){
        String definition = "Carton "+ this.id_carton + ", type " + this.type_piece+ 
                " fini à " + this.horaire + ", pièces defectueuses: " + this.pourcentage+"%";
        return definition;
    }

}
