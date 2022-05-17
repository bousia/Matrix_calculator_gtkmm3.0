
/*
 * fichier Observable.hpp
 *
 * Definit les méthodes abstraites venant de la classe abstraite Observer
 * Permet de faire le lien entre le Modele et la VueG
 *
 */

////////////////////////////////////////////////////////////////////
// IMPORTATION bibliotheques / headers                            //
////////////////////////////////////////////////////////////////////

#pragma once

#include "Observer.hpp"
#include <list>

////////////////////////////////////////////////////////////////////
// Classe OBSERVABLE                                              //
////////////////////////////////////////////////////////////////////

template<typename T>
class Observable {
private:
  std::list<Observer<T> *> list_observers;

public:
  void notifyObserversResult(T info, const int precision) {
    for (auto obs : this->list_observers) obs->updateResult(info, precision);
  }
  void notifyObserversResult(const std::vector<double> vp, const int precision) {
    for (auto obs : this->list_observers) obs->updateResult(vp, precision);
  }

  void notifyObserversMat(T info, const int i, const int j, const int precision) {
    for (auto obs : this->list_observers) obs->updateMat(info, i, j, precision);
  }
  void notifyObserversResize(const int tabx, const int taby, const int i, const int j) {
    for (auto obs : this->list_observers) obs->updateResize(tabx, taby, i, j);
  }
  void notifyObserversWarning(const std::string titre, const std::string message) {
    for (auto obs : this->list_observers) obs->updateWarning(titre, message);
  }

  void addObserver(Observer<T> * observer) {
    this->list_observers.push_back(observer);
  }
};

