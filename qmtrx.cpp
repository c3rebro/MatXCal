#include <iostream>
#include "qmtrx.h"
using namespace std;

qmatrix::qmatrix(const int dimension): matrix(dimension, dimension)
{
}


Complex qmatrix::determinante(int dim)
{
	qmatrix temp(dim);			// temp - matrix
	int temp_i=0, temp_j=0;		// elements of temp matrix
	int temp_pos=1;					// current element

										// i row of matrix; j column of matrix
	int i, j;							// elements of main matrix (a)

	int s=1;							// sign counter
	int n=0;							// ignored matrix row 
	int m=0;							// ignored matrix column	
	Complex result=0;						// determinante result = result1 + result2
	Complex result1=0;						// result for - elements	(+ - exchange)
	Complex result2=0;						// result for + elements
	
						
	// loop for matrix rank larger than 2
	if(dim>2)  
	{
		// ignore row (n) column (m)
		for(m=0; m<dim; m++)
		{
			temp_i=0;
			temp_j=0;
			temp_pos=1;

			// ckeck elements of main matrix 
			for(i=0; i<dim; i++)
			{
				for(j=0; j<dim; j++)
				{

					if(i==n || j==m);		// if element ignored -> do nothing
					else
					{
				
							// store selected elements to temp matrix
							temp.elem[temp_i][temp_j] = this->elem[i][j]; 

					
							if(temp_pos%(dim-1)==0) {temp_j=0; temp_i++;}	 // next elements
							else temp_j++;
							temp_pos++;
						
					}
				}
			}
			// + - exchange ...	|				...		function call recursiv
			if(s%2==0)	{result1 = result1 - this->elem[n][m] * determinante(dim-1);}	// control +-+-... changes
			else		{result2 = result2 + this->elem[n][m] * determinante(dim-1);}
			s++;	
		}	
		// n matrix
		result=result1+result2;
		return(result);
	}

	///** 2 x 2 matrix
	else
	{
		
		result = (this->elem[0][0]*this->elem[1][1])-(this->elem[0][1]*this->elem[1][0]);		
		return(result);
	}
}