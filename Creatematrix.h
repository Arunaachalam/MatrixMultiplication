/***********************************************************
Create matrices for the calculation

As the problem is a multiplication of two matrices, matrices A * B = C, for checking result another RESULt matrix are created

TOTAL MATRIX CREATED: 4

*************************************************************/

#ifndef CREATEMATRIX_HH
#define CREATEMATRIX_HH

#include "Matrix.h"

#include <iostream>
#include <string>

class Creatematrix
{
public:

  //Constructor for creating matrix
  Creatematrix (int Arow, int Acol, int Brow, int Bcol, int Crow, int Ccol);

  //Getter Functions
  Matrix & A() {return A_;}
  Matrix & B() {return B_;}
  Matrix & C() {return C_;}
  Matrix & Res() {return Res_;}

  const Matrix & A() const {return A_;}
  const Matrix & B() const {return B_;}
  const Matrix & C() const {return C_;}
  const Matrix & Res() const {return Res_;}

  int Arows() const {return Arows_;}
  int Brows() const {return Brows_;}
  int Crows() const {return Crows_;}
  int Acols() const {return Acols_;}
  int Bcols() const {return Bcols_;}
  int Ccols() const {return Ccols_;}


private:
  //Create matrix for multiplication
  Matrix A_;			//Read matrix 1
  Matrix B_;			//Read matrix 2
  Matrix C_;			//Result of A*B
  Matrix Res_;			//Validation of Result

  int Arows_,Brows_,Crows_;
  int Acols_,Bcols_,Ccols_;

};


//Constructor

Creatematrix::Creatematrix(int Arow,int Acol,int Brow,int Bcol,int Crow,int Ccol)
:A_(Arow,Acol), B_(Brow,Bcol),C_(Crow,Ccol),Res_(Arow,Bcol)
{

  Arows_ = Arow;
  Acols_ = Acol;
  Brows_ = Brow;
  Bcols_ = Bcol;
  Crows_ = Crow;
  Ccols_ = Ccol;

}

#endif
