#include <iostream>
#include "VectorTestor.h"
#include "stdlib.h"

void main()
{
    // Vector test
    ZH::TEST::VectorTestor vectorTestor;
    vectorTestor.beginTest( "Vector" );
    vectorTestor.doTest("Vector");
    vectorTestor.endTest( "Vector" );



    // Pause
    system("PAUSE");
}
