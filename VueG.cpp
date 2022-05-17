
/*
 * fichier VueG.cpp
 *
 * Definit les méthodes de la classe VueG (cf VueG.hpp)
 *
 */

////////////////////////////////////////////////////////////////////
// IMPORTATION bibliotheques / headers                            //
////////////////////////////////////////////////////////////////////

#include <iomanip>

#include "VueG.hpp"
#include "Controleur.hpp"
#include "Modele.hpp"

////////////////////////////////////////////////////////////////////
// Gestion des entrées                                            //
////////////////////////////////////////////////////////////////////

// fonction de suppression des caractères isspace()

std::string ltrim(const std::string &s) {
  size_t start = s.find_first_not_of(" \n\r\t\f\v");
  return (start == std::string::npos) ? "" : s.substr(start);
}
 
std::string rtrim(const std::string &s) {
  size_t end = s.find_last_not_of(" \n\r\t\f\v");
  return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

std::string trim(const std::string &s) {
  return rtrim(ltrim(s));
}

// lecture des valeurs d'entrée

std::string VueG::getX() const {
  std::string text = this->tabX.get_text();
  return trim(text);
}

std::string VueG::getY() const {
  std::string text = this->tabY.get_text();
  return trim(text);
}

std::string VueG::getEntry() const {
  std::string text = this->entry.get_text();
  std::replace(text.begin(), text.end(), '.', ','); // acceptation virgule et point
  return trim(text);
}

std::string VueG::getEntMat(const int i, const int j) const {
  std::string text = this->entMat[i][j].get_text();
  std::replace(text.begin(), text.end(), '.', ','); // acceptation virgule et point
  return trim(text);
}

// modification des entrées

void VueG::setEntry(const std::string text) {
  this->entry.set_text(text);
}
void VueG::setTabx(const std::string text) {
  this->tabX.set_text(text);
}
void VueG::setTaby(const std::string text) {
  this->tabY.set_text(text);
}
void VueG::setEntMat(const std::string text, const int i, const int j) {
  this->entMat[i][j].set_text(text);
}

// valeur v et position (i,j)
void VueG::setElemTab(const double v, const int i, const int j, const int precision) {
  Glib::ustring text = Glib::ustring::format(std::fixed, std::setprecision(precision), v);
  this->entMat[i][j].set_text(text);
  this->resetResult();
}

////////////////////////////////////////////////////////////////////
// Gestion de l'affichage                                         //
////////////////////////////////////////////////////////////////////

// affichage des résultats

void VueG::setResult(const double d, const int precision) {
  Glib::ustring text0 = Glib::ustring::format(std::fixed, " Result : ");
  Glib::ustring text1 = Glib::ustring::format(std::fixed, std::setprecision(precision), d);
  Glib::ustring text = text0 + text1;
  this->resultText.set_text(text);
}

void VueG::setResult(const std::string txt) {
  Glib::ustring text0 = Glib::ustring::format(std::fixed, " Result : ");
  Glib::ustring text = text0 + txt;
  this->resultText.set_text(text);
}

void VueG::resetResult() {
  this->resultText.set_text(Glib::ustring::format(std::fixed, " Result : "));
}

// tabx, taby : taille max atteint par la table (affichage)
// x, y : nouvelle taille
void VueG::resize(const int tabx, const int taby, const int x, const int y) {
  int tabmax = tabx;
  if (tabx<taby) 
    tabmax = taby;
  int max = x;
  if (x<y) 
    max = y;
  for(int i=0; i<max; i++) {
    for(int j=0; j<max; j++) {
      if (tabmax<=i || tabmax<=j) {
        this->entMat[i][j].set_alignment(0.5);
        this->entMat[i][j].set_max_length(20);
        this->tabMat.attach(entMat[i][j], i, i+1, j, j+1);
      }
    }
  }
  for(int i=0; i<tabx || i<x; i++) {
    for(int j=0; j<taby || j<y; j++) {
      this->entMat[i][j].hide();
    }
  }
  for(int i=0; i<x; i++) {
    for(int j=0; j<y; j++) {
      this->entMat[i][j].show();
    }
  }
}

// affichage taille de la matrice
void VueG::setSize(const int x, const int y) {
  Glib::ustring text0 = Glib::ustring::format(std::fixed, "Matrice ");
  Glib::ustring text1 = Glib::ustring::format(std::fixed, x);
  Glib::ustring text2 = Glib::ustring::format(std::fixed, "x");
  Glib::ustring text3 = Glib::ustring::format(std::fixed, y);
  Glib::ustring text = text0 + text1 + text2 + text3;
  this->sizeText.set_text(text);
}

////////////////////////////////////////////////////////////////////
// Réécriture des méthodes abstraite provenant de l'Observer      //
////////////////////////////////////////////////////////////////////

// affiche le résultat à partir d'un double d et du nombre de chiffres après la virgule
void VueG::updateResult(const double &d, const int &precision) {
  this->setResult(d, precision);
}

// affiche le résultat à partir d'une liste de double vp et du nombre de chiffres après la virgule
void VueG::updateResult(const std::vector<double> &vp, const int &precision) {
  std::string txt;
  for (auto& i : vp) {
    txt += Glib::ustring::format(std::fixed, std::setprecision(precision), i);
    txt += " ";
  }
  this->setResult(txt);
}

// affiche un nouvel élément d à la position (i,j) de la matrice provenant du modele
void VueG::updateMat(const double &d, const int &i, const int &j, const int &precision) {
  this->setElemTab(d, i, j, precision);
  this->resetResult();
}

// affiche le changement de taille 
// en entrée : taille max atteinte à l'affichage (tabx et taby) et la taille voulue ( i et j)
void VueG::updateResize(const int &tabx, const int &taby, const int &i, const int &j) {
  this->resize(tabx, taby, i, j);
  this->setSize(i, j);
}

// affichage une fenetre d'avertissement avec un titre et un message
void VueG::updateWarning(const std::string &titre, const std::string &message) {
  this->fenetre_avertissement(titre, message);
}

VueG::~VueG() {}

////////////////////////////////////////////////////////////////////
// Fenetre supplementaire                                         //
////////////////////////////////////////////////////////////////////

void VueG::fenetre_avertissement(std::string titre, std::string mess) {
  this->messDial.reset(new Gtk::MessageDialog(*this, titre));
  this->messDial->set_secondary_text(mess);
  this->messDial->set_modal(true);
  //this->messDial->set_hide_on_close(true);
  this->messDial->signal_response().connect(sigc::hide(sigc::mem_fun(*(this->messDial), &Gtk::Widget::hide)));
  this->messDial->show();
}

////////////////////////////////////////////////////////////////////
// Gestion des actions                                            //
////////////////////////////////////////////////////////////////////

void VueG::on_button_close() {
  hide();
  //gtk_main_quit();
}

// fenetre explicative
void VueG::on_button_help_fct() {
  std::string helpTxt;
  helpTxt += " • Enter Values : affecte les valeurs entrées par l'utilisateur \n";
  helpTxt += " • resize (x,y) : redimensionne la matrice pour une nouvelle taille (x,y) \n";
  helpTxt += " • precision (x) : définit le nombre de chiffres après la virgule \n";
  helpTxt += " • add (v) : ajoute v à tous les éléments de la matrice \n";
  helpTxt += " • mult (v) : multiplie par v tous les éléments de la matrice \n";
  helpTxt += " • div (v) : divise par v tous les éléments de la matrice \n";
  helpTxt += " • square : met au carré tous les éléments de la matrice \n";
  helpTxt += " • rac : met à la racine carré tous les éléments de la matrice \n";
  helpTxt += " • sum : calcule la somme de tous les éléments de la matrice \n";
  helpTxt += " • sum diag : calcule de la somme de la diagonale principale (trace) \n";
  helpTxt += " • mean : calcule la moyenne de tous les éléments de la matrice \n";
  helpTxt += " • det : calcule le déterminant de la matrice \n";
  helpTxt += " • rank : calcule le rang de la matrice \n";
  helpTxt += " • eigen val : calcule les valeurs propres de la matrice \n";
  helpTxt += " • inv : calcule la matrice inverse \n";
  helpTxt += " • pseudo-inv : calcule la matrice pseudo-inverse \n";
  helpTxt += " • trans : calcule la transposée de la matrice \n";
  helpTxt += " • alea (x,y) : remplit la matrice d'entiers compris entre x et y \n";
  helpTxt += " • reset : mets tous les éléments de la matrice à 0 \n";
  helpTxt += " • disco : change les couleurs de la matrice \n";
  helpTxt += " • undo : retour en arrière (undo) \n";
  helpTxt += " • redo : annulation retour en arrière (redo) \n";
  helpTxt += " • help : ouvre une fenêtre explicative \n";
  helpTxt += " • exit : permet de quitter l'application \n";
  this->fenetre_avertissement("Aide à l'utilisation", helpTxt);
}

void VueG::addResizeListener(Controleur *c) {
  bResize.signal_clicked().connect(sigc::mem_fun(*c, &Controleur::on_button_resize));
}

void VueG::addPrecisionListener(Controleur *c) {
  bPrecision.signal_clicked().connect(sigc::mem_fun(*c, &Controleur::on_button_precision));
}

void VueG::addModifListener(Controleur *c) {
  bModif.signal_clicked().connect(sigc::mem_fun(*c, &Controleur::on_button_modif));
}

void VueG::addResetListener(Controleur *c) {
  bReset.signal_clicked().connect(sigc::mem_fun(*c, &Controleur::on_button_reset));
}

void VueG::addRangListener(Controleur *c) {
  bRang.signal_clicked().connect(sigc::mem_fun(*c, &Controleur::on_button_rang));
}

void VueG::addTransListener(Controleur *c) {
  bTrans.signal_clicked().connect(sigc::mem_fun(*c, &Controleur::on_button_trans));
}

void VueG::addPseudoInvListener(Controleur *c) {
  bPseudoInv.signal_clicked().connect(sigc::mem_fun(*c, &Controleur::on_button_pseudoinv));
}

void VueG::addInvListener(Controleur *c) {
  bInv.signal_clicked().connect(sigc::mem_fun(*c, &Controleur::on_button_inv));
}

void VueG::addAleaListener(Controleur *c) {
  bAlea.signal_clicked().connect(sigc::mem_fun(*c, &Controleur::on_button_alea));
}

void VueG::addDetListener(Controleur *c) {
  bDet.signal_clicked().connect(sigc::mem_fun(*c, &Controleur::on_button_det));
}

void VueG::addAddListener(Controleur *c) {
  bAdd.signal_clicked().connect(sigc::mem_fun(*c, &Controleur::on_button_add));
}

void VueG::addDivListener(Controleur *c) {
  bDiv.signal_clicked().connect(sigc::mem_fun(*c, &Controleur::on_button_div));
}

void VueG::addSquareListener(Controleur *c) {
  bSquare.signal_clicked().connect(sigc::mem_fun(*c, &Controleur::on_button_square));
}

void VueG::addRacListener(Controleur *c) {
  bRac.signal_clicked().connect(sigc::mem_fun(*c, &Controleur::on_button_rac));
}

void VueG::addSetListener(Controleur *c) {
  bSet.signal_clicked().connect(sigc::mem_fun(*c, &Controleur::on_button_set));
}

void VueG::addUndoListener(Controleur *c) {
  bUndo.signal_clicked().connect(sigc::mem_fun(*c, &Controleur::on_button_undo));
}

void VueG::addRedoListener(Controleur *c) {
  bRedo.signal_clicked().connect(sigc::mem_fun(*c, &Controleur::on_button_redo));
}

void VueG::addHelpListener(Controleur *c) {
  bHelp.signal_clicked().connect(sigc::mem_fun(*c, &Controleur::on_button_help));
}

void VueG::addMultListener(Controleur *c) {
  bMult.signal_clicked().connect(sigc::mem_fun(*c, &Controleur::on_button_mult));
}

void VueG::addSumDiagListener(Controleur *c) {
  bSumDiag.signal_clicked().connect(sigc::mem_fun(*c, &Controleur::on_button_sumdiag));
}

void VueG::addSumListener(Controleur *c) {
  bSum.signal_clicked().connect(sigc::mem_fun(*c, &Controleur::on_button_sum));
}

void VueG::addVpListener(Controleur *c) {
  bVp.signal_clicked().connect(sigc::mem_fun(*c, &Controleur::on_button_vp));
}

void VueG::addMoyListener(Controleur *c) {
  bMoy.signal_clicked().connect(sigc::mem_fun(*c, &Controleur::on_button_moy));
}

void VueG::addExitListener(Controleur *c) {
  bExit.signal_clicked().connect(sigc::mem_fun(*c, &Controleur::on_button_exit));
}

void VueG::addDiscoListener(Controleur *c) {
  bDisco.signal_clicked().connect(sigc::mem_fun(*c, &Controleur::on_button_disco));
}

void VueG::change_color_disco(const std::string color1, const std::string color2) {
  for (int i=0; i<TAILLE_MAX; i++) {
    for (int j=0; j<TAILLE_MAX; j++) { 
      if (i%2) {
        if (j%2) {
          this->entMat[i][j].override_background_color(Gdk::RGBA(color1)); 
        }
        else {
          this->entMat[i][j].override_background_color(Gdk::RGBA(color2)); 
        }   
      }
      else {
        if (j%2) {
          this->entMat[i][j].override_background_color(Gdk::RGBA(color2)); 
        }
        else {
          this->entMat[i][j].override_background_color(Gdk::RGBA(color1)); 
        }       
      }
      this->entMat[i][j].override_color(Gdk::RGBA("#222222")); // black
    }
  }
}
