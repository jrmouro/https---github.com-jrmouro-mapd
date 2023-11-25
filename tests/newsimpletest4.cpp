/* 
 * File:   newsimpletest4.cpp
 * Author: ronaldo
 *
 * Created on 24 de novembro de 2023, 11:46
 */

#include <stdlib.h>
#include <iostream>
#include "_path.h"
#include "_stepPath.h"
#include "_stepSite.h"

/*
 * Simple C++ Test Suite
 */

void testRlistPortion() {
    
    unsigned portion = 2;
        
    _path path;
    
    path.progress(_site(0,1));
    path.progress(_site(0,2));
    path.progress(_site(0,3));
    path.progress(_site(0,4));
    path.progress(_site(0,5));
    path.progress(_site(0,6));
//    path.progress(_site(0,7));
//    path.progress(_site(0,8));
//    path.progress(_site(0,9));
//    path.progress(_site(1,9));
    
    unsigned current = 5;
    
    path.rlistPortion(portion, [&current](const _path& p){
        
        _stepPath stepPath;
        _stepPath::buildFromPath(current, p, stepPath);
        current = stepPath.goalSite().GetStep();
        
        std::cout << stepPath << std::endl;
        
        return false;   
    });
    
    
    
    
    if (true /*check result*/) {
        std::cout << "%TEST_FAILED% time=0 testname=testRlistPortion (newsimpletest4) message=error message sample" << std::endl;
    }
}

int main(int argc, char** argv) {
    std::cout << "%SUITE_STARTING% newsimpletest4" << std::endl;
    std::cout << "%SUITE_STARTED%" << std::endl;

    std::cout << "%TEST_STARTED% testRlistPortion (newsimpletest4)" << std::endl;
    testRlistPortion();
    std::cout << "%TEST_FINISHED% time=0 testRlistPortion (newsimpletest4)" << std::endl;

    std::cout << "%SUITE_FINISHED% time=0" << std::endl;

    return (EXIT_SUCCESS);
}

