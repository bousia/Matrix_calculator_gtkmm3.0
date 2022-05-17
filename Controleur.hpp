
/*    Controleur.hpp
 *  
 *    Gestion de la communication entre VueG et Modele
 *
 */

////////////////////////////////////////////////////////////////////
// IMPORTATION bibliotheques / headers                            //
////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include <regex>

#include "Modele.hpp"
#include "VueG.hpp"
#include "parametres.hpp"

////////////////////////////////////////////////////////////////////
// Classe CONTROLEUR                                              //
////////////////////////////////////////////////////////////////////

class Controleur {
private:  
  Modele *m;
  VueG *v;
public:
  Controleur(Modele *mm, VueG *vv) : m(mm), v(vv) {
    v->addPrecisionListener(this);
    v->addResizeListener(this);
    v->addModifListener(this);

    v->addRangListener(this);
    v->addTransListener(this);
    v->addInvListener(this);
    v->addPseudoInvListener(this);
    v->addAleaListener(this);
    v->addDetListener(this);

    v->addVpListener(this);
    v->addMoyListener(this);
    v->addDiscoListener(this);

    v->addResetListener(this);
    v->addAddListener(this);
    v->addDivListener(this);
    v->addRacListener(this);
    v->addSquareListener(this);
    v->addSetListener(this);
    v->addUndoListener(this);
    v->addRedoListener(this);

    v->addHelpListener(this);
    v->addMultListener(this);
    v->addSumDiagListener(this);
    v->addSumListener(this);
    v->addExitListener(this);
  }

  ////////////////////////////////////////////////////////////////////
  // Boutons - Actions                                              //
  ////////////////////////////////////////////////////////////////////

  void on_button_resize() {
    std::string xtext = v->getX(); v->setTabx(xtext);
    std::string ytext = v->getY(); v->setTaby(ytext);
    if (verif_int(xtext) && verif_int(ytext)) {
      int x = atoi(xtext.c_str());
      int y = atoi(ytext.c_str());
      if (x < TAILLE_MINI || TAILLE_MAX < x || y < TAILLE_MINI || TAILLE_MAX < y) {
        v->fenetre_avertissement("Attention", "Les dimensions de la matrice doivent être comprises entre " + std::to_string(TAILLE_MINI) + " et " + std::to_string(TAILLE_MAX));
      }
      else {
        m->resizeModele(x, y);
      }
    }
  }

  void on_button_precision() {
    std::string xtext = v->getX(); v->setTabx(xtext);
    if (verif_int(xtext)) {
      int x = atoi(xtext.c_str());
      if (x < PRECISION_MINI || PRECISION_MAX < x) {
        v->fenetre_avertissement("Attention", "Le nombre de chiffres après la virgule doit être compris entre " + std::to_string(PRECISION_MINI) + " et " + std::to_string(PRECISION_MAX));
      }
      else {
        m->precisionModele(x);
      }
    }
  }

  void on_button_modif() {
    Matrice *mat = new Matrice(m->get_line(), m->get_col(), 0);
    for (int i=0; i < m->get_line(); i++) {
      for (int j=0; j < m->get_col(); j++) {
        std::string vtext = v->getEntMat(i, j);
        if (verif_double_mat(vtext, i, j)) {
          double vd = atof(vtext.c_str());
          mat->set(i, j, vd);
        }
      }
    }
    m->modifModele(mat);
  }

  void on_button_det() {
    m->detModele();
  }

  void on_button_alea() {
    std::string xtext = v->getX(); v->setTabx(xtext);
    std::string ytext = v->getY(); v->setTaby(ytext);
    if (verif_int(xtext) && verif_int(ytext)) {
      int x = atoi(xtext.c_str());
      int y = atoi(ytext.c_str());
      if (x <= y) {
        m->aleaModele(x, y);
      }
      else {
        v->fenetre_avertissement("Attention", "Il faut que y soit plus grand que x pour pouvoir tirer un entier entre x et y");
      }
    }
  }

  void on_button_rang() {
    m->rangModele();
  }

  void on_button_trans() {
    m->transModele();
  }

  void on_button_inv() {
    m->invModele();
  }

  void on_button_pseudoinv() {
    m->pseudoinvModele();
  }

  void on_button_reset() {
    m->resetModele();
  }

  void on_button_add() {
    std::string vtext = v->getEntry(); v->setEntry(vtext);
    if (verif_double(vtext)) {
      double vd = atof(vtext.c_str());
      m->addModele(vd);
    }
  }

  void on_button_div() {
    std::string vtext = v->getEntry(); v->setEntry(vtext);
    if (verif_double(vtext)) {
      double vd = atof(vtext.c_str());
      if (vd != 0) {
        m->divModele(vd);
      }
      else {
        v->fenetre_avertissement("Attention", "Division par zéro impossible");
      }
    }
  }

  void on_button_square() {
    m->squareModele();
  }

  void on_button_rac() {
    m->racModele();
  }

  void on_button_set() {
    std::string vtext = v->getEntry(); v->setEntry(vtext);
    std::string xtext = v->getX(); v->setTabx(xtext);
    std::string ytext = v->getY(); v->setTaby(ytext);
    if (verif_double(vtext) && verif_int(xtext) && verif_int(ytext)) {
      double vd = atof(vtext.c_str());
      int x = atoi(xtext.c_str());
      int y = atoi(ytext.c_str());
      m->setModele(vd, x, y);
    }
  }

  void on_button_undo() {
    m->undoModele();
  }

  void on_button_redo() {
    m->redoModele();
  }

  void on_button_help() {
    v->on_button_help_fct();
  }

  void on_button_mult() {
    std::string vtext = v->getEntry(); v->setEntry(vtext);
    if (verif_double(vtext)) {
      double vd = atof(vtext.c_str());
      m->multModele(vd);
    }
  }

  void on_button_sumdiag() {
    m->sumdiagModele();
  }

  void on_button_sum() {
    m->sumModele();
  }

  void on_button_moy() {
    m->moyModele();
  }

  void on_button_disco() {
    switch(m->getDisco()) {
      case 0 : v->change_color_disco("#6CB7E0", "#2A8FC6"); m->incrDisco(); break;
      case 1 : v->change_color_disco("#1A587A", "#2A8FC6"); m->incrDisco(); break;
      case 2 : v->change_color_disco("#6CB7E0", "#1A587A"); m->incrDisco(); break;
      case 3 : v->change_color_disco("#FFFFFF", "#FFFFFF"); m->rstDisco(); break;
    }
  }

  void on_button_vp() {
    m->vpModele();
  }
  
  void on_button_exit() {
    v->on_button_close();
  }

  ////////////////////////////////////////////////////////////////////
  // Verification des entrées                                       //
  ////////////////////////////////////////////////////////////////////

  // fonctions de vérifications pour les nombres entiers et doubles sous forme de string

  bool verif_int(std::string str) {
    // test si entier
    if (!(std::regex_match(str, std::regex{"[+-]?[0-9]+"}))) {
      v->fenetre_avertissement("Attention", "Veuillez entrer un entier pour x et y");
      return false;
    }
    return true;
  }

  bool verif_double(std::string str) {
    // test si reel
    if (!(std::regex_match(str, std::regex{"[+-]?\\d*\\.?\\,?\\d+"}))) {
      v->fenetre_avertissement("Attention", "Veuillez entrer un réel pour v (séparateur autorisé : virgule ou point)");
      return false;
    }
    // test si nb chiffres après la virgule cohérent
    std::string s = "[+-]?\\d*\\.?\\,?\\d{0," + std::to_string(m->getPrecisionModele()) + "}";
    if (!(std::regex_match(str, std::regex{s}))) {
      v->fenetre_avertissement("Attention", "Veuillez entrer un réel avec un nombre de chiffres après la virgule cohérent pour v \nprécision actuelle : " + std::to_string(m->getPrecisionModele()) + " chiffres après la virgule");      
      return false;
    }
    return true;
  }

  bool verif_double_mat(std::string str, const int i, const int j) {
    // test si reel
    if (!(std::regex_match(str, std::regex{"[+-]?\\d*\\.?\\,?\\d+"}))) {
      v->fenetre_avertissement("Attention", "Veuillez entrer un réel à l'élément (" + std::to_string(i) + "," + std::to_string(j) + ") (séparateur autorisé : virgule ou point)");
      return false;
    }
    // test si nb chiffres après la virgule cohérent
    std::string s = "[+-]?\\d*\\.?\\,?\\d{0," + std::to_string(m->getPrecisionModele()) + "}";
    if (!(std::regex_match(str, std::regex{s}))) {
      v->fenetre_avertissement("Attention", "Veuillez entrer un réel avec un nombre de chiffres après la virgule cohérent pour l'élément (" + std::to_string(i) + "," + std::to_string(j) + ") \nprécision actuelle : " + std::to_string(m->getPrecisionModele()) + " chiffres après la virgule");      
      return false;
    }
    return true;
  }

};

