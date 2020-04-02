/***********************************************************
Create a Matrix class to store the input matrix as Vector

Vector class is used for its advantages in dynamic memory allocations
************************************************************/



#ifndef MATRIX_HH
#define MATRIX_HH

#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>

class Matrix
{
public:


    //Constructor
   Matrix( int xSize, int ySize );


   // Destructor
   ~Matrix() {}


   inline double & operator () ( int i ,int j );


   // initialize the whole array with a constant value
   void fill( double value );


   // return total size of the array
   int getSize() const;

   // return xSize for dimension==0 and ySize for dimension==1
   // other dimension values are not allowed
   int getSize(int dimension ) const;


   // Print the whole array ( for debugging purposes )
   void print();

   //Multiply any elements
   void multiply(int x, int y, int multip);

   //Initialise the matrix
   void Initialise(std::string name);
   void Inverseinitialise (std::string name);
   void printfile(std::string name);

    //Getter Function
    int rows() const { return rows_;}
    int column() const { return column_;}
//    std::vector <double> fullarray() const { return array;}

   private:

   std::vector<double> array;
   int rows_;
   int column_;
   int dimension;
   int rowval_,columnval_;


};



// Operator() 2D
inline double& Matrix::operator ()(int i,int j)
{
    return array[(i*(column_))+j];
}


//Consruct 2D array
Matrix::Matrix( int xSize, int ySize )
{
   rows_ = xSize;
   column_ = ySize;
   dimension = 1;
   array.resize(xSize*ySize);
}


/*****************************
Other functions
*****************************/


//Multiply a specific element
void Matrix::multiply(int x, int y, int multip)
{
   array[(x*column_)+y] *= multip;

}

//initialize the whole array with a constant value
void Matrix::fill( double value )
{

   int i = getSize();
   array.assign(i,value);

}


// Print the whole array (for debugging purposes)
void Matrix::print()
{
   {
    for (int i = 0; i < rows_; i++)
    {
     for (int j = 0; j < column_; j++)
     {
        std::cout << array[(i*(column_))+j] << "\t";;
     }
     std::cout << std::endl;
    }
   }

}

//Return dimensions
int Matrix::getSize( int dimensions ) const
{
   if (dimensions == 1)
    return column_;
   else
    return rows_;

}

//return total size of the array
int Matrix::getSize() const
{
        return rows_*column_;
}

//Initialise and read values from file
void Matrix::Initialise(std::string name)
{
    std::fstream fileInput(name,std::ios_base::in);
    fileInput >> rowval_;
    fileInput >> columnval_;
    for (int i=0;i<rows_;i++)
       for(int j=0;j<column_;j++)
            fileInput >> array[(i*(column_))+j];

}

//Initialise in inverse
void Matrix::Inverseinitialise(std::string name)
{
    std::fstream fileInput(name,std::ios_base::in);
    fileInput >> rowval_;
    fileInput >> columnval_;
    for (int i = rowval_-1;i>=0;i--)
        for(int j=column_-1;j>=0;j--)
                fileInput >> array[(i*column_)+j];
}

//Print out file
void Matrix::printfile(std::string name)
{
    std::ofstream fileOutput;
    fileOutput.open(name);
    fileOutput << rows_ << " " << column_ << std::endl;
    for(int i=0;i<rows_;i++)
        for(int j=0; j<column_;j++)
            fileOutput << array[(i*column_)+j] << std::endl;
    fileOutput.close();

}



#endif

