main () 
{
	Matrix a = readMatrix ( "../samples/matrixA.data" ) ; 
	Matrix b = readMatrix ( "../samples/matrixB.data" ) ;

	Int rows;
	Int cols;
	rows = numRows(a); 
	cols = numCols(a);
	Int i;
	Int j;
	for(i = 0 : rows - 1)
	{
		for(j = 0 : cols - 1)
		{
			Int currA;
			currA = a[i, j];
			Int currB;
			currB = b[i, j];

			a[i, j] = currA * currB;
			print(a[i, j]);
			print(" ");
		}
		print("\n");
	}
}



