/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/testFiles/cppunittestclassfile.cpp to edit this template
 */

/*
 * File:   newtestclass1.cpp
 * Author: ronaldo
 *
 * Created on 13 de nov de 2023, 14:13:15
 */

#include "newtestclass1.h"


CPPUNIT_TEST_SUITE_REGISTRATION(newtestclass1);

newtestclass1::newtestclass1() {
}

newtestclass1::~newtestclass1() {
}

void newtestclass1::setUp() {
}

void newtestclass1::tearDown() {
}

void newtestclass1::testMethod() {
    CPPUNIT_ASSERT(true);
}

void newtestclass1::testFailedMethod() {
    CPPUNIT_ASSERT(false);
}

