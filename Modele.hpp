
/*
 *    Modele.hpp
 *
 *    Gestion des données de l'application
 *
 */

////////////////////////////////////////////////////////////////////
// IMPORTATION bibliotheques / headers                            //
////////////////////////////////////////////////////////////////////

#pragma once

#include "Observable.hpp"
#include "MatrixCom.hpp"
#include "parametres.hpp"

////////////////////////////////////////////////////////////////////
// Classe MODELE                                                  //
////////////////////////////////////////////////////////////////////

class Modele : public Observable<double> {
  
private:

  // objet qui réunit matrice / commandes / historique
  MatrixCom *MatCom;

  // nombre de cases déjà attachées à la table dans VueG.hpp
  int table_x;
  int table_y;

  // nb de chiffres affichés après la virgule 
  int precision_virgule;

  // etape changement couleur
  int nbDisco;

public:

  Modele() : MatCom(new MatrixCom(TAILLE_INIT, TAILLE_INIT)), table_x(TAILLE_INIT), table_y(TAILLE_INIT), precision_virgule(PRECISION_INIT), nbDisco(0) {}

  ///////////////////////////////////////
  // methodes acces objets Modele      //
  ///////////////////////////////////////

  int get_line() {
    return this->MatCom->get_line();
  }

  int get_col() {
    return this->MatCom->get_col();
  }

  // renvoie le nb de chiffres après la virgule 
  int getPrecisionModele() {
    return this->precision_virgule;
  }

  // renvoie etape changement couleur
  int getDisco() {
    return this->nbDisco;
  }
  // etape suivante couleur
  void incrDisco() {
    this->nbDisco++;
  }
  // remise à zero couleur
  void rstDisco() {
    this->nbDisco=0;
  }

  ///////////////////////////////////////
  // Fonctions de mise à jour de VueG  //
  ///////////////////////////////////////

  // affiche le modele dans VueG (matrice)
  void majModele() {
    //std::cout << "lines : " << this->MatCom->get_line() << std::endl << "col : " << this->MatCom->get_col() << std::endl;
    for (int i=0; i<this->MatCom->get_line(); i++) {
      for (int j=0; j<this->MatCom->get_col(); j++) {
        notifyObserversMat(this->MatCom->get(i,j), i, j, this->precision_virgule);
        //std::cout << this->MatCom->get(i,j) << " ";
      }
      //std::cout << std::endl;
    }
    //std::cout << std::endl;
  }

  // modifie la taille de la matrice dans VueG
  void majResizeModele() {
    int i = this->MatCom->get_line();
    int j = this->MatCom->get_col();
    notifyObserversResize(this->table_x, this->table_y, i, j);
    this->majModele();
    if (this->table_x < i)
      this->table_x = i;
    if (this->table_y < j)
      this->table_y = j;
  }

  ///////////////////////////////////////
  // Fonctions de commande             //
  ///////////////////////////////////////

  // redimmensioner matrice pour taille (i,j)
  void resizeModele(const int i, const int j) {
    this->MatCom->resize_MatCom(i, j);
    this->majResizeModele();
  }

  // modifie le nombre de chiffres après la virgule (p)
  void precisionModele(const int p) {
    if (this->MatCom->get_abs_max()*pow(10,p) < VALEUR_MAX) {
      this->precision_virgule = p;
      this->majModele();
    }
    else {
      notifyObserversWarning("Attention", "Dépassement de la valeur maximale d'affichage");
    }
  }

  // récupère les valeurs entrées par l'utilisateur
  void modifModele(Matrice *mat) {
    this->MatCom->modif_MatCom(mat);
    this->majModele();
  }

  // met à zero la matrice
  void resetModele() {
    this->MatCom->reset_MatCom();
    this->majModele();
  }
  
  // ajoute v à l'ensemble des éléments de la matrice
  void addModele(const double v) {
    if(this->MatCom->get_max()+v < VALEUR_MAX/pow(10,this->getPrecisionModele()) && this->MatCom->get_min()+v > -VALEUR_MAX/pow(10, this->getPrecisionModele())) {
      this->MatCom->add_MatCom(v);
      this->majModele();
    }
    else {
      notifyObserversWarning("Attention", "Dépassement de la valeur maximale d'affichage");      
    }
  }

  // set v à l'element (i,j) de la matrice
  void setModele(const double v, const int i, const int j) {
    this->MatCom->set_MatCom(v, i, j);
    notifyObserversMat(this->MatCom->get(i,j), i, j, this->precision_virgule);
  }

  // retour en arriere
  void undoModele() {
    this->MatCom->undo_MatCom();
    this->majModele();
    this->majResizeModele();
  }

  // annulation retour en arriere
  void redoModele() {
    this->MatCom->redo_MatCom();
    this->majModele();
    this->majResizeModele();
  }

  // multiplie par v l'ensemble des éléments de la matrice
  void multModele(const double v) {
    if(this->MatCom->get_abs_max()*v < VALEUR_MAX/pow(10,this->getPrecisionModele())) {
      this->MatCom->mult_MatCom(v);
      this->majModele();
    }
    else {
      notifyObserversWarning("Attention", "Dépassement de la valeur maximale d'affichage");      
    }
  }

  // divise par v l'ensemble des éléments de la matrice
  void divModele(const double v) {
    if(this->MatCom->get_abs_max()/v < VALEUR_MAX/pow(10,this->getPrecisionModele())) {
      this->MatCom->div_MatCom(v);
      this->majModele();
    }
    else {
      notifyObserversWarning("Attention", "Dépassement de la valeur maximale d'affichage");      
    }
  }

  // met à la racine carré l'ensemble des éléments de la matrice
  void racModele() {
    if (this->MatCom->get_min() >= 0) {
      if(pow(this->MatCom->get_min(),0.5) < VALEUR_MAX/pow(10,this->getPrecisionModele())) {
        this->MatCom->rac_MatCom();
        this->majModele();
      }
      else {
        notifyObserversWarning("Attention", "Dépassement de la valeur maximale d'affichage");      
      }
    }
    else {
      notifyObserversWarning("Attention", "Impossible de réaliser la racine carré d'un nombre négatif dans la matrice");      
    }
  }

  // met au carré l'ensemble des éléments de la matrice
  void squareModele() {
    if(this->MatCom->get_abs_max()*this->MatCom->get_abs_max() < VALEUR_MAX/pow(10,this->getPrecisionModele())) {
      this->MatCom->square_MatCom();
      this->majModele();
    }
    else {
      notifyObserversWarning("Attention", "Dépassement de la valeur maximale d'affichage");      
    }
  }

  // réalise la transposée de la matrice
  void transModele() {
    this->MatCom->trans_MatCom();
    this->majModele();
    this->majResizeModele();
  }

  // réalise l'inverse de la matrice
  void invModele() {
    if (this->get_line() == this->get_col() && this->MatCom->rang_MatCom() == this->get_line()) {
      this->MatCom->inv_MatCom();
      this->majModele();
    }
    else {
      notifyObserversWarning("Attention", "La matrice n'est pas inversible");      
    }
  }

  // réalise la pseudo-inverse 
  void pseudoinvModele() {
    if (this->MatCom->rang_MatCom() == this->get_col()) {
      this->MatCom->pseudoinv_MatCom();
      this->majModele();
    }
    else {
      notifyObserversWarning("Attention", "La matrice n'est pas pseudo-inversible");            
    }
  }

  // affecte des entiers aleatoires compris entre min et max à la matrice
  void aleaModele(const int min, const int max) {
    this->MatCom->alea_MatCom(min, max);
    this->majModele();
  }

  ///////////////////////////////////////
  // Fonctions d'affichage résultat    //
  ///////////////////////////////////////

  // affiche la somme des elements sur la diagonale principale de la matrice
  void sumdiagModele() {
    notifyObserversResult(this->MatCom->sumdiag_MatCom(), this->precision_virgule);
  }

  // affiche la somme de tous les éléments de la matrice
  void sumModele() {
    notifyObserversResult(this->MatCom->sum_MatCom(), this->precision_virgule);
  }

  // affiche le rang de la matrice
  void rangModele() {
    notifyObserversResult(this->MatCom->rang_MatCom(), 0);
  }

  // affiche le déterminant de la matrice
  void detModele() {
    if(this->get_line()==this->get_col()) {
      notifyObserversResult(this->MatCom->det_MatCom(), this->precision_virgule);
    }
    else {
      notifyObserversWarning("Attention", "Impossible de calculer le déterminant, le nombre de lignes est différent du nombre de colonnes");
    }
  }

  // affiche les valeurs propres de la matrice
  void vpModele() {
    if(this->get_line()==this->get_col()) {
      notifyObserversResult(this->MatCom->vp_MatCom(), this->precision_virgule);
    }
    else {
      notifyObserversWarning("Attention", "Impossible de calculer les valeurs propres, le nombre de lignes est différent du nombre de colonnes");
    }
  }

  // affiche la moyenne des éléments de la matrice
  void moyModele() {
    notifyObserversResult(this->MatCom->moy_MatCom(), this->precision_virgule);
  }
  
};

