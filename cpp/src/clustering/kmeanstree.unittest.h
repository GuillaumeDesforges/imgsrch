#pragma once

#include <iostream>
#include <cxxtest/TestSuite.h>

#include "kmeanstree.h"


class TestSuiteKMeansTree : public CxxTest::TestSuite {
public:

  void test_addPointTree(void){
    KMeansTree<Point<int>,int> kmeanstree (2,8,2);
    Point<int> p0(8);
    TS_ASSERT_THROWS_NOTHING(kmeanstree.addPoint(p0));
  }

  void test_initTree(void){
    KMeansTree<Point<float>,float> kmeanstree (2,8,2);
    Point<float> p0(8), p1(8), p2(8), p3(8);
    std::vector<Point<float>> v {p0, p1, p2, p3};
    kmeanstree.setPoints(v);
    TS_ASSERT_THROWS_NOTHING(kmeanstree.init());
    TS_ASSERT_THROWS_NOTHING(kmeanstree.fit());
    TS_ASSERT_EQUALS(kmeanstree.isTrained(), true);
  }

  void test_getWords(void){
    KMeansTree<Point<float>,float> kmeanstree (2,8,2);
    Point<float> p0(8), p1(8), p2(8), p3(8);
    std::vector<Point<float>> v {p0, p1, p2, p3};
    kmeanstree.setPoints(v);
    kmeanstree.init();
    kmeanstree.fit();
    //...
  }

private:


};
