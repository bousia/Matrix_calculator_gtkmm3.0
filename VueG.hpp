
/*    VueG.hpp
 *  
 *    Création et gestion de la fenetre d'affichage
 *
 */

////////////////////////////////////////////////////////////////////
// IMPORTATION bibliotheques / headers                            //
////////////////////////////////////////////////////////////////////

#pragma once

#include <gtkmm/button.h>
#include <gtkmm/togglebutton.h>
#include <gtkmm/window.h>
#include <gtkmm/box.h>
#include <gtkmm/entry.h>
#include <gtkmm/table.h>
#include <gtkmm/textview.h>
#include <gtkmm/messagedialog.h>
#include <gtkmm/image.h>
//#include <gtkmm.h>

#include <cassert>
#include <iostream>

#include "Observer.hpp"
#include "parametres.hpp"

////////////////////////////////////////////////////////////////////
// Classe VueG                                                    //
////////////////////////////////////////////////////////////////////

class Controleur;

class VueG : public Gtk::Window, public Observer<double> {

private:

  Gtk::Box box_globale_v;
  Gtk::Box box_matrice_v;
  Gtk::Box box_size_modif_h;
  Gtk::Box box_menu_v;
  Gtk::Box box_rp_h;
  Gtk::Box box_entry_h;
  Gtk::Box box_bouton_h;
  Gtk::Box box_b1_v;
  Gtk::Box box_b2_v;
  Gtk::Box box_b3_v;
  Gtk::Label sizeText;
  Gtk::Label entryText;
  Gtk::Entry entry;
  Gtk::Label xText;
  Gtk::Entry tabX;
  Gtk::Label yText;
  Gtk::Entry tabY;
  Gtk::Label resultText;
  Gtk::Button bResize, bPrecision;
  Gtk::Button bModif, bVp, bMoy;
  Gtk::Button bRang, bInv, bTrans, bPseudoInv, bDet, bAlea;
  Gtk::Button bDiv, bRac, bSquare;
  Gtk::Button bExit, bReset, bAdd, bSet, bUndo, bRedo;
  Gtk::Button bHelp, bMult, bSumDiag, bSum, bQuit;
  Gtk::Button bDisco;
  Gtk::Table tabMat;
  Gtk::Entry entMat[TAILLE_MAX][TAILLE_MAX];

  // fenetre d'Avertissement
  std::unique_ptr<Gtk::MessageDialog> messDial;
  
public:
  VueG() :
    box_globale_v(Gtk::ORIENTATION_VERTICAL),
    box_matrice_v(Gtk::ORIENTATION_VERTICAL),
    box_size_modif_h(Gtk::ORIENTATION_HORIZONTAL),
    box_menu_v(Gtk::ORIENTATION_VERTICAL),
    box_rp_h(Gtk::ORIENTATION_HORIZONTAL),
    box_entry_h(Gtk::ORIENTATION_HORIZONTAL),
    box_bouton_h(Gtk::ORIENTATION_HORIZONTAL),
    box_b1_v(Gtk::ORIENTATION_VERTICAL),
    box_b2_v(Gtk::ORIENTATION_VERTICAL),
    box_b3_v(Gtk::ORIENTATION_VERTICAL),
    entryText(" v : "),
    xText(" x : "),
    yText(" y : "),
    resultText(" Result : ")
  {

    /////////////////////////////
    // BOUTONS (ICONE + TEXTE) //
    /////////////////////////////

    bModif.add_pixlabel("img36/modif.png", "Enter Values");           // affecte les valeurs entrées par l'utilisateur
    bResize.add_pixlabel("img36/resize.png", "resize (x,y)");         // redimensionne la matrice pour une nouvelle taille (x,y)
    bPrecision.add_pixlabel("img36/precision.png", "precision (x)");  // definit le nb de chiffres après la virgule
    bAdd.add_pixlabel("img36/add.png", "add (v)");                    // ajoute v à tous les éléments de la matrice
    bMult.add_pixlabel("img36/mult.png", "mult (v)");                 // multiplie par v tous les éléments de la matrice
    bDiv.add_pixlabel("img36/div.png", "div (v)");                    // divise par v tous les éléments de la matrice
    bSquare.add_pixlabel("img36/square.png", "square");               // met au carré tous les éléments de la matrice
    bRac.add_pixlabel("img36/rac.png", "rac");                        // met à la racine carré tous les éléments de la matrice
    bSum.add_pixlabel("img36/sum.png", "sum");                        // calcule la somme de tous les éléments de la matrice
    bSumDiag.add_pixlabel("img36/sumdiag.png", "sum diag");           // calcule la somme de la diagonale principale
    bDet.add_pixlabel("img36/det.png", "det");                        // calcule le déterminant
    bRang.add_pixlabel("img36/rang.png", "rank");                     // calcule le rang 
    bInv.add_pixlabel("img36/inv.png", "inv");                        // calcule la matrice inversée
    bPseudoInv.add_pixlabel("img36/pseudoinv.png", "pseudo-inv");     // calcule la matrice pseudo-inversée
    bTrans.add_pixlabel("img36/trans.png", "trans");                  // calcule la transposée
    bAlea.add_pixlabel("img36/alea.png", "alea (x,y)");               // remplit la matrice d'entiers tirés aléatoirement entre x et y
    bReset.add_pixlabel("img36/reset.png", "reset");                  // mets tous les éléments de la matrice à 0
    bUndo.add_pixlabel("img36/undo.png", "undo");                     // retour en arrière (undo)
    bRedo.add_pixlabel("img36/redo.png", "redo");                     // annulation retour en arrière (redo)
    bVp.add_pixlabel("img36/vp.png", "eigen val");                    // calcule les valeurs propres de la matrice
    bMoy.add_pixlabel("img36/moy.png", "mean");                       // calcule la moyenne de tous les éléments de la matrice
    bHelp.add_pixlabel("img36/help.png", "help");                     // affiche une fenetre d'explication
    bExit.add_pixlabel("img36/exit.png", "exit");                     // sortir de l'application
    bDisco.add_pixlabel("img36/disco.png", "disco");                  // permet de changer les couleurs de la matrice

    bSet.add_pixlabel("img36/set.png", "set (v) (x,y)");              // affecte l'élément (x,y) avec la valeur v (non utilisé)

    //////////////////////////////////
    // CONTRAINTES / INITIALISATION //
    //////////////////////////////////

    // titre de l'application
    set_title("Matrice");

    // création tableau pour affichage matrice
    for(int i=0; i<TAILLE_INIT; i++) {
      for(int j=0; j<TAILLE_INIT; j++) {
        entMat[i][j].set_alignment(0.5);
        entMat[i][j].set_max_length(20);
        entMat[i][j].set_text("0.00");
        tabMat.attach(entMat[i][j], i, i+1, j, j+1);
      }
    }

    // afficher taille de la matrice selon paramètres
    this->setSize(TAILLE_INIT, TAILLE_INIT);

    // contraintes box
    box_entry_h.set_spacing(5);
    box_globale_v.set_spacing(15);
    box_b1_v.set_spacing(5);
    box_b2_v.set_spacing(5);
    box_b3_v.set_spacing(5);
    box_bouton_h.set_spacing(5);
    box_menu_v.set_spacing(5);
    box_matrice_v.set_spacing(2);
    box_rp_h.set_spacing(5);

    box_bouton_h.set_margin_right(2);
    box_bouton_h.set_margin_left(2);
    box_rp_h.set_margin_right(2);
    box_rp_h.set_margin_left(2);
    box_size_modif_h.set_margin_right(2);
    box_size_modif_h.set_margin_left(2);
    tabMat.set_margin_right(2);
    tabMat.set_margin_left(2);


    // Entree
    entry.set_max_length(15);
    entry.set_text("0,00");
    entry.select_region(0, entry.get_text_length());
    entry.set_margin_top(5);
    entry.set_margin_bottom(5);

    tabX.set_text("0");
    tabX.set_max_length(10);
    tabX.set_margin_top(5);
    tabX.set_margin_bottom(5);

    tabY.set_text("0");
    tabY.set_max_length(10);
    tabY.set_margin_top(5);
    tabY.set_margin_bottom(5);
    tabY.set_margin_right(5);

    // sortie
    resultText.set_size_request(-1,50);

    // Police d'écriture
    sizeText.override_font(Pango::FontDescription("sans bold 10."));
    entryText.override_font(Pango::FontDescription("sans bold 16"));
    xText.override_font(Pango::FontDescription("sans bold 16"));
    yText.override_font(Pango::FontDescription("sans bold 16"));
    entry.override_font(Pango::FontDescription("sans 10"));
    tabX.override_font(Pango::FontDescription("sans 10"));
    tabY.override_font(Pango::FontDescription("sans 10"));
    resultText.override_font(Pango::FontDescription("sans bold 16"));

    // couleur en fond
    resultText.override_background_color(Gdk::RGBA("#428CAD"));    // bleu doux
    box_entry_h.override_background_color(Gdk::RGBA("#428CAD"));

    /////////////////////////////
    // BOX GLOBALE             //
    /////////////////////////////

    // box matrice et box commande
    box_globale_v.pack_start(box_matrice_v);
    box_globale_v.pack_start(box_menu_v);

    /////////////////////////////
    // BOX MATRICE             //
    /////////////////////////////

    // box matrice et infos matrice
    box_matrice_v.pack_start(tabMat);
    box_matrice_v.pack_start(box_size_modif_h);

    // box info taille matrice et bouton validation de saisie
    box_size_modif_h.pack_start(sizeText);
    box_size_modif_h.pack_start(bModif); 

    /////////////////////////////
    // BOX COMMANDE            //
    /////////////////////////////

    // sortie
    resultText.set_alignment(0);

    // box de commande
    box_menu_v.pack_start(box_rp_h);    
    box_menu_v.pack_start(box_entry_h);
    box_menu_v.pack_start(box_bouton_h);
    box_menu_v.pack_start(resultText); 

    // box pour les boutons resize et precision
    box_rp_h.pack_start(bResize);
    box_rp_h.pack_start(bPrecision);

    // box pour entrée saisie
    box_entry_h.pack_start(entryText);
    box_entry_h.pack_start(entry);
    box_entry_h.pack_start(xText);
    box_entry_h.pack_start(tabX);
    box_entry_h.pack_start(yText);
    box_entry_h.pack_start(tabY);

    // Box pour les 3 colonnes de boutons
    box_bouton_h.pack_start(box_b1_v);
    box_bouton_h.pack_start(box_b2_v);
    box_bouton_h.pack_start(box_b3_v);

    // boutons dans la boite verticale 1    
    //box_b1_v.pack_start(bSet);
    box_b1_v.pack_start(bAdd);
    box_b1_v.pack_start(bSquare);
    box_b1_v.pack_start(bAlea);
    box_b1_v.pack_start(bDet);
    box_b1_v.pack_start(bInv);
    box_b1_v.pack_start(bUndo);
    box_b1_v.pack_start(bReset);

    // boutons dans la boite verticale 2
    box_b2_v.pack_start(bMult);
    box_b2_v.pack_start(bRac);
    box_b2_v.pack_start(bMoy);
    box_b2_v.pack_start(bRang);
    box_b2_v.pack_start(bPseudoInv);
    box_b2_v.pack_start(bRedo);
    box_b2_v.pack_start(bHelp);

    // boutons dans la boite verticale 3
    box_b3_v.pack_start(bDiv);
    box_b3_v.pack_start(bSum);
    box_b3_v.pack_start(bSumDiag);
    box_b3_v.pack_start(bVp);
    box_b3_v.pack_start(bTrans);
    box_b3_v.pack_start(bDisco);
    box_b3_v.pack_start(bExit);

    /////////////////////////////
    // AFFICHAGE               //
    /////////////////////////////
    
    // box finale
    add(box_globale_v);
    
    // affichage
    show_all_children();

  }
  
  virtual ~VueG();

  // méthode get pour les entrées
  std::string getX() const;
  std::string getY() const;
  std::string getEntry() const;
  std::string getEntMat(const int, const int) const;
  
  // méthode set pour les entrées
  void setEntry(const std::string);
  void setTabx(const std::string);
  void setTaby(const std::string);
  void setEntMat(const std::string text, const int i, const int j); 
  
  // affichage du résultat
  void setResult(const double d, const int precision);
  void setResult(const std::string txt);
  void resetResult();
  
  // affichage de la matrice
  void resize(const int tabx, const int taby, const int i, const int j);
  void setSize(const int x, const int y);
  void setElemTab(const double v, const int i, const int j, const int precision);
  
  // fonctions ré-écrites en provenance de l'observer
  void updateResult(const double &d, const int &precision) override;
  void updateResult(const std::vector<double> &vp, const int &precision) override;
  void updateMat(const double &d, const int &i, const int &j, const int &precision) override;
  void updateResize(const int &tabx, const int &taby, const int &i, const int &j) override;
  void updateWarning(const std::string &titre, const std::string &message) override;
  
  // gestion des événements
  void addResizeListener(Controleur *c);
  void addPrecisionListener(Controleur *c);
  void addModifListener(Controleur *c);
  void addResetListener(Controleur *c);
  void addInvListener(Controleur *c);
  void addPseudoInvListener(Controleur *c);
  void addTransListener(Controleur *c);
  void addRangListener(Controleur *c);
  void addAleaListener(Controleur *c);
  void addDetListener(Controleur *c);
  void addAddListener(Controleur *c);
  void addDivListener(Controleur *c);
  void addRacListener(Controleur *c);
  void addSquareListener(Controleur *c);
  void addSetListener(Controleur *c);
  void addUndoListener(Controleur *c);
  void addRedoListener(Controleur *c);
  void addHelpListener(Controleur *c);
  void addMultListener(Controleur *c);
  void addSumDiagListener(Controleur *c);
  void addSumListener(Controleur *c);
  void addVpListener(Controleur *c);
  void addMoyListener(Controleur *c);
  void addExitListener(Controleur *c);
  void addDiscoListener(Controleur *c);
  void on_button_help_fct();
  void on_button_close();

  // fenetre supplémentaire
  void fenetre_avertissement(std::string, std::string);

  // couleurs matrice
  void change_color_disco(const std::string color1, const std::string color2);

};

