/*********************************************************

FileReader function to read two matrix inputs

The inputs are provided as arguments while the code is executed

**********************************************************/


#include "Matrix.h"



#include <string>
#include <fstream>
#include <iostream>


class Fileread
{
 public:
    Fileread (std::string name1,std::string name2,std::string name3);
    void readvars();
    

    //Getter functions
    int Arowval() const { return Arowval_;}
    int Acolumnval() const { return Acolumnval_;}
    int Browval() const {return Browval_;}
    int Bcolumnval() const {return Bcolumnval_;}
    int Crowval() const {return Crowval_;}
    int Ccolumnval() const {return Ccolumnval_;}
    std::string file1() const {return file1_;}
    std::string file2() const {return file2_;}
    std::string file3() const {return file3_;}

 private:
    int Arowval_,Browval_,Crowval_;
    int Acolumnval_,Bcolumnval_,Ccolumnval_;
    std::string file1_,file2_,file3_;

};



//Constructor
Fileread::Fileread(std::string name1,std::string name2, std::string name3): file1_(name1),file2_(name2),file3_(name3)
{


}

//Read functions function

void Fileread::readvars()
{
    std::fstream myFile(file1_,std::ios_base::in);
    myFile >> Arowval_;
    myFile >> Acolumnval_;
    myFile.close();


    std::fstream myFile2(file2_,std::ios_base::in);
    myFile2 >> Browval_;
    myFile2 >> Bcolumnval_;
    myFile2.close();

    Crowval_ = Arowval_;
    Ccolumnval_ = Bcolumnval_;

    std::cout << "File Reader function has been executed" << std::endl;		
}
