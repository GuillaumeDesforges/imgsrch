#include <iostream>
#include <vector>
#include <iomanip>
#include <fstream>
using namespace std;

#include "clustering/point.h"
#include "clustering/kmeans.h"
#include "clustering/kmeanstree.h"

#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>

int main() {
   cout << "Init data" << endl;
    int n = 4;
    int N = 50, D = 2;
    vector<Point<float>> data;
    for(int i = 0; i < N; i++) {
        Point<float> p(D);
        for(int d = 0; d < D; d++) {
            p[d] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        }
        data.push_back(p);
    }

    cout << "* Test KMeans" << endl;
    cout << "Init kmeans" << endl;
    KMeans<Point<float>, float> kmeans(n, D);
    kmeans.addPoints(data);

    cout << "Train kmeans" << endl;
    kmeans.init();
    kmeans.fit();

    cout << endl;
    cout << "Results" << endl;
    vector<Point<float>> means = kmeans.getMeans();
    for(int i = 0; i < n; i++) {
        cout << "cluster " << i << " = " << means[i] << endl;
    }
    cout << endl;
    vector<vector<Point<float>*>> partitions = kmeans.getPartitions();
    for(int i = 0; i < n; i++) {
        for(int k = 0; k < partitions[i].size(); k++) {
            cout << "cluster " << i << " point " << k << " = " << (*partitions[i][k]) << endl;
        }
    }

    cout << endl << endl;

    cout << "Serialize" << endl;
    {
        std::ofstream ofs("kmeans.txt");
        boost::archive::xml_oarchive oa(ofs);
        oa << BOOST_SERIALIZATION_NVP(kmeans);
    }

    cout << "Deserialize" << endl;
    KMeans<Point<float>, float> kmeans1(0, 0);
    {
        std::ifstream ifs("kmeans.txt");
        boost::archive::xml_iarchive ia(ifs);
        ia >> BOOST_SERIALIZATION_NVP(kmeans1);
    }

    cout << endl;
    cout << "Results" << endl;
    vector<Point<float>> means1 = kmeans1.getMeans();
    for(int i = 0; i < n; i++) {
        cout << "cluster " << i << " = " << means1[i] << endl;
    }
    cout << endl;
    
    cout << kmeans1.getCluster(data[0]) << endl;

    // cout << "* Test KMeansTree" << endl;
    // KMeansTree<Point<float>, float> kmeanstree(n, D, 2, "");
    // kmeanstree.addPoints(data);
    // cout << "Init kmeanstree" << endl;
    // kmeanstree.init();
    // cout << "Train kmeanstree" << endl;
    // kmeanstree.fit();
    // cout << endl;
    // cout << kmeanstree;

    return 0;
}
