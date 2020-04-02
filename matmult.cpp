/*
Matrix multiplication Options
1 - 
2 - 
3 - 
4 -

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
    
    std::cout <<  "Which Algorithm would you like to use for multiplying the matrix?\n 1) Normal multiplication as in beginners book\n2) Efficient multiplication by reducing catch misses (inverse loop/transpose matrix Algorithm)\n3) Two-Way Loop Unrolling (Only for AVX capable Systems)\n4) Four-way loop unrolling (Only for AVX capable Systems) 5) Blocked Algorithm for L2 Cache leven\n6) Blocked Algorithm for L1 Cache level\n7) Fastest Algorithm: Strassen\n 8) BLAS standard in your computer (The fastest possible)\n Enter your option (1-8): " << std::endl;
    
    int option = 8;
    
    if (std::cin>>option)
        std::cout <<  "\n\n\nYou chose the option: " << option << ". Performing the calculations:\n\n\n" << std::endl;
    else
    {
        std::cout <<  "\n\n\n Invalid Option, The program is terminating...." << std::endl;
        exit(0);
    }
    
    
    //Checker = 10; // uncomment for BLAS 
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
    std::cout << "Strassen Algorithm is Implemented" << std::endl;
    ProgTimer:: Timer timer2;
    Mult.strassenmultiply();
    double time2 = timer2.elapsed();
    Mult.Creturn();
    std::cout << "Time taken to do Strassen Algorithm(Winograd Variant) Matrix multiplication: " << time2 << std::endl;
    }
    else if (option == 2)
    {
        std::cout << "Blocking Algorithm is Implemented" << std::endl;
        ProgTimer:: Timer timer3;
        Mult.blockingmultiply();
        double time3 = timer3.elapsed();
        std::cout << "Time taken to do Blocking Altorithm Matrix multiplication: " << time3 << std::endl;
    }
       else if (option == 10) // need to manually set the checker to 10
    {
          ProgTimer::Timer timer7;
          std::cout << "BLAS is implemented" << std::endl;
          Mult.Blas();
          double time3 = timer7.elapsed();
          std::cout << "Time taken to do BLAS Multiplication: " << time3 << std::endl;

	  }
    else
    {
        std::cout << "Naive matrix multiplication is implemented" << std::endl;
        ProgTimer:: Timer timer4;
        Mult.naivematrixmultiplyreverseloop();
        double time4 = timer4.elapsed();
        std::cout << "Time taken to do Naive Matrix multiplication: " << time4 << std::endl;
    }


    #ifdef USE_LIKWID
    likwid_markerStopRegion( "array" );
    likwid_markerClose();
    #endif


    Mult.C1().Res().printfile(argv[3]);


    /*  siwir::Timer timer6;
    bool result = Mult.Checkresult();
    double time5 = timer6.elapsed();
    std::cout << "Time taken to check the result matrix: " << time5 << std::endl;
    if(result)
	std::cout << "Matrix Multiplication is a success" << std::endl;
    else
    std::cout << "Something Went Wrong :(" << std::endl;*/
    double time4 = overalltime.elapsed();
    std::cout << "Time for overall completion of the program: " << time4 << std::endl;


    std::cout << "Hurray I think it is completed" << std::endl;
    return 0;
    
}
