
/*
 * fichier Matrice_Calc.cpp
 *
 * Permet de créer et modifier des matrices (cf Matrice_Calc.hpp)
 *
 */


////////////////////////////////////////////////////////////////////
// IMPORTATION bibliotheques / headers                            //
////////////////////////////////////////////////////////////////////

#include "Matrice.hpp"
#include "Matrice_Calc.hpp"
#include "cmath"

////////////////////////////////////////////////////////////////////
// Operation sur les matrices                                     //
////////////////////////////////////////////////////////////////////

//C = A + B
//if can do that, return true; if not, return false
bool Matrice_Calc::add(const Matrice &A, const Matrice &B,Matrice &C){
    if(A.get_col()!=B.get_col() || A.get_line()!=B.get_line()||
        A.get_col()!=C.get_col()||A.get_line()!=C.get_line()){
            return false;
    }
    for(int i=0;i<C.get_line();i++){
        for(int j=0;j<C.get_col();j++){
            C.set(i,j,A(i,j)+B(i,j));
        }
    }
    return true;
}
//C = A - B
//if can do that, return true; if not, return false
bool Matrice_Calc::subtract(const Matrice &A, const Matrice &B,Matrice &C){
    if(A.get_col()!=B.get_col() || A.get_line()!=B.get_line()||
        A.get_col()!=C.get_col()||A.get_line()!=C.get_line()){
            return false;
    }
    for(int i=0;i<C.get_line();i++){
        for(int j=0;j<C.get_col();j++){
            C.set(i,j,A(i,j)-B(i,j));
        }
    }
    return true;
}
//C = A * B
//if can do that, return true; if not, return false
bool Matrice_Calc::multiply(const Matrice &A, const Matrice &B,Matrice &C){
    if(A.get_line()!=C.get_line()||B.get_col()!=C.get_col()||A.get_col()!=B.get_line()){
        return false;
    }
	
    for(int i=0;i<C.get_line();i++){
        for(int j=0;j<C.get_col();j++){
            double temp = 0;
            for(int k=0;k<A.get_col();k++){
                temp+=A(i,k)*B(k,j);
            }
            C.set(i,j,temp);
        }
    }
    return true;
}


//
//calculer det(A),mais A's colome should == A's line
//if cant calculer,return 
double Matrice_Calc::det(const Matrice &A, int n) {
  	assert(A.get_line() == A.get_col());
  	int p, h, k, i, j;
  	double det_v = 0;
  	Matrice temp{n,n,0};
  	if(n==1) {
    	return A(0,0);
  	} 
  	else {
  		if(n==2) {
    		det_v=(A(0,0)*A(1,1)-A(0,1)*A(1,0));
    		return det_v;
  		} 
  		else {
    		for(p=0;p<n;p++) {
      			h = 0;
      			k = 0;
      			for(i=1;i<n;i++) {
        			for( j=0;j<n;j++) {
          				if(j==p) {
            				continue;
          				}
          				temp(h,k) = A(i,j);
          				k++;
          				if(k==n-1) {
            			h++;
            			k = 0;
          				}
        			}
      			}
      			det_v=det_v+A(0,p)*pow(-1,p)*det(temp,n-1);
    		}
    		return det_v;
  		}
	}
}


//for matrix AT
bool Matrice_Calc::transpos(const Matrice &A,Matrice &B){
    if(A.get_col()!=B.get_line()||A.get_line()!=B.get_col()){
        return false;
    }
    for(int i=0;i<B.get_line();i++){
        for(int j=0;j<B.get_col();j++){
            B.set(i,j,A(j,i));
        }
    }
    return true;
}



//for matrix inverse
bool Matrice_Calc::inverse(const Matrice &A, Matrice &B)
{
	
	
	//judge si il peux calculer
	if(A.get_line()!=A.get_col() || B.get_line()!=B.get_col()||A.get_col()!=B.get_line()){
		
		return false;
	}

	int i = 0;
	int j = 0;
	int k = 0;
	Matrice m(A.get_line(),2*A.get_line(),0);
	float temp = 0;
	float b = 0;
	
	//Matrice augmentée
	for (i = 0;i < m.get_line();i++)
	{
		for (j = 0;j < m.get_col();j++)
		{
			if (j <= A.get_col() - 1)
				m.set(i,j,A(i,j));
			else
			{
				if (i == j - A.get_col())
					m.set(i,j,1);
			}
		}
	}
	
	
    //Élimination gaussienne
    //Transforme le triangle inférieur
	for (k = 0;k < m.get_line()-1;k++)
	{
		// Si la coordonnée est k et le nombre de k est 0, alors la ligne est transformée
		if (m(k,k) == 0)
		{
			for (i = k + 1;i < m.get_line();i++)
			{
				if (m(i,k) != 0)
				{
					break;
				}
			}
			if (i >= m.get_line())
			{
				
				return false;
			}
			else
			{
				//swap linge
				for (j = 0;j < m.get_col();j++)
				{
					temp = m(k,j);
					m.set(k,j,m(k + 1,j));
					m.set(k + 1,j,temp);
				}
			}
		}
		
		
        //Élimination
		for (i = k + 1;i < m.get_line();i++)
		{
			// Obtenez des multiples
			b = m(i,k) / m(k,k);
			// Transformation de ligne
			for (j = 0;j < m.get_col();j++)
			{
				temp = m(i,j) - b * m(k,j);
				m.set(i,j,temp);
			}
		}
	}
	// Transforme le triangle supérieur
	for (k = m.get_line() - 1;k > 0;k--)
	{
		// Si la coordonnée est k et le nombre de k est 0, alors la ligne est transformée
		if (m(k,k) == 0)
		{
			for (i = k + 1;i < m.get_line();i++)
			{
				if (m(i,k) != 0)
				{
					break;
				}
			}
			if (i >= m.get_line())
			{
				return false;
			}
			else
			{
				// Échange de lignes
				for (j = 0;j < m.get_col();j++)
				{
					temp = m(k,j);
					m.set(k,j,m(k + 1,j));
					m.set(k + 1,j,temp);
				}
			}
		}
		
		//Élimination
		for (i = k - 1;i >= 0;i--)
		{
			// Obtenez des multiples
			b = m(i,k) / m(k,k);
			// Transformation de ligne
			for (j = 0;j < m.get_col();j++)
			{
				temp = m(i,j) - b * m(k,j);
				m.set(i,j,temp);
			}
		}
	}
	// Transforme la matrice carrée de gauche en matrice d'identité
	for (i = 0;i < m.get_line();i++)
	{
		if (m(i,i) != 1)
		{
			// Obtenez des multiples
			b = 1 / m(i,i);
			// Transformation de ligne
			for (j = 0;j < m.get_col();j++)
			{
				temp = m(i,j) * b;
				m.set(i,j,temp);
			}
		}
	}
	// Trouver la matrice inverse
	for (i = 0;i < B.get_line();i++)
	{
		for (j = 0;j < B.get_line();j++)
		{
			B.set(i,j,m(i,j + m.get_line()));
		}
	}
	return true;
}




int Matrice_Calc::rank(const Matrice &A)
{
	int rank = A.get_col();
	Matrice B{A};
	for(int row=0;row<rank;row++)
	{
		// Diagonal element is not zero
		if(B(row,row))
		{
			for(int col=0;col<A.get_line();col++)
			{
				if(col!=row)
				{
					double temp = B(col,row) / B(row,row);
                 	for (int i = 0; i < rank; i++)
                   		B.set(col,i,B(col,i) - temp * B(row,i)) ;
				}
			}
		}

		// Diagonal element is already zero. Two cases
        // arise:
        // 1) If there is a row below it with non-zero
        //    entry, then swap this row with that row
        //    and process that row
        // 2) If all elements in current column below
        //    mat[r][row] are 0, then remvoe this column
        //    by swapping it with last column and
        //    reducing number of columns by 1.
		else{
			bool reduce = true;
			for(int i = row+1;i<A.get_line();i++){
				if(B(i,row)){
					// Swap the row with non-zero element
                	// with this row.
					for (int p = 0; p < rank; p++)
					{
						double temp = B(row,p);
						B.set(row,p,B(i,p));
						B.set(i,p,temp);
					}
					reduce = false;
					break;
				}
			}
			if(reduce){
				// Reduce number of columns
				rank--;
				for(int i=0;i<A.get_line();i++)
					B.set(i,row,B(i,rank));
				
			}
			 // Process this row again
			row--;
			std::cout <<B <<std::endl;
		}
		
	}
	return rank;
}

void Matrice_Calc::pseudo_inverse(const Matrice &A,Matrice &B){
     Matrice *A_T = new Matrice(A.get_col(),A.get_line(),0);
     Matrice *A_TA_INV = new Matrice(A.get_col(),A.get_col(),0);
     Matrice *temp = new Matrice(A.get_col(),A.get_col(),0);
     this->transpos(A,*A_T);
     this->multiply(*A_T,A,*temp);
     this->inverse(*temp,*A_TA_INV);
     this->multiply(*A_TA_INV,*A_T,B);
     delete A_T;
	 delete A_TA_INV;
	 delete temp;
}
	
//A should be the squart matrix 
void Matrice_Calc::eigen_values(const Matrice &A, std::vector<double> &eigen_value){
	assert(A.get_line() == A.get_col());
    Matrice *B = new Matrice(A);
    for(int i=0;i<B->get_line();i++){
     	double maxi = i;
     	for(int j = i;j<B->get_col();j++){
     	  if(B->get(maxi,i)<B->get(j,i))
     	  	maxi = j;
     	}
			if(i!=maxi){
				double temp = i;
				i = maxi;
				maxi = temp;
			} 
			if(B->get(i,i)==0)continue;
			for(int k = i+1;k<B->get_line();k++){
				for(int t=i+1;t<B->get_line();t++){
					double value = B->get(k,t) - B->get(i,t)*B->get(k,i)/B->get(i,i);
					B->set(k,t,value);
				}
				B->set(k,i,0);
			}    
     }
     for(int i=0;i<B->get_line();i++){
     	eigen_value.push_back(B->get(i,i));
     }
     delete B;
}