#include <iostream>
#include "VectorTestor.h"
#include "MatrixTestor.h"
#include "stdlib.h"

void main()
{
    // Vector test
    ZH::TEST::VectorTestor vectorTestor;
    vectorTestor.beginTest( "Vector" );
    vectorTestor.doTest("Vector");
    vectorTestor.endTest( "Vector" );

    // Matrix test
    ZH::TEST::MatrixTestor matrixTestor;
    matrixTestor.beginTest("Matrix");
    matrixTestor.doTest("Matrix");
    matrixTestor.endTest("Matrix");

    // Pause
    system("PAUSE");
}
