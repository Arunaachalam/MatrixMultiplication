#ifndef MATMULT_HH
#define MATMULT_HH

#include "Matrix.h"
#include "Creatematrix.h"

#include <iostream>
#include <string>
#include <cmath>
#include <array>

extern "C" {
#include <cblas.h>
}

class Matmult
{
public:
  Matmult(int Arow, int Acol, int Brow,int Bcol, int Crow,int Ccol, std::string file1, std::string file2, std::string file3);

  void Readinputmatrix();
  void naivematrixmultiply();
  void naivematrixmultiplyreverseloop();
  void loopunrollmatrixmultiply();
  void fourloopunrollmultiply();
  void blockingmultiplyL1();
  void blockingmultiplyL2();
  void strassenmultiply();
  void Blas();
  void inline strassen(double *C,double *A,double *B,const int size1,const int size2,const int size3,const int size);
  void Creturn();
  bool Checkresult();
  bool checkresultextra(std::string in, std::string out);


  //Getter functions

  Creatematrix & C1() {return C1_;}
  const Creatematrix & C1() const {return C1_;}

  ~Matmult()
  {
      delete[] A_;
      delete[] B_;
      delete[] C_;

  }

  int Arow() const {return Arow_;}
  int Acol() const {return Acol_;}
  int Brow() const {return Brow_;}
  int Bcol() const {return Bcol_;}
  int Crow() const {return Crow_;}
  int Ccol() const {return Ccol_;}
  std::string name1() const {return name1_;}
  std::string name2() const {return name2_;}
  std::string name3() const {return name3_;}
  
private:
  Creatematrix C1_;
  int Arow_,Acol_,Brow_,Bcol_,Crow_,Ccol_;
  std::string name1_,name2_,name3_;
  double *A_,*B_,*C_;

};

/************************* 
Other Functions
***********************/

//Constructor
Matmult::Matmult(int Arows, int Acols, int Brows, int Bcols, int Crows, int Ccols, std::string file1, std::string file2, std::string file3)
  :C1_(Arows, Acols, Brows, Bcols, Crows, Ccols)
{
  Arow_ = Arows;
  Acol_ = Acols;
  Brow_ = Brows;
  Bcol_ = Bcols;
  Crow_ = Crows;
  Ccol_ = Ccols;
  name1_ = file1;
  name2_ = file2;
  name3_ = file3;
  A_ = new double[Arows*Acols+81];
  B_ = new double[Brows*Bcols+81];
  C_ = new double[Arows*Bcols+81];

//  std::cout << name1_ << "\t" << name2_ << std::endl;
}

//Other Functions

void Matmult::Readinputmatrix()
{
  C1().B().Initialise(name2());
  C1().A().Initialise(name1());


  //create new matrices for strassen
  //A matrix
  for(int i=0;i<Arow_;i++)
     for(int j=0;j<Acol_;j++)
         A_[(i*Acol_)+j] = C1().A().operator ()(i,j);

  //B matrix
  for(int i=0;i<Brow_;i++)
      for(int j=0;j<Bcol_;j++)
          B_[(i*Bcol_)+j] = C1().B().operator ()(i,j);


}


/*

Naive Matrix Multiplication
As defined in beginner books of C++: Algorithm for all

*/

void Matmult::naivematrixmultiply()
{
   for (int i=0; i<Arow_; i++)
   {
   for (int j=0; j<Bcol_; j++)
     {
      for (int k=0;k<Brow_;k++)
       {
           C1().Res().operator ()(i,j) += C1().A().operator()(i,k) * C1().B().operator()(k,j);

       }
      }
     }

}


/*

Matrix Multiplication - Inverse Loop
Taking advantage of the memory location of loaded input, catch misses are reduced: Simple memory manipulation for Intermediate

*/


void Matmult::naivematrixmultiplyreverseloop()
{
   for (int i=0; i<Arow_; i++)
   {
    for (int k=0;k<Brow_;k++)
     {
      for (int j=0; j<Bcol_; j++)
       {
           C1().Res().operator ()(i,j) += C1().A().operator()(i,k) * C1().B().operator()(k,j);

       }
      }
     }

}


/*

Loop unrolling gives better effect if you have an AVX computer
NOTE: AVX Needed
2-Waz loop unrolling for vectorization 

*/

void Matmult::loopunrollmatrixmultiply()
{
    int Brownew = 2*(Brow_/2);
    for (int i=0; i<Arow_; i+=2)
    {
     for (int k=0;k<Brownew;k++)
      {
       for (int j=0; j<Bcol_; j++)
        {
            C1().Res().operator ()(i,j) += C1().A().operator()(i,k) * C1().B().operator()(k,j);
            C1().Res().operator () (i+1,j) += C1().A().operator ()(i+1,k+1) * C1().B().operator ()(k+1,j);
        }
      }
    }
    if(Arow_%2 != 0)
    {
        int remvar = Arow_%2;
        int compvar = Arow_/2;
        for (int i=compvar*2;i<remvar;i++)
        {
         for (int j=0;j<Bcol_;j++)
         {
             for (int k=0;k<Brow_;k++)
             {
                 C1().Res().operator ()(i,j) += C1().A().operator()(i,k) * C1().B().operator ()(k,j);

             }
         }
        }
    }
     if(Brow_%2 != 0)
     {
         int remvar = Brow_%2;
         int compvar = Brow_/2;
         for (int i=0;i<Arow_;i++)
         {
          for (int k=compvar*2; k<remvar; k++ )
          {
           for (int j = 0;j<Bcol_; j++)
           {
               C1().Res().operator ()(i,j) += C1().A().operator ()(i,k) * C1().B().operator ()(k,j);
           }
          }
         }
     }
}

/*

Loop unrolling gives better effect if you have an AVX computer
NOTE: AVX needed
4-Way loop unrolling for vectorization 

*/

void Matmult::fourloopunrollmultiply()
{
    int Brownew = 4*(Brow_/4);
    for (int i=0; i<Arow_; i+=4)
    {
     for (int k=0;k<Brownew;k++)
      {
       for (int j=0; j<Bcol_; j++)
        {
            C1().Res().operator ()(i,j) += C1().A().operator()(i,k) * C1().B().operator()(k,j);
            C1().Res().operator ()(i+1,j) += C1().A().operator ()(i+1,k+1) * C1().B().operator ()(k+1,j);
            C1().Res().operator ()(i+2,j) += C1().A().operator ()(i+2,k+2) * C1().B().operator ()(k+2,j);
            C1().Res().operator ()(i+3,j) += C1().A().operator ()(i+3,k+3) * C1().B().operator ()(k+3,j);
        }
      }
    }
    if(Arow_%4 != 0)
    {
        int remvar = Arow_%4;
        int compvar = Arow_/4;
        for (int i=compvar*4;i<remvar;i++)
        {
         for (int j=0;j<Bcol_;j++)
         {
             for (int k=0;k<Brow_;k++)
             {
                 C1().Res().operator ()(i,j) += C1().A().operator()(i,k) * C1().B().operator ()(k,j);

             }
         }
        }
    }
     if(Brow_%4 != 0)
     {
         int remvar = Brow_%4;
         int compvar = Brow_/4;
         for (int i=0;i<Arow_;i++)
         {
          for (int k=compvar*4; k<remvar; k++ )
          {
           for (int j = 0;j<Bcol_; j++)
           {
               C1().Res().operator ()(i,j) += C1().A().operator ()(i,k) * C1().B().operator ()(k,j);
           }
          }
         }
     }
}

/*

Blocking Algorithm - Advanced memory utilization

Using Cache memory to reduce the catch misses L1 level

*/



void Matmult::blockingmultiplyL1()
{
  int block_size_x = 256;
  int block_size_y = 512;
  int block_size_z = 256;
  int block_size_x_ = 0;
  int block_size_y_ = 0;
  int block_size_z_ = 0;

  for (int b_x=0; b_x<Arow_; b_x+=block_size_x)
    {
      block_size_x_ = std::min(block_size_x,Arow_-b_x);
      for(int b_z=0; b_z<Brow_; b_z+=block_size_z)
	{      
	  block_size_z_ = std::min(block_size_z,Brow_-b_z);
	  for(int b_y=0; b_y<Bcol_;b_y+=block_size_y)
	    {
	      block_size_y_ = std::min(block_size_y, Bcol_-b_y);
	      for (int i=b_x; i<b_x+block_size_x_ ; ++i)
		for (int k=b_z; k<b_z+block_size_z_ ; ++k)
		  for(int j=b_y; j<b_y+block_size_y_; ++j)
		    {
		      C1().Res().operator ()(i,j) += C1().A().operator ()(i,k) * C1().B().operator ()(k,j);
		    }
	      block_size_y_ = block_size_y;
	    }
	  block_size_z_ = block_size_z;
	}
      block_size_x_ = block_size_x;
    }
}

/*

Blocking Algorithm - Advanced memory utilization

Using Cache memory to reduce the catch misses L2 level

*/



void Matmult::blockingmultiplyL2()
{
  int block_size_x = 512;
  int block_size_y = 1024;
  int block_size_z = 512;
  int block_size_x_ = 0;
  int block_size_y_ = 0;
  int block_size_z_ = 0;

  for (int b_x=0; b_x<Arow_; b_x+=block_size_x)
    {
      block_size_x_ = std::min(block_size_x,Arow_-b_x);
      for(int b_z=0; b_z<Brow_; b_z+=block_size_z)
	{      
	  block_size_z_ = std::min(block_size_z,Brow_-b_z);
	  for(int b_y=0; b_y<Bcol_;b_y+=block_size_y)
	    {
	      block_size_y_ = std::min(block_size_y, Bcol_-b_y);
	      for (int i=b_x; i<b_x+block_size_x_ ; ++i)
		for (int k=b_z; k<b_z+block_size_z_ ; ++k)
		  for(int j=b_y; j<b_y+block_size_y_; ++j)
		    {
		      C1().Res().operator ()(i,j) += C1().A().operator ()(i,k) * C1().B().operator ()(k,j);
		    }
	      block_size_y_ = block_size_y;
	    }
	  block_size_z_ = block_size_z;
	}
      block_size_x_ = block_size_x;
    }
}


/****************************************************************

Strassen - Winograd Matrix Multiplication Algorithm

One of the fastest implementation of matrix multiplications

***************************************************************/

void Matmult::strassenmultiply()
{
    strassen(C_,A_,B_,Arow_,Brow_,Crow_,Arow_);
}

void inline Matmult::strassen(double *C,double *A,double *B,const int size1,const int size2,const int size3,const int size)
{
    if(size>64)
        {
            const int m = size/2;
            const int n = size/2;
            const int k = size/2;

            double *const R1  = new double[m*n];
            double *const R2  = new double[k*n];
            double *const R3_C = C;
            double *const R4_C = &C[m*size1+n];
            double *const R5_C = &C[m*size1];
            double *const R6_C = &C[n];
            double *const A11  =  A;
            double *const A12  = &A[n];
            double *const A21  = &A[m*size2];
            double *const A22 = &A[m*size2+n];
            double *const B11 =  B;
            double *const B12 = &B[n];
            double *const B21 = &B[m*size3];
            double *const B22 = &B[m*size3+n];

        // R1 = A11 - A21
         for(int i=0;i<m;++i)
             for(int j=0;j<m;++j)
         {
             R1[i*m+j] = A11[i*size2+j] - A21[i*size2+j];
         }
       //R2 = B22 -B12
         for(int i=0;i<m;++i)
             for(int j=0;j<m;++j)
             {
                 R2[i*m+j] = B22[i*size3+j] - B12[i*size3+j];
             }
         //R3_C = R1*R2
         strassen(R3_C,R1,R2,size1,m,m,m);
         //R1 = A21+A22
         for(int i=0;i<m;++i)
             for(int j=0;j<m;++j)
             {
                 R1[i*m+j] = A21[i*size2+j] + A22[i*size2+j];
             }
         //R2 = B12-B11
         for(int i=0;i<m;++i)
             for(int j=0;j<m;++j)
             {
                 R2[i*m+j] = B12[i*size3+j] - B11[i*size3+j];
             }
         //R4_C = R1*R2
         strassen(R4_C,R1,R2,size1,m,m,m);
         //R1 = R1 - A11
         for(int i=0;i<m;++i)
             for(int j=0;j<m;++j)
             {
                 R1[i*m+j] = R1[i*m+j] - A11[i*size2+j];
             }
         //R2 = B22 - R2
         for(int i=0;i<m;++i)
             for(int j=0;j<m;++j)
             {
                 R2[i*m+j] = B22[i*size3+j] - R2[i*m+j];
             }
         //R5_C = R1*R2
         strassen(R5_C,R1,R2,size1,m,m,m);
         //R1 = A12-R1
         for(int i=0;i<m;++i)
                 for(int j=0;j<m;++j)
                 {
                     R1[i*m+j] = A12[i*size2+j] - R1[i*m+j];
                 }
         //R2 = B21 - R2
         for(int i=0;i<m;++i)
                 for(int j=0;j<m;++j)
                 {
                     R2[i*m+j] = B21[i*size3+j] - R2[i*m+j];
                 }
         //R6_C = R1*B22
         strassen(R6_C,R1,B22,size1,m,size3,m);
         //R6_C = R6_C + R4_C
         for(int i=0;i<m;++i)
             for(int j=0;j<m;++j)
             {
                 R6_C[i*size1+j] = R6_C[i*size1+j] + R4_C[i*size1+j];
             }
         //R1 = A11*B11
         strassen(R1,A11,B11,m,size2,size3,m);
         //R5_C = R5_C + R1
         for(int i=0;i<m;++i)
             for(int j=0;j<m;++j)
             {
                 R5_C[i*size1+j] = R5_C[i*size1+j] + R1[i*m+j];
         //	 }
         //R6_C = R5_C + R6_C
         //for(int i=0;i<m;++i)
         //	 for(int j=0;j<m;++j)
        //	 {
                 R6_C[i*size1+j] = R6_C[i*size1+j] + R5_C[i*size1+j];
             }
         //R5_C = R5_C + R3_C
         for(int i=0;i<m;++i)
             for(int j=0;j<m;++j)
             {
                 R5_C[i*size1+j] = R5_C[i*size1+j] + R3_C[i*size1+j];
        // 	 }
         //R4_C = R4_C + R5_C
         //for(int i=0;i<m;++i)
         //	 for(int j=0;j<m;++j)
         //	 {
                 R4_C[i*size1+j] = R4_C[i*size1+j] + R5_C[i*size1+j];
             }
         //R3_C = A12*B21
         strassen(R3_C,A12,B21,size1,size2,size3,m);
         //R3_C = R3_C + R1
         for(int i=0;i<m;++i)
             for(int j=0;j<m;++j)
             {
                 R3_C[i*size1+j] = R3_C[i*size1+j] + R1[i*m+j];
             }
         //R1 = A22*R2
         strassen(R1,A22,R2,m,size2,m,m);
         //R5_C = R5_C + R1
         for(int i=0;i<m; ++i)
             for(int j=0;j<m;++j)
             {
                 R5_C[i*size1+j] = R5_C[i*size1+j] + R1[i*m+j];

             }
         delete[] R1;
         delete[] R2;

        }

        else
        {

               for(int i=0;i<size;++i)
                for(int j=0;j<size; ++j)
                    C[i*size1+j]=0;

          const int block_size_x = 32;
          const int block_size_y = 64;
          const int block_size_z = 8;


          for (int b_x=0; b_x<size; b_x+=block_size_x)
           for(int b_z=0; b_z<size;b_z+=block_size_z)
            for(int b_y=0; b_y<size; b_y+=block_size_y)
             for (int i=b_x; i<b_x+block_size_x ; ++i)
              {
               for(int k=b_z; k<b_z+block_size_z; ++k)
                {
                for (int j=b_y; j<b_y+block_size_y ; ++j)
                  {
                    C[i*size1+j] += A[i*size2+ k] * B[k*size3 +j];
                 }           
              }
            }    
}

}


/********************************************************

BLAS = Basic Linear Algebra Subprograms

Library gives best optimization for your hardware 

********************************************************/

void Matmult::Blas()
{
   cblas_dgemm( CblasRowMajor, CblasNoTrans, CblasNoTrans, Arow(), Bcol(), Acol(), 1.0, A_, Acol(), B_, Bcol(), 0.0, C_, Ccol() );
}


/**************************************************

Return the result of the operation A * B

**************************************************/

void Matmult::Creturn()
{
    for (int i=0;i<Crow_;i++)
      for (int j=0;j<Ccol_;j++)
          C1().Res().operator ()(i,j) = C_[(i*Ccol_)+j];

}

/******************************************

Validation of the result

*******************************************/

bool Matmult::Checkresult()
{

    bool result = true;

    if (C1().Res().rows() >1000 || C1().Res().column()>1000)	
     result=true;
    else
    {
    for (int i=0; i< Arow_; i++)
    {
     for (int j=0; j< Bcol_; j++)
     {
        if(std::abs((C1().Res().operator()(i,j) - C1().C().operator()(i,j))) > 0.01)
        {
        result = false;
        break;
        }
      }
     }
     }

    return result;
}

#endif
