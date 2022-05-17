
/*
 * fichier MatrixCom.hpp
 *
 * Classe qui permet de gérer une matrice, ses commandes et son historique
 *
 */


////////////////////////////////////////////////////////////////////
// IMPORTATION bibliotheques / headers                            //
////////////////////////////////////////////////////////////////////

#include <iostream>
#include <cassert>
#include <cstdlib>

#include <iterator>
#include <sstream>
#include <functional>

#include <typeinfo>
#include <ctime>

#include "Matrice.hpp"
#include "Command.hpp"
#include "History.hpp"
#include "Matrice_Calc.hpp"


////////////////////////////////////////////////////////////////////
// Classe gestion matrice / historique / command                  //
////////////////////////////////////////////////////////////////////

class MatrixCom {
  
protected:
  // matrice
  Matrice *mat ;

  // command  
  Command *cmd;

  // historique
  History<Command *> *history;
  
public:
  
  /////////////////
  // contructeur //
  /////////////////
  
  MatrixCom(int line, int col) : mat(new Matrice(line, col, 0)),
				 cmd(nullptr),
				 history(new HistoryList<Command *>())
  {}

  //////////////////////
  // Méthodes matrice //
  //////////////////////

  // obtenir l'element a la position (i,j)
  double get(int i, int j) const {
    assert(i<=this->mat->get_line() and j<=this->mat->get_col());
    return this->mat->get(i,j);
  }

  // obtenir nombre de lignes
  int get_line() const {
    return this->mat->get_line();
  }
  
  // obtenir nombre de colonnes
  int get_col() const {
    return this->mat->get_col();
  }

  // obtenir nombre d'elements
  int get_size() const {
    return this->mat->get_line() * this->mat->get_col();
  }

  // retourne la valeur absolue la plus grande
  double get_abs_max() const {
    double max = 0;
    for (int i=0; i<this->get_line(); i++) {
      for (int j=0; j<this->get_col(); j++) {
        if (max < abs(this->get(i,j))) {
          max = abs(this->get(i,j));
        }
      }
    }
    return max;
  }

  // retourne la valeur minimum
  double get_min() const {
    double min = 0;
    for (int i=0; i<this->get_line(); i++) {
      for (int j=0; j<this->get_col(); j++) {
        if (min > this->get(i,j)) {
          min = this->get(i,j);
        }
      }
    }
    return min;
  }

  // retourne la valeur maximum
  double get_max() const {
    double max = 0;
    for (int i=0; i<this->get_line(); i++) {
      for (int j=0; j<this->get_col(); j++) {
        if (max < this->get(i,j)) {
          max = this->get(i,j);
        }
      }
    }
    return max;
  }

  //////////////////////////
  // Commandes annulables //
  //////////////////////////

  void modif_MatCom(Matrice *matTab) {
    this->cmd = new ModifCmd(this->mat, matTab);
    hist_cmd_normal();
  }

  void resize_MatCom(const int x, const int y) {
    this->cmd = new ResizeCmd(this->mat, x, y);
    hist_cmd_normal();
  }

  // réinitialiser la matrice
  void reset_MatCom() {
    this->cmd = new ResetValueCmd(this->mat);
    hist_cmd_normal();
  }
  
  // affecter la valeur v a la position (i,j)
  void set_MatCom(double v, int i, int j) {
    this->cmd = new SetValueCmd(this->mat, v, i, j);
    hist_cmd_normal();
  }

  // ajouter la valeur v a tous les elements de la matrice
  void add_MatCom(double v) {
    this->cmd = new AddValueCmd(this->mat, v);
    hist_cmd_normal();
  }

  // divise la valeur v a tous les elements de la matrice
  void div_MatCom(double v) {
    this->cmd = new DivCmd(this->mat, v);
    hist_cmd_normal();
  }

  // mettre au carré tous les elements de la matrice
  void square_MatCom() {
    this->cmd = new SquareCmd(this->mat);
    hist_cmd_normal();
  }

  // mettre à la racine carré tous les elements de la matrice
  void rac_MatCom() {
    this->cmd = new RacCmd(this->mat);
    hist_cmd_normal();
  }

  // retour en arriere
  void undo_MatCom() {
    if(this->history->on_item()){
      this->history->item()->undo();
      this->history->back();
    }
  }

  // annulation retour en arriere
  void redo_MatCom() {
    if(!this->history->is_last()){
      this->history->forth();
      this->history->item()->redo();
    }
  }

  // multiplier la valeur v a tous les elements de la matrice
  void mult_MatCom(double v) {
    this->cmd = new MultValueCmd(this->mat, v);
    hist_cmd_normal();
  }

  // réalise la transposée de la matrice
  void trans_MatCom() {
    this->cmd = new TransCmd(this->mat);
    hist_cmd_normal();
  }

  // réalise l'inverse de la matrice
  void inv_MatCom() {
    // verifier faisabilité  ici
    assert(this->rang_MatCom()==this->get_line() && this->get_col()==this->get_line());
    this->cmd = new InvCmd(this->mat);
    hist_cmd_normal();
  }

  // réalise la pseudo-inverse la matrice
  void pseudoinv_MatCom() {
    assert(this->rang_MatCom()==this->get_col());
    this->cmd = new PseudoInvCmd(this->mat);
    hist_cmd_normal();
  }

  // affecte des valeur aléatoire à chaque élément de la matrice
  void alea_MatCom(const int x, const int y) {
    this->cmd = new AleaCmd(this->mat, x, y);
    hist_cmd_normal();
  }

  ////////////////////////
  // gestion historique //
  ////////////////////////

  // ajouter nouvelle commande
  void hist_cmd_normal() {
    this->cmd->execute();
    if (this->cmd->is_Undoable_Command()) {
	    // y a-t-il eu des undo préalables ?
	    if (!this->history->is_last())
	      this->history->remove_all_right();
	  // la mettre dns l'historique en derniere position
	  this->history->put(cmd);
    }
  }

  //////////////////////////////
  // Commandes non annulables //
  //////////////////////////////

  // renvoie la somme tous les elements de la matrice
  double sum_MatCom() {
    double res = 0;
    for (int i=0; i<this->get_line(); i++) {
      for (int j=0; j<this->get_col(); j++) {
        res += this->mat->get(i,j);
      }
    }
    return res;
  }

  // renvoie la somme tous les elements appartenant à la diagonale de la matrice
  double sumdiag_MatCom() {
    double res = 0;
    for (int i=0; i<this->get_line() && this->get_col(); i++) {
      res += this->mat->get(i,i);
    }
    return res;
  }

  // renvoie l'entier le plus grand entre i et j
  int max(const int i, const int j) {
    if (i<j)
      return j;
    return i;
  }

  // renvoie le rang de la matrice
  int rang_MatCom() {
    Matrice *Mat_cal = new Matrice(*(this->mat));
    int carre = max(this->get_line(), this->get_col());
    Mat_cal->resize(carre, carre);
    Matrice_Calc *Mat_Tool = new Matrice_Calc();
    int rang = Mat_Tool->rank(*(Mat_cal));
    delete Mat_Tool;
    delete Mat_cal;
    return rang;
  }

  // renvoie le determinant de la matrice
  int det_MatCom() {
    Matrice_Calc *Mat_Tool = new Matrice_Calc();
    int det = Mat_Tool->det(*(this->mat), this->get_line());
    delete Mat_Tool;
    return det;
  }

  // renvoie les valeurs propres de la matrice
  std::vector<double> vp_MatCom() {
    std::vector<double> vp;
    Matrice_Calc *Mat_Tool = new Matrice_Calc();
    Mat_Tool->eigen_values(*(this->mat), vp);
    delete Mat_Tool;
    return vp;
  }

  // renvoie la moyenne des elements de la matrice
  double moy_MatCom() {
    double res;
    for (int i=0; i<this->get_line(); i++) {
      for (int j=0; j<this->get_col(); j++) {
        res += this->mat->get(i,j);
      }
    }
    return res/(this->get_line()*this->get_col());
  }

};


