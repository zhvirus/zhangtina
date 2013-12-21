#include "stdafx.h"
#include "Config.h"
#include <fstream>
using namespace std;

Config::Config()
{

}


Config::~Config()
{


}


bool Config::writeToFile(char* fileName)
{
    if(fileName==NULL)
        return false;

    std::ofstream ooFile(fileName,std::ios::binary);
    if(ooFile.fail())
        return false;

    //write data to file 
    ooFile.write((char*)(this),sizeof(Config));
   
    ooFile.close();

    //MY_DELETE_ARRAY(normalBuf);

    return true;
}

bool Config::loadFromFile(char* fileName)
{
    if(fileName==NULL)
        return false;

    std::ifstream iiFile(fileName,std::ios::binary);
    if(iiFile.fail())
        return false;

    //write data to file 
    iiFile.read((char*)(this),sizeof(Config));

    iiFile.close();

    //MY_DELETE_ARRAY(normalBuf);

    return true;
}