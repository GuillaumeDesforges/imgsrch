#include <iostream>
#include <vector>
using namespace std;

#include "clustering/point.h"
#include "clustering/kmeans.h"
#include "clustering/kmeanstree.h"

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
    string kmeans_out = kmeans.serialize();
    cout << kmeans_out << endl;

    KMeans<Point<float>, float> kmeans1(kmeans_out);

    cout << endl;
    cout << "Results" << endl;
    vector<Point<float>> means1 = kmeans1.getMeans();
    for(int i = 0; i < n; i++) {
        cout << "cluster " << i << " = " << means1[i] << endl;
    }
    cout << endl;
    // vector<vector<Point<float>*>> partitions1 = kmeans1.getPartitions();
    // for(int i = 0; i < n; i++) {
    //     for(int k = 0; k < partitions1[i].size(); k++) {
    //         cout << "cluster " << i << " point " << k << " = " << (*partitions1[i][k]) << endl;
    //     }
    // }

    return 0;
}
