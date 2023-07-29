/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/main.cc to edit this template
 */

/* 
 * File:   main.cpp
 * Author: ronaldo
 *
 * Created on 6 de abril de 2023, 10:31
 */


#include "InstanceExperiment.h"
#include "AstarAlgorithm.h"
#include "Render.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    
    auto instance = InstanceExperiment(
        "./Instances/small/kiva-50-500-5.map", 
        "./Instances/small/kiva-0.2.task");
    
    instance.run();
    

    return 0;
    
}

