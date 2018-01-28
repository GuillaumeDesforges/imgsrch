#pragma once

#include <iostream>
#include <cxxtest/TestSuite.h>

#include "point.h"
#include "kmeans.h"

class TestSuiteKMeans : public CxxTest::TestSuite {
public:
    // void setUp() {
    //     km1 = new KMeans<Point<int>, int>(4, 8);
    //     km2 = new KMeans<Point<int>, int>(4, 8);
    // }

    // void tearDown(){
    //     delete km1, km2;
    // }

    void test_addPoint(void) {
        KMeans<Point<int>, int> kmeans(2, 8);
        Point<int> p0(8);
        TS_ASSERT_THROWS_NOTHING(kmeans.addPoint(p0));
        Point<int> p1(7);
        TS_ASSERT_THROWS_ANYTHING(kmeans.addPoint(p1));
    }


private:
    // KMeans<Point<int>, int> *km1, *km2;
};
