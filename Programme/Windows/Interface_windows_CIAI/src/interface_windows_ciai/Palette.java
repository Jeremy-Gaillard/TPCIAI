/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package interface_windows_ciai;

import java.util.LinkedList;
import java.util.List;
import vue.Suivi;

/**
 *
 * @author michael
 */
public class Palette {
    
    public static String get_unique_id_for(int id_palette, String type_palette) {
        return id_palette+type_palette;
    }
    
    //int id_palette;
    Integer id_palette = null;
    String horaire = null;
    List<Carton> listeCarton = new LinkedList<>();
    String type_palette;
    boolean disponible; //Pour les livraisons
    boolean remplie;
    
    //public Palette(int id_palette, String type_palette, int horaire) {
    public Palette(int id_palette, String type_palette) {
        this.id_palette = id_palette;
        this.type_palette = type_palette;
        //this.horaire = horaire;
        this.disponible = true;
        this.remplie = false;
    }
    /*
    public void set_remplie(boolean remplie) {
        this.remplie = remplie;
    }
    */
    public void finaliser(String horaire) {
        //id_palette = id;
        remplie = true;
        this.horaire = horaire;
    }
    
    public boolean is_remplie() {
        return remplie;
    }
    
    public void set_disponible(boolean disponible) {
        this.disponible = disponible;
    }

    public boolean is_disponible() {
        return disponible;
    }

    public List<Carton> getListeCarton() {
        return listeCarton;
    }

    public String get_type_palette() {
        return type_palette;
    }
    
    public String get_unique_id() {
        //return id_palette+type_palette;
        return get_unique_id_for(id_palette, type_palette);
    }
    
    
    public void ajouter_carton(Carton carton){
        listeCarton.add(carton);
    }
    /*
     * Fonction ToSTring pour récuperer les différents paramètres sous forme d'une string
     * 
     */
    @Override
    public String toString() {
        String flag = "";
        if (horaire == null) flag = "[En remplissage] ";
        else if(!is_disponible()) flag = "[Envoyée] ";
        return flag
                + "Palette " + id_palette + " de type " + type_palette
                + (horaire == null?"":", finie à " + Suivi.horaire_format(horaire));
    }
}
