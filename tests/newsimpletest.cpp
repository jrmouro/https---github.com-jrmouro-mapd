/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/testFiles/simpletest.cpp to edit this template
 */

/* 
 * File:   newsimpletest.cpp
 * Author: ronaldo
 *
 * Created on 25 de julho de 2023, 16:13
 */

#include <stdlib.h>
#include <iostream>
#include "../Site.h"
#include "../BinaryPath.h"

/*
 * Simple C++ Test Suite
 */

void test1() {
    std::cout << "newsimpletest test 1" << std::endl;
    BinaryPath p;
    Site s1(0, 0, 0);
    Site s2(1, 0, 1);
    Site s3(2, 0, 2);
    Site s4(3, 2, 2);
    
    p.add(s1);
    p.add(s2);
    p.add(s3);  
    
    
    std::cout << p << std::endl << std::endl;
    
    p.set(2, Site(4, 1, 1));
    
    std::cout << p << std::endl;
    
}

void test2() {
    
    unsigned i = 3;
    unsigned a = 5;
    unsigned z = std::abs((int)i - (int)a);
    
    std::cout << z << std::endl;
    
    
    std::cout << "newsimpletest test 2" << std::endl;
    std::cout << "%TEST_FAILED% time=0 testname=test2 (newsimpletest) message=error message sample" << std::endl;
}

int main(int argc, char** argv) {
    std::cout << "%SUITE_STARTING% newsimpletest" << std::endl;
    std::cout << "%SUITE_STARTED%" << std::endl;

    std::cout << "%TEST_STARTED% test1 (newsimpletest)" << std::endl;
    test1();
    std::cout << "%TEST_FINISHED% time=0 test1 (newsimpletest)" << std::endl;

    std::cout << "%TEST_STARTED% test2 (newsimpletest)\n" << std::endl;
    test2();
    std::cout << "%TEST_FINISHED% time=0 test2 (newsimpletest)" << std::endl;

    std::cout << "%SUITE_FINISHED% time=0" << std::endl;

    return (EXIT_SUCCESS);
}

