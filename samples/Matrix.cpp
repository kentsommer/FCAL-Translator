#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "Matrix.h"

using namespace std;

int Matrix::numRows (){
	return rows;
}
int Matrix::numCols (){
	return cols;
}

int numRows (Matrix dat){
	return dat.numRows();
}
	
int numCols (Matrix dat){
	return dat.numCols();
}

Matrix::Matrix(int i, int j){
	rows = i;
	cols = j;
	data = (float *) malloc(sizeof(float) * i * j);
}

Matrix::Matrix(const Matrix& m){
	rows = m.rows;
	cols = m.cols;
	data = m.data;
}

float* Matrix::access(const int i, const int j) const{
	return (data + (i * cols) + j);	
}

std::ostream& operator<<(std::ostream &os, Matrix &m){
	int i, j, rows = m.numRows(), cols = m.numCols();
	
	os << rows << " " << cols;
	
	for(i = 0; i < rows; i++){
		os << endl;
		for(j = 0; j < cols; j++){
			os << *(m.access(i, j)) << "  ";
		}
	}
	os << endl;
	return os;
}
	
Matrix Matrix::readMatrix(std::string filename){
	int i, j, rows, cols;
	ifstream matrix_data;

	matrix_data.open(filename.c_str());
	if(matrix_data.fail()){
		cout << "Failed to open matrix file.\n";
		exit(1);
	}
	
	for(i = 0; ((i < 2) && (!matrix_data.eof())); i++){
		switch(i){
			case 0: 
				matrix_data >> rows;
				break;
			case 1:
				matrix_data >> cols;
				break;
			default:
				cout << "ReadMatrix failed.\n";			
		}
		if(matrix_data.eof()){
			break;
		}
	}

	Matrix *m = new Matrix(rows, cols);

	for(i = 0; i < rows; i++){
		for(j = 0; j < cols; j++){
			matrix_data >> *(m->access(i, j));
			if(matrix_data.eof()){
				break;
			}
		}
	}
	
	matrix_data.close();

	return *m;
}
		
