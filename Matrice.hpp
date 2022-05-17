
/*
 * fichier Matrice.hpp
 *
 * Permet de créer et modifier des matrices
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

////////////////////////////////////////////////////////////////////
// Classe MATRICE                                                 //
////////////////////////////////////////////////////////////////////

class Matrice {
 
protected:

  // taille non modifiable après initialisation
  int M;
  int N;
  int size;
  // tableau d’éléments de type double
  double *mat; 

public:
  
  // l’itérateur
  class iterator {
  public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = std::remove_cv_t<double>; // constant ou volatile donc supp ces qualifieurs
    using difference_type = std::ptrdiff_t;
    using pointer = double*;
    using reference = double&;
    iterator(pointer p) : position(p) { }
    // incrémentation préfixe
    iterator &operator++() { this->position++; return *this; }
    // incrémentation postfixe
    iterator operator++(int) { iterator i=*this; this->position++; return i; }
    // accès
    reference operator*() const { return *(this->position); }
    pointer operator->() const { return this->position; }
    // opérateurs de comparaison
    bool operator==(const iterator& it) const {
      return this->position == it.position;
    }
    bool operator!=(const iterator& it) const {
      return this->position != it.position;
    }
  private:
    // la position courante
    pointer position;
  }; // fin classe iterator

  class const_iterator {
  public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = std::remove_cv_t<double>; // constant ou volatile donc supp ces qualifieurs
    using difference_type = std::ptrdiff_t;
    using pointer = const double*;
    using reference = const double&;
    const_iterator(pointer p) : position(p) { }
    // incrémentation préfixe
    const_iterator &operator++() { this->position++; return *this; }
    // incrémentation postfixe
    const_iterator operator++(int) { const_iterator i=*this; this->position++;
      // pas de &operator++(int) car pointerait sur variable locale i
      return i; }
    // accès
    reference operator*() const { return *(this->position); }
    pointer operator->() const { return this->position; }
    bool operator==(const const_iterator& it) const {
      return this->position == it.position;
    }
    bool operator!=(const const_iterator& it) const {
      return this->position != it.position;
    }
  private:
    // la position courante
    pointer position;
  }; // fin classe const_iterator

  
  // le constructeur
  Matrice(int lig, int col, double val) : M(lig), N(col), size(lig*col), mat(new double[lig*col]) {
    // affecter la valeur v
    for(int i=0 ; i<this->size ; i++)
      this->mat[i]=val;
  }

  // le constructeur de copie
  Matrice(const Matrice &m) : M(m.get_line()), N(m.get_col()), size(m.get_size()) {
    this->mat = new double[m.get_size()];
    // copie les valeurs
    for(int i=0 ; i<this->size ; i++)
      this->mat[i]=m.mat[i];
  }
  
  // le destructeur
  ~Matrice() {
    delete [] this->mat;
  }

  // copie tous les éléments d'une autre matrice de même taille
  void copie(const Matrice &m) {
    assert(this->get_line()==m.get_line() && this->get_col()==m.get_col());
    for(int i=0 ; i<this->size ; i++)
      this->mat[i]=m.mat[i];
  }
  
  // Rôle : renvoie une référence sur l’élément mat[i][j]
  double &get(int i, int j) const {
    int index = this->N*i+j;
    assert(index>=0 and index<this->size);
    return this->mat[index];
  }

  // set l'element 
  void set(int i, int j, double v) {
    int index = this->N*i+j;
    assert(index>=0 and index<this->size);
    mat[index]=v;
  }

  void resize(int x, int y) {
    assert(0<x and 0<y);
    int taille = x*y;
    double *nouv = new double[taille];
    for(int i=0; i<x; i++) {
        for(int j=0; j<y; j++) {
          if (i<this->M && j<this->N) {
            nouv[y*i+j] = this->mat[N*i+j];
          }
          else {
            nouv[y*i+j] = 0;
          }
        }
    }
    delete this->mat;
    this->mat = new double[taille];
    for(int i=0; i<taille; i++)
      this->mat[i]=nouv[i];
    delete nouv;
    this->M = x;
    this->N = y;
    this->size = taille;
  }

  // Rôle : renvoie le nombre de lignes
  const int get_line() const {
    return this->M;
  }

  // Rôle : renvoie le nombre de colonnes
  const int get_col() const {
    return this->N;
  }

  // Rôle : renvoie le nombre total d'éléments
  const int get_size() const {
    return this->size;
  }

  // Rôle : redéfinition de () pour renvoyer une référence sur
  // l’élément mat[i][j]
  double &operator() (int i, int j) const {
    return this->get(i,j);
  }

  // Rôle :
  std::string toString() const {
    std::ostringstream s;
    for (int i=0; i<this->M; i++) {
      for (int j=0; j<this->N; j++)
	s << this->mat[this->N*i+j] << " ";
      s << "\n";
    }
    return s.str();
  }

  friend std::ostream& operator<<(std::ostream &f, const Matrice &m) {
    return f << m.toString();
  }

  // méthodes pour renvoyer les itérateurs de début et de fin
  iterator begin() noexcept { return iterator(mat); }
  iterator end() noexcept { return iterator(mat+size); } //&mat[size]); }
  const_iterator cbegin() noexcept { return const_iterator(mat); }
  const_iterator cend() noexcept { return const_iterator(mat+size); } //&mat[size]); }
};

