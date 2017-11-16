#include <iostream>
#include <vector>
using namespace std;

#include "point.h"
#include "kmeans.h"

int main() {
    cout << "Test clustering" << endl;

    cout << "Init data" << endl;
    int n = 4;
    int N = 10, D = 2;
    vector<Point<float>> data;
    for(int i = 0; i < N; i++) {
        Point<float> p(D);
        for(int d = 0; d < D; d++) {
            p[d] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        }
        data.push_back(p);
    }

    cout << "Init kmeans" << endl;
    KMeans<Point<float>, float> kmeans(n, D);
    kmeans.init(data);

    cout << "Train kmeans" << endl;
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

    return 0;
}
