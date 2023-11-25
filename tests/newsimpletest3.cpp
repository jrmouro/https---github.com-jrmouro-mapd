
/* 
 * File:   newsimpletest3.cpp
 * Author: ronaldo
 *
 * Created on 21 de novembro de 2023, 10:22
 */

#include <stdlib.h>
#include <iostream>
#include "_siteBox.h"

/*
 * Simple C++ Test Suite
 */

void testColideWith() {
    
    _site sa(10, 20), sb(5, 15), sc(30, 40), sd(8, 15);
    
    
    _siteBox b1(sa, sb), b2(sc, sd);
    bool result = b1.colideWith(b2);
    if (true /*check result*/) {
        std::cout << "%TEST_FAILED% time=0 testname=testColideWith (newsimpletest3) message=error message sample" << std::endl;
    }
}

int main(int argc, char** argv) {
    std::cout << "%SUITE_STARTING% newsimpletest3" << std::endl;
    std::cout << "%SUITE_STARTED%" << std::endl;

    std::cout << "%TEST_STARTED% testColideWith (newsimpletest3)" << std::endl;
    testColideWith();
    std::cout << "%TEST_FINISHED% time=0 testColideWith (newsimpletest3)" << std::endl;

    std::cout << "%SUITE_FINISHED% time=0" << std::endl;

    return (EXIT_SUCCESS);
}

