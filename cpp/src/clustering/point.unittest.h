#pragma once

#include <iostream>
#include <cxxtest/TestSuite.h>

#include "point.h"


class TestSuiteKMeans : public CxxTest::TestSuite {
public:
    void setUp() {
        pi_0 = new Point<int>(8), pi_1 = new Point<int>(8);
        pf_0 = new Point<float>(8), pf_1 = new Point<float>(8);
        pd_0 = new Point<double>(8), pd_1 = new Point<double>(8);

        for(int i = 0; i < 8; i++) {
            (*pi_0)[i] = i;
            (*pf_0)[i] = i;                
            (*pd_0)[i] = i;
            (*pi_1)[i] = 2*i;
            (*pf_1)[i] = 2*i;                
            (*pd_1)[i] = 2*i;
        }
    }

    void tearDown() {
        delete pi_0, pi_1;
        delete pf_0, pf_1;
        delete pd_0, pd_1;
    }

    void test_access(void) {
        Point<int> p(8);
        p[0] = 1;
        TS_ASSERT_EQUALS(p[0], 1);
        p[0] = 2;
        TS_ASSERT_DIFFERS(p[0], 1);
    }

    void test_equal_assign(void) {
        Point<int> p0(8);
        Point<int> p1(8);
        for(int i = 0; i < 8; i++) {
            p0[i] = 1;
            p1[i] = 2;
        }
        p0 = p1;
        for(int i = 0; i < 8; i++) {
            TS_ASSERT_EQUALS(p0[i], 2);
            TS_ASSERT_EQUALS(p1[i], 2);
        }
        for(int i = 0; i < 8; i++) {
            p0[i] = 1;
        }
        for(int i = 0; i < 8; i++) {
            TS_ASSERT_EQUALS(p0[i], 1);
            TS_ASSERT_EQUALS(p1[i], 2);
        }
    }

private:
    Point<int> *pi_0, *pi_1;
    Point<float> *pf_0, *pf_1;
    Point<double> *pd_0, *pd_1;
};
