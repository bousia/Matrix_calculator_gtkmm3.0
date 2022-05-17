
/* 
 * fichier Command.hpp comportant la classe Command abstraite
 * 
 * ce fichier réunit l'ensemble des commandes effectuées sur la matrice
 * 
 */

////////////////////////////////////////////////////////////////////
// IMPORTATION bibliotheques / headers                            //
////////////////////////////////////////////////////////////////////

#pragma once

#include <iostream>
#include <cassert>
#include <iterator>
#include <sstream>
#include <functional>
#include <vector>

#include "Matrice.hpp"
#include "Matrice_Calc.hpp"

////////////////////////////////////////////////////////////////////
// Classe COMMAND                                                 //
////////////////////////////////////////////////////////////////////

/*
 * Classe abstraite command qui regroupe l'ensemble des commandes
 */

class Command {
protected:
  bool undoable_command;
public :
  virtual void execute() =0;
  virtual void undo() =0;
  virtual void redo() =0;

  bool is_Undoable_Command() {
    return this->undoable_command;
  }
};


////////////////////////////////////////////////////////////////////
// Classes de commandes annulables (undoable)                     //
////////////////////////////////////////////////////////////////////

/*
 * Classe Undoable_command qui regroupe l'ensemble des commandes annulables (undoable)
 */

class Undoable_Command : public Command {
public:
  Undoable_Command() { Command::undoable_command = true;}

  virtual void redo() override {
    this->execute();
  }
};

/*
 * Classe ResizeCmd qui change la taille de la matrice
 * Antécédents : matrice, nouvelle taille x, y
 */

class ResizeCmd : public Undoable_Command {

private :
  Matrice *mat ;
  Matrice *pred_mat ;
  int new_x, new_y ;
  
public :
  ResizeCmd(Matrice *m, int x, int y) : mat(m), new_x(x), new_y(y) { }
  
  virtual void execute() override {
    this->pred_mat = new Matrice(*(this->mat)); // sauvegarde de la matrice actuelle
    this->mat->resize(new_x, new_y);            // modification de la taille
  }
  
  virtual void undo() override {
    this->mat->resize(pred_mat->get_line(), pred_mat->get_col());  // modification de la taille
    this->mat->copie(*(this->pred_mat));                           // copie de l'ancienne matrice
  }
  
};

/*
 * Classe ModifCmd qui copie la matrice entrée par l'utilisateur
 * Antécédents : matrice et matTab qui provient de VueG (matrice entrée par l'utilisateur)
 */

class ModifCmd : public Undoable_Command {
  
private :
  Matrice *mat ;
  Matrice *pred_mat;
  Matrice *matTab;   // matrice entrée dans VueG
public :
  ModifCmd(Matrice *m, Matrice *mt) : mat(m), matTab(mt) { }

  virtual void execute() override {
    this->pred_mat = new Matrice(*(this->mat));  // sauvegarde de la matrice actuelle
    for (int i=0; i<mat->get_line(); i++) {
      for (int j=0; j<mat->get_col(); j++) {
        mat->set(i, j, this->matTab->get(i,j));
      }
    }
  }
  
  virtual void undo() override {
    this->mat->copie(*(this->pred_mat));
  }

};

/*
 * Classe ResetValueCmd qui met à 0 tous les éléments de la matrice
 * Antécédents : matrice
 */

class ResetValueCmd : public Undoable_Command {
  
private :
  Matrice *mat ;
  Matrice *pred_mat;
public :
  ResetValueCmd(Matrice *m) : mat(m) { }

  virtual void execute() override {
    this->pred_mat = new Matrice(*(this->mat));
    for (typename Matrice::iterator it = this->mat->begin(); it != this->mat->end(); it++)
      *it=0;
  }
  
  virtual void undo() override {
    this->mat->copie(*(this->pred_mat));
  }

};


/*
 * Classe SetValue qui modifie un élément de la matrice
 * Antécédents : matrice, valeur, ligne, colonne 
 */

class SetValueCmd : public Undoable_Command {
  
private :
  Matrice *mat ;
  double value;
  double pred_value;
  int line, col;
  
public :
  SetValueCmd(Matrice *m, double v, int l, int c) : mat(m), value(v), line(l), col(c) { }

  virtual void execute() override {
    this->pred_value = (*(this->mat))(this->line,this->col);
    (*(this->mat))(this->line,this->col) = this->value;
  }
  
  virtual void undo() override {
    (*(this->mat))(this->line,this->col) = this->pred_value;
  }

};

/*
 * Classe AddValue qui additionne une valeur à l'ensemble des éléments de la matrice
 * Antécédents : matrice, valeur
 */

class AddValueCmd : public Undoable_Command {

private :
  Matrice *mat ;
  double value;
  
public :
  AddValueCmd(Matrice *m, double v) : mat(m), value(v) { }
  
  virtual void execute() override {
    for (typename Matrice::iterator it = this->mat->begin(); it != this->mat->end(); it++)
      *it+=this->value;
  }
  
  virtual void undo() override {
    for (typename Matrice::iterator it = this->mat->begin(); it != this->mat->end(); it++)
      *it-=this->value;
  }
  
};

/*
 * Classe DivCmd qui divise une valeur à l'ensemble des éléments de la matrice
 * Antécédents : matrice, valeur
 */

class DivCmd : public Undoable_Command {

private :
  Matrice *mat ;
  double value;
  
public :
  DivCmd(Matrice *m, double v) : mat(m), value(v) { }
  
  virtual void execute() override {
    for (typename Matrice::iterator it = this->mat->begin(); it != this->mat->end(); it++)
      *it/=this->value;
  }
  
  virtual void undo() override {
    for (typename Matrice::iterator it = this->mat->begin(); it != this->mat->end(); it++)
      *it*=this->value;
  }
  
};

/*
 * Classe SquareCmd qui met au carré l'ensemble des éléments de la matrice
 * Antécédents : matrice
 */

class SquareCmd : public Undoable_Command {

private :
  Matrice *mat ;
  
public :
  SquareCmd(Matrice *m) : mat(m) { }
  
  virtual void execute() override {
    for (typename Matrice::iterator it = this->mat->begin(); it != this->mat->end(); it++)
      *it *= *it;
  }
  
  virtual void undo() override {
    for (typename Matrice::iterator it = this->mat->begin(); it != this->mat->end(); it++)
      *it = pow(*it,0.5);
  }
  
};

/*
 * Classe RacCmd qui met à la racine carré l'ensemble des éléments de la matrice
 * Antécédents : matrice
 */

class RacCmd : public Undoable_Command {

private :
  Matrice *mat ;
  
public :
  RacCmd(Matrice *m) : mat(m) { }
  
  virtual void execute() override {
    for (typename Matrice::iterator it = this->mat->begin(); it != this->mat->end(); it++)
      *it = pow(*it,0.5);
  }
  
  virtual void undo() override {
    for (typename Matrice::iterator it = this->mat->begin(); it != this->mat->end(); it++)
      *it *= *it;
  }
  
};

/*
 * Classe MultValue qui multiplie une valeur à l'ensemble des éléments de la matrice
 * Antécédents : matrice, valeur
 */

class MultValueCmd : public Undoable_Command {

private :
  Matrice *mat ;
  Matrice *pred_mat;
  double value;
  
public :
  MultValueCmd(Matrice *m, double v) : mat(m), value(v) { }

  virtual void execute() override {
    this->pred_mat = new Matrice(*(this->mat));
    for (typename Matrice::iterator it = this->mat->begin(); it != this->mat->end(); it++)
      *it*=this->value;
  }
  
  virtual void undo() override {
    this->mat->copie(*(this->pred_mat));   // on choisit de reprendre la matrice précédente afin d'éviter des erreurs d'approximmation
  }
  
};

/*
 * Classe AleaCmd qui affecte à la matrice des entiers aléatoires tirés entre x et y 
 * Antécédents : matrice, valeur min x et max y
 */

class AleaCmd : public Undoable_Command {

private :
  Matrice *mat ;
  Matrice *pred_mat ;
  int min, max ;
  
public :
  AleaCmd(Matrice *m, int x, int y) : mat(m), min(x), max(y) { }
  
  virtual void execute() override {
    this->pred_mat = new Matrice(*(this->mat));
    srand((unsigned int)time(0));
    for (typename Matrice::iterator it = this->mat->begin(); it != this->mat->end(); it++)
      *it = min+rand()%(max-min+1);
  }
  
  virtual void undo() override {
    this->mat->copie(*(this->pred_mat));
  }
  
};

/*
 * Classe TransCmd qui calcule la transposée
 * Antécédents : matrice
 */

class TransCmd : public Undoable_Command {
  
private :
  Matrice *mat ;
  Matrice *pred_mat;
public :
  TransCmd(Matrice *m) : mat(m) { }

  virtual void execute() override {
    this->pred_mat = new Matrice(*(this->mat));
    this->mat->resize(this->pred_mat->get_col(),this->pred_mat->get_line());
    Matrice_Calc *Mat_Tool = new Matrice_Calc();
    Mat_Tool->transpos(*(this->pred_mat),*(this->mat));
    delete Mat_Tool;
  }
  
  virtual void undo() override {
    this->mat->resize(pred_mat->get_line(), pred_mat->get_col());
    this->mat->copie(*(this->pred_mat));
  }

};

/*
 * Classe InvCmd qui calcule l'inverse de la matrice
 * Antécédents : matrice
 */

class InvCmd : public Undoable_Command {
  
private :
  Matrice *mat ;
  Matrice *pred_mat;
public :
  InvCmd(Matrice *m) : mat(m) { }
  ~InvCmd(){
    delete pred_mat;
  }
  virtual void execute() override {
    this->pred_mat = new Matrice(*(this->mat));
    Matrice_Calc *Mat_Tool = new Matrice_Calc();
    Mat_Tool->inverse(*(this->pred_mat),*(this->mat));
    delete Mat_Tool;
  }
  
  virtual void undo() override {
    this->mat->copie(*(this->pred_mat));  // on choisit de reprendre la matrice précédente afin d'éviter des erreurs d'approximmation
  }

};

/*
 * Classe PseudoInvCmd qui calcule la pseudo inverse
 * Antécédents : matrice
 */

class PseudoInvCmd : public Undoable_Command {
  
private :
  Matrice *mat ;
  Matrice *pred_mat;
public :
  PseudoInvCmd(Matrice *m) : mat(m) { }
  ~PseudoInvCmd(){
    delete pred_mat;
  }
  virtual void execute() override {
    Matrice_Calc *Mat_Tool = new Matrice_Calc();
    this->pred_mat = new Matrice(*(this->mat));
    this->mat->resize(this->pred_mat->get_col(),this->pred_mat->get_line());
    Mat_Tool->pseudo_inverse(*(this->pred_mat),*(this->mat));
    delete Mat_Tool;
    
  }
  
  virtual void undo() override {
    this->mat->resize(this->pred_mat->get_line(),this->pred_mat->get_col());
    this->mat->copie(*(this->pred_mat));
  }

};

////////////////////////////////////////////////////////////////////
// Classes de commandes non annulables (non undoable)             //
////////////////////////////////////////////////////////////////////

/*
 * Classe NON_Undoable_command qui regroupe l'ensemble des commandes non annulables (non undoable)
 */

class Non_Undoable_Command : public Command {
private:
  virtual void undo() override { assert(false); };
  virtual void redo() override { assert(false); };
public:
  Non_Undoable_Command() { Command::undoable_command = false; }
};


// non utilisé dans notre projet d'inferface graphique

