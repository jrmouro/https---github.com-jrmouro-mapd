/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/testFiles/cppunittestclassfile.cpp to edit this template
 */

/*
 * File:   newtestclass.cpp
 * Author: ronaldo
 *
 * Created on 25 de jul de 2023, 16:11:39
 */

#include "newtestclass.h"
#include "MapdException.h"


CPPUNIT_TEST_SUITE_REGISTRATION(newtestclass);

newtestclass::newtestclass() {
}

newtestclass::~newtestclass() {
}

void newtestclass::setUp() {
}

void newtestclass::tearDown() {
}

void newtestclass::testMapdException() {
    const char* msg;
    MapdException mapdException(msg);
    if (true /*check result*/) {
        CPPUNIT_ASSERT(false);
    }
}

void newtestclass::testWhat() {
    MapdException mapdException;
    const char* result = mapdException.what();
    if (true /*check result*/) {
        CPPUNIT_ASSERT(false);
    }
}

