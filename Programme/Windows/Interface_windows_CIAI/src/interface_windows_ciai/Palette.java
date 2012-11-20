/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package interface_windows_ciai;

import java.util.LinkedList;
import java.util.List;

/**
 *
 * @author michael
 */
public class Palette {
    int id_palette;
    List<Carton> listeCarton = new LinkedList<Carton>();
    int horaire;
    String type_palette;
    boolean disponible; //Pour les livraisons

    public Palette(int id_palette, String type_palette, int horaire) {
        this.id_palette = id_palette;
        this.type_palette = type_palette;
        this.horaire = horaire;
        this.disponible = true;
    }

    public void setDisponible(boolean disponible) {
        this.disponible = disponible;
    }

    public boolean isDisponible() {
        return disponible;
    }

    public List<Carton> getListeCarton() {
        return listeCarton;
    }

    public String getType_palette() {
        return type_palette;
    }
    
    
    
    public void Ajouter_carton(Carton carton){
        listeCarton.add(carton);
    }
    /*
     * Fonction ToSTring pour récuperer les différents paramètres sous forme d'une string
     * 
     */
    public String ToString(){
        String definition = "Palette " + this.id_palette + " de type " + this.type_palette + " finie à " + this.horaire;
        return definition;
    }
}
