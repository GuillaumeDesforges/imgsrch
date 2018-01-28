#pragma once

#include <iostream>
#include <cxxtest/TestSuite.h>

 #include "kmeans.h"

 class TestSuiteKMeans : public CxxTest::TestSuite{
 public:
   void setUp() {
     km1= new kmeans(4, 8);
     km2 = new kmeans (4, 8);
   }
   void tearDown(){
     delete km1, km2;
   }


private:
  kmeans *km1, *km2;
 }
