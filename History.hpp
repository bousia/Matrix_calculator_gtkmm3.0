
/*
 * fichier History.hpp
 *
 * Permet de gérer un historique
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

////////////////////////////////////////////////////////////////////
// Classe d'historique (history)                                  //
////////////////////////////////////////////////////////////////////

/*
 * Classe abstraite History qui gère l'historique
 */

template <typename T>
class History {
public:
  virtual void put(const T &x) =0;
  virtual bool empty() const =0;
  virtual bool before() const =0;
  virtual bool after() const =0;
  virtual bool is_first() const =0;
  virtual bool is_last() const =0;
  virtual void back() =0;
  virtual void forth() =0;
  virtual const T & item() const =0;
  virtual void remove_all_right() =0;
  
  bool on_item() const {
    return !this->empty() && !this->before() && !this->after();
  }
};


////////////////////////////////////////////////////////////////////
// Implémentation de la Classe d'historique - Methode : liste     //
////////////////////////////////////////////////////////////////////

/*
 * classe HistoryList qui utilise vector et iterator pour gérer un historique
 */

template <typename T>
class HistoryList : public History<T> {
protected :
  std::vector<T> h1;
  typename std::vector<T>::iterator cursor;
public :
  HistoryList() {
    this->cursor = this->h1.begin()-1; //rend();
  }
  
  virtual void put(const T &x) override {
    this->h1.push_back(x);
    this->cursor = this->h1.end()-1;
  }
  
  virtual bool empty() const override {
    return this->h1.empty();
  }
  
  virtual bool before() const override {
    return this->cursor < this->h1.begin();
  }
  
  virtual bool after() const override {
    return this->cursor >= this->h1.end();
  }
  
  virtual bool is_first() const override {
    return this->cursor == this->h1.begin();
  }
  
  virtual bool is_last() const override {
    return this->cursor == this->h1.end()-1;
  }
  
  virtual void back() override {
    if (!this->before())
      this->cursor--;
  }

  virtual void forth() override {
    if (!this->after())
      this->cursor++;
  }
  
  virtual const T &item() const override {
    assert(this->on_item());
    return *(this->cursor);
  }

  virtual void remove_all_right() override {
    this->h1.erase(this->cursor+1, this->h1.end());
  }
  
};


