/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/testFiles/cppunittestclassfile.h to edit this template
 */

/*
 * File:   newtestclass.h
 * Author: ronaldo
 *
 * Created on 25 de jul de 2023, 16:11:39
 */

#ifndef NEWTESTCLASS_H
#define NEWTESTCLASS_H

#include <cppunit/extensions/HelperMacros.h>

class newtestclass : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(newtestclass);

    CPPUNIT_TEST(testMapdException);
    CPPUNIT_TEST(testWhat);

    CPPUNIT_TEST_SUITE_END();

public:
    newtestclass();
    virtual ~newtestclass();
    void setUp();
    void tearDown();

private:
    void testMapdException();
    void testWhat();

};

#endif /* NEWTESTCLASS_H */

