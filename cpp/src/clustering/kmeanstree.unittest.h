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
    KMeansTree<Point<float>,float> kmeanstree (2,2,2);
    Point<float> p0(2), p1(2), p2(2), p3(2), p4(2);
    (p0[0],p0[1])=(0,0);
    (p1[0],p1[1]) = (10, 10);
    (p2[0],p2[1]) = (10, 10.1);
    (p3[0],p3[1]) = (11, 11);
    (p4[0],p4[1]) = (11,11.1);
    std::vector<Point<float>> points {p0, p1, p2, p3, p4};
    kmeanstree.setPoints(points);
    kmeanstree.init();
    kmeanstree.fit();
    std::vector<string> words;
    for (int k=0; k<5; k++){
          string word = kmeanstree.getWord(points[k]);
          TS_ASSERT_LESS_THAN_EQUALS(word.size(), 3);
          words.push_back(word);
    }
  }

private:


};
