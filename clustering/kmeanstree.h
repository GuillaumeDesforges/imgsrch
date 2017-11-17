#pragma once

#include <vector>
#include <string>
using namespace std;

#include "kmeans.h"
#include "point.h"

template<typename T>
class KMeansTree {
public:
    KMeansTree(int n_clusters, int dimension, int depth, string prefix);

    void init(vector<Point<T>*> &data);
    void init(vector<Point<T>> &data);

    void fit();

    const string getWord(const Point<T> point) const;
private:
    int n_clusters, depth;
    string prefix;
    vector<Point<T>*> data;
    KMeans<Point<T>, T> kmeans;
    vector<KMeansTree<T>> subtrees;
};

template<typename T>
KMeansTree<T>::KMeansTree(int n_clusters, int dimension, int depth, string prefix) :
    n_clusters(n_clusters),
    depth(depth),
    prefix(prefix),
    kmeans(KMeans<Point<T>, T>(n_clusters, dimension))
    {
        if(depth > 0) {
            for(int i = 0; i < n_clusters; i++) {
                char c = (char) (((int) 'a') + i);
                KMeansTree<T> subtree(n_clusters, dimension, depth-1, prefix+c);
                subtrees.push_back(subtree);
            }
        }
    }

template<typename T>
void KMeansTree<T>::init(vector<Point<T>*> &data) {
    this->data = data;
    kmeans.init(data);
}

template<typename T>
void KMeansTree<T>::init(vector<Point<T>> &data) {
    vector<Point<T>*> data_p;
    for(int k = 0; k < data.size(); k++) {
        Point<T>* p = &(data[k]);
        data_p.push_back(p);
    }
    init(data_p);
}

template<typename T>
void KMeansTree<T>::fit() {
    kmeans.fit();
    if(depth > 0) {
        for(int i = 0; i < n_clusters; i++) {
            if(kmeans.getPartitions()[i].size() >= n_clusters) {
                KMeansTree<T>& subtree = subtrees[i];
                subtree.init(kmeans.getPartitions()[i]);
                subtree.fit();
            }
        }
    }

}

template<typename T>
const string KMeansTree<T>::getWord(Point<T> point) const { // TODO
    // should be recursive
    // BUT be careful not to go into subtrees not initiated !
    return;
}
