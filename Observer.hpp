
/*
 * fichier Observer.hpp
 * 
 * Classe abstraite qui permet de faire le lien entre le Modele et la VueG
 *
 */

////////////////////////////////////////////////////////////////////
// IMPORTATION bibliotheques / headers                            //
////////////////////////////////////////////////////////////////////

#pragma once

////////////////////////////////////////////////////////////////////
// Classe abstraite OBSERVER                                      //
////////////////////////////////////////////////////////////////////

template<typename T>
class Observer {
public:

	// affichage résultat
	virtual void updateResult(const T &info, const int &precision) = 0;
	// affichage résultat
	virtual void updateResult(const std::vector<double> &vp, const int &precision) = 0;
  	
  // affichage élément matrice
  virtual void updateMat(const T &info, const int &i, const int &j, const int &precision) = 0;
  // affichage changement de taille
  virtual void updateResize(const int &tabx, const int &taby, const int &i, const int &j) = 0;
  // ouverture fenetre avec titre et message
  virtual void updateWarning(const std::string &titre, const std::string &message) = 0;

};
