#pragma once

#include <iostream>
#include <cxxtest/TestSuite.h>
#include <cstdlib>
#include <ctime>

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
        std::vector<Point<int>> v0(3,p0);
        TS_ASSERT_THROWS_NOTHING(kmeans.addPoints(v0));
        std::vector<Point<int>> v1(3,p1);
        TS_ASSERT_THROWS_ANYTHING(kmeans.addPoints(v1));
        TS_ASSERT_THROWS_NOTHING(kmeans.setPoints(v0));

    }


    void test_init(void){
      KMeans<Point<int>, int> kmeans(3, 8);
      Point <int> p0(8);
      vector<Point<int>> v0(2,p0);
      kmeans.setPoints(v0);
      TS_ASSERT_EQUALS(kmeans.isInit(), false);
      //TS_ASSERT_THROWS_ANYTHING(kmeans.init());
      Point<int> p1(8);
      kmeans.addPoint(p1);
      TS_ASSERT_THROWS_NOTHING(kmeans.init());
      TS_ASSERT_EQUALS(kmeans.isInit(), true);

    }


    void test_getCluster(void){
      bool fit(true);
      KMeans<Point<float>, float> kmeans(3, 8);
      Point<float> p0(8), p1(8), p2(8);
      for (int i=0; i<8; i++){
        p0[i]=0;
        p1[i]=1;
        p2[i]=2;
      }
      std::vector<Point<float>> v {p0,p1,p2};
      kmeans.setPoints(v);
      kmeans.init();
      if (fit)
        kmeans.fit();
      vector<Point<float>> means = kmeans.getMeans();
      Point<float> p_test(p0);
      int best_cluster = kmeans.getCluster(p_test);
      Point<float> point_cluster = means[best_cluster];
      for (int i=0; i<8; i++){
        TS_ASSERT_EQUALS(point_cluster[i], p0[i]);
      }
      p_test[0]=1;
      point_cluster=means[kmeans.getCluster(p_test)];
      for (int i=0; i<8; i++){
        TS_ASSERT_EQUALS(point_cluster[i], p0[i]);
      }
      for (int i=0; i<8; i++){
        p_test[i]=2+i;
      }
      point_cluster=means[kmeans.getCluster(p_test)];
      for (int i=0; i<8; i++){
        TS_ASSERT_EQUALS(point_cluster[i], p2[i]);
      }
    }


    void test_getCluster_random(void){
      bool fit (true);
      KMeans<Point<float>, float> kmeans(6, 8);     //Random KMeans with 6 clusters (with Point<float>)
      std::vector<Point<float>> v;
      Point<float> p(8);
      for (int i=0; i<7; i++){
        for (int j=0; j<8; j++){
          p[j]=static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/100));       //7 points, every coordinate is a random float between 0 and 100
        }
        v.push_back(p);
      }
      kmeans.setPoints(v);
      kmeans.init();
      if (fit)
        kmeans.fit();
      vector<Point<float>> means = kmeans.getMeans();
      Point<float> p_test(8);
      for (int j=0; j<8; j++){
        p[j]=static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/100));
      }
      int best_cluster = kmeans.getCluster(p_test);
      Point<float> point_cluster = means[best_cluster];
      for (int i=0; i<6; i++){
        TS_ASSERT_LESS_THAN_EQUALS((p_test -point_cluster).norm2(), (p_test - means[i]).norm2());
      }
    }


    void test_means(void) {
      KMeans<Point<float>, float> kmeans(2,8);
      Point<float> p0(8), p1(8), p2(8), p3(8);
      for (int i=0; i<8; i++){
        p0[i]=0;
        p1[i]=1;
        p2[i]=10;
        p3[i]=11;
      }
      std::vector<Point<float>> v {p0, p1, p2, p3};
      kmeans.setPoints(v);
      kmeans.init();
      kmeans.fit();
      std::vector<Point<float>> means = kmeans.getMeans();
      std::vector<std::vector<Point<float>*>> partitions = (kmeans.getPartitions());
      TS_ASSERT_EQUALS(kmeans.getCluster(p0), kmeans.getCluster(p1));
      TS_ASSERT_EQUALS(kmeans.getCluster(p2), kmeans.getCluster(p3));
      TS_ASSERT_DIFFERS(kmeans.getCluster(p1), kmeans.getCluster(p2));
      int cluster0 = kmeans.getCluster(p0);
      int cluster2 = kmeans.getCluster(p2);
      Point<float> mean0(8), mean2(8);
      mean0 = (p0+p1)/2;
      mean2 = (p2+p3)/2;
      for (int i=0; i<8; i++){
        TS_ASSERT_EQUALS((means[cluster0])[i], mean0[i]);
        TS_ASSERT_EQUALS((means[cluster2])[i], mean2[i]);
      }
      TS_ASSERT_EQUALS(partitions[cluster0].size(), 2);
      TS_ASSERT_EQUALS(partitions[cluster2].size(), 2);
      //Can be improved by checking the 2 partitions
    }

    // void test_serialize(void) {
    //     KMeans<Point<float>, float> kmeans(2,8);
    //     TS_ASSERT_THROWS_NOTHING(kmeans.serialize());
    //     Point<float> p0(8);
    //     for (int i=0; i<8; i++)
    //         p0[i]=1;
    //     std::vector<Point<float>> v (5,p0);
    //     kmeans.addPoints(v);
    //     kmeans.init();
    //     kmeans.fit();
    //     TS_ASSERT_THROWS_NOTHING(kmeans.serialize());
    // }



private:
// KMeans<Point<int>, int> *km1, *km2;
};
