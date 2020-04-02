/*
Matrix multiplication Options
1 - Naive Matrix Multiplication
2 - Naive Matrix Reverse Loop Multiplication
3 - Two-way loop unrolling (only for AVX capable systems)
4 - Four-way loop unrolling (only for AVX capable systems)
5 - Blocking algorithm for L2 cache level
6 - Blocking algorithm for L1 cache level
7 - Strassen-Winograd algorithm
8 - BLAS implementation

*/


#include "Matrix.h"
#include "Filereader.h"
#include "Creatematrix.h"
#include "Matmult.h"
#include "Timer.h"

#include <iostream>
#include <vector>


#ifdef USE_LIKWID
extern "C" {
#include <likwid.h>
}
#endif



int main(int argc, char** argv)
{
    ProgTimer::Timer overalltime;

    if (argc < 2)
    {
        std::cout << "Error with file input method Check Again" << std:: endl;
        exit(0);
    }
    
    Fileread F1(argv[1],argv[2],argv[3]);
    
    
    F1.readvars();
    
    // Check the possibility of matrix multiplication
    if (F1.Acolumnval() != F1.Browval())
    {
      std::cout << "Matrix dimension Mismatches:  Please check the input file" << std::endl;
      exit(0);
    }
    
    //Choose the Algorithm to execute
    
    std::cout <<  "Which Algorithm would you like to use for multiplying the matrix?\n1) Normal multiplication as in beginners book\n2) Efficient multiplication by reducing catch misses (inverse loop/transpose matrix Algorithm)\n3) Two-Way Loop Unrolling (Only for AVX capable Systems)\n4) Four-way loop unrolling (Only for AVX capable Systems)\n5) Blocked Algorithm for L2 Cache leven\n6) Blocked Algorithm for L1 Cache level\n7) Fastest Algorithm: Strassen\n8) BLAS standard in your computer (The fastest possible)\n Enter your option (1-8): " << std::endl;
    
    int option = 8;
    
    if (std::cin>>option)
        std::cout <<  "\n\n\nYou chose the option: " << option << ". Performing the calculations:\n\n\n" << std::endl;
    else
    {
        std::cout <<  "\n\n\n Invalid Option, The program is terminating...." << std::endl;
        exit(0);
    }
    
    
    Matmult Mult(F1.Arowval(),F1.Acolumnval(),F1.Browval(),F1.Bcolumnval(),F1.Crowval(),F1.Ccolumnval(),F1.file1(),F1.file2(),F1.file3());

    #ifdef USE_LIKWID
    likwid_markerInit();
    likwid_markerStartRegion( "array" );
    #endif

    ProgTimer::Timer timer;
    Mult.Readinputmatrix();
    double time = timer.elapsed();
    Mult.Creturn();
    std::cout << "Time taken to read the given matrix: " << time << std::endl;

    if (option == 1)
    {
        std::cout << "Regular Matrix Multiplication Algorithm is Used" << std::endl;
        ProgTimer:: Timer timer1;
        Mult.naivematrixmultiply();
        double time1 = timer1.elapsed();
        Mult.Creturn();
        std::cout << "Time taken to complete the computation: " << time1 << std::endl;
    }
    else if (option == 2)
    {
        std::cout << "Transpose Matrix Multiplication Algorithm is Used" << std::endl;
        ProgTimer:: Timer timer1;
        Mult.naivematrixmultiplyreverseloop();
        double time1 = timer1.elapsed();
        Mult.Creturn();
        std::cout << "Time taken to complete the computation: " << time1 << std::endl;
    }
    else if (option == 3)
    {
        std::cout << "Two-Way Loop unroll Algorithm is Used" << std::endl;
        ProgTimer:: Timer timer1;
        Mult.loopunrollmatrixmultiply();
        double time1 = timer1.elapsed();
        Mult.Creturn();
        std::cout << "Time taken to complete the computation: " << time1 << std::endl;
    }
    else if (option == 4)
    {
        std::cout << "Four-Way Loop unroll Algorithm is Used" << std::endl;
        ProgTimer:: Timer timer1;
        Mult.fourloopunrollmultiply();
        double time1 = timer1.elapsed();
        Mult.Creturn();
        std::cout << "Time taken to complete the computation: " << time1 << std::endl;
    }
    else if (option == 5)
    {
        std::cout << "Blocking Algorithm for L2 Cache is Used" << std::endl;
        ProgTimer:: Timer timer1;
        Mult.blockingmultiplyL2();
        double time1 = timer1.elapsed();
        Mult.Creturn();
        std::cout << "Time taken to complete the computation: " << time1 << std::endl;
    }
    else if (option == 6)
    {
        std::cout << "BLocking Algorithm for L1 Cache is used" << std::endl;
        ProgTimer:: Timer timer1;
        Mult.blockingmultiplyL1();
        double time1 = timer1.elapsed();
        Mult.Creturn();
        std::cout << "Time taken to complete the computation: " << time1 << std::endl;
    }
    
    else if (option == 7)
    {
        std::cout << "Strassen-Winograd Algorithm is Used" << std::endl;
        ProgTimer:: Timer timer1;
        Mult.strassenmultiply();
        double time1 = timer1.elapsed();
        Mult.Creturn();
        std::cout << "Time taken to complete the computation: " << time1 << std::endl;
    }
    else if (option == 8)
    {
        std::cout << "BLAS Implementation is executed" << std::endl;
        ProgTimer:: Timer timer1;
        Mult.Blas();
        double time1 = timer1.elapsed();
        Mult.Creturn();
        std::cout << "Time taken to complete the computation: " << time1 << std::endl;
    }
    else
    {
        std::cout <<  "\n\n\n Invalid Option, The program is terminating...." << std::endl;
        exit(0);
    }
    
    #ifdef USE_LIKWID
    likwid_markerStopRegion( "array" );
    likwid_markerClose();
    #endif


    Mult.C1().Res().printfile(argv[3]);

    
    //Check the Matrix result for correctness
    
    ProgTimer::Timer timer3;
    bool result = Mult.Checkresult();
    double time3 = timer3.elapsed();
    std::cout << "Time taken to check the result matrix: " << time3 << std::endl;
    if(result)
	std::cout << "Matrix Multiplication is a success" << std::endl;
    else
    std::cout << "Something Went Wrong :(" << std::endl;
    
    //Overall Program Run Time
    double time4 = overalltime.elapsed();
    std::cout << "Time for overall completion of the program: " << time4 << std::endl;


    std::cout << "Hope It Works Good For You" << std::endl;
    return 0;
    
}
