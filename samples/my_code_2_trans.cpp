#include <iostream>
#include "Matrix.h"
#include <math.h>
using namespace std;
int main( ) {
	Matrix a (Matrix::readMatrix("../samples/matrixA.data") );
	Matrix b (Matrix::readMatrix("../samples/matrixB.data") );
	int rows;
	int cols;
	rows = numRows(a);
	cols = numCols(a);
	int i;
	int j;
	for(i = 0 ; i <= (rows - 1); i ++ )
{
	for(j = 0 ; j <= (cols - 1); j ++ )
{
	int currA;
	currA = *(a.access(i, j));
	int currB;
	currB = *(b.access(i, j));
	*(a.access(i, j)) = (currA * currB);
	cout << *(a.access(i, j));
	cout << " ";

}

	cout << "\n";

}


}
