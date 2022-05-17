
/*
 * fichier Matrice_Calc.hpp
 * 
 * Realise des operations sur les matrices
 *
 */

////////////////////////////////////////////////////////////////////
// IMPORTATION bibliotheques / headers                            //
////////////////////////////////////////////////////////////////////

#pragma once
#include "Matrice.hpp"
#include <iostream>
#include <cassert>
#include <cstdlib>
#include <iterator>
#include <sstream>
#include <functional>
#include <vector>

////////////////////////////////////////////////////////////////////
// Classe MATRICE_CALC                                            //
////////////////////////////////////////////////////////////////////

class Matrice_Calc{
private:
public:
    Matrice_Calc(){}
    //C = A + B
    //if can do that, return true; if not, return false
    bool add(const Matrice &A, const Matrice &B,Matrice &C);
    //C = A - B
    //if can do that, return true; if not, return false
    bool subtract(const Matrice &A, const Matrice &B,Matrice &C);
    //C = A * B
    //if can do that, return true; if not, return false
    bool multiply(const Matrice &A, const Matrice &B,Matrice &C);

    //calculer det(A)
    //if cant calculer,return 
    double det(const Matrice &A,int n);
    // role: obtenir matrice transposee
    bool transpos(const Matrice &A,Matrice &B);
    //role: obtenir matrice inversee
    bool inverse(const Matrice &A, Matrice &B);
    //role: obtenir matrice pseudo inversee
    void pseudo_inverse(const Matrice &A,Matrice &B);
 
    //role : obtenir range
    int rank(const Matrice &A);
    //for the eigen value
    void eigen_values(const Matrice &A, std::vector<double> &eigen_value);

};
